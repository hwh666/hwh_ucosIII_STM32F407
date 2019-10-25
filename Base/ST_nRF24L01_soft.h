#ifndef __nRF24L01_H
#define __nRF24L01_H

/*********     24L01���ͽ������ݿ�ȶ���	  ***********/
#define ST_TX_ADR_WIDTH    5     //5�ֽڵ�ַ���
#define ST_RX_ADR_WIDTH    5     //5�ֽڵ�ַ���
#define ST_TX_PLOAD_WIDTH  32    //32�ֽ���Ч���ݿ��
#define ST_RX_PLOAD_WIDTH  32    //32�ֽ���Ч���ݿ��
/**********  NRF24L01�Ĵ�����������  ***********/
#define ST_nRF_READ_REG        0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define ST_nRF_WRITE_REG       0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define ST_RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define ST_WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define ST_FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define ST_FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define ST_REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define ST_NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
/**********  NRF24L01�Ĵ�����ַ   *************/
#define ST_CONFIG          0x00  //���üĴ�����ַ                             
#define ST_EN_AA           0x01  //ʹ���Զ�Ӧ���� 
#define ST_EN_RXADDR       0x02  //���յ�ַ����
#define ST_SETUP_AW        0x03  //���õ�ַ���(��������ͨ��)
#define ST_SETUP_RETR      0x04  //�����Զ��ط�
#define ST_RF_CH           0x05  //RFͨ��
#define ST_RF_SETUP        0x06  //RF�Ĵ���
#define ST_STATUS          0x07  //״̬�Ĵ���
#define ST_OBSERVE_TX      0x08  // ���ͼ��Ĵ���
#define ST_CD              0x09  // �ز����Ĵ���
#define ST_RX_ADDR_P0      0x0A  // ����ͨ��0���յ�ַ
#define ST_RX_ADDR_P1      0x0B  // ����ͨ��1���յ�ַ
#define ST_RX_ADDR_P2      0x0C  // ����ͨ��2���յ�ַ
#define ST_RX_ADDR_P3      0x0D  // ����ͨ��3���յ�ַ
#define ST_RX_ADDR_P4      0x0E  // ����ͨ��4���յ�ַ
#define ST_RX_ADDR_P5      0x0F  // ����ͨ��5���յ�ַ
#define ST_TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define ST_RX_PW_P0        0x11  // ��������ͨ��0��Ч���ݿ��(1~32�ֽ�) 
#define ST_RX_PW_P1        0x12  // ��������ͨ��1��Ч���ݿ��(1~32�ֽ�) 
#define ST_RX_PW_P2        0x13  // ��������ͨ��2��Ч���ݿ��(1~32�ֽ�) 
#define ST_RX_PW_P3        0x14  // ��������ͨ��3��Ч���ݿ��(1~32�ֽ�) 
#define ST_RX_PW_P4        0x15  // ��������ͨ��4��Ч���ݿ��(1~32�ֽ�)
#define ST_RX_PW_P5        0x16  // ��������ͨ��5��Ч���ݿ��(1~32�ֽ�)
#define ST_FIFO_STATUS     0x17  // FIFO״̬�Ĵ���
/*����������������������������������������������������������������������������������������������������������������������������������������*/

/******   STATUS�Ĵ���bitλ����      *******/
#define ST_MAX_TX  	0x10  	  //�ﵽ����ʹ����ж�
#define ST_TX_OK   	0x20  	  //TX��������ж�
#define ST_RX_OK   	0x40  	  //���յ������ж�
/*����������������������������������������������������������������������������������������������������*/

#endif
