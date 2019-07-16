#include<reg52.h>
#include <intrins.h>

#define AddWr 0x90   //д���ݵ�ַ 
#define AddRd 0x91   //�����ݵ�ַ

typedef unsigned int u16;
typedef unsigned char u8;

 
sbit Sda=P2^0;      //�����������Ӷ˿�
sbit Scl=P2^1;

u8 num=0,N=0;
//num���ֵΪ256

void Start(void)
{
	Sda=1;
	_nop_();
	Scl=1;
	_nop_();
	Sda=0;
	_nop_();
	Scl=0;
}

void Stop(void)
{
	Sda=0;
	_nop_();
	Scl=1;
	_nop_();
	Sda=1;
	_nop_();
	Scl=0;
}

void Ack(void)
{
	Sda=0;
	_nop_();
	Scl=1;
	_nop_();
	Scl=0;
	_nop_();
}

void Send(u8 Data)
{ 
	u8 BitCounter=8;
	u8 temp;

	do {
		temp=Data;
		Scl=0;
		_nop_();
     	if((temp&0x80)==0x80)
      	Sda=1;
     	else
      	Sda=0;
		Scl=1;
		temp=Data<<1;
		Data=temp;
		BitCounter--;
   } while(BitCounter);
   Scl=0;
}

void DAC(unsigned char Data)
{
	Start();
	Send(AddWr); //д��оƬ��ַ
	Ack();
	Send(0x40);  //д�����λ��ʹ��DAC���
	Ack();
	Send(Data);  //д����
	Ack();
	Stop();
}

void UsartInit() {
	TMOD=0x21;
	TH1=0xf3;
	TL1=0xf3;
	PCON=0x80;
	TR1=1;
	SCON=0x50;
	ES=1;
	EA=1;
}

void main() {
	UsartInit();
	while(1)
		DAC(num);       //DA�����������LEDģ���ѹ�仯
}

void Usart() interrupt 4 {
	u8 receiveData;
	N++;
	receiveData=SBUF;	   //��0��
	RI=0;
	if (N==4) {
		N=1;
		num=receiveData-'0';
	}
	else
		num=num*10+receiveData-'0';	
	//SBUF=receiveData;
	SBUF = '1';
	while(!TI);
	TI=0;
}