/*
  I2CSerialIMU example
  by Paolo Bosetti 2015
*/

#include <Wire.h>
#include <I2CDevices.h>
#include <FiniteStateMachine.h>

#define BAUD 9600
#define PROMPT Serial.println(">");

State Idle = State(wait_state);
State Run  = State(measure_state); 
State Calibrate  = State(calibrate_state); 
FSM StateMachine = FSM(Calibrate);

I2CAccel accel  = I2CAccel(); // default 0x53
I2CGyro  gyro   = I2CGyro();  // default 0x68
I2CMagneto magn = I2CMagneto();


void setup() {
  Serial.begin(BAUD);
  pinMode(13, OUTPUT);
  
  accel.init();
  gyro.init();
  magn.init();
}

void loop() {
  static unsigned int v = 0;
  char ch;
  if (Serial.available()) {
    ch = Serial.read();
    switch (ch) {
      case 'R': StateMachine.transitionTo(Run); break;
      case 'I': StateMachine.transitionTo(Idle); break;
      case 'C': StateMachine.transitionTo(Calibrate); break;
    }
  }
  
  StateMachine.update();
  delay(50);
}

void wait_state() {
  digitalWrite(13, LOW);
  delay(100);
}

void calibrate_state() {
  Serial.print("Calibration...");
  accel.calibrate(50, 5);
  gyro.calibrate(50, 5);
  magn.calibrate(50, 5);
  Serial.println("done.");
  PROMPT;
  StateMachine.transitionTo(Idle);
}

void measure_state() {
  digitalWrite(13, HIGH);
  accel.update();
  gyro.update();
  magn.update();

  Serial.print((magn.theta() - magn.north())/ PI * 180.0);
  Serial.print("+");
  Serial.print(magn.north() / PI * 180.0);
  Serial.print(" ");

  accel.describe();
  Serial.print(" - ");
  gyro.describe();
  Serial.print(" - ");
  magn.describe();

  PROMPT;
}