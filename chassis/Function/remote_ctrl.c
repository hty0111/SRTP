/*
 * @Description: 
 * @version: 
 * @Author: Hello_World
 * @Date: 2022-03-21 23:57:44
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-22 18:51:57
 */


#include "remote_ctrl.h"

#define RC_OFFSET 660.0f

RcType rc, last_rc; // remote control data (dbus / keyboard&mouse)
uint8_t dbus_buff[DBUS_RX_MAX_BUFLEN] = {0};

/**
 ***********************************************************************
 * @brief   initialize dbus device
 * @note    after MX_USART#_UART_Init() use this function in main.c
 ***********************************************************************
 */
void Dbus_Init()
{
	HAL_UART_Receive_DMA(&DBUS_HUART, dbus_buff, DBUS_RX_MAX_BUFLEN);
}

/**
 ***********************************************************************
 * @brief    Rx buffer handle，rc structure assignment
 * @param    RC_Type *rc
 * @param	 uint8_t *buff
 * @note     None
 ***********************************************************************
 */
void RC_Data_Handler(RcType *rc, uint8_t *buff)
{
	HAL_IWDG_Refresh(&hiwdg);

	if (buff[0] == 0 && buff[1] == 0 && buff[2] == 0 &&
		buff[3] == 0 && buff[4] == 0 && buff[5] == 0)
		return;

	rc->ch1 = (buff[0] | buff[1] << 8) & 0x07FF;
	rc->ch1 -= 1024;
	rc->ch2 = (buff[1] >> 3 | buff[2] << 5) & 0x07FF;
	rc->ch2 -= 1024;
	rc->ch3 = (buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF;
	rc->ch3 -= 1024;
	rc->ch4 = (buff[4] >> 1 | buff[5] << 7) & 0x07FF;
	rc->ch4 -= 1024;

	rc->RH = rc->ch1 / RC_OFFSET;
	rc->RV = rc->ch2 / RC_OFFSET;
	rc->LH = rc->ch3 / RC_OFFSET;
	rc->LV = rc->ch4 / RC_OFFSET;

	rc->sw1 = ((buff[5] >> 4) & 0x000C) >> 2;
	rc->sw2 = (buff[5] >> 4) & 0x0003;

	rc->mouse.x = buff[6] | (buff[7] << 8); // x axis
	rc->mouse.y = buff[8] | (buff[9] << 8);
	rc->mouse.z = buff[10] | (buff[11] << 8);

	rc->mouse.l = buff[12];
	rc->mouse.r = buff[13];

	rc->kb.key_code = buff[14] | buff[15] << 8; // key board code
	rc->wheel = buff[16] | buff[17] << 8;
}

