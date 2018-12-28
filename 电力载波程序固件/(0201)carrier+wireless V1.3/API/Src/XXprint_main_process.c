#include "hardware_layer_data_process.h"
#include "main_process.h"
#include "main.h"
#include "stm32f030_peripheral.h"
#include "sample.h"
#include "string.h"
#include "stdlib.h"

#define struct_com_Frame  STDownUartFrame
u16 un_access_10sec_count;
unsigned char Trans_mode = 0;/*����ģʽ*/
unsigned int G_rand;
unsigned int G_rand_ext_delay;
unsigned int G_tick;
unsigned char G_BandRate = 0;
unsigned char G_across_the_phase;
StructTypedefLowLevelDateFrame StructLowLevelFrame;

StructTimeOut register_event_timeout;//��ʱ��

unsigned char lowlevel_buf[224];
unsigned char Local_globe_addr[6];

unsigned char Power_line_tx_flag = 0;
unsigned char bit_regiested_flag = 0;
unsigned char collision_detected_count;

void WDT_Feed (void)
/*===============================================
    ������  :    WDT_Feed
    ����    :    ι��
    ����    :    ��
    ���    :    ��
    ��������:
===============================================*/
{
#ifdef USE_WDT  
  HAL_IWDG_Refresh(&IwdgHandle);
#endif 
}

/***********************************************************���ݽṹ��ʼ��***************************************/
void Init_lowlevel_para(void)
{
  sample_eval_init();
  StructLowLevelFrame.len = 7;
  StructLowLevelFrame.p_buf = lowlevel_buf;
  StructLowLevelFrame.trans_type[0] = 0x91;
  StructLowLevelFrame.trans_type[1] = 0xa1;
  StructLowLevelFrame.trans_type[2] = 0xb1;  
}

/********************************main_process ���������*****************************************/
void main_process(void)
{ 

  unsigned char In67Mode = 0;
  SET_R_LED;
  SET_S_LED;  
  WDT_Feed();
  HAL_Delay(150); 
  WDT_Feed();
  HAL_Delay(150); 
  WDT_Feed();
  HAL_Delay(150);
  CLR_R_LED;
  CLR_S_LED;
  
  while(1)
  {
    WDT_Feed();
    
    if(struct_com_Frame.is_flag & 0x01)//���ڽ������ݴ���
    {
      unsigned char l,*p,m,sum,rota_index,t_type;
      struct_com_Frame.is_flag &= 0xfe; 
      if(struct_com_Frame.head == 0x67)
      {  
        In67Mode = 1;
        l = struct_com_Frame.len;
        p = struct_com_Frame.p_buf;
      
        sum = sum_check(struct_com_Frame.p_buf,struct_com_Frame.len-2);
        l = struct_com_Frame.len-2;
        if(sum != struct_com_Frame.p_buf[l]) return;
       
        
        if(struct_com_Frame.p_buf[0] == 0x01)
        {
          return;
        }
        unsigned char phase = (struct_com_Frame.p_buf[2]&0x0c)>>2;
        set_current_valid_phase(phase);     
        p = struct_com_Frame.p_buf+2;
        m= p[1] & 0xcf; 
        StructLowLevelFrame.trans_type[0] =(0x10 | m);
        StructLowLevelFrame.trans_type[1] =(0x20 | m);
        StructLowLevelFrame.trans_type[2] =(0x30 | m);
        rota_index = m&0x0f;
        t_type = p[2+2+2*rota_index];
        G_BandRate = (t_type&0x0c)>>2;
        p = StructLowLevelFrame.p_buf;
        l = struct_com_Frame.len-6;//ȥǰ4��2����6�ֽ� 
        memmove(p,&struct_com_Frame.p_buf[4],l);//ȥǰ���ֽڡ�//:81 91
        p+=l;
        *p = sum_check(StructLowLevelFrame.p_buf,l); 
        StructLowLevelFrame.len = l+2;//type+�������ݳ���+CRC
        power_line_carrier_transment(Trans_mode+(G_BandRate<<4),&StructLowLevelFrame);
        StructLowLevelFrame.p_buf[StructLowLevelFrame.len - 1] = StructLowLevelFrame.trans_type[0];
        Si4438_Transmit_Start(&WIRELESS_DATA,  WIRELESS_CHANNEL, StructLowLevelFrame.p_buf, StructLowLevelFrame.len);
      }
      else if(struct_com_Frame.head == 0x68)
      {
        In67Mode = 0; 
        t_type = 0x40;  
        StructLowLevelFrame.len = (uint8_t)struct_com_Frame.len;
        StructLowLevelFrame.trans_type[0] = t_type + 0x11;
        StructLowLevelFrame.trans_type[1] = t_type + 0x21;
        StructLowLevelFrame.trans_type[2] = t_type + 0x31;
        rota_index = 0x01;
        
        //���͵�����һ�ڵ�
        p = StructLowLevelFrame.p_buf;
        *p++ = 0xaa;
        *p++ = 0xaa;
        *p++ = 0xaa;//������е�ַ��[0];
        *p++ = 0xaa;
        
        p= StructLowLevelFrame.p_buf + 2 + 2*rota_index;
        
        *p++ = 0x80+(G_BandRate<<2);//��͸����
        *p++ = StructLowLevelFrame.len;//�������ݳ��ȡ�
        memcpy(p,struct_com_Frame.p_buf,StructLowLevelFrame.len);//��ַ��
        p += StructLowLevelFrame.len;
        l = p-StructLowLevelFrame.p_buf;
      
        *p = sum_check(StructLowLevelFrame.p_buf,l); 
      
        //���䷽ʽ��1��+Ŀ���ַ��2��+·�ɱ�2*index��+ �����루1��+ ��֡�����ܳ��ȣ�1��+CRC16��
        StructLowLevelFrame.len += (1+2+2*(StructLowLevelFrame.trans_type[0]&0x0f)+1+1+1);
        
        power_line_carrier_transment(Trans_mode+(G_BandRate<<4),&StructLowLevelFrame);
        StructLowLevelFrame.p_buf[StructLowLevelFrame.len - 1] = StructLowLevelFrame.trans_type[0];
        Si4438_Transmit_Start(&WIRELESS_DATA,  WIRELESS_CHANNEL, StructLowLevelFrame.p_buf, StructLowLevelFrame.len);
      }
      else
        In67Mode = 0;
    }
    unsigned char z_flag = 0;
    z_flag = 0;
    z_flag = power_line_carrier_receive(&StructLowLevelFrame);
    if(z_flag == 1)//&&(z_flag != 4))
    {      
        uint8_t *p,*q;
        uint8_t rota_index,l;
        rota_index = StructLowLevelFrame.trans_type[0] & 0x0f;
        
        if(StructLowLevelFrame.is_my_phase)  
          G_across_the_phase = 0;
        else 
          G_across_the_phase = 1;
        //·����Ȳ���Ϊ0;
        if(rota_index == 0) return;

        l = StructLowLevelFrame.len-2;//��͸�������ݳ���(ȥģʽ�ֽ� �� CRC)��
        p = StructLowLevelFrame.p_buf;
        q = struct_com_Frame.p_buf;
        //���͸�������ģ����оƬ
        *q++ = 0x67;
        *q++ = l+6;//��֡�����ܳ��ȡ�4(head)+l+1(DC)+1(0x16)
        //Ӳ����λ��Ϣ
        *q++ = G_across_the_phase|(get_current_valid_phase()<<2);
        *q++ = StructLowLevelFrame.trans_type[0];
     
        memmove(q,p,l);
     
        q += l;   
        *q++ = sum_check(struct_com_Frame.p_buf,l+4); //�ۼӺ�
        *q = 0x16;            
        l += 6;
        p = struct_com_Frame.p_buf; 
        if(In67Mode == 1)
          HAL_UART_Transmit_IT(&UartHandleDown,p,l); 
        else
          HAL_UART_Transmit_IT(&UartHandleDown,p+8+2*rota_index,l-8-2*rota_index-2);      
    }
  }
}