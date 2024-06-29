#include "Print.h"
#include "HardwareSerial.h"
#include "transmitter.h"


// Joystick
void Joystick::init(Joystick::pins_t pins) {
  _btn_pin = pins.btn_pin;
  _x_axis_pin = pins.x_axis_pin;
  _y_axis_pin = pins.y_axis_pin;
  _center_adc_reading = analogRead(_x_axis_pin);
  _max_adc_reading = MAX_ADC_BASE;
  lastButtonState = false;
}
float Joystick::getAxisPercent(inputAxis axis) {
  _adc_reading = analogRead(axis ? _y_axis_pin : _x_axis_pin);
  this->calibrateMax(false);
  if(_adc_reading < _center_adc_reading){
    percent = map(
      _adc_reading,
      0, _center_adc_reading,
      0, 50
    );
    percent = constrain(percent, 0, 50);
  }
  else{
    percent = map(
      _adc_reading,
      _center_adc_reading, _max_adc_reading,
      50, 100
    );
    percent = constrain(percent, 50, 100);
  }
  return constrain(percent, 0, 100); 
}
bool Joystick::isButtonPressed() {
  return !digitalRead(_btn_pin);
}
void Joystick::calibrateMax(bool reset){
  if (reset){
    _max_adc_reading = MAX_ADC_BASE; 
  }
  _max_adc_reading = max(_max_adc_reading, _adc_reading);
}


// LED
void LED::init(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
  set(LOW);
}
void LED::set(bool state) {
  _state = state;
  digitalWrite(_pin, _state);
}


// Transmitter
Transmitter::Transmitter() {
  _transmit_enable = false;
  joystick = Joystick();
  led = LED();
  radio = RCSwitch();
}
void Transmitter::initJoystick(Joystick::pins_t pins) {
  joystick.init(pins);
}
void Transmitter::initLED(uint8_t pin) {
  led.init(pin);
  led.set(_transmit_enable);
}
void Transmitter::initRadio(uint8_t data_pin, uint8_t rf_sleep_pin) {
  radio = RCSwitch();
  _rf_sleep_pin = rf_sleep_pin;
  pinMode(_rf_sleep_pin, OUTPUT);
  radio.enableTransmit(data_pin);  // Set the transmitter data pin
  radio.setPulseLength(200);       // Set the pulse length in microseconds
  radio.setRepeatTransmit(4);      // Set the number of transmission repeats
}
void Transmitter::updateInput() {
  this->packet.APPS = joystick.getAxisPercent(Joystick::inputAxis::X);
}
void Transmitter::transmitPacket() {
  if(_transmit_enable) {
      radio.send(*((uint16_t*) &packet), 16); 
      ESK8Comms::printPacket(packet, true);
  }
  else{
    if(!_isr_flag){
      goto_sleep();
    }
  }
}
void Transmitter::goto_sleep() {
  Serial.println("sleep");
  _isr_flag_prev = _isr_flag;
  delay(10);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  joystick.calibrateMax(true);
  Serial.println("awake");
}
void Transmitter::initISR(uint8_t pin, void (*userFunc)(void)) {
  _isr_flag_prev, _isr_flag = false;
  _isr_btn_pin = pin;
  pinMode(_isr_btn_pin, INPUT_PULLUP);
  attachInterrupt(0, userFunc, LOW);
}
void Transmitter::isrFlag() {
  _isr_flag = true;
}
void Transmitter::isrCheck() {
  if (_isr_flag & !_isr_flag_prev) {
    _transmit_enable = !_transmit_enable;
    digitalWrite(_rf_sleep_pin, _transmit_enable);
    led.set(_transmit_enable);
  }
  _isr_flag_prev = _isr_flag;
  _isr_flag = false;
}
void Transmitter::run(){
  this->isrCheck();
  this->updateInput();
  this->transmitPacket();
  delay(10);
}
