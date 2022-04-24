/*
 * @Description: mode update
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-22 18:42:53
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-22 23:48:05
 */


#ifndef __MODE_TASK_H__
#define __MODE_TASK_H__

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include "remote_ctrl.h"

/* Exported types ------------------------------------------------------------*/

// chassis mode
typedef enum {
    Chassis_Normal_Mode,	// 常规控制
	Chassis_Auto_Mode,		// 自动行驶
	Chassis_PID_Mode,		// 用于整定PID参数
} Chassis_Mode_t;

// signal source
typedef enum
{
	Remote_Control,
	Keyboard_Mouse,
}Source_R_K_t;

extern Source_R_K_t RK_source;
extern Chassis_Mode_t chassis_mode;

void Mode_Init(void);
void Mode_Task(void);


#endif
