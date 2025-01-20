#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>
#include "Arduino.h"

// ADC and calibration constants
#define MAX_ADC_BASE 600
#define MIN_PERCENT 0
#define MID_PERCENT 50
#define MAX_PERCENT 100

// Mapping constants for joystick axis
#define AXIS_MIN 0
#define AXIS_MAX 100
#define LOWER_HALF_MAX MID_PERCENT
#define UPPER_HALF_MIN MID_PERCENT

class Joystick {
    private:
        uint8_t   _btn_pin;
        uint8_t   _x_axis_pin;
        uint8_t   _y_axis_pin;
        uint16_t  _adc_reading;
        uint16_t  _center_adc_reading;
        uint16_t  _max_adc_reading;
        int8_t    percent;
        
    public:
        enum inputAxis{ X, Y };

        struct pins_t {
            uint8_t btn_pin;
            uint8_t x_axis_pin;
            uint8_t y_axis_pin;
        };

        Joystick();
        void    init(Joystick::pins_t pins);
        bool    isButtonPressed();
        bool    lastButtonState;
        float   getAxisPercent(inputAxis);
        void    calibrateMax(bool);
};

#endif // JOYSTICK_H_
