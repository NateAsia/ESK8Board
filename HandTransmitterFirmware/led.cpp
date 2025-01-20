#include "led.h"

LED::LED() {}

void LED::init(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
    set(LOW);
}

void LED::set(bool state) {
    _state = state;
    digitalWrite(_pin, _state);
}