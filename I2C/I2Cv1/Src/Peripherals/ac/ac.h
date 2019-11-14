/*
 * ac.h
 *
 * Created: 2017-07-17 22:48:08
 *  Author: Zbyszek
 */ 


#ifndef AC_H_INCLUDED
#define AC_H_INCLUDED

#include <avr/io.h>

//-----------------------------------------------------------------------------

/* Hysteresis mode selection */
//	AC_HYSMODE_NO_gc = (0x00<<1),				/* No hysteresis */
//	AC_HYSMODE_SMALL_gc = (0x01<<1),			/* Small hysteresis */
//	AC_HYSMODE_LARGE_gc = (0x02<<1),			/* Large hysteresis */

/* High Speed */
//	zero										/* Normal (low) speed */
//	non-zero									/* High speed */

/* Positive input multiplexer selection */
//	AC_MUXPOS_PIN0_gc = (0x00<<3),				/* Pin 0 */
//	AC_MUXPOS_PIN1_gc = (0x01<<3),				/* Pin 1 */
//	AC_MUXPOS_PIN2_gc = (0x02<<3),				/* Pin 2 */
//	AC_MUXPOS_PIN3_gc = (0x03<<3),				/* Pin 3 */
//	AC_MUXPOS_PIN4_gc = (0x04<<3),				/* Pin 4 */
//	AC_MUXPOS_PIN5_gc = (0x05<<3),				/* Pin 5 */
//	AC_MUXPOS_PIN6_gc = (0x06<<3),				/* Pin 6 */
//	AC_MUXPOS_DAC_gc = (0x07<<3),				/* DAC output */

/* Negative input multiplexer selection */
//	AC_MUXNEG_PIN0_gc = (0x00<<0),				/* Pin 0 */
//	AC_MUXNEG_PIN1_gc = (0x01<<0),				/* Pin 1 */
//	AC_MUXNEG_PIN3_gc = (0x02<<0),				/* Pin 3 */
//	AC_MUXNEG_PIN5_gc = (0x03<<0),				/* Pin 5 */
//	AC_MUXNEG_PIN7_gc = (0x04<<0),				/* Pin 7 */
//	AC_MUXNEG_DAC_gc = (0x05<<0),				/* DAC output */
//	AC_MUXNEG_BANDGAP_gc = (0x06<<0),			/* Bandgap Reference */
//	AC_MUXNEG_SCALER_gc = (0x07<<0),			/* Internal voltage scaler */

// void AC_Cfg( AC_t *pAC, uint8_t acNum, uint8_t hysMode, uint8_t muxPos, uint8_t muxNeg );
static inline void AC0_Cfg( AC_t *pAC, uint8_t hysMode, uint8_t highSpeed, uint8_t muxPos, uint8_t muxNeg );
static inline void AC1_Cfg( AC_t *pAC, uint8_t hysMode, uint8_t highSpeed, uint8_t muxPos, uint8_t muxNeg );
//-----------------------------------------------------------------------------

static inline void AC0_EnableHighSpeed( AC_t *pAC );
static inline void AC0_DisableHighSpeed( AC_t *pAC);
static inline void AC1_EnableHighSpeed( AC_t *pAC );
static inline void AC1_DisableHighSpeed( AC_t *pAC );
//-----------------------------------------------------------------------------

/* Hysteresis mode selection */
//	AC_HYSMODE_NO_gc = (0x00<<1),				/* No hysteresis */
//	AC_HYSMODE_SMALL_gc = (0x01<<1),			/* Small hysteresis */
//	AC_HYSMODE_LARGE_gc = (0x02<<1),			/* Large hysteresis */

static inline void AC0_SetHysMode( AC_t *pAC, uint8_t hysMode );
static inline void AC1_SetHysMode( AC_t *pAC, uint8_t hysMode );
//-----------------------------------------------------------------------------

static inline void AC0_EnableOutPin( AC_t *pAC );
static inline void AC0_DisableOutPin( AC_t *pAC );
static inline void AC1_EnableOutPin( AC_t *pAC );
static inline void AC1_DisableOutPin( AC_t *pAC );
//-----------------------------------------------------------------------------

static inline void AC0_Enable(AC_t *pAC );
static inline void AC1_Enable(AC_t *pAC );
static inline void AC0_Disable(AC_t *pAC );
static inline void AC1_Disable(AC_t *pAC );
//-----------------------------------------------------------------------------

/* Interrupt mode */
//	AC_INTMODE_BOTHEDGES_gc = (0x00<<6),		/* Interrupt on both edges */
//	AC_INTMODE_FALLING_gc = (0x02<<6),			/* Interrupt on falling edge */
//	AC_INTMODE_RISING_gc = (0x03<<6),			/* Interrupt on rising edge */

/* Interrupt level */
//	AC_INTLVL_OFF_gc = (0x00<<4),				/* Interrupt disabled */
//	AC_INTLVL_LO_gc = (0x01<<4),				/* Low level */
//	AC_INTLVL_MED_gc = (0x02<<4),				/* Medium level */
//	AC_INTLVL_HI_gc = (0x03<<4),				/* High level */

static inline void AC0_CfgInt( AC_t *pAC, uint8_t intMode, uint8_t intLevel );
static inline void AC1_CfgInt( AC_t *pAC, uint8_t intMode, uint8_t intLevel );
static inline void AC0_DisableInt( AC_t *pAC );
static inline void AC1_DisableInt( AC_t *pAC );
//-----------------------------------------------------------------------------

static inline void AC_SetVCCScaleFactor(AC_t *pAC, uint8_t scaleFactor );
//----------------------------------------------------------------------------

static inline void AC_Win_Enable( AC_t *pAC );
static inline void AC_Win_Disable( AC_t *pAC );

/* Windows interrupt mode */
//	AC_WINTMODE_ABOVE_gc = (0x00<<2),			/* Interrupt on above window */
//	AC_WINTMODE_INSIDE_gc = (0x01<<2),			/* Interrupt on inside window */
//	AC_WINTMODE_BELOW_gc = (0x02<<2),			/* Interrupt on below window */
//	AC_WINTMODE_OUTSIDE_gc = (0x03<<2),			/* Interrupt on outside window */

/* Interrupt level */
//	AC_INTLVL_OFF_gc = (0x00<<4),				/* Interrupt disabled */
//	AC_INTLVL_LO_gc = (0x01<<4),				/* Low level */
//	AC_INTLVL_MED_gc = (0x02<<4),				/* Medium level */
//	AC_INTLVL_HI_gc = (0x03<<4),				/* High level */

static inline void AC_Win_CfgInt( AC_t *pAC, uint8_t intMode, uint8_t intLevel );
static inline void AC_Win_DisableInt( AC_t *pAC );

static inline uint8_t AC_GetStatus( AC_t *pAC );
static inline uint8_t AC0_GetState( AC_t *pAC );
static inline uint8_t AC1_GetState( AC_t *pAC );
static inline uint8_t AC_Win_GetState( AC_t *pAC );

static inline uint8_t AC_GetIntFlags( AC_t *pAC );
static inline void AC_ClrIntFlags( AC_t *pAC, uint8_t intMask );

//=============================================================================

static inline void AC0_Cfg( AC_t *pAC, uint8_t hysMode, uint8_t highSpeed, uint8_t muxPos, uint8_t muxNeg )
{
	pAC->AC0CTRL = hysMode | ((highSpeed == 0) ? 0 : AC_HSMODE_bm);
	pAC->AC0MUXCTRL = muxPos | muxNeg;
}
//----------------------------------------------------------------------------

static inline void AC1_Cfg( AC_t *pAC, uint8_t hysMode, uint8_t highSpeed, uint8_t muxPos, uint8_t muxNeg )
{
	pAC->AC1CTRL = hysMode | ((highSpeed == 0) ? 0 : AC_HSMODE_bm);
	pAC->AC1MUXCTRL = muxPos | muxNeg;
}
//----------------------------------------------------------------------------

static inline void AC0_EnableHighSpeed( AC_t *pAC )
{
	pAC->AC0CTRL |= AC_HSMODE_bm;
}
//----------------------------------------------------------------------------

static inline void AC0_DisableHighSpeed( AC_t *pAC)
{
	pAC->AC0CTRL &= ~AC_HSMODE_bm;
}
//----------------------------------------------------------------------------

static inline void AC1_EnableHighSpeed( AC_t *pAC )
{
	pAC->AC1CTRL |= AC_HSMODE_bm;
}
//----------------------------------------------------------------------------

static inline void AC1_DisableHighSpeed( AC_t *pAC )
{
	pAC->AC1CTRL &= ~AC_HSMODE_bm;
}
//----------------------------------------------------------------------------

static inline void AC0_SetHysMode( AC_t *pAC, uint8_t hysMode )
{
	pAC->AC0CTRL = (pAC->AC0CTRL & ~AC_HYSMODE_gm) | hysMode;
}
//----------------------------------------------------------------------------

static inline void AC1_SetHysMode( AC_t *pAC, uint8_t hysMode )
{
	pAC->AC1CTRL = (pAC->AC1CTRL & ~AC_HYSMODE_gm) | hysMode;
}
//----------------------------------------------------------------------------

static inline void AC0_EnableOutPin( AC_t *pAC )
{
	pAC->CTRLA |= AC_AC0OUT_bm;
}
//----------------------------------------------------------------------------

static inline void AC0_DisableOutPin( AC_t *pAC )
{
	pAC->CTRLA &= ~AC_AC0OUT_bm;
}
//----------------------------------------------------------------------------

static inline void AC1_EnableOutPin( AC_t *pAC )
{
	pAC->CTRLA |= AC_AC1OUT_bm;
}
//----------------------------------------------------------------------------

static inline void AC1_DisableOutPin( AC_t *pAC )
{
	pAC->CTRLA &= ~AC_AC1OUT_bm;
}
//----------------------------------------------------------------------------

static inline void AC0_Enable(AC_t *pAC )
{
	pAC->AC0CTRL |= AC_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void AC1_Enable(AC_t *pAC )
{
	pAC->AC1CTRL |= AC_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void AC0_Disable(AC_t *pAC )
{
	pAC->AC0CTRL &= ~AC_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void AC1_Disable(AC_t *pAC )
{
	pAC->AC1CTRL &= ~AC_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void AC_SetVCCScaleFactor(AC_t *pAC, uint8_t scaleFactor )
{
	pAC->CTRLB = scaleFactor;
}
//----------------------------------------------------------------------------

static inline void AC0_CfgInt( AC_t *pAC, uint8_t intMode, uint8_t intLevel )
{
	pAC->AC0CTRL = (pAC->AC0CTRL & ~(AC_INTMODE_gm | AC_INTLVL_gm)) | (intMode | intLevel);
}
//----------------------------------------------------------------------------

static inline void AC1_CfgInt( AC_t *pAC, uint8_t intMode, uint8_t intLevel )
{
	pAC->AC1CTRL = (pAC->AC1CTRL & ~(AC_INTMODE_gm | AC_INTLVL_gm)) | (intMode | intLevel);
}
//----------------------------------------------------------------------------

static inline void AC0_DisableInt( AC_t *pAC )
{
	pAC->AC0CTRL &= ~AC_INTLVL_gm;
}
//----------------------------------------------------------------------------

static inline void AC1_DisableInt( AC_t *pAC )
{
	pAC->AC1CTRL &= ~AC_INTLVL_gm;
}
//----------------------------------------------------------------------------

static inline void AC_Win_Enable( AC_t *pAC )
{
	pAC->WINCTRL |= AC_WEN_bm;
}
//----------------------------------------------------------------------------

static inline void AC_Win_Disable( AC_t *pAC )
{
	pAC->WINCTRL &= ~AC_WEN_bm;
}
//----------------------------------------------------------------------------

static inline void AC_Win_CfgInt( AC_t *pAC, uint8_t intMode, uint8_t intLevel )
{
	pAC->WINCTRL = (pAC->WINCTRL & ~(AC_WINTMODE_gm | AC_WINTLVL_gm)) | ( intMode | (intLevel >> 4));
}
//----------------------------------------------------------------------------

static inline void AC_Win_DisableInt( AC_t *pAC )
{
	pAC->WINCTRL &= ~AC_WINTLVL_gm;
}
//----------------------------------------------------------------------------

static inline uint8_t AC_GetStatus( AC_t *pAC )
{
	return (pAC->STATUS & (AC_AC0STATE_bm | AC_AC1STATE_bm | AC_WSTATE_gm));
}
//----------------------------------------------------------------------------

static inline uint8_t AC0_GetState( AC_t *pAC )
{
	return (pAC->STATUS & AC_AC0STATE_bm);
}
//----------------------------------------------------------------------------

static inline uint8_t AC1_GetState( AC_t *pAC )
{
	return (pAC->STATUS & AC_AC1STATE_bm);
}
//----------------------------------------------------------------------------

static inline uint8_t AC_Win_GetState( AC_t *pAC )
{
	return (pAC->STATUS & AC_WSTATE_gm);
}
//----------------------------------------------------------------------------

static inline uint8_t AC_GetIntFlags( AC_t *pAC )
{
	return (pAC->STATUS & (AC_AC0IF_bm | AC_AC1IF_bm | AC_WIF_bm));
}
//----------------------------------------------------------------------------

static inline void AC_ClrIntFlags( AC_t *pAC, uint8_t intMask )
{
	pAC->STATUS |= intMask;
}
//----------------------------------------------------------------------------


#endif /* AC_H_INCLUDED */