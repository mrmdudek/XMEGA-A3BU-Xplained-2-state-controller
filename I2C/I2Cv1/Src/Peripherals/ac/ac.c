/*
 * ac.c
 *
 * Created: 2017-07-17 22:47:33
 *  Author: Zbyszek
 */ 

//----------------------------------------------------------------------------

#include "ac/ac.h"

//----------------------------------------------------------------------------
/*
void AC_Cfg( AC_t *pAC, uint8_t acNum, uint8_t hysMode, uint8_t muxPos, uint8_t muxNeg )
{
	
	if( acNum == 0 )
	{
		pAC->AC0CTRL = hysMode;
		pAC->AC0MUXCTRL = muxPos | muxNeg;
	}
	else
	{
		pAC->AC1CTRL = hysMode;
		pAC->AC1MUXCTRL = muxPos | muxNeg;
	}
}
//-----------------------------------------------------------------------------

void AC_OutputPinEnable( AC_t *pAC, uint8_t acNum )
{
	if( acNum == 0 )
		pAC->CTRLA |= AC_AC0OUT_bm;
	else
		pAC->CTRLA |= AC_AC1OUT_bm;
}

//-----------------------------------------------------------------------------

void AC_OutputPinDisable( AC_t *pAC, uint8_t acNum )
{
	if( acNum == 0 )
		pAC->CTRLA &= ~AC_AC0OUT_bm;
	else
		pAC->CTRLA &= ~AC_AC1OUT_bm;
}

//----------------------------------------------------------------------------
  
void AC_HighSpeedEnable( AC_t *pAC, uint8_t acNum )
{
	if( acNum == 0 )
		pAC->AC0CTRL |= AC_HSMODE_bm;
	else
		pAC->AC1CTRL |= AC_HSMODE_bm;
}
//----------------------------------------------------------------------------

void AC_HighSpeedDisable( AC_t *pAC, uint8_t acNum )
{
	if( acNum == 0 )
	pAC->AC0CTRL &= ~AC_HSMODE_bm;
	else
	pAC->AC1CTRL &= ~AC_HSMODE_bm;
}
//----------------------------------------------------------------------------

*/

