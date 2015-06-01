/*
  I2CSerialIMU example
  by Paolo Bosetti 2015
*/

#include <Wire.h>
#include <I2CDevices.h>
#define BAUD 9600
#define PROMPT Serial.println(">");

//MyStepper left(0xA);
//MyStepper right(0xB);
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
  Serial.println("done.");
  PROMPT;
}

void loop() {
  accel.update();
  accel.describe();
  
  Serial.print(" - ");

  gyro.update();
  gyro.describe();

  Serial.print(" - ");

  magn.update();
  magn.describe();

  Serial.println();
  delay(100);
}