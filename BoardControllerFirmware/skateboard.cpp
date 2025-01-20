#include "Arduino.h"
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

// #define RUN_TIMING_TEST

// #define USE_LINEAR_SOC


void VoltMeter::init(uint8_t pin, uint8_t num_v_readings_for_avg = 1) {
  this->_pin = pin;
  
  this->v_avg_l = malloc(num_v_readings_for_avg * sizeof(float));
  this->_v_avg_l_size = num_v_readings_for_avg;
  this->v_l_oldest_index = 0;
  this->v_l_sum = 0;
  this->_v_list_count = 0;

  for(int i = 0; i<this->_v_avg_l_size; i++){ this->v_avg_l[i] = 0; }

}
float VoltMeter::readVoltage() {
  this->voltage = analogRead(this->_pin);
  this->voltage *= 5.0; // ADC Voltage Max
  this->voltage *= 9.0; // Voltage Divider Multiplier Inverse 
  this->voltage /= 1023.0;
  return this->voltage;
}
float VoltMeter::averageVoltage(){
    // Remove Oldest Index From Sum
    this->v_l_sum -= this->v_avg_l[this->v_l_oldest_index];

    // Add the new value to the buffer
    this->v_avg_l[this->v_l_oldest_index] = readVoltage();

    // Add the new value to the sum
    this->v_l_sum += this->v_avg_l[this->v_l_oldest_index];

    // Update the buffer index
    this->v_l_oldest_index = (this->v_l_oldest_index + 1) % this->_v_avg_l_size;

    // Update the count of elements added to the buffer (if not yet full)
    if (this->_v_list_count < this->_v_avg_l_size) {
      this->_v_list_count++;
    }

    // Calculate the moving average
    return this->voltAvg = this->v_l_sum / (float)this->_v_list_count;

}
uint16_t VoltMeter::getSOC() {
  #ifdef USE_LINEAR_SOC
    this->stateOfCharge = (averageVoltage()-MIN_BAT_V) * 100;
    this->stateOfCharge /= (MAX_BAT_V-MIN_BAT_V);
    // this->stateOfCharge = constrain(this->stateOfCharge, 0, 100);
    return stateOfCharge;
  #else
    this->averageVoltage(); // This updates the voltAvg

    // Number of entries of the lithium 
    const int numEntries = sizeof(lithium_ion_6s_SOC) / sizeof(lithium_ion_6s_SOC[0]);

    // If the input voltage is below the minimum, return the lowest SOC
    if (this->voltAvg <= lithium_ion_6s_SOC[0].voltage) return lithium_ion_6s_SOC[0].soc;

    // If the input voltage is above the maximum, return an extrapolation
    if (this->voltAvg >= lithium_ion_6s_SOC[numEntries-1].voltage){
        stateOfCharge =   (float) (lithium_ion_6s_SOC[numEntries-1].soc - lithium_ion_6s_SOC[numEntries-2].soc) / (lithium_ion_6s_SOC[numEntries-1].voltage - lithium_ion_6s_SOC[numEntries-2].voltage);
        stateOfCharge *=  (float) (this->voltAvg - lithium_ion_6s_SOC[numEntries-2].voltage);
        stateOfCharge +=  (float)  lithium_ion_6s_SOC[numEntries-2].soc;
        return (uint16_t) stateOfCharge;
      // return lithium_ion_6s_SOC[numEntries - 1].soc;
    }

    // Interpolate the SOC for the given voltage
    for (int i = 1; i < numEntries; i++) {
      if (this->voltAvg < lithium_ion_6s_SOC[i].voltage) {
        stateOfCharge =   (float) (lithium_ion_6s_SOC[i].soc - lithium_ion_6s_SOC[i-1].soc) / (lithium_ion_6s_SOC[i].voltage - lithium_ion_6s_SOC[i-1].voltage);
        stateOfCharge *=  (float) (this->voltAvg - lithium_ion_6s_SOC[i-1].voltage);
        stateOfCharge +=  (float)  lithium_ion_6s_SOC[i-1].soc;
        return (uint16_t) stateOfCharge;
      }
    }

    // Default return (should never be reached)
    return 0;
  #endif
}

// LED
void LED::init(uint8_t pin, uint16_t flash_period = 0) {
  this->_pin = pin;
  this->_flash_period = flash_period;
  this->_last_flash_time = millis();
  this->_state = false;
  this->_led_state = false;
  pinMode(_pin, OUTPUT);
}
void LED::set(bool state) {
  this->_state = state;
}
void LED::run(long currTime){ 
  if(this->_state){
    if (this->_flash_period && currTime - this->_last_flash_time > this->_flash_period){
      this->_last_flash_time=currTime;
      this->_led_state = !this->_led_state;
      digitalWrite(_pin, this->_led_state);
    }
  } 
  else{
    digitalWrite(_pin, _led_state);
  }
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
void ButtonSwitch::setState(bool state){
  _switchState = state;
}


Skateboard::Skateboard() {
    _pwm_enable = false;
    _rf_value = 1150;
    _inputThrottlePercent = 50;
    _outputThrottlePercent = 50;
    _throttle_servo_position = 90;
    _last_message_time = millis();
    _last_display_update = _last_message_time;
    esc = new Servo(); 
    statusSwitch.setState(true);
}

void Skateboard::initESC(uint8_t pin) {
    esc->attach(pin);
}

void Skateboard::initStatusLight(uint8_t pin, uint16_t flash_period) {
    statusLight.init(pin, flash_period);
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

void Skateboard::initBattery(uint8_t pin, uint8_t num_samples) {
    battery.init(pin, num_samples);
}

void Skateboard::updateThrottleInput() {
    if (radio.available()) {
        _rf_value = radio.getReceivedValue();
        packet = *((ESK8Comms::packet_t*) &_rf_value);
        radio.resetAvailable();
        if (ESK8Comms::validPacket(packet)) {
            _inputThrottlePercent = ESK8Comms::readAPPSFromPacket(packet);
            _last_message_time = millis();
        }
    }
}

void Skateboard::checkActiveStatus() {
    _pwm_enable = statusSwitch.getState();
    statusLight.set(_pwm_enable);
    statusLight.run(millis());
}

void Skateboard::updateBatteryStatus() {
    if (millis() - _last_display_update > DISPLAY_UPDATE_INTERVAL) {
        _soc = battery.getSOC();
        display->showNumberDec(_soc);
        _last_display_update = millis();
    }
}

void Skateboard::updateThrottleOutput() {
    _outputThrottlePercent = _inputThrottlePercent;

    if (!_pwm_enable || millis() - _last_message_time > RADIO_DISCONNECT_TIME) {
        _outputThrottlePercent = MOTOR_OFF_THROTTLE;
    }
    _throttle_servo_position = map(_outputThrottlePercent, 0, 100, 0, 180);
    esc->write(_throttle_servo_position);
}

void Skateboard::printStatus() {
    snprintf(buffer,
             sizeof(buffer),
             "\tBattery SOC: %d"
             "\tRADIO_INPUT: %x"
             "\tThrottle Input: %d"
             "\tThrottle Output: %d"
             "\tServo Position: %d",
             _soc,
             _rf_value,
             _inputThrottlePercent,
             _outputThrottlePercent,
             _throttle_servo_position
            );
    Serial.println(buffer);
}

void Skateboard::run() {
    updateBatteryStatus();
    checkActiveStatus();
    updateThrottleInput();
    updateThrottleOutput();
    #ifdef SERIAL_ENABLE
        printStatus();
    #endif
}
