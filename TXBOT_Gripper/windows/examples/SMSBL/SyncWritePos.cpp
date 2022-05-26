/*
以下例子在SMS40BL中测试通过，舵机出厂速度单位为0.732rpm，舵机机运行速度V=80
如果使用的出厂速度单位是0.0146rpm，则速度改为V=4000，延时公式T=[(P1-P0)/V]*1000+[V/(A*100)]*1000
*/

#include "stdafx.h"
#include <iostream>
#include "../../SCServo/SCServo.h"

SMSBL sm;

u8 ID[2] = {1, 2};
s16 Position[2];
u16 Speed[2] = {80, 80};
u8 ACC[2] = {100, 100};

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
	while(1){
		Position[0] = 4095;
		Position[1] = 4095;
		sm.SyncWritePosEx(ID, 2, Position, Speed, ACC);//舵机(ID1/ID2)以最高速度V=80(50*80步/秒)，加速度A=100(100*100步/秒^2)，运行至P1=4095位置
		std::cout<< "pos ="<<4095<<std::endl;
		Sleep(1495);//[(P1-P0)/(50*V)]*1000+[(50*V)/(A*100)]*1000
  
		Position[0] = 0;
		Position[1] = 0;
		sm.SyncWritePosEx(ID, 2, Position, Speed, ACC);//舵机(ID1/ID2)以最高速度V=80(50*80步/秒)，加速度A=100(100*100步/秒^2)，运行至P0=0位置
		std::cout<< "pos ="<<0<<std::endl;
		Sleep(1495);//[(P1-P0)/(50*V)]*1000+[(50*V)/(A*100)]*1000
	}
	sm.end();
	return 1;
}

