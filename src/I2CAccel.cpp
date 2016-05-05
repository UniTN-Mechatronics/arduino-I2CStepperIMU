/*
 * Accel class
 */
#include "I2CDevices.h"

I2CAccel::I2CAccel(int addr) : I2CDevice(addr) {
  _cal_factor = double(250.0);
  _data = {0, 0, 0};
}

I2CAccel::I2CAccel() : I2CDevice(0x53) {
  _cal_factor = double(250.0);
  _data = {0, 0, 0};
}

void I2CAccel::init() {
  send_cmd(0x2D, 0x08);
  send_cmd(0x31, 0x08);
  send_cmd(0x2C, 0x0B);
  send_cmd(0x38, 0x80);
}

void I2CAccel::update() {
  send_cmd(0x32);
  Wire.requestFrom(_addr, 6);
  _data.b[0] = (byte)Wire.read();
  _data.b[1] = (byte)Wire.read();

  _data.b[2] = (byte)Wire.read();
  _data.b[3] = (byte)Wire.read();

  _data.b[4] = (byte)Wire.read();
  _data.b[5] = (byte)Wire.read();
}

short int I2CAccel::x() {
  return - _data.i[0];
}

short int I2CAccel::y() {
  return _data.i[1];
}

short int I2CAccel::z() {
  return _data.i[2];
}

double I2CAccel::x_g() {
  return x() / _cal_factor;
}

double I2CAccel::y_g() {
  return y() / _cal_factor;
}

double I2CAccel::z_g() {
  return z() / _cal_factor;
}

double I2CAccel::a() {
  return sqrt(pow(x(), 2) + pow(y(), 2) + pow(z(), 2));
}

double I2CAccel::a_g() {
  return a() / _cal_factor;
}

double I2CAccel::calibrate(int n_sample, int dly) {
  double cal = 0.0;
  for (int i = 0; i < n_sample; i++) {
    update();
    cal += a();
    delay(dly);
  }
  _cal_factor = cal / double(n_sample);
  return _cal_factor;
}

void I2CAccel::describe() {
  Serial.print("ACC@0x");
  Serial.print(_addr, HEX);

  Serial.print(" ax: ");
  Serial.print(x_g());
  Serial.print(" ay: ");
  Serial.print(y_g());
  Serial.print(" az: ");
  Serial.print(z_g());

  Serial.print(" a: ");
  Serial.print(a());
  Serial.print(" a_g: ");
  Serial.print(a_g());
}
