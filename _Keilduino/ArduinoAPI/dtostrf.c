/*
  dtostrf - Emulation for dtostrf function from avr-libc
  Copyright (c) 2013 Arduino.  All rights reserved.
  Written by Cristian Maglie <c.maglie@bug.st>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*********************
 *      INCLUDES
 *********************/

#include "dtostrf.h"
#include <stdio.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

char* dtostrf(double val, signed char width, unsigned char prec, char* sout)
{
    char fmt[20];
    snprintf(fmt, sizeof(fmt), "%%%d.%df", width, prec);
    sprintf(sout, fmt, val);
    return sout;
}

char* dtostrnf(double val, signed char width, unsigned char prec, char* sout, size_t sout_size)
{
    char fmt[20];
    snprintf(fmt, sizeof(fmt), "%%%d.%df", width, prec);
    snprintf(sout, sout_size, fmt, val);
    return sout;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
