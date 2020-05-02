#ifndef __STM32L1XX_CONF_H
#define __STM32L1XX_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l1xx_adc.h"
#include "stm32l1xx_aes.h"
#include "stm32l1xx_crc.h"
#include "stm32l1xx_comp.h"
#include "stm32l1xx_dac.h"
#include "stm32l1xx_dbgmcu.h"
#include "stm32l1xx_dma.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_flash.h"
#include "stm32l1xx_gpio.h"
#include "stm32l1xx_syscfg.h"
#include "stm32l1xx_i2c.h"
#include "stm32l1xx_iwdg.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_spi.h"
#include "stm32l1xx_tim.h"
#include "stm32l1xx_usart.h"
#include "stm32l1xx_wwdg.h"
#include "misc.h"  /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */


/* 取消对下面一行展开assert_param宏用于标准外设库的驱动程序代码*/
/*#define USE_FULL_ASSERT    1 */

/* 导出的宏定义 --------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/***************************************************************************//**
  * @brief   assert_param 用于参数检查.
  * @param   expr: 假如expr是flase, 将会调用 assert_param
  *          报告错误发生所在的源文件名和所在的行数
  *          假如expr 是 true, 将步返回值.
  * @retval  无
*******************************************************************************/
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* 函数定义------------------------------------------------------------------ */
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}// extern "C"
#endif

#endif /* __STM32L1XX_CONF_H */
