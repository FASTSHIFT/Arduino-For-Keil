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

#ifndef DTOSTRF_H
#define DTOSTRF_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/

#include <stddef.h>

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * @brief Convert a double to a string with a given width and precision.
 * @param val The double value to convert.
 * @param width The minimum width of the resulting string.
 * @param prec The number of decimal places to include.
 * @param sout The output buffer to write the string to.
 * @return The output buffer.
 */
char *dtostrf(double val, signed char width, unsigned char prec, char *sout);

/**
 * @brief Convert a double to a string with a given width and precision, with a maximum size for the output buffer.
 * @param val The double value to convert.
 * @param width The minimum width of the resulting string.
 * @param prec The number of decimal places to include.
 * @param sout The output buffer to write the string to.
 * @param sout_size The maximum size of the output buffer.
 * @return The output buffer.
 */
char *dtostrnf(double val, signed char width, unsigned char prec, char *sout, size_t sout_size);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* DTOSTRF_H */
