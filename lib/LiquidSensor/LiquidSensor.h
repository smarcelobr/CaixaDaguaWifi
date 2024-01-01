//
// Created by smarc on 01/01/2024.
//

#ifndef CAIXADAGUAWIFI_LIQUIDSENSOR_H
#define CAIXADAGUAWIFI_LIQUIDSENSOR_H

#include <Arduino.h>

#define CAIXADAGUAWIFI_LIQUIDSENSOR_NOISE_FILTER 15

class LiquidSensor {
public:
    typedef std::function<void(LiquidSensor &, boolean)> THandlerFunction_onChange;

    explicit LiquidSensor(uint8_t pin) : _pin(pin),
                                         _value(false),
                                         _noiseFilterCount(CAIXADAGUAWIFI_LIQUIDSENSOR_NOISE_FILTER),
                                         _id(0) {}

    void onChange(THandlerFunction_onChange fn);

    void begin();

    void update();

    [[nodiscard]] boolean getValue() const { return _value; }

    void setId(int id);

    [[nodiscard]] int getId() const;

private:
    uint8_t _pin;
    boolean _value;
    int _noiseFilterCount;

    THandlerFunction_onChange _change_callback = nullptr;
    int _id;
};


#endif //CAIXADAGUAWIFI_LIQUIDSENSOR_H
