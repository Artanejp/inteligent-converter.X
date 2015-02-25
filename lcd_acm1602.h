/*
 * OpenI2CRADIO
 * I2C/Parallel LCD ACM1602 Handler, variant of ST7066U.
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
#ifndef LCD_ACM1602_H
#define	LCD_ACM1602_H

#include <stdarg.h>
#include <stdio.h>
#ifdef __SDCC
#include <delay.h>
#else
#include <xc.h>
#endif

#include <string.h>

#include "iodef.h"
#include "i2c_io.h"
#include "idle.h"


#ifdef	__cplusplus
extern "C" {
#endif

#ifndef LCD_I2CADDR
#define LCD_I2CADDR 0xa0
#endif
// Notes: Master clock = 8MHz(0.125uS)
// Wait 5us
#ifdef __SDCC
//#define _ACM1602_TC_WAIT()     delay10tcy(4)
// Wait 50us
#define _ACM1602_SHORT_WAIT()     delay100tcy(4)
// Wait 5ms
#define _ACM1602_LONG_WAIT()     idle_time_ms(50)
// Wait 200ms
//#define _ACM1602_LONG_LONG_WAIT()     delay100ktcy(10)
#define _ACM1602_LONG_LONG_WAIT()     idle_time_ms(125)
// Wait 5.5ms
#define _ACM1602_I2C_WAIT()     idle_time_ms(6)
#else
//#define _ACM1602_TC_WAIT()     __delay_us(5)
// Wait 50us
#define _ACM1602_SHORT_WAIT()     __delay_us(50)
// Wait 5ms
#define _ACM1602_LONG_WAIT()     idle_time_ms(50)
// Wait 125ms
#define _ACM1602_LONG_LONG_WAIT()     idle_time_ms(125)
// Wait 5.5ms
#define _ACM1602_I2C_WAIT()     idle_time_ms(6)
#endif


extern void acm1602_cls(unsigned char addr);
extern void acm1602_putchar(unsigned char addr, unsigned char c);
extern void acm1602_locate_16x2(unsigned char addr, unsigned char x, unsigned char y);
extern void acm1602_locate_8x2(unsigned char addr, unsigned char x, unsigned char y);
extern void acm1602_home(unsigned char addr);
extern void acm1602_cursordir(unsigned char addr, unsigned char right);
extern void acm1602_dispcursor(unsigned char addr, unsigned char flag);
extern void acm1602_init(unsigned char addr, unsigned char cls);
extern void acm1602_suspend(unsigned char addr);
extern void acm1602_resume(unsigned char addr);

//extern unsigned char acm1602_getchar(unsigned char addr);

#ifdef _USE_I2C_1602
/*
 * ST7032 extension functions
 */
extern void st7032_extcmd(unsigned char addr, unsigned char mode,
        unsigned char cmd, unsigned char busyf);
extern void st7032_setcontrast(unsigned char addr, unsigned char value,
        unsigned char busyf);
#endif
#ifdef	__cplusplus
}
#endif

#endif	/* LCD_ACM1602_H */

