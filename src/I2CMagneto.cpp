/*
 * Magneto class
 */
#import "I2CDevices.h"

I2CMagneto::I2CMagneto(int addr) : I2CDevice(addr) {
  _data = {0, 0, 0};
  _north = 0.0;
}

I2CMagneto::I2CMagneto() : I2CDevice(0x1E) {
  _data = {0, 0, 0};
  _north = 0.0;
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

void I2CMagneto::get_xyz(double coord[3]) {
  coord[0] = double(x());
  coord[1] = double(y());
  coord[2] = double(z());
}

// Radial coord
double I2CMagneto::r() {
  return sqrt(pow(x(), 2) + pow(y(), 2) + pow(z(), 2));
}

// Azimuthal coord
double I2CMagneto::theta() {
  return atan2(double(y()), double(x())) - _north;
}

// Polar coord
double I2CMagneto::phi() {
  return acos(double(z()) / double(r()));
}

void I2CMagneto::get_spherical(double coord[3]) {
  coord[0] = r();
  coord[1] = atan2(y(), x());
  coord[2] = acos(z()/coord[0]);
}

void I2CMagneto::calibrate(int n_sample, int dly) {
  _north = 0.0;
  for (int i = 0; i < n_sample; i++) {
    update();
    _north += atan2(double(y()), double(x()));
    delay(dly);
  }
  _north /= double(n_sample);
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
