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
#include "HardwareSerial.h"
#include <stdint.h>
#include <TM1637Display.h>
#include <Servo.h>
#include <RCSwitch.h>
#include "ESK8Comms.h"
#include "wiring_private.h"

#define DISPLAY_UPDATE_INTERVAL 500 // 500 milliseconds
#define BTN_DEBOUNCE            40 // 100 milliseconds
#define RADIO_DISCONNECT_TIME   100 // 100 milliseconds
#define MOTOR_OFF_THROTTLE      50  // 50% 
// #define SERIAL_ENABLE true

#define MAX_BAT_V 25.2
#define MIN_BAT_V 18.0


typedef struct vToSOC_t{
  float voltage;
  uint8_t soc;
} voltageToSOC ;

const voltageToSOC lithium_ion_6s_SOC[] = {
  {18.0, 0},
  {19.8, 10},
  {21.6, 50},
  {23.2, 90},
  {25.2, 100},
};


class VoltMeter
{
private:
    uint8_t   _pin;
    
    float     *v_avg_l;
    uint8_t   _v_avg_l_size;
    float     v_l_sum;
    uint8_t   v_l_oldest_index; 
    uint8_t   _v_list_count;
    
    float     voltage;
    float     voltAvg; 
    
    float     stateOfCharge; 
    
    float     readVoltage();
    float     averageVoltage();
public:
    VoltMeter() {}

    void      init(uint8_t, uint8_t = 1);
    uint16_t  getSOC();
};

class LED{
  private:
      uint8_t   _pin;
      bool      _state;
      bool      _led_state;  
      long      _last_flash_time;
      uint16_t  _flash_period; 
  public:
      LED(){}
      void init(uint8_t, uint16_t = 0);
      void set(bool);
      void run(long);
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
    void setState(bool);
};

class Skateboard{
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
    void initESC(uint8_t);
    void initStatusLight(uint8_t, uint16_t = 0);
    void initStatusSwitch(uint8_t);
    void initRadio();
    void initDisplay(uint8_t, uint8_t);
    void initBattery(uint8_t, uint8_t = 1);

    void run();
};



#endif // SKATEBOARD_H_