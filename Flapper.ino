#include <Servo.h>  // ESP32-compatible Servo library

Servo flapper1;
Servo flapper2;

// Define GPIO pins
const int irSensor1 = 14;
const int irSensor2 = 27;
const int servoPin1 = 18;
const int servoPin2 = 19;

const int openAngle = 45;
const int closedAngle = 0;

void setup() {
  Serial.begin(115200);

  // Attach servos to pins
  flapper1.attach(servoPin1);
  flapper2.attach(servoPin2);

  // IR sensors as input
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);

  // Initialize flappers in closed position
  flapper1.write(closedAngle);
  flapper2.write(closedAngle);
}

void loop() {
  int sensor1State = digitalRead(irSensor1);
  int sensor2State = digitalRead(irSensor2);

  // IR sensors typically return LOW when detecting an object
  if (sensor1State == LOW) {
    Serial.println("Waste detected at Flapper 1");
    flapper1.write(openAngle);
    delay(1000);  // Keep open for 1 second
    flapper1.write(closedAngle);
  }

  if (sensor2State == LOW) {
    Serial.println("Waste detected at Flapper 2");
    flapper2.write(openAngle);
    delay(1000);
    flapper2.write(closedAngle);
  }

  delay(100);  // Small delay to stabilize sensor reading
}
