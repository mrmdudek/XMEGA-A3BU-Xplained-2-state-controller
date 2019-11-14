/*
 * tc.h
 *
 * Created: 2017-10-31 22:48:08
 *  Author: Zbyszek
 */ 


#ifndef TC_H_INCLUDED
#define TC_H_INCLUDED

#include <avr/io.h>

//-----------------------------------------------------------------------------

#define TC_INTLVL_OFF_gc	TC_CCAINTLVL_OFF_gc			/* Disable interrupt */
#define TC_INTLVL_LO_gc		TC_CCAINTLVL_LO_gc			/* Enable low level interrupt */
#define TC_INTLVL_MED_gc	TC_CCAINTLVL_MED_gc			/* Enable medium level interrupt */
#define TC_INTLVL_HI_gc		TC_CCAINTLVL_HI_gc			/* Enable high level interrupt */

/* Compare/Capture Enable */
#define TC_CCAEN_bm			TC0_CCAEN_bm		/* Compare or Capture A Enable bit mask. */
#define TC_CCBEN_bm			TC0_CCBEN_bm		/* Compare or Capture B Enable bit mask. */
#define TC_CCCEN_bm			TC0_CCCEN_bm		/* Compare or Capture C Enable bit mask. */
#define TC_CCDEN_bm			TC0_CCDEN_bm		/* Compare or Capture D Enable bit mask. */

/* Force Compare Output */
#define TC_CMPA_bm			TC0_CMPA_bm			/* Compare A Output Value bit mask. */
#define TC_CMPB_bm			TC0_CMPB_bm			/* Compare B Output Value bit mask. */
#define TC_CMPC_bm			TC0_CMPC_bm			/* Compare C Output Value bit mask. */
#define TC_CMPD_bm			TC0_CMPD_bm			/* Compare D Output Value bit mask. */

/* Interrupt flags bit masks */
#define TC_OVFIF_bm			TC0_OVFIF_bm		/* Overflow Interrupt Flag bit mask. */
#define TC_ERRIF_bm			TC0_ERRIF_bm		/* Error Interrupt Flag bit mask. */
#define TC_CCAIF_bm			TC0_CCAIF_bm		/* Compare or Capture A Interrupt Flag bit mask. */
#define TC_CCBIF_bm			TC0_CCBIF_bm		/* Compare or Capture B Interrupt Flag bit mask. */
#define TC_CCCIF_bm			TC0_CCCIF_bm		/* Compare or Capture C Interrupt Flag bit mask. */
#define TC_CCDIF_bm			TC0_CCDIF_bm		/* Compare or Capture D Interrupt Flag bit mask. */

/* Buffer Valid bit masks */
#define TC_PERBV_bm			TC0_PERBV_bm		/* Period Buffer Valid bit mask. */
#define TC_CCABV_bm			TC0_CCABV_bm		/* Compare or Capture A Buffer Valid bit mask. */
#define TC_CCBBV_bm			TC0_CCBBV_bm		/* Compare or Capture B Buffer Valid bit mask. */
#define TC_CCCBV_bm			TC0_CCCBV_bm		/* Compare or Capture C Buffer Valid bit mask. */
#define TC_CCDBV_bm			TC0_CCDBV_bm		/* Compare or Capture D Buffer Valid bit mask. */

//=============================================================================

/* Clock Selection  (clkSel argument) */
//	TC_CLKSEL_OFF_gc = (0x00<<0),			/* Timer Off */
//	TC_CLKSEL_DIV1_gc = (0x01<<0),			/* System Clock */
//	TC_CLKSEL_DIV2_gc = (0x02<<0),			/* System Clock / 2 */
//	TC_CLKSEL_DIV4_gc = (0x03<<0),			/* System Clock / 4 */
//	TC_CLKSEL_DIV8_gc = (0x04<<0),			/* System Clock / 8 */
//	TC_CLKSEL_DIV64_gc = (0x05<<0),			/* System Clock / 64 */
//	TC_CLKSEL_DIV256_gc = (0x06<<0),		/* System Clock / 256 */
//	TC_CLKSEL_DIV1024_gc = (0x07<<0),		/* System Clock / 1024 */
//	TC_CLKSEL_EVCH0_gc = (0x08<<0),			/* Event Channel 0 */
//	TC_CLKSEL_EVCH1_gc = (0x09<<0),			/* Event Channel 1 */
//	TC_CLKSEL_EVCH2_gc = (0x0A<<0),			/* Event Channel 2 */
//	TC_CLKSEL_EVCH3_gc = (0x0B<<0),			/* Event Channel 3 */
//	TC_CLKSEL_EVCH4_gc = (0x0C<<0),			/* Event Channel 4 */
//	TC_CLKSEL_EVCH5_gc = (0x0D<<0),			/* Event Channel 5 */
//	TC_CLKSEL_EVCH6_gc = (0x0E<<0),			/* Event Channel 6 */
//	TC_CLKSEL_EVCH7_gc = (0x0F<<0),			/* Event Channel 7 */

static inline void TC_EnableClk( void *pTC, uint8_t clkSel );
static inline void TC_DisableClk( void *pTC );
//-----------------------------------------------------------------------------

static inline void TC_WGMode_Normal( void *pTC );
static inline void TC_WGMode_Frq( void *pTC );
static inline void TC_WGMode_PWM_SS( void *pTC );
static inline void TC_WGMode_PWM_DS_T( void *pTC );
static inline void TC_WGMode_PWM_DS_TB( void *pTC );
static inline void TC_WGMode_PWM_DS_B( void *pTC );
//-----------------------------------------------------------------------------

/* Byte Mode (byteMode argument) */
//	TC_BYTEM_NORMAL_gc = (0x00<<0),		/* 16-bit mode */
//	TC_BYTEM_BYTEMODE_gc = (0x01<<0),	/* Timer/Counter operating in byte mode only */
//	TC_BYTEM_SPLITMODE_gc = (0x02<<0),	/* Timer/Counter split into two 8-bit Counters (TC2) */

static inline void TC_ByteMode( void *pTC, uint8_t byteMode );
//-----------------------------------------------------------------------------

/* Compare/Capture Enable/ Disable (ccMask argument) */
//	#define TC_CCAEN_bm					/* Compare or Capture A Enable bit mask. */
//	#define TC_CCBEN_bm					/* Compare or Capture B Enable bit mask. */
//	#define TC_CCCEN_bm					/* Compare or Capture C Enable bit mask. */
//	#define TC_CCDEN_bm					/* Compare or Capture D Enable bit mask. */

static inline void TC_EnableCC( void *pTC, uint8_t ccMask );
static inline void TC_DisableCC( void *pTC, uint8_t ccMask );
//-----------------------------------------------------------------------------

/* Force Compare Output (fcoMask argument) */
//	#define TC_CMPA_bm					/* Compare A Output Value bit mask. */
//	#define TC_CMPB_bm					/* Compare B Output Value bit mask. */
//	#define TC_CMPC_bm					/* Compare C Output Value bit mask. */
//	#define TC_CMPD_bm					/* Compare D Output Value bit mask. */

static inline void TC_ForceCmpOutput( void *pTC, uint8_t fcoMask );
//-----------------------------------------------------------------------------

/* Event Selection (evSel argument) */
//	TC_EVSEL_OFF_gc = (0x00<<0),		/* No Event Source */
//	TC_EVSEL_CH0_gc = (0x08<<0),		/* Event Channel 0 */
//	TC_EVSEL_CH1_gc = (0x09<<0),		/* Event Channel 1 */
//	TC_EVSEL_CH2_gc = (0x0A<<0),		/* Event Channel 2 */
//	TC_EVSEL_CH3_gc = (0x0B<<0),		/* Event Channel 3 */
//	TC_EVSEL_CH4_gc = (0x0C<<0),		/* Event Channel 4 */
//	TC_EVSEL_CH5_gc = (0x0D<<0),		/* Event Channel 5 */
//	TC_EVSEL_CH6_gc = (0x0E<<0),		/* Event Channel 6 */
//	TC_EVSEL_CH7_gc = (0x0F<<0),		/* Event Channel 7 */

/* Event Action (evAct argument) */
//	TC_EVACT_OFF_gc = (0x00<<5),		/* No Event Action */
//	TC_EVACT_CAPT_gc = (0x01<<5),		/* Input Capture */
//	TC_EVACT_UPDOWN_gc = (0x02<<5),		/* Externally Controlled Up/Down Count */
//	TC_EVACT_QDEC_gc = (0x03<<5),		/* Quadrature Decode */
//	TC_EVACT_RESTART_gc = (0x04<<5),	/* Restart */
//	TC_EVACT_FRQ_gc = (0x05<<5),		/* Frequency Capture */
//	TC_EVACT_PW_gc = (0x06<<5),			/* Pulse-width Capture */

static inline void TC_CfgEvents( void *pTC, uint8_t evSel, uint8_t evAct, uint8_t evDly );
//-----------------------------------------------------------------------------

/* Interrupt Level (intLevel argument) */
//	TC_INTLVL_OFF						/* Disable interrupt */
//	TC_INTLVL_LO						/* Enable low level interrupt */
//	TC_INTLVL_MED						/* Enable medium level interrupt */
//	TC_INTLVL_HI						/* Enable high level interrupt */

static inline void TC_CfgCCInt( void *pTC, uint8_t CCAIntLevel, uint8_t CCBIntLevel, uint8_t CCCIntLevel, uint8_t CCDIntLevel );
static inline void TC_DisableCCInt( void *pTC );

static inline void TC_CfgCCAInt( void *pTC, uint8_t intLevel );
static inline void TC_DisableCCAInt( void *pTC );
static inline void TC_CfgCCBInt( void *pTC, uint8_t intLevel );
static inline void TC_DisableCCBInt( void *pTC );
static inline void TC_CfgCCCInt( void *pTC, uint8_t intLevel );
static inline void TC_DisableCCCInt( void *pTC );
static inline void TC_CfgCCDInt( void *pTC, uint8_t intLevel );
static inline void TC_DisableCCDInt( void *pTC );

static inline void TC_CfgOvfInt( void *pTC, uint8_t intLevel );
static inline void TC_DisableOvfInt( void *pTC );

static inline void TC_CfgErrInt( void *pTC, uint8_t intLevel );
static inline void TC_DisableErrInt( void *pTC );
//-----------------------------------------------------------------------------

/* Interrupt flags bit masks */
//	#define TC_OVFIF_bm					/* Overflow Interrupt Flag bit mask. */
//	#define TC_ERRIF_bm					/* Error Interrupt Flag bit mask. */
//	#define TC_CCAIF_bm					/* Compare or Capture A Interrupt Flag bit mask. */
//	#define TC_CCBIF_bm					/* Compare or Capture B Interrupt Flag bit mask. */
//	#define TC_CCCIF_bm					/* Compare or Capture C Interrupt Flag bit mask. */
//	#define TC_CCDIF_bm					/* Compare or Capture D Interrupt Flag bit mask. */

static inline uint8_t TC_GetIntFlags( void *pTC );
static inline void TC_ClrIntFlags( void *pTC, uint8_t intMask );
//-----------------------------------------------------------------------------

static inline void TC_CntDirUp( void *pTC );
static inline void TC_CntDirDown( void *pTC );
static inline uint8_t TC_GetCntDir( void *pTC );
//-----------------------------------------------------------------------------

/* Timer/Counter Command (cmd argument) */
//	TC_CMD_NONE_gc = (0x00<<2),			/* No Command */
//	TC_CMD_UPDATE_gc = (0x01<<2),		/* Force Update */
//	TC_CMD_RESTART_gc = (0x02<<2),		/* Force Restart */
//	TC_CMD_RESET_gc = (0x03<<2),		/* Force Hard Reset */

static inline void TC_Cmd( void *pTC, uint8_t cmd );
//-----------------------------------------------------------------------------

/* Buffer Valid bit masks */
//	#define TC_PERBV_bm					/* Period Buffer Valid bit mask. */
//	#define TC_CCABV_bm					/* Compare or Capture A Buffer Valid bit mask. */
//	#define TC_CCBBV_bm					/* Compare or Capture B Buffer Valid bit mask. */
//	#define TC_CCCBV_bm					/* Compare or Capture C Buffer Valid bit mask. */
//	#define TC_CCDBV_bm					/* Compare or Capture D Buffer Valid bit mask. */

static inline uint8_t TC_GetBVFlags( void *pTC );
static inline void TC_SetBVFlags( void *pTC, uint8_t flagMask );
static inline void TC_ClrBVFlags( void *pTC, uint8_t flagMask );
//-----------------------------------------------------------------------------

static inline void TC_ClrCNT( void *pTC );
static inline uint16_t TC_ReadCNT( void *pTC );
static inline void TC_WriteCNT( void *pTC, uint16_t value );
//-----------------------------------------------------------------------------

static inline void TC_InitPER( void *pTC, uint16_t period );
static inline void TC_UpdPER( void *pTC, uint16_t period );

static inline void TC_InitCCA( void *pTC, uint16_t value );
static inline void TC_UpdCCA( void *pTC, uint16_t value );
static inline void TC_InitCCB( void *pTC, uint16_t value );
static inline void TC_UpdCCB( void *pTC, uint16_t value );
static inline void TC_InitCCC( void *pTC, uint16_t value );
static inline void TC_UpdCCC( void *pTC, uint16_t value );
static inline void TC_InitCCD( void *pTC, uint16_t value );
static inline void TC_UpdCCD( void *pTC, uint16_t value );

static inline void TC_LockUpdate( void *pTC );
static inline void TC_UnlockUpdate( void *pTC );
//-----------------------------------------------------------------------------

static inline uint16_t TC_ReadCCA( void *pTC );
static inline uint16_t TC_ReadCCB( void *pTC );
static inline uint16_t TC_ReadCCC( void *pTC );
static inline uint16_t TC_ReadCCD( void *pTC );

//=============================================================================

static inline void TC_EnableClk( void *pTC, uint8_t clkSel )
{
	((TC0_t*)pTC)->CTRLA = clkSel;
}
//-----------------------------------------------------------------------------
static inline void TC_DisableClk( void *pTC )
{
	((TC0_t*)pTC)->CTRLA = TC_CLKSEL_OFF_gc;
}
//-----------------------------------------------------------------------------

static inline void TC_WGMode_Normal( void *pTC )
{
	((TC0_t*)pTC)->CTRLB = TC_WGMODE_NORMAL_gc;
}
//-----------------------------------------------------------------------------

static inline void TC_WGMode_Frq( void *pTC )
{
	((TC0_t*)pTC)->CTRLB = TC_WGMODE_FRQ_gc;
}
//-----------------------------------------------------------------------------

static inline void TC_WGMode_PWM_SS( void *pTC )
{
	((TC0_t*)pTC)->CTRLB = TC_WGMODE_SINGLESLOPE_gc;
}
//-----------------------------------------------------------------------------

static inline void TC_WGMode_PWM_DS_T( void *pTC )
{
	((TC0_t*)pTC)->CTRLB = TC_WGMODE_DSTOP_gc;
}
//-----------------------------------------------------------------------------

static inline void TC_WGMode_PWM_DS_TB( void *pTC )
{
	((TC0_t*)pTC)->CTRLB = TC_WGMODE_DSBOTH_gc;
}
//-----------------------------------------------------------------------------

static inline void TC_WGMode_PWM_DS_B( void *pTC )
{
	((TC0_t*)pTC)->CTRLB = TC_WGMODE_DSBOTTOM_gc;
}
//-----------------------------------------------------------------------------

static inline void TC_ByteMode( void *pTC, uint8_t byteMode )
{
	((TC0_t*)pTC)->CTRLE = byteMode;
}
//-----------------------------------------------------------------------------

static inline void TC_EnableCC( void *pTC, uint8_t ccMask )
{
	((TC0_t*)pTC)->CTRLB |= ccMask;
}
//-----------------------------------------------------------------------------

static inline void TC_DisableCC( void *pTC, uint8_t ccMask )
{
	((TC0_t*)pTC)->CTRLB &= ~ccMask;
}
//-----------------------------------------------------------------------------

static inline void TC_ForceCmpOutput( void *pTC, uint8_t fcoMask )
{
	((TC0_t*)pTC)->CTRLC |= fcoMask;
}
//-----------------------------------------------------------------------------

static inline void TC_CfgEvents( void *pTC, uint8_t evSel, uint8_t evAct, uint8_t evDly )
{
	((TC0_t*)pTC)->CTRLD = evSel | evAct | ((evDly != 0) ? TC0_EVDLY_bm : 0);
}
//-----------------------------------------------------------------------------

static inline void TC_CfgCCInt( void *pTC, uint8_t CCAIntLevel, uint8_t CCBIntLevel, uint8_t CCCIntLevel, uint8_t CCDIntLevel )
{
	((TC0_t*)pTC)->INTCTRLB = CCAIntLevel | (CCBIntLevel << 2) | (CCCIntLevel << 4) | (CCDIntLevel << 6);
}
//-----------------------------------------------------------------------------

static inline void TC_DisableCCInt( void *pTC )
{
	((TC0_t*)pTC)->INTCTRLB = TC_INTLVL_OFF_gc;
}
//-----------------------------------------------------------------------------

static inline void TC_CfgCCAInt( void *pTC, uint8_t intLevel )
{
	((TC0_t*)pTC)->INTCTRLB = (((TC0_t*)pTC)->INTCTRLB & ~TC0_CCAINTLVL_gm) | intLevel;
}
//-----------------------------------------------------------------------------

static inline void TC_DisableCCAInt( void *pTC )
{
	((TC0_t*)pTC)->INTCTRLB &= (~TC0_CCAINTLVL_gm);
}
//-----------------------------------------------------------------------------

static inline void TC_CfgCCBInt( void *pTC, uint8_t intLevel )
{
	((TC0_t*)pTC)->INTCTRLB = (((TC0_t*)pTC)->INTCTRLB & ~TC0_CCBINTLVL_gm) | (intLevel << 2);
}
//-----------------------------------------------------------------------------

static inline void TC_DisableCCBInt( void *pTC )
{
	((TC0_t*)pTC)->INTCTRLB &= (~TC0_CCBINTLVL_gm);
}
//-----------------------------------------------------------------------------

static inline void TC_CfgCCCInt( void *pTC, uint8_t intLevel )
{
	((TC0_t*)pTC)->INTCTRLB = (((TC0_t*)pTC)->INTCTRLB & ~TC0_CCCINTLVL_gm) | (intLevel << 4);
}
//-----------------------------------------------------------------------------

static inline void TC_DisableCCCInt( void *pTC )
{
	((TC0_t*)pTC)->INTCTRLB &= (~TC0_CCCINTLVL_gm);
}
//-----------------------------------------------------------------------------

static inline void TC_CfgCCDInt( void *pTC, uint8_t intLevel )
{
	((TC0_t*)pTC)->INTCTRLB = (((TC0_t*)pTC)->INTCTRLB & ~TC0_CCDINTLVL_gm) | (intLevel << 6);
}
//-----------------------------------------------------------------------------

static inline void TC_DisableCCDInt( void *pTC )
{
	((TC0_t*)pTC)->INTCTRLB &= (~TC0_CCDINTLVL_gm);
}
//-----------------------------------------------------------------------------

static inline void TC_CfgOvfInt( void *pTC, uint8_t intLevel )
{
	((TC0_t*)pTC)->INTCTRLA = (((TC0_t*)pTC)->INTCTRLA & ~TC0_OVFINTLVL_gm) | intLevel;
}
//-----------------------------------------------------------------------------

static inline void TC_DisableOvfInt( void *pTC )
{
	((TC0_t*)pTC)->INTCTRLA &= (~TC0_OVFINTLVL_gm);
}
//-----------------------------------------------------------------------------

static inline void TC_CfgErrInt( void *pTC, uint8_t intLevel )
{
	((TC0_t*)pTC)->INTCTRLA = (((TC0_t*)pTC)->INTCTRLA & ~TC0_ERRINTLVL_gm) | (intLevel << 2);
}
//-----------------------------------------------------------------------------

static inline void TC_DisableErrInt( void *pTC )
{
	((TC0_t*)pTC)->INTCTRLA &= (~TC0_ERRINTLVL_gm);
}
//-----------------------------------------------------------------------------

static inline uint8_t TC_GetIntFlags( void *pTC )
{
	return ((TC0_t*)pTC)->INTFLAGS;
}
//-----------------------------------------------------------------------------

static inline void TC_ClrIntFlags( void *pTC, uint8_t intMask )
{
	((TC0_t*)pTC)->INTFLAGS = intMask; 
}
//-----------------------------------------------------------------------------

static inline void TC_CntDirUp( void *pTC )
{
	((TC0_t*)pTC)->CTRLFCLR = TC0_DIR_bm;
}
//-----------------------------------------------------------------------------

static inline void TC_CntDirDown( void *pTC )
{
	((TC0_t*)pTC)->CTRLFSET = TC0_DIR_bm;
}
//-----------------------------------------------------------------------------

static inline uint8_t TC_GetCntDir( void *pTC )
{
	return (((TC0_t*)pTC)->CTRLFCLR & TC0_DIR_bm);
}
//-----------------------------------------------------------------------------

static inline void TC_Cmd( void *pTC, uint8_t cmd )
{
	((TC0_t*)pTC)->CTRLFSET = cmd;
}
//-----------------------------------------------------------------------------

static inline uint8_t TC_GetBVFlags( void *pTC )
{
	return ((TC0_t*)pTC)->CTRLGSET;
}
//-----------------------------------------------------------------------------

static inline void TC_SetBVFlags( void *pTC, uint8_t flagMask )
{
	((TC0_t*)pTC)->CTRLGSET = flagMask;
}
//-----------------------------------------------------------------------------

static inline void TC_ClrBVFlags( void *pTC, uint8_t flagMask )
{
	((TC0_t*)pTC)->CTRLGCLR = flagMask;
}
//-----------------------------------------------------------------------------

static inline void TC_ClrCNT( void *pTC )
{
	((TC0_t*)pTC)->CNT = 0;
}
//-----------------------------------------------------------------------------

static inline uint16_t TC_ReadCNT( void *pTC )
{
	return ((TC0_t*)pTC)->CNT;
}
//-----------------------------------------------------------------------------

static inline void TC_WriteCNT( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CNT = value;
}
//-----------------------------------------------------------------------------

static inline void TC_InitPER( void *pTC, uint16_t period )
{
	((TC0_t*)pTC)->PERBUF = period;
	((TC0_t*)pTC)->PER = period;
}
//-----------------------------------------------------------------------------

static inline void TC_UpdPER( void *pTC, uint16_t period )
{
	((TC0_t*)pTC)->PERBUF = period;
}
//-----------------------------------------------------------------------------

static inline void TC_InitCCA( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CCABUF = value;
	((TC0_t*)pTC)->CCA = value;
}
//-----------------------------------------------------------------------------

static inline void TC_UpdCCA( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CCABUF = value;
}
//-----------------------------------------------------------------------------

static inline void TC_InitCCB( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CCBBUF = value;
	((TC0_t*)pTC)->CCB = value;
}
//-----------------------------------------------------------------------------

static inline void TC_UpdCCB( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CCBBUF = value;
}
//-----------------------------------------------------------------------------

static inline void TC_InitCCC( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CCCBUF = value;
	((TC0_t*)pTC)->CCC = value;
}
//-----------------------------------------------------------------------------

static inline void TC_UpdCCC( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CCCBUF = value;
}
//-----------------------------------------------------------------------------

static inline void TC_InitCCD( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CCDBUF = value;
	((TC0_t*)pTC)->CCD = value;
}
//-----------------------------------------------------------------------------

static inline void TC_UpdCCD( void *pTC, uint16_t value )
{
	((TC0_t*)pTC)->CCDBUF = value;
}
//-----------------------------------------------------------------------------

static inline void TC_LockUpdate( void *pTC )
{
	((TC0_t*)pTC)->CTRLFSET = TC0_LUPD_bm;
}
//-----------------------------------------------------------------------------

static inline void TC_UnlockUpdate( void *pTC )
{
	((TC0_t*)pTC)->CTRLFCLR = TC0_LUPD_bm;
}
//-----------------------------------------------------------------------------

static inline uint16_t TC_ReadCCA( void *pTC )
{
	return ((TC0_t*)pTC)->CCA;
}
//-----------------------------------------------------------------------------

static inline uint16_t TC_ReadCCB( void *pTC )
{
	return ((TC0_t*)pTC)->CCB;
}
//-----------------------------------------------------------------------------

static inline uint16_t TC_ReadCCC( void *pTC )
{
	return ((TC0_t*)pTC)->CCC;
}
//-----------------------------------------------------------------------------

static inline uint16_t TC_ReadCCD( void *pTC )
{
	return ((TC0_t*)pTC)->CCD;
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------


#endif /* TC_H_INCLUDED */