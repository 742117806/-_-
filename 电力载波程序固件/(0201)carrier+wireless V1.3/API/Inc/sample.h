#ifndef __SAMPLE_H
#define __SAMPLE_H
#include "stm32f030_peripheral.h"
#include "stm32f0308_plc.h"


typedef unsigned short u16;
typedef unsigned int u32;

#define SET_R_LED BSP_LED_On(LED_RED)
#define CLR_R_LED BSP_LED_Off(LED_RED)

#define SET_S_LED  BSP_LED_On(LED_BLUE)
#define CLR_S_LED  BSP_LED_Off(LED_BLUE)

#define SET_STA_LED  BSP_LED_On(LED_STA)
#define CLR_STA_LED  BSP_LED_Off(LED_STA)
#ifdef ISPLC_13_1X
#define  TEST_EVEN_IN  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)
#else
#define  TEST_EVEN_IN  HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)
#endif
typedef enum FSK_MODE
{
   TXmode = 0,
   RXmode = 1,
}FSK_mode;

typedef enum __phase
{
   PhaseA = 0,
   PhaseB = 1,
   PhaseC = 2,
   PhaseAll = 3,
}t_phase;

/*λ�����ṹ��*/
typedef struct struct_bit_sample{ 
  unsigned char accept_byte_buf_800;
  unsigned char accept_byte_buf_400;//λ���պ󱣴浽���ֽڻ��档 
  unsigned char accept_byte_buf_200;//λ���պ󱣴浽���ֽڻ��档
  unsigned char accept_byte_buf_100;//λ���պ󱣴浽���ֽڻ��档
  unsigned char accept_bit_count_800;//���յ���λ������
  unsigned char accept_bit_count_400;//���յ���λ������ 
  unsigned char accept_bit_count_200;//���յ���λ������
  unsigned char accept_bit_count_100;//���յ���λ������ 
}StructTypedefBitSample;

/*�ֽڲ����ṹ��*/
typedef struct struct_byte_sample {
  unsigned char buf_800;
  unsigned char buf_400;//buf0
  unsigned char buf_200;//
  unsigned char buf_100;//
}StructTypedefByteSample;

/*��λ�ж���ͬ������ṹ��*/
typedef struct struct_phase_byte_sample{ 
  StructTypedefBitSample bit_sample[3];
  StructTypedefByteSample byte_sample[3]; 
  unsigned char extension_sample_phase;
  //u16 extension_bit[3]; 
  
}StructTypedefPhaseByteSample;

//�ڲ�������
static void Trans_a_phase_bits(void);
static void prepare_buf(void);
static void start_trans_bits(void);
void set_fsk_mode(FSK_mode mode,t_phase phase);
FSK_mode get_fsk_mode(t_phase phase);
unsigned char get_phase_power_value(void);
//��������
extern void WDT_Feed(void); 
void sample_eval_init(void);
void ReInit_Para(unsigned char *p);
void start_send_buf(unsigned char mode,t_phase phase);
unsigned char LowLevel_send_slice(uint64_t buf,t_phase phase);
unsigned char LowLevel_send_byte(unsigned char *p,unsigned char sBandrate,t_phase phase);
void start_send_buf2(unsigned char mode);
void stop_send_buf(t_phase phase);
unsigned char lowLevel_get_800bps_phase_data(unsigned char *p,unsigned char phase);
unsigned char lowLevel_get_400bps_phase_data(unsigned char *p,unsigned char phase);
unsigned char lowLevel_get_200bps_phase_data(unsigned char *p,unsigned char phase);
unsigned char lowLevel_get_100bps_phase_data(unsigned char *p,unsigned char phase);
unsigned char lowLevel_get_800bps_phase_data2(unsigned char *p,unsigned char phase);
unsigned char lowLevel_get_400bps_phase_data2(unsigned char *p,unsigned char phase);
unsigned char lowLevel_get_200bps_phase_data2(unsigned char *p,unsigned char phase);
unsigned char lowLevel_get_100bps_phase_data2(unsigned char *p,unsigned char phase);

void Signal_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void PhaseTimeCallBack(void); 
void TimeSliceProcessPhase(void);
void SampleTimeCallBack(void);
void user_slice__prcess(void);
void relatedprocess(uint64_t sPhase,unsigned char phase);
void ADC_SampleRelatedProcess(uint8_t *ADCBuff);

#endif