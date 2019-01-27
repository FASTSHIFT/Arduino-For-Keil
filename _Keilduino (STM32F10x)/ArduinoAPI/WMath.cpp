/* -*- mode: jde; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Part of the Wiring project - http://wiring.org.co
  Copyright (c) 2004-06 Hernando Barragan
  Modified 13 August 2006, David A. Mellis for Arduino - http://www.arduino.cc/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/
#include "WMath.h"
#include "Arduino.h"

/**
  * @brief  初始化伪随机数生成器
  * @param  seed：数字初始化伪随机序列
  * @retval 无
  */
void randomSeed(unsigned long seed)
{
    if (seed != 0) {
        srand(seed);
    }
}

/**
  * @brief  随机函数生成伪随机数
  * @param  howbig：随机值的上界
  * @retval 最小和 max-1 之间的随机数
  */
long random(long howbig)
{
    if (howbig == 0) {
        return 0;
    }
    return rand() % howbig;
}

/**
  * @brief  随机函数生成伪随机数
  * @param  howsmall：随机值的下界
			howbig：随机值的上界
  * @retval 最小和 max-1 之间的随机数
  */
long random(long howsmall, long howbig)
{
    if (howsmall >= howbig) {
        return howsmall;
    }
    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

unsigned int makeWord(unsigned int w) {
    return w;
}
unsigned int makeWord(unsigned char h, unsigned char l) {
    return (h << 8) | l;
}
