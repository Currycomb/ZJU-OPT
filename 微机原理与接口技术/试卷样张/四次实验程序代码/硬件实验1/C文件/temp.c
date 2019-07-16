#include<reg51.h>
#include<intrins.h>

//������ 
#define bright 0
#define out 1
//ʱ�� 
#define greentime 4	//�̵�ʱ��
#define flashtime 3		//��˸ʱ��
#define yellowtime 2	//�Ƶ�ʱ��

sbit P11=P1^1;		//�ϱ���
sbit P12=P1^2;		//�ϱ���
sbit P13=P1^3;		//�ϱ���
sbit P14=P1^4;		//������
sbit P15=P1^5;		//������
sbit P16=P1^6;		//������
char times,i;		//

void delay();

void main(){
	P1=0xFF;	//��ʼ��
	TMOD=0x10;	//T1����ʽ1����
	TR1=1;		//������ʱ��
	//���������
	P11=bright;
	P14=bright;
	delay();
	delay();
	while(1){
		P14=out;	//���������
		P16=bright;	//�����̵���

		for(times=0;times<greentime;times++)
			delay();
		//�̵���
		for(times=0;times<flashtime;times++){
			P16=~P16;
			delay();
		}
		//�̵���
		P16=out;
		//�Ƶ���
		P15=bright;
		for(times=0;times<yellowtime;times++)
			delay();
		//�����Ƶ��𣬺�������ϱ��̵���
		P15=out;	P14=bright;
		P11=out;	//�ϱ������ 
		P13=bright;
		//�ϱ��̵���greentime
		for(times=0;times<greentime;times++)
			delay();
		//�ϱ��̵���˸
		for(times=0;times<flashtime;times++){
			P13=~P13;
			delay();
		}
		//�ϱ��̵���
		P13=out;
		//�ϱ��Ƶ���yellowtime
		P12=bright;
		for(times=0;times<yellowtime;times++)
			delay();
		//�ϱ��Ƶ���,����� 
		P12=out; 
		P11=bright;
	}
}

void delay(){
	for(i=0;i<10;i++){
		TH1=0x3C;
		TL1=0xB0;
		while(TF1==0);	
		TF1=0;
	}
	return;
}

