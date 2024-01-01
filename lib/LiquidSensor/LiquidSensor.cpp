//
// Created by smarc on 01/01/2024.
//

#include "LiquidSensor.h"

#include <utility>

void LiquidSensor::begin() {
    pinMode(_pin, INPUT);
    _value = digitalRead(_pin) == HIGH;
}

void LiquidSensor::update() {
    boolean newValue = digitalRead(_pin) == HIGH;
    if (newValue!=_value) {
        _noiseFilterCount--; // faz várias leituras iguais para evitar ruidos.
        if (_noiseFilterCount == 0) {
            _value = newValue;
            if (_change_callback) {
                _change_callback(*this, _value);
            }
        }
    } else {
        _noiseFilterCount = CAIXADAGUAWIFI_LIQUIDSENSOR_NOISE_FILTER;
    }
}

void LiquidSensor::onChange(THandlerFunction_onChange fn) {
    _change_callback = std::move(fn);
}

void LiquidSensor::setId(int id) {
    _id = id;
}

int LiquidSensor::getId() const {
    return _id;
}
