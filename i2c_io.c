/*
 * OpenI2CRADIO
 * I2C Handler
 * Copyright (C) 2013-06-10 K.Ohta <whatisthis.sowhat ai gmail.com>
 * License: GPL2+LE
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2,
 *  or (at your option) any later version.
 *  This library / program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; see the file COPYING. If not, write to the
 *  Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
 *
 *  As a special exception, if you link this(includeed from sdcc) library
 *  with other files, some of which are compiled with SDCC,
 *  to produce an executable, this library does not by itself cause
 *  the resulting executable to be covered by the GNU General Public License.
 *  This exception does not however invalidate any other reasons why
 *  the executable file might be covered by the GNU General Public License.
 */

#include <stdarg.h>
#include <stdio.h>
#ifdef __SDCC
#include <delay.h>
#include <pic18fregs.h>
#endif

#include "i2c_io.h"



#ifdef _I2C_IO_TWO_MSSPS
void i2c1_init(void)
{
    unsigned char b;
//    b = SSP1CON1;
    b = _SSPEN | _SSPM3;
    SSP1CON1 = b;
    SSP1ADD = 19; // Fosc:8000[KHz] / (4 * I2C:Clock:100[KHz]) - 1
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 0;
    IPR1bits.SSPIP = 1;
}
void i2c2_init(void)
{
    unsigned char b;
    b =  _SSPEN | _SSPM3;
    SSP2CON1 = b;
    SSP2ADD = 4; // Fosc:8000[KHz] / (4 * I2C:Clock:400[KHz]) - 1
}
#else
 #ifdef _I2C_IO_ONE_MSSP
void i2c1_init(void)
{
 //   unsigned char b;
//    b = _SSPEN | _SSPM3;
 //   SSPCON1bits.SSPEN = 1;
//    SSPCON1bits.SSPM3 = 1;
//    SSPADD = 4; // Fosc:8000[KHz] / (4 * I2C:Clock:400[KHz]) - 1
//    SSPSTAT = 0x80;
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 0;
//    IPR1bits.SSPIP = 1; // PIC16F HAS no IPR.
}
void i2c2_init(void)
{
    i2c1_init();
}

 #else // None MSSP, Software I2C
void i2c1_init(void)
{
}
void i2c2_init(void)
{
}
 #endif
// I2C_IO
void i2c_send_byte(unsigned char addr, unsigned char reg, unsigned char data)
{
#ifdef __SDCC
    i2c_open(I2C_MASTER, I2C_SLEW_ON, 5);
    I2C_START();
    i2c_writechar(addr);
    i2c_writechar(reg);
    i2c_writechar(data);
    I2C_STOP();
    i2c_close();
//    delay100tcy(2);
#else
    OpenI2C(MASTER, SLEW_ON);
    SSPADD = 0x5;
//    IdleI2C();
    StartI2C();
    while (SSPCON2bits.SEN);
    WriteI2C(addr);
    while(SSPCON2bits.SEN);
    WriteI2C(reg);
    while(SSPCON2bits.SEN);
    WriteI2C(data);
    StopI2C();
    while (SSPCON2bits.PEN);
    CloseI2C();
#endif  //    i2c_idle();
}

unsigned char i2c_read_byte(unsigned char addr, unsigned char reg)
{
    unsigned char c;
  #ifdef __SDCC
    i2c_open(I2C_MASTER, I2C_SLEW_ON, 5);
    I2C_START();
    i2c_writechar(addr);
    i2c_writechar(reg);
    I2C_STOP();
    i2c_idle();
//   delay100tcy(2);
    I2C_START();
    i2c_writechar(addr | 1);
    c = i2c_readchar();
    I2C_ACK();
    I2C_STOP();
    i2c_close();
#else
    OpenI2C(MASTER, SLEW_ON);
    SSPADD = 0x5;
    StartI2C();
    while(SSPCON2bits.SEN);
    WriteI2C(addr);
  //  delay1ktcy(8);
    while(SSPCON2bits.SEN);
    WriteI2C(reg);
  //  delay1ktcy(8);
    StopI2C();
    IdleI2C();
    StartI2C();
    WriteI2C(addr | 1);
    if (!SSPCON2bits.ACKSTAT){
      SSPCON2bits.RCEN = 1;
      while(SSPCON2bits.RCEN);
      NotAckI2C();
      while (SSPCON2bits.ACKEN);
      StopI2C();
      while (SSPCON2bits.PEN);
    }
    c = SSPBUF;
  //  delay1ktcy(8);
 //   delay1ktcy(8);
    CloseI2C();
#endif
    //    CLOSEASMASTER();
    return c;

}

#endif
