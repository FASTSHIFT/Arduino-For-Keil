#include "ButtonEvent.h"

ButtonEvent::ButtonEvent(bool NoPressState, uint16_t LongPressTimeMs_Set)
{
    Button_NoPressState = NoPressState;
    LongPressTimeMs = LongPressTimeMs_Set;

    PressKey_TimePoint = 0;
    IS_Pressed = false;

    ButtonPress_Function = 0;
    ButtonLongPress_Function = 0;
    ButtonRelease_Function = 0;
    ButtonChange_Function = 0;
}

//说明：按键按下事件绑定
//参数：回调函数指针
void ButtonEvent::EventAttach_Press(ButtonEvent_FuncCallBack_t Function)
{
    ButtonPress_Function = Function;
}

//说明：按键长按事件绑定
//参数：回调函数指针
void ButtonEvent::EventAttach_LongPress(ButtonEvent_FuncCallBack_t Function)
{
    ButtonLongPress_Function = Function;
}

//说明：按键释放事件绑定
//参数：回调函数指针
void ButtonEvent::EventAttach_Release(ButtonEvent_FuncCallBack_t Function)
{
    ButtonRelease_Function = Function;
}

//说明：按键改变事件绑定
//参数：回调函数指针
void ButtonEvent::EventAttach_Change(ButtonEvent_FuncCallBack_t Function)
{
    ButtonChange_Function = Function;
}

//说明：直接读取按键
//参数：按键IO的状态
void ButtonEvent::Read_NoWipeShake(uint8_t NowState)
{
    if (!IS_Pressed && (NowState != Button_NoPressState))
    {
        Button_NowState = IS_Pressed = true;
        PressKey_TimePoint = millis() + LongPressTimeMs;
        if(ButtonPress_Function) ButtonPress_Function();
        if(ButtonChange_Function)ButtonChange_Function();
    }
    else if (IS_Pressed && millis() >= PressKey_TimePoint && (NowState != Button_NoPressState))
    {
        Button_NowState = 2;
        if(ButtonLongPress_Function) ButtonLongPress_Function();
    }
    else if (IS_Pressed && (NowState == Button_NoPressState))
    {
        Button_NowState = IS_Pressed = false;
        if(ButtonRelease_Function) ButtonRelease_Function();
        if(ButtonChange_Function) ButtonChange_Function();
    }
}

