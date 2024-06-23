#include "skateboard.h"

// Digital connection pins (Digital Pins)
#define DISPLAY_CLK_PIN 4
#define DISPLAY_DIN_IN 5
#define LED_PIN 8
#define BUTTON_PIN 12
#define ESC_PIN 9

// Software Definitions  
#define SERIAL_ENABLE true

Skateboard board = Skateboard();

void setup() {
  if (SERIAL_ENABLE) Serial.begin(9600);

  board.initStatusLight(LED_PIN);
  board.initRadio();
  board.initESC(ESC_PIN);
  board.initStatusSwitch(ESC_PIN);
  board.initDisplay(
    DISPLAY_CLK_PIN, 
    DISPLAY_DIN_IN
    );
}

void loop() {
  board.run();
}
