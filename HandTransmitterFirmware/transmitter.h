/**
 * @file transmitter.h
 * @author Nathaniel Asia
 * @brief Hand-held remote control transmitter for electric skateboard
 * @version 0.1
 * @date 2024-06-27
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

#include <stdint.h>
#include <RCSwitch.h>
#include <LowPower.h>
#include "Arduino.h"
#include "ESK8Comms.h"
#include "joystick.h"
#include "led.h"

/**
 * @class Transmitter
 * @brief Main controller class for the hand-held remote
 * 
 * Manages all remote control components including:
 * - Joystick for throttle input
 * - Status LED indicator
 * - 433MHz radio transmission
 * - Power management with sleep mode
 * - Interrupt-based wake/sleep control
 */
class Transmitter {
    private:
        Joystick    joystick;           ///< Throttle input control
        LED         led;                ///< Status LED indicator
        RCSwitch    radio;              ///< 433MHz radio transmitter

        uint8_t     _isr_btn_pin;       ///< Pin for wake/sleep button interrupt
        bool        _isr_flag;          ///< Interrupt event flag
        bool        _isr_flag_prev;     ///< Previous interrupt state

        uint8_t     _rf_sleep_pin;      ///< Pin to control radio power
        bool        _transmit_enable;   ///< Transmission active flag

        ESK8Comms::packet_t packet;     ///< Radio communication packet

    public:
        /**
         * @brief Construct a new Transmitter object
         * 
         * Initializes all components to safe default values
         */
        Transmitter();

        /**
         * @brief Initialize the joystick input
         * @param pins Structure containing pin assignments for joystick
         */
        void initJoystick(Joystick::pins_t pins);

        /**
         * @brief Initialize the status LED
         * @param pin Digital pin for LED control
         */
        void initLED(uint8_t pin);

        /**
         * @brief Initialize the radio transmitter
         * @param data_pin Pin for radio data transmission
         * @param rf_sleep_pin Pin to control radio power state
         */
        void initRadio(uint8_t data_pin, uint8_t rf_sleep_pin);

        /**
         * @brief Initialize the interrupt service routine
         * @param pin Digital pin for interrupt input
         * @param userFunc Pointer to ISR callback function
         */
        void initISR(uint8_t pin, void (*userFunc)(void));

        /**
         * @brief Set interrupt flag from ISR
         * 
         * Called by the ISR to signal a button press event
         */
        void isrFlag();

        /**
         * @brief Process interrupt flag and update system state
         * 
         * Handles wake/sleep transitions and updates LED/radio state
         */
        void isrCheck();
        
        /**
         * @brief Enter low-power sleep mode
         * 
         * Disables unnecessary peripherals and enters deep sleep
         */
        void goto_sleep();

        /**
         * @brief Update throttle input from joystick
         * 
         * Reads current joystick position and updates packet data
         */
        void updateInput();

        /**
         * @brief Transmit current control packet
         * 
         * Sends packet over radio if transmission is enabled
         */
        void transmitPacket();

        /**
         * @brief Check if transmission is currently enabled
         * @return true if radio transmission is active
         */
        bool canTransmit();

        /**
         * @brief Main control loop
         * 
         * Updates all transmitter systems:
         * - Interrupt handling
         * - Input processing
         * - Radio transmission
         */
        void run();
};

#endif // TRANSMITTER_H_
