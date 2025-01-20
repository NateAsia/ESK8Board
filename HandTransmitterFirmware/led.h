/**
 * @file led.h
 * @author Nathaniel Asia
 * @brief Simple LED control interface
 * @version 0.1
 * @date 2024-06-27
 * 
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include "Arduino.h"

/**
 * @class LED
 * @brief Basic LED control functionality
 * 
 * Provides simple interface to control a digital LED output,
 * with the ability to set its on/off state.
 */
class LED {
    private:
        uint8_t _pin;      ///< Digital pin number for LED control
        bool    _state;    ///< Current LED on/off state
    
    public:
        /**
         * @brief Default constructor
         */
        LED();

        /**
         * @brief Initialize the LED
         * @param pin Digital pin number to control
         */
        void init(uint8_t pin);

        /**
         * @brief Set the LED state
         * @param state True for on, false for off
         */
        void set(bool state);
};

#endif // LED_H_
