/*
 * @Description: BMI088数据解算
 * @version: v1.0
 * @Author: Hello_World
 * @Date: 2022-03-23 10:11:15
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-23 13:46:55
 */


#ifndef _HW_BMI088_H_
#define _HW_BMI088_H_

/* ------------------------------ Include ------------------------------ */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"

#include "spi.h"
#include "tim.h"

#include "BMI088_reg.h"
#include "pid.h"
#include "can_receive.h"


/* ------------------------------ Macro Definition ------------------------------ */
#define TEMP_INIT_TIME 1000  		//开机后测量的时间，单位为ms
#define TEMP_CALC_TIME 10000		//温度调节时间，单位为ms
#define OFFSET_CALC_TIME 5000	//温漂测量时间，单位为ms
#define TEMP_ADD	15				//期望温度比实际温度高5℃
#define TEMP_ACC  0.125		//温度传感器测量精度0.125℃

#ifndef RADIAN_TO_DEGREE
	#define RADIAN_TO_DEGREE 57.296f  // 弧度制到角度值，180/PI
#endif

// GYRO_SENSITIVITY_x，则角速度量程为±x degree/s
// 从陀螺仪寄存器读到的数据需要除以对应灵敏度（即32768/x）才得到单位为degree/s的数据
#define GYRO_SENSITIVITY_125	263.144f
#define GYRO_SENSITIVITY_250	131.072f
#define GYRO_SENSITIVITY_500	65.536f
#define GYRO_SENSITIVITY_1000	32.768f
#define GYRO_SENSITIVITY_2000	16.384f
//加速度计灵敏度
#define ACCEL_SENSITIVITY_3G 	10920.0f
#define ACCEL_SENSITIVITY_6G 	5460.0f
#define ACCEL_SENSITIVITY_12G 	2730.0f
#define ACCEL_SENSITIVITY_24G 	1365.0f
//磁力计灵敏度
#define MEG_SENSITIVITY 0.3f
// 从传感器的寄存器读取到的数据需要乘以TEMP_SLOPE（0.125），并加上偏移TEMP_OFFSET（+23）才能得到温度数据（℃）
#define TEMP_SLOPE 0.125
#define TEMP_OFFSET 23


// 限制云台yaw轴在(-180, 180]
#define YAW_ANGLE_LIMIT(val)\
do {\
	if ((val) > 180)\
	{\
		(val) -= 360;\
	}\
	else if ((val) <= -180)\
	{\
		(val) += 360;\
	}\
} while(0)\

// 定义BMI088传感器的各种错误码
typedef enum {
    BMI088_NO_ERROR = 0x00,
	
    BMI088_ACC_PWR_CTRL_ERROR = 0x01,
    BMI088_ACC_PWR_CONF_ERROR = 0x02,
    BMI088_ACC_CONF_ERROR = 0x03,
    BMI088_ACC_SELF_TEST_ERROR = 0x04,
    BMI088_ACC_RANGE_ERROR = 0x05,
    BMI088_INT1_IO_CTRL_ERROR = 0x06,
    BMI088_INT_MAP_DATA_ERROR = 0x07,
    BMI088_GYRO_RANGE_ERROR = 0x08,
    BMI088_GYRO_BANDWIDTH_ERROR = 0x09,
    BMI088_GYRO_LPM1_ERROR = 0x0A,
    BMI088_GYRO_CTRL_ERROR = 0x0B,
    BMI088_GYRO_INT3_INT4_IO_CONF_ERROR = 0x0C,
    BMI088_GYRO_INT3_INT4_IO_MAP_ERROR = 0x0D,

    BMI088_SELF_TEST_ACCEL_ERROR = 0x80,
    BMI088_SELF_TEST_GYRO_ERROR = 0x40,
    BMI088_NO_SENSOR = 0xFF,
} bmi_error_type;

/* ------------------------------ Function Declaration (used in other .c files) ------------------------------ */
uint8_t BMI088_Init(void);
void BMI088_GYRO_NSS_High(void);
void BMI088_GYRO_NSS_Low(void);
void BMI088_ACCEL_NSS_High(void);
void BMI088_ACCEL_NSS_Low(void);
void BMI088_Write_Gyro_Single_Reg(uint8_t const reg, uint8_t const data);
uint8_t BMI088_Read_Gyro_Single_Reg(uint8_t const reg);
void BMI088_Read_Gyro_Multi_Reg(uint8_t *bmi_rx_data);
uint8_t BMI088_Read_Accel_Single_Reg(uint8_t const reg);
void BMI088_Read_Accel_Multi_Reg(uint8_t *bmi_rx_data);
void BMI088_Read_Temp_Multi_Reg(uint8_t *bmi_rx_data);
uint8_t BMI088_Gyro_Init(void);
uint8_t BMI088_Accel_Init(void);
void bmi_delay_us(uint16_t us);

#endif
