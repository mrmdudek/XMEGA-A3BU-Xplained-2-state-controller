//-----------------------------------------------------------------------------
//    KAmodCOMBO module library for ATmega and ATxmega microconrollers
//    
//    Modified by ZM based on KAmodCOMBO library for ATmega32
//    Ver. 1.0 - 2017.03
//    Ver. 2.0 - 2018.02
//-----------------------------------------------------------------------------
//
//    THE SOFTWARE INCLUDED IN THIS FILE IS FOR GUIDANCE ONLY.
//    BTC KORPORACJA SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
//    OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//    FROM USE OF THIS SOFTWARE.
//
//-----------------------------------------------------------------------------

#include <avr/io.h>

#ifndef KAMODCOMBO_H_INCLUDED
#define KAMODCOMBO_H_INCLUDED

//-----------------------------------------------------------------------------

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)

#define STLED316_DIO_DDR		DDRD
#define STLED316_CLK_DDR		DDRD
#define STLED316_STB_DDR		DDRD

#define STLED316_DIO_PIN		PIND  
#define STLED316_CLK_PIN		PIND
#define STLED316_STB_PIN		PIND

#define STLED316_DIO_PORT		PORTD 
#define STLED316_CLK_PORT		PORTD
#define STLED316_STB_PORT		PORTD

#define STLED316_DIO_PIN_BP		0  
#define STLED316_CLK_PIN_BP		1
#define STLED316_STB_PIN_BP		2

#define STLED316_DIO_PIN_BM		(1 << STLED316_DIO_PIN_BP)
#define STLED316_CLK_PIN_BM		(1 << STLED316_CLK_PIN_BP)
#define STLED316_STB_PIN_BM		(1 << STLED316_STB_PIN_BP)

//---------------------------------------------------------

#elif defined(__AVR_ATxmega256A3BU__)

#define STLED316_DIO_PORT		PORTE
#define STLED316_CLK_PORT		PORTE
#define STLED316_STB_PORT		PORTE
#define STLED316_IRQ_PORT		PORTE

#define STLED316_DIO_PIN_BP		0
#define STLED316_CLK_PIN_BP		1
#define STLED316_STB_PIN_BP		2
#define STLED316_IRQ_PIN_BP		3

#define STLED316_DIO_PIN_BM		(1 << STLED316_DIO_PIN_BP)
#define STLED316_CLK_PIN_BM		(1 << STLED316_CLK_PIN_BP)
#define STLED316_STB_PIN_BM		(1 << STLED316_STB_PIN_BP)
#define STLED316_IRQ_PIN_BM		(1 << STLED316_IRQ_PIN_BP)
//---------------------------------------------------------

#else
	#error AVR device not defined or not supported
#endif
//---------------------------------------------------------

// LED display segments
#define STLED316_TOP_SEG			0x1
#define STLED316_MID_SEG			0x40
#define STLED316_BOT_SEG			0x8
#define STLED316_LT_SEG				0x20	// Left-top
#define STLED316_LB_SEG				0x10	// Left-bottom
#define STLED316_RT_SEG				0x2  	// Right-top
#define STLED316_RB_SEG				0x4  	// Right-bottom
#define STLED316_DOT_SEG			0x80

#define STLED316_BRGHT_MODE_CONST 	0x18
#define STLED316_BRGHT_MODE_VAR 	0x0

//---------------------------------------------------------

void STLED316_Initialize( void );
int8_t STLED316_InitializeInt( uint8_t intNum );

void STLED316_DisplayOn( void );
void STLED316_DisplayOff( void );
void STLED316_SetBrghtCtrlModeAndDigitCnt( uint8_t BrghtCtrlMode, uint8_t ConstBrght, uint8_t DigitCount );

void STLED316_LEDs_Write( uint8_t LEDs );

void STLED316_7Seg_Write( uint8_t DisplayNum, uint8_t Segments );
void STLED316_7Seg_WriteTxt( uint8_t DisplayNum, const char *Txt );

void STLED316_LEDsAndButs_Read( uint8_t *LEDs, uint16_t *Buttons );
uint16_t STLED316_Buts_Read( void );
uint8_t STLED316_LEDs_Read( void );

void STLED316_LEDs_SetBrghtInd( uint8_t *BrightnessTab );
void STLED316_LEDs_SetBrght( uint8_t Brightness );
void STLED316_LEDs_GetBrghtInd( uint8_t *BrightnessTab );

void STLED316_7Seg_SetBrghtInd( uint8_t *BrightnessTab );
void STLED316_7Seg_SetBrght( uint8_t Brightness );
void STLED316_7Seg_GetBrghtInd( uint8_t *BrightnessTab );

//---------------------------------------------------------

// These functions are for internal use by driver

void STLED316_Write( uint8_t *Buffer, uint8_t BytesToWrite );
void STLED316_Read( uint8_t Command, uint8_t *Buffer, uint8_t BytesToRead );

//---------------------------------------------------------

#endif	/* KAMODCOMBO_H_INCLUDED */
