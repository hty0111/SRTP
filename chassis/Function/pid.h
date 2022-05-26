/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-22 18:28:03
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-22 19:26:58
 */
/** 
 ***********************************************************************
 * @file    pid.h
 * @author  HTY
 * @version v1.0
 * @date    2022.01.18
 * @brief   TODO
 ***********************************************************************
 * @attention TODO
 *  
 ***********************************************************************
 *  Copyright (c) 2022 HelloWorld, Zhejiang University.
 *  All Rights Reserved.
 ***********************************************************************
 */
 
#ifndef __PID_H__
#define __PID_H__

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"

// 底盘速度环
#define CHASSIS_SPEED_PID_KP 3.0f
#define CHASSIS_SPEED_PID_KI 0.0f
#define CHASSIS_SPEED_PID_KD 0.0f
#define CHASSIS_SPEED_PID_MAX_OUT 5000.f
#define CHASSIS_SPEED_PID_MAX_IOUT 1000.0f

// 整车角度环
#define CHASSIS_ANGLE_PID_KP 2.0f
#define CHASSIS_ANGLE_PID_KI 0.0f
#define CHASSIS_ANGLE_PID_KD 0.0f
#define CHASSIS_ANGLE_PID_MAX_OUT 3000.f
#define CHASSIS_ANGLE_PID_MAX_IOUT 500.0f

enum PID_MODE
{
    PID_POSITION = 0,
    PID_DELTA,
};

typedef struct
{
    uint8_t mode;
    float Kp;
    float Ki;
    float Kd;

    float max_out;
    float max_iout;

    float set;
    float fdb;

    float out;
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[3];
    float error[3];

} PidTypeDef;

extern PidTypeDef chassis_speed_pid_t[4];
extern PidTypeDef chassis_angle_pid_t;

extern float CHASSIS_SPEED_PID[3];
extern float CHASSIS_ANGLE_PID[3];

extern void PID_Clear(PidTypeDef *pid);
extern float PID_Calc(PidTypeDef *pid, float fdb, float set);
extern float PID_Loop_Calc(PidTypeDef *pid, float fdb, float set, PidTypeDef *inner_pid, float inner_fdb);
extern void PID_Init(void);

#endif
