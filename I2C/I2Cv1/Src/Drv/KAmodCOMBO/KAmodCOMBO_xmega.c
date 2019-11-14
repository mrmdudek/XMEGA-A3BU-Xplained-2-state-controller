//-----------------------------------------------------------------------------
//    KAmodCOMBO library for ATmega and ATxmega microcontrollers
//    AVR ATxmega specific function
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

#include "KAmodCOMBO/KAmodCOMBO.h"
#include <util/delay.h>

//=============================================================================

//=============================================================================

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
	#error This driver does not support AVR ATmega microcontrollers
#endif

//=============================================================================

// STLED316_Initialize
// Configures DIO, CLK and STB I/O lines.

void STLED316_Initialize( void )
{
// DIO - Totem pole with pull-up, no slew rate limit, no inversion, input sense - both edges
	PORTCFG.MPCMASK = STLED316_DIO_PIN_BM;
	STLED316_DIO_PORT.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	STLED316_DIO_PORT.DIRSET = STLED316_DIO_PIN_BM;

// CLK - Totem pole, no slew rate limit, no inversion, input sense - both edges
	PORTCFG.MPCMASK = STLED316_CLK_PIN_BM;
	STLED316_CLK_PORT.PIN0CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	STLED316_CLK_PORT.DIRSET = STLED316_CLK_PIN_BM;

// STB - Totem pole, no slew rate limit, no inversion, input sense - both edges
	PORTCFG.MPCMASK = STLED316_STB_PIN_BM;
	STLED316_STB_PORT.PIN0CTRL = PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc;
	STLED316_STB_PORT.DIRSET = STLED316_STB_PIN_BM;

// Set high level on output lines
	STLED316_DIO_PORT.OUTSET = STLED316_DIO_PIN_BM;
	STLED316_CLK_PORT.OUTSET = STLED316_CLK_PIN_BM;
	STLED316_STB_PORT.OUTSET = STLED316_STB_PIN_BM;
}

//=============================================================================

// STLED316_InitializeInt
// Configures INT I/O lines and interrupt
// - intNum - PORT interrupt number (valid values are 0 or 1)
// - return value - configured interrupt number or -1 if the interrupt number is invalid

int8_t STLED316_InitializeInt( uint8_t intNum )
{
	if( (intNum == 0) || (intNum == 1) ) 
	{
// Configure IRQ pin - input, no inversion, input sense - falling edge
		STLED316_IRQ_PORT.DIRCLR = STLED316_IRQ_PIN_BM;
		PORTCFG.MPCMASK = STLED316_IRQ_PIN_BM;
		STLED316_IRQ_PORT.PIN0CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_FALLING_gc;

		switch( intNum )
		{
		case 0:						// Port interrupt 0 configuration
			STLED316_IRQ_PORT.INTCTRL = PORT_INT0LVL_LO_gc;
			STLED316_IRQ_PORT.INT0MASK |= STLED316_IRQ_PIN_BM;
			PMIC.CTRL |= PMIC_LOLVLEN_bm;
			return 0;
		case 1:						// Port interrupt 1 configuration
			STLED316_IRQ_PORT.INTCTRL = PORT_INT1LVL_LO_gc;
			STLED316_IRQ_PORT.INT1MASK |= STLED316_IRQ_PIN_BM;
			PMIC.CTRL |= PMIC_LOLVLEN_bm;
			return 1;
		}
	}
	return -1;
}

//=============================================================================

void STLED316_Write( uint8_t *Buffer, uint8_t BytesToWrite )
{
	uint8_t		BitNum, ByteNum;
	uint8_t		BitMask;

	STLED316_DIO_PORT.DIRSET = STLED316_DIO_PIN_BM;
	STLED316_STB_PORT.OUTCLR = STLED316_STB_PIN_BM;
	_delay_us( 1 );

	for( ByteNum = 0; ByteNum < BytesToWrite; ByteNum++ )
	{
		BitMask = 0x01;
		for( BitNum = 0; BitNum < 8; BitNum++ )
		{
			STLED316_CLK_PORT.OUTCLR = STLED316_CLK_PIN_BM;
			if( Buffer[ByteNum] & BitMask )
				STLED316_DIO_PORT.OUTSET = STLED316_DIO_PIN_BM;
			else
				STLED316_DIO_PORT.OUTCLR = STLED316_DIO_PIN_BM;
			_delay_us( 0.5 );

			STLED316_CLK_PORT.OUTSET = STLED316_CLK_PIN_BM;
			_delay_us( 0.5 );

			BitMask <<= 1;
		}
	}
	_delay_us( 1 );

	STLED316_STB_PORT.OUTSET = STLED316_STB_PIN_BM;
}

//=============================================================================

void STLED316_Read( uint8_t Command, uint8_t *Buffer, uint8_t BytesToRead )
{
	uint8_t		BitNum, ByteNum;
	uint8_t		BitMask;

//Send command
	STLED316_DIO_PORT.DIRSET = STLED316_DIO_PIN_BM;
	STLED316_STB_PORT.OUTCLR = STLED316_STB_PIN_BM;
	_delay_us( 1 );

	BitMask = 0x01;
	for( BitNum = 0; BitNum < 8; BitNum++ )
	{
		STLED316_CLK_PORT.OUTCLR = STLED316_CLK_PIN_BM;
		if( Command & BitMask )
			STLED316_DIO_PORT.OUTSET = STLED316_DIO_PIN_BM;
		else
			STLED316_DIO_PORT.OUTCLR = STLED316_DIO_PIN_BM;
		_delay_us( 0.5 );

		STLED316_CLK_PORT.OUTSET = STLED316_CLK_PIN_BM;
		_delay_us( 0.5 );

		BitMask <<= 1;
	}
	_delay_us( 1 );

//Read data
	STLED316_DIO_PORT.DIRCLR = STLED316_DIO_PIN_BM;

	for( ByteNum = 0; ByteNum < BytesToRead; ByteNum++ )
	{
		Buffer[ByteNum] = 0;
		BitMask = 0x01;
		for( BitNum = 0; BitNum < 8; BitNum++ )
		{
			STLED316_CLK_PORT.OUTCLR = STLED316_CLK_PIN_BM;
			_delay_us( 0.5 );

			STLED316_CLK_PORT.OUTSET = STLED316_CLK_PIN_BM;
			_delay_us( 0.5 );

			if ( STLED316_DIO_PORT.IN & STLED316_DIO_PIN_BM )
				Buffer[ByteNum] |= BitMask;
			BitMask <<= 1;
		}
	}

	STLED316_STB_PORT.OUTSET = STLED316_STB_PIN_BM;
}

//=============================================================================



