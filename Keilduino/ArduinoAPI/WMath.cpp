/*
 * MIT License
 * Copyright (c) 2017 - 2022 _VIFEXTech
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
#include "WMath.h"
#include <stdlib.h>

void randomSeed(unsigned long seed)
{
    if (seed == 0)
    {
        return;
    }

    srand(seed);
}

long random(long howbig)
{
    if (howbig == 0)
    {
        return 0;
    }

    return rand() % howbig;
}

long random(long howsmall, long howbig)
{
    if (howsmall >= howbig)
    {
        return howsmall;
    }

    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

unsigned int makeWord(unsigned int w)
{
    return w;
}

unsigned int makeWord(unsigned char h, unsigned char l)
{
    return (h << 8) | l;
}
