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

#include <stdint.h>
#include <TM1637Display.h>
#include <Servo.h>
#include <RCSwitch.h>

#define DISPLAY_UPDATE_INTERVAL 500 // 500 milliseconds
#define BTN_DEBOUNCE            40 // 100 milliseconds
#define RADIO_DISCONNECT_TIME   100 // 100 milliseconds
#define MOTOR_OFF_THROTTLE      50  // 50% 
#define SERIAL_ENABLE true

class VoltMeter
{
private:
    uint8_t _pin;
    float voltage;
    uint8_t stateOfCharge; 
public:
    VoltMeter() {}

    void      init(uint8_t);
    float     readVoltage();
    uint8_t   getSOC();
};

class LED{
  private:
      uint8_t _pin;
      bool    _state; 
  public:
      LED(){}
      void init(uint8_t);
      void set(bool);
};

class ButtonSwitch{
private:
    uint8_t _pin;
    long    _last_hold_start;
    bool    _disable;
    bool    _state;
    bool    _last_state;
    bool    _switchState;
public:
    Button() {}
    void init(uint8_t);
    bool readPin();
    bool readEdge();
    bool getState();
};

class Skateboard{
private:
    Servo           esc; 
    LED             statusLight;
    ButtonSwitch    statusSwitch;
    RCSwitch        radio;
    TM1637Display   &display;
    VoltMeter       battery;
    
    char        buffer[200];
    uint8_t     _soc;
    bool        _pwm_enable;
    int    _rf_value;
    int     _inputThrottlePercent;
    int     _outputThrottlePercent;
    int     _throttle_servo_position;
    long        _last_message_time;
    long        _last_display_update;

    void checkActiveStatus(); 
    void updateThrottleInput();
    void updateBatteryStatus(); 
    void updateThrottleOutput();
    void printStatus();

public:
    Skateboard();
    void initESC(uint8_t);
    void initStatusLight(uint8_t);
    void initStatusSwitch(uint8_t);
    void initRadio();
    void initDisplay(uint8_t, uint8_t);
    void initBattery(uint8_t);

    void run();
};



#endif // SKATEBOARD_H_