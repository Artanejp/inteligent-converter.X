/*
 * OpenI2CRADIO
 * Idle routine.
 * Using timer0.
 * Copyright (C) 2013-06-11 K.Ohta <whatisthis.sowhat ai gmail.com>
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
#if defined(__SDCC)
#include <sdcc-lib.h>
#include <pic18fregs.h> /* ONLY FOR PIC18x */
#else
#include <xc.h>
#endif
#include <signal.h>
#include "iodef.h"


void idle_init(void)
{
    INTCONbits.GIE = 1;
    INTCONbits.PEIE =  1;
    PIR1bits.TMR1IF = 0;

   WDTCON = 1; // OK? WDT=Disabled.
}


void stop_idle(void)
{
    T1CONbits.TMR1ON = 0;
}


void idle(unsigned int initial)
{

   unsigned int i;
   WDTCONbits.SWDTEN = 0; // Lame WDT OFF.
   /* Enable IDLE */
   /* Set TMR0 for interrupt*/
   /* Pre-scaler: 1/2, PSA=1(ON), TOSE=0, T0CS=0(INTERNAL), T08BIT=0(16bit), TMR0ON=1(START) */
   /* 1Tick = 1/1000 ms*/
   T1CON = 0b00001000; // 1/2
//   T0CON = 0b00001010; // 1/8
   //OSCCONbits.IDLEN = 1; // PIC16F has no IDLEN on OSCCON.
   PIR1bits.TMR1IF = 1;
//   TMR0L = initial & 0xff;
//   TMR0H = initial >> 8; // Write order : L->H
   TMR1H = initial >> 8; // Write order : H->L
   TMR1L = initial & 0xff;
   T1CONbits.TMR1ON = 1; // Start
  do {
       Sleep();
//       i = TMR0H << 8 + TMR0L; // Check if IDLE-Timer was elapsed.
   } while(PIR1bits.TMR1IF == 0); // Dead area : 0-2.
   PIR1bits.TMR1IF = 0;
   WDTCONbits.SWDTEN = 1; // WDT ON.
}


void idle_time_ms(unsigned int ms)
{
    unsigned int tim;
    unsigned int upper;

    if(ms == 0) return;
    upper = (ms & 0xffc0) >> 6;
    tim = (ms   & 0x3f) * 1000; //
    tim = (65535 - tim) + 1; // tim = 65536 - tim;
    while(upper > 0) {
        idle(65535 - 64000 + 1); // Upper is 512ms
        upper--;
    }
    idle(tim);
}

void idle_time_62_5ms(void)
{
    // Tim = 1ms * 64 - 1ms - 0.5ms
    // Tim = 0.128ms * (488 + 2.2)
    //     =
    idle_time_ms(62); // 62ms
    idle(500); // 500us
}

void idle_time_35ms(void)
{
    // Tim = 35 / 0.128 = 273.44
    idle_time_ms(35);
}
