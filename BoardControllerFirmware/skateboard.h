/**
 * @file skateboard.h
 * @author Nathaniel Asia
 * @brief Header file containing all skateboard electromechanical object abstractions
 * @version 0.1
 * @date 2024-06-23
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef SKATEBOARD_H_
#define SKATEBOARD_H_

#include "Arduino.h"
#include <TM1637Display.h>
#include <Servo.h>
#include <RCSwitch.h>
#include "ESK8Comms.h"
#include "led.h"
#include "button_switch.h"
#include "voltmeter.h"

/// Time between display updates in milliseconds
#define DISPLAY_UPDATE_INTERVAL 500
/// Maximum time without radio communication before safety cutoff
#define RADIO_DISCONNECT_TIME   100
/// Default throttle position when motor is disabled (50%)
#define MOTOR_OFF_THROTTLE      50

/**
 * @class Skateboard
 * @brief Main controller class for the electric skateboard
 * 
 * Manages all skateboard components including:
 * - ESC (Electronic Speed Controller)
 * - Status LED
 * - Power switch
 * - Radio communication
 * - Display
 * - Battery monitoring
 */
class Skateboard {
private:
    Servo           *esc;           ///< Electronic Speed Controller servo interface
    LED             statusLight;    ///< Status LED indicator
    ButtonSwitch    statusSwitch;   ///< Power switch
    RCSwitch        radio;          ///< 433MHz radio receiver
    TM1637Display   *display;       ///< 7-segment display
    VoltMeter       battery;        ///< Battery voltage monitor
    
    char        buffer[200];             ///< Buffer for status message formatting
    uint8_t     _soc;                    ///< Battery State of Charge (0-100%)
    bool        _pwm_enable;             ///< Motor enable flag
    uint16_t    _rf_value;               ///< Raw radio packet value
    uint8_t     _inputThrottlePercent;   ///< Throttle input (0-100%)
    uint8_t     _outputThrottlePercent;  ///< Actual throttle output (0-100%)
    uint8_t     _throttle_servo_position;///< ESC servo position (0-180°)
    long        _last_message_time;      ///< Timestamp of last valid radio packet
    long        _last_display_update;    ///< Timestamp of last display refresh

    ESK8Comms::packet_t packet;         ///< Radio communication packet

    /**
     * @brief Check power switch status and update LED accordingly
     */
    void checkActiveStatus(); 

    /**
     * @brief Process incoming radio packets and update throttle input
     */
    void updateThrottleInput();

    /**
     * @brief Update battery status and display
     */
    void updateBatteryStatus(); 

    /**
     * @brief Calculate and apply throttle output to ESC
     * 
     * Includes safety checks for:
     * - Power switch state
     * - Radio connection timeout
     */
    void updateThrottleOutput();

    /**
     * @brief Print debug status information over serial
     */
    void printStatus();

public:
    /**
     * @brief Construct a new Skateboard object
     * 
     * Initializes all components to safe default values
     */
    Skateboard();

    /**
     * @brief Initialize the Electronic Speed Controller
     * @param pin PWM pin for ESC servo control
     */
    void initESC(uint8_t pin);

    /**
     * @brief Initialize the status LED
     * @param pin Digital pin for LED control
     * @param flash_period Optional flash interval in milliseconds (0 for solid)
     */
    void initStatusLight(uint8_t pin, uint16_t flash_period = 0);

    /**
     * @brief Initialize the power switch
     * @param pin Digital pin for switch input (uses internal pullup)
     */
    void initStatusSwitch(uint8_t pin);

    /**
     * @brief Initialize the radio receiver
     */
    void initRadio();

    /**
     * @brief Initialize the 7-segment display
     * @param clk_pin Clock pin for display
     * @param dio_pin Data I/O pin for display
     */
    void initDisplay(uint8_t clk_pin, uint8_t dio_pin);

    /**
     * @brief Initialize the battery monitor
     * @param pin Analog pin for voltage measurement
     * @param num_samples Number of samples for voltage averaging
     */
    void initBattery(uint8_t pin, uint8_t num_samples = 1);

    /**
     * @brief Main control loop
     * 
     * Updates all skateboard systems:
     * - Battery monitoring
     * - Power state
     * - Radio communication
     * - Motor control
     * - Status display
     */
    void run();
};

#endif // SKATEBOARD_H_
