#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

#include <stdint.h>
#include <RCSwitch.h>
#include <LowPower.h>
#include "Arduino.h"
#include "ESK8Comms.h"
#include "joystick.h"
#include "led.h"

class Transmitter {
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
