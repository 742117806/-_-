#ifndef __HARDWARE_DATA_PROCESS_H
#define __HARDWARE_DATA_PROCESS_H
#include "intrinsics.h"
typedef enum ERR_No
{
   NO_ERR = 0,
   ERR_not_data = 1,
 //  ERR_err_data = 2,
}ERR;

typedef enum TRANS_Mode
{
   ThrphaseMode = 0,
   SignalMode = 1,
}Trans_Mode;

typedef enum RECV_Mode
{
   RecvThrphaseMode = 0,
   RecvSignalMode = 1,

}Recv_Mode;

typedef enum SAMPLE_Phase
{
   SAMPLE_PHASE_A = 0,
   SAMPLE_PHASE_B = 1,
   SAMPLE_PHASE_C = 2,

}Sample_Phase;

typedef struct struct_lowlevel_data_frame{
  unsigned char *p_buf;//���յ�������ָ�롣  
  unsigned char trans_type[3];//����ʱ��Ƭ���ԵĴ������͡�
  unsigned char is_my_phase;
  unsigned char mode;//��ʱ��Ƭ���Ƕ�ʱ��Ƭģ��
  unsigned char len;//���յ���λ������  
  unsigned char enable74codec;

}StructTypedefLowLevelDateFrame;



//���Ժ��� DBG�¿��á�
unsigned char lowLevel_get_phase_data_demo(unsigned char phase);
void hardware_data_demo();
void hardware_data_receive_demo(void);
void hardware_data_send_demo(void);


//�ڲ�����
static void re_synchronization(void);
static ERR receive_cmd_message(unsigned char *p);
static ERR receive_data_message(unsigned char *p,unsigned char );

//ȫ�ֺ���
void set_current_valid_phase(unsigned char phase);
unsigned char get_current_valid_phase(void);
void power_line_carrier_send_head(unsigned char mode);
unsigned char sum_check(unsigned char *p,unsigned char len);
unsigned char CRC_Check_1(unsigned char *ptr,unsigned char len);
unsigned char  power_line_carrier_receive(StructTypedefLowLevelDateFrame *LowLevel_frame_r);
unsigned char power_line_carrier_transment(unsigned char mode,StructTypedefLowLevelDateFrame *LowLevel_frame_t);
#endif