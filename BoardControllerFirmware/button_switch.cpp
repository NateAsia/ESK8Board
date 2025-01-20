#include "button_switch.h"

void ButtonSwitch::init(uint8_t pin) {
    _pin = pin;
    _state = false;
    pinMode(_pin, INPUT_PULLUP);
}

bool ButtonSwitch::readPin() {
    return _state = !digitalRead(_pin);
}

bool ButtonSwitch::getState() {
    if (_last_state != readPin()) {
        _last_hold_start = millis();
        _last_state = _state;

        if (!_state) {
            _disable = false;
        }
    }

    if (_state && !_disable && (millis() - _last_hold_start > BTN_DEBOUNCE)) {
        _switchState = !_switchState;
        _disable = true;
    }
    return _switchState;
}

void ButtonSwitch::setState(bool state) {
    _switchState = state;
}