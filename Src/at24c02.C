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
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:       启动I2C总线,即发送I2C起始条件.
  
********************************************************************/
void Start_I2c(void)
{
  SDA_HIGH;     /*发送起始条件的数据信号*/
  delay_us(10);  /*发送结束条件的时钟信号*/
  SCL_HIGH;
  delay_us(10); /*起始条件建立时间大于4.7us,延时*/
  SDA_LOW;     /*发送起始信号*/
  delay_us(10);  /* 起始条件锁定时间大于4μs*/     
  SCL_LOW;     /*钳住I2C总线，准备发送或接收数据 */
  nopsw;
  nopsw;
}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:       结束I2C总线,即发送I2C结束条件.
  
********************************************************************/
void Stop_I2c(void)
{
  SDA_LOW;    /*发送结束条件的数据信号*/
  nopsw;   /*发送结束条件的时钟信号*/
  nopsw;   /*发送结束条件的时钟信号*/
  SCL_HIGH;    /*结束条件建立时间大于4μs*/
delay_us(10); 
  SDA_HIGH;    /*发送I2C总线结束信号*/
  nopsw;
  nopsw;
  nopsw;
  nopsw;
}

/*******************************************************************
                 字节数据传送函数               
函数原型: void  SendByte(uchar c);
功能:  将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
     此状态位进行操作.(不应答或非应答都使ack=0 假)     
     发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void  SendByte(uint8_t c)
{
 uint8_t BitCnt;
 
 for(BitCnt=0;BitCnt<8;BitCnt++)  /*要传送的数据长度为8位*/
    {
     if(((c<<BitCnt)&0x80)!=0)SDA_HIGH;   /*判断发送位*/
       else  SDA_LOW;                
     nopsw;
     SCL_HIGH;                 /*置时钟线为高，通知被控器开始接收数据位*/
	 delay_us(10);   /*保证时钟高电平周期大于4μs*/     
     SCL_LOW; 
    }
    
    nopsw;
    nopsw;
   SDA_HIGH;                 /*8位发送完后释放数据线，准备接收应答位*/
    nopsw;
    nopsw;   
    SCL_HIGH;
	//GPIO_Init(I2C_GPIO, (GPIO_Pin_TypeDef)(SDA_GION_PIN), GPIO_MODE_IN_FL_NO_IT);
    nopsw;
    nopsw;
    nopsw;
	
	
    if(GET_SDA)ack=0;     
       else ack=1;        /*判断是否接收到应答信号*/
    SCL_LOW;
    nopsw;
    nopsw;
	//GPIO_Init(I2C_GPIO, (GPIO_Pin_TypeDef)(SDA_GION_PIN), GPIO_MODE_OUT_OD_HIZ_FAST);
}

/*******************************************************************
                 字节数据传送函数               
函数原型: uchar  RcvByte();
功能:  用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
     发完后请用应答函数。  
********************************************************************/	
uint8_t  RcvByte(void)
{
  uint8_t retc;
  uint8_t BitCnt;
  
  retc=0; 
  SDA_HIGH;              /*置数据线为输入方式*/
  //GPIO_Init(I2C_GPIO, (GPIO_Pin_TypeDef)(SDA_GION_PIN), GPIO_MODE_IN_FL_NO_IT);
  for(BitCnt=0;BitCnt<8;BitCnt++)
      {
        nopsw;         
        SCL_LOW;        /*置时钟线为低，准备接收数据位*/
	   delay_us(10);    /*时钟低电平周期大于4.7μs*/
        SCL_HIGH;        /*置时钟线为高使数据线上数据有效*/
        nopsw;
        nopsw;
        retc=retc<<1;
        if(GET_SDA)retc=retc+1; /*读数据位,接收的数据位放入retc中 */
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
                     应答子函数
原型:  void Ack_I2c(bit a);
 
功能:主控器进行应答信号,(可以是应答或非应答信号)
********************************************************************/
void Ack_I2c(uint8_t a)
{
  
  if(a==0)SDA_LOW;         /*在此发出应答或非应答信号 */
        else SDA_HIGH;
	nopsw;
	nopsw;
	nopsw;      
	SCL_HIGH;
	delay_us(10);     /*时钟低电平周期大于4μs*/
	SCL_LOW;                  /*清时钟线，钳住I2C总线以便继续接收*/
	nopsw;
	nopsw; 
}

/*******************************************************************
                    向无子地址器件发送字节数据函数               
函数原型: bit  ISendByte(uchar sla,ucahr c);  
功能:     从启动总线到发送地址，数据，结束总线的全过程,从器件地址sla.
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
/*
uint8_t ISendByte(uint8_t sla,uint8_t c)
{
   Start_I2c();               //启动总线
   SendByte(sla);             //发送器件地址
     if(ack==0)return(0);
   SendByte(c);               //发送数据
     if(ack==0)return(0);
  Stop_I2c();                 //结束总线 
  return(1);
}
*/


uint8_t IWritePage( uint8_t WriteAddr, uint8_t* pBuffer,uint8_t NumByteToWrite)
{
	
   Start_I2c();               //启动总线
   SendByte(DIAddr);             //发送器件地址
     if(ack==0)return(0);
   SendByte(WriteAddr);               //发送数据
     if(ack==0)return(0);

	 while (NumByteToWrite>0)
    {   
     SendByte(*pBuffer);            /*发送数据*/
       if(ack==0)
       	{
	   	return(0);
       	}
     pBuffer++;
	 NumByteToWrite--;
    }  
  Stop_I2c();                 //结束总线 
  delay_ms(50);
  return(1);
}



/*******************************************************************
                    向有子地址器件发送多字节数据函数               
函数原型: bit  ISendStr(uchar sla,uchar suba,ucahr *s,uchar no);  
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
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
                    向无子地址器件读字节数据函数               
函数原型: bit  IRcvByte(uchar sla,ucahr *c);  
功能:     从启动总线到发送地址，读数据，结束总线的全过程,从器件地
          址sla，返回值在c.
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************
uint8_t IRcvByte(uint8_t sla,uint8_t *c)
{
   Start_I2c();                //启动总线
   SendByte(sla+1);            //发送器件地址
     if(ack==0)return(0);
   *c=RcvByte();               //读取数据
     Ack_I2c(1);               //发送非就答位
  Stop_I2c();                  //结束总线/ 
  return(1);
}

/*******************************************************************
                    向有子地址器件读取多字节数据函数               
函数原型: bit  ISendStr(uchar sla,uchar suba,ucahr *s,uchar no);  
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
          地址sla  (0xa0)，子地址suba，读出的内容放入s指向的存储区，读no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
uint8_t IRcvStr(uint8_t ReadAddr,uint8_t *s,uint8_t NumByteToRead)
{
   Start_I2c();               /*启动总线*/
   SendByte(DIAddr);             /*发送器件地址*/
     if(ack==0)	
	 	{
	 	return(0);
     		}
   SendByte(ReadAddr);            /*发送器件子地址*/
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
     *s=RcvByte();               /*发送数据*/
      Ack_I2c(0);                /*发送就答位*/  
     s++;
	 NumByteToRead--;
    } 
   *s=RcvByte();
    Ack_I2c(1);                  /*发送非应位*/
 Stop_I2c();                     /*结束总线*/ 
  return(1);
}
/***************上面这段程序取自zlgmcu.com********************/




