/**
 * @file joystick.h
 * @author Nathaniel Asia
 * @brief Analog joystick input handling with button support
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 */

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

/**
 * @class Joystick
 * @brief Handles analog joystick input with integrated button
 * 
 * Provides functionality to read and process analog joystick positions
 * on both X and Y axes, with automatic calibration and mapping to
 * percentage values. Also handles the joystick's integrated button.
 */
class Joystick {
    private:
        uint8_t   _btn_pin;            ///< Digital pin for joystick button
        uint8_t   _x_axis_pin;         ///< Analog pin for X-axis
        uint8_t   _y_axis_pin;         ///< Analog pin for Y-axis
        uint16_t  _adc_reading;        ///< Current ADC reading
        uint16_t  _center_adc_reading; ///< Calibrated center position
        uint16_t  _max_adc_reading;    ///< Maximum observed ADC value
        int8_t    percent;             ///< Calculated position percentage
        
    public:
        /**
         * @enum inputAxis
         * @brief Specifies which joystick axis to read
         */
        enum inputAxis{ X, Y };

        /**
         * @struct pins_t
         * @brief Pin configuration for joystick connections
         */
        struct pins_t {
            uint8_t btn_pin;    ///< Digital pin for button input
            uint8_t x_axis_pin; ///< Analog pin for X-axis
            uint8_t y_axis_pin; ///< Analog pin for Y-axis
        };

        /**
         * @brief Default constructor
         */
        Joystick();

        /**
         * @brief Initialize joystick with pin configuration
         * @param pins Structure containing pin assignments
         */
        void    init(Joystick::pins_t pins);

        /**
         * @brief Check if joystick button is currently pressed
         * @return true if button is pressed
         */
        bool    isButtonPressed();

        /**
         * @brief Previous button state for edge detection
         */
        bool    lastButtonState;

        /**
         * @brief Get position percentage for specified axis
         * @param axis Which axis to read (X or Y)
         * @return Position as percentage (-100 to 100)
         */
        float   getAxisPercent(inputAxis axis);

        /**
         * @brief Calibrate maximum ADC reading
         * @param reset If true, resets calibration to default
         */
        void    calibrateMax(bool reset);
};

#endif // JOYSTICK_H_
