/*
 * General I2C device simple command interface
 */
#import "I2CDevices.h"


I2CDevice::I2CDevice(int addr) {
  _addr = addr;
  _cmd_delay = 0;
  Wire.begin();
}

void I2CDevice::send_cmd(char cmd, char value) {
  Wire.beginTransmission(_addr);
  Wire.write(cmd);
  Wire.write(value);
  Wire.endTransmission();
  if (_cmd_delay > 0)
    delay(_cmd_delay);
}

void I2CDevice::send_cmd(char cmd) {
  Wire.beginTransmission(_addr);
  Wire.write(cmd);
  Wire.endTransmission();
  if (_cmd_delay > 0)
    delay(_cmd_delay);
}

void I2CDevice::table(unsigned short n = 256) {
  byte b;
  send_cmd(0x00);
  Wire.requestFrom(_addr, n);
  for (unsigned short i = 0; i < n; i++) {
    if (i % 16 == 0) {
      Serial.println();
      Serial.print(i, HEX);
      Serial.print(": ");
    }
    b = (byte)Wire.read();
    Serial.print(" ");
    Serial.print((byte)b, HEX);
  }
  Serial.println();
}


/*
 * Stepper motor class
 */
I2CStepper::I2CStepper(int addr) : I2CDevice(addr) { }

void I2CStepper::set_speed(char speed) {
  char sign = (speed >= 0 ? 'S' : 's');
  send_cmd(sign, abs(speed));
}

void I2CStepper::set_speed(double speed) {
  char sign = (speed >= 0 ? 'S' : 's');
  send_cmd(sign, abs(char(speed)));
}

void I2CStepper::speed_ctrl(bool enable, byte val) {
  send_cmd(enable ? 'R' : 'r', val);
}

void I2CStepper::speed_ctrl(bool enable, double val) {
  speed_ctrl(enable, (byte)map(val, 0, 100, 0, 180)) ;
}

void I2CStepper::speed_ctrl(bool enable) {
  speed_ctrl(enable, byte(0));
}

void I2CStepper::set_zero() {
  send_cmd('z');
}

long I2CStepper::position() {
  union {
    long l;
    char b[4];
  } data;

  Wire.requestFrom((int)_addr, 4);
  for (int i = 0; i < 4; i++) {
    data.b[i] = Wire.read();
  }
  //  int i = 0;
  //  while (Wire.available()) {
  //    data.b[i++] = Wire.read();
  //  }
  return data.l;
}

void I2CStepper::step(int steps, double pwr) {
  send_cmd('W', map(pwr, 0, 100, 0, 180));
  send_cmd(steps > 0 ? '+' : '-', abs(steps));
  send_cmd('w');
}

void I2CStepper::step(int steps, byte pwr) {
  send_cmd('W', constrain(pwr, 0, 180));
  send_cmd(steps > 0 ? '+' : '-', abs(steps));
  send_cmd('w');
}









