#include<reg51.h>
#include<intrins.h>
void delay();

void main(){
	TMOD=0x10;	//T1����ʽ1����
	TR1=1;		//������ʱ��
	P1=0xAA;	//ȫ��
	delay();
	P1=~P1;		//ȫ��
	delay();
	P1=0xFF;	//ȫ��
	delay();
	
	P3=0x9C;	//P3 = 1001 1100 (���κ졢�̡��ơ�����
	P1=P3;
	while(1){
		delay();
		P3=_cror_(P3,2);  //ÿ��ѭ���ƶ�2��
		P1=P3;
	}
	
}

void delay(){
	char i;
	for(i=0;i<20;i++){
		TH1=0x3C;
		TL1=0xB0;
		while(TF1==0);	//ֱ����ѯ��TF==1
		TF1=0;
	 }
	 return;
}
