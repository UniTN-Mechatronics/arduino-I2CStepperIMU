/*
 * General I2C device simple command interface
 */
#ifndef I2CDEVICES_H
#define I2CDEVICES_H
#if ARDUINO >= 100
#include "Arduino.h" // for delayMicroseconds,digitalPinToBitMask, etc
#else
#include "WProgram.h"     // for delayMicroseconds
#include "pins_arduino.h" // for digitalPinToBitMask, etc
#endif

#include <Wire.h>

typedef union {
  byte      b[6];
  short int i[3];
  char      c[6];
} vec_data_t;

class I2CDevice {
  public:
    I2CDevice(int addr);
    void send_cmd(char cmd, char value);
    void send_cmd(char cmd);
    int addr() {
      return _addr;
    }
    void table(unsigned short n);
  protected:
    int _addr;
};

class I2CStepper : public I2CDevice {
  public:
    I2CStepper(int addr);
    void set_speed(char speed);
    void speed_ctrl(bool enable);
    void speed_ctrl(bool enable, byte val);
    void speed_ctrl(bool enable, double val);
    void set_zero();
    long position();
    void step(int steps, byte pwr);
    void step(int steps, double pwr);
  private:
    int _addr;
};

class I2CAccel : public I2CDevice {
  public:
    I2CAccel(int addr);
    I2CAccel();
    void init();
    void update();
    short int x();
    short int y();
    short int z();
    double x_g();
    double y_g();
    double z_g();
    double a();
    double a_g();
    double calibrate(int n_sample, int dly);
    void set_cal_factor(double val) {
      _cal_factor = val;
    }
    void describe();
  private:
    double _cal_factor;

    vec_data_t _data;

};

class I2CGyro : public I2CDevice {
  public:
    I2CGyro(int addr);
    I2CGyro();
    void init();
    void update();
    short int temp() {
      return _temp.i;
    };
    float temp_c();
    double x_ds();
    double y_ds();
    double z_ds();
    short int x();
    short int y();
    short int z();
    void calibrate(int n_sample, int dly);
    void set_cal_factor(double val) {
      _cal_factor = val;
    }
    double cal_factor() {
      return _cal_factor;
    };
    void describe();
    void describe(int mod);
  private:
    double _cal_factor;
    short int _offset[3];
    vec_data_t _data;
    union temp_data {
      short int i;
      byte c[2];
    } _temp;
};

class I2CMagneto : public I2CDevice {
  public:
    I2CMagneto(int addr);
    I2CMagneto();
    void init();
    void update();
    short int x();
    short int y();
    short int z();
    void set_cal_factor(double val) {
      _cal_factor = val;
    }
    double cal_factor() {
      return _cal_factor;
    };
    void describe();
    void describe(int mod);
  private:
    double _cal_factor;
    vec_data_t _data;
};

#endif