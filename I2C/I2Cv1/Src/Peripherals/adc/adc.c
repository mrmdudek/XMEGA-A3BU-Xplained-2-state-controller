/*
 * adc.c
 *
 * Created: 2017-04-18 22:47:33
 *  Author: Zbyszek
 */ 

//----------------------------------------------------------------------------

#include "adc/adc.h"

//----------------------------------------------------------------------------

void ADC_Cfg1( ADC_t *pADC, uint8_t convMode, uint8_t resolution )
{
	uint8_t		tmp;

/*	
	uzupe³niæ kalibracjê
*/	
	tmp = pADC->CTRLB & ~(ADC_CONMODE_bm | ADC_FREERUN_bm | ADC_RESOLUTION_gm);
	pADC->CTRLB = tmp | (convMode | resolution);
}
//----------------------------------------------------------------------------

void ADC_Cfg2( ADC_t *pADC, uint8_t impedanceMode, uint8_t currentLimit )
{
	pADC->CTRLB = (pADC->CTRLB & ~(ADC_IMPMODE_bm | ADC_CURRLIMIT_gm)) | (impedanceMode | currentLimit);
}
//----------------------------------------------------------------------------

void ADC_CH_Cfg( ADC_t *pADC, uint8_t chNum, uint8_t inputMode, uint8_t muxPos, uint8_t muxNeg, uint8_t gain )
{
	ADC_CH_t	*pADC_CH;
	
	pADC_CH = &(pADC->CH0);
	pADC_CH[chNum].CTRL = gain | inputMode;	
	pADC_CH[chNum].MUXCTRL = muxPos | muxNeg;
}
//----------------------------------------------------------------------------


uint16_t ADC_Result( ADC_t *pADC, uint8_t chNum )
{
	volatile uint16_t	*pADC_RES;
	
	pADC_RES = &(pADC->CH0RES);
	return pADC_RES[chNum];
}
//----------------------------------------------------------------------------
  
