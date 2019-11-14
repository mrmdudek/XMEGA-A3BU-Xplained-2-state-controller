/*
 * lcd_hd44780.h
 *
 * Created: 2017-06-07 13:55:27
 *  Author: Zbyszek
 */ 


#ifndef LCD_HD44780_H_INCLUDED
#define LCD_HD44780_H_INCLUDED

#include <avr/io.h>

//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------

// #define HD44780_INTERFACE_SER_4
#define HD44780_INTERFACE_SER_8
// #define HD44780_INTERFACE_PAR_4
// #define HD44780_INTERFACE_PAR_8

#if (defined(HD44780_INTERFACE_SER_4) + defined(HD44780_INTERFACE_SER_8) + defined(HD44780_INTERFACE_PAR_4) + defined(HD44780_INTERFACE_PAR_8)) != 1
#error "There must be defined exactly one of HD44780_INTERFACE_SER_4, HD44780_INTERFACE_SER_8, HD44780_INTERFACE_PAR_4 or HD44780_INTERFACE_PAR_8."
#endif

#if (defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_PAR_4))
	#define HD44780_INTERFACE_NBITS		4
#else
	#define HD44780_INTERFACE_NBITS		8
#endif

//-----------------------------------------------------------------------------

#define HD44780_DATA_PORT_NAME	C
#define HD44780_E_PORT_NAME		C
#define HD44780_RS_PORT_NAME	C
#define HD44780_CLK_PORT_NAME	C

#define HD44780_DATA_PIN_BP		4				// When interface is parallel, data lines MUST be assigned to 4 (or 8) successive pins.
#define HD44780_E_PIN_BP		7
#define HD44780_RS_PIN_BP		6
#define HD44780_CLK_PIN_BP		5

#if (defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_SER_8))
	#define HD44780_DATA_PIN_BM		(1 << HD44780_DATA_PIN_BP)
#elif defined(HD44780_INTERFACE_PAR_4)
	#define HD44780_DATA_PIN_BM		((1 << (HD44780_DATA_PIN_BP + 3)) | (1 << (HD44780_DATA_PIN_BP + 2)) | \
									(1 << (HD44780_DATA_PIN_BP + 1)) | (1 << HD44780_DATA_PIN_BP))
#else
	#define HD44780_DATA_PIN_BM		(0xFF)
#endif	// (defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_SER_8))

#define HD44780_E_PIN_BM		(1 << HD44780_E_PIN_BP)
#define HD44780_RS_PIN_BM		(1 << HD44780_RS_PIN_BP)	// 0 - instruction register, 1 - data register
#define HD44780_CLK_PIN_BM		(1 << HD44780_CLK_PIN_BP)

//-----------------------------------------------------------------------------

#define __PORT(s)		__PORT1(s)
#define __PORT1(s)		PORT##s

#define HD44780_DATA_PORT		(__PORT(HD44780_DATA_PORT_NAME))
#define HD44780_E_PORT			(__PORT(HD44780_E_PORT_NAME))
#define HD44780_RS_PORT			(__PORT(HD44780_RS_PORT_NAME))
#define HD44780_CLK_PORT		(__PORT(HD44780_CLK_PORT_NAME))

//-----------------------------------------------------------------------------

#if !(defined(__AVR_ATmega8__) || \
	defined(__AVR_ATmega16__) || \
	defined(__AVR_ATmega32__) || \
	defined(__AVR_ATmega64__) || \
	defined(__AVR_ATmega128__) || \
	defined(__AVR_ATxmega256A3BU__))
#error AVR device not defined or not supported
#endif

//---------------------------------------------------------

#define HD44780_DISPLAY_CLR		0x01

#define HD44780_DISPLAY_ON		0x0C
#define HD44780_DISPLAY_OFF		0x08

#define HD44780_CURSOR_OFF		0x0C
#define HD44780_CURSOR_ON		0x0E
#define HD44780_CURSOR_BLINK	0x0F
#define HD44780_CURSOR_LEFT		0x10
#define HD44780_CURSOR_RIGHT	0x14
#define HD44780_CURSOR_HOME		0x02

#define HD44780_SHIFT_LEFT		0x18
#define HD44780_SHIFT_RIGHT		0x1C

#define HD44780_SET_DDRAM_ADDR	0x80

#define HD44780_LINE0_ADDR		0x00		// Line 0 DDRAM address
#define HD44780_LINE1_ADDR		0x40		// Line 1 DDRAM address

//---------------------------------------------------------

void lcd_hd44780_Init( unsigned char nOfLCDScrLines );
void hd44780_SendCmd( unsigned char cmd );

void lcd_hd44780_PutChar( unsigned char c );
void lcd_hd44780_PutText( const char *txt );

void lcd_hd44780_GotoXY( unsigned char x, unsigned char y );	// x=0..39, y=0..1

//---------------------------------------------------------

#define lcd_hd44780_DisplayOn()		hd44780_SendCmd( HD44780_DISPLAY_ON )
#define lcd_hd44780_DisplayOff()	hd44780_SendCmd( HD44780_DISPLAY_OFF )
#define lcd_hd44780_DisplayClr()	hd44780_SendCmd( HD44780_DISPLAY_CLR )

#define lcd_hd44780_CursorOff()		hd44780_SendCmd( HD44780_CURSOR_OFF )
#define lcd_hd44780_CursorOn()		hd44780_SendCmd( HD44780_CURSOR_ON )
#define lcd_hd44780_CursorBlink()	hd44780_SendCmd( HD44780_CURSOR_BLINK )
#define lcd_hd44780_CursorLeft()	hd44780_SendCmd( HD44780_CURSOR_LEFT )
#define lcd_hd44780_CursorRight()	hd44780_SendCmd( HD44780_CURSOR_RIGHT )
#define lcd_hd44780_CursorHome()	hd44780_SendCmd( HD44780_CURSOR_HOME )

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* LCD_HD44780_H_INCLUDED */