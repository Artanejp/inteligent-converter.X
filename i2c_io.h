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
#ifdef __SDCC
#include <sdcc-lib.h>
#include <pic18fregs.h> /* ONLY FOR PIC18x */
#else
#include <xc.h>
#endif

#include <plib/i2c.h>
#include "iodef.h"

#ifndef I2C_IO_H
#define	I2C_IO_H

#ifdef	__cplusplus
extern "C" {
#endif
/*
 * #1
 */
#define I2C_CLOCK_PARAM 4 // 8MHz/(4*(1+I2C_CLOCK_PARAM)) -> 400KHz
//#define I2C_CLOCK_PARAM 15 // 8MHz/(4*(1+I2C_CLOCK_PARAM)) -> 100KHz
#define I2C_DEFAULT_MODE I2C_MASTER
#define I2C_SLEW_DEFAULT I2C_SLEW_OFF

#define OPENASMASTER() i2c_open(I2C_DEFAULT_MODE, I2C_SLEW_DEFAULT, I2C_CLOCK_PARAM)
#define CLOSEASMASTER() i2c_close()
extern void i2c1_init(void);
extern void i2c2_init(void);
extern void i2c_send_byte(unsigned char addr, unsigned char reg, unsigned char data);
extern unsigned char i2c_read_byte(unsigned char addr, unsigned char reg);


#ifdef	__cplusplus
}
#endif

#endif	/* I2C_IO_H */

