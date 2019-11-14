/*
 * lcd_hd44780_xmega.c
 *
 * Created: 2017-06-07 13:55:07
 *  Author: Zbyszek
 */ 


#if !defined(__AVR_ATxmega256A3BU__)
#error "File __FILE__ is not intended for selected MCU type."
#endif

//=============================================================================

#include "LCD_hd44780/lcd_hd44780.h"

#include <util/delay.h>
#include "gpio/gpio.h"

//=============================================================================

/*
 * Generates about 100 us pulse on pin E
 */
static inline void hd44780_EPulse( void )							
{
	GPIO_SetPinsHigh( &HD44780_E_PORT, HD44780_E_PIN_BM );			// set E to high
	_delay_us( 100 );
	GPIO_SetPinsLow( &HD44780_E_PORT, HD44780_E_PIN_BM );			// set E to low
}

//-----------------------------------------------------------------------------

#if (defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_SER_8))

void hd44780_SetData( unsigned char data )				// 4- and 8-bit serial interface
{
	unsigned char i;

	GPIO_SetPinsLow( &HD44780_CLK_PORT, HD44780_CLK_PIN_BM );

	for( i = 0; i < HD44780_INTERFACE_NBITS; i++ )
	{
		if( (data & 0x01) != 0 )
			GPIO_SetPinsHigh( &HD44780_DATA_PORT, HD44780_DATA_PIN_BM );
		else
			GPIO_SetPinsLow( &HD44780_DATA_PORT, HD44780_DATA_PIN_BM );

		_delay_us( 1 );
		GPIO_SetPinsHigh( &HD44780_CLK_PORT, HD44780_CLK_PIN_BM );
		_delay_us( 1 );
		GPIO_SetPinsLow( &HD44780_CLK_PORT, HD44780_CLK_PIN_BM );

		data >>= 1;
	}
}

#elif defined(HD44780_INTERFACE_PAR_4)

void hd44780_SetData( unsigned char data )			// 4-bit parallel interface
{
	data = (data << HD44780_DATA_PIN_BP) & HD44780_DATA_PIN_BM;
	GPIO_WriteOut( &HD44780_DATA_PORT, (GPIO_ReadOut( &HD44780_DATA_PORT ) & ~HD44780_DATA_PIN_BM) | data );
}

#else

void hd44780_SetData( unsigned char data )			// 8-bit parallel interface
{
	GPIO_WriteOut( &HD44780_DATA_PORT, data );
}

#endif	// (defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_SER_8))

//--------------------------------------------------------------

#if (defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_PAR_4))

void hd44780_SendCmd( unsigned char cmd )			// 4-bit interface
{
// Write D7-D4 command bits
	hd44780_SetData( cmd >> 4 );
	hd44780_EPulse( );

// Write D3-D0 command bits
	hd44780_SetData( cmd );
	hd44780_EPulse( );

	if( cmd == HD44780_DISPLAY_CLR )
		_delay_ms( 2 );
}

#else

void hd44780_SendCmd( unsigned char cmd )			// 8-bit interface
{
// Write D7-D0 command bits
	hd44780_SetData( cmd );
	hd44780_EPulse( );

	if( cmd == HD44780_DISPLAY_CLR )
	_delay_ms( 2 );
}

#endif	// (defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_PAR_4))

//--------------------------------------------------------------

#if	(defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_PAR_4))

void lcd_hd44780_PutChar( unsigned char c )			// 4-bit interface
{
	GPIO_SetPinsHigh( &HD44780_RS_PORT, HD44780_RS_PIN_BM );

// Write D7-D4 data bits
	hd44780_SetData( c >> 4 );
	hd44780_EPulse( );

// Write D3-D0 data bits
	hd44780_SetData( c );
	hd44780_EPulse( );

	GPIO_SetPinsLow( &HD44780_RS_PORT, HD44780_RS_PIN_BM );
}

#else

void lcd_hd44780_PutChar( unsigned char c )			// 8-bit interface
{
	GPIO_SetPinsHigh( &HD44780_RS_PORT, HD44780_RS_PIN_BM );

// Write D7-D0 data bits
	hd44780_SetData( c );
	hd44780_EPulse( );

	GPIO_SetPinsLow( &HD44780_RS_PORT, HD44780_RS_PIN_BM );
}

#endif	// (defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_PAR_4))

//--------------------------------------------------------------

void lcd_hd44780_Init( unsigned char nOfLCDScrLines )
{
	uint8_t		cmdFunctionSet;

// LCD initialization with software reset
// step by step from DATASHEET

	cmdFunctionSet = (nOfLCDScrLines < 2) ? 0x00 : 0x08;

// set all LCD pins as output and set them low
	GPIO_CfgPins( &HD44780_DATA_PORT, HD44780_DATA_PIN_BM, (PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc), 1, 0, 0 );
	GPIO_CfgPins( &HD44780_E_PORT, HD44780_E_PIN_BM, (PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc), 1, 0, 0 );
	GPIO_CfgPins( &HD44780_RS_PORT, HD44780_RS_PIN_BM, (PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc), 1, 0, 0 );

#if	(defined(HD44780_INTERFACE_SER_4) || defined(HD44780_INTERFACE_SER_8))
	GPIO_CfgPins( &HD44780_CLK_PORT, HD44780_CLK_PIN_BM, (PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc), 1, 0, 0 );
#endif

	hd44780_SetData( 0 );

	_delay_ms( 30 );

#if (HD44780_INTERFACE_NBITS == 4)
											// 4-bit initialization
// software reset; write 3x D4=1, D5=1		
	hd44780_SetData( 0x03 );
	hd44780_EPulse( );
	_delay_ms( 5 );

	hd44780_EPulse( );
	_delay_ms( 1 );

	hd44780_EPulse( );
	_delay_ms( 1 );

// switch to 4-bit data bus; write D4=0, D5=1
	hd44780_SetData( 0x02 );
	hd44780_EPulse( );
	_delay_ms( 1 );

// Function Set instruction command
	cmdFunctionSet |= 0x20;

#else
										// 8-bit initialization
// software reset; write 3x D4=1, D5=1
	hd44780_SetData( 0x30 );
	hd44780_EPulse( );
	_delay_ms( 5 );

	hd44780_EPulse( );
	_delay_ms( 1 );

	hd44780_EPulse( );
	_delay_ms( 1 );

// Function Set instruction command
	cmdFunctionSet |= 0x30;

#endif	// (HD44780_INTERFACE_NBITS == 4)

	hd44780_SendCmd( cmdFunctionSet );
	_delay_ms( 1 );

	lcd_hd44780_DisplayOff( );
	lcd_hd44780_DisplayClr( );
	lcd_hd44780_DisplayOn( );
}

//--------------------------------------------------------------

void lcd_hd44780_GotoXY( unsigned char x, unsigned char y )
{
	unsigned char cmd;

	cmd = (HD44780_LINE1_ADDR * y) + x;			// convert (x, y) into DDRAM address
	cmd = HD44780_SET_DDRAM_ADDR | (cmd & 0x7F);
	hd44780_SendCmd( cmd );
}

//---------------------------------------------------------

void lcd_hd44780_PutText( const char *text )
{
	while( *text != '\0' )
	{
		lcd_hd44780_PutChar( *text );
		text++;
	}
}

//--------------------------------------------------------------


