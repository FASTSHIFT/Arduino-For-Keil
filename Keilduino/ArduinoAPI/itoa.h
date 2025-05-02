/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef ITOA_H
#define ITOA_H

#ifdef __cplusplus
extern "C" {
#endif

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
 * @brief Convert an integer to a string.
 * @param value The integer value to convert.
 * @param string The output buffer to write the string to.
 * @param radix The base of the number system to use.
 * @return The output buffer.
 */
char* itoa(int value, char* string, int radix);

/**
 * @brief Convert a long integer to a string.
 * @param value The long integer value to convert.
 * @param string The output buffer to write the string to.
 * @param radix The base of the number system to use.
 * @return The output buffer.
 */
char* ltoa(long value, char* string, int radix);

/**
 * @brief Convert an unsigned integer to a string.
 * @param value The unsigned integer value to convert.
 * @param string The output buffer to write the string to.
 * @param radix The base of the number system to use.
 * @return The output buffer.
 */
char* utoa(unsigned int value, char* string, int radix);

/**
 * @brief Convert an unsigned long integer to a string.
 * @param value The unsigned long integer value to convert.
 * @param string The output buffer to write the string to.
 * @param radix The base of the number system to use.
 * @return The output buffer.
 */
char* ultoa(unsigned long value, char* string, int radix);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* ITOA_H */
