#include <LowPower.h>
#include <RCSwitch.h>

#define BUTTON_PIN 9
#define WAKEUP_PIN 2
#define LED_PIN 13
#define RF_SLEEP 12

RCSwitch mySwitch = RCSwitch();

int potentiometerPin = A0;
int potValue = 0;
int percentage = 0;

bool transmit = false;

void goto_sleep(){
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

void button_pressed(){
  transmit = !transmit;
  digitalWrite(RF_SLEEP, transmit);
  digitalWrite(LED_PIN, transmit);
  delay(200);  
}

void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(5); // Set the transmitter data pin
  mySwitch.setPulseLength(320); // Set the pulse length in microseconds
  mySwitch.setRepeatTransmit(5); // Set the number of transmission repeats
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(WAKEUP_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RF_SLEEP, OUTPUT);
  attachInterrupt(0, button_pressed, LOW);
}

void loop() {
  potValue = analogRead(potentiometerPin);
  percentage = map(potValue, 0, 1023, 1100, 1200); // Map the potentiometer reading to a percentage (0-100)
  

//  check_button();

  if (transmit){
    mySwitch.send(percentage, 11); // Send the percentage value as a custom code
  }
  else{
    goto_sleep();
  }
  delay(10); // Adjust the delay as needed
}
