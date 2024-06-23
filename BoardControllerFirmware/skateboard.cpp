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


void VoltMeter::init(uint8_t pin){
    _pin = pin;
}
float VoltMeter::readVoltage(){
    return voltage = analogRead(A0)* 5.0 * 9.0 / 1023.0;
}
float VoltMeter::getSOC(){

    stateOfCharge = map(readVoltage(), 20, 25, 0, 100); 
        // change this from a linear map to lithium-ion SOC map
    #ifdef SERIAL_ENABLE
        Serial.print("V_div ");
        Serial.print(voltage);
        Serial.print(" ");
        Serial.println(stateOfCharge);
    #endif

    return stateOfCharge;
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


// ButtonSwitch
void ButtonSwitch::init(uint8_t pin){
    _pin = pin;
    _state = false;
    pinMode(_pin, INPUT_PULLUP);
}
bool ButtonSwitch::readPin(){
    return _state = !digitalRead(_pin);
}
bool ButtonSwitch::getState(){
    
    // Detect Rising Edge, and start debounce
    if (_last_state != readPin()){
        _last_hold_start = millis();
        _last_state = _state;

        if (!_state){
            _disable = false;
        }
    }
    // After debouncing, perform action
    if (_state & !_disable & (millis() - _last_hold_start > BTN_DEBOUNCE)){
        _switchState = ~_switchState; 
        _disable = true;
    }
    return _switchState;
}


Skateboard::Skateboard(){
    _pwm_enable = false;
    _inputThrottlePercent = 50.0;
    _outputThrottlePercent = 50.0;
    _last_message_time = millis(); 
}
void Skateboard::initESC(uint8_t pin){
    esc.attach(pin); 
}
void Skateboard::initStatusLight(uint8_t pin){
    statusLight.init(pin);
    statusLight.set(_pwm_enable);
}
void Skateboard::initStatusSwitch(uint8_t pin){
    statusSwitch.init(pin);
}
void Skateboard::initRadio(){
    radio.enableReceive(0);
}
void Skateboard::initDisplay(uint8_t clk_pin, uint8_t dio_pin){
    display = TM1637Display(clk_pin, dio_pin);
    display.setBrightness(0x08);
}
void Skateboard::initBattery(uint8_t pin){
    battery.init(pin);
}
void Skateboard::updateThrottleInput(){
    if (radio.available()) {
        _rf_value = radio.getReceivedValue();
        _last_message_time = millis();
        radio.resetAvailable();
    }
    if ((_rf_value >= 1100) && (_rf_value <= 1200)){
        _inputThrottlePercent = _rf_value - 1100;
    }
}
void Skateboard::checkActiveStatus(){
    _pwm_enable = statusSwitch.getState();
    statusLight.set(_pwm_enable);
}
void Skateboard::updateBatteryStatus(){
    if (millis() - _last_display_update > DISPLAY_UPDATE_INTERVAL){
        display.showNumberDec(
            battery.getSOC() * 100, 
            false
        );
        _last_display_update = millis();
    }
}
void Skateboard::updateThrottleOutput(){
    _outputThrottlePercent = _inputThrottlePercent;

    if ( ~_pwm_enable || millis()-_last_message_time > RADIO_DISCONNECT_TIME){
        _outputThrottlePercent = MOTOR_OFF_THROTTLE;
    }
    #ifdef SERIAL_ENABLE
        Serial.println( _outputThrottlePercent );
    #endif

    _throttle_servo_position = map(_outputThrottlePercent, 0, 100, 0, 180);
    
    esc.write(_throttle_servo_position); // Makes the PWM Waveform 

}
void Skateboard::run(){
    updateBatteryStatus();
    checkActiveStatus();
    updateThrottleInput();
    updateThrottleOutput();
}