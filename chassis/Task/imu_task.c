/*
 * @Description: imu解算yaw轴数据
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-22 19:19:46
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-23 13:46:54
 */


#include "imu_task.h"

imu_raw_data_t imu_raw_data = {0};  // IMU原始数据结构体
imu_data_t imu_data;

void IMU_Get_Gyro_Data(void);
void IMU_Get_Accel_Data(void);
void IMU_Get_Data(void);

/**
 * @brief   读取imu数据
 * @param   none
 * @retval  none
 * @note    分别调用两个函数读取角速度和温度数据
**/
void IMU_Get_Data(void)
{		
	IMU_Get_Gyro_Data();
	IMU_Get_Accel_Data();
}

/**
 * @brief   读取imu角速度数据
 * @param   none
 * @retval  none
 * @note    none
**/
uint8_t gyro_buff[6];
void IMU_Get_Gyro_Data(void)
{		
	//uint8_t gyro_buff[6];
	int16_t tmp;

	BMI088_Read_Gyro_Multi_Reg(gyro_buff);
	
	tmp = (int16_t) ((gyro_buff[1]<<8) | gyro_buff[0]);
	imu_raw_data.gx = tmp/GYRO_SENSITIVITY_2000;
	tmp = (int16_t) ((gyro_buff[3]<<8) | gyro_buff[2]);
	imu_raw_data.gy = tmp/GYRO_SENSITIVITY_2000;
	tmp = (int16_t) ((gyro_buff[5]<<8) | gyro_buff[4]);
	imu_raw_data.gz = tmp/GYRO_SENSITIVITY_2000;		
}

/**
 * @brief   读取imu加速度数据
 * @param   none
 * @retval  none
 * @note    none
**/
void IMU_Get_Accel_Data(void)
{		
	uint8_t accel_buff[6];
	int16_t tmp;

	BMI088_Read_Accel_Multi_Reg(accel_buff);
	
	tmp = (int16_t) ((accel_buff[1]<<8) | accel_buff[0]);
	imu_raw_data.ax = tmp/ACCEL_SENSITIVITY_3G;
	tmp = (int16_t) ((accel_buff[3]<<8) | accel_buff[2]);
	imu_raw_data.ay = tmp/ACCEL_SENSITIVITY_3G;
	tmp = (int16_t) ((accel_buff[5]<<8) | accel_buff[4]);
	imu_raw_data.az = tmp/ACCEL_SENSITIVITY_3G;		
}


/**
 * @brief   imu数据解算任务函数
 * @param   none
 * @retval  none
 * @note    需要在control_task.c中调用
**/
void IMU_Task()
{
	IMU_Get_Data();  // 读取imu的原始数据
	if (control_tick < OFFSET_CALC_TIME)  // 计算零漂
	{
		imu_data.gx_offset += imu_raw_data.gx/OFFSET_CALC_TIME;
		imu_data.gy_offset += imu_raw_data.gy/OFFSET_CALC_TIME;
		imu_data.gz_offset += imu_raw_data.gz/OFFSET_CALC_TIME;
	}
	else
	{
		imu_data.yaw_angularv = -(imu_raw_data.gz-imu_data.gz_offset);
    	imu_data.yaw_angle += (imu_data.yaw_angularv * CONTROL_INTERVAL);  // 用矩形估计积分，时长为1ms
		YAW_ANGLE_LIMIT(imu_data.yaw_angle);  // 将yaw轴数据约束到(-180, 180]
    }
}
