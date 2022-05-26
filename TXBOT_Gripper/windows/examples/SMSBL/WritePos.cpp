/*
����������SMS40BL�в���ͨ������������ٶȵ�λΪ0.732rpm������������ٶ�V=80
���ʹ�õĳ����ٶȵ�λ��0.0146rpm�����ٶȸ�ΪV=4000����ʱ��ʽT=[(P1-P0)/V]*1000+[V/(A*100)]*1000
*/

#include "stdafx.h"
#include <iostream>
#include "../../SCServo/SCServo.h"

SMSBL sm;
int main(int argc, char* argv[])
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
		sm.WritePosEx(1, 4095, 80, 100);//���(ID1)������ٶ�V=80(50*80��/��)�����ٶ�A=100(100*100��/��^2)��������P1=4095λ��
		std::cout<< "pos ="<<4095<<std::endl;
		Sleep(1495);//[(P1-P0)/(50*V)]*1000+[(50*V)/(A*100)]*1000
  
		sm.WritePosEx(1, 0, 80, 100);//���(ID1)������ٶ�V=80(50*80��/��)�����ٶ�A=100(100*100��/��^2)��������P0=0λ��
		std::cout<< "pos ="<<0<<std::endl;
		Sleep(1495);//[(P1-P0)/(50*V)]*1000+[(50*V)/(A*100)]*1000
	}
	sm.end();
	return 1;
}
