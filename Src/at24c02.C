#include "main.h"
#include "intrins.h"


#define nopsw _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
#define DIAddr     0xA0
#define I2C_PAGESIZE    16

/*
#define I2C_GPIO   GPIOE
#define SCL_GION_PIN  GPIO_PIN_1
#define SDA_GION_PIN  GPIO_PIN_2
*/
//GPIO_WriteHigh(I2C_GPIO, SDA_GION_PIN)
#define SDA_HIGH  P14=1  
#define SDA_LOW  P14=0 
//GPIO_WriteLow(I2C_GPIO, SDA_GION_PIN)
#define SCL_HIGH  P15=1 
//GPIO_WriteHigh(I2C_GPIO, SCL_GION_PIN)
#define SCL_LOW  P15=0 
//GPIO_WriteLow(I2C_GPIO, SCL_GION_PIN)
#define GET_SDA  P14==1
//GPIO_ReadInputPin(I2C_GPIO, SDA_GION_PIN)

#define EEP_HIGH P16=1
#define EEP_LOW P16=0

uint8_t   ack;

void Init_I2c_GPIO(void)
{
	//GPIO_Init(I2C_GPIO, (GPIO_Pin_TypeDef)(SCL_GION_PIN | SDA_GION_PIN), GPIO_MODE_OUT_OD_HIZ_FAST);
	SDA_HIGH;
	SCL_HIGH;
	//EEPROM WP
	//GPIO_Init(EEP_WP_GPIO,(GPIO_Pin_TypeDef)(WP_GION_PIN), GPIO_MODE_OUT_PP_HIGH_FAST);
	//GPIO_WriteHigh(EEP_WP_GPIO,(GPIO_Pin_TypeDef)(WP_GION_PIN));
	EEP_HIGH;
}




/*******************************************************************
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:       ����I2C����,������I2C��ʼ����.
  
********************************************************************/
void Start_I2c(void)
{
  SDA_HIGH;     /*������ʼ�����������ź�*/
  delay_us(10);  /*���ͽ���������ʱ���ź�*/
  SCL_HIGH;
  delay_us(10); /*��ʼ��������ʱ�����4.7us,��ʱ*/
  SDA_LOW;     /*������ʼ�ź�*/
  delay_us(10);  /* ��ʼ��������ʱ�����4��s*/     
  SCL_LOW;     /*ǯסI2C���ߣ�׼�����ͻ�������� */
  nopsw;
  nopsw;
}

/*******************************************************************
                      �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:       ����I2C����,������I2C��������.
  
********************************************************************/
void Stop_I2c(void)
{
  SDA_LOW;    /*���ͽ��������������ź�*/
  nopsw;   /*���ͽ���������ʱ���ź�*/
  nopsw;   /*���ͽ���������ʱ���ź�*/
  SCL_HIGH;    /*������������ʱ�����4��s*/
delay_us(10); 
  SDA_HIGH;    /*����I2C���߽����ź�*/
  nopsw;
  nopsw;
  nopsw;
  nopsw;
}

/*******************************************************************
                 �ֽ����ݴ��ͺ���               
����ԭ��: void  SendByte(uchar c);
����:  ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
     ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0 ��)     
     ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
void  SendByte(uint8_t c)
{
 uint8_t BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  /*Ҫ���͵����ݳ���Ϊ8λ*/
    {
     if(((c<<BitCnt)&0x80)!=0)SDA_HIGH;   /*�жϷ���λ*/
       else  SDA_LOW;                
     nopsw;
     SCL_HIGH;                 /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
	 delay_us(10);   /*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/     
     SCL_LOW; 
    }
    
    nopsw;
    nopsw;
   SDA_HIGH;                 /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
    nopsw;
    nopsw;   
    SCL_HIGH;
	//GPIO_Init(I2C_GPIO, (GPIO_Pin_TypeDef)(SDA_GION_PIN), GPIO_MODE_IN_FL_NO_IT);
    nopsw;
    nopsw;
    nopsw;
	
	
    if(GET_SDA)ack=0;     
       else ack=1;        /*�ж��Ƿ���յ�Ӧ���ź�*/
    SCL_LOW;
    nopsw;
    nopsw;
	//GPIO_Init(I2C_GPIO, (GPIO_Pin_TypeDef)(SDA_GION_PIN), GPIO_MODE_OUT_OD_HIZ_FAST);
}

/*******************************************************************
                 �ֽ����ݴ��ͺ���               
����ԭ��: uchar  RcvByte();
����:  �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
     ���������Ӧ������  
********************************************************************/	
uint8_t  RcvByte(void)
{
  uint8_t retc;
  uint8_t BitCnt;
  
  retc=0; 
  SDA_HIGH;              /*��������Ϊ���뷽ʽ*/
  //GPIO_Init(I2C_GPIO, (GPIO_Pin_TypeDef)(SDA_GION_PIN), GPIO_MODE_IN_FL_NO_IT);
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        nopsw;         
        SCL_LOW;        /*��ʱ����Ϊ�ͣ�׼����������λ*/
	   delay_us(10);    /*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
        SCL_HIGH;        /*��ʱ����Ϊ��ʹ��������������Ч*/
        nopsw;
        nopsw;
        retc=retc<<1;
        if(GET_SDA)retc=retc+1; /*������λ,���յ�����λ����retc�� */
        nopsw;
        nopsw;
      }
  SCL_LOW;    
  nopsw;
  nopsw;
  //GPIO_Init(I2C_GPIO, (GPIO_Pin_TypeDef)(SDA_GION_PIN), GPIO_MODE_OUT_OD_HIZ_FAST);
  return(retc);
}

/********************************************************************
                     Ӧ���Ӻ���
ԭ��:  void Ack_I2c(bit a);
 
����:����������Ӧ���ź�,(������Ӧ����Ӧ���ź�)
********************************************************************/
void Ack_I2c(uint8_t a)
{
  
  if(a==0)SDA_LOW;         /*�ڴ˷���Ӧ����Ӧ���ź� */
        else SDA_HIGH;
	nopsw;
	nopsw;
	nopsw;      
	SCL_HIGH;
	delay_us(10);     /*ʱ�ӵ͵�ƽ���ڴ���4��s*/
	SCL_LOW;                  /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
	nopsw;
	nopsw; 
}

/*******************************************************************
                    �����ӵ�ַ���������ֽ����ݺ���               
����ԭ��: bit  ISendByte(uchar sla,ucahr c);  
����:     ���������ߵ����͵�ַ�����ݣ��������ߵ�ȫ����,��������ַsla.
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
/*
uint8_t ISendByte(uint8_t sla,uint8_t c)
{
   Start_I2c();               //��������
   SendByte(sla);             //����������ַ
     if(ack==0)return(0);
   SendByte(c);               //��������
     if(ack==0)return(0);
  Stop_I2c();                 //�������� 
  return(1);
}
*/


uint8_t IWritePage( uint8_t WriteAddr, uint8_t* pBuffer,uint8_t NumByteToWrite)
{
	
   Start_I2c();               //��������
   SendByte(DIAddr);             //����������ַ
     if(ack==0)return(0);
   SendByte(WriteAddr);               //��������
     if(ack==0)return(0);

	 while (NumByteToWrite>0)
    {   
     SendByte(*pBuffer);            /*��������*/
       if(ack==0)
       	{
	   	return(0);
       	}
     pBuffer++;
	 NumByteToWrite--;
    }  
  Stop_I2c();                 //�������� 
  delay_ms(50);
  return(1);
}



/*******************************************************************
                    �����ӵ�ַ�������Ͷ��ֽ����ݺ���               
����ԭ��: bit  ISendStr(uchar sla,uchar suba,ucahr *s,uchar no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
uint8_t ISendStr(uint8_t WriteAddr,uint8_t *s,uint8_t NumByteToWrite)
{
  uint8_t NumOfPage = 0;
  uint8_t NumOfSingle = 0;
  uint8_t count = 0;
  uint8_t Adr;
  EEP_LOW;
  Adr=WriteAddr % I2C_PAGESIZE;
  if (Adr!=0)
  {
  count = (uint8_t)(I2C_PAGESIZE -Adr);

	if (IWritePage(WriteAddr,s,count)==0) 
		{
		EEP_HIGH;
		return 0;
		}
	NumByteToWrite-=count;
	WriteAddr+=count;
	s+=count;
  }

  NumOfPage =  (uint8_t)(NumByteToWrite / I2C_PAGESIZE);
  NumOfSingle = (uint8_t)(NumByteToWrite % I2C_PAGESIZE);

	while(NumOfPage>0)
	{
		if (IWritePage(WriteAddr,s,I2C_PAGESIZE)==0) 
			{
			//GPIO_WriteHigh(EEP_WP_GPIO,(GPIO_Pin_TypeDef)(WP_GION_PIN));
			EEP_HIGH;
			return 0;
			}
		NumByteToWrite-=I2C_PAGESIZE;
		WriteAddr+=I2C_PAGESIZE;
		s+=I2C_PAGESIZE;
		NumOfPage--;
	}

	if (NumOfSingle>0) 
	{
	if (IWritePage(WriteAddr,s,NumOfSingle)==0) 
		{
		//GPIO_WriteHigh(EEP_WP_GPIO,(GPIO_Pin_TypeDef)(WP_GION_PIN));
		EEP_HIGH;
		return 0;
		}
	}
	
 //GPIO_WriteHigh(EEP_WP_GPIO,(GPIO_Pin_TypeDef)(WP_GION_PIN));	
 EEP_HIGH;
 return(1);
}

/*******************************************************************
                    �����ӵ�ַ�������ֽ����ݺ���               
����ԭ��: bit  IRcvByte(uchar sla,ucahr *c);  
����:     ���������ߵ����͵�ַ�������ݣ��������ߵ�ȫ����,��������
          ַsla������ֵ��c.
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************
uint8_t IRcvByte(uint8_t sla,uint8_t *c)
{
   Start_I2c();                //��������
   SendByte(sla+1);            //����������ַ
     if(ack==0)return(0);
   *c=RcvByte();               //��ȡ����
     Ack_I2c(1);               //���ͷǾʹ�λ
  Stop_I2c();                  //��������/ 
  return(1);
}

/*******************************************************************
                    �����ӵ�ַ������ȡ���ֽ����ݺ���               
����ԭ��: bit  ISendStr(uchar sla,uchar suba,ucahr *s,uchar no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ��������ߵ�ȫ����,������
          ��ַsla  (0xa0)���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
uint8_t IRcvStr(uint8_t ReadAddr,uint8_t *s,uint8_t NumByteToRead)
{
   Start_I2c();               /*��������*/
   SendByte(DIAddr);             /*����������ַ*/
     if(ack==0)	
	 	{
	 	return(0);
     		}
   SendByte(ReadAddr);            /*���������ӵ�ַ*/
     if(ack==0)
	 	{
	 	return(0);
     	}

   Start_I2c();
   SendByte(DIAddr+1);
      if(ack==0)
	  	{
	  	return(0);
      	}

	while (NumByteToRead>1)
    {   
     *s=RcvByte();               /*��������*/
      Ack_I2c(0);                /*���;ʹ�λ*/  
     s++;
	 NumByteToRead--;
    } 
   *s=RcvByte();
    Ack_I2c(1);                  /*���ͷ�Ӧλ*/
 Stop_I2c();                     /*��������*/ 
  return(1);
}
/***************������γ���ȡ��zlgmcu.com********************/



