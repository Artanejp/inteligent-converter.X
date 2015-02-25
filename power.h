/*
 * OpenI2CRADIO
 * Power on managing.
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
#include <stdlib.h>
#include <string.h>
#ifdef __SDCC
#include <sdcc-lib.h>
#include <pic18fregs.h> /* ONLY FOR PIC18x */
#include <delay.h>
#else
#include <xc.h>
#endif
#include <signal.h>

#include "iodef.h"
#include "idle.h"
#include "i2c_io.h"
#include "akc6955.h"
#include "lcd_acm1602.h"
#include "ui.h"
#include "eeprom.h"
#include "ioports.h"
//#include "menu.h"

#ifndef POWER_H
#define	POWER_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef __SDCC
//#define _POWER_DELAY() delay1ktcy(2*8) // 2ms
#define _POWER_DELAY() idle_time_ms(2) // 2ms
#else
//#define _POWER_DELAY() __delay_ms(2)
#define _POWER_DELAY() idle_time_ms(2) // 2ms
#endif


    enum {
        RESET_POR = 0,
        RESET_BOR,
        RESET_MCLR,
        RESET_WDT,
        RESET_STACK_FULL,
        RESET_STACK_UNDER,
        RESET_SOFTWARE,
        RESET_POWERDOWN,
        RESET_INTEXIT
    };
    extern unsigned char chk_reset(void);
    extern void rfamp_power(unsigned char b);
    extern void power_on_inthook(void);
    extern void power_on(unsigned char f);
    extern unsigned char chk_powerbutton(void);
    extern void power_off(unsigned char save);
    extern void shutdown(unsigned char save);

#ifdef	__cplusplus
}
#endif

#endif	/* POWER_H */

