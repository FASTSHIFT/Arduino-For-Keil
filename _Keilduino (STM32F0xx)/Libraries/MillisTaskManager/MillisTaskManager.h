
/**********     超轻量级分时合作式任务调度器     **********/
/**********        Designed By _VIFEXTech        **********/

//Finished 2018.7.26  v1.0 可以替代旧的millis()循环判断的方案，不依赖ArduinoAPI
//Update   2018.10.9  v1.1 将任务状态标志位类型换成bool类型
//Update   2018.11.17 v1.2 将typedef转移至class内部
//Update   2018.11.18 v1.3 修复50天后数值溢出导致任务停止的bug
//Update   2018.12.17 v1.4 将 TaskCtrl 修改为 TaskStateCtrl，添加修改任务间隔时间的接口，添加 TaskFind 用于遍历列表寻找任务
//Update   2019.2.5   v1.5 添加析构函数，用于释放内存

#ifndef __MILLISTASKMANAGER_H
#define __MILLISTASKMANAGER_H

#define _MILLISTASKMANAGER_VERSION "v1.5"

#include "stdint.h"

class MillisTaskManager {
public:
    typedef void(*void_TaskFunction_t)(void);//回调函数定义
    typedef uint8_t TaskNum_t;
    typedef struct {
        bool State; //任务状态
        void_TaskFunction_t Function; //任务函数指针
        uint32_t IntervalTime; //任务触发间隔时间
        uint32_t TimePoint; //任务触发时间点
    } MillisTaskManager_TypeDef; //任务类型定义
	
    MillisTaskManager(TaskNum_t TaskNum_MAX_Set);
    ~MillisTaskManager();
    bool TaskRegister(TaskNum_t FuncPos, void_TaskFunction_t Function, uint32_t TimeSetMs, bool TaskState = true);
    bool TaskFind(void_TaskFunction_t Function, TaskNum_t *FuncPos);
    bool TaskLogout(void_TaskFunction_t Function);
    bool TaskLogout(TaskNum_t FuncPos);
    bool TaskStateCtrl(void_TaskFunction_t Function, bool TaskState);
    bool TaskStateCtrl(TaskNum_t FuncPos, bool TaskState);
    bool TaskSetIntervalTime(void_TaskFunction_t Function, uint32_t TimeSetMs);
    bool TaskSetIntervalTime(TaskNum_t FuncPos, uint32_t TimeSetMs);
    void Running(uint32_t MillisSeed);
private:
    MillisTaskManager_TypeDef* TaskList;//任务列表
    TaskNum_t TaskNum_MAX;//任务列表长度
    void TaskClear(TaskNum_t FuncPos);//清除任务
};

#endif
