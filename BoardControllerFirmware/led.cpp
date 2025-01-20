#include "led.h"

void LED::init(uint8_t pin, uint16_t flash_period) {
    _pin = pin;
    _flash_period = flash_period;
    _state = false;
    _led_state = false;
    pinMode(_pin, OUTPUT);
}

void LED::set(bool state) {
    _state = state;
}

void LED::run(long currTime) { 
    if(_state) {
        if (_flash_period && currTime - _last_flash_time > _flash_period) {
            _last_flash_time = currTime;
            _led_state = !_led_state;
            digitalWrite(_pin, _led_state);
        }
    } 
    else {
        digitalWrite(_pin, _led_state);
    }
}