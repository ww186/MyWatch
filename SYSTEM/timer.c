#include "timer.h"
#include "usart.h"
#include "common.h"	
#include <stdlib.h>

typedef struct{
	void (*Task)(u32 data);
	u16 Period;
	u16 Cnt;
	u32 Data;
}TimerTaskData_S;

typedef struct TimerTaskNode{
	TimerTaskData_S* TimerTaskData; 
	struct TimerTaskNode* NextNode;
}TimerTaskNode_S;

LinkList* TimerTaskLinkList;

static void InitTimerTaskLinkList(void)
{
	TimerTaskLinkList = CreateLinkList();
}

static void TIM6_Int_Init(u16 ms)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = ms*10 - 1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //10k计数频率
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM6, ENABLE);  //使能TIMx					 
}

void TimerTaskInit(void)
{
	InitTimerTaskLinkList();
	TIM6_Int_Init(1);
}

static TimerTaskNode_S* CreateTimerTaskNode(TimerTaskData_S* Data)
{
	TimerTaskNode_S* Node_t = (TimerTaskNode_S*)CreateLinkListNode(Data, sizeof(TimerTaskData_S));

	return Node_t;
}

BOOL_E TimerTaskReg(TimerTask iTask, u32 iPeriod, u32 iData)
{
	TimerTaskNode_S* Node_t;
	TimerTaskData_S TimerTask_t;
	TimerTask_t.Cnt = iPeriod;
	TimerTask_t.Period = iPeriod;
	TimerTask_t.Task = iTask;
	TimerTask_t.Data = iData;

    Node_t = CreateTimerTaskNode(&TimerTask_t);
	return LinkList_AddNodeToTail(TimerTaskLinkList, (LinkListNode_S*)Node_t);
}

static void TimerTaskProcess(void)
{
	TimerTaskNode_S* TimerTaskNode_t;
	TimerTaskNode_t = (TimerTaskNode_S*)TimerTaskLinkList->HeadNode;

	while(TimerTaskNode_t != NULL)
	{
		if((TimerTaskNode_t->TimerTaskData->Cnt--) == 0)
		{
			TimerTaskNode_t->TimerTaskData->Cnt = TimerTaskNode_t->TimerTaskData->Period;
			TimerTaskNode_t->TimerTaskData->Task(TimerTaskNode_t->TimerTaskData->Data);
		}

		TimerTaskNode_t = TimerTaskNode_t->NextNode;
	}

}

//定时器3中断服务程序
void TIM6_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx更新中断标志 
		TimerTaskProcess();
	
	}
}



