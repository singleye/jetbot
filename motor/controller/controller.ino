/*
 * Jetbot motor driver with Arduino
 *
 * Author: singleye_512@gmail.com
 * Date: 2019-04-30
 */

#include <Wire.h>

// Device address
#define I2C_ADDRESS       8

// PWM controll PINs
#define PWM_A_PIN         9
#define PWM_B_PIN         11

// Use the PWM pin as the register address mapping to each motor
#define MOTOR_A_REGISTER  PWM_A_PIN
#define MOTOR_B_REGISTER  PWM_B_PIN

#define BAUD_RATE         9600

// range finder
#define RF_TRIG_PIN       6
#define RF_ECHO_PIN       7
// 340m/s or 34000cm/s or 0.034cm/us
#define SPEED_OF_SOUND    0.034
#define RANGE_MAX         255
#define RANGE_MIN         20
// to speed up detection speed in unexpected conditions and to keep the maximum detection range of 340cm
#define RF_TIMEOUT        20000

#define ASYNC_DETECT

static int motorASpeed = 0, motorBSpeed = 0;
static int newSpeedA = 0, newSpeedB = 0;
static byte distance = 0;


long detectRange() {
  long duration = 0;
  long distance = 0;

  digitalWrite(RF_TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(RF_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(RF_TRIG_PIN, LOW);

  pinMode(RF_ECHO_PIN, INPUT);
  duration = pulseIn(RF_ECHO_PIN, HIGH, RF_TIMEOUT);
  distance = SPEED_OF_SOUND*(duration/2);

  if (distance < 0) {
    distance = 0;
  }
  if (distance > RANGE_MAX) {
    distance = RANGE_MAX;
  }

  return distance;
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int bytes) {
  Serial.println("----------------------------------------------");
  Serial.print("Data length: ");
  Serial.println(bytes);

  while (2 < Wire.available()) {        // loop through all but the last
    char c = Wire.read();               // receive byte as a character
    Serial.print(c);                    // print the character
  }

  int regAddr = Wire.read();
  if (regAddr == MOTOR_A_REGISTER) {
    newSpeedA = Wire.read();
    Serial.print("Receive speed [motor A]: ");
    Serial.println(newSpeedA);
  }else if (regAddr == MOTOR_B_REGISTER) {
    newSpeedB = Wire.read();
    Serial.print("Receive speed [motor B]: ");
    Serial.println(newSpeedB);
  }else {
    Wire.read();
  }
  Serial.println("----------------------------------------------");
  Serial.println("");
}

// function handle distance query
void requestEvent() {
#ifndef ASYNC_DETECT
  long r = detectRange();
  distance = r & 0xFF;
  Serial.print("Distance: ");
  Serial.println(distance);
#endif

  Wire.write(distance);
}

void setup() {
  // start serial for output
  Serial.begin(BAUD_RATE);

  // join i2c bus with address #8
  Wire.begin(I2C_ADDRESS);
  // register event
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.println("Initialize motors");
  analogWrite(PWM_A_PIN, motorASpeed);
  analogWrite(PWM_B_PIN, motorBSpeed);

  Serial.println("Initialize range finder");
  pinMode(RF_TRIG_PIN, OUTPUT);
  pinMode(RF_ECHO_PIN, INPUT);

  long r = detectRange();
  distance = r & 0xFF;
}

static int rangeDetectCounter = 0;

void loop() {
  if (motorASpeed != newSpeedA) {
    motorASpeed = newSpeedA;
    Serial.print("Setting motor A speed: ");
    Serial.println(motorASpeed);
    analogWrite(PWM_A_PIN, motorASpeed);
  }

  if (motorBSpeed != newSpeedB) {
    motorBSpeed = newSpeedB;
    Serial.print("Setting motor B speed: ");
    Serial.println(motorBSpeed);
    analogWrite(PWM_B_PIN, motorBSpeed);
  }

#ifdef ASYNC_DETECT
  long r = detectRange();
  distance = r & 0xFF;
#endif

  delay(100);
}
