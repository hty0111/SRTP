/*
 * @Description: 
 * @version: v1.0
 * @Author: Hello_World
 * @Date: 2022-03-21 23:26:48
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-22 16:47:48
 */

#include "can_receive.h"

CAN_RxHeaderTypeDef  RxMessageHeader;
CAN_TxHeaderTypeDef  TxMessageHeader;
Motor_Encoder_t ChassisEncoder[4];
uint8_t TxMessage[8];
uint32_t pTxMailbox = 0;    //传入HAL_CAN_AddTxMessage()提供存储空间

/**
* @brief  can filter的初始化，直接抄的官方代码
* @param  can的结构体
* @retval none
* @note   必须在主函数中初始化
**/
void CanFilter_Init(CAN_HandleTypeDef* hcan) {
    CAN_FilterTypeDef canfilter;

    canfilter.FilterMode = CAN_FILTERMODE_IDMASK;
    canfilter.FilterScale = CAN_FILTERSCALE_32BIT;

    //filtrate any ID you want here
    canfilter.FilterIdHigh = 0x0000;
    canfilter.FilterIdLow = 0x0000;
    canfilter.FilterMaskIdHigh = 0x0000;
    canfilter.FilterMaskIdLow = 0x0000;

    canfilter.FilterFIFOAssignment = CAN_FilterFIFO0;
    canfilter.FilterActivation = ENABLE;
    canfilter.SlaveStartFilterBank = 14;
    canfilter.FilterBank = 0;
	
    if (HAL_CAN_ConfigFilter(hcan, &canfilter) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
* @brief  编码器数据更新
* @param  编码器结构体指针
* @param  can接收到的原始数据
* @retval none
* @note   
**/
static void update_encoder(Motor_Encoder_t * encoder, uint8_t * data)
{
	encoder->last_angle_raw_value = encoder->angle_raw_value;
	encoder->angle_raw_value = (data[0] << 8 | data[1]);	
	encoder->rotate_speed = (data[2] << 8 | data[3]);
	encoder->angular_velocity = 2 * PI * encoder->rotate_speed / 60;	//单位转化到rad/s
	encoder->real_angle = encoder->angle_raw_value * 1.0 / 8191 * 360;  //单位转化到°
	// 角度统一映射至-180~180
	if(encoder->real_angle > 180)
		encoder->real_angle -= 360;
	else if(encoder->real_angle < -180)
		encoder->real_angle += 360;
}

/**
* @brief  can的数据解析
* @param  can的结构体
* @return none
* @note   只能通过Add和Get配置发送和接收的结构体指针
**/
void CAN_Data_Process(CAN_HandleTypeDef* hcan)
{
	uint8_t rawData[8];
	uint8_t index;
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxMessageHeader, rawData) == HAL_OK)		// 获得接收到的数据头和数据
	{	
		if(hcan == &hcan1)
		{
			switch (RxMessageHeader.StdId)
			{	
				case CHASSIS1_RxID:
				case CHASSIS2_RxID:
				case CHASSIS3_RxID:
				case CHASSIS4_RxID:
					index = RxMessageHeader.StdId - CHASSIS1_RxID;
					update_encoder(&ChassisEncoder[index], rawData);
					break;
			}
		}
    }
}

/**
* @brief  向can总线发送数据
* @param  can的结构体
* @param  实际要发送的电流或电压值
* @param  can的发送数组
* @param  发送ID
* @param  数据长度
* @retval none
* @note   
**/
void CAN_Send_Msg(CAN_HandleTypeDef* hcan, int32_t * current, uint8_t* TxMsg, uint32_t id, uint8_t len)
{
	TxMsg[0] = (unsigned char) (current[0] >> 8);
	TxMsg[1] = (unsigned char) current[0];
	TxMsg[2] = (unsigned char) (current[1] >> 8);
	TxMsg[3] = (unsigned char) current[1];
	TxMsg[4] = (unsigned char) (current[2] >> 8);
	TxMsg[5] = (unsigned char) current[2];
	TxMsg[6] = (unsigned char) (current[3] >> 8);
	TxMsg[7] = (unsigned char) current[3];
	TxMessageHeader.StdId = id;
	TxMessageHeader.IDE = CAN_ID_STD;
	TxMessageHeader.RTR = CAN_RTR_DATA;
	TxMessageHeader.DLC = len;
	if(HAL_CAN_AddTxMessage(hcan, &TxMessageHeader, TxMsg, (uint32_t*) pTxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
* @brief  can初始化，包括过滤器和中断
* @param  none
* @retval none
* @note   
**/
void CAN_Receive_Init(void)
{
	CanFilter_Init(&hcan1);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}
