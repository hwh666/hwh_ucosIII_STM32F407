#include "ST_ICM20602.h"
void ST_ICM20602_CS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	ICM_CS=1; 
}
static void icm20602_enable(u8 ena)
{
	if(ena)
		ICM_CS=0; //低电平有效
	else
		ICM_CS=1; 
}

static void icm20602_readbuf(u8 reg, u8 length, u8 *data)
{
	icm20602_enable(1);
	ST_SPI_ReadWriteByte(SPI1,reg|0x80);
	ST_SPI_Receive(SPI1,data,length);
	icm20602_enable(0);
}

static u8 icm20602_writebyte(u8 reg, u8 data)
{
	u8 status;
	
	icm20602_enable(1);
	status = ST_SPI_ReadWriteByte(SPI1,reg);
	ST_SPI_ReadWriteByte(SPI1,data);
	icm20602_enable(0);
	return status;
}
/**************************实现函数********************************************
*功　　能:	  读 修改 写 指定设备 指定寄存器一个字节 中的1个位
reg	   寄存器地址
bitNum  要修改目标字节的bitNum位
data  为0 时，目标位将被清0 否则将被置位
*******************************************************************************/
//static void icm20602_writeBit(u8 reg, u8 bitNum, u8 data) 
//{
//    u8 b;
//    icm20602_readbuf(reg, 1, &b);
//    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
//	icm20602_writebyte(reg, b);
//}

/**************************实现函数********************************************
*功　　能:	    初始化icm进入可用状态。
*******************************************************************************/
u8 ST_ICM20602_Init(void)
{

	u8 tmp;
	ST_ICM20602_CS_Init();
	ST_SPI1_Init();
	icm20602_writebyte(MPU_RA_PWR_MGMT_1,0x80);
	ST_Delay_ms(10);
	icm20602_writebyte(MPU_RA_PWR_MGMT_1,0x01);
	ST_Delay_ms(10);
	
	icm20602_readbuf(MPUREG_WHOAMI, 1, &tmp);
	if(tmp != MPU_WHOAMI_20602)
	return 0;

	/*复位reg*/
	icm20602_writebyte(MPU_RA_SIGNAL_PATH_RESET,0x03);
	ST_Delay_ms(10);
  /*复位reg*/
	icm20602_writebyte(MPU_RA_USER_CTRL,0x01);	
	ST_Delay_ms(10);

	icm20602_writebyte(0x70,0x40);
	ST_Delay_ms(10);
	icm20602_writebyte(MPU_RA_PWR_MGMT_2,0x00);
	ST_Delay_ms(10);
	icm20602_writebyte(MPU_RA_SMPLRT_DIV,0);
	ST_Delay_ms(10);

	icm20602_writebyte(MPU_RA_CONFIG,ICM20602_LPF_20HZ);
	ST_Delay_ms(10);
	icm20602_writebyte(MPU_RA_GYRO_CONFIG,(3 << 3));
	ST_Delay_ms(10);
	icm20602_writebyte(MPU_RA_ACCEL_CONFIG,(2 << 3));
	ST_Delay_ms(10);
	/*加速度计LPF 20HZ*/
	icm20602_writebyte(0X1D,0x04);
	ST_Delay_ms(10);
	/*关闭低功耗*/
	icm20602_writebyte(0X1E,0x00);
	ST_Delay_ms(10);
	/*关闭FIFO*/
	icm20602_writebyte(0X23,0x00);
	ST_Delay_ms(10);
	
	Sensor.ICM20602.acc_lpf.z=4096;
	
//	ST_flag.acc_cal = 1; //开机自动对准
//	ST_flag.gyro_cal= 1;//开机自动校准陀螺仪
	return 1;

}
///////////////////////////////////////////////////////////////////////////
xyz_s32_t sum_temp_acc,sum_temp_gyro;
u16 acc_sum_cnt = 0,gyro_sum_cnt = 0;
void ST_ICM_Data_Offset(void)
{
    if(ST_flag.acc_cal)
    {
        acc_sum_cnt++;
        sum_temp_acc.x += Sensor.ICM20602.acc_raw.x;
        sum_temp_acc.y += Sensor.ICM20602.acc_raw.y;
        sum_temp_acc.z += Sensor.ICM20602.acc_raw.z - 4096;   // +-8g  //4096  
        if( acc_sum_cnt >= 50 )
        {
			Sensor.ICM20602.acc_offset.x = (float)sum_temp_acc.x/50;
            Sensor.ICM20602.acc_offset.y = (float)sum_temp_acc.y/50;
			Sensor.ICM20602.acc_offset.z = (float)sum_temp_acc.z/50;
            acc_sum_cnt =0;
			ST_flag.acc_cal=0;
			ST_flag.needsave=1;
			sum_temp_acc.x=sum_temp_acc.y=sum_temp_acc.z=0;
        }
    }
	if(ST_flag.gyro_cal)
    {
        gyro_sum_cnt++;
        sum_temp_gyro.x += Sensor.ICM20602.gyro_raw.x;
        sum_temp_gyro.y += Sensor.ICM20602.gyro_raw.y;
        sum_temp_gyro.z += Sensor.ICM20602.gyro_raw.z ;   
        if( gyro_sum_cnt >= 50 )
        {
			Sensor.ICM20602.gyro_offset.x = (float)sum_temp_gyro.x/50;
            Sensor.ICM20602.gyro_offset.y = (float)sum_temp_gyro.y/50;
			Sensor.ICM20602.gyro_offset.z = (float)sum_temp_gyro.z/50;
            gyro_sum_cnt =0;
			ST_flag.gyro_cal=0;
			ST_flag.needsave=1;
			sum_temp_gyro.x=sum_temp_gyro.y=sum_temp_gyro.z=0;
        }
    }

}
void ST_ICM_DataPrepare(float dT_s)
{
	u8 icm_buffer[14];
	icm20602_readbuf(MPUREG_ACCEL_XOUT_H,14,icm_buffer);
	ST_ICM_Data_Offset();
	/*读取buffer原始数据*/
	Sensor.ICM20602.acc_raw.x =  (s16)((((u16)icm_buffer[0]) << 8) | icm_buffer[1]);
	Sensor.ICM20602.acc_raw.y =  (s16)((((u16)icm_buffer[2]) << 8) | icm_buffer[3]);
	Sensor.ICM20602.acc_raw.z =  (s16)((((u16)icm_buffer[4]) << 8) | icm_buffer[5]);
	
	//temperature= (s16)((((u16)icm_buffer[6]) << 8) | icm_buffer[7]);	sensor.Tempreature_C = sensor.Tempreature/326.8f + 25 ;
	
	Sensor.ICM20602.gyro_raw.x = (s16)((((u16)icm_buffer[ 8]) << 8) | icm_buffer[ 9]);
	Sensor.ICM20602.gyro_raw.y = (s16)((((u16)icm_buffer[10]) << 8) | icm_buffer[11]);
	Sensor.ICM20602.gyro_raw.z = (s16)((((u16)icm_buffer[12]) << 8) | icm_buffer[13]);
	
	Sensor.ICM20602.acc.x = (float)Sensor.ICM20602.acc_raw.x - Sensor.ICM20602.acc_offset.x;
	Sensor.ICM20602.acc.y = (float)Sensor.ICM20602.acc_raw.y - Sensor.ICM20602.acc_offset.y;
	Sensor.ICM20602.acc.z = (float)Sensor.ICM20602.acc_raw.z - Sensor.ICM20602.acc_offset.z;

	Sensor.ICM20602.gyro.x = (float)Sensor.ICM20602.gyro_raw.x - Sensor.ICM20602.gyro_offset.x;
	Sensor.ICM20602.gyro.y = (float)Sensor.ICM20602.gyro_raw.y - Sensor.ICM20602.gyro_offset.y;
	Sensor.ICM20602.gyro.z = (float)Sensor.ICM20602.gyro_raw.z - Sensor.ICM20602.gyro_offset.z;
	
	/*lpf//0.24f，1ms ，50hz截止; 0.15f,1ms,28hz; 0.1f,1ms,18hz*/
	Sensor.ICM20602.acc_lpf.x += 0.12f * (Sensor.ICM20602.acc.x - Sensor.ICM20602.acc_lpf.x) ;
	Sensor.ICM20602.acc_lpf.y += 0.12f * (Sensor.ICM20602.acc.y - Sensor.ICM20602.acc_lpf.y) ;
	Sensor.ICM20602.acc_lpf.z += 0.12f * (Sensor.ICM20602.acc.z - Sensor.ICM20602.acc_lpf.z) ;
	
	Sensor.ICM20602.gyro_lpf.x += 0.12f * (Sensor.ICM20602.gyro.x - Sensor.ICM20602.gyro_lpf.x) ;
	Sensor.ICM20602.gyro_lpf.y += 0.12f * (Sensor.ICM20602.gyro.y - Sensor.ICM20602.gyro_lpf.y) ;
	Sensor.ICM20602.gyro_lpf.z += 0.12f * (Sensor.ICM20602.gyro.z - Sensor.ICM20602.gyro_lpf.z) ;
	
	/*单位转换*/
	Sensor.ICM20602.acc_cmss.x = Sensor.ICM20602.acc_lpf.x * 0.2395f;// cm/s2 厘米每秒方
	Sensor.ICM20602.acc_cmss.y = Sensor.ICM20602.acc_lpf.y * 0.2395f;// cm/s2 
	Sensor.ICM20602.acc_cmss.z = Sensor.ICM20602.acc_lpf.z * 0.2395f;// cm/s2

	Sensor.ICM20602.gyro_deg.x = Sensor.ICM20602.gyro_lpf.x * 0.06103f;//   °/s 度每秒
	Sensor.ICM20602.gyro_deg.y = Sensor.ICM20602.gyro_lpf.y * 0.06103f;
	Sensor.ICM20602.gyro_deg.z = Sensor.ICM20602.gyro_lpf.z * 0.06103f;
	
	Sensor.ICM20602.gyro_rad.x = Sensor.ICM20602.gyro_lpf.x * 0.001065f;// 弧度每秒
	Sensor.ICM20602.gyro_rad.y = Sensor.ICM20602.gyro_lpf.y * 0.001065f;
	Sensor.ICM20602.gyro_rad.z = Sensor.ICM20602.gyro_lpf.z * 0.001065f;
}


