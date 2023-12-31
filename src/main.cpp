/*
 * Usa o sensor de presença d´água sem contato XKC-Y25-NPN para informar o nível de água
 * na caixa d´água via rede usando Wifi. Essa informação pode ser lida por um app de
 * celular ou qualquer outro dispositivo ligado a rede para, por exemplo, ligar ou
 * desligar a bomba d'água.
 *
 * Datasheet sensor: https://arduinoecia.com.br/downloads/Datasheet_Sensor-de-liquidos-sem-contato-XKC-Y25-T12V.pdf
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <WiFiUdp.h>
//#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "jardimdomeier";
const char* password = "sergioeluciene";

WiFiUDP wifiUDP = WiFiUDP();
IPAddress broadcastIP;
unsigned long lastMillis;
unsigned int number;

#define INTERVAL_BETWEEN_PUBLISH 10000
#define SENSOR_100 D2
#define SENSOR_0 D5
#define SENSOR_50 D6
#define SENSOR_10 D7

void conectaWifi();

void setup() {
    pinMode(SENSOR_100, INPUT);
    pinMode(SENSOR_0, INPUT);
    pinMode(SENSOR_50, INPUT);
    pinMode(SENSOR_10, INPUT);
    Serial.begin(115200);

    conectaWifi();

    broadcastIP = WiFi.broadcastIP();

    lastMillis = millis();
    number = 0;
}

void loop() {
    unsigned long now = millis();
    if (now > lastMillis + INTERVAL_BETWEEN_PUBLISH) {
        lastMillis = now;
        wifiUDP.beginPacket(broadcastIP, 19020);
        wifiUDP.print(F("N="));
        wifiUDP.print(number++);
        wifiUDP.print('-');
        wifiUDP.print(digitalRead(SENSOR_100));
        wifiUDP.print(digitalRead(SENSOR_0));
        wifiUDP.print(digitalRead(SENSOR_50));
        wifiUDP.print(digitalRead(SENSOR_10));
        wifiUDP.endPacket();
    }
}

void conectaWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.println();
    Serial.print(F("Aguardando WIFI"));

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print(F("Connected to "));
    Serial.println(ssid);
    Serial.print(F("IP address: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("Broadcast IP address: "));
    Serial.println(WiFi.broadcastIP());
}
