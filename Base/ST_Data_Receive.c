#include "ST_Data_Receive.h"

extern u8 steps[];
extern u8 Sign_act;
Adjust_LED LED_ctrl;
u8 RxBuffer[50];
void ANO_DT_Data_Receive_Prepare(u8 data)
{
	static u8 _data_len = 0,_data_cnt = 0;
	static u8 state = 0;
	
    switch(state)
	{
		case 0: 
            if(data==0x53)              //"S"
             {
                state=1;
                RxBuffer[0]=data;
             }else state = 0;
        break;
             
		case 1:
			if(data==0x54)              //"T"
			{
				state=2;
				RxBuffer[1]=data;
			}else state = 0;
		break;
			
		case 2:                          //"C"
			{
				state=3;
				RxBuffer[2]=data;
			} 
		break;
			
		case 3:                          //"L"
			{
				state = 4;
				RxBuffer[3]=data;
				_data_len = data;
				_data_cnt = 0;
			}
		break;

		case 4:
			if(_data_len>0)
			{
                switch(RxBuffer[2])
                {
                    case 0x79:                    //O(order:动作序列)
                        _data_len--;		
                        RxBuffer[4+_data_cnt++]=((data>>4)&0x0f); 
                        if(_data_len==0) 
                            state = 5;	
                        else
                        {
                            _data_len--;
                           RxBuffer[4+_data_cnt++]=(data&0x0f); 	
                           if(_data_len==0)
                              state = 5;
                        } 
                      break;
                    case 0x43:                    //C(Command)   
                        _data_len--;
                        RxBuffer[4+_data_cnt++]=data;
                        if(_data_len==0)	state = 5;   
                    break; 
                    case 0x42:                    //B(Bright)   
                        _data_len--;
                        RxBuffer[4+_data_cnt++]=data;
                        if(_data_len==0)	state = 5;   
                    break;                    
                }
				
			}else state = 0;
		break;			
			
		case 5:
			state = 0;
			RxBuffer[4+_data_cnt]=data;
			ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);
		break;

		default:	state = 0;	break;
	}
}


void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num)
{
	u8 sum = 0;
//	for(u8 i=0;i<(num-1);i++)
//		sum += *(data_buf+i);
//	if(!(sum/num==*(data_buf+num-1)))		return;		 //判断sum
	//if(!(*(data_buf)==0x53 && *(data_buf+1)==0x54))	
        if((data_buf[0]!=0x53)||(data_buf[1]!=0x54))
        return;		//判断帧头
	
    switch(*(data_buf+2))         
    {
        case 0x79:                    //"O"
           break;
        case 0x43:                   //"C"
            break; 
        case 0x42:  
              LED_ctrl.LED_Adjust_Flag=1;
              LED_ctrl.LED_ID=*(data_buf+4);
              LED_ctrl.LED_State=*(data_buf+5);
              LED_ctrl.LED_Offset=*(data_buf+6);
        break;  
    }
    
}


