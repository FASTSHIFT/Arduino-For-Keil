/**
  ******************************************************************************
  * File   : RTC/Calendar/rtc.h
  * Version: V1.2.3
  * Date   : 2020-08-15
  * Brief  : This file contains the headers of the calendar API.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H

/* Includes ------------------------------------------------------------------*/
#include "mcu_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint16_t w_year;
    uint8_t  w_month;
    uint8_t  w_date;
    uint8_t  week;
} RTC_Calendar_TypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_Init(void);
void RTC_GetCalendar(RTC_Calendar_TypeDef* calendar);
uint8_t RTC_SetAlarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);
uint8_t RTC_GetWeek(uint16_t year, uint8_t month, uint8_t day);
uint8_t RTC_Set(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
