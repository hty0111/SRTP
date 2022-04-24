/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-22 14:58:46
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-23 13:31:10
 */

#include "tim.h"
#include "chassis_task.h"
#include "mode_task.h"
#include "imu_task.h"

#define CONTROL_TIMER htim6
#define CONTROL_INTERVAL 0.001f

extern uint32_t control_tick;

void Control_Init(void);
void Control_Task(void);
