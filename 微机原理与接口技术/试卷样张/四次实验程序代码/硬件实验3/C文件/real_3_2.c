#include<reg51.h>
#include<intrins.h>

void delay_ms(int z);
void main(){		  
	//8������ܵ����һ���ǹ�����
	//P0��������룬P1�����λ��
	//P1 = 0x01; P0 = _cror_
	P1 = 0x01;
	P0 = 0x01;
	while(1){
		delay_ms(100);
		P0 = _cror_(P0,1);
	}
}
void delay_ms(int z){
	int i,j;
	for(i = z;i>0;i--)
		for( j=110;j>0;j--);
}