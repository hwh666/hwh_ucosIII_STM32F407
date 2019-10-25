#include "ST_StepMotor.h"
#define Adjustment_DELAY_us 1500 //微调脉冲延时 单位us
#define PulsesPerCycle 1600	  //1600个脉冲一圈
//#define SUBDIVISION 8 //步进电机细分数  
#define Quater 0.25f
#define sum_time   5

#define DIR_DELAY  ST_Delay_us(20)		////换向延时
#define AIR_DELAY  120					////手指开合延时
int L_Mul=0;
float L_JudgeDirection=0;     /////圈数标记
int R_Mul=0;
float R_JudgeDirection=0;
int Cycle_Number_Res=0;

#define KZ90AcNum  98
int KZAc90[98]={
93,75,63,55,50,45,42,39,36,34,33,31,30,28,27,26,25,25,24,23,22,22,21,
21,20,20,19,19,19,18,18,18,17,17,17,16,16,16,16,15,15,15,15,15,14,14,14,14,
14,14,13,13,13,13,13,13,13,12,12,12,12,12,12,12,12,12,12,11,11,11,11,11,11,
11,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,9,8,8,8
};
int KZAc180[98]={
93,75,63,55,50,45,42,39,36,34,33,31,30,28,27,26,25,25,24,23,22,22,21,
21,20,20,19,19,19,18,18,18,17,17,17,16,16,16,16,15,15,15,15,15,14,14,14,14,
14,14,13,13,13,13,13,13,13,12,12,12,12,12,12,12,12,12,12,11,11,11,11,11,11,
11,11,11,11,11,11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,9,9,8,8,8
};
//拧动90度
#define ND90AcNum  108
int NDAc90[108]={
91,73,62,54,48,44,41,38,35,33,32,30,29,28,27,26,25,24,23,22,22,21,21,
20,20,19,19,18,18,18,17,17,17,16,16,16,16,15,15,15,15,15,14,14,14,14,14,13,
13,13,13,13,13,13,12,12,12,12,12,12,12,12,12,11,11,11,11,11,11,11,11,11,11,
11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
9,9,9,9,9,9,9,9,9,9
};
//拧动180度
#define ND180AcNum  108
int NDAc180[108]={
91,73,62,54,48,44,41,38,35,33,32,30,29,28,27,26,25,24,23,22,22,21,21,
20,20,19,19,18,18,18,17,17,17,16,16,16,16,15,15,15,15,15,14,14,14,14,14,13,
13,13,13,13,13,13,12,12,12,12,12,12,12,12,12,11,11,11,11,11,11,11,11,11,11,
11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
9,9,9,9,9,9,9,9,9,9
};
//带动90度
#define DD90AcNum  183
int DDAc90[183]={
96,91,87,83,80,77,74,72,70,68,66,64,62,
61,59,58,56,55,54,53,52,51,50,49,48,47,47,46,45,45,44,43,43,42,41,41,40,40,
39,39,38,38,38,37,37,36,36,36,35,35,35,34,34,34,33,33,33,32,32,32,32,31,31,
31,31,30,30,30,30,29,29,29,29,29,28,28,28,28,28,27,27,27,27,27,27,26,26,26,
26,26,26,26,25,25,25,25,25,25,25,24,24,24,24,24,24,24,24,24,23,23,23,23,23,
23,23,23,23,22,22,22,22,22,22,22,22,22,22,22,21,21,21,21,21,21,21,21,21,21,
21,21,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,18,18,18,18,18,18,18,18,18,18,18
};
//带动180度
#define DD180AcNum  183
int DDAc180[183]={
96,91,87,83,80,77,74,72,70,68,66,64,62,
61,59,58,56,55,54,53,52,51,50,49,48,47,47,46,45,45,44,43,43,42,41,41,40,40,
39,39,38,38,38,37,37,36,36,36,35,35,35,34,34,34,33,33,33,32,32,32,32,31,31,
31,31,30,30,30,30,29,29,29,29,29,28,28,28,28,28,27,27,27,27,27,27,26,26,26,
26,26,26,26,25,25,25,25,25,25,25,24,24,24,24,24,24,24,24,24,23,23,23,23,23,
23,23,23,23,22,22,22,22,22,22,22,22,22,22,22,21,21,21,21,21,21,21,21,21,21,
21,21,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,19,19,19,19,19,19,19,19,
19,19,19,19,19,19,19,19,19,18,18,18,18,18,18,18,18,18,18,18
};
void ST_LeftHandAdjustment(u8 dir,int pulses)              ///左手微调
{
	LeftHand_Dir=dir;
	for(int i=0;i<pulses;i++)
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(Adjustment_DELAY_us);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(Adjustment_DELAY_us);
	}
	LeftHand_Dir=0;
}
void ST_RightHandAdjustment(u8 dir,int pulses)            ///右手微调
{
	RightHand_Dir=dir;
	for(int i=0;i<pulses;i++)
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(Adjustment_DELAY_us);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(Adjustment_DELAY_us);
	}
	RightHand_Dir=0;
}

////左手
u32 time;
void ST_LeftHand_KZ90(void)               ////空转90°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/4;
	int sum=sum_time;	
	for(int i=0;i<98;i++)                       //加速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc90[i]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc90[i]*sum);
	}
	for(int i=98;i<302;i++)                     //匀速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc90[97]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc90[97]*sum);
	}
	for(int i=302;i<400;i++)                    //减速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc90[totalPulse-i-1]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc90[totalPulse-i-1]*sum);
	}	
	time=ST_GetSysTime_us()-sta;
}
void ST_LeftHand_KZ180(void)              ////空转180°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/2;
	for(int i=0;i<98;i++)                       //加速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc180[i]*5);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc180[i]*5);
	}
	for(int i=98;i<702;i++)                     //匀速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc180[97]*5);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc180[97]*5);
	}
	for(int i=702;i<800;i++)                    //减速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc180[totalPulse-i-1]*5);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(KZAc180[totalPulse-i-1]*5);
	}
	time=ST_GetSysTime_us()-sta;
}


void ST_LeftHand_ND90(void)               ////拧动90°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/4;
	int sum=sum_time;
	for(int i=0;i<108;i++)                       //加速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc90[i]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc90[i]*sum);
	}
	for(int i=108;i<292;i++)                     //匀速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc90[107]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc90[107]*sum);
	}
	for(int i=292;i<400;i++)                    //减速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc90[totalPulse-i-1]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc90[totalPulse-i-1]*sum);
	}	
	time=ST_GetSysTime_us()-sta;

}
void ST_LeftHand_ND180(void)              ////拧动180°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/2;
	int sum=sum_time;
	for(int i=0;i<108;i++)                       //加速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc180[i]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc180[i]*sum);
	}
	for(int i=108;i<692;i++)                     //匀速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc180[107]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc180[107]*sum);
	}
	for(int i=692;i<800;i++)                    //减速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc180[totalPulse-i-1]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(NDAc180[totalPulse-i-1]*sum);
	}
	time=ST_GetSysTime_us()-sta;

}
void ST_LeftHand_DD90(void)               ////带动90°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/4;
	int sum=sum_time;
	for(int i=0;i<183;i++)                       //加速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc90[i]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc90[i]*sum);
	}
	for(int i=183;i<217;i++)                     //匀速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc90[182]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc90[182]*sum);
	}
	for(int i=217;i<400;i++)                    //减速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc90[totalPulse-i-1]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc90[totalPulse-i-1]*sum);
	}
	time=ST_GetSysTime_us()-sta;

}


void ST_LeftHand_DD180(void)              ////带动180°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/2;
	int sum=sum_time;
	for(int i=0;i<183;i++)                       //加速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc180[i]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc180[i]*sum);
	}
	for(int i=183;i<617;i++)                     //匀速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc180[182]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc180[182]*sum);
	}
	for(int i=617;i<800;i++)                    //减速阶段
	{
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc180[totalPulse-i-1]*sum);
		LeftHand_Pulse=!LeftHand_Pulse;
		ST_Delay_us(DDAc180[totalPulse-i-1]*sum);
	}
	time=ST_GetSysTime_us()-sta;

}



/////右手
void ST_RightHand_KZ90(void)               ////空转90°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/4;
	int sum=sum_time;	
	for(int i=0;i<98;i++)                       //加速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc90[i]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc90[i]*sum);
	}
	for(int i=98;i<302;i++)                     //匀速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc90[97]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc90[97]*sum);
	}
	for(int i=302;i<400;i++)                    //减速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc90[totalPulse-i-1]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc90[totalPulse-i-1]*sum);
	}	
	time=ST_GetSysTime_us()-sta;
}
void ST_RightHand_KZ180(void)              ////空转180°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/2;
	for(int i=0;i<98;i++)                       //加速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc180[i]*5);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc180[i]*5);
	}
	for(int i=98;i<702;i++)                     //匀速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc180[97]*5);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc180[97]*5);
	}
	for(int i=702;i<800;i++)                    //减速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc180[totalPulse-i-1]*5);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(KZAc180[totalPulse-i-1]*5);
	}
	time=ST_GetSysTime_us()-sta;
}


void ST_RightHand_ND90(void)               ////拧动90°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/4;
	int sum=sum_time;
	for(int i=0;i<108;i++)                       //加速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc90[i]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc90[i]*sum);
	}
	for(int i=108;i<292;i++)                     //匀速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc90[107]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc90[107]*sum);
	}
	for(int i=292;i<400;i++)                    //减速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc90[totalPulse-i-1]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc90[totalPulse-i-1]*sum);
	}	
	time=ST_GetSysTime_us()-sta;

}
void ST_RightHand_ND180(void)              ////拧动180°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/2;
	int sum=sum_time;
	for(int i=0;i<108;i++)                       //加速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc180[i]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc180[i]*sum);
	}
	for(int i=108;i<692;i++)                     //匀速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc180[107]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc180[107]*sum);
	}
	for(int i=692;i<800;i++)                    //减速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc180[totalPulse-i-1]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(NDAc180[totalPulse-i-1]*sum);
	}
	time=ST_GetSysTime_us()-sta;

}
void ST_RightHand_DD90(void)               ////带动90°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/4;
	int sum=sum_time;
	for(int i=0;i<183;i++)                       //加速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc90[i]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc90[i]*sum);
	}
	for(int i=183;i<217;i++)                     //匀速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc90[182]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc90[182]*sum);
	}
	for(int i=217;i<400;i++)                    //减速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc90[totalPulse-i-1]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc90[totalPulse-i-1]*sum);
	}
	time=ST_GetSysTime_us()-sta;

}


void ST_RightHand_DD180(void)              ////带动180°
{
	u32 sta=ST_GetSysTime_us();
	int totalPulse=PulsesPerCycle/2;
	int sum=sum_time;
	for(int i=0;i<183;i++)                       //加速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc180[i]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc180[i]*sum);
	}
	for(int i=183;i<617;i++)                     //匀速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc180[182]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc180[182]*sum);
	}
	for(int i=617;i<800;i++)                    //减速阶段
	{
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc180[totalPulse-i-1]*sum);
		RightHand_Pulse=!RightHand_Pulse;
		ST_Delay_us(DDAc180[totalPulse-i-1]*sum);
	}
	time=ST_GetSysTime_us()-sta;

}




void ST_LeftHand_Move(u8 act)
{
	L_JudgeDirection=L_Mul*Quater;	
	switch(act)
	{
		case  L_C:LeftHand_Air=CLOSE;ST_Delay_ms(AIR_DELAY);
			break;
		case  L_O:LeftHand_Air=OPEN;ST_Delay_ms(25);
			break;
		case  L_1:
		{
			if(LeftHand_Air==CLOSE)
			{
				LeftHand_Dir=CW;
				DIR_DELAY;
				if(RightHand_Air==CLOSE)
					{
						ST_LeftHand_ND90();
						L_Mul+=1;
					}
				else	
					{
						ST_LeftHand_DD90();	
						L_Mul+=1;
					}							
			}
			else
			{
				if(L_JudgeDirection>1|L_JudgeDirection<-1)
				{
					LeftHand_Dir=!LeftHand_Dir;		
					DIR_DELAY;	
				}					
				ST_LeftHand_KZ90();
				L_Mul=L_Mul-LeftHand_Dir*2+1;
			}
			break;
		}
		case  L_2:
		{		
				if(L_JudgeDirection>1|L_JudgeDirection<-1)
				{
					LeftHand_Dir=!LeftHand_Dir;		
					DIR_DELAY;
				}
				
				if(RightHand_Air==CLOSE)
				{
					ST_LeftHand_ND180();
					L_Mul=L_Mul-LeftHand_Dir*4+2;
				}
				else
				{
					ST_LeftHand_DD180();
					L_Mul=L_Mul-LeftHand_Dir*4+2;
				}																	
			break;
		}
		case  L_3:
		{
			if(LeftHand_Air==CLOSE)
			{
				LeftHand_Dir=CCW;
				DIR_DELAY;
				if(RightHand_Air==CLOSE)
				{
					ST_LeftHand_ND90();
					L_Mul-=1;
				}
				else
				{
					ST_LeftHand_DD90();	
					L_Mul-=1;						
				}							
			}
			else
			{
				if(L_JudgeDirection>1|L_JudgeDirection<-1)
				{
					LeftHand_Dir=!LeftHand_Dir;	
					DIR_DELAY;
				}
				ST_LeftHand_KZ90();
			    L_Mul=L_Mul-LeftHand_Dir*2+1;
			}
			break;
		 }
	}

}


void ST_RightHand_Move(u8 act)
{
	R_JudgeDirection=R_Mul*Quater;	
	switch(act)
	{
		case  R_C:RightHand_Air=CLOSE;ST_Delay_ms(AIR_DELAY);
			break;
		case  R_O:RightHand_Air=OPEN;ST_Delay_ms(25);
			break;
		case  R_1:
		{
			if(RightHand_Air==CLOSE)
			{
				RightHand_Dir=CW;
				DIR_DELAY;
				if(LeftHand_Air==CLOSE)
					{
						ST_RightHand_ND90();
						R_Mul+=1;
					}
				else	
					{
						ST_RightHand_DD90();	
						R_Mul+=1;
					}							
			}
			else
			{
				if(R_JudgeDirection>1|R_JudgeDirection<-1)
				{
					RightHand_Dir=!RightHand_Dir;
					DIR_DELAY;
				}				  
				ST_RightHand_KZ90();
				R_Mul=R_Mul-RightHand_Dir*2+1;
			}
			break;
		}
		case  R_2:
		{		
				if(R_JudgeDirection>1|R_JudgeDirection<-1)
				{
					RightHand_Dir=!RightHand_Dir;
					DIR_DELAY;
				}						
				if(LeftHand_Air==CLOSE)
				{
					ST_RightHand_ND180();
					R_Mul=R_Mul-RightHand_Dir*4+2;
				}
				else
				{
					ST_RightHand_DD180();
					R_Mul=R_Mul-RightHand_Dir*4+2;
				}																	
			break;
		}
		case  R_3:
		{
			if(RightHand_Air==CLOSE)
			{
				RightHand_Dir=CCW;
				DIR_DELAY;
				if(LeftHand_Air==CLOSE)
				{
					ST_RightHand_ND90();
					R_Mul-=1;
				}
				else
				{
					ST_RightHand_DD90();	
					R_Mul-=1;						
				}							
			}
			else
			{
				if(R_JudgeDirection>1|R_JudgeDirection<-1)
				{
					RightHand_Dir=!RightHand_Dir;		
					DIR_DELAY;
				}
				ST_RightHand_KZ90();
			    R_Mul=R_Mul-RightHand_Dir*2+1;
			}
			break;
		 }
	}
		Cycle_Number_Res=R_JudgeDirection/Quater;
}


void Hand_Move(u8 act)
{
	if(act<5)
		ST_LeftHand_Move(act);
	else
		ST_RightHand_Move(act);
}









