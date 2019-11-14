/*
 * adc.h
 *
 * Created: 2017-04-18 22:48:08
 *  Author: Zbyszek
 */ 


#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

#include <avr/io.h>

#define ADC_CH0_MASK	(1 << 0)
#define ADC_CH1_MASK	(1 << 1)
#define ADC_CH2_MASK	(1 << 2)
#define ADC_CH3_MASK	(1 << 3)

//-----------------------------------------------------------------------------

/* Conversion mode */
#define ADC_CONVMODE_UNSIGNED_bm	0
#define ADC_CONVMODE_SIGNED_bm		ADC_CONMODE_bm

/* Conversion result resolution */
//	ADC_RESOLUTION_12BIT_gc = (0x00<<1),		/* 12-bit right-adjusted result */
//	ADC_RESOLUTION_8BIT_gc = (0x02<<1),			/* 8-bit right-adjusted result */
//	ADC_RESOLUTION_LEFT12BIT_gc = (0x03<<1),	/* 12-bit left-adjusted result */

void ADC_Cfg1( ADC_t *pADC, uint8_t convMode, uint8_t resolution );
//-----------------------------------------------------------------------------

/* Impedance Mode */
#define ADC_IMPEDANCEMODE_HIGH_bm	0
#define ADC_IMPEDANCEMODE_LOW_bm	ADC_IMPMODE_bm

/* Current Limitation Mode */
//	ADC_CURRLIMIT_NO_gc = (0x00<<5),			/* No limit */
//	ADC_CURRLIMIT_LOW_gc = (0x01<<5),			/* Low current limit, max. sampling rate 1.5MSPS */
//	ADC_CURRLIMIT_MED_gc = (0x02<<5),			/* Medium current limit, max. sampling rate 1MSPS */
//	ADC_CURRLIMIT_HIGH_gc = (0x03<<5),			/* High current limit, max. sampling rate 0.5MSPS */

void ADC_Cfg2( ADC_t *pADC, uint8_t impedanceMode, uint8_t currentLimit );
//-----------------------------------------------------------------------------

/* Clock prescaler */
//	ADC_PRESCALER_DIV4_gc = (0x00<<0),			/* Divide clock by 4 */
//	ADC_PRESCALER_DIV8_gc = (0x01<<0),			/* Divide clock by 8 */
//	ADC_PRESCALER_DIV16_gc = (0x02<<0),			/* Divide clock by 16 */
//	ADC_PRESCALER_DIV32_gc = (0x03<<0),			/* Divide clock by 32 */
//	ADC_PRESCALER_DIV64_gc = (0x04<<0),			/* Divide clock by 64 */
//	ADC_PRESCALER_DIV128_gc = (0x05<<0),		/* Divide clock by 128 */
//	ADC_PRESCALER_DIV256_gc = (0x06<<0),		/* Divide clock by 256 */
//	ADC_PRESCALER_DIV512_gc = (0x07<<0),		/* Divide clock by 512 */

static inline void ADC_CfgClk( ADC_t *pADC, uint8_t clkPres );
//-----------------------------------------------------------------------------

/* Voltage reference selection */
//	ADC_REFSEL_INT1V_gc = (0x00<<4),		/* Internal 1V */
//	ADC_REFSEL_INTVCC_gc = (0x01<<4),		/* Internal VCC / 1.6 */
//	ADC_REFSEL_AREFA_gc = (0x02<<4),		/* External reference on PORT A */
//	ADC_REFSEL_AREFB_gc = (0x03<<4),		/* External reference on PORT B */
//	ADC_REFSEL_INTVCC2_gc = (0x04<<4),		/* Internal VCC / 2 */

static inline void ADC_CfgVRef( ADC_t *pADC, uint8_t refSel );
//-----------------------------------------------------------------------------

static inline void ADC_Enable( ADC_t *pADC );
static inline void ADC_Disable( ADC_t *pADC );

static inline void ADC_Flush( ADC_t *pADC );

static inline void ADC_EnableBandgap( ADC_t *pADC );
static inline void ADC_DisableBandgap( ADC_t *pADC );

static inline void ADC_EnableTempref( ADC_t *pADC );
static inline void ADC_DisableTempref( ADC_t *pADC );

static inline void ADC_SetCompareVal( ADC_t *pADC, uint16_t cmpVal );
//-----------------------------------------------------------------------------

static inline void ADC_StartConv( ADC_t *pADC, uint8_t chMask );
static inline void ADC_WaitEndOfConv( ADC_t *pADC, uint8_t chMask );

/* Channel sweep selection (chSweep argument) */
//	ADC_SWEEP_0_gc = (0x00<<6),			/* ADC Channel 0 */
//	ADC_SWEEP_01_gc = (0x01<<6),		/* ADC Channel 0,1 */
//	ADC_SWEEP_012_gc = (0x02<<6),		/* ADC Channel 0,1,2 */
//	ADC_SWEEP_0123_gc = (0x03<<6),		/* ADC Channel 0,1,2,3 */

static inline void ADC_StartFreeRun( ADC_t *pADC, uint8_t chSweep );
static inline void ADC_StopFreeRun( ADC_t *pADC );
//-----------------------------------------------------------------------------

/* Channel sweep selection (chSweep argument) */
//	ADC_SWEEP_0_gc = (0x00<<6),			/* ADC Channel 0 */
//	ADC_SWEEP_01_gc = (0x01<<6),		/* ADC Channel 0,1 */
//	ADC_SWEEP_012_gc = (0x02<<6),		/* ADC Channel 0,1,2 */
//	ADC_SWEEP_0123_gc = (0x03<<6),		/* ADC Channel 0,1,2,3 */

/* Event channel input selection (evSel argument) */
//	ADC_EVSEL_0123_gc = (0x00<<3),		/* Event Channel 0,1,2,3 */
//	ADC_EVSEL_1234_gc = (0x01<<3),		/* Event Channel 1,2,3,4 */
//	ADC_EVSEL_2345_gc = (0x02<<3),		/* Event Channel 2,3,4,5 */
//	ADC_EVSEL_3456_gc = (0x03<<3),		/* Event Channel 3,4,5,6 */
//	ADC_EVSEL_4567_gc = (0x04<<3),		/* Event Channel 4,5,6,7 */
//	ADC_EVSEL_567_gc = (0x05<<3),		/* Event Channel 5,6,7 */
//	ADC_EVSEL_67_gc = (0x06<<3),		/* Event Channel 6,7 */
//	ADC_EVSEL_7_gc = (0x07<<3),			/* Event Channel 7 */

/* Event action selection (evAct argument) */
//	ADC_EVACT_NONE_gc = (0x00<<0),		/* No event action */
//	ADC_EVACT_CH0_gc = (0x01<<0),		/* First event triggers channel 0 */
//	ADC_EVACT_CH01_gc = (0x02<<0),		/* First two events trigger channel 0,1 */
//	ADC_EVACT_CH012_gc = (0x03<<0),		/* First three events trigger channel 0,1,2 */
//	ADC_EVACT_CH0123_gc = (0x04<<0),	/* Events trigger channel 0,1,2,3 */
//	ADC_EVACT_SWEEP_gc = (0x05<<0),		/* First event triggers sweep */
//	ADC_EVACT_SYNCSWEEP_gc = (0x06<<0),	/* The ADC is flushed and restarted for accurate timing */

static inline void ADC_CfgEvents( ADC_t *pADC, uint8_t chSweep, uint8_t evSel, uint8_t evAct );
//-----------------------------------------------------------------------------

/* Input mode (inputMode argument) */
//	ADC_CH_INPUTMODE_INTERNAL_gc = (0x00<<0),		/* Internal inputs, no gain */
//	ADC_CH_INPUTMODE_SINGLEENDED_gc = (0x01<<0),	/* Single-ended input, no gain */
//	ADC_CH_INPUTMODE_DIFF_gc = (0x02<<0),			/* Differential input, no gain */
//	ADC_CH_INPUTMODE_DIFFWGAIN_gc = (0x03<<0),		/* Differential input, with gain */

/* Positive input multiplexer selections - internal input (Input Mode = 0) (muxPos argument) */
//	ADC_CH_MUXINT_TEMP_gc = (0x00<<3),			/* Temperature Reference */
//	ADC_CH_MUXINT_BANDGAP_gc = (0x01<<3),		/* Bandgap Reference */
//	ADC_CH_MUXINT_SCALEDVCC_gc = (0x02<<3),		/* 1/10 scaled VCC */
//	ADC_CH_MUXINT_DAC_gc = (0x03<<3),			/* DAC output */

/* Positive input multiplexer selection (Input Mode = 1, 2 or 3) (muxPos argument) */
//	ADC_CH_MUXPOS_PIN0_gc = (0x00<<3),			/* Input pin 0 (Input Mode = 1, 2, 3) */
//	ADC_CH_MUXPOS_PIN1_gc = (0x01<<3),			/* Input pin 1 (Input Mode = 1, 2, 3) */
//	ADC_CH_MUXPOS_PIN2_gc = (0x02<<3),			/* Input pin 2 (Input Mode = 1, 2, 3) */
//	ADC_CH_MUXPOS_PIN3_gc = (0x03<<3),			/* Input pin 3 (Input Mode = 1, 2, 3) */
//	ADC_CH_MUXPOS_PIN4_gc = (0x04<<3),			/* Input pin 4 (Input Mode = 1, 2, 3) */
//	ADC_CH_MUXPOS_PIN5_gc = (0x05<<3),			/* Input pin 5 (Input Mode = 1, 2, 3) */
//	ADC_CH_MUXPOS_PIN6_gc = (0x06<<3),			/* Input pin 6 (Input Mode = 1, 2, 3) */
//	ADC_CH_MUXPOS_PIN7_gc = (0x07<<3),			/* Input pin 7 (Input Mode = 1, 2, 3) */
//	ADC_CH_MUXPOS_PIN8_gc = (0x08<<3),			/* Input pin 8 (Input Mode = 1, 2) */
//	ADC_CH_MUXPOS_PIN9_gc = (0x09<<3),			/* Input pin 9 (Input Mode = 1, 2) */
//	ADC_CH_MUXPOS_PIN10_gc = (0x0A<<3),			/* Input pin 10 (Input Mode = 1, 2) */
//	ADC_CH_MUXPOS_PIN11_gc = (0x0B<<3),			/* Input pin 11 (Input Mode = 1, 2) */
//	ADC_CH_MUXPOS_PIN12_gc = (0x0C<<3),			/* Input pin 12 (Input Mode = 1, 2) */
//	ADC_CH_MUXPOS_PIN13_gc = (0x0D<<3),			/* Input pin 13 (Input Mode = 1, 2) */
//	ADC_CH_MUXPOS_PIN14_gc = (0x0E<<3),			/* Input pin 14 (Input Mode = 1, 2) */
//	ADC_CH_MUXPOS_PIN15_gc = (0x0F<<3),			/* Input pin 15 (Input Mode = 1, 2) */

/* Negative input multiplexer selection (muxNeg argument)*/
//	ADC_CH_MUXNEG_PIN0_gc = (0x00<<0),			/* Input pin 0 (Input Mode = 2) */
//	ADC_CH_MUXNEG_PIN4_gc = (0x00<<0),			/* Input pin 4 (Input Mode = 3) */
//	ADC_CH_MUXNEG_PIN1_gc = (0x01<<0),			/* Input pin 1 (Input Mode = 2) */
//	ADC_CH_MUXNEG_PIN5_gc = (0x01<<0),			/* Input pin 5 (Input Mode = 3) */
//	ADC_CH_MUXNEG_PIN2_gc = (0x02<<0),			/* Input pin 2 (Input Mode = 2) */
//	ADC_CH_MUXNEG_PIN6_gc = (0x02<<0),			/* Input pin 6 (Input Mode = 3) */
//	ADC_CH_MUXNEG_PIN3_gc = (0x03<<0),			/* Input pin 3 (Input Mode = 2) */
//	ADC_CH_MUXNEG_PIN7_gc = (0x03<<0),			/* Input pin 7 (Input Mode = 3) */
//	ADC_CH_MUXNEG_INTGND_MODE4_gc = (0x04<<0),	/* Internal Ground (Input Mode = 3) */
//	ADC_CH_MUXNEG_GND_MODE3_gc = (0x05<<0),		/* PAD Ground (Input Mode = 2) */
//	ADC_CH_MUXNEG_INTGND_MODE3_gc = (0x07<<0),	/* Internal Ground (Input Mode = 2) */
//	ADC_CH_MUXNEG_GND_MODE4_gc = (0x07<<0),		/* PAD Ground (Input Mode = 3) */

/* Gain factor (gain argument)*/
//	ADC_CH_GAIN_1X_gc = (0x00<<2),					/* 1x gain */
//	ADC_CH_GAIN_2X_gc = (0x01<<2),					/* 2x gain */
//	ADC_CH_GAIN_4X_gc = (0x02<<2),					/* 4x gain */
//	ADC_CH_GAIN_8X_gc = (0x03<<2),					/* 8x gain */
//	ADC_CH_GAIN_16X_gc = (0x04<<2),					/* 16x gain */
//	ADC_CH_GAIN_32X_gc = (0x05<<2),					/* 32x gain */
//	ADC_CH_GAIN_64X_gc = (0x06<<2),					/* 64x gain */
//	ADC_CH_GAIN_DIV2_gc = (0x07<<2),				/* x/2 gain */

void ADC_CH_Cfg( ADC_t *pADC, uint8_t chNum, uint8_t inputMode, uint8_t muxPos, uint8_t muxNeg, uint8_t gain );
//-----------------------------------------------------------------------------

/* Interrupt mode */
//	ADC_CH_INTMODE_COMPLETE_gc = (0x00<<2),			/* Interrupt on conversion complete */
//	ADC_CH_INTMODE_BELOW_gc = (0x01<<2),			/* Interrupt on result below compare value */
//	ADC_CH_INTMODE_ABOVE_gc = (0x03<<2),			/* Interrupt on result above compare value */

/* Interrupt level */
//	ADC_CH_INTLVL_OFF_gc = (0x00<<0),				/* Interrupt disabled */
//	ADC_CH_INTLVL_LO_gc = (0x01<<0),				/* Low level */
//	ADC_CH_INTLVL_MED_gc = (0x02<<0),				/* Medium level */
//	ADC_CH_INTLVL_HI_gc = (0x03<<0),				/* High level */

static inline void ADC_CH_CfgInt( ADC_t *pADC, uint8_t chNum, uint8_t intMode, uint8_t intLevel );
static inline void ADC_CH_DisableInt( ADC_t *pADC, uint8_t chNum );

static inline uint8_t ADC_GetIntFlags( ADC_t *pADC );
static inline void ADC_ClrIntFlags( ADC_t *pADC, uint8_t chMask );
//-----------------------------------------------------------------------------

uint16_t ADC_Result( ADC_t *pADC, uint8_t chNum );
static inline uint16_t ADC_Result_Ch0( ADC_t *pADC );
static inline uint16_t ADC_Result_Ch1( ADC_t *pADC );
static inline uint16_t ADC_Result_Ch2( ADC_t *pADC );
static inline uint16_t ADC_Result_Ch3( ADC_t *pADC );

//=============================================================================

static inline void ADC_CfgClk( ADC_t *pADC, uint8_t clkPres )
{
	pADC->PRESCALER = clkPres;
}
//----------------------------------------------------------------------------

static inline void ADC_CfgVRef( ADC_t *pADC, uint8_t refSel )
{
	pADC->REFCTRL = (pADC->REFCTRL & ~ADC_REFSEL_gm) | refSel;
}
//----------------------------------------------------------------------------

static inline void ADC_Enable( ADC_t *pADC )
{
	pADC->CTRLA |= ADC_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_Disable( ADC_t *pADC )
{
	pADC->CTRLA &= ~ADC_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_Flush( ADC_t *pADC )
{
	pADC->CTRLA |= ADC_FLUSH_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_EnableBandgap( ADC_t *pADC )
{
	pADC->REFCTRL |= ADC_BANDGAP_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_DisableBandgap( ADC_t *pADC )
{
	pADC->REFCTRL &= ~ADC_BANDGAP_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_EnableTempref( ADC_t *pADC )
{
	pADC->REFCTRL |= ADC_TEMPREF_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_DisableTempref( ADC_t *pADC )
{
	pADC->REFCTRL &= ~ADC_TEMPREF_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_StartConv( ADC_t *pADC, uint8_t chMask )
{
	pADC->CTRLA |= (chMask << 2);
}
//----------------------------------------------------------------------------

static inline void ADC_WaitEndOfConv( ADC_t *pADC, uint8_t chMask )
{
	while( (pADC->INTFLAGS & chMask) != chMask );
}
//----------------------------------------------------------------------------

static inline void ADC_StartFreeRun( ADC_t *pADC, uint8_t chSweep )
{
	pADC->EVCTRL = chSweep;
	pADC->CTRLB |= ADC_FREERUN_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_StopFreeRun( ADC_t *pADC )
{
	pADC->CTRLB &= ~ADC_FREERUN_bm;
}
//----------------------------------------------------------------------------

static inline void ADC_CH_CfgInt( ADC_t *pADC, uint8_t chNum, uint8_t intMode, uint8_t intLevel )
{
	(&(pADC->CH0))[chNum].INTCTRL = intMode | intLevel;
}

//----------------------------------------------------------------------------

static inline void ADC_CH_DisableInt( ADC_t *pADC, uint8_t chNum )
{
	(&(pADC->CH0))[chNum].INTCTRL &= ~ADC_CH_INTLVL_gm;
}
//----------------------------------------------------------------------------

static inline uint8_t ADC_GetIntFlags( ADC_t *pADC )
{
	return pADC->INTFLAGS;
}
//----------------------------------------------------------------------------

static inline void ADC_ClrIntFlags( ADC_t *pADC, uint8_t chMask )
{
	pADC->INTFLAGS = chMask;
}
//----------------------------------------------------------------------------

static inline void ADC_SetCompareVal( ADC_t *pADC, uint16_t cmpVal )
{
	pADC->CMP = cmpVal;
}
//----------------------------------------------------------------------------

static inline void ADC_CfgEvents( ADC_t *pADC, uint8_t chSweep, uint8_t evSel, uint8_t evAct )
{
	pADC->EVCTRL = chSweep | evSel | evAct;
}
//----------------------------------------------------------------------------

static inline uint16_t ADC_Result_Ch0( ADC_t *pADC )
{
	return pADC->CH0RES;
}
//----------------------------------------------------------------------------

static inline uint16_t ADC_Result_Ch1( ADC_t *pADC )
{
	return pADC->CH1RES;
}
//----------------------------------------------------------------------------

static inline uint16_t ADC_Result_Ch2( ADC_t *pADC )
{
	return pADC->CH2RES;
}
//----------------------------------------------------------------------------

static inline uint16_t ADC_Result_Ch3( ADC_t *pADC )
{
	return pADC->CH3RES;
}
//----------------------------------------------------------------------------

#endif /* ADC_H_INCLUDED */