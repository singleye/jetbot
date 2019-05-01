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


static int motorASpeed = 0, motorBSpeed = 0;
static int newSpeedA = 0, newSpeedB = 0;

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

void setup() {
  // start serial for output
  Serial.begin(BAUD_RATE);

  // join i2c bus with address #8
  Wire.begin(I2C_ADDRESS);
  // register event
  Wire.onReceive(receiveEvent);

  Serial.println("Initialize motors");
  analogWrite(PWM_A_PIN, motorASpeed);
  analogWrite(PWM_B_PIN, motorBSpeed);
}

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

  delay(10);
}
