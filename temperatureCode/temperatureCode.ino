#include <Adafruit_MAX31865.h>
const int enablePin = 11;  // (1)
const int in1Pin = 10;
const int in2Pin = 9;
const int tempPin = 12;
const float RREF = 4300.0;
Adafruit_MAX31865 max = Adafruit_MAX31865(4, 5, 6, 7);
float currentTemp = 0;
float idealTemp = 450;

void setup() {                 // (2)
  pinMode(enablePin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  Serial.begin(9600);
  // Use software SPI: CS, DI, DO, CLK
  max.begin(MAX31865_4WIRE);
  if (testFailure()) {
    noInterrupts();
    while(1) {} // continue to loop and do nothing so that there is not a hardware issue with
  } 
}

bool testFailure() {
  uint8_t fault = max.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold");
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold");
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias");
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage");
    }
    max.clearFault();
    return true;
  }
  return false;
}

float getTemp() {
  float temp = max.temperature(1000, RREF); // 1000 because PT1000
  return temp;
}
void loop() {                         // (3)
  float currTemp = getTemp();
  // note that this is not proper thermodynamics, 
  // but i am doing a computation where Q ranges from 100%-0% based on 0-350degC inversely proportional
  int rate = 255*(int ((idealTemp - currTemp)/idealTemp));
  forward(rate);
  delay(10);
}

void forward(int pwm)          // (9)
{
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enablePin, pwm);
  Serial.print("Forward ");
  Serial.println(pwm);
}

void reverse(int pwm)          // (10)
{
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enablePin, pwm);
  Serial.print("Reverse ");
  Serial.println(pwm);
}

void stop()                    // (11)
{
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enablePin, 0);
  Serial.println("Stop");
}
