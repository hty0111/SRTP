/*
 * @Description: mode update
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-22 18:42:39
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-22 23:46:40
 */

#include "mode_task.h"

Source_R_K_t RK_source = Remote_Control;
Chassis_Mode_t chassis_mode;

void Mode_Init(void);
void Mode_Task(void);
static void rc_update(void);
static void kb_update(void);

/**
 * @brief 初始化底盘模式
 * @param {*}
 * @return {*}
 */
void Mode_Init(void)
{
	chassis_mode = Chassis_Normal_Mode;
}

/**
 * @brief 一旦有键盘按键就切换为键鼠控制，遥控器sw改变则切换为遥控器控制，否则维持原来模式
 * @param {*}
 * @return {*}
 */
void Mode_Task(void)
{
	// 切换遥控器控制的条件
	if (last_rc.sw1 != rc.sw1 || last_rc.sw2 != rc.sw2)
	{
		RK_source = Remote_Control;
	}
	
	// 切换键鼠控制的条件
	if (rc.kb.key_code > 0)
	{
		RK_source = Keyboard_Mouse;
	}
	
	switch (RK_source)
	{
        case Keyboard_Mouse: kb_update();break;
        case Remote_Control: rc_update();break;
	}
	last_rc = rc;
}

static void rc_update(void)
{
	// normal mode
	if (rc.sw1 == RC_MI)
	{
		chassis_mode = Chassis_Normal_Mode;
		if (rc.sw2 == RC_UP || rc.sw2 == RC_DN)
			chassis_mode = Chassis_Lock_Mode;
	}
		
	// lock mode
	if (rc.sw1 == RC_UP || rc.sw1 == RC_DN)
	{
		chassis_mode = Chassis_Lock_Mode;
	}
}

static void kb_update(void)
{
	if (rc.kb.bit.Q)  // auto mode
	{
		chassis_mode = Chassis_Auto_Mode;
	}
	else if(rc.kb.bit.E)  // normal mode
	{
		chassis_mode = Chassis_Normal_Mode;
	}
}
