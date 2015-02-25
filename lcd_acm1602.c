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

#include "iodef.h"
#include "lcd_acm1602.h"

#ifdef _USE_I2C_1602
static unsigned char lcd_busychk(addr)
{
    unsigned char b, d;
    i2c_idle();
    OPENASMASTER();
    i2c_writechar(addr | 0x01); // Read
    i2c_writechar(0x00);
    d = i2c_readchar();
    CLOSEASMASTER();
    _ACM1602_I2C_WAIT(); // 5.5ms
    if((d & _LCDPORT_BUSYMASK) != 0){
        d = 0xff; // BUSY
    } else {
        d = 0;
    }
    return d;
}

void lcd_waitbusy(unsigned char addr, unsigned char flag)
{
    unsigned char f,ff;
    ff = flag;
    if(ff != 0) ff = 0xff;
    do {
        f = lcd_busychk(addr);
        _ACM1602_SHORT_WAIT(); // Wait 0.1ms
    } while(f != ff);
}


static void sendcmd(unsigned char addr, unsigned char cmd, unsigned char busyflag)
{
    i2c_idle();
    OPENASMASTER();
    i2c_writechar(addr & 0xfe);
    i2c_writechar(0x00);
    i2c_writechar(cmd);
    CLOSEASMASTER();
    _ACM1602_I2C_WAIT(); // 5.5ms
}

static void sendonce(unsigned char addr, unsigned char cmd)
{
    sendcmd(addr, cmd, 0x00);
}

static void acm1602_ioinit(unsigned char addr)
{
    _ACM1602_LONG_LONG_WAIT();
    return;
}

void acm1602_putchar(unsigned char addr, unsigned char c)
{
    i2c_idle();
    OPENASMASTER();
    i2c_writechar(addr & 0xfe);
    i2c_writechar(0x80); //Data
    i2c_writechar(c); // Putchar
    CLOSEASMASTER();
    _ACM1602_I2C_WAIT(); // Wait 50us
}


unsigned char acm1602_getchar(unsigned char addr)
{
    unsigned char d;
    i2c_idle();
    OPENASMASTER();
    i2c_writechar(addr | 0x01); // Read
    i2c_writechar(0x80);
    d = i2c_readchar();
    CLOSEASMASTER();
    _ACM1602_I2C_WAIT(); // 5.5ms
    return d;
}
#else // Parallel
static void _ACM1602_TC_WAIT(void)
{
#ifdef __SDCC
   delay10tcy(4);
#else
   __delay_us(5);
#endif
}

static unsigned char lcd_busychk(unsigned char addr)
{
    unsigned char b, d;

    // Set mask for Tristate.
    b = _LCDPORT_TRIS_DATA;
    b |= _LCDPORT_READMASK;// Set mask for Tristate, direction = read.
    _LCDPORT_TRIS_DATA = b;

//    b = _LCDPORT_CONT_LATCH;
    _LCDPORT_CONT_RS = 0;
    _LCDPORT_CONT_RW = 1;
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 1; // Send CMD
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    d = _LCDPORT_DATA;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 0; // Disable EN
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 1; // Disable EN
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 0;; // Disable EN
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    b = _LCDPORT_TRIS_DATA;
    b &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
    _LCDPORT_TRIS_DATA = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    if((d & _LCDPORT_BUSYMASK) != 0){
        d = 0xff; // BUSY
    } else {
        d = 0;
    }
    return d;
}

void lcd_waitbusy(unsigned char addr, unsigned char flag)
{
    unsigned char f,ff;
    ff = flag;
    if(ff != 0) ff = 0xff;
    do {
        f = lcd_busychk(addr);
        _ACM1602_SHORT_WAIT(); // Wait 0.1ms
    } while(f != ff);
}

static void sendcmd(unsigned char addr, unsigned char cmd,unsigned char busyflag)
{
#ifdef _LCD_IF_4BIT
    unsigned char b, d;
    unsigned char h,l;

    h = cmd & 0xf0; // Higher bit is used.
    l = (cmd & 0x0f) << 4; // Higher bit is used.
    // Higher NIBBLE
    if(busyflag != 0) lcd_waitbusy(addr, 0);
    d = _LCDPORT_TRIS_CONT;
    d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
    _LCDPORT_TRIS_CONT = d;

    _LCDPORT_CONT_RW = 0;
    _LCDPORT_CONT_EN = 0;
    _LCDPORT_CONT_RS = 0;
    _LCDPORT_CONT_LATCH &= 0x0f; // Upper
//    b &= ~(_LCDPORT_CONT_RW | 0xf0 | _LCDPORT_CONT_EN | _LCDPORT_CONT_RS); // DATA=blank,RW='0',RS='0"
    _LCDPORT_CONT_LATCH |= h;
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 1;
    //_LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 0; // Disable EN
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT();

    // Lower NIBBLE
//    if(busyflag != 0) lcd_waitbusy(addr, 0);
//    d = _LCDPORT_TRIS_CONT;
//    d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
//    _LCDPORT_TRIS_CONT = d;

    _LCDPORT_CONT_RW = 0;
    _LCDPORT_CONT_EN = 0;
    _LCDPORT_CONT_RS = 0;
    _LCDPORT_CONT_LATCH &= 0x0f; // Lower

    _LCDPORT_CONT_LATCH |= l;
    _ACM1602_TC_WAIT(); // Wait 2.5?us
    _LCDPORT_CONT_EN = 1;
//    _LCDPORT_CONT_LATCH = b;

    _ACM1602_TC_WAIT(); // Wait 2.5?us
    _LCDPORT_CONT_EN = 0; // Disable EN
//    _LCDPORT_CONT_LATCH = b;

//    d = _LCDPORT_TRIS_CONT;
 //   d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
  //  _LCDPORT_TRIS_CONT = d;

#else
    unsigned char b,d;

    if(busyflag != 0) lcd_waitbusy(addr, 0);
    d = _LCDPORT_TRIS_DATA;
    d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
    _LCDPORT_TRIS_DATA = d;

    b = _LCDPORT_CONT_LATCH;
    b &= ~(_LCDPORT_CONT_RS | _LCDPORT_CONT_RW | _LCDPORT_CONT_EN); // RS='0',RW='0'
    _LCDPORT_CONT_LATCH = b;

    _LCDPORT_LATCH_DATA = cmd;

    b |= _LCDPORT_CONT_EN; // Send CMD
    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    b &= ~_LCDPORT_CONT_EN; // Disable EN
    _LCDPORT_CONT_LATCH = b;

    d = _LCDPORT_TRIS_DATA;
    d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
    _LCDPORT_TRIS_DATA = d;

#endif
    // End of Command.
    _ACM1602_SHORT_WAIT(); // Wait 50us
}

static void sendonce(unsigned char addr, unsigned char cmd)
{
#ifdef _LCD_IF_4BIT
    unsigned char b, d;
    unsigned char h;

    h = cmd & 0xf0; // Higher bit is used.
    // Higher NIBBLE
    //d = _LCDPORT_TRIS_CONT;
//    d = TRIS_D_VAL; // Clear mask for Tristate, direction = write.
//    _LCDPORT_TRIS_CONT = d;
    d = _LCDPORT_TRIS_CONT;
    d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
    _LCDPORT_TRIS_CONT = d;

    _LCDPORT_CONT_RS = 0;
    _LCDPORT_CONT_RW = 0;
    _LCDPORT_CONT_EN = 0; // RS='0',RW='0'
    _LCDPORT_CONT_LATCH &= 0x0f;
    _LCDPORT_CONT_LATCH |= h;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 1; // Send CMD
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 0; // Disable EN
    _ACM1602_TC_WAIT();

//    d = _LCDPORT_TRIS_CONT;
//    d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
//    _LCDPORT_TRIS_CONT = d;

#else
    unsigned char b, d;

    d = _LCDPORT_TRIS_DATA;
    d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
    _LCDPORT_TRIS_DATA = d;

    b = _LCDPORT_CONT_LATCH;
    b &= ~(_LCDPORT_CONT_RS | _LCDPORT_CONT_RW | _LCDPORT_CONT_EN); // RS='0',RW='0'
    _LCDPORT_CONT_LATCH = b;

    _LCDPORT_LATCH_DATA = cmd;

    b |= _LCDPORT_CONT_EN; // Send CMD
    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    b &= ~_LCDPORT_CONT_EN; // Disable EN
    _LCDPORT_CONT_LATCH = b;
//    _ACM1602_TC_WAIT();
#endif
    _ACM1602_SHORT_WAIT(); // Wait 50us
}

static void acm1602_ioinit(unsigned char addr)
{
    unsigned char b;

//    lcd_waitbusy(0);
    _ACM1602_LONG_LONG_WAIT();
#ifdef _LCD_IF_4BIT
    _LCDPORT_TRIS_CONT = TRIS_D_VAL;

    _LCDPORT_CONT_RS = 0;
    _LCDPORT_CONT_RW = 0;
    _LCDPORT_CONT_EN = 0;
    _LCDPORT_CONT_LATCH &= ~0xf0;
//    _LCDPORT_CONT_LATCH = b;
  //  _LCDPORT_TRIS_DATA = 0x00; // OUT
  //  _LCDPORT_LATCH_DATA = 0x00;
#else
    b = _LCDPORT_TRIS_CONT;
    b &= ~(_LCDPORT_CONT_RS | _LCDPORT_CONT_RW | _LCDPORT_CONT_EN);
    _LCDPORT_TRIS_CONT = b; // Clear Tristate bits, output.
    b = _LCDPORT_CONT_LATCH;
    b &= ~(_LCDPORT_CONT_RS | _LCDPORT_CONT_RW | _LCDPORT_CONT_EN);
    _LCDPORT_CONT_LATCH = b;
    _LCDPORT_TRIS_DATA = 0x00; // OUT
    _LCDPORT_LATCH_DATA = 0x00;
#endif
}


void acm1602_putchar(unsigned char addr, unsigned char c)
{
    unsigned char b, d;
#ifdef _LCD_IF_4BIT
    unsigned char h, l;
    h = c & 0xf0;
    l = (c & 0x0f)<<4;

    lcd_waitbusy(addr, 0);
//    d = _LCDPORT_TRIS_CONT;
//    d &= 0x00; // Clear mask for Tristate, direction = write.
    d = TRIS_D_VAL; // ALL OUT
    _LCDPORT_TRIS_CONT = d;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    // Send high nibble
    //b = _LCDPORT_CONT_LATCH;
    _LCDPORT_CONT_RW = 0;
    _LCDPORT_CONT_EN = 0; // DATA=blank,RW='0'
    _LCDPORT_CONT_LATCH &= ~0xf0;
    _LCDPORT_CONT_RS = 1; // RS='1"
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 1; // Do high write.
//    b |= h;
    _LCDPORT_CONT_LATCH |= h;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 0; // Disable EN
//    _LCDPORT_CONT_LATCH = b;
    _ACM1602_SHORT_WAIT();

//    lcd_waitbusy(addr, 0);
    _LCDPORT_CONT_LATCH &= ~0xf0;
    _LCDPORT_CONT_RS = 1; // RS='1"
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_LATCH |= l; // Do low write.
    _LCDPORT_CONT_EN = 1;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    _LCDPORT_CONT_EN = 0; // Disable EN
    _ACM1602_SHORT_WAIT();


#else
    lcd_waitbusy(addr, 0);
    d = _LCDPORT_TRIS_DATA;
    d &= ~_LCDPORT_READMASK; // Clear mask for Tristate, direction = write.
    _LCDPORT_TRIS_DATA = d;

    b = _LCDPORT_CONT_LATCH;
    b &= ~_LCDPORT_CONT_RW; // ,RW='0'
    b |= _LCDPORT_CONT_RS; // RS='1"
    _LCDPORT_LATCH_DATA = c;
    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    b |= _LCDPORT_CONT_EN; // Send character
    _LCDPORT_CONT_LATCH = b;
    _ACM1602_TC_WAIT(); // Wait 2.5?us

    b &= ~_LCDPORT_CONT_EN; // Disable EN
    _LCDPORT_CONT_LATCH = b;
    _ACM1602_SHORT_WAIT(); // Wait 0.1ms
#endif
}

#endif

void acm1602_cls(unsigned char addr)
{
    sendcmd(addr, 0x01, 0xff);
    _ACM1602_LONG_WAIT(); // Wait 5ms
}

void acm1602_setdataramaddress(unsigned char addr, unsigned char pos)
{
    unsigned char cmd = (pos & 0x7f) | 0x80;
    sendcmd(addr, cmd, 0xff);
}


void acm1602_locate_16x2(unsigned char addr, unsigned char x, unsigned char y)
{
    unsigned char ramaddr;
    if(x > 16) return;
    if(y > 1) return;
    ramaddr = (y == 0)? x  : (x  | 0x40);
    acm1602_setdataramaddress(addr, ramaddr);
}

/*
 * For using
 */
void acm1602_locate_8x2(unsigned char addr, unsigned char x, unsigned char y)
{
    unsigned char ramaddr;
    if(x > 8) return;
    if(y > 1) return;
    ramaddr = (y == 0)? x : x | 0x40;
    acm1602_setdataramaddress(addr, ramaddr);
}

void acm1602_home(unsigned char addr)
{
    sendcmd(addr, 0x02, 0xff);
    _ACM1602_LONG_WAIT(); // Wait 5ms
}


void acm1602_cursordir(unsigned char addr, unsigned char right)
{
    unsigned char cmd;

    cmd = 0x14;
    if(right == 0)  {
        cmd = 0x10;
    }
    sendcmd(addr, cmd, 0xff);
}

void acm1602_dispcursor(unsigned char addr, unsigned char flag)
{
    unsigned char cmd;
    cmd = 0x0d; // Cursor ON
    if(flag == 0x00) {
        cmd = 0x0c; // Cursor off
    }
    sendcmd(addr, cmd, 0xff);
}

void acm1602_setcgramaddress(unsigned char addr, unsigned char pos)
{
    unsigned char cmd = (pos & 0x3f) | 0x40;
    sendcmd(addr, cmd, 0xff);
}

void acm1602_init(unsigned char addr, unsigned char cls)
{
    acm1602_ioinit(addr);
#ifdef _USE_I2C_1602
    sendcmd(addr,0x01,0x00);
    sendcmd(addr, 0x38, 0x00); // 2lines, 8x10dot fonts.
//    sendcmd(addr, 0x38, 0x00); // 2lines, 8x10dot fonts.
#else
  #ifndef _LCD_IF_4BIT
    sendonce(addr, 0x38); // 2lines, 8x10dot fonts.
    sendcmd(addr, 0x38, 0x00); // 2lines, 8x10dot fonts.
  #else // 4Bit
    _ACM1602_LONG_LONG_WAIT();
    sendonce(addr, 0x30); // 2lines, 8x10dot fonts.
//    _ACM1602_SHORT_WAIT();
    sendonce(addr, 0x30); // 2lines, 8x10dot fonts.
//    _ACM1602_SHORT_WAIT();
    sendonce(addr, 0x30); // 2lines, 8x10dot fonts.
//    _ACM1602_SHORT_WAIT();
    sendonce(addr, 0x20); // 2lines, 8x10dot fonts.
//    _ACM1602_SHORT_WAIT();
    // Send twice on 4Bit Mode.
//    sendonce(addr, 0x30); // 2lines, 8x10dot fonts.
    sendcmd(addr, 0x28, 0xff); // 2lines, 8x5dot fonts.
  #endif
#endif
//    sendcmd(addr, 0x08, 0xff); // Display OFF.
    sendcmd(addr, 0x0c, 0xff); // Display ON.
//    sendcmd(addr, 0x06, 0xff); // Cursor increment,not shift.
   if(cls == 0){
        acm1602_home(addr);
    } else {
        acm1602_cls(addr);
    }
//    _ACM1602_LONG_WAIT();
    sendcmd(addr, 0x06, 0xff);
}

/*
 * Suspend lcd
 */
void acm1602_suspend(unsigned char addr)
{
    sendcmd(addr, 0x08, 0xff);
    _ACM1602_LONG_WAIT();
}

void acm1602_resume(unsigned char addr)
{
    sendcmd(addr, 0x0c, 0xff);
    _ACM1602_LONG_WAIT();
}


#ifdef _USE_I2C_1602
/*
 * st7032 extensions.
 */
void st7032_extcmd(unsigned char addr, unsigned char mode,
        unsigned char cmd, unsigned char busyf)
{
    unsigned char m = mode & 0x1c;
    m = m | 0x21;
    sendcmd(addr, m, busyf); // Shift to IS1
    sendcmd(addr, cmd, busyf);
    m &= 0xfe; // Shift to IS0
    sendcmd(addr, m, busyf);
}

void st7032_setcontrast(unsigned char addr, unsigned char value,
        unsigned char busyf)
{
    unsigned char val = value & 0x0f;
    val |= 0x70;
    st7032_extcmd(addr, 0x38, val, busyf);
}
#endif

