#include "MillisTaskManager.h"

void NullFunction()//空函数
{
}


//用途：初始化任务列表
//参数: (设定任务列表总长度)
MillisTaskManager::MillisTaskManager(uint8_t TaskNum_MAX_Set)
{
	if(TaskList != NULL) delete TaskList;//清空列表
	TaskList = new MillisTaskManager_TypeDef[TaskNum_MAX_Set];//为任务列表申请内存
	TaskNum_MAX = TaskNum_MAX_Set;//记录任务列表总长度
	
	for(uint8_t i = 0; i < TaskNum_MAX; i++)//初始化任务列表，清零
	{
		TaskClear(i);
	}
}

//用途：不带越界判断地清除一个任务，private权限
//参数: (任务注册的位置)
void MillisTaskManager::TaskClear(uint8_t FuncPos)
{
	TaskList[FuncPos].Function = NullFunction;
	TaskList[FuncPos].State = 0;
	TaskList[FuncPos].IntervalTime = 0;
	TaskList[FuncPos].TimePoint = 0;
}

//用途：在任务列表内的一个位置注册一个任务，设定间隔执行时间
//参数: (任务注册地址，任务函数指针，时间设定(毫秒))
int16_t MillisTaskManager::TaskRegister(uint8_t FuncPos, void_TaskFunction_t Function, uint32_t TimeSetMs, uint8_t TaskState)
{
	if(FuncPos < TaskNum_MAX)//判断是否越界
	{
		TaskList[FuncPos].Function = Function;//注册函数指针
		TaskList[FuncPos].State = TaskState;//初始状态设定
		TaskList[FuncPos].IntervalTime = TimeSetMs;//注册时间
		return FuncPos;//如果注册成功，返回注册地址
	}
	return -1;//如果注册失败，返回-1
}

//用途：注销任务
//参数: (任务函数指针)
int16_t MillisTaskManager::TaskLogout(void_TaskFunction_t Function)
{
	for(uint8_t i = 0; i < TaskNum_MAX; i++)
	{
		if(TaskList[i].Function == Function)
		{
			TaskClear(i);
			return i;
		}
	}
	return -1;
}

//用途：注销任务
//参数: (任务注册的位置)
int16_t MillisTaskManager::TaskLogout(uint8_t FuncPos)
{
	if(FuncPos < TaskNum_MAX)
	{
		TaskClear(FuncPos);
		return FuncPos;
	}
	return -1;
}

//用途：任务状态控制
//参数: (任务函数指针，任务状态)
int16_t MillisTaskManager::TaskCtrl(void_TaskFunction_t Function, uint8_t TaskState)
{
	for(uint8_t i = 0; i < TaskNum_MAX; i++)
	{
		if(TaskList[i].Function == Function)
		{
			TaskList[i].State = TaskState;
			return i;
		}
	}
	return -1;
}

//用途：任务状态控制
//参数: (任务注册的位置，任务状态)
int16_t MillisTaskManager::TaskCtrl(uint8_t FuncPos, uint8_t TaskState)
{
	if(FuncPos < TaskNum_MAX)
	{
		TaskList[FuncPos].State = TaskState;
		return FuncPos;
	}
	return -1;
}

//用途：执行调度器
//参数: (一个精确到毫秒的系统时钟)
void MillisTaskManager::Running(uint32_t MillisSeed)
{
	for(uint8_t i = 0; i < TaskNum_MAX; i++)//遍历任务列表
	{
		if(TaskList[i].Function != NullFunction)//判断是否为空函数
		{
			if(TaskList[i].State && MillisSeed >= TaskList[i].TimePoint)//判断是否运行任务，是否到达触发时间点
			{
				TaskList[i].TimePoint = MillisSeed + TaskList[i].IntervalTime;//标记下一个时间点
				TaskList[i].Function();//执行任务
			}
		}
	}
}
