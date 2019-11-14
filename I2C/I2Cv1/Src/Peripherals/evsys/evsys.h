/*
 * evsys.h
 *
 * Created: 2017-07-17 22:48:08
 *  Author: Zbyszek
 */ 


#ifndef EVSYS_H_INCLUDED
#define EVSYS_H_INCLUDED

#include <avr/io.h>

//-----------------------------------------------------------------------------

//	chNum = 0, ..., 7
static inline void EVSYS_ChMux( uint8_t chNum, uint8_t evSrc );

//	chNum = 0, ..., 7
/* Digital filter coefficient */
//	EVSYS_DIGFILT_1SAMPLE_gc = (0x00<<0),  /* 1 SAMPLE */
//	EVSYS_DIGFILT_2SAMPLES_gc = (0x01<<0),  /* 2 SAMPLES */
//	EVSYS_DIGFILT_3SAMPLES_gc = (0x02<<0),  /* 3 SAMPLES */
//	EVSYS_DIGFILT_4SAMPLES_gc = (0x03<<0),  /* 4 SAMPLES */
//	EVSYS_DIGFILT_5SAMPLES_gc = (0x04<<0),  /* 5 SAMPLES */
//	EVSYS_DIGFILT_6SAMPLES_gc = (0x05<<0),  /* 6 SAMPLES */
//	EVSYS_DIGFILT_7SAMPLES_gc = (0x06<<0),  /* 7 SAMPLES */
//	EVSYS_DIGFILT_8SAMPLES_gc = (0x07<<0),  /* 8 SAMPLES */
void EVSYS_ChDigFilter( uint8_t chNum, uint8_t digFilt );

//	chNum = 0, 2, 4
//	EVSYS_QDIEN_bm  0x10  /* Quadrature Decoder Index Enable bit mask. */
/* Quadrature Decoder Index Recognition Mode */
//	EVSYS_QDIRM_00_gc = (0x00<<5),  /* QDPH0 = 0, QDPH90 = 0 */
//	EVSYS_QDIRM_01_gc = (0x01<<5),  /* QDPH0 = 0, QDPH90 = 1 */
//	EVSYS_QDIRM_10_gc = (0x02<<5),  /* QDPH0 = 1, QDPH90 = 0 */
//	EVSYS_QDIRM_11_gc = (0x03<<5),  /* QDPH0 = 1, QDPH90 = 1 */

void EVSYS_QD_Enable( uint8_t chNum, uint8_t indexEnable, uint8_t indexMode );
void EVSYS_QD_Disable( uint8_t chNum );

static inline void EVSYS_SendEvent( uint8_t chMask, uint8_t chData );

//=============================================================================

static inline void EVSYS_ChMux( uint8_t chNum, uint8_t evSrc )
{
	*(&EVSYS.CH0MUX + chNum) = evSrc;
}
//----------------------------------------------------------------------------

static inline void EVSYS_SendEvent( uint8_t chMask, uint8_t chData )
{
	EVSYS.DATA = chData;
	EVSYS.STROBE = chMask;
}
//----------------------------------------------------------------------------



#endif /* EVSYS_H_INCLUDED */