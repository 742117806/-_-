
#ifndef __UART_H__
#define __UART_H__

#include "stm32f0xx.h"


#define _DEBUG_ 1				//�����Ƿ��ӡ������Ϣ��0����ӡ��1��ӡ

#if _DEBUG_
#define DebugPrintf(...)        printf(__VA_ARGS__)
#define DebugSendBytes(b,n)	    UartSendBytes(USART2,b,n);
#define DebugSendStr(s)  		UartSendStr(USART2,s)
#else
#define DebugPrintf(...) 
#define DebugSendBytes(b,n)
#define DebugSendStr(s)
#endif

extern uint8_t uart1Rec;        //UART1�жϽ������ݱ���
extern uint8_t uart2Rec;		 //UART2�жϽ������ݱ���

void Uart1SendData(uint8_t byte);
void Uart1SendBytes(uint8_t *buf,uint16_t len);
void UartSendData(USART_TypeDef *USARTx,uint8_t byte);
void UartSendBytes(USART_TypeDef *USARTx,uint8_t *buf,uint16_t len);
void UartSendStr(USART_TypeDef *USARTx,char *str);
#endif

