//
// Created by smarc on 01/01/2024.
//

#ifndef CAIXADAGUAWIFI_OTA_H
#define CAIXADAGUAWIFI_OTA_H

#include <ArduinoOTA.h>

class OTA {
private:
    boolean _Updating = false;

    void onStart();
    void onEnd();
    void onProgress(unsigned int progress, unsigned int total);
    void onError(ota_error_t error);
public:
    void begin();
    void handle();
};


#endif //CAIXADAGUAWIFI_OTA_H
