/**
  ******************************************************************************
  * @file    DigitalFilter.h
  * @author  FASTSHIFT
  * @version V1.1.0
  * @date    2019-6-28
  * @brief   通用数字滤波器，采用模版类编写，支持泛型
  * @Upgrade 2019.10.28 添加注释和例程
  ******************************************************************************
  * @attention
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIGITALFILTER_H
#define __DIGITALFILTER_H

#include "stdint.h"

/*取两个数最小值*/
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

/*取两个数最大值*/
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

/******************* 均值滤波器 *******************/
template <class T> class FilterAverage
{
public:
    FilterAverage(uint32_t _filterSamples);
    ~FilterAverage();
    T getNext(T rawIn);
private:
    T *sensSmoothArray;
    T *sorted;
    uint32_t filterSamples;
    uint32_t cnt;
};

template <class T> FilterAverage<T>::FilterAverage(uint32_t _filterSamples)
{
    if(_filterSamples % 2 == 0)_filterSamples++;
    filterSamples = _filterSamples;
    sensSmoothArray = new T[filterSamples];
    sorted = new T[filterSamples];
}

template <class T> FilterAverage<T>::~FilterAverage()
{
    delete[] sensSmoothArray;
    delete[] sorted;
}

template <class T> T FilterAverage<T>::getNext(T rawIn)
{
    int32_t j, k, top, bottom;
    T total;
    bool done;

    cnt = (cnt + 1) % filterSamples;    // increment counter and roll over if necc. -  % (modulo operator) rolls over variable
    sensSmoothArray[cnt] = rawIn;                 // input new data into the oldest slot

    for (j = 0; j < filterSamples; j++)
    {
        // transfer data array into anther array for sorting and averaging
        sorted[j] = sensSmoothArray[j];
    }

    done = false;                // flag to know when we're done sorting
    while(!done)
    {
        // simple swap sort, sorts numbers from lowest to highest
        done = true;
        for (j = 0; j < (filterSamples - 1); j++)
        {
            if (sorted[j] > sorted[j + 1])
            {
                // numbers are out of order - swap
                T temp = sorted[j + 1];
                sorted [j + 1] =  sorted[j] ;
                sorted [j] = temp;
                done = false;
            }
        }
    }

    // throw out top and bottom 15% of samples - limit to throw out at least one from top and bottom
    bottom = max(((filterSamples * 15)  / 100), 1);
    top = min((((filterSamples * 85) / 100) + 1  ), (filterSamples - 1));   // the + 1 is to make up for asymmetry caused by integer rounding
    k = 0;
    total = 0;
    for ( j = bottom; j < top; j++)
    {
        total += sorted[j];  // total remaining indices
        k++;
    }
    return total / k;    // divide by number of samples
}

/******************* 滞回滤波器 *******************/
template <class T> class FilterHysteresis
{
public:
    FilterHysteresis(T hysNum);
    T getNext(T now);
private:
    T HysNum, LastNum;
};

template <class T> FilterHysteresis<T>::FilterHysteresis(T hysNum)
{
    HysNum = hysNum;
}

template <class T> T FilterHysteresis<T>::getNext(T now)
{
    if(ABS(now - LastNum) > HysNum)
    {
        LastNum = now;
    }
    return LastNum;
}

#endif
