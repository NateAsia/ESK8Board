/**
 * @file led.h
 * @author Nathaniel Asia
 * @brief LED control with optional flashing functionality
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef LED_H_
#define LED_H_

#include <stdint.h>
#include "Arduino.h"

/**
 * @class LED
 * @brief Controls an LED with optional flashing capability
 * 
 * Provides functionality to control a digital LED output with the ability
 * to set a steady state or configure automatic flashing at a specified interval.
 */
class LED {
private:
    uint8_t  _pin;              ///< Digital pin number for LED control
    bool     _state;            ///< Current enabled/disabled state
    bool     _led_state;        ///< Current LED on/off state
    long     _last_flash_time;  ///< Timestamp of last flash state change
    uint16_t _flash_period;     ///< Time between flashes in milliseconds (0 for steady state)

public:
    /**
     * @brief Default constructor
     */
    LED() {}

    /**
     * @brief Initialize the LED
     * @param pin Digital pin number to control
     * @param flash_period Optional flash interval in milliseconds (0 for steady state)
     */
    void init(uint8_t pin, uint16_t flash_period = 0);

    /**
     * @brief Set the LED enabled/disabled state
     * @param state True to enable, false to disable
     */
    void set(bool state);

    /**
     * @brief Update LED state based on flash timing
     * @param currTime Current time in milliseconds
     * 
     * Should be called regularly in the main loop to handle flashing behavior
     */
    void run(long currTime);
};

#endif // LED_H_
