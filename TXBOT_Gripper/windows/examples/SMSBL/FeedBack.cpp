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
	while(1){
		int Pos;
		int Speed;
		int Load;
		int Voltage;
		int Temper;
		int Move;
		int Current;
		if(sm.FeedBack(1)!=-1){
			Pos = sm.ReadPos(-1);
			Speed = sm.ReadSpeed(-1);
			Load = sm.ReadLoad(-1);
			Voltage = sm.ReadVoltage(-1);
			Temper = sm.ReadTemper(-1);
			Move = sm.ReadMove(-1);
			Current = sm.ReadCurrent(-1);
			std::cout<< "pos ="<<Pos<<std::endl;
			std::cout<< "Speed ="<<Speed<<std::endl;
			std::cout<< "Load ="<<Load<<std::endl;
			std::cout<< "Voltage ="<<Voltage<<std::endl;
			std::cout<< "Temper ="<<Temper<<std::endl;
			std::cout<< "Move ="<<Move<<std::endl;
			std::cout<< "Current ="<<Current<<std::endl;
			Sleep(10);
		}else{
			std::cout<< "read err =-1"<<std::endl;
			Sleep(2000);
		}
		Pos = sm.ReadPos(1);
		if(Pos!=-1){
			std::cout<<"Servo position:"<<Pos<<std::endl;
			Sleep(10);
		}else{
			std::cout<<"read position err"<<std::endl;
			Sleep(500);
		}
	
		Voltage = sm.ReadVoltage(1);
		if(Voltage!=-1){
			std::cout<<"Servo Voltage:"<<std::endl;
			Sleep(10);
		}else{
			std::cout<<"read Voltage err"<<Voltage<<std::endl;
			Sleep(500);
		}
	
		Temper = sm.ReadTemper(1);
		if(Temper!=-1){
			std::cout<<"Servo temperature:"<<Voltage<<std::endl;
			Sleep(10);
		}else{
			std::cout<<"read temperature err"<<std::endl;
			Sleep(500);
		}

		Speed = sm.ReadSpeed(1);
		if(Speed!=-1){
			std::cout<<"Servo Speed:"<<Speed<<std::endl;
			Sleep(10);
		}else{
			std::cout<<"read Speed err"<<std::endl;
			Sleep(500);
		}

		Load = sm.ReadLoad(1);
		if(Load!=-1){
			std::cout<<"Servo Load:"<<Load<<std::endl;
			Sleep(10);
		}else{
			std::cout<<"read Load err"<<std::endl;
			Sleep(500);
		}
		
		Current = sm.ReadCurrent(1);
		if(Current!=-1){
			std::cout<<"Servo Current:"<<Current<<std::endl;
			Sleep(10);
		}else{
			Sleep(500);
		}

		Move = sm.ReadMove(1);
		if(Move!=-1){
			std::cout<<"Servo Move:"<<Move<<std::endl;
			Sleep(10);
		}else{
			std::cout<<"read Move err"<<std::endl;
			Sleep(500);
		}
		std::cout<<std::endl;
		Sleep(2000);
	}
	sm.end();
	return 1;
}
