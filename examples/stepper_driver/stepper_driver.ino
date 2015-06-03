#include <EEPROM.h>
#include <Stepper.h>
#include <Wire.h>

#define VERSION 5
#define RESET_PIN 10
#define LED_PIN 4
#define PWM_PWR 180
#define SLAVE_ADDRESS 0x03
#define PWMA 3
#define PWMB 11
#define BRAKEA 9
#define BRAKEB 8
#define DIRA 12
#define DIRB 13
#define STEPS_PER_REV 200
#define BAUD 115200
#define PROMPT Serial.println(">");
#define MAX_SPEED 75
#define LOOP_TIME 50

Stepper myStepper(STEPS_PER_REV, DIRA, DIRB);

byte g_eeprom_addr = 0x10;

struct {
  int speed;
  union {
    long l;
    byte c[4];
  } pos;
  bool speed_ctrl;
  int delay;
  int pwr;
  int spwr;
} g_state;

// callback for received data
void receiveData(int byteCount) {
  static char cmd = 0;
  int val;
  while (Wire.available()) {
    val = Wire.read();
    if (cmd == 0) {
      cmd = (char)val;
    }
    else {
      command_parsing(cmd, val);
      cmd = 0;
    }
  }
}

// callback for sending data
void sendData() {
  Wire.write(g_state.pos.c, 4);
}


// Pulses-per-second:
float pps() {
  return g_state.speed * STEPS_PER_REV / 60.0;
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);;
  g_state.speed = 0;
  g_state.pos.l = 0;
  g_state.speed_ctrl = false;
  g_state.delay = LOOP_TIME;
  g_state.pwr = 0;
  g_state.spwr = PWM_PWR;

  //EEPROM storage of I2C address
  byte i2c_address = SLAVE_ADDRESS;
  pinMode(RESET_PIN, INPUT_PULLUP);
  if (digitalRead(RESET_PIN) == LOW) {
    EEPROM.put(g_eeprom_addr, i2c_address);
  }
  EEPROM.get(g_eeprom_addr, i2c_address);
  Wire.begin(i2c_address);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);

  Serial.begin(BAUD);
  Serial.flush();
  Serial.println("---");
  Serial.print(":name: ");
  Serial.println("stepper controller");
  Serial.print(":version: ");
  Serial.println(VERSION);
  Serial.print(":i2c_address: 0x");
  Serial.println(i2c_address, HEX);
  PROMPT;
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  process_serial();
  if (g_state.speed_ctrl && g_state.speed != 0) {
    int ns = (pps() * g_state.delay / 1000);
    if (ns == 0) {
      ns = abs(g_state.speed) / g_state.speed;
    }
    g_state.pos.l += ns;
    analogWrite(PWMA, g_state.spwr);
    analogWrite(PWMB, g_state.spwr);
    myStepper.setSpeed(abs(g_state.speed));
    myStepper.step(ns);
  }
  else {
    analogWrite(PWMA, g_state.pwr);
    analogWrite(PWMB, g_state.pwr);
    delay(g_state.delay);
  }
}
