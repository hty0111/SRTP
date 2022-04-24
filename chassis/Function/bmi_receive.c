/*
 * @Description: BMI088数据解算
 * @version: v1.0
 * @Author: Hello_World
 * @Date: 2022-03-23 10:11:06
 * @LastEditors: HTY
 * @LastEditTime: 2022-03-23 13:46:55
 */

/* ------------------------------ Include ------------------------------ */
#include "bmi_receive.h"

/* ------------------------------ Macro Definition ------------------------------ */
//管脚定义
//加速度计片选信号（包括温度传感器）
#define CS1_ACCEL_Pin GPIO_PIN_4
#define CS1_ACCEL_GPIO_Port GPIOA
//加速度计外部中断
#define INT1_ACCEL_Pin GPIO_PIN_4
#define INT1_ACCEL_GPIO_Port GPIOC

//陀螺仪片选信号
#define CS1_GYRO_Pin GPIO_PIN_0
#define CS1_GYRO_GPIO_Port GPIOB
//陀螺仪外部中断
#define INT1_GYRO_Pin GPIO_PIN_5
#define INT1_GYRO_GPIO_Port GPIOC

//加热电阻
#define TIM10_CH1_Pin GPIO_PIN_6
#define TIM10_CH1_GPIO_Port GPIOF
#define HEAT_CH	(TIM10->CCR1)		//定时器比较寄存器，对应通道1
#define TEMP_ADD	15		//期望温度比实际温度高15℃
#define HEAT_CH_MAX 5000	//最大加热功率

#define BMI088_COM_WAIT_SENSOR_TIME		150		// 通信等待需延时150us
#define BMI088_LONG_DELAY_TIME			80		// 有些设置后需要较长延时，设为80ms

/* ------------------------------ Function Declaration (only used in this .c file) ------------------------------ */
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
/* ------------------------------ Function Definition ------------------------------ */


/**
 * @brief   拉高陀螺仪的片选信号
 * @param   none
 * @retval  none
 * @note    
**/
void BMI088_GYRO_NSS_High(void)
{
	HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
}

/**
 * @brief   拉低陀螺仪的片选信号
 * @param   none
 * @retval  none
 * @note    
**/
void BMI088_GYRO_NSS_Low(void)
{
	HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_RESET);
}

/**
 * @brief   拉高加速度计的片选信号
 * @param   none
 * @retval  none
 * @note    
**/
void BMI088_ACCEL_NSS_High(void)
{
	HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);
}

/**
 * @brief   拉低加速度计的片选信号
 * @param   none
 * @retval  none
 * @note    
**/
void BMI088_ACCEL_NSS_Low(void)
{
	HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_RESET);
}

/**
* @brief  	向BMI088的加速度计的一个寄存器写入值
* @param  	reg: 加速度计寄存器地址
* @param	data: 待写入寄存器的数据
* @retval 	0
* @note
**/
uint8_t BMI088_Write_Accel_Single_Reg(uint8_t const reg, uint8_t const data)
{
		uint8_t BMI_Rx, BMI_Tx;
		
		BMI088_ACCEL_NSS_Low();
	
		BMI_Tx = reg&0x7f;  // 第一个bit为0表示写
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		BMI_Tx = data;
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
	
		BMI088_ACCEL_NSS_High();
	
		return 0;
}	

/**
* @brief  读取BMI088的加速度计的一个寄存器的值
* @param  reg: 加速度计寄存器地址
* @retval BMI_Rx: 读取出的值
* @note		第一次发寄存器地址，没有收；
			第二次是因为加速度计返回的第一个字节是无效的，所以要再等一个字节；
			第三次才是接收到的值
**/
uint8_t BMI088_Read_Accel_Single_Reg(uint8_t const reg)
{
		uint8_t BMI_Rx, BMI_Tx;
		
		BMI088_ACCEL_NSS_Low();
	
		BMI_Tx = reg|0x80;  // 第一个bit为1表示读
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
	
		BMI088_ACCEL_NSS_High();
	
		return BMI_Rx;
}	

/**
* @brief 	读取BMI088的加速度计的多个寄存器的值
* @param  	BMI_Rx_Data: 待读取数组，这样不需要用return的形式来返回
* @retval 	0
* @note		读取6个加速度计相关值，地址为0x12~0x17；接收数组前要再等一个字节
**/
void BMI088_Read_Accel_Multi_Reg(uint8_t *bmi_rx_data)
{	
		uint8_t BMI_Rx, BMI_Tx, len = 6;  // 不要加static，否则无法正常读取
		
		BMI088_ACCEL_NSS_Low();
	
		BMI_Tx = BMI088_ACCEL_XOUT_L|0x80;  // 第一个bit为1表示读
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		
		while (len!=0){
				HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &bmi_rx_data[6-len], 1, 55);
				len--;
		}

		BMI088_ACCEL_NSS_High();	
}	

/**
* @brief  	向BMI088的陀螺仪的一个寄存器写入值
* @param  	reg: 陀螺仪寄存器地址
* @param	data: 待写入寄存器的数据
* @retval 	0
* @note
**/
void BMI088_Write_Gyro_Single_Reg(uint8_t const reg, uint8_t const data)
{
		uint8_t BMI_Rx, BMI_Tx;
		
		BMI088_GYRO_NSS_Low();
	
		BMI_Tx = reg&0x7f;  // 第一个bit为0表示写
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		BMI_Tx = data;
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
	
		BMI088_GYRO_NSS_High();
}	

/**
* @brief  读取BMI088的陀螺仪的一个寄存器的值
* @param  reg: 陀螺仪寄存器地址
* @retval BMI_Rx: 读取出的值
* @note
**/
uint8_t BMI088_Read_Gyro_Single_Reg(uint8_t const reg)
{
		uint8_t BMI_Rx, BMI_Tx;
		
		BMI088_GYRO_NSS_Low();
	
		BMI_Tx = reg|0x80;  // 第一个bit为1表示读
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
	
		BMI088_GYRO_NSS_High();
	
		return BMI_Rx;
}	

/**
* @brief  读取BMI088的陀螺仪的多个寄存器的值
* @param  BMI_Rx_Data: 待读取数组，这样不需要用return的形式来返回
* @retval 0
* @note		读取6个陀螺仪相关值，地址为0x02~0x07
**/
void BMI088_Read_Gyro_Multi_Reg(uint8_t *BMI_Rx_Data)
{	
		uint8_t BMI_Rx, BMI_Tx, len = 6;
		
		BMI088_GYRO_NSS_Low();
	
		BMI_Tx = BMI088_GYRO_X_L|0x80;  // 第一个bit为1表示读
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		
		while (len!=0){
				HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx_Data[6-len], 1, 55);
				len--;
		}

		BMI088_GYRO_NSS_High();
}	

/**
* @brief  	读取BMI088的温度传感器的多个寄存器的值
* @param  	BMI_Rx_Data: 待读取数组，这样不需要用return的形式来返回
* @retval 	0
* @note		读取2个加速度计中与温度相关的值，地址为0x22~0x23；接收数组前要再等一个字节
**/
void BMI088_Read_Temp_Multi_Reg(uint8_t *BMI_Rx_Data)
{		
		uint8_t BMI_Rx, BMI_Tx, len = 2;
		
		BMI088_ACCEL_NSS_Low();
	
		BMI_Tx = BMI088_TEMP_M|0x80;  // 第一个bit为1表示读
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx, 1, 55);
		
		while (len!=0){
				HAL_SPI_TransmitReceive(&hspi1, &BMI_Tx, &BMI_Rx_Data[2-len], 1, 55);
				len--;
		}

		BMI088_ACCEL_NSS_High();
}	

/**
* @brief  BMI088加速度计的自检
* @param  none
* @retval 0表示没有错误，其他对应于HW_imu.h中的错误值
* @note		删掉了一些RM-C示例代码中读BMI088_ACC_CHIP_ID两次
**/
uint8_t BMI088_Accel_Self_Test(void)
{
		// 配置一些需要配置的寄存器，并定义对应的错误
		uint8_t BMI088_Accel_Self_Test_Config[6][3] = {
				// NORMAL|MUST_Set为[7:4]，影响LFP的带宽，为Normal；800_Hz为[3:0]，ODR为800Hz，为加速度自检的推荐值
				{BMI088_ACC_CONF, BMI088_ACC_NORMAL|BMI088_ACC_1600_HZ|BMI088_ACC_CONF_MUST_Set, BMI088_ACC_CONF_ERROR},
				// turn on the accelerometer，每次reset后都要到on才能获取数据
				{BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR},
				// measurement range测量范围为±24g，为加速度计自检的推荐值
				{BMI088_ACC_RANGE, BMI088_ACC_RANGE_24G, BMI088_ACC_RANGE_ERROR},
				// suspend mode用于save power，但不能读取数据，所以要用active mode
				{BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, BMI088_ACC_PWR_CONF_ERROR},
				// positive自检
				{BMI088_ACC_SELF_TEST, BMI088_ACC_SELF_TEST_POSITIVE_SIGNAL, BMI088_ACC_PWR_CONF_ERROR},
				// negative自检
				{BMI088_ACC_SELF_TEST, BMI088_ACC_SELF_TEST_NEGATIVE_SIGNAL, BMI088_ACC_PWR_CONF_ERROR}
		};
		uint8_t read_value;  // 写入后再读取的值，用来检查是否正确配置
		uint8_t tmp_buff[6];  // 读取的数组
		int16_t self_test_accel[2][3];  // 保存positive和negative自检读取的加速度值
		
		// reset
		BMI088_Write_Accel_Single_Reg(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
		HAL_Delay(60);
		
		BMI088_ACCEL_NSS_Low();
		HAL_Delay(50);
		BMI088_ACCEL_NSS_High();
		
		read_value = BMI088_Read_Accel_Single_Reg(BMI088_ACC_CHIP_ID);
		HAL_Delay(1);
    if (read_value!=BMI088_ACC_CHIP_ID_VALUE){
        return BMI088_NO_SENSOR;
    }
		
		for(uint8_t i = 0; i<4; i++){
				BMI088_Write_Accel_Single_Reg(BMI088_Accel_Self_Test_Config[i][0], BMI088_Accel_Self_Test_Config[i][1]);
				HAL_Delay(1);
				if (i==3){  // 当power mode改变后，需要5ms来settle，期间避免通信
						HAL_Delay(5);
				}
				read_value = BMI088_Read_Accel_Single_Reg(BMI088_Accel_Self_Test_Config[i][0]);
				HAL_Delay(1);
				if (read_value!=BMI088_Accel_Self_Test_Config[i][1]){
						return BMI088_Accel_Self_Test_Config[i][2];  // 返回出错的第一个，其他就不继续了
				}
		}
		
		for(uint8_t i = 0; i<2; i++){
				BMI088_Write_Accel_Single_Reg(BMI088_Accel_Self_Test_Config[i+4][0], BMI088_Accel_Self_Test_Config[i+4][1]);
				HAL_Delay(60);  // enable positive/negative self test需要延时大于50ms
				read_value = BMI088_Read_Accel_Single_Reg(BMI088_Accel_Self_Test_Config[i+4][0]);
				HAL_Delay(1);
				if (read_value!=BMI088_Accel_Self_Test_Config[i+4][1]){
						return BMI088_Accel_Self_Test_Config[i+4][2];  // 返回出错的第一个，其他就不继续了
				}
				
				BMI088_Read_Temp_Multi_Reg(tmp_buff);

        self_test_accel[i][0] = (int16_t) tmp_buff[1]<<8 | tmp_buff[0];
        self_test_accel[i][1] = (int16_t) tmp_buff[3]<<8 | tmp_buff[2];
        self_test_accel[i][2] = (int16_t) tmp_buff[5]<<8 | tmp_buff[4];
		}
		
		// 取消自检模式
		BMI088_Write_Accel_Single_Reg(BMI088_ACC_SELF_TEST, BMI088_ACC_SELF_TEST_OFF);
		HAL_Delay(1);
		read_value = BMI088_Read_Accel_Single_Reg(BMI088_ACC_SELF_TEST);
		if (read_value!=(BMI088_ACC_SELF_TEST_OFF)){
        return BMI088_ACC_SELF_TEST_ERROR;
    }
		
		// reset
		BMI088_Write_Accel_Single_Reg(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
		HAL_Delay(60);
		
		if ((self_test_accel[0][0]-self_test_accel[1][0]<1365) || (self_test_accel[0][1]-self_test_accel[1][1]<1365) || (self_test_accel[0][2]-self_test_accel[1][2]<680))
    {
        return BMI088_SELF_TEST_ACCEL_ERROR;
    }
		
		return BMI088_NO_ERROR;
}

/**
* @brief  初始化BMI088的加速度计配置
* @param  none
* @retval 0表示没有错误，其他对应于HW_imu.h中的错误值
* @note		2020.6.5：没有看懂RM-C示例代码中为什么要先读两次，再写入BMI088_ACC_SOFTRESET寄存器
					来reset，然后再读两次；并且也不懂为什么一般延时150us，而reset写后延时80ms
					本代码中直接先写，后读，并且直接用HAL_Delay()延时一点
					2020.6.6：由于这一版失败了，所以还是乖乖地抄了RM-C示例代码中的流程
**/
uint8_t BMI088_Accel_Init(void)
{
		// 配置一些需要配置的寄存器，并定义对应的错误
		uint8_t BMI088_Accel_Init_Config[6][3] = {
				// turn on the accelerometer，每次reset后都要到on才能获取数据
				{BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR},
				// suspend mode用于save power，但不能读取数据，所以要用active mode
				{BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, BMI088_ACC_PWR_CONF_ERROR},
				// NORMAL|MUST_Set为[7:4]，影响LFP的带宽，为Normal；800_Hz为[3:0]，输出数据速率（output data rate，ODR）为800Hz
				{BMI088_ACC_CONF, BMI088_ACC_NORMAL|BMI088_ACC_800_HZ|BMI088_ACC_CONF_MUST_Set, BMI088_ACC_CONF_ERROR},
				// measurement range测量范围为±3g
				{BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G, BMI088_ACC_RANGE_ERROR},
				// enable INT1 as output pin, pin behaviour为push-pull, active low
				{BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE|BMI088_ACC_INT1_GPIO_PP|BMI088_ACC_INT1_GPIO_LOW, BMI088_INT1_IO_CTRL_ERROR},
				// Map data ready interrupt to pin INT1
				{BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT, BMI088_INT_MAP_DATA_ERROR}
		};
		uint8_t read_value;  // 写入后再读取的值，用来检查是否正确配置
		
		// check communication
		read_value = BMI088_Read_Accel_Single_Reg(BMI088_ACC_CHIP_ID);
		bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
		read_value = BMI088_Read_Accel_Single_Reg(BMI088_ACC_CHIP_ID);
		bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
		
		// 向0x7E寄存器写入0xB6就会reset，后面要加1ms延时才能使configuration setting都被覆盖为reset value
		BMI088_Write_Accel_Single_Reg(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
		HAL_Delay(BMI088_LONG_DELAY_TIME);
		
		// check communication is normal after reset
		read_value = BMI088_Read_Accel_Single_Reg(BMI088_ACC_CHIP_ID);
		bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
		read_value = BMI088_Read_Accel_Single_Reg(BMI088_ACC_CHIP_ID);
		bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
		
		// check the "who am I"
    if (read_value!=BMI088_ACC_CHIP_ID_VALUE){
        return BMI088_NO_SENSOR;
    }
		
		// 这里与上面的6个需要配置的寄存器相对应
		for(uint8_t i = 0; i<6; i++){
				BMI088_Write_Accel_Single_Reg(BMI088_Accel_Init_Config[i][0], BMI088_Accel_Init_Config[i][1]);
				bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

				read_value = BMI088_Read_Accel_Single_Reg(BMI088_Accel_Init_Config[i][0]);
				bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
				if (read_value!=BMI088_Accel_Init_Config[i][1]){
						return BMI088_Accel_Init_Config[i][2];  // 返回出错的第一个，其他就不继续了
				}
		}
		
		return BMI088_NO_ERROR;
}

/**
* @brief  BMI088陀螺仪的自检
* @param  none
* @retval 0表示没有错误，其他对应于HW_imu.h中的错误值
* @note		删掉了一些RM-C示例代码中读BMI088_GYRO_CHIP_ID两次
**/
uint8_t BMI088_Gyro_Self_Test(void)
{
		uint8_t read_value;  // 写入后再读取的值，用来检查是否正确配置
		uint8_t retry = 0;
		
		// reset
		BMI088_Write_Gyro_Single_Reg(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
		HAL_Delay(60);
		
		read_value = BMI088_Read_Gyro_Single_Reg(BMI088_GYRO_CHIP_ID);
		HAL_Delay(1);
    if (read_value!=BMI088_GYRO_CHIP_ID_VALUE){
        return BMI088_NO_SENSOR;
    }
		
		BMI088_Write_Gyro_Single_Reg(BMI088_GYRO_SELF_TEST, BMI088_GYRO_TRIG_BIST);
		HAL_Delay(60);
		
		do{
				read_value = BMI088_Read_Gyro_Single_Reg(BMI088_GYRO_SELF_TEST);
				HAL_Delay(1);
				retry++;
		}while(!(read_value&BMI088_GYRO_BIST_RDY) && retry<10);

    if (retry==10){
        return BMI088_SELF_TEST_GYRO_ERROR;
    }

    if (read_value&BMI088_GYRO_BIST_FAIL){
        return BMI088_SELF_TEST_GYRO_ERROR;
    }
		
		return BMI088_NO_ERROR;
}

/**
* @brief  初始化BMI088的陀螺仪配置
* @param  none
* @retval 0表示没有错误，其他对应于HW_imu.h中的错误值
* @note		没有看懂RM-C示例代码中为什么要先读两次，再写入BMI088_ACC_SOFTRESET寄存器
					来reset，然后再读两次；并且也不懂为什么一般延时150us，而reset写后延时80ms
					本代码中直接先写，后读，并且直接用HAL_Delay()延时一点
**/
uint8_t BMI088_Gyro_Init(void)
{
		// 配置一些需要配置的寄存器，并定义对应的错误
		uint8_t BMI088_Gyro_Init_Config[6][3] = {
				// measurement range测量范围为±2000°/s
				{BMI088_GYRO_RANGE, BMI088_GYRO_2000, BMI088_GYRO_RANGE_ERROR},
				// ODR为1000Hz，Filter bandwidth为116Hz
				{BMI088_GYRO_BANDWIDTH, BMI088_GYRO_1000_116_HZ|BMI088_GYRO_BANDWIDTH_MUST_Set, BMI088_GYRO_BANDWIDTH_ERROR},
				// power mode为normal
				{BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, BMI088_GYRO_LPM1_ERROR},
				// enable the new data interrupt to be triggered on new data
				{BMI088_GYRO_CTRL, BMI088_DRDY_ON, BMI088_GYRO_CTRL_ERROR},
				// 对INT3，pin behaviour为push-pull, active low
				{BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP|BMI088_GYRO_INT3_GPIO_LOW, BMI088_GYRO_INT3_INT4_IO_CONF_ERROR},
				// Data ready interrupt is mapped to INT3 pin. 
				{BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3, BMI088_GYRO_INT3_INT4_IO_MAP_ERROR}
		};
		uint8_t read_value;  // 写入后再读取的值，用来检查是否正确配置
		
		// check commiunication
		read_value = BMI088_Read_Gyro_Single_Reg(BMI088_GYRO_CHIP_ID);
		bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
		read_value = BMI088_Read_Gyro_Single_Reg(BMI088_GYRO_CHIP_ID);
		bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
		
		BMI088_Write_Gyro_Single_Reg(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
		HAL_Delay(BMI088_LONG_DELAY_TIME);
		
		// check commiunication is normal after reset
		read_value = BMI088_Read_Gyro_Single_Reg(BMI088_GYRO_CHIP_ID);
		bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
		read_value = BMI088_Read_Gyro_Single_Reg(BMI088_GYRO_CHIP_ID);
		bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
		
		// check the "who am I"
    if (read_value!=BMI088_GYRO_CHIP_ID_VALUE)
	{
        return BMI088_NO_SENSOR;
    }
		
		// 这里与上面的6个需要配置的寄存器相对应
		for(uint8_t i = 0; i<6; i++)
		{
			BMI088_Write_Gyro_Single_Reg(BMI088_Gyro_Init_Config[i][0], BMI088_Gyro_Init_Config[i][1]);
			bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
			read_value = BMI088_Read_Gyro_Single_Reg(BMI088_Gyro_Init_Config[i][0]);
			bmi_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
			if (read_value!=BMI088_Gyro_Init_Config[i][1])
			{
				return BMI088_Gyro_Init_Config[i][2];  // 返回出错的第一个，其他就不继续了
			}
		}
		
		return BMI088_NO_ERROR;
}

/**
* @brief  初始化BMI088的管脚
* @param  none
* @retval none
* @note		none				
**/
void BMI088_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS1_ACCEL_GPIO_Port, CS1_ACCEL_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS1_GYRO_GPIO_Port, CS1_GYRO_Pin, GPIO_PIN_SET);
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CS1_ACCEL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS1_ACCEL_GPIO_Port, &GPIO_InitStruct);
	
	/*Configure GPIO pins : PCPin PCPin */
  GPIO_InitStruct.Pin = INT1_ACCEL_Pin|INT1_GYRO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CS1_GYRO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(CS1_GYRO_GPIO_Port, &GPIO_InitStruct);
}

/**
* @brief  初始化BMI088
* @param  none
* @retval 0表示没有错误，其他对应于HW_imu.h中的错误值
* @note		
**/
uint8_t BMI088_Init(void)
{
	uint8_t error = BMI088_NO_ERROR;
	BMI088_GPIO_Init();
	HEAT_CH = 0;		//初始化关闭加热电阻

	error |= BMI088_Accel_Init();
	error |= BMI088_Gyro_Init();
	
    return error;
}

/**
 * @brief   微秒级延时，用于BMI088的初始化操作
 * @param   us为需要延时的时间，单位为us
 * @retval  none
 * @note    
**/
void bmi_delay_us(uint16_t us)
{
	uint32_t ticks = 0;
	uint32_t told = 0;
	uint32_t tnow = 0;
	uint32_t tcnt = 0;
	uint32_t reload = 0;

	reload = SysTick->LOAD;
	ticks = us*168;
	told = SysTick->VAL;

	while (1){
		tnow = SysTick->VAL;
		if (tnow!=told){
			if (tnow<told){
				tcnt += told-tnow;
			}
			else{
				tcnt += reload-tnow+told;
			}
			told = tnow;
			if (tcnt>=ticks){
				break;
			}
		}
	}
}

