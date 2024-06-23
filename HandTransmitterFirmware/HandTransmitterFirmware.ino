#include "CommsProtocol/ESK8Comms.h"
#include "transmiter.h"

#define BUTTON_PIN 9
#define WAKEUP_PIN 2
#define LED_PIN 13
#define RF_DATA_PIN 5
#define RF_SLEEP_PIN 12
#define POT_PIN A0

Transmitter remote = Transmitter();

static void myISR();
void initISR(); 

void setup() {
  Serial.begin(9600);
  remote.initJoystick({
    .btn_pin = BUTTON_PIN, 
    .x_axis_pin = POT_PIN, 
    .y_axis_pin = POT_PIN
    });
  remote.initLED(LED_PIN);
  remote.initRadio(RF_DATA_PIN, RF_SLEEP_PIN);
  initISR(WAKEUP_PIN);
}

void loop() {
  remote.updateInput();
  remote.transmitPacket();
  delay(10); // Adjust the delay as needed
}

static void myISR(){
  remote.isrCheck();
}

void initISR(uint8_t pin){
    pinMode(pin, INPUT);
    attachInterrupt(0, myISR, LOW);
}
