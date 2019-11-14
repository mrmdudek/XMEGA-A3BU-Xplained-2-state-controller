/*
 * evsys.c
 *
 * Created: 2017-07-17 22:47:33
 *  Author: Zbyszek
 */ 

//----------------------------------------------------------------------------

#include "evsys/evsys.h"

//----------------------------------------------------------------------------

void EVSYS_ChDigFilter( uint8_t chNum, uint8_t digFilt )
{
	volatile uint8_t	*pReg;

	pReg = &EVSYS.CH0CTRL + chNum;
	*pReg = (*pReg & ~EVSYS_DIGFILT_gm) | digFilt;
}
//----------------------------------------------------------------------------

void EVSYS_QD_Enable( uint8_t chNum, uint8_t indexEnable, uint8_t indexMode )
{
	volatile uint8_t	*pReg;

	if( indexEnable != 0 )
		indexEnable = EVSYS_QDIEN_bm;
	pReg = &EVSYS.CH0CTRL + chNum;
	*pReg = (*pReg & ~(EVSYS_QDIEN_bm | EVSYS_QDIRM_gm)) | (EVSYS_QDEN_bm | indexEnable | indexMode);
}

//----------------------------------------------------------------------------

void EVSYS_QD_Disable( uint8_t chNum )
{
	volatile uint8_t	*pReg;

	pReg = &EVSYS.CH0CTRL + chNum;
	*pReg = (*pReg & ~(EVSYS_QDEN_bm | EVSYS_QDIEN_bm | EVSYS_QDIRM_gm));
}

//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
  
