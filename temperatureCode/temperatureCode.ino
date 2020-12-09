// verify compilation using the verify/compile tool
#include <Adafruit_MAX31865.h> // be sure to have the Adafruit_MAX31865 and Adafruit_BusIO libraries installed.
const int enablePin = 11;  // adjust all pin values need be
const int tempPin = 12;
const float RREF = 4300.0;
Adafruit_MAX31865 max = Adafruit_MAX31865(4, 5, 6, 7); // adjust pin values need be
float currTemp = 0;
float idealTemp = 350;

void setup() {                 // (2)
  pinMode(enablePin, OUTPUT);
  Serial.begin(9600);
  // Use software SPI: CS, DI, DO, CLK
  max.begin(MAX31865_4WIRE);
  if (testFailure()) {
    noInterrupts();
    while (1) {
      Serial.print("Stuck! ");  
    } // continue to loop and do nothing so that there is not a hardware issue with
  }
}

bool testFailure() {
  uint8_t fault = max.readFault();
  Serial.print("Fault 0x"); Serial.println(fault, HEX);
  if (fault) {
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
  currTemp = getTemp();
  if (currTemp >= idealTemp) {
    stop();
  }
  else {
    // Q ranges from 100%-0% based on 0-350degC inversely proportional
    float rate = 255.0 * (((idealTemp - currTemp) / idealTemp));
    forward(rate);
    Serial.print(rate);
    Serial.print(",");
    Serial.println(currTemp);
    delay(20);
  }
}

void forward(int pwm)          // (9)
{
  analogWrite(enablePin, pwm);
}

void stop()                    // (11)
{
  analogWrite(enablePin, 0);
}
