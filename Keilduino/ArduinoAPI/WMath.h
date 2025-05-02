/*
 * MIT License
 * Copyright (c) 2017 - 2025 _VIFEXTech
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef WMATH_H
#define WMATH_H

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
 * @brief  Seed the random number generator with a specific seed value.
 * @param  seed  The seed value to initialize the random number generator.
 */
void randomSeed(unsigned long seed);

/**
 * @brief  Generate a random number between 0 and the specified maximum value.
 * @param  howbig  The upper limit for the random number (exclusive).
 * @return A random number between 0 and howbig - 1.
 */
long random(long howbig);

/**
 * @brief  Generate a random number between the specified minimum and maximum values.
 * @param  howsmall  The lower limit for the random number (inclusive).
 * @param  howbig    The upper limit for the random number (exclusive).
 * @return A random number between howsmall and howbig - 1.
 */
long random(long howsmall, long howbig);

/**
 * @brief  Create a word (16-bit value) from a single unsigned integer.
 * @param  w  The unsigned integer to convert to a word.
 * @return The word created from the input integer.
 */
static inline unsigned int makeWord(unsigned int w)
{
    return w;
}

/**
 * @brief  Create a word (16-bit value) from two unsigned char values.
 * @param  h  The high byte of the word.
 * @param  l  The low byte of the word.
 * @return The word created from the input bytes.
 */
static inline unsigned int makeWord(unsigned char h, unsigned char l)
{
    return (h << 8) | l;
}

#endif /* WMATH_H */
