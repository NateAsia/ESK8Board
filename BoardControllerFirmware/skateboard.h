/**
 * @file skateboard.h
 * @author Nathaniel Asia
 * @brief 
 * Header file containing all skateboard 
 * electromechanical object abstractions
 * @version 0.1
 * @date 2024-06-23
 * 
 * @copyright Copyright (c) 2024
 * 
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

#define DISPLAY_UPDATE_INTERVAL 500 // 500 milliseconds
#define RADIO_DISCONNECT_TIME   100 // 100 milliseconds
#define MOTOR_OFF_THROTTLE      50  // 50% 

class Skateboard {
private:
    Servo           *esc; 
    LED             statusLight;
    ButtonSwitch    statusSwitch;
    RCSwitch        radio;
    TM1637Display   *display;
    VoltMeter       battery;
    
    char        buffer[200];
    uint8_t     _soc;
    bool        _pwm_enable;
    uint16_t    _rf_value;
    uint8_t     _inputThrottlePercent;
    uint8_t     _outputThrottlePercent;
    uint8_t     _throttle_servo_position;
    long        _last_message_time;
    long        _last_display_update;

    ESK8Comms::packet_t packet;

    void checkActiveStatus(); 
    void updateThrottleInput();
    void updateBatteryStatus(); 
    void updateThrottleOutput();
    void printStatus();

public:
    Skateboard();
    void initESC(uint8_t pin);
    void initStatusLight(uint8_t pin, uint16_t flash_period = 0);
    void initStatusSwitch(uint8_t pin);
    void initRadio();
    void initDisplay(uint8_t clk_pin, uint8_t dio_pin);
    void initBattery(uint8_t pin, uint8_t num_samples = 1);
    void run();
};

#endif // SKATEBOARD_H_
