/*
 * pit.h
 *
 * Created: 2017-03-06 09:54:32
 *  Author: Zbyszek
 */ 


#ifndef PIT_H_INCLUDED
#define PIT_H_INCLUDED

#include <avr/io.h>

void PIT_Cfg( uint8_t clkSel, uint16_t intPrd, uint8_t tickPrdMul );
void PIT_SetPar( uint16_t intPrd, uint8_t tickPrdMul );

void PIT_Start( void );
void PIT_Stop( void );
void PIT_Wait( void );
uint8_t PIT_CheckTick( void );

#endif /* PIT_H_INCLUDED */