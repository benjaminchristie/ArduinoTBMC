// verify compilation using the verify/compile tool
const int enablePin = 11;  // adjust all pin values need be
float currentTemp = 0;
float idealTemp = 350;
float iterator = 0;

void setup() {                 // (2)
  pinMode(enablePin, OUTPUT);
  Serial.begin(9600);
}

void loop() {                         // (3)
  if (currentTemp >= idealTemp) {
    stop();
  }
  else {
    // note that this is not proper thermodynamics, 
    // but i am doing a computation where Q ranges from 100%-0% based on 0-350degC inversely proportional
    float rate = 255.0*(((idealTemp - currentTemp)/idealTemp));
    forward(rate);
    delay(20);
    Serial.print(rate);
    Serial.print(",");
    Serial.println(currentTemp);
    currentTemp = currentTemp + 1;
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
