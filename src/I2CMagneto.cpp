/*
 * Magneto class
 */
#import "I2CDevices.h"

I2CMagneto::I2CMagneto(int addr) : I2CDevice(addr) {
  _data = {0, 0, 0};
}

I2CMagneto::I2CMagneto() : I2CDevice(0x1E) {
  _data = {0, 0, 0};
}


void I2CMagneto::init() {
  send_cmd(0x02, 0x00);
  send_cmd(0x00, 0b00011000);
}

void I2CMagneto::update() {
  // Request for data
  send_cmd(0x03); // Temperature
  Wire.requestFrom(_addr, 6);
  _data.b[1] = (byte)Wire.read();
  _data.b[0] = (byte)Wire.read();

  _data.b[3] = (byte)Wire.read();
  _data.b[2] = (byte)Wire.read();

  _data.b[5] = (byte)Wire.read();
  _data.b[4] = (byte)Wire.read();
}

short int I2CMagneto::x() {
  return _data.i[0];
}

short int I2CMagneto::y() {
  return _data.i[2];
}

short int I2CMagneto::z() {
  return - _data.i[1];
}

void I2CMagneto::describe() {
  Serial.print(" MAGNETO@0x");
  Serial.print(_addr, HEX);
  Serial.print(" x: ");
  Serial.print(x());
  Serial.print(" y: ");
  Serial.print(y());
  Serial.print(" z: ");
  Serial.print(z());
}

void I2CMagneto::describe(int mod) {
  Serial.print(" MAGNETO@0x");
  Serial.print(_addr, HEX);
  Serial.print(" x: ");
  Serial.print(_data.b[0], mod);
  Serial.print("|");
  Serial.print(_data.b[1], mod);
  Serial.print(" y: ");
  Serial.print(_data.b[2], mod);
  Serial.print("|");
  Serial.print(_data.b[3], mod);
  Serial.print(" z: ");
  Serial.print(_data.b[4], mod);
  Serial.print("|");
  Serial.print(_data.b[5], mod);
}
