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

void conectaWifi();

void setup() {
    conectaWifi();

}

void loop() {
// write your code here
}

void conectaWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print(F("Aguardando WIFI"));

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}
