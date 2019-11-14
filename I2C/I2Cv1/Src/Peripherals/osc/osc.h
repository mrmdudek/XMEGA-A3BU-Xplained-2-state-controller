/*
 * osc.h
 *
 * Created: 2017-02-02 13:28:55
 *  Author: Zbyszek
 */ 


#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED

#include <avr/io.h>

/* System Clock Selection */
//	CLK_SCLKSEL_RC2M_gc = (0x00<<0),	/* Internal 2 MHz RC Oscillator */
//	CLK_SCLKSEL_RC32M_gc = (0x01<<0),	/* Internal 32 MHz RC Oscillator */
//	CLK_SCLKSEL_RC32K_gc = (0x02<<0),	/* Internal 32.768 kHz RC Oscillator */
//	CLK_SCLKSEL_XOSC_gc = (0x03<<0),	/* External Crystal Oscillator or Clock */
//	CLK_SCLKSEL_PLL_gc = (0x04<<0),		/* Phase Locked Loop */
void CLK_SysClkSel( uint8_t sclkSel );

/* Prescaler A Division Factor */
//	CLK_PSADIV_1_gc = (0x00<<2),  /* Divide by 1 */
//	CLK_PSADIV_2_gc = (0x01<<2),  /* Divide by 2 */
//	CLK_PSADIV_4_gc = (0x03<<2),  /* Divide by 4 */
//	CLK_PSADIV_8_gc = (0x05<<2),  /* Divide by 8 */
//	CLK_PSADIV_16_gc = (0x07<<2),  /* Divide by 16 */
//	CLK_PSADIV_32_gc = (0x09<<2),  /* Divide by 32 */
//	CLK_PSADIV_64_gc = (0x0B<<2),  /* Divide by 64 */
//	CLK_PSADIV_128_gc = (0x0D<<2),  /* Divide by 128 */
//	CLK_PSADIV_256_gc = (0x0F<<2),  /* Divide by 256 */
//	CLK_PSADIV_512_gc = (0x11<<2),  /* Divide by 512 */

/* Prescaler B and C Division Factor */
//	CLK_PSBCDIV_1_1_gc = (0x00<<0),  /* Divide B by 1 and C by 1 */
//	CLK_PSBCDIV_1_2_gc = (0x01<<0),  /* Divide B by 1 and C by 2 */
//	CLK_PSBCDIV_4_1_gc = (0x02<<0),  /* Divide B by 4 and C by 1 */
//	CLK_PSBCDIV_2_2_gc = (0x03<<0),  /* Divide B by 2 and C by 2 */
void CLK_PerClkCfg( uint8_t perADiv, uint8_t perBCDiv );

/* PLL Clock Source */
//	OSC_PLLSRC_RC2M_gc = (0x00<<6),  /* Internal 2 MHz RC Oscillator */
//	OSC_PLLSRC_RC32M_gc = (0x02<<6),  /* Internal 32 MHz RC Oscillator */
//	OSC_PLLSRC_XOSC_gc = (0x03<<6),  /* External Oscillator */
void OSC_PLLCfg( uint8_t pllScrSel, uint8_t pllMul, uint8_t pllDiv );

/* OSC_Enable(), OSC_Disable() bit masks  */
//	OSC_RC2MEN_bm = 0x01  /* Internal 2 MHz RC Oscillator Enable bit mask. */
//	OSC_RC32MEN_bm = 0x02  /* Internal 32 MHz RC Oscillator Enable bit mask. */
//	OSC_RC32KEN_bm = 0x04  /* Internal 32.768 kHz RC Oscillator Enable bit mask. */
//	OSC_XOSCEN_bm = 0x08  /* External Oscillator Enable bit mask. */
//	OSC_PLLEN_bm = 0x10  /* PLL Enable bit mask. */
static inline void OSC_Enable( uint8_t oscMask );
static inline void OSC_Disable( uint8_t oscMask );

/* OSC_Ready() bit masks  */
//	OSC_RC2MRDY_bm = 0x01  /* Internal 2 MHz RC Oscillator Enable bit mask. */
//	OSC_RC32MRDY_bm = 0x02  /* Internal 32 MHz RC Oscillator Enable bit mask. */
//	OSC_RC32KRDY_bm = 0x04  /* Internal 32.768 kHz RC Oscillator Enable bit mask. */
//	OSC_XOSCRDY_bm = 0x08  /* External Oscillator Enable bit mask. */
//	OSC_PLLRDY_bm = 0x10  /* PLL Enable bit mask. */
static inline uint8_t OSC_Ready( uint8_t oscMask );

/* Oscillator Frequency Range */
//	OSC_FRQRANGE_04TO2_gc = (0x00<<6),  /* 0.4 - 2 MHz */
//	OSC_FRQRANGE_2TO9_gc = (0x01<<6),  /* 2 - 9 MHz */
//	OSC_FRQRANGE_9TO12_gc = (0x02<<6),  /* 9 - 12 MHz */
//	OSC_FRQRANGE_12TO16_gc = (0x03<<6),  /* 12 - 16 MHz */

/* XTAL Startup Time */
//	OSC_XOSCSEL_XTAL_256CLK_gc = (0x03<<0),  /* 0.4-16 MHz XTAL - 256 CLK */
//	OSC_XOSCSEL_XTAL_1KCLK_gc = (0x07<<0),  /* 0.4-16 MHz XTAL - 1K CLK */
//	OSC_XOSCSEL_XTAL_16KCLK_gc = (0x0B<<0),  /* 0.4-16 MHz XTAL - 16K CLK */

static inline void OSC_XOsc_XTal_Sel( uint8_t frqRange, uint8_t startupTime );
static inline void OSC_XOsc_TOSC32K_Sel( uint8_t lowPowerMode );
static inline void OSC_XOsc_ExtClk_Sel( void );

void OSC_TOSC32K_Start( void );

/* 2 MHz DFLL Calibration Reference */
//	OSC_RC2MCREF_RC32K_gc = (0x00<<0),  /* Internal 32.768 kHz RC Oscillator */
//	OSC_RC2MCREF_XOSC32K_gc = (0x01<<0),  /* External 32.768 kHz Crystal Oscillator */
void DFLL_RC2M_RefClkSel( uint8_t refClkSel );

/* 32 MHz DFLL Calibration Reference */
//	OSC_RC32MCREF_RC32K_gc = (0x00<<1),  /* Internal 32.768 kHz RC Oscillator */
//	OSC_RC32MCREF_XOSC32K_gc = (0x01<<1),  /* External 32.768 kHz Crystal Oscillator */
//	OSC_RC32MCREF_USBSOF_gc = (0x02<<1),  /* USB Start of Frame */
void DFLL_RC32M_RefClkSel( uint8_t refClkSel );

void DFLL_Cfg( DFLL_t *pDfll, uint16_t ratio );
static inline void DFLL_Enable( DFLL_t *pDfll );
static inline void DFLL_Disable( DFLL_t *pDfll );
static inline uint8_t DFLL_Run( DFLL_t *pDfll );

//=============================================================================

static inline void OSC_Enable( uint8_t oscMask )
{
	OSC.CTRL |= oscMask;
}
//----------------------------------------------------------------------------

static inline void OSC_Disable( uint8_t oscMask )
{
	OSC.CTRL &= (~oscMask);
}
//----------------------------------------------------------------------------

static inline uint8_t OSC_Ready( uint8_t oscMask )
{
	return (OSC.STATUS & oscMask);
}
//----------------------------------------------------------------------------

static inline void OSC_XOsc_XTal_Sel( uint8_t frqRange, uint8_t startupTime )
{
	OSC.XOSCCTRL = frqRange | startupTime;
}
//----------------------------------------------------------------------------

static inline void OSC_XOsc_TOSC32K_Sel( uint8_t lowPowerMode )
{
	OSC.XOSCCTRL = OSC_XOSCSEL_32KHz_gc | ((lowPowerMode != 0) ? OSC_X32KLPM_bm : 0);
}
//----------------------------------------------------------------------------

static inline void OSC_XOsc_ExtClk_Sel( void )
{
	OSC.XOSCCTRL = OSC_XOSCSEL_EXTCLK_gc;
}

//----------------------------------------------------------------------------

static inline void DFLL_Enable( DFLL_t *pDfll )
{
	pDfll->CTRL = DFLL_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void DFLL_Disable( DFLL_t *pDfll )
{
	pDfll->CTRL = 0;
}
//----------------------------------------------------------------------------

static inline uint8_t DFLL_Run( DFLL_t *pDfll )
{
	return (pDfll->CTRL & DFLL_ENABLE_bm);
}
//----------------------------------------------------------------------------




#endif /* OSC_H_INCLUDED */