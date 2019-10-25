#include "include.h"

int old_main(void)
{   
    ST_SysTick_Configuration();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
    LED_Init();
    while(1)
    {  
        LED0=1;
        ST_Delay_ms(500);
        LED0=0;
        ST_Delay_ms(500);
    }	
}
