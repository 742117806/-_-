#include "hardware_layer_data_process.h"
#include "sample.h"
#include "main.h"
#include "74.h"
#define MAX_BUF 224
//#define DBG
//#define PRINT_ALL

static Trans_Mode transmission_mode;
static unsigned char current_valid_sample_phase; //���յ��ź����ڵ������λ����ʱ��Ƭ��
static unsigned char negated_sample_phase;
static unsigned char turn_over_flag;
static u32 buf32bitA, buf32bitB, buf32bitC;
static u32 buf32bitA1, buf32bitB1, buf32bitC1;
static u32 buf32bitA2, buf32bitB2, buf32bitC2;
static u32 buf32bitA3, buf32bitB3, buf32bitC3;
extern unsigned char GloblePhaseValue;
extern u16 un_access_10sec_count;
extern StructTypedefLowLevelDateFrame StructLowLevelFrame; //�ײ��ز����ݽ��սṹ������

void set_current_valid_phase( unsigned char phase )
{
    current_valid_sample_phase = phase;
}

unsigned char get_current_valid_phase( void )
{
    return current_valid_sample_phase;
}

/****************************************��ʼ������*********************************************
**********************************************************************************************/
static void re_synchronization( void )
{
    un_access_10sec_count = 0;
    transmission_mode = SignalMode; //���෢��ģ��
}

static ERR receive_len_message_800bps( unsigned char* p, unsigned char* samle_start_phase, unsigned char* cmd )
{
    unsigned char flag, tmp_buf, tmp_bufA, tmp_bufB, tmp_bufC, tmp_bufD, L;
    u16* pa, *pb, *pc;
    static unsigned char f_phase = 0;
    static unsigned char* ptr, head_valid_count;
    ERR err = ERR_not_data;
    transmission_mode = SignalMode;
    f_phase++;
    if( f_phase > 2 )
    {
        f_phase = 0;
    }
    /*800bps�Ƿ���յ�һ������*/
    flag = lowLevel_get_800bps_phase_data( &tmp_buf, f_phase );
    if( flag )
    {
        pa = ( u16* )&buf32bitA3;
        pb = ( u16* )&buf32bitB3;
        pc = ( u16* )&buf32bitC3;
        pa++;
        pb++;
        pc++;
        if( f_phase == 0 )
        {
            buf32bitA3 <<= 8;
            buf32bitA3 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitA3;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_800bps_phase_data( &tmp_buf, SAMPLE_PHASE_B ) )
                    ;
                buf32bitB3 <<= 8;
                buf32bitB3 |= tmp_buf;
                while( !lowLevel_get_800bps_phase_data( &tmp_buf, SAMPLE_PHASE_C ) )
                    ;
                buf32bitC3 <<= 8;
                buf32bitC3 |= tmp_buf;
                if( *pa == *pb )
                {
                    head_valid_count++;
                }
                if( *pa == *pc )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = tmp_bufA; //(unsigned char )buf32bitA3;
                *p++ = ( unsigned char )buf32bitB3;
                *p = ( unsigned char )buf32bitC3;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
        else if( f_phase == 1 )
        {
            buf32bitB3 <<= 8;
            buf32bitB3 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitB3;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_800bps_phase_data( &tmp_buf, SAMPLE_PHASE_C ) )
                    ;
                buf32bitC3 <<= 8;
                buf32bitC3 |= tmp_buf;
                while( !lowLevel_get_800bps_phase_data( &tmp_buf, SAMPLE_PHASE_A ) )
                    ;
                buf32bitA3 <<= 8;
                buf32bitA3 |= tmp_buf;
                if( *pb == *pa )
                {
                    head_valid_count++;
                }
                if( *pb == *pc )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = ( unsigned char )buf32bitB3;
                *p++ = ( unsigned char )buf32bitC3;
                *p = ( unsigned char )buf32bitA3;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
        else if( f_phase == 2 )
        {
            buf32bitC3 <<= 8;
            buf32bitC3 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitC3;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_800bps_phase_data( &tmp_buf, SAMPLE_PHASE_A ) )
                    ;
                buf32bitA3 <<= 8;
                buf32bitA3 |= tmp_buf;
                while( !lowLevel_get_800bps_phase_data( &tmp_buf, SAMPLE_PHASE_B ) )
                    ;
                buf32bitB3 <<= 8;
                buf32bitB3 |= tmp_buf;
                if( *pc == *pa )
                {
                    head_valid_count++;
                }
                if( *pc == *pb )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = ( unsigned char )buf32bitC3;
                *p++ = ( unsigned char )buf32bitA3;
                *p = ( unsigned char )buf32bitB3;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
    }
    return err;
}

/********************************************������ʼ��Ϣ******************************************
**********************************************************************************************/
static ERR receive_len_message( unsigned char* p, unsigned char* samle_start_phase, unsigned char* cmd )
{
    unsigned char flag, tmp_buf, tmp_bufA, tmp_bufB, tmp_bufC, tmp_bufD, L;
    u16* pa, *pb, *pc;
    static unsigned char f_phase = 0;
    static unsigned char* ptr, head_valid_count;
    ERR err = ERR_not_data;
    transmission_mode = SignalMode;
    f_phase++;
    if( f_phase > 2 )
    {
        f_phase = 0;
    }
    /*400bps�Ƿ���յ�һ������*/
    flag = lowLevel_get_400bps_phase_data( &tmp_buf, f_phase );
    if( flag )
    {
        pa = ( u16* )&buf32bitA;
        pb = ( u16* )&buf32bitB;
        pc = ( u16* )&buf32bitC;
        pa++;
        pb++;
        pc++;
        if( f_phase == 0 )
        {
            buf32bitA <<= 4;
            buf32bitA |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitA;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_400bps_phase_data( &tmp_buf, SAMPLE_PHASE_B ) )
                    ;
                buf32bitB <<= 4;
                buf32bitB |= tmp_buf;
                while( !lowLevel_get_400bps_phase_data( &tmp_buf, SAMPLE_PHASE_C ) )
                    ;
                buf32bitC <<= 4;
                buf32bitC |= tmp_buf;
                if( *pa == *pb )
                {
                    head_valid_count++;
                }
                if( *pa == *pc )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = tmp_bufA; //(unsigned char )buf32bitA;
                *p++ = ( unsigned char )buf32bitB;
                *p = ( unsigned char )buf32bitC;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
        else if( f_phase == 1 )
        {
            buf32bitB <<= 4;
            buf32bitB |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitB;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_400bps_phase_data( &tmp_buf, SAMPLE_PHASE_C ) )
                    ;
                buf32bitC <<= 4;
                buf32bitC |= tmp_buf;
                while( !lowLevel_get_400bps_phase_data( &tmp_buf, SAMPLE_PHASE_A ) )
                    ;
                buf32bitA <<= 4;
                buf32bitA |= tmp_buf;
                if( *pb == *pa )
                {
                    head_valid_count++;
                }
                if( *pb == *pc )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = ( unsigned char )buf32bitB;
                *p++ = ( unsigned char )buf32bitC;
                *p = ( unsigned char )buf32bitA;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
        else if( f_phase == 2 )
        {
            buf32bitC <<= 4;
            buf32bitC |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitC;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_400bps_phase_data( &tmp_buf, SAMPLE_PHASE_A ) )
                    ;
                buf32bitA <<= 4;
                buf32bitA |= tmp_buf;
                while( !lowLevel_get_400bps_phase_data( &tmp_buf, SAMPLE_PHASE_B ) )
                    ;
                buf32bitB <<= 4;
                buf32bitB |= tmp_buf;
                if( *pc == *pa )
                {
                    head_valid_count++;
                }
                if( *pc == *pb )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = ( unsigned char )buf32bitC;
                *p++ = ( unsigned char )buf32bitA;
                *p = ( unsigned char )buf32bitB;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
    }
    return err;
}

/********************************************200bps������ʼ��Ϣ************************
**********************************************************************************************/
static ERR receive_len_message_200bps( unsigned char* p, unsigned char* samle_start_phase, unsigned char* cmd )
{
    unsigned char flag, tmp_buf, tmp_bufA, tmp_bufB, tmp_bufC, tmp_bufD, L;
    u16* pa, *pb, *pc;
    static unsigned char f_phase = 0;
    static unsigned char* ptr, head_valid_count;
    ERR err = ERR_not_data;
    transmission_mode = SignalMode;
    f_phase++;
    if( f_phase > 2 )
    {
        f_phase = 0;
    }
    /*200bps�Ƿ���յ�һ������*/
    flag = lowLevel_get_200bps_phase_data( &tmp_buf, f_phase );
    if( flag )
    {
        pa = ( u16* )&buf32bitA1;
        pb = ( u16* )&buf32bitB1;
        pc = ( u16* )&buf32bitC1;
        pa++;
        pb++;
        pc++;
        if( f_phase == 0 )
        {
            buf32bitA1 <<= 2;
            buf32bitA1 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitA1;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_200bps_phase_data( &tmp_buf, SAMPLE_PHASE_B ) )
                    ;
                buf32bitB1 <<= 2;
                buf32bitB1 |= tmp_buf;
                while( !lowLevel_get_200bps_phase_data( &tmp_buf, SAMPLE_PHASE_C ) )
                    ;
                buf32bitC1 <<= 2;
                buf32bitC1 |= tmp_buf;
                if( *pa == *pb )
                {
                    head_valid_count++;
                }
                if( *pa == *pc )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = tmp_bufA; //(unsigned char )buf32bitA;
                *p++ = ( unsigned char )buf32bitB1;
                *p = ( unsigned char )buf32bitC1;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
        else if( f_phase == 1 )
        {
            buf32bitB1 <<= 2;
            buf32bitB1 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitB1;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_200bps_phase_data( &tmp_buf, SAMPLE_PHASE_C ) )
                    ;
                buf32bitC1 <<= 2;
                buf32bitC1 |= tmp_buf;
                while( !lowLevel_get_200bps_phase_data( &tmp_buf, SAMPLE_PHASE_A ) )
                    ;
                buf32bitA1 <<= 2;
                buf32bitA1 |= tmp_buf;
                if( *pb == *pa )
                {
                    head_valid_count++;
                }
                if( *pb == *pc )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = ( unsigned char )buf32bitB1;
                *p++ = ( unsigned char )buf32bitC1;
                *p = ( unsigned char )buf32bitA1;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
        else if( f_phase == 2 )
        {
            buf32bitC1 <<= 2;
            buf32bitC1 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitC1;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_200bps_phase_data( &tmp_buf, SAMPLE_PHASE_A ) )
                    ;
                buf32bitA1 <<= 2;
                buf32bitA1 |= tmp_buf;
                while( !lowLevel_get_200bps_phase_data( &tmp_buf, SAMPLE_PHASE_B ) )
                    ;
                buf32bitB1 <<= 2;
                buf32bitB1 |= tmp_buf;
                if( *pc == *pa )
                {
                    head_valid_count++;
                }
                if( *pc == *pb )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = ( unsigned char )buf32bitC1;
                *p++ = ( unsigned char )buf32bitA1;
                *p = ( unsigned char )buf32bitB1;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
    }
    return err;
}

/********************************************100bps������ʼ��Ϣ************************
**********************************************************************************************/
static ERR receive_len_message_100bps( unsigned char* p, unsigned char* samle_start_phase, unsigned char* cmd )
{
    unsigned char flag, tmp_buf, tmp_bufA, tmp_bufB, tmp_bufC, tmp_bufD, L;
    u16* pa, *pb, *pc;
    static unsigned char f_phase = 0;
    static unsigned char* ptr, head_valid_count;
    ERR err = ERR_not_data;
    transmission_mode = SignalMode;
    f_phase++;
    if( f_phase > 2 )
    {
        f_phase = 0;
    }
    /*100bps�Ƿ���յ�һ������*/
    flag = lowLevel_get_100bps_phase_data( &tmp_buf, f_phase );
    if( flag )
    {
        pa = ( u16* )&buf32bitA2;
        pb = ( u16* )&buf32bitB2;
        pc = ( u16* )&buf32bitC2;
        pa++;
        pb++;
        pc++;
        if( f_phase == 0 )
        {
            buf32bitA2 <<= 1;
            buf32bitA2 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitA2;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_100bps_phase_data( &tmp_buf, SAMPLE_PHASE_B ) )
                    ;
                buf32bitB2 <<= 1;
                buf32bitB2 |= tmp_buf;
                while( !lowLevel_get_100bps_phase_data( &tmp_buf, SAMPLE_PHASE_C ) )
                    ;
                buf32bitC2 <<= 1;
                buf32bitC2 |= tmp_buf;
                if( *pa == *pb )
                {
                    head_valid_count++;
                }
                if( *pa == *pc )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = tmp_bufA; //(unsigned char )buf32bitA;
                *p++ = ( unsigned char )buf32bitB2;
                *p = ( unsigned char )buf32bitC2;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
        else if( f_phase == 1 )
        {
            buf32bitB2 <<= 1;
            buf32bitB2 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitB2;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_100bps_phase_data( &tmp_buf, SAMPLE_PHASE_C ) )
                    ;
                buf32bitC2 <<= 1;
                buf32bitC2 |= tmp_buf;
                while( !lowLevel_get_100bps_phase_data( &tmp_buf, SAMPLE_PHASE_A ) )
                    ;
                buf32bitA2 <<= 1;
                buf32bitA2 |= tmp_buf;
                if( *pb == *pa )
                {
                    head_valid_count++;
                }
                if( *pb == *pc )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = ( unsigned char )buf32bitB2;
                *p++ = ( unsigned char )buf32bitC2;
                *p = ( unsigned char )buf32bitA2;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
        else if( f_phase == 2 )
        {
            buf32bitC2 <<= 1;
            buf32bitC2 |= tmp_buf;
            ptr = ( unsigned char* )&buf32bitC2;
            tmp_bufA = *ptr++;
            tmp_bufB = *ptr++;
            tmp_bufC = *ptr++;
            tmp_bufD = *ptr;
            tmp_bufB = ~tmp_bufB;
            if( ( ( tmp_bufD == 0xac ) || ( tmp_bufD == 0x98 ) ) && ( tmp_bufC == tmp_bufB ) )
            {
                L = tmp_bufC;
                if( tmp_bufD == 0x98 )
                {
                    *cmd = 1;
                }
                else
                {
                    *cmd = 0;
                }
                head_valid_count = 1;
                current_valid_sample_phase = f_phase;
                while( !lowLevel_get_100bps_phase_data( &tmp_buf, SAMPLE_PHASE_A ) )
                    ;
                buf32bitA2 <<= 1;
                buf32bitA2 |= tmp_buf;
                while( !lowLevel_get_100bps_phase_data( &tmp_buf, SAMPLE_PHASE_B ) )
                    ;
                buf32bitB2 <<= 1;
                buf32bitB2 |= tmp_buf;
                if( *pc == *pa )
                {
                    head_valid_count++;
                }
                if( *pc == *pb )
                {
                    head_valid_count++;
                }
                if( head_valid_count > 1 )
                {
                    transmission_mode = ThrphaseMode;
                }
                *p++ = L;
                *p++ = ( unsigned char )buf32bitC2;
                *p++ = ( unsigned char )buf32bitA2;
                *p = ( unsigned char )buf32bitB2;
                *samle_start_phase = f_phase;
                err = NO_ERR;
                return err;
            }
        }
    }
    return err;
}

/***************************************����һ�ֽ�����*********************************************
**********************************************************************************************/
typedef unsigned char( fnc_t )( unsigned char* p, unsigned char );
static ERR receive_data_message( unsigned char* p, unsigned char isBandRate )
{
    fnc_t* fnc_P;
    unsigned char tmp_buf, tmp_bufA, tmp_bufB, tmp_bufC;
    ERR err;
    err = ERR_not_data;
    if( isBandRate == 0 )
    {
        fnc_P = lowLevel_get_400bps_phase_data2;
    }
    else if( isBandRate == 1 )
    {
        fnc_P = lowLevel_get_200bps_phase_data2;
    }
    else if( isBandRate == 2 )
    {
        fnc_P = lowLevel_get_100bps_phase_data2;
    }
    else
    {
        fnc_P = lowLevel_get_800bps_phase_data2;
    }
    if( transmission_mode == ThrphaseMode ) /*����ͬ��ģʽ*/
    {
        if( fnc_P( &tmp_buf, current_valid_sample_phase ) )
        {
            tmp_bufA = tmp_buf;
            while( !fnc_P( &tmp_buf, current_valid_sample_phase + 1 ) )
                ;
            tmp_bufB = tmp_buf;
            while( !fnc_P( &tmp_buf, current_valid_sample_phase + 2 ) )
                ;
            tmp_bufC = tmp_buf;
            if( turn_over_flag )
            {
                if( negated_sample_phase == ( current_valid_sample_phase % 3 ) )
                {
                    tmp_bufA = ~tmp_bufA;
                }
                else if( negated_sample_phase == ( ( current_valid_sample_phase + 1 ) % 3 ) )
                {
                    tmp_bufB = ~tmp_bufB;
                }
                else
                {
                    tmp_bufC = ~tmp_bufC;
                }
            }
            if( ( tmp_bufB == tmp_bufA ) || ( tmp_bufA == tmp_bufC ) )
            {
                *p = tmp_bufA;
            }
            else if( tmp_bufB == tmp_bufC )
            {
                *p = tmp_bufB;
            }
            else
            {
                *p = tmp_bufA;
            }
            err = NO_ERR;
            return err;
        }
    }
    else //��ʱ��Ƭģ�飬ֻ��һ��ʱ��Ƭ��ȷ�յ���AC��L ~L��
    {
#ifdef DBG
        if( fnc_P( &tmp_buf, current_valid_sample_phase ) )
        {
            err = NO_ERR;
            *p = tmp_buf;
            USART6_TX_Buf( tmp_buf );
            return err;
        }
#else
        if( fnc_P( &tmp_buf, current_valid_sample_phase ) )
        {
            err = NO_ERR;
            if( turn_over_flag )
            {
                if( current_valid_sample_phase == negated_sample_phase )
                {
                    tmp_buf = ~tmp_buf;
                }
            }
            *p = tmp_buf;
            return err;
        }
#endif
    }
    return err;
}

/*************************************�Ӻͳ���**************************************************/

unsigned char sum_check( unsigned char* p, unsigned char len )
{
    unsigned char sum;
    sum = 0;
    while( len-- )
    {
        sum += *p++;
    }
    return ( sum );
}
/*************************************�����߽��ճ���**************************************************/
unsigned char power_line_carrier_receive( StructTypedefLowLevelDateFrame* LowLevel_frame_r )
{
    unsigned char tmp_buf, head_buf[4], L, i, j, *p, start_phase, cmd, c_type, isBand;
    //#ifndef MASTER
    //#if 0
    CLR_R_LED;
    //#endif
    if( receive_len_message_200bps( head_buf, &start_phase, &cmd ) == NO_ERR )
    {
        isBand = 1;
        goto receive;
    }
    if( receive_len_message_100bps( head_buf, &start_phase, &cmd ) == NO_ERR )
    {
        isBand = 2;
        goto receive;
    }
    if( receive_len_message_800bps( head_buf, &start_phase, &cmd ) == NO_ERR )
    {
        isBand = 3;
        goto receive;
    }
    if( receive_len_message( head_buf, &start_phase, &cmd ) == NO_ERR ) /*��ȷ���յ�ͷ֡�ͳ����ֽ�*/
    {
        isBand = 0;
receive:
        SET_R_LED;
#ifndef MASTER
#ifdef PRINT_ALL
        SET_R_LED;
#endif
#else
        SET_R_LED;
#endif
        tmp_buf = head_buf[0];
        LowLevel_frame_r->len = tmp_buf;
        if( tmp_buf == 0xf0 )
        {
            return 4;    /*head ��ͻ֡*/
        }
        L = tmp_buf;
        if( L > MAX_BUF )
        {
            return 3;    //���ݹ�����
        }
        j = L;
        i = 0;
        LowLevel_frame_r->is_my_phase = 0;
        LowLevel_frame_r->enable74codec = cmd;
        if( LowLevel_frame_r->enable74codec )
        {
            while( receive_data_message( &head_buf[0], isBand ) != NO_ERR )
                ;
            tmp_buf = decode16_74bit( ( head_buf[1] << 8 ) + head_buf[0] );
            LowLevel_frame_r->trans_type[0] = tmp_buf;
        }
        else
        {
            LowLevel_frame_r->trans_type[0] = head_buf[1];
        }
        /*��ȡ�����ֽ�����*/
        c_type = LowLevel_frame_r->trans_type[0];
        if( start_phase == ( ( c_type & 0x30 ) >> 4 ) - 1 ) /*�Ǳ���λ������*/
        {
            LowLevel_frame_r->is_my_phase = 2;
        }
        else /*����λ����*/
        {
            //      if((c_type & 0x40) == 0) return 6;/*��������࣬�˳�*/
#ifdef PRINT_ALL
#else
            if( ( c_type & 0x40 ) == 0 )
            {
                return 6;    /*��������࣬�˳�*/
            }
#endif
        }
        p = LowLevel_frame_r->p_buf;
        if( p == 0 )
        {
            return 2;    //�����ݻ��档
        }
        while( j-- )
        {
            WDT_Feed();
            if( LowLevel_frame_r->enable74codec )
            {
                while( receive_data_message( &head_buf[1], isBand ) != NO_ERR )
                    ;
                while( receive_data_message( &head_buf[0], isBand ) != NO_ERR )
                    ;
                tmp_buf = decode16_74bit( ( head_buf[1] << 8 ) + head_buf[0] );
            }
            else
            {
                while( receive_data_message( &tmp_buf, isBand ) != NO_ERR )
                    ;
            }
            p[i] = tmp_buf;
            i++;
        }
        i -= 2;
        if( p[i] != sum_check( p, i ) )
        {
            return 5; /*check sum error*/
        }
        return 1; /*recieve OK */
    }
    return 0;
}

/*************************************�����߷��ͳ���**************************************************/
unsigned char power_line_carrier_transment( unsigned char mode, StructTypedefLowLevelDateFrame* LowLevel_frame_t )
{
    unsigned char buf[3], i, N, j, *p;
#ifndef ISPLC_13_1X
    unsigned char power_value;
#endif
    unsigned char SBandRate;
    WIRELESS_LED_FLAG = 2; //���Ҫ�����ز���ͨ���˱�־������ߵ���
    t_phase sPhase = PhaseAll;
#if 1
    SET_STA_LED;
    SET_S_LED;
#endif
#ifdef ISPLC_13_1X
    if( current_valid_sample_phase == 0 )
    {
        start_send_buf( mode, PhaseA );
        sPhase = PhaseA;
    }
    else if( current_valid_sample_phase == 1 )
    {
        start_send_buf( mode, PhaseB );
        sPhase = PhaseB;
    }
    else
    {
        start_send_buf( mode, PhaseC );
        sPhase = PhaseC;
    }
#else
    power_value = get_phase_power_value();
    if( ( power_value & 0x07 ) == 0x07 ) //������λͬʱ�ϵ硣
    {
        if( current_valid_sample_phase == 0 )
        {
            start_send_buf( mode, PhaseA );
            sPhase = PhaseA;
        }
        else if( current_valid_sample_phase == 1 )
        {
            start_send_buf( mode, PhaseB );
            sPhase = PhaseB;
        }
        else
        {
            start_send_buf( mode, PhaseC );
            sPhase = PhaseC;
        }
    }
    else if( power_value & 0x01 )
    {
        start_send_buf( mode, PhaseA );
        sPhase = PhaseA;
    }
    else if( power_value & 0x02 )
    {
        start_send_buf( mode, PhaseB );
        sPhase = PhaseB;
    }
    else if( power_value & 0x04 )
    {
        start_send_buf( mode, PhaseC );
        sPhase = PhaseC;
    }
    else
    {
        start_send_buf( mode, PhaseA );
        sPhase = PhaseA;
    }
#endif
    SBandRate = ( ( mode >> 4 ) & 0x03 );
    if( LowLevel_frame_t->enable74codec )
    {
        buf[0] = 0xf3;
    }
    else
    {
        buf[0] = 0xac;
    }
    WDT_Feed();
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    if( LowLevel_frame_t->enable74codec )
    {
        buf[0] = 0x98;
    }
    else
    {
        buf[0] = 0xac;
    }
    WDT_Feed();
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    N = LowLevel_frame_t->len;
    buf[0] = N;
    WDT_Feed();
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    buf[0] = ~N;
    WDT_Feed();
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    uint8_t jcc = sPhase;
    buf[0] = LowLevel_frame_t->trans_type[jcc % 3];
    WDT_Feed();
    if( LowLevel_frame_t->enable74codec )
    {
        uint16_t buf16bit;
        buf16bit = code16_74bit( buf[0] );
        buf[1] = buf16bit >> 8;
        buf[0] = buf16bit & 0xff;
        while( !LowLevel_send_byte( &buf[1], SBandRate, sPhase ) )
            ;
        while( !LowLevel_send_byte( &buf[0], SBandRate, sPhase ) )
            ;
    }
    else
        while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
            ;
    p = LowLevel_frame_t->p_buf;
    N--;
    for( i = 0; i < N; i++ )
    {
        j = *p++;
        buf[0] = j;
        //UartSendData(USART2,j);
        /*�ȴ��������ݿ�*/
        WDT_Feed();
        if( LowLevel_frame_t->enable74codec )
        {
            uint16_t buf16bit;
            buf16bit = code16_74bit( buf[0] );
            buf[1] = buf16bit >> 8;
            buf[0] = buf16bit & 0xff;
            while( !LowLevel_send_byte( &buf[1], SBandRate, sPhase ) )
                ;
            while( !LowLevel_send_byte( &buf[0], SBandRate, sPhase ) )
                ;
        }
        else
            while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
                ;
    };
    WDT_Feed();
    //�ȴ��������
    //  UartSendData(USART2,buf[0]);
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    WDT_Feed();
    // UartSendData(USART2,buf[0]);
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    //������ɡ�
#ifndef ISPLC_13_1X
    if( mode & 0x01 )
    {
        stop_send_buf( PhaseA );
        stop_send_buf( PhaseB );
        stop_send_buf( PhaseC );
    }
    else
#endif
        stop_send_buf( sPhase );
    re_synchronization();
#if 1
    CLR_S_LED;
    CLR_STA_LED;
#endif
    /********* ������߷��� ***********/
    //StructLowLevelFrame.p_buf[StructLowLevelFrame.len - 1] = StructLowLevelFrame.trans_type[0];
    //Si4438_Transmit_Start(&WIRELESS_DATA, WIRELESS_CHANNEL, StructLowLevelFrame.p_buf, StructLowLevelFrame.len);
    return 0;
}
/*************************************�����߷���ͬ����Ϣ**************************************************/

void power_line_carrier_send_head( unsigned char mode )
{
    unsigned char buf[3], SBandRate;
#ifndef ISPLC_13_1X
    unsigned char power_value;
#endif
    static t_phase sPhase = PhaseAll;
#if 1
    SET_S_LED;
#endif
#ifdef ISPLC_13_1X
    if( current_valid_sample_phase == 0 )
    {
        start_send_buf( mode, PhaseA );
        sPhase = PhaseA;
    }
    else if( current_valid_sample_phase == 1 )
    {
        start_send_buf( mode, PhaseB );
        sPhase = PhaseB;
    }
    else
    {
        start_send_buf( mode, PhaseC );
        sPhase = PhaseC;
    }
#else
    power_value = get_phase_power_value();
    if( ( power_value & 0x07 ) == 0x07 ) //������λͬʱ�ϵ硣
    {
        if( current_valid_sample_phase == 0 )
        {
            start_send_buf( mode, PhaseA );
            sPhase = PhaseA;
        }
        else if( current_valid_sample_phase == 1 )
        {
            start_send_buf( mode, PhaseB );
            sPhase = PhaseB;
        }
        else
        {
            start_send_buf( mode, PhaseC );
            sPhase = PhaseC;
        }
    }
    else if( power_value & 0x01 )
    {
        start_send_buf( mode, PhaseA );
        sPhase = PhaseA;
    }
    else if( power_value & 0x02 )
    {
        start_send_buf( mode, PhaseB );
        sPhase = PhaseB;
    }
    else if( power_value & 0x04 )
    {
        start_send_buf( mode, PhaseC );
        sPhase = PhaseC;
    }
    else
    {
        start_send_buf( mode, PhaseA );
        sPhase = PhaseA;
    }
#endif
    SBandRate = ( ( mode >> 4 ) & 0x03 );
    buf[0] = 0xac;
    buf[1] = 0xac;
    buf[2] = 0xac;
    WDT_Feed();
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    buf[0] = 0xf0;
    buf[1] = 0xf0;
    buf[2] = 0xf0;
    WDT_Feed();
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    buf[0] = 0x0f;
    buf[1] = 0x0f;
    buf[2] = 0x0f;
    /*�ȴ��������ݿ�*/
    WDT_Feed();
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    /*�ȴ��������*/
    WDT_Feed();
    while( !LowLevel_send_byte( buf, SBandRate, sPhase ) )
        ;
    //������ɡ�
#ifndef ISPLC_13_1X
    if( mode & 0x01 )
    {
        stop_send_buf( PhaseA );
        stop_send_buf( PhaseB );
        stop_send_buf( PhaseC );
    }
    else
#endif
        stop_send_buf( sPhase );
    transmission_mode = SignalMode; //���෢��ģ��
#if 1
    CLR_S_LED;
#endif
}