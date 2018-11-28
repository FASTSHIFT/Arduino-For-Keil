//Designed By _VIFEXTech
//Finished in 2018.7.26 V_1.0 可以替代旧的millis()循环判断的方案，不依赖ArduinoAPI

#ifndef __MILLISTASKMANAGER_H
#define __MILLISTASKMANAGER_H

#include "stdint.h"

#ifndef NULL
#define NULL 0
#endif

typedef void(*void_TaskFunction_t)(void);//函数指针类型定义

typedef struct{
	void_TaskFunction_t Function; //任务函数指针
	uint8_t State; //任务状态
	uint32_t IntervalTime; //任务触发间隔时间
	uint32_t TimePoint; //任务触发时间点
}MillisTaskManager_TypeDef;//任务类型定义

class MillisTaskManager{
	public:
		MillisTaskManager(uint8_t TaskNum_MAX_Set);
		int16_t TaskRegister(uint8_t FuncPos, void_TaskFunction_t Function, uint32_t TimeSetMs, uint8_t TaskState = 1);
		int16_t TaskLogout(void_TaskFunction_t Function);
		int16_t TaskLogout(uint8_t FuncPos);
		int16_t TaskCtrl(void_TaskFunction_t Function, uint8_t TaskState);
		int16_t TaskCtrl(uint8_t FuncPos, uint8_t TaskState);
		void Running(uint32_t MillisSeed);
	private:
		MillisTaskManager_TypeDef* TaskList;//任务列表
		uint8_t TaskNum_MAX;//任务列表长度
		void TaskClear(uint8_t FuncPos);//清除任务
};

#endif
