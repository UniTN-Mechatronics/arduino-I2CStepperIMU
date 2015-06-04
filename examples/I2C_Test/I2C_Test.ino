/*
  I2CSerialIMU example
  by Paolo Bosetti 2015
*/

#include <Wire.h>
#include <I2CDevices.h>
#define BAUD 9600
#define PROMPT Serial.println(">");


I2CAccel accel  = I2CAccel(); // default 0x53
I2CGyro  gyro   = I2CGyro();  // default 0x68
I2CMagneto magn = I2CMagneto();


void setup() {
  Serial.begin(BAUD);
  
  accel.init();
  gyro.init();
  magn.init();

  Serial.print("Calibration...");
  accel.calibrate(50, 2);
  gyro.calibrate(50, 2);
  magn.calibrate(50, 2);
  Serial.println("done.");
  PROMPT;
}

void loop() {
  accel.update();
  gyro.update();
  magn.update();

  Serial.print(magn.theta() / PI * 180.0);
  Serial.print("+");
  Serial.print(magn.north() / PI * 180.0);
  Serial.print(" ");

  accel.describe();
  Serial.print(" - ");
  gyro.describe();
  Serial.print(" - ");
  magn.describe();

  Serial.println();
  delay(100);
}