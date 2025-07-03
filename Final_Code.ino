#include <Servo.h>

// Create servo objects
Servo plasticServo;         // Flapper for plastic waste
Servo nonPlasticServo;      // Flapper for non-plastic waste

// Pin assignments
const int capSensor1 = 2;   // Capacitive Sensor 1 
const int capSensor2 = 3;   // Capacitive Sensor 2 
const int irSensor1 = 4;    // IR Sensor 1 (activates Flapper 1)
const int irSensor2 = 5;    // IR Sensor 2 (activates Flapper 2)
const int plasticServoPin = 9;    // Servo pin for plastic flapper
const int nonPlasticServoPin = 10; // Servo pin for non-plastic flapper

// Flapper angles
const int openAngle = 45;
const int closedAngle = 0;

void setup() {
  Serial.begin(9600);

  // Attach servos to their respective pins
  plasticServo.attach(plasticServoPin);
  nonPlasticServo.attach(nonPlasticServoPin);

  // Initialize flappers to closed position
  plasticServo.write(closedAngle);
  nonPlasticServo.write(closedAngle);

  // Set sensor pins as input
  pinMode(capSensor1, INPUT);
  pinMode(capSensor2, INPUT);
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
}

void loop() {
  // Read the sensor states
  bool flag_cap1 = digitalRead(capSensor1);  // HIGH if plastic detected
  bool flag_cap2 = digitalRead(capSensor2);  // HIGH if plastic detected

  bool ir1_detected = digitalRead(irSensor1); // IR Sensor 1 for Flapper 1
  bool ir2_detected = digitalRead(irSensor2); // IR Sensor 2 for Flapper 2

  // Debug output
  Serial.print("CapSensor1: ");
  Serial.print(flag_cap1);
  Serial.print(" | CapSensor2: ");
  Serial.print(flag_cap2);
  Serial.print(" | IR Sensor1: ");
  Serial.print(ir1_detected);
  Serial.print(" | IR Sensor2: ");
  Serial.println(ir2_detected);

  // 1.Plastic Waste Detection
  if (flag_cap1 == HIGH && flag_cap2 == LOW) {
    Serial.println("Plastic Waste Detected");

    // Flapper 1 opens when IR Sensor 1 detects waste
    if (ir1_detected == HIGH) {
      plasticServo.write(openAngle); // Open the first flapper
      delay(2000); // Keep it open for 2 second
      plasticServo.write(closedAngle); // Close the flapper
    }

    // Flapper 2 remains closed
    nonPlasticServo.write(closedAngle);
  }

  // 2. Non-Plastic Waste Detection
  else if (flag_cap1 == LOW && flag_cap2 == LOW) {
    Serial.println("Non-Plastic Waste Detected");

    // Flapper 1 opens when IR Sensor 1 detects waste
    if (ir1_detected == HIGH) {
      plasticServo.write(openAngle); // Open the first flapper
      delay(1000); // Keep it open for 1 second
      plasticServo.write(closedAngle); // Close the flapper
    }

    // Wait for 2 seconds for flapper 2 to opem

    // Flapper 2 opens when IR Sensor 2 detects waste
    if (ir2_detected == HIGH) {
      nonPlasticServo.write(openAngle); // Open the second flapper
      delay(1000); // Keep it open for 1 second
      nonPlasticServo.write(closedAngle); // Close the flapper
    }
  }

  delay(200); // Debounce delay to avoid sensor glitches
}
