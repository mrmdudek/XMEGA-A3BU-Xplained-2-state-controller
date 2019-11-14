/*
 * OSC.c
 *
 * Created: 2017-02-02 13:27:12
 *  Author: Zbyszek
 */ 

//----------------------------------------------------------------------------

#include <util/delay.h>

#include "osc/osc.h"

//----------------------------------------------------------------------------

void CLK_SysClkSel( uint8_t sclkSel )
{
	uint8_t		selTmp;

	selTmp = sclkSel & 0x07;
	CCP = CCP_IOREG_gc;				// = 0xD8
	CLK.CTRL = selTmp;
}
//----------------------------------------------------------------------------

void CLK_PerClkCfg( uint8_t perADiv, uint8_t perBCDiv )
{
	uint8_t		divTmp;

	divTmp = (perADiv & 0x7C) | (perBCDiv & 0x03);
	CCP = CCP_IOREG_gc;				// = 0xD8
	CLK.PSCTRL = divTmp;
}
//----------------------------------------------------------------------------

void OSC_PLLCfg( uint8_t pllScrSel, uint8_t pllMul, uint8_t pllDiv )
{
	pllDiv = (pllDiv < 2) ? 0 : 0x20;
	OSC.PLLCTRL = (pllScrSel & 0xC0) | pllDiv | (pllMul & 0x1F);
}
//----------------------------------------------------------------------------

/*
void OSC_Enable( uint8_t oscMask )
{
	OSC.CTRL |= oscMask;
}
//----------------------------------------------------------------------------

void OSC_Disable( uint8_t oscMask )
{
	OSC.CTRL &= (~oscMask);
}
//----------------------------------------------------------------------------

uint8_t OSC_Ready( uint8_t oscMask )
{
	return (OSC.STATUS & oscMask);
}
//----------------------------------------------------------------------------

void OSC_XOsc_XTal_Sel( uint8_t frqRange, uint8_t startupTime )
{
	OSC.XOSCCTRL = frqRange | startupTime;
}
//----------------------------------------------------------------------------

void OSC_XOsc_TOSC32K_Sel( uint8_t lowPowerMode )
{
	OSC.XOSCCTRL = OSC_XOSCSEL_32KHz_gc | ((lowPowerMode != 0) ? OSC_X32KLPM_bm : 0);
}
//----------------------------------------------------------------------------

void OSC_XOsc_ExtClk_Sel( void )
{
	OSC.XOSCCTRL = OSC_XOSCSEL_EXTCLK_gc;
}

//----------------------------------------------------------------------------
*/

void DFLL_RC2M_RefClkSel( uint8_t refClkSel )
{
	OSC.DFLLCTRL = (OSC.DFLLCTRL & ~OSC_RC2MCREF_bm) | refClkSel;
}
//----------------------------------------------------------------------------

void DFLL_RC32M_RefClkSel( uint8_t refClkSel )
{
	OSC.DFLLCTRL = (OSC.DFLLCTRL & ~OSC_RC32MCREF_gm) | refClkSel;
}
//----------------------------------------------------------------------------

void OSC_TOSC32K_Start( void )
{
	VBAT.CTRL |= VBAT_ACCEN_bm;

	CCP = CCP_IOREG_gc;
	VBAT.CTRL = VBAT_RESET_bm;
	
	VBAT.CTRL |= VBAT_XOSCFDEN_bm;
	_delay_us( 200 );

	VBAT.CTRL |= VBAT_XOSCEN_bm;
	while( (VBAT.STATUS & VBAT_XOSCRDY_bm) == 0 );
}
//----------------------------------------------------------------------------

void DFLL_Cfg( DFLL_t *pDfll, uint16_t ratio )
{
	pDfll->COMP1 = (uint8_t)(ratio & 0x00FF);
	pDfll->COMP2 = (uint8_t)(ratio >> 8);
}
//----------------------------------------------------------------------------

/*
void DFLL_Enable( DFLL_t *pDfll )
{
	pDfll->CTRL = DFLL_ENABLE_bm;
}
//----------------------------------------------------------------------------

void DFLL_Disable( DFLL_t *pDfll )
{
	pDfll->CTRL = 0;
}
//----------------------------------------------------------------------------

uint8_t DFLL_Run( DFLL_t *pDfll )
{
	return (pDfll->CTRL & DFLL_ENABLE_bm);
}
//----------------------------------------------------------------------------
*/

