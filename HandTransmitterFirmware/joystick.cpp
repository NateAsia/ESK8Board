#include "joystick.h"

Joystick::Joystick() {}

void Joystick::init(Joystick::pins_t pins) {
    _btn_pin = pins.btn_pin;
    _x_axis_pin = pins.x_axis_pin;
    _y_axis_pin = pins.y_axis_pin;
    _center_adc_reading = analogRead(_x_axis_pin);
    _max_adc_reading = MAX_ADC_BASE;
    lastButtonState = false;
}

float Joystick::getAxisPercent(inputAxis axis) {
    _adc_reading = analogRead(axis ? _y_axis_pin : _x_axis_pin);
    this->calibrateMax(false);
    
    if(_adc_reading < _center_adc_reading) {
        percent = map(
            _adc_reading,
            AXIS_MIN, _center_adc_reading,
            MIN_PERCENT, LOWER_HALF_MAX
        );
        percent = constrain(percent, MIN_PERCENT, LOWER_HALF_MAX);
    }
    else {
        percent = map(
            _adc_reading,
            _center_adc_reading, _max_adc_reading,
            UPPER_HALF_MIN, MAX_PERCENT
        );
        percent = constrain(percent, UPPER_HALF_MIN, MAX_PERCENT);
    }
    return constrain(percent, MIN_PERCENT, MAX_PERCENT); 
}

bool Joystick::isButtonPressed() {
    return !digitalRead(_btn_pin);
}

void Joystick::calibrateMax(bool reset) {
    if (reset) {
        _max_adc_reading = MAX_ADC_BASE; 
    }
    _max_adc_reading = max(_max_adc_reading, _adc_reading);
}
