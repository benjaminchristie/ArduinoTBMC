// verify compilation using the verify/compile tool
const int enablePin = 11;  // adjust all pin values need be
const int in1Pin = 10;
const int in2Pin = 9;
const int tempPin = 12;
float currentTemp = 0;
float idealTemp = 350;
float iterator = 0;

void setup() {                 // (2)
  pinMode(enablePin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
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
    Serial.print("Rate ");
    Serial.println(rate);
    Serial.print("Current Temp ");
    Serial.println(currentTemp);
    currentTemp = currentTemp + 1;
  }
}

void forward(int pwm)          // (9)
{
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enablePin, pwm);
  Serial.print("Forward ");
  Serial.println(pwm);
}

void stop()                    // (11)
{
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enablePin, 0);
  Serial.println("Stop");
}
