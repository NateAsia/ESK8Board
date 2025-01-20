/**
 * @file button_switch.h
 * @author Nathaniel Asia
 * @brief Button input with debouncing and toggle switch functionality
 * @version 0.1
 * @date 2024-06-27
 * 
 */

#ifndef BUTTON_SWITCH_H_
#define BUTTON_SWITCH_H_

#include <stdint.h>
#include "Arduino.h"

/// Time in milliseconds to wait for button debouncing
#define BTN_DEBOUNCE 40

/**
 * @class ButtonSwitch
 * @brief Implements a debounced button that acts as a toggle switch
 * 
 * Provides functionality to read a physical button input and convert it
 * into a latching toggle switch behavior with proper debouncing.
 * The button state only changes after the debounce period has elapsed.
 */
class ButtonSwitch {
private:
    uint8_t _pin;              ///< Digital pin number for button input
    long    _last_hold_start;  ///< Timestamp of last state change for debouncing
    bool    _disable;          ///< Flag to prevent multiple toggles during single press
    bool    _state;            ///< Current physical button state
    bool    _last_state;       ///< Previous physical button state
    bool    _switchState;      ///< Current logical switch state (toggled output)

public:
    /**
     * @brief Default constructor
     */
    ButtonSwitch() {}

    /**
     * @brief Initialize the button switch
     * @param pin Digital pin number to read (will be set as INPUT_PULLUP)
     */
    void init(uint8_t pin);

    /**
     * @brief Read the current physical button state
     * @return Current button state (true if pressed)
     */
    bool readPin();

    /**
     * @brief Detect button state changes (edge detection)
     * @return True if button state has changed
     */
    bool readEdge();

    /**
     * @brief Get the current logical switch state
     * @return Current switch state (toggled by button presses)
     * 
     * Updates the internal state based on button presses and debouncing.
     * Should be called regularly in the main loop.
     */
    bool getState();

    /**
     * @brief Manually set the logical switch state
     * @param state Desired switch state
     */
    void setState(bool state);
};

#endif // BUTTON_SWITCH_H_
