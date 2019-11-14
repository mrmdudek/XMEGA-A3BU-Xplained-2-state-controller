/*
 * pmic.h
 *
 * Created: 2017-10-12 20:25:18
 *  Author: Zbyszek
 */ 


#ifndef PMIC_H_INCLUDED
#define PMIC_H_INCLUDED

#include <avr/io.h>

//-----------------------------------------------------------------------------

static inline void PMIC_LoLvlIntEnable( void );
static inline void PMIC_LoLvlIntDisable( void );
static inline void PMIC_MedLvlIntEnable( void );
static inline void PMIC_MedLvlIntDisable( void );
static inline void PMIC_HiLvlIntEnable( void );
static inline void PMIC_HiLvlIntDisable( void );

//-----------------------------------------------------------------------------

static inline void PMIC_RRSchedulingEnable( void );
static inline void PMIC_RRSchedulingDisable( void );

static inline uint8_t PMIC_GetStatus( void );

//=============================================================================

static inline void PMIC_LoLvlIntEnable( void )
{
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
}
//----------------------------------------------------------------------------

static inline void PMIC_LoLvlIntDisable( void )
{
	PMIC.CTRL &= ~PMIC_LOLVLEN_bm;
}
//----------------------------------------------------------------------------

static inline void PMIC_MedLvlIntEnable( void )
{
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
}
//----------------------------------------------------------------------------

static inline void PMIC_MedLvlIntDisable( void )
{
	PMIC.CTRL &= ~PMIC_MEDLVLEN_bm;
}
//----------------------------------------------------------------------------

static inline void PMIC_HiLvlIntEnable( void )
{
	PMIC.CTRL |= PMIC_HILVLEN_bm;
}
//----------------------------------------------------------------------------

static inline void PMIC_HiLvlIntDisable( void )
{
	PMIC.CTRL &= ~PMIC_HILVLEN_bm;
}
//----------------------------------------------------------------------------

static inline void PMIC_RRSchedulingEnable( void )
{
	PMIC.CTRL |= PMIC_RREN_bm;
}
//----------------------------------------------------------------------------

static inline void PMIC_RRSchedulingDisable( void )
{
	PMIC.CTRL &= ~PMIC_RREN_bm;
	PMIC.INTPRI = 0;
}
//----------------------------------------------------------------------------

static inline uint8_t PMIC_GetStatus( void )
{
	return PMIC.STATUS;
}
//----------------------------------------------------------------------------

#endif /* PMIC_H_INCLUDED */