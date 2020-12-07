// verify compilation using the verify/compile tool
const int enablePin = 11;  // adjust all pin values need be
const int in1Pin = 10;
const int in2Pin = 9;
const int tempPin = 12;
int currentTemp = 0;
int idealTemp = 350;
int iterator = 0;

void setup() {                 // (2)
  pinMode(enablePin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {                         // (3)
  if (currentTemp >= idealTemp()) {
    stop();
  }
  else {
    // note that this is not proper thermodynamics, 
    // but i am doing a computation where Q ranges from 100%-0% based on 0-350degC inversely proportional
    int rate = 255*(int ((idealTemp - currentTemp)/idealTemp));
    forward(rate);
    delay(50);
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
