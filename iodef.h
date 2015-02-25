/*
 * OpenI2CRADIO
 * I/O Port defines / Main.
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

#ifndef IODEF_H
#define	IODEF_H

#ifdef __SDCC
#else
#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

#endif
#include "commondef.h"

#define _I2C_IO_ONE_MSSP
/*
 * LCD(ACM1602 Type)
 */
// If you use I2C-Type ACM1602, You should define, otherwise #undef.
#undef _USE_I2C_1602
#define _LCD_IF_4BIT


#ifdef	__cplusplus
extern "C" {
#endif

#include "iodef_i2c.h"

/*
 * MAP OF IOPORT
 *
 * RE3: VPP (ICSP Programming Voltage) / MCLR, Dont use for I/O.
 *
 * PORTA/B/C USED basic.
 *
 * PORTA:
 *        RA0 : KO0
 *        RA1 : KO1
 *        RA2 : KO2
 *        RA3 : KO7
 *        RA4 : AM LAMP('1' = On)
 *        RA5:  FM LAMP('1) = On)
 *        RA6-RA7 : Used for CLOCK, connect to 8MHz resonator.
 *
 * PORTB:
 *        RB0 : KI0
 *        RB1 : KI1
 *        RB2 : KI2
 *        RB3 : KI3
 *        RB4 : Power Key
 *        RB5-RB7 : FOR ICSP
 *
 * PORTC:
 *        RC0 : RF AMP SEL bit0
 *        RC1 : LCD-Backlight
 *        RC2 : RF AMP SEL bit1
 *        RC3-RC4 : SCL/SDA
 *        RC5 : Reserved
 *        RC6-RC7 : USART
 *
 * In addition, usage of PARALLEL-LCD,You should use 40Pin(or greater) PIC.
 * ON 4BIT LCD I/F:
 *
 * PORTD:
 *        RD0: LCD RS
 *        RD1: LCD RW
 *        RD2: LCD EN
 *        RD3: POWER LAMP
 *        RD4-RD7: LCD DB4-DB7
 *
 * PORTE:
 *        RE0: External Power AMP
 *        RE1: Power to Radio-Chip.
 *        RE2: ADC(IN) to measure battery
 *        RE3: VPP (ICSP Programming Voltage) / MCLR
 */

#define TRIS_A_VAL   0b11000000
#define AN_A_VAL     0b00000000

#define TRIS_B_VAL   0b11111111 /* PORTB as INPUT.*/
#define AN_B_VAL     0b00000000

#define TRIS_C_VAL_O 0b10011000 /* FOR I2C-WRITE, RC7=INPUT */
#define TRIS_C_VAL_I  0b10011000 /* FOR I2C-WRITE, RC7=INPUT */
#define AN_C_VAL     0b00000000

#define TRIS_D_VAL   0b00001000 /* RD3="RTS IN" another = FOR OUTPUT, LCD(Optional) */
#ifdef _LCD_IF_4BIT
 #define TRIS_D_RVAL 0b11111000 /* RD3="RTS IN" FOR INPUT*/
#else
 #define TRIS_D_RVAL 0b11111111 /* RD3="RTS IN" FOR INPUT*/
#endif

#define TRIS_E_VAL   0b00000100 /* FOR OUTPUT, ADC, PSP=OFF */


#ifdef	__cplusplus
}
#endif

#endif	/* IODEF_H */

