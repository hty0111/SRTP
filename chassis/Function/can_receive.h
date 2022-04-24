/*
 * @Description: 
 * @version: v1.0
 * @Author: Hello_World
 * @Date: 2022-03-21 23:26:59
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-22 16:47:26
 */

#ifndef __CAN_RECEIVE_H__
#define __CAN_RECEIVE_H__

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "can.h"

/*底盘4个3508回传ID*/
#define CHASSIS1_RxID    0x201
#define CHASSIS2_RxID    0x202
#define CHASSIS3_RxID    0x203
#define CHASSIS4_RxID    0x204

// 主控板发给电调/电机的标识符
#define M3508_1_to_4_ID 		0x200
#define M3508_5_to_8_ID 		0x1ff
#define M2006_1_to_4_ID 		0x200
#define M2006_5_to_8_ID 		0x1ff
#define GM6020_1_to_4_ID 		0x1ff
#define GM6020_5_to_8_ID 		0x2ff

// 发送数据长度
#define CAN_Send_Len 	8

#ifndef PI
#define PI 3.1415926f
#endif

// 角度单位为°，角速度单位为rad/s
typedef struct
{
    int16_t rotate_speed;				// RPM
    int16_t angle_raw_value;            // 码盘原始值0-8191
    int16_t last_angle_raw_value;		// 上一次的原始值
	int16_t current;					// 转矩电流
    float real_angle;                   // 映射到0-360的值(°)
	float angular_velocity;				// 角速度(rad/s)
} Motor_Encoder_t;

extern Motor_Encoder_t YawEncoder;
extern Motor_Encoder_t PitchEncoder;
extern Motor_Encoder_t ChassisEncoder[4];
extern Motor_Encoder_t FeedEncoder;
extern Motor_Encoder_t MagazineEncoder;
extern Motor_Encoder_t FricEncoder[2];
extern uint8_t TxMessage[8];

void CAN_Data_Process(CAN_HandleTypeDef* hcan);
void CAN_Send_Msg(CAN_HandleTypeDef* hcan, int32_t * current, uint8_t* TxMsg, uint32_t id, uint8_t len);
void CAN_Receive_Init(void);

#endif

