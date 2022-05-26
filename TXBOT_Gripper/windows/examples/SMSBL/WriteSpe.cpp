/*
以下例子在SMS40BL中测试通过，舵机出厂速度单位V=0为停止状态
*/

#include "stdafx.h"
#include <iostream>
#include "../../SCServo/SCServo.h"

SMSBL sm;

int main(int argc, char **argv)
{
	if(argc<2){
        std::cout<< "argc error!"<<std::endl;
		getchar();
        return 0;
	}
	std::cout<< "serial:"<<argv[1]<<std::endl;
    if(!sm.begin(115200, argv[1])){
        std::cout<< "Failed to init smsbl motor!"<<std::endl;
		getchar();
        return 0;
    }
	sm.WheelMode(1);//恒速模式
	std::cout<< "mode="<<1<<std::endl;
	while(1){
		sm.WriteSpe(1, 80, 100);//舵机(ID1)以最高速度V=80(50*80步/秒)，加速度A=100(100*100步/秒^2)，旋转
		std::cout<< "speed="<<80<<std::endl;
		Sleep(2000);
		sm.WriteSpe(1, 0, 100);//舵机(ID1)以加速度A=100(100*100步/秒^2)，停止旋转(V=0)
		std::cout<< "speed="<<0<<std::endl;
		Sleep(2000);
		sm.WriteSpe(1, -80, 100);//舵机(ID1)以最高速度V=-80(-50*80步/秒)，加速度A=100(100*100步/秒^2)，反向旋转
		std::cout<< "speed="<<-80<<std::endl;
		Sleep(2000);
		sm.WriteSpe(1, 0, 100);//舵机(ID1)以加速度A=100(100*100步/秒^2)，停止旋转(V=0)
		std::cout<< "speed="<<0<<std::endl;
		Sleep(2000);
	}
	sm.end();
	return 1;
}

