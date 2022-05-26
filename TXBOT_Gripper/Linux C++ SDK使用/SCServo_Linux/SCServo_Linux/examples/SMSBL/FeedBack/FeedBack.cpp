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
			usleep(10*1000);
		}else{
			std::cout<< "read err ="<<std::endl;
			sleep(2);
		}
	}
	sm.end();
	return 1;
}

