#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

#include <stdint.h>
#include <RCSwitch.h>
#include "HardwareSerial.h"
#include "Arduino.h"
#include "wiring_private.h"
#include <LowPower.h>
#include "ESK8Comms.h"

#define BUTTON_DEBOUNCE_TIME 40 // 40 ms
#define RF_BASE 1100
#define MAX_ADC_BASE 600


class Joystick{
    private:
        uint8_t   _btn_pin;
        uint8_t   _x_axis_pin;
        uint8_t   _y_axis_pin;
        uint16_t  _adc_reading;
        uint16_t  _center_adc_reading;
        uint16_t  _max_adc_reading;
        int8_t   percent;
        
    public:
        enum inputAxis{ X, Y };

        struct pins_t
        {
            uint8_t btn_pin;
            uint8_t x_axis_pin;
            uint8_t y_axis_pin;
        };

        Joystick(){} 
        void    init(Joystick::pins_t pins);
        bool    isButtonPressed();
        bool    lastButtonState;
        float   getAxisPercent(inputAxis);
        void    calibrateMax(bool);
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

class Transmitter{
    private:
        Joystick    joystick;
        LED         led;
        RCSwitch    radio;

        uint8_t     _isr_btn_pin;
        bool        _isr_flag;
        bool        _isr_flag_prev;

        uint8_t     _rf_sleep_pin;
        bool        _transmit_enable;

        ESK8Comms::packet_t packet;
    public:
        Transmitter();
        void initJoystick(Joystick::pins_t);
        void initLED(uint8_t);
        void initRadio(uint8_t data_pin, uint8_t rf_sleep_pin); 
        void initISR(uint8_t pin, void (*userFunc)(void));

        void isrFlag();
        void isrCheck();
        
        void goto_sleep();

        void updateInput();
        void transmitPacket();

        bool canTransmit();

        void run();
};


#endif // TRANSMITTER_H_