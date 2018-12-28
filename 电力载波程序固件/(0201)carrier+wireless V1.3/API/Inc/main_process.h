#ifndef __MAIN_PROCESS_H
#define __MAIN_PROCESS_H
#include "hardware_layer_data_process.h"
//transmit type one byte.
#define TYPE_TRANS_MODE_BIT 0x80
#define TYPE_CODEC_BIT      0x40
#define TYPE_PHASE_BITS     0x30
#define TYPE_ROUT_BITS      0x0f

#define THR_TRANS_MODE        1
#define SIG_TRANS_MODE        0
#define CODEC_MODE            1
#define NOCODEC_MODE          0
#define PHASE_MSG_A           0x1
#define PHASE_MSG_B           0x2
#define PHASE_MSG_C           0x3

//control byte;
#define DATA_DIR_BIT          0x80//���򣬵������0�� ������������
#define DATA_FORMAL_BIT       0x40//����֡���ͣ�0����͸֡��1����֡��
#define BROADCAST_BIT         0x20// 0�ǹ㲥���ݡ�1���㲥���ݡ� 
#define FOLLOW_UP_FRAME_BIT   0x10//0 �޺���֡��1���к���֡��
#define DATA_TYPE_BIT         0x08 //�������� �����ݻ��������ݡ�

#define ROTA_TX_CAOBIAO_REQ           0x01
#define ROTA_TX_REGSTER_REQ           0x02
#define TX_REGISGER_REQ               0x04
#define TX_CAOBIAO_REQ                0x08

typedef struct StructTime_Out{

  unsigned char is_flag,is_sta;  
  unsigned char try_times;
}StructTimeOut;
typedef struct StructNear_addr{

  unsigned char addr[12];
  unsigned char signal_quality[6];
  unsigned char count;  
}StructNearAddr;

static void delay_ms(unsigned int ms);

void Init_lowlevel_para(void);

void slave_com_receive_process(void);
void host_com_receive_process(void);

void slave_power_line_carrier_receive_prcess(void);
void host_power_line_carrier_receive_prcess(void);
void main_process(void);

#endif