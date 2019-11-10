#include "ButtonEvent.h"

using namespace ButtonEvent_Type;

#ifdef ARDUINO
#include "Arduino.h"
#define ButtonEvent_Millis millis()
#else
#error "Please set the system clock used by ButtonEvent"
#endif

#define TriggerEvent(event) if(event)event()

/**
  * @brief  按键事件构造函数
  * @param  NoPressState: 按键未按下时的状态
  * @param  LongPressTimeMs_Set: 按键长按触发超时设置
  * @retval 无
  */
ButtonEvent::ButtonEvent(bool NoPressState, uint16_t LongPressTimeMs_Set)
{
    Button_NoPressState = NoPressState;
    LongPressTimeMs = LongPressTimeMs_Set;

    LastClick_TimePoint = LastPress_TimePoint = 0;
    IS_LongPressed = IS_Pressed = false;

    for(uint8_t i = 0; i < EVENT_MAX; i++)
    {
        CallbackGroup[i] = 0;
    }
}

/**
  * @brief  按键按下事件绑定
  * @param  Function: 回调函数指针
  * @retval 无
  */
void ButtonEvent::EventAttach_Press(ButtonEvent_FuncCallBack_t function)
{
    CallbackGroup[EVENT_ButtonPress] = function;
}

/**
  * @brief  按键长按事件绑定
  * @param  Function: 回调函数指针
  * @retval 无
  */
void ButtonEvent::EventAttach_LongPress(ButtonEvent_FuncCallBack_t function)
{
    CallbackGroup[EVENT_ButtonLongPress] = function;
}

/**
  * @brief  按键释放事件绑定
  * @param  Function: 回调函数指针
  * @retval 无
  */
void ButtonEvent::EventAttach_Release(ButtonEvent_FuncCallBack_t function)
{
    CallbackGroup[EVENT_ButtonRelease] = function;
}

/**
  * @brief  按键状态改变事件绑定
  * @param  Function: 回调函数指针
  * @retval 无
  */
void ButtonEvent::EventAttach_Change(ButtonEvent_FuncCallBack_t function)
{
    CallbackGroup[EVENT_ButtonChange] = function;
}

/**
  * @brief  按键状态改变事件绑定
  * @param  event: 事件类型
  * @param  Function: 回调函数指针
  * @retval 无
  */
void ButtonEvent::EventAttach(Event_Type event, ButtonEvent_FuncCallBack_t function)
{
    if(event != EVENT_MAX)
    {
        CallbackGroup[event] = function;
    }
}

/**
  * @brief  监控事件，建议扫描周期10ms
  * @param  NowState: 当前按键引脚状态
  * @retval 无
  */
void ButtonEvent::EventMonitor(uint8_t NowState)
{
    if (!IS_Pressed && (NowState != Button_NoPressState))
    {
        IsPressed = IS_Pressed = true;
        Button_NowState = Press;
        LastPress_TimePoint = ButtonEvent_Millis;

        TriggerEvent(CallbackGroup[EVENT_ButtonPress]);
        TriggerEvent(CallbackGroup[EVENT_ButtonChange]);
    }
    else if (IS_Pressed && ButtonEvent_Millis - LastPress_TimePoint > LongPressTimeMs && (NowState != Button_NoPressState))
    {
        Button_NowState = LongPress;
        if(!IS_LongPressed)
        {
            TriggerEvent(CallbackGroup[EVENT_ButtonLongPressOnce]);
            IsLongPressed = IS_LongPressed = true;
        }
        TriggerEvent(CallbackGroup[EVENT_ButtonLongPress]);
    }
    else if (IS_Pressed && (NowState == Button_NoPressState))
    {
        IS_LongPressed = IS_Pressed = false;
        IsClicked = true;
        Button_NowState = NoPress;
        LastClick_TimePoint = ButtonEvent_Millis;

        TriggerEvent(CallbackGroup[EVENT_ButtonRelease]);
        TriggerEvent(CallbackGroup[EVENT_ButtonChange]);
    }

    if(IS_Pressed && ButtonEvent_Millis - LastClick_TimePoint < 200)
    {
        Click_Cnt++;
        TriggerEvent(CallbackGroup[EVENT_ButtonDoubleClick]);
    }
}

