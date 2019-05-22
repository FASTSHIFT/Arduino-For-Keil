#ifndef __BUTTONEVENT_H
#define __BUTTONEVENT_H

#include "Arduino.h"

class ButtonEvent{
    typedef void(*ButtonEvent_FuncCallBack_t)(void);
public:
	ButtonEvent(bool NoPressState = 0, uint16_t LongPressTimeMs_Set = 500);
	void EventAttach_Press(ButtonEvent_FuncCallBack_t Function);
	void EventAttach_LongPress(ButtonEvent_FuncCallBack_t Function);
	void EventAttach_Release(ButtonEvent_FuncCallBack_t Function);
	void EventAttach_Change(ButtonEvent_FuncCallBack_t Function);
	void Read_WipeShake();
	void Read_NoWipeShake(uint8_t NowState);
	operator uint8_t() { return Button_NowState;};
private:
	bool Button_NoPressState;
	uint8_t Button_NowState;
	uint16_t LongPressTimeMs;
	uint32_t PressKey_TimePoint;
	bool IS_Pressed;

	ButtonEvent_FuncCallBack_t ButtonPress_Function;
	ButtonEvent_FuncCallBack_t ButtonLongPress_Function;
	ButtonEvent_FuncCallBack_t ButtonRelease_Function;
	ButtonEvent_FuncCallBack_t ButtonChange_Function;
};

#endif
