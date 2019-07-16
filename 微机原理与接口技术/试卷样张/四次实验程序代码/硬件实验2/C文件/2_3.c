#include<reg51.h>
#include<intrins.h>
sbit P30 = P3^0;
sbit UP = P2^0;
sbit shift = P2^1;
unsigned int count;
unsigned char table[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned int pwm=50;
bit flag = 1;
unsigned int i; 
void delay10ms();
void delayms(int z);
void main(){
	TMOD = 0x12; //T1����ʽ1��16��ʱ��T0����ʽ2��8λ��װ�ء�
	TH0 = 0x9b;
	TL0 = 0x9b;	  

	EA = 1;
	ET0 = 1;
	ET1 = 1;
	pwm = 1;						 //
	P30 = 0;
	TR0 = 1;
	count = 0;
	while(1){
		if(UP == 0 ){
			delayms(100);
			if(UP == 0){
				pwm++;
				if(pwm == 99){
					pwm = 0;
				}

			}
		}
	}														 		
}
void delayms(int z){
	int i,j;
	for( i = z ;i>0;i--){
		for(j=110;j>0;j--);
	}
}
void delay10ms() { 
	TH1 = 0xD8;
	TL1 = 0xF0;
	TR1 = 1;
	while(TF1==0);
	TR1 = 0;							   			   
}

void timer1() interrupt 1 using 1{	//200��s
	//TH0+TH1=20ms;
	//TH0 + TH0 = 512 - 200;
	//��ÿ�εĶ�ʱ��0�ķ�ʽ2�ж��У��ı�TH0����װ��ֵ��TL0�ĳ�ֵ�����ı�ռ�ձ�
	if(count == 100){	  //ÿ2*pwm ms �� 200 - 2*pwm ms ʱ�仯һ�Ρ�
		P30 = ~P30;
		if(flag){	
		TL0 = 0xff + pwm*2 - 200;	//��һ���ж�ʱ���Ѿ���ȥ 20*(1 - pwm/100) ms��
		TH0 = 0xff + pwm*2 - 200;
		}else{
			TL0 = 0xff - 2*pwm;		//��һ���ж�ʱ���Ѿ���ȥ 20*pwm/100 ms ��
			TH0 = 0xff - 2*pwm;
		}
		count = 0;
	}
	if( count%100 == 0 ){
		if(!flag){
			P0 = 0xbf;	 //ռ�ձ�ʮλ
			P1 = table[pwm/10];
		}else{
			P0 = 0x7f;	 //ռ�ձȸ�λ
			P1 = table[pwm%10];
		}
		flag = ~flag;

	}
	count++;	
}



