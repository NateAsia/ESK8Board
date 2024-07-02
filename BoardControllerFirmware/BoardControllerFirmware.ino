#include "skateboard.h"

#define DISPLAY_CLK_PIN 4
#define DISPLAY_DIN_IN 5
// #define LED_PIN 8
#define LED_PIN LED_BUILTIN
#define FLASH_PERIOD 50 
#define BUTTON_PIN 12
#define ESC_PIN 9
#define VOLT_METER_PIN A0
#define VOLTMETER_SAMPLES 40

// Software Definitions  
#define SERIAL_ENABLE

Skateboard board = Skateboard();

void setup() {
  #ifdef SERIAL_ENABLE
    Serial.begin(9600);
    Serial.println("ESK8 Microcontroller Startup");
  #endif

  board.initStatusLight(LED_PIN, FLASH_PERIOD);
  board.initBattery(VOLT_METER_PIN, VOLTMETER_SAMPLES);
  board.initRadio();
  board.initESC(ESC_PIN);
  board.initStatusSwitch(BUTTON_PIN);
  board.initDisplay(
    DISPLAY_CLK_PIN, 
    DISPLAY_DIN_IN
    );
}

void loop() {
  board.run();
}
