/*
 * OpenI2CRADIO
 * I/O Port defines / I2C.
 * Copyright (C) 2013-08-22 K.Ohta <whatisthis.sowhat ai gmail.com>
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

#ifndef IODEF_I2C_H
#define	IODEF_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Machine depended flags
 */
#if defined(__SDCC)
#if defined(pic18f23k22) || defined(pic18f24k22) || defined(pic18f25k22) || defined(pic18f26k22)
#define _I2C_IO_TWO_MSSPS
#define _PIO_HAS_THREE
#endif

#if defined(pic18f23k20) || defined(pic18f24k20) || defined(pic18f25k20) || defined(pic18f26k20)
#define _I2C_IO_ONE_MSSP
#define _PIO_HAS_THREE
#endif

#if defined(pic18f43k20) || defined(pic18f44k20) || defined(pic18f45k20) || defined(pic18f46k20)
#define _I2C_IO_ONE_MSSP
#define _PIO_HAS_FIVE
#endif
#else // __XC

#if defined(_18F23K22) || defined(_18F24K22) || defined(_18F25K22) || defined(_18F26K22)
#define _I2C_IO_TWO_MSSPS
#define _PIO_HAS_THREE
#endif

#if defined(_18F23K20) || defined(_18F24K20) || defined(_18F25K20) || defined(_18F26K20)
#define _I2C_IO_ONE_MSSP
#define _PIO_HAS_THREE
#endif

#if defined(_18F43K20) || defined(_18F44K20) || defined(_18F45K20) || defined(_18F46K20)
#define _I2C_IO_ONE_MSSP
#define _PIO_HAS_FIVE
#endif
#endif



#ifdef	__cplusplus
}
#endif

#endif	/* IODEF_I2C_H */

