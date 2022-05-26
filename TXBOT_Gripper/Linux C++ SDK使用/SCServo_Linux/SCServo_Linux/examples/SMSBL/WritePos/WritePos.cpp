/*
以下例子在SMS40BL中测试通过，舵机出厂速度单位为0.732rpm，舵机机运行速度V=80
如果使用的出厂速度单位是0.0146rpm，则速度改为V=4000，延时公式T=[(P1-P0)/V]*1000+[V/(A*100)]*1000
*/

#include <iostream>
#include "SCServo.h"

SMSBL sm;

int main(int argc, char **argv)
{
	if(argc<2){
        std::cout<< "argc error!"<<std::endl;
        return 0;
	}
	std::cout<< "serial:"<<argv[1]<<std::endl;
    if(!sm.begin(115200, argv[1])){
        std::cout<< "Failed to init smsbl motor!"<<std::endl;
        return 0;
    }
	while(1){
		sm.WritePosEx(1, 0, 80, 100);//舵机(ID1)以最高速度V=80(50*80步/秒)，加速度A=100(100*100步/秒^2)，运行至P1=0位置，机械夹开口最大位置
		std::cout<< "pos ="<<4095<<std::endl;
		usleep(800*1000);//[(P1-P0)/(50*V)]*1000+[(50*V)/(A*100)]*1000
  
		sm.WritePosEx(1, 800, 80, 100);//舵机(ID1)以最高速度V=80(50*80步/秒)，加速度A=100(100*100步/秒^2)，运行至P0=800位置，机械夹完全闭合位置
		std::cout<< "pos ="<<0<<std::endl;
		usleep(800*1000);//[(P1-P0)/(50*V)]*1000+[(50*V)/(A*100)]*1000
	}
	sm.end();
	return 1;
}

