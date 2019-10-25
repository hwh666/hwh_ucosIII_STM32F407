#include "ST_AK8975.h"
void ST_AK8975_CS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	AK_CS=1; 
}
static void ak8975_enable(u8 ena)
{
	if(ena)
		AK_CS=0;
	else
		AK_CS=1;
}
static void ak8975_Trig(void)
{
	ak8975_enable(1);
	ST_SPI_ReadWriteByte(SPI1,AK8975_CNTL_REG);
	ST_SPI_ReadWriteByte(SPI1,0x01);
	ak8975_enable(0);
}

static u8 ak8975_buf[6];
void ST_AK8975_Read(void)
{	
	
	ak8975_enable(1);
	ST_SPI_ReadWriteByte(SPI1,AK8975_HXL_REG|0x80);
	for(u8 i=0; i<6; i++)
		ak8975_buf[i] = ST_SPI_ReadWriteByte(SPI1,0xff);
	ak8975_enable(0);	
	ak8975_Trig();
	
	Sensor.AK8975.mag_raw.x = + ((((s16)ak8975_buf[1]) << 8) | ak8975_buf[0]); 
	Sensor.AK8975.mag_raw.y = - ((((s16)ak8975_buf[3]) << 8) | ak8975_buf[2]); 
	Sensor.AK8975.mag_raw.z = - ((((s16)ak8975_buf[5]) << 8) | ak8975_buf[4]); 
	
	Sensor.AK8975.mag.x = Sensor.AK8975.mag_raw.x - Sensor.AK8975.mag_offset.x; 
	Sensor.AK8975.mag.y = (Sensor.AK8975.mag_raw.y - Sensor.AK8975.mag_offset.y) * Sensor.AK8975.mag_gain.y;
	Sensor.AK8975.mag.z = (Sensor.AK8975.mag_raw.z - Sensor.AK8975.mag_offset.z) * Sensor.AK8975.mag_gain.z;
	

	LPF_1(5,0.01f,Sensor.AK8975.mag.x,&(Sensor.AK8975.mag_lpf.x));
	LPF_1(5,0.01f,Sensor.AK8975.mag.y,&(Sensor.AK8975.mag_lpf.y));
	LPF_1(5,0.01f,Sensor.AK8975.mag.z,&(Sensor.AK8975.mag_lpf.z));
	ST_AK8975_Offset();
}

#define CALIBRATING_MAG_CYCLES    1500 //10ms一次  1500次是15s  充分采集数据 时间长点好
void ST_AK8975_Offset(void)
{
	static xyz_f_t	MagMAX = { -100 , -100 , -100 }, MagMIN = { 100 , 100 , 100 }, MagSum;
	static uint16_t cnt_m=0;
	
	if(ST_flag.mag_cal)
	{	
		
		if(ABS(Sensor.AK8975.mag_raw.x)<400&&ABS(Sensor.AK8975.mag_raw.y)<400&&ABS(Sensor.AK8975.mag_raw.z)<400)  //附近有强磁的话 校准也没用 所以做了限幅
		{
			MagMAX.x = _MAX(Sensor.AK8975.mag_raw.x, MagMAX.x);
			MagMAX.y = _MAX(Sensor.AK8975.mag_raw.y, MagMAX.y);
			MagMAX.z = _MAX(Sensor.AK8975.mag_raw.z, MagMAX.z);
			
			MagMIN.x = _MIN(Sensor.AK8975.mag_raw.x, MagMIN.x);
			MagMIN.y = _MIN(Sensor.AK8975.mag_raw.y, MagMIN.y);
			MagMIN.z = _MIN(Sensor.AK8975.mag_raw.z, MagMIN.z);		
			
			if(cnt_m == CALIBRATING_MAG_CYCLES)
			{
				Sensor.AK8975.mag_offset.x = (int16_t)((MagMAX.x + MagMIN.x) * 0.5f);
				Sensor.AK8975.mag_offset.y = (int16_t)((MagMAX.y + MagMIN.y) * 0.5f);
				Sensor.AK8975.mag_offset.z = (int16_t)((MagMAX.z + MagMIN.z) * 0.5f);
	
				MagSum.x = MagMAX.x - MagMIN.x;
				MagSum.y = MagMAX.y - MagMIN.y;
				MagSum.z = MagMAX.z - MagMIN.z;
				
				Sensor.AK8975.mag_gain.y = MagSum.x / MagSum.y;
				Sensor.AK8975.mag_gain.z = MagSum.x / MagSum.z;
				
				ST_flag.needsave=1;//保存数据
				cnt_m = 0;
				ST_flag.mag_cal = 0;
			}
		}
		cnt_m++;
		
	}
}


