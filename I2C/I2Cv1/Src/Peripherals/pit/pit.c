/*
 * pit.c
 *
 * Created: 2017-03-06 09:53:55
 *  Author: Zbyszek
 */ 


#include <avr/interrupt.h>
#include "pit/pit.h"
 
//----------------------------------------------------------------------------

volatile uint8_t		pit_ClkSel;
volatile uint8_t		pit_IntCnt, pit_IntCntMax;
volatile uint8_t		pit_Tick;

//----------------------------------------------------------------------------

void PIT_Cfg( uint8_t clkSel, uint16_t intPrd, uint8_t tickPrdMul )
{
	pit_ClkSel = clkSel;
	pit_IntCntMax = tickPrdMul;
	pit_IntCnt = 0;

	TCF0.CTRLA = TC_CLKSEL_OFF_gc;
	TCF0.CTRLB = TC_WGMODE_NORMAL_gc;
	TCF0.CTRLD = 0;
	TCF0.CTRLE = TC_BYTEM_NORMAL_gc;
	TCF0.CTRLFCLR = TC0_DIR_bm;

	TCF0.CNT = 0;
	TCF0.PER = intPrd;
	TCF0.PERBUF = intPrd;

	TCF0.INTCTRLA = TC_OVFINTLVL_LO_gc;
	TCF0.INTCTRLB = 0;

	PMIC.CTRL |= PMIC_LOLVLEN_bm;
}
//----------------------------------------------------------------------------

void PIT_SetPar( uint16_t intPrd, uint8_t tickPrdMul )
{
	pit_IntCntMax = tickPrdMul;
	TCF0.PERBUF = intPrd;
	if( (TCF0.CTRLA & TC0_CLKSEL_gm) == TC_CLKSEL_OFF_gc )
		TCF0.PER = intPrd;
}
//----------------------------------------------------------------------------

void PIT_Start( void )
{
	TCF0.CTRLA = pit_ClkSel;
}
//----------------------------------------------------------------------------

void PIT_Stop( void )
{
	TCF0.CTRLA = TC_CLKSEL_OFF_gc;
	pit_IntCnt = 0;
}
//----------------------------------------------------------------------------

void PIT_Wait( void )
{
	while( pit_Tick == 0 );
//	pit_Tick = 0;
	pit_Tick--;
}
//----------------------------------------------------------------------------

uint8_t PIT_CheckTick( void )
{
	volatile uint8_t tmp;
	
	cli( );	
	tmp = pit_Tick;
	pit_Tick = 0;
	sei( );

	return tmp;
}
//----------------------------------------------------------------------------

ISR( TCF0_OVF_vect )
{
	if( ++pit_IntCnt >= pit_IntCntMax )
	{
		pit_IntCnt = 0;
		pit_Tick++;
	}
}
//----------------------------------------------------------------------------

