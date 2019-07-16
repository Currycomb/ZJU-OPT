#include<reg51.h>	 
typedef unsigned int u16;
typedef unsigned char u8; 
u8 Buf[]="  0%\n"; 
u8 d;
u8 code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39 ,0x5e,0x79,0x71};
//----------------------------------����λ����-------------------------------------------//
sbit sda=P2^0;
sbit scl=P2^1;
//-----------------------------------IICʱ��-------------------------------------------//
void delay(u16 n){
	while (n--);
}
//--------------------------------us��ʱ
void init_iic(){
	scl=0;
	delay(1);
	sda=1;
	delay(1);
	scl=1;
	delay(1);
}
//-------------------------IIC��ʼ���ͷ�����
void start() {
	scl=0;
	delay(1);
	sda=1;
	delay(1);
	scl=1;
	delay(1);
	sda=0;//������ʼ�ź�
//--------------------------------------------
	delay(1);
	scl=0;
	delay(1);
	sda=1;//�ͷ��������ߣ������ź�
	delay(1);
}
//--------------------------IIC��ʼ�źţ�SDA��SCL=1ʱ�����½��أ�
void write_byte(u8 date)
{
	u8 i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp=temp<<1;
		scl=0;//�����������߽��������ݣ����ݿɱ�
		delay(1);
		sda=CY;//�����ɸ�λ���λ���δ���<<ѭ�����ƽ�����������������
		delay(1);//�ȶ��ź�
		scl=1;//��������
		delay(1);
	}
	scl=0;
	delay(1);
	sda=1;
	delay(1);//׼������Ӧ���ź�
	scl=1;
}
//--------------------------IIC�ֽ�д������ʱ����  ------i---temp
void response()
{
	u8 t;//��ʱϵͳĬ��Ӧ��
	while((sda==1)&&(t<250))t++;
	scl=0;
	delay(1);
}
//-----------------------Ӧ���ź�--------------��ʱ����----t(��ʱ��)
void stop()
{
	scl=0;
	delay(1);
	sda=0;
	delay(1);
	scl=1;
	delay(1);
	sda=1;
	delay(1);
}
//----------------------�����źţ�SCL=1ʱ��SDA������Ч��������
u8 read_byte()
{
	u8 k,n;
	k=0x00;
	for(n=0;n<8;n++)
	{
		scl=0;
		delay(1);
		scl=1;
		delay(1);
		k=(k<<1)|sda;
	}//�������ݺ󽫻������Ӧ���Խ������ݶ�ȡ
	delay(1);
	return k;
}
//---------------------�ֽڶ������з���ֵ����ʱ����-----k(�洢����)------n(ѭ����λ������������)
//----------------------------��ʾ����-----------------------------------------------------------------------//
void display()
{
	u8 bai,shi,ge;
	long y=d;
	long x=100-y*100/255;
	bai=(x/100)%10;
	shi=(x%100)/10;
	ge=x%10;
	P1=0xfe;
	P0=table[bai];
	delay(110);
	P1=0xFD;
	P0=table[shi];
	delay(110);
	P1=0xFB;
	P0=table[ge];
	delay(110);
	P1=0xFF;
	if (!bai) {
		Buf[0]=' ';
		if (!shi)
			Buf[1]=' ';
		else
			Buf[1]=shi+'0';	
	}
	else {
		Buf[0]=bai+'0';
		Buf[1]=shi+'0';
	}
	Buf[2]=ge+'0';
}
/*������Ϊ2400*/
void UART_init(void)
{
    SCON = 0x50;        //���ڷ�ʽ1
 
    TMOD = 0x20;        // ��ʱ��ʹ�÷�ʽ2�Զ�����
    TL1 = 0xF3;  //�趨��ʱ��ֵ
	 TH1 = 0xF3;  //�趨��ʱ��ֵ
 
    TR1 = 1;//������ʱ������ʼ����������
}
 
/*����һ���ַ�*/
void UART_send_byte(u8 dat)
{
	SBUF = dat;       //�����ݷŵ�SBUF��
	while (TI == 0);//δ������Ͼ͵ȴ�
	TI = 0;    //������Ϻ�Ҫ��TI������0
}
 
/*����һ���ַ���*/
void UART_send_string(u8 *buf)
{
	while (*buf != '\0')
	{
		UART_send_byte(*buf++);
	}
}
//--------------------------------------------------------------------------------------------------//
void main()
{
	char i=0,j=0;
	init_iic();
	UART_init();
	while(1)
	{
		start();
		write_byte(0x90);//������ַ10010001
		response();
		write_byte(0x00);//�����ֽ�
		response();
		delay(1100);//�ȴ��ɼ����ݣ��������ݼĴ���
		stop();
		delay(550);
		start();
		write_byte(0x91);
		response();
		if (i==5) {
			d=read_byte();
			i=0;
		}
		delay(550);
		stop();
		display();
		i++;
		if (j>7) {
			UART_send_string(Buf);
			j=0;
		}
		else
			j++;
	}
}