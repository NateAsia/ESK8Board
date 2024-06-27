#include "Arduino.h"
#include "HardwareSerial.h"
/**
 * @file skateboard.cpp
 * @author Nathaniel Asia
 * @brief 
 * Implementations of skateboard.h
 * @version 0.1
 * @date 2024-06-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "skateboard.h"
#include "wiring_private.h"
#include <TM1637Display.h>
#include <stdint.h>

// #define RUN_TIMING_TEST


void VoltMeter::init(uint8_t pin) {
  _pin = pin;
}
float VoltMeter::readVoltage() {
  voltage = analogRead(A0) * 5.0 * 9.0 / 1023.0;
  return voltage;
}
int8_t VoltMeter::getSOC() {
  stateOfCharge = map(readVoltage(), 20, 25, 0, 100);
  stateOfCharge = constrain(stateOfCharge, 0, 100);
  return stateOfCharge;
}

// LED
void LED::init(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}
void LED::set(bool state) {
  _state = state;
  digitalWrite(_pin, _state);
}


// ButtonSwitch
void ButtonSwitch::init(uint8_t pin) {
  _pin = pin;
  _state = false;
  pinMode(_pin, INPUT_PULLUP);
}
bool ButtonSwitch::readPin() {
  return _state = !digitalRead(_pin);
}
bool ButtonSwitch::getState() {

  // Detect Rising Edge, and start debounce
  if (_last_state != readPin()) {
    _last_hold_start = millis();
    _last_state = _state;

    if (!_state) {
      _disable = false;
    }
  }
  // After debouncing, perform action
  if (_state & !_disable & (millis() - _last_hold_start > BTN_DEBOUNCE)) {
    _switchState = !_switchState;
    _disable = true;
  }
  return _switchState;
}


Skateboard::Skateboard() {
  _pwm_enable = false;
  _rf_value = 1150;
  _inputThrottlePercent = 50.0;
  _outputThrottlePercent = 50.0;
  _throttle_servo_position = 90;
  _last_message_time = millis();
  _last_display_update = _last_message_time;
  esc = new Servo(); 
}
void Skateboard::initESC(uint8_t pin) {
  esc->attach(pin);
}
void Skateboard::initStatusLight(uint8_t pin) {
  statusLight.init(pin);
  statusLight.set(_pwm_enable);
}
void Skateboard::initStatusSwitch(uint8_t pin) {
  statusSwitch.init(pin);
}
void Skateboard::initRadio() {
  radio.enableReceive(0);
}
void Skateboard::initDisplay(uint8_t clk_pin, uint8_t dio_pin) {
  display = new TM1637Display(clk_pin, dio_pin);
  display->setBrightness(0x08);
}
void Skateboard::initBattery(uint8_t pin) {
  battery.init(pin);
}
void Skateboard::updateThrottleInput() {
  if (radio.available()) {
    _rf_value = radio.getReceivedValue();
    // add encoding (switch to using a 32 bit unsigned int) add a code at the start and the end
    _last_message_time = millis();
    radio.resetAvailable();
  }
  if ((_rf_value >= 1100) && (_rf_value <= 1200)) {
    _inputThrottlePercent = _rf_value - 1100;
  }
}
void Skateboard::checkActiveStatus() {
  _pwm_enable = statusSwitch.getState();
  statusLight.set(_pwm_enable);
}
void Skateboard::updateBatteryStatus() {
  if (millis() - _last_display_update > DISPLAY_UPDATE_INTERVAL) {
    _soc = battery.getSOC() * 100;
    display->showNumberDec(
      _soc,
      false);
    _last_display_update = millis();
  }
}
void Skateboard::updateThrottleOutput() {
  _outputThrottlePercent = _inputThrottlePercent;

  if (!_pwm_enable || millis() - _last_message_time > RADIO_DISCONNECT_TIME) {
    _outputThrottlePercent = MOTOR_OFF_THROTTLE;
  }
  _throttle_servo_position = map(_outputThrottlePercent, 0, 100, 0, 180);
  esc->write(_throttle_servo_position);  // Makes the PWM Waveform
}
void Skateboard::printStatus() {
  snprintf(buffer,
           sizeof(buffer),
           "Battery SOC: %d"
           "\tRADIO_INPUT: %d"
           "\tThrottle Input: %d"
           "\tThrottle Output: %d"
           "\tServo Position: %d",
           this->_soc,
           this->_rf_value,
           this->_inputThrottlePercent,
           this->_outputThrottlePercent,
           this->_throttle_servo_position
          );
  Serial.println(buffer);
}
void Skateboard::run() {
  #ifdef RUN_TIMING_TEST
    long bt = micros();
    this->updateBatteryStatus();
    long as =  micros();
    this->checkActiveStatus();
    long ti = micros();
    this->updateThrottleInput();
    long to = micros();
    this->updateThrottleOutput();
    long p = micros();
    #if SERIAL_ENABLE
      this->printStatus();
    #endif
    long now = micros();
    Serial.println(as-bt);
    Serial.println(ti-as);
    Serial.println(to-ti);
    Serial.println(p-to);
    Serial.println(now-p);
    while(1);
  #else
    this->updateBatteryStatus();
    this->checkActiveStatus();
    this->updateThrottleInput();
    this->updateThrottleOutput();
    #if SERIAL_ENABLE
      this->printStatus();
    #endif
  #endif 
  delay(1);
}