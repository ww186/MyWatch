#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "stm32f10x_gpio.h"
#include "common.h"
#include "Page.h"

static u16 KeyVaule1, KeyVaule2, KeyVaule3, KeyVaule4;
static BOOL_E IsKeyVauleVailed;

#define KEY_MASK 0x0fff     //按键掩码
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2    //按下为0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0              //按下为1

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!'

static u8 GetKey1Input(void)
{
  return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);

}

static u8 GetKey2Input(void)
{
  return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);

}

static u8 GetKey3Input(void)
{
  return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);

}

static u8 GetKey4Input(void)
{
  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);

}

static void Key1_Handl(void)
{
  Page_Key1Handl();
}

static void Key2_Handl(void)
{

 Page_Key2Handl();
}

static void Key3_Handl(void)
{
 Page_Key3Handl();
  
}

static void Key4_Handl(void)
{
 Page_Key4Handl();
  
}


static void Key_Handl(void)
{
  if(IsKeyVauleVailed)
  {
    if((KeyVaule1 & KEY_MASK) == 0x0f00) Key1_Handl();

    if((KeyVaule2 & KEY_MASK) == 0x0f00) Key2_Handl();

    if((KeyVaule3 & KEY_MASK) == 0x0f00) Key3_Handl();

    if((KeyVaule4 & KEY_MASK) == 0x00ff) Key4_Handl();

  }

}

static void Key_GetInput(void)
{
   static u8 i = 0;
    if(i > 8) IsKeyVauleVailed = TRUE; 
    else i++;

    KeyVaule1 <<= 1;
    if(GetKey1Input()) KeyVaule1 |= 0x01;
    else KeyVaule1 &= ~0x01;

    KeyVaule2 <<= 1;
    if(GetKey2Input()) KeyVaule2 |= 0x01;
    else KeyVaule2 &= ~0x01;

    KeyVaule3 <<= 1;
    if(GetKey3Input()) KeyVaule3 |= 0x01;
    else KeyVaule3 &= ~0x01;

    KeyVaule4 <<= 1;
    if(GetKey4Input()) KeyVaule4 |= 0x01;
    else KeyVaule4 &= ~0x01;

}

void KEY_ScanProcess(void)
{	
  Key_GetInput();
  Key_Handl();

}


