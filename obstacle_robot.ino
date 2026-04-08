#include <Servo.h>

/* ---------------- Pin Definitions ---------------- */

// Motor Driver (L298N)
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 5
#define ENB 6

// Ultrasonic Sensor
#define TRIG_PIN 2
#define ECHO_PIN 3

// Servo Motor
#define SERVO_PIN 7

/* ---------------- Constants ---------------- */

#define OBSTACLE_DISTANCE 20   // cm threshold
#define MOTOR_SPEED 180        // PWM speed (0–255)

/* ---------------- Objects ---------------- */

Servo scanServo;

/* ---------------- Global Variables ---------------- */

long duration;
int distance;

/* ---------------- Function Prototypes ---------------- */

int measureDistance();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
void setMotorSpeed(int speed);

/* ---------------- Setup ---------------- */

void setup() {
  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo setup
  scanServo.attach(SERVO_PIN);
  scanServo.write(90); // center position

  // Set motor speed
  setMotorSpeed(MOTOR_SPEED);

  delay(1000);
}

/* ---------------- Main Loop ---------------- */

void loop() {

  int frontDistance = measureDistance();

  if (frontDistance > OBSTACLE_DISTANCE) {
    moveForward();
  } 
  else {
    stopMotors();
    delay(300);

    // Scan Right
    scanServo.write(30);
    delay(500);
    int rightDistance = measureDistance();

    // Scan Left
    scanServo.write(150);
    delay(500);
    int leftDistance = measureDistance();

    // Reset servo
    scanServo.write(90);
    delay(300);

    // Decision making
    if (rightDistance > leftDistance) {
      turnRight();
      delay(400);
    } else {
      turnLeft();
      delay(400);
    }
  }
}

/* ---------------- Functions ---------------- */

// Measure distance using HC-SR04
int measureDistance() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms

  // If no signal received
  if (duration == 0) {
    return 400; // max distance
  }

  distance = duration * 0.034 / 2;
  return distance;
}

// Motor control functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Control motor speed using PWM
void setMotorSpeed(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}
