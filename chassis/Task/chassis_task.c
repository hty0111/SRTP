/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-21 23:24:24
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-23 13:58:08
 */

#include "chassis_task.h"

//遥控器转换到线速度的比例
#define RC_VX_RATIO 5000             //mm/s   正方向：up
#define RC_VY_RATIO 5000             //mm/s   正方向：right
#define RC_VW_RATIO 50
#define VW_MAX 5000

int32_t set_VX = 0, set_VY = 0, set_VW = 0;
float raw_VX = 0, raw_VY = 0, raw_VW = 0;
float set_yaw_angle = 0;
int32_t chassis_current[4] = {0};
int32_t chassis_speed[4] = {0};

void Chassis_Task(void);


/**
 * @brief 底盘控制 
 * @param {*}
 * @return {*}
 */
void Chassis_Task(void)
{
	if (chassis_mode == Chassis_Normal_Mode)
	{
		// set X, Y
		if (RK_source == Remote_Control)
		{
			raw_VX = rc.RV;
			raw_VY = rc.RH;
            
            set_yaw_angle += rc.LH / 10;
            if (set_yaw_angle > 180)
                set_yaw_angle -=360;
            if (set_yaw_angle < -180)
                set_yaw_angle += 360;
		}	
		else if (RK_source == Keyboard_Mouse)
		{
			if (rc.kb.bit.W && !rc.kb.bit.S)  // 按下W，不按S，前进
				raw_VX = 1.0f;
			else if (!rc.kb.bit.W && rc.kb.bit.S)  // 按下S，不按W，后退
				raw_VX = -1.0f;
			else  // W和S都没按，或误操作同时按下，X轴方向不动
				raw_VX = 0;
			if (rc.kb.bit.A && !rc.kb.bit.D)  // 按下A，不按D，向右
				raw_VY = 1.0f;
			else if (!rc.kb.bit.A && rc.kb.bit.D)  // 按下D，不按A，向左
				raw_VY = -1.0f;
			else  // A和D都没按，或误操作同时按下，Y轴方向不动
				raw_VY = 0;
		}
		// 云台方向映射到底盘方向
//		float angle = 0;
//		// angle = set_yaw_angle - imu_data.yaw_angle;
//		set_VX = (-raw_VX * cos(angle) + raw_VY * sin(angle)) * RC_VX_RATIO;
//		set_VY = (-raw_VX * sin(angle) - raw_VY * cos(angle)) * RC_VY_RATIO;
		
		set_VX = -raw_VX * RC_VX_RATIO;
		set_VY = -raw_VY * RC_VY_RATIO;
		set_VW = PID_Calc(&chassis_angle_pid_t, set_yaw_angle, imu_data.yaw_angle) * RC_VW_RATIO;
	}
	else if (chassis_mode == Chassis_Lock_Mode)
	{
		set_VX = set_VY = set_VW = 0;
	}
	
	// 平动和转动
	chassis_speed[0] = -set_VY + set_VX - set_VW;
	chassis_speed[1] = set_VY + set_VX - set_VW;
	chassis_speed[2] = set_VY - set_VX - set_VW;
	chassis_speed[3] = -set_VY - set_VX - set_VW;
	
	for (uint8_t i = 0; i < 4; i++)
		chassis_current[i] = PID_Calc(&chassis_speed_pid_t[i], ChassisEncoder[i].rotate_speed, chassis_speed[i]);	
	
	CAN_Send_Msg(&hcan1, chassis_current, TxMessage, M3508_1_to_4_ID, CAN_Send_Len);
}
/* External variables --------------------------------------------------------*/

