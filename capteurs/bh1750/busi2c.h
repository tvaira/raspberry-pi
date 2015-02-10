#ifndef BUSI2C_H
#define BUSI2C_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// cf. Makefile
//#define I2C_DEBUG

#define MIN_I2C_BUS 0
#define MAX_I2C_BUS 7

#define MAX_WRITE_LEN 511

// #define i2c_write	linux_i2c_write
// #define i2c_read	linux_i2c_read
// #define delay_ms	linux_delay_ms
// #define get_ms		linux_get_ms
#define log_i		printf
#define log_e		printf
#define min(a, b) 	((a < b) ? a : b)

class BusI2C
{
   private:      
      int i2c_bus;// = 1 (default on RPI B)
      int i2c_fd;
      int current_slave;
      unsigned char txBuff[MAX_WRITE_LEN + 1];
      
   public:
      BusI2C(int i2c_bus=1);
      int open();
      void close();
      int select_slave(unsigned char slave_addr);      
      void set_i2c_bus(int bus);
      int write(unsigned char slave_addr, unsigned char reg_addr,
       unsigned char length, unsigned char const *data);
      int read(unsigned char slave_addr, unsigned char reg_addr,
       unsigned char length, unsigned char *data); 
      int read(unsigned char slave_addr, unsigned char length, unsigned char *data);
      int delay_ms(unsigned long num_ms);
      int get_ms(unsigned long *count);
};

#endif 

