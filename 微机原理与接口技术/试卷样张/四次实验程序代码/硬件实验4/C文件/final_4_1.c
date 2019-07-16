#include<reg51.h>
#include<intrins.h>
#include<math.h>
#include<stdio.h>
typedef   unsigned char  uint8;
typedef   unsigned int   uint16;
unsigned char Init_DS18B20();
void Write_byte(unsigned char dat);	//DS18B20дһ���ֽں���
void Delay60us();		//@11.9157MHz
void Delay480us();		//@11.9157MHz
void Delay600us();		//@11.9157MHz
void Delay500ms();		//@11.9157MHz

unsigned char Read_byte(void);		//DS18B20��һ���ֽں���
unsigned int Get_Temperature(void);
unsigned char table[10]={'0','1','2','3','4','5','6','7','8','9'};
uint8 buf[]="The temperature is:";
uint8 bufend[]="��C right now!\n";
sbit DQ=P3^7;		  

unsigned char string[10];//����10�����¶�ȫ�ֱ��� 
/*����һ���ַ�*/
void UART_send_byte(uint8 dat){
	SBUF = dat;       //�����ݷŵ�SBUF��
	while (TI == 0);//δ������Ͼ͵ȴ�
	TI = 0;    //������Ϻ�Ҫ��TI������0
}	
/*����һ���ַ���*/	   
void UART_send_string(uint8 *buf){								
	while (*buf != '\0')	{							
		UART_send_byte(*buf++);	
	}							
}
void main(){
	//int t1;
	int t;

	unsigned char temp[8];
	double Temp;

	SCON=0x50;
	//PCON=0x80;
	TMOD=0x20;
	TH1=0xF3;
	TL1=0xF3;
	TR1=1;
//	Init_DS18B20();
/*	while(1){
		UART_send_string(buf);
		t1=Get_Temperature()/0x0f;
		SBUF=t1/10*16+t1%10;
		while(TI==0);
		TI=0;
		UART_send_string(bufend);
		Delay500ms(); 
	}
*/
;
	//UART_init();
	Init_DS18B20();
	while(1){
			UART_send_string(buf);
			t=Get_Temperature();
			// t = 0x5678;
			Temp=(double)(t&0x07ff)*0.0625;//��ֵλ����0.0625
			//Temp = �¶�
			if(t&0xf000){
				temp[0]='-';
			}
			else{
				temp[0]='+'; //����λ
			}
			temp[1]=table[(unsigned char)(Temp/100)%100];//��λ
			if(temp[1]=='0'){
				temp[1]=' ';
			}
			Temp=Temp-(unsigned char)(Temp/100)*100;
			temp[2]=table[(unsigned char)(Temp/10)%10];//ʮλ
			Temp=Temp-(unsigned char)(Temp/10)*10;
			temp[3]=table[(unsigned char)Temp]; //��λ
			Temp=Temp-(unsigned char)Temp;
			temp[4]='.';
			Temp=Temp*10;
			temp[5]=table[(unsigned char)Temp];
			Temp=Temp-(unsigned char)Temp;
			Temp=Temp*10;
			temp[6]=table[(unsigned char)Temp];
			temp[7]='\0';
			UART_send_string(temp);
			UART_send_string(bufend);
			Delay500ms();
	}	
}
unsigned char Init_DS18B20(){
	unsigned char tdq;
	DQ=0;				//������������
	Delay480us();			//�ȴ�480us
	DQ=1;				//�����ͷ�����
	Delay60us();			//�ȴ�60us
	tdq=DQ;				//�����������
	Delay480us();			//�ȴ�Ӧ���������
	return tdq;			//���ز���ֵ
}
unsigned char Read_byte(void){
	unsigned char i=0;
	unsigned dat=0;
	for(i=8;i>0;i--){
		_nop_();
		_nop_();			//2�ζ�����֮�����ټ��1us
		DQ=0;				//�Ӹ������ͣ������ֵ͵�ƽ����1us
		_nop_();
		_nop_();
		DQ=1;				//�ͷ�����
		_nop_();
		_nop_();
		dat>>=1;			//�������ӵ͵��ߣ��ȶ����λ
		if(DQ){
			dat|=0x80;
		}
		Delay60us();			//���ٵȴ�60us����ȷ�������ɹ�
	}
	DQ=1;				//�ͷ�����
	return(dat);

}
void Write_byte(unsigned char dat){
	unsigned char i=0;
	for(i=8;i>0;i--){
		DQ=1;
		_nop_();
		_nop_();		//��֤����д�����������1us
		DQ=0;			//��ʼһ��д����
		_nop_();
		_nop_();
		DQ=dat&0x01;	//д����Ϊ�ȵ�λ���λ
		Delay60us();		//���ٱ���60usдʱ��϶
		dat>>=1;		//����һλ
	}
	DQ=1;				//�ͷ�����
}
unsigned int Get_Temperature(void){	//DS18B20���¶Ⱥ���
	unsigned char a=0;
	unsigned char b=0;
	unsigned char c=0;
	unsigned int t=0;
	c=Init_DS18B20();				//DS18B20��ʼ����������������1����wire��ʼ������Reset()��ͬ
	
	if(c==0)						//���������ҽ���������
	{
		Write_byte(0xCC);			//ֻ��һ��18b20оƬ������ROM
		Write_byte(0x44);			//�¶�ת��
		Delay600us();					//��ʱ600us(�ȴ�������ϣ���һ�β���ʱ��Ϊ500us
		Init_DS18B20();				//���³�ʼ��
		Write_byte(0xcc);
		Write_byte(0xbe);			//��RAM������
		a=Read_byte();				//�ȶ����ֽ�
		b=Read_byte();				//�ٶ����ֽ�
		t=b<<8;
		t=t|a;						//tΪ�����õ�����
	}
	else
		t=0;
	return(t);
}
void Delay480us()		//@11.9157MHz
{
	unsigned char i;

	_nop_();
	i = 235;
	while (--i);
}


void Delay60us()		//@11.9157MHz
{
	unsigned char i;

	i = 27;
	while (--i);
}


void Delay600us()		//@11.9157MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 36;
	do
	{
		while (--j);
	} while (--i);
}

void Delay500ms()		//@11.9157MHz
{
	unsigned char i, j, k;

	i = 4;
	j = 198;
	k = 230;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}