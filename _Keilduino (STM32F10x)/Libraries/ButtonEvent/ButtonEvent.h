/**
  ******************************************************************************
  * @file    ButtonEvent.h
  * @author  FASTSHIFT
  * @version V1.1.0
  * @date    2018-12-23
  * @brief   非阻塞式按键事件驱动库，支持短摁、长摁、释放、状态改变事件的识别
  * @Upgrade 2019.6.18 添加按键双击事件、长按单次触发事件
  ******************************************************************************
  * @attention
  * 需要提供一个精确到毫秒级的系统时钟，在ButtonEvent.cpp里定义ButtonEvent_Millis
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTONEVENT_H
#define __BUTTONEVENT_H

#include "stdint.h"

namespace ButtonEvent_Type{
	typedef enum {
		EVENT_ButtonPress,
		EVENT_ButtonLongPress,
		EVENT_ButtonLongPressOnce,
		EVENT_ButtonRelease,
		EVENT_ButtonChange,
		EVENT_ButtonDoubleClick,
		EVENT_MAX
	}Event_Type;
	
	typedef enum {
		NoPress, 
		Press, 
		LongPress
	} ButtonState_Type;
};

class ButtonEvent {
    typedef void(*ButtonEvent_FuncCallBack_t)(void);
	
public:
    ButtonEvent(bool NoPressState = 0, uint16_t LongPressTimeMs_Set = 500);

    void EventAttach_Press(ButtonEvent_FuncCallBack_t function);
    void EventAttach_LongPress(ButtonEvent_FuncCallBack_t function);
    void EventAttach_Release(ButtonEvent_FuncCallBack_t function);
    void EventAttach_Change(ButtonEvent_FuncCallBack_t function);

	void EventAttach(ButtonEvent_Type::Event_Type event, ButtonEvent_FuncCallBack_t function);
    void EventMonitor(uint8_t NowState);

	uint8_t GetClickCnt();

    operator uint8_t() {
        return Button_NowState;
    };
	
private:
    bool Button_NoPressState;
    uint8_t Button_NowState;
    uint16_t LongPressTimeMs;
    uint32_t LastPress_TimePoint;
	uint32_t LastClick_TimePoint;
	uint8_t Click_Cnt;
    bool IS_Pressed;
	bool IS_LongPressed;

    ButtonEvent_FuncCallBack_t CallbackGroup[ButtonEvent_Type::EVENT_MAX];
};

#endif
