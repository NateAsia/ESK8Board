

#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

#include <stdint.h>
#include <RCSwitch.h>

class Joystick{
    private:
        uint8_t _btn_pin;
        uint8_t _x_axis_pin;
        uint8_t _y_axis_pin;
        
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
        bool        _isr_btn_last_state;

        uint8_t     _rf_sleep_pin;
        bool        _transmit_enable;
        float       percentage;

        bool isrButtonPressed();
    public:
        Transmitter();
        void initJoystick(Joystick::pins_t);
        void initLED(uint8_t);
        void initRadio(uint8_t data_pin, uint8_t rf_sleep_pin); 
        void initISR(uint8_t);

        void isrCheck();
        
        void goto_sleep();

        void updateInput();
        void transmitPacket();

        bool canTransmit();
};


#endif // TRANSMITTER_H_