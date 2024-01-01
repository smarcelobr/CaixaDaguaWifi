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
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include "OTA.h"
#include "LiquidSensor.h"
#include "LittleFS.h"

const char *ssid = "jardimdomeier";
const char *password = "sergioeluciene";

OTA ota = OTA();

#define SENSOR_100 D2
#define SENSOR_0 D5
#define SENSOR_50 D6
#define SENSOR_10 D7
#define NUM_SENSORES 4
LiquidSensor sensor[NUM_SENSORES] = {LiquidSensor(SENSOR_100),
                                     LiquidSensor(SENSOR_50),
                                     LiquidSensor(SENSOR_10),
                                     LiquidSensor(SENSOR_0)
};

WiFiUDP wifiUDP = WiFiUDP();
IPAddress broadcastIP;
unsigned long lastMillis;
unsigned int number;
boolean valueChanged = true;

#define INTERVAL_BETWEEN_PUBLISH 60000

void conectaWifi();

void writeSensors(unsigned long now);

void sensorOnChange(LiquidSensor &liquidSensor, boolean newValue) {
    Serial.printf("S[%d]=%d\r\n", liquidSensor.getId(), newValue ? 1 : 0);
    valueChanged = true;
}

void setup() {

    for (int s = 0; s < NUM_SENSORES; s++) {
        sensor[s].begin();
        sensor[s].onChange(sensorOnChange);
        sensor[s].setId(s);
    }
    Serial.begin(115200);

    conectaWifi();

    ota.begin();

    broadcastIP = WiFi.broadcastIP();


    //Start LittleFS
    if(!LittleFS.begin()){
        Serial.println("An Error has occurred while mounting LittleFS");
        delay(1000);
    }

    lastMillis = millis();
    number = 0;
}

void loop() {
    ota.handle();

    for (LiquidSensor &s: sensor) {
        s.update();
    }

    unsigned long now = millis();
    if (valueChanged || (now > lastMillis + INTERVAL_BETWEEN_PUBLISH)) {
        writeSensors(now);
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

void writeSensors(unsigned long now) {
    lastMillis = now;
    valueChanged = false;
    wifiUDP.beginPacket(broadcastIP, 19020);
    wifiUDP.print(F("N="));
    wifiUDP.print(number++);
    wifiUDP.print('-');
    for (const LiquidSensor &s: sensor) {
        wifiUDP.write(s.getValue() ? '1' : '0');
    }
    wifiUDP.endPacket();
    Serial.println(F("UDP"));

    if (number >= 9999) {
        number = 0;
    }
}
