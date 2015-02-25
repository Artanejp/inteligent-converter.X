/*
 * OpenI2CRADIO
 * Power switch.
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

#include "commondef.h"
#include "lcd_acm1602.h"
#include "power.h"
#include "menu.h"
#include "backlight.h"

/*
 * Detect reset condition.
 */
unsigned char chk_reset(void)
{

#ifdef __SDCC
    union {
        __RCONbits_t b;
        unsigned char byte;
    } rcon;
    union {
        __STKPTRbits_t b;
        unsigned char byte;
    } stkptr;
#else
    union {
        RCONbits_t b;
        unsigned char byte;
    } rcon;
    union {
        STKPTRbits_t b;
        unsigned char byte;
    } stkptr;
#endif
    rcon.byte = RCON;
    stkptr.byte = STKPTR;
    if(rcon.b.POR ==0) {
        RCONbits.POR = 1;
        return RESET_POR;
    }

    if(rcon.b.BOR == 0) {
        RCONbits.BOR = 1;
        return RESET_BOR;
    }
    if(rcon.b.RI == 0) {
        RCONbits.RI = 1;
        return RESET_SOFTWARE;
    }
    if(rcon.b.TO == 0) return RESET_WDT;
    if(rcon.b.PD == 0) return RESET_POWERDOWN;
    if(stkptr.b.STKOVF) return RESET_STACK_FULL;
    if(stkptr.b.STKUNF) return RESET_STACK_UNDER;
    return RESET_MCLR;
}

void power_on_inthook(void)
{

    IOCB = 0x00;
//    IOCBbits.IOCB4 = 1;
    INTCONbits.RBIE = 0;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}

void power_on(unsigned char f)
{
    if(f == 0x00){
        set_radiopower(0x00);
//        idle_time_ms(200);
        set_examp_mute(0xff);
    } else {
        set_examp_mute(0x00);
        idle_time_ms(100);
        set_radiopower(0xff);
    }
}

void rfamp_power(unsigned char b)
{
    __bitops_t bx;
    
    bx.byte = b;
    if(bx.b0) {
        _RFAMP_SEL_B0 = 1;
    } else {
        _RFAMP_SEL_B0 = 0;
    }
    if(bx.b1) {
        _RFAMP_SEL_B1 = 1;
    } else {
        _RFAMP_SEL_B1 = 0;
    }

}

unsigned char chk_powerbutton(void)
{
    unsigned char count = 0;
    char i;

    TRISBbits.RB4 = 1; // Set Input
    for(i = 0; i < 24; i++) {  // 2*24=48ms.
        if(PORTBbits.RB4) count++;
        _POWER_DELAY();
        ClrWdt();
    }
    if(count <= 12) { // 24ms
        return 0xff; // Pressed
    }
    return 0; // Not pressed
}

void shutdown(unsigned char save_v)
{
    rfamp_power(RFAMP_OFF);
    power_on(0);
    acm1602_suspend(LCD_I2CADDR);
    lcd_setbacklight(0, 0);
    power_off(save_v);
}

void power_off(unsigned char save_v)
{
    unsigned char sts;

    if(save_v != 0) {
        save_eeprom();
//        save_frequencies();
    }
    _AM_STATLAMP = 0;
    _FM_STATLAMP = 0;
    set_powerlamp(0);

    WDTCONbits.SWDTEN = 0; // Lame WDT OFF.
    sts = 0;
    do {
        RCONbits.IPEN = 1;
        IOCB = 0x10; // IOCB4 ONLY.
        OSCCONbits.IDLEN = 0; // Not Idle.
        INTCON2bits.RBIP = 1; // Priority = HIGH;
        INTCON = 0b00001000;
        T0CONbits.TMR0ON = 0;
        Sleep();
        // Wake up
        if(INTCONbits.RBIF) {
                INTCONbits.RBIF = 0;
                idle_init();
                sts = chk_powerbutton();
          //      sts = 0xff;
        }
    } while(sts == 0);
    
    INTCONbits.RBIE = 0;
    INTCONbits.RBIF = 0;
    // Button pressed, software reset.
    Reset();
}