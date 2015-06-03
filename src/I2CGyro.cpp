/*
 * Gyro class
 */
#import "I2CDevices.h"
#define DEG_TO_RAD 0.01745329278

I2CGyro::I2CGyro(int addr) : I2CDevice(addr) {
  _data = {0, 0, 0};
}

I2CGyro::I2CGyro() : I2CDevice(0x68) {
  _data = {0, 0, 0};
  _offset[0] = _offset[1] = _offset[2] = 0;
  _temp.i = 0;
  _cal_factor = 0.06956521739;
}


void I2CGyro::init() {
  send_cmd(0x3E, 0x80); // Reset default
  send_cmd(0x16, 0x19); // Low Pass filter at 188Hz
  send_cmd(0x15, 0x04); // 1kHz / (n+1) = f
  send_cmd(0x3E, 0x01); // select X Gyro for clock reference
}

void I2CGyro::update() {
  // Request for data
  send_cmd(0x1B); // Temperature
  Wire.requestFrom(_addr, 8);
  _temp.c[1] = (byte)Wire.read();
  _temp.c[0] = (byte)Wire.read();

  _data.b[1] = (byte)Wire.read();
  _data.b[0] = (byte)Wire.read();

  _data.b[3] = (byte)Wire.read();
  _data.b[2] = (byte)Wire.read();

  _data.b[5] = (byte)Wire.read();
  _data.b[4] = (byte)Wire.read();
}

float I2CGyro::temp_c() {
  return (_temp.i + 23300) / 280.0;
}

short int I2CGyro::x() {
  return - (_data.i[0] - _offset[0]);
}

short int I2CGyro::y() {
  return - (_data.i[1] - _offset[1]);
}

short int I2CGyro::z() {
  return _data.i[2] - _offset[2];
}

double I2CGyro::x_ds() {
  return x() * _cal_factor;
}

double I2CGyro::y_ds() {
  return y() * _cal_factor;
}

double I2CGyro::z_ds() {
  return z() * _cal_factor;
}

double I2CGyro::x_rs() {
  return x_ds() * DEG_TO_RAD;
}

double I2CGyro::y_rs() {
  return y() * DEG_TO_RAD;
}

double I2CGyro::z_rs() {
  return z() * DEG_TO_RAD;
}


void I2CGyro::calibrate(int n_sample, int dly) {
  double cal = 0.0;
  for (int i = 0; i < n_sample; i++) {
    update();
    _offset[0] += _data.i[0];
    _offset[1] += _data.i[1];
    _offset[2] += _data.i[2];
    delay(dly);
  }
  _offset[0] /= double(n_sample);
  _offset[1] /= double(n_sample);
  _offset[2] /= double(n_sample);
}

void I2CGyro::describe() {
  Serial.print("GYRO@0x");
  Serial.print(_addr, HEX);
  Serial.print(" T: ");
  Serial.print(temp_c());
  Serial.print(" x: ");
  Serial.print(x_ds());
  Serial.print(" y: ");
  Serial.print(y_ds());
  Serial.print(" z: ");
  Serial.print(z_ds());
}

void I2CGyro::describe(int mod) {
  Serial.print("GYRO@0x");
  Serial.print(_addr, HEX);
  Serial.print(" T: ");
  Serial.print(_temp.c[0], mod);
  Serial.print("|");
  Serial.print(_temp.c[1], mod);
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
