
#include<reg51.h>
#include<intrins.h>
sbit p10 = P1^0;
unsigned int i;
unsigned int count;
//16λ��ʱ��ʽʵ��1s��ʱ��
void main(){
	TMOD = 0X10;
	//THTL = 0x3C6B = 15536;
	// 1s = 1000 000��s = 50000 ��s * 20 ��
	TH1 = 0x3C;
	TL1 = 0x6B;
	P1 = 0x01;
	EA = 1;
	ET1 = 1;
	TR1 = 1;
	count = 0;
	while(1);		
}
void timer1() interrupt 3 using 1{
	TH1 = 0x3C;
	TL1 = 0x6B;
	count++;
	if(count==20){
		count = 0;
		P1 = _cror_(P1,1);
	} 	
}



