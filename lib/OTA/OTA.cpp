//
// Created by smarc on 01/01/2024.
//

#include "OTA.h"

void OTA::begin() {

    // OTA SETUP
    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);

    ArduinoOTA.setHostname("caixadagua");

    ArduinoOTA.setPasswordHash("26a4b3151ec19724b0c29513bce9aa50");

    ArduinoOTA.onStart([this](){onStart();});
    ArduinoOTA.onEnd([this]() {onEnd();});
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
        onProgress(progress, total);
    });
    ArduinoOTA.onError([this](ota_error_t error) {onError(error);});
    ArduinoOTA.begin();
}

void OTA::handle() {
    ArduinoOTA.handle();
}

void OTA::onStart() {
    _Updating = true;
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
    } else {  // U_FS
        type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
}

void OTA::onEnd() {
    Serial.println("\nEnd");
    _Updating = false;
}

void OTA::onProgress(unsigned int progress, unsigned int total) {
    unsigned int perc = progress / (total / 100);
    if (perc%10==0) {
        Serial.printf("Progress: %u%%\r", perc);
    }

}

void OTA::onError(ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
    }
}