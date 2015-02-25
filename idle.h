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

#ifndef IDLE_H
#define	IDLE_H

#ifdef	__cplusplus
extern "C" {
#endif
#define _IDLE_ONE_TICK 128 // uS
extern void idle(unsigned int initial);
extern void idle_init(void);
extern void idle_stop(void); // Stop TMR0.
extern void idle_time_ms(unsigned int ms);
extern void idle_time_62_5ms(void);
extern void idle_time_35ms(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IDLE_H */

