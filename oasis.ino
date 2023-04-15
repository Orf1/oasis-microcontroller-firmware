#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

#define DHTTYPE DHT11

uint8_t DHTPin = D1; 
DHT dht(DHTPin, DHTTYPE);
ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);

  pinMode(D2, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.println();

  setupDHT();
  setupWiFi();

}

void loop() {
  Serial.println("");
  sendData(dht.readTemperature(), dht.readHumidity());
  delay(1000);
}

void sendData(float t, float h) {
  String url = "http://172.20.20.238/api/upload?t=" + String(t) + "&h=" + String(h);
  WiFiClient client;
  HTTPClient http;

  Serial.println("Making http request to: " + url);

  bool success = http.begin(client, url);
  int httpCode = http.GET();

  if (!success || httpCode <= 0) {
    Serial.println("Http request failed.");
    return;
  }

  Serial.printf("Http response code: %d\n", httpCode);

  http.end();

  check();
}

void check() {
  String url = "http://172.20.20.238/api/check";
  WiFiClient client;
  HTTPClient http;

  Serial.println("Making http request to: " + url);

  bool success = http.begin(client, url);
  int httpCode = http.GET();

  if (!success || httpCode <= 0) {
    Serial.println("Http request failed.");
    httpCode == 0;
    digitalWrite(D2, LOW);
    return;
  }

  Serial.printf("Http response code: %d\n", httpCode);
    digitalWrite(D2, LOW);
  if (httpCode == 100) {
    digitalWrite(D2, HIGH);
    Serial.println("Watering!!!");
  } else {
    digitalWrite(D2, LOW);
  }
}

void setupWiFi() {
  Serial.println("Attempting to connect to WiFi");

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("GuestAccess", "guestaccess");

  while(!(WiFiMulti.run() == WL_CONNECTED)) {
    Serial.println(".");
    delay(1000);
  }

  Serial.println("Connection sucessfull!");
}

void setupDHT() {
  pinMode(DHTPin, INPUT);
  dht.begin(); 
}