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

	sm.CalibrationOfs(1);
	std::cout<< "Calibration Ofs"<<std::endl;
	sm.end();
	getchar();
	return 1;
}

