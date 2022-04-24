/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-22 14:58:39
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-23 13:30:36
 */

#include "control_task.h"

uint32_t control_tick;

void Control_Init(void);
void Control_Task(void);

/**
 * @brief 初始化总控
 * @param {*}
 * @return {*}
 */
void Control_Init(void)
{
    HAL_TIM_Base_Start_IT(&CONTROL_TIMER);
}

/**
 * @brief 总控任务
 * @param {*}
 * @return {*}
 */
void Control_Task(void)
{
    control_tick++;
    IMU_Task();
    Mode_Task();
    Chassis_Task();
}
