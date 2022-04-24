/*
 * @Description: imu解算yaw轴数据
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-22 19:19:57
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-23 13:46:51
 */

#ifndef __IMU_TASK_H__
#define __IMU_TASK_H__

#include "main.h"
#include "bmi_receive.h"
#include "control_task.h"

// IMU原始数据结构体，已除以灵敏度
typedef struct {
	// 加速度数据，单位为g
	float ax;  
	float ay;
	float az;
	
	// 温度数据，单位为℃
	float temperature;  
	
	// 角速度数据，单位为degree/s
	float gx;  
	float gy;
	float gz;
	
	// 磁力数据，单位为uT
	float mx;  
	float my;
	float mz;
} imu_raw_data_t;

// imu解算后的数据结构体，指机器人坐标系而非世界坐标系
typedef struct {
	// imu本身x、y、z轴的角速度温漂，单位为degree/s
	float gx_offset;
	float gy_offset;
	float gz_offset;
	
	float yaw_angularv;				// yaw轴当前角速度，单位为degree/s
	float yaw_angle;				// yaw轴当前角度，单位为degree，范围为(-180, 180]
} imu_data_t;


extern imu_raw_data_t imu_raw_data;
extern imu_data_t imu_data;

void IMU_Get_Data(void);
void IMU_Task(void);


#endif

