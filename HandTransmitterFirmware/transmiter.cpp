#include "HardwareSerial.h"
#include "Arduino.h"
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
    set(LOW);
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
    switch (_transmit_enable) {
      case true:
        // radio.send(percentage, 11); // Send the percentage value as a custom code
      case false:
        // goto_sleep();
        return;
    }
}
void Transmitter::goto_sleep(){
    LowPower.powerDown(SLEEP_30MS, ADC_OFF, BOD_OFF);
}
bool Transmitter::isrButtonPressed(){
    return !digitalRead(_isr_btn_pin);
}
void Transmitter::initISR(uint8_t pin, void (*userFunc)(void)){
  _isr_flag_prev, _isr_flag = false;
  _isr_btn_pin = pin;
  pinMode(_isr_btn_pin, INPUT_PULLUP);
  attachInterrupt(0, userFunc, LOW);
}
void Transmitter::isrFlag(){
  _isr_flag = true;
}
void Transmitter::isrCheck(){

  if(_isr_flag & !_isr_flag_prev){
    _transmit_enable = !_transmit_enable;
    digitalWrite(_rf_sleep_pin, _transmit_enable);
    led.set(_transmit_enable);
  }
  _isr_flag_prev = _isr_flag;
  _isr_flag = false;
  Serial.println(1);

}