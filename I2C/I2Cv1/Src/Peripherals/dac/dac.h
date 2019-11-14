/*
 * dac.h
 *
 * Created: 2017-07-17 22:48:08
 *  Author: Zbyszek
 */ 


#ifndef DAC_H_INCLUDED
#define DAC_H_INCLUDED

#include <avr/io.h>

//-----------------------------------------------------------------------------

/* Output channel selection */
//	DAC_CHSEL_SINGLE_gc = (0x00<<5),  /* Single channel operation (Channel 0 only) */
//	DAC_CHSEL_SINGLE1_gc = (0x01<<5),  /* Single channel operation (Channel 1 only) */
//	DAC_CHSEL_DUAL_gc = (0x02<<5),  /* Dual channel operation (Channel 0 and channel 1) */

/* Reference voltage selection */
//	DAC_REFSEL_INT1V_gc = (0x00<<3),  /* Internal 1V  */
//	DAC_REFSEL_AVCC_gc = (0x01<<3),  /* Analog supply voltage */
//	DAC_REFSEL_AREFA_gc = (0x02<<3),  /* External reference on AREF on PORTA */
//	DAC_REFSEL_AREFB_gc = (0x03<<3),  /* External reference on AREF on PORTB */

void DAC_Cfg( DAC_t *pDAC, uint8_t chSel, uint8_t refSel, uint8_t lowPowerMode );
//-----------------------------------------------------------------------------

/* Output channel selection */
//	DAC_CHSEL_SINGLE_gc = (0x00<<5),  /* Single channel operation (Channel 0 only) */
//	DAC_CHSEL_SINGLE1_gc = (0x01<<5),  /* Single channel operation (Channel 1 only) */
//	DAC_CHSEL_DUAL_gc = (0x02<<5),  /* Dual channel operation (Channel 0 and channel 1) */

static inline void DAC_SelCh( DAC_t *pDAC, uint8_t chSel );
//-----------------------------------------------------------------------------

static inline void DAC_Enable( DAC_t *pDAC );
static inline void DAC_Disable( DAC_t *pDAC );
//-----------------------------------------------------------------------------

/* Internal Output Selection */
// == 0					/* output pin */
// != 0					/* AC or ADC input */

static inline void DAC_Ch0_EnableOut( DAC_t *pDAC, uint8_t internalOutSel );
static inline void DAC_Ch0_DisableOut( DAC_t *pDAC );
static inline void DAC_Ch1_EnableOut( DAC_t *pDAC );
static inline void DAC_Ch1_DisableOut( DAC_t *pDAC );
//-----------------------------------------------------------------------------

static inline void DAC_Ch0_WriteData( DAC_t *pDAC, uint16_t data );
static inline void DAC_Ch1_WriteData( DAC_t *pDAC, uint16_t data );
//-----------------------------------------------------------------------------

static inline void DAC_Ch0_EnableEvTrig( DAC_t *pDAC );
static inline void DAC_Ch0_DisableEvTrig( DAC_t *pDAC );
static inline void DAC_Ch1_EnableEvTrig( DAC_t *pDAC );
static inline void DAC_Ch1_DisableEvTrig( DAC_t *pDAC );
//-----------------------------------------------------------------------------

/* Event channel selection */
//    DAC_EVSEL_0_gc = (0x00<<0),  /* Event Channel 0 */
//    DAC_EVSEL_1_gc = (0x01<<0),  /* Event Channel 1 */
//    DAC_EVSEL_2_gc = (0x02<<0),  /* Event Channel 2 */
//    DAC_EVSEL_3_gc = (0x03<<0),  /* Event Channel 3 */
//    DAC_EVSEL_4_gc = (0x04<<0),  /* Event Channel 4 */
//    DAC_EVSEL_5_gc = (0x05<<0),  /* Event Channel 5 */
//    DAC_EVSEL_6_gc = (0x06<<0),  /* Event Channel 6 */
//    DAC_EVSEL_7_gc = (0x07<<0),  /* Event Channel 7 */

static inline void DAC_CfgEvents( DAC_t *pDAC, uint8_t evSel, uint8_t splitEvCh );
//-----------------------------------------------------------------------------

static inline uint8_t DAC_Ch0_DataRegEmpty( DAC_t *pDAC );
static inline uint8_t DAC_Ch1_DataRegEmpty( DAC_t *pDAC );

//=============================================================================

static inline void DAC_SelCh( DAC_t *pDAC, uint8_t chSel )
{
	pDAC->CTRLB = (pDAC->CTRLB & ~DAC_CHSEL_gm) | chSel;
}
//-----------------------------------------------------------------------------

static inline void DAC_Enable( DAC_t *pDAC )
{
	pDAC->CTRLA |= DAC_ENABLE_bm;
}
//-----------------------------------------------------------------------------

static inline void DAC_Disable( DAC_t *pDAC )
{
	pDAC->CTRLA &= ~DAC_ENABLE_bm;
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch0_EnableOut( DAC_t *pDAC, uint8_t internalOutSel )
{
	pDAC->CTRLA = (pDAC->CTRLA & ~(DAC_CH0EN_bm | DAC_IDOEN_bm)) | ((internalOutSel == 0) ? DAC_CH0EN_bm : DAC_IDOEN_bm);
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch0_DisableOut( DAC_t *pDAC )
{
	pDAC->CTRLA &= ~(DAC_CH0EN_bm | DAC_IDOEN_bm);
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch1_EnableOut( DAC_t *pDAC )
{
	pDAC->CTRLA |= DAC_CH1EN_bm;
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch1_DisableOut( DAC_t *pDAC )
{
	pDAC->CTRLA &= ~DAC_CH1EN_bm;
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch0_WriteData( DAC_t *pDAC, uint16_t data )
{
	pDAC->CH0DATA = data;
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch1_WriteData( DAC_t *pDAC, uint16_t data )
{
	pDAC->CH1DATA = data;
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch0_EnableEvTrig( DAC_t *pDAC )
{
	pDAC->CTRLB |= DAC_CH0TRIG_bm;
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch0_DisableEvTrig( DAC_t *pDAC )
{
	pDAC->CTRLB &= ~DAC_CH0TRIG_bm;
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch1_EnableEvTrig( DAC_t *pDAC )
{
	pDAC->CTRLB |= DAC_CH1TRIG_bm;
}
//-----------------------------------------------------------------------------

static inline void DAC_Ch1_DisableEvTrig( DAC_t *pDAC )
{
	pDAC->CTRLB &= ~DAC_CH1TRIG_bm;
}
//-----------------------------------------------------------------------------

static inline void DAC_CfgEvents( DAC_t *pDAC, uint8_t evSel, uint8_t splitEvCh )
{
	pDAC->EVCTRL = evSel | ((splitEvCh != 0) ? DAC_EVSPLIT_bm : 0);
}
//-----------------------------------------------------------------------------

static inline uint8_t DAC_Ch0_DataRegEmpty( DAC_t *pDAC )
{
	return (pDAC->STATUS & DAC_CH0DRE_bm);
}
//-----------------------------------------------------------------------------

static inline uint8_t DAC_Ch1_DataRegEmpty( DAC_t *pDAC )
{
	return (pDAC->STATUS & DAC_CH1DRE_bm);
}
//-----------------------------------------------------------------------------


#endif /* DAC_H_INCLUDED */