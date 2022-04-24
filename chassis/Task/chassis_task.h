/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-21 23:26:00
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-23 13:38:56
 */

void Chassis_Task(void);

#ifndef __CHASSIS_TASK_H__
#define __CHASSIS_TASK_H__

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>
#include <math.h>

#include "can_receive.h"
#include "remote_ctrl.h"
#include "pid.h"
#include "mode_task.h"
#include "imu_task.h"

#ifndef DEGREE_TO_RADIAN
	#define DEGREE_TO_RADIAN	PI / 180
#endif
/* Exported types ------------------------------------------------------------*/
extern int32_t set_VX, set_VY;
extern int32_t chassis_current[4];
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void Chassis_Task(void);
/* Private defines -----------------------------------------------------------*/


#endif

