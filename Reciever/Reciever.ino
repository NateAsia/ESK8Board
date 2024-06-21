#include <RCSwitch.h>
#include <Servo.h>
#include <Arduino.h>
#include <TM1637Display.h>

// Digital connection pins (Digital Pins)
#define CLK 4
#define DIO 5
#define LED_PIN 8
#define BUTTON_PIN 12
#define ESC_PIN 9

// Software Definitions  
#define SERIAL_ENABLE true

TM1637Display display(CLK, DIO);

Servo ESC;  
RCSwitch rec = RCSwitch();

int rf_value;
int pwm_percent;
int servoPosition;

float voltage_div;
float battery_v = 0.0;
int percent;

int counter;

bool pwm_r_enable = false;
bool pwm_enable = false;

long last_time = 0; 

void setup() {
  if (SERIAL_ENABLE) Serial.begin(9600);
  rec.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  ESC.attach(ESC_PIN);
  
  rf_value = 1150;
  display.setBrightness(0x08);
  read_battery_voltage();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, pwm_enable);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
}


void check_rf(){
  if (rec.available()) {
    rf_value = rec.getReceivedValue();
    last_time = millis();
    rec.resetAvailable();
  }
}

void read_battery_voltage(){
  
  voltage_div = analogRead(A0);
  battery_v = voltage_div * 5.0 * 9.0 / 1023.0;
  percent = map(battery_v, 20, 25, 0, 100);
  
  if (SERIAL_ENABLE){
    Serial.print("V_div ");
    Serial.print(voltage_div);
    Serial.print(" ");
    Serial.println( battery_v );
  }

  display.showNumberDec(battery_v * 100, false);
  
}

void read_button(){
  if (digitalRead(BUTTON_PIN) == 0 ) {
    pwm_r_enable = !pwm_enable;
  }
  
  if (pwm_r_enable != pwm_enable){
    pwm_enable = pwm_r_enable;
    digitalWrite(LED_PIN, pwm_enable);
  }
}

void loop() {
   
  if (counter >= 100) {
    read_battery_voltage();
    counter = 0;
    read_button();
  }

  check_rf();

  if ((rf_value >= 1100) && (rf_value <= 1200)){
     pwm_percent = rf_value - 1100;
     
  }
  
  servoPosition = map(pwm_percent, 0, 100, 0, 180);
  if (not pwm_enable || millis()-last_time > 100){
    servoPosition=100;
  }
  if (SERIAL_ENABLE) Serial.println( pwm_percent );
  ESC.write(servoPosition);  // Set the servo position
  counter++;
  delay(1);
}
