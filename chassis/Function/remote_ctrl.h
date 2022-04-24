/*
 * @Description: 
 * @version: v1.0
 * @Author: Hello_World
 * @Date: 2022-03-21 23:57:58
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-22 19:09:57
 */

#ifndef __REMOTE_CTRL_H__
#define __REMOTE_CTRL_H__

#include "usart.h"
#include "iwdg.h"

#define DBUS_HUART huart3
#define DBUS_RX_MAX_BUFLEN 18
#define RC_UP 1
#define RC_DN 2
#define RC_MI 3

typedef struct
{
	float LV;
	float LH;
	float RV;
	float RH;
	// ch value: -660 ~ 660
	int16_t ch1;
	int16_t ch2;
	int16_t ch3;
	int16_t ch4;
	// switch val: 1 3 2
	uint8_t sw1;
	uint8_t sw2;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z; // no use
		// press:1 release:0
		uint8_t l;
		uint8_t r;
	} mouse;

	union
	{
		uint16_t key_code;
		/**********************************************************************************
		 * keyboard :15   14   13   12   11   10   9   8   7   6     5     4   3   2   1
		 *            V    C    X	 Z    G    F   R   E   Q  CTRL  SHIFT  D   A   S   W
		 ************************************************************************************/
		struct
		{
			uint16_t W : 1;
			uint16_t S : 1;
			uint16_t A : 1;
			uint16_t D : 1;
			uint16_t SHIFT : 1;
			uint16_t CTRL : 1;
			uint16_t Q : 1;
			uint16_t E : 1;
			uint16_t R : 1;
			uint16_t F : 1;
			uint16_t G : 1;
			uint16_t Z : 1;
			uint16_t X : 1;
			uint16_t C : 1;
			uint16_t V : 1;
			uint16_t B : 1;
		} bit;
	} kb;
	int16_t wheel;
} RcType;

extern RcType rc, last_rc;
extern uint8_t dbus_buff[DBUS_RX_MAX_BUFLEN];

void Dbus_Init(void);
void RC_Data_Handler(RcType *rc, uint8_t *buff);

#endif

