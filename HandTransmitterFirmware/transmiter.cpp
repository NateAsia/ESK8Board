#include "wiring_private.h"
#include <RCSwitch.h>
#include <LowPower.h>
#include "transmiter.h"


// Joystick
void Joystick::init(Joystick::pins_t pins){
    _btn_pin    = pins.btn_pin;
    _x_axis_pin = pins.x_axis_pin;
    _y_axis_pin = pins.y_axis_pin;
    lastButtonState = false;
}

float Joystick::getAxisPercent(inputAxis axis){
    return map(
            analogRead(axis ? _y_axis_pin : _x_axis_pin),
            0, 1023,
            0, 100
            );
}

bool Joystick::isButtonPressed(){
    return !digitalRead(_btn_pin);
}


// LED
void LED::init(uint8_t pin){
    _pin = pin;
    pinMode(_pin, OUTPUT);
}

void LED::set(bool state){
    _state = state;
    digitalWrite(_pin, _state);
}


// Transmitter 
Transmitter::Transmitter(){
    _transmit_enable = false;
    joystick = Joystick();
    led = LED();
    radio = RCSwitch(); 
}

void Transmitter::initJoystick(Joystick::pins_t pins){
    joystick.init(pins);
}

void Transmitter::initLED(uint8_t pin){
    led.init(pin);
    led.set(_transmit_enable);
}

void Transmitter::initRadio(uint8_t data_pin, uint8_t rf_sleep_pin){

    radio = RCSwitch();
    _rf_sleep_pin = rf_sleep_pin;
    pinMode(_rf_sleep_pin, OUTPUT);

    radio.enableTransmit(data_pin);    // Set the transmitter data pin
    radio.setPulseLength(320);  // Set the pulse length in microseconds
    radio.setRepeatTransmit(5); // Set the number of transmission repeats
}

void Transmitter::updateInput(){
        percentage = joystick.getAxisPercent(Joystick::inputAxis::X);
}

void Transmitter::transmitPacket(){
    if (_transmit_enable){
        radio.send(percentage, 11); // Send the percentage value as a custom code
    }
    else{
        goto_sleep();
    }
}

void Transmitter::goto_sleep(){
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

bool Transmitter::isrButtonPressed(){
    return !digitalRead(_isr_btn_pin);
}

void Transmitter::isrCheck(){

  // this enures this only runs on rising edge of button press
  if(isrButtonPressed() & !_isr_btn_last_state){
      _transmit_enable = !_transmit_enable;
      digitalWrite(_rf_sleep_pin, _transmit_enable);
      led.set(_transmit_enable);
  }
  _isr_btn_last_state = isrButtonPressed();

}