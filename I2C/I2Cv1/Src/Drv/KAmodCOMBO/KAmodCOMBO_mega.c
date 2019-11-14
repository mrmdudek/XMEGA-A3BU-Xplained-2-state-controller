//-----------------------------------------------------------------------------
//    KAmodCOMBO library for ATmega and ATxmega microcontrollers
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

#if !(defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) || defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__))
	#error This driver does not support AVR ATxmega microcontrollers
#endif


//=============================================================================

// STLED316_Initialize
// Configures DIO, CLK and STB I/O lines.

void STLED316_Initialize( void )
{
// Configure input/output lines
	STLED316_CLK_DDR |= STLED316_CLK_PIN_BM;
	STLED316_STB_DDR |= STLED316_STB_PIN_BM;
	STLED316_DIO_DDR |= STLED316_DIO_PIN_BM;

// Set high level on comm. lines
	STLED316_CLK_PORT |= STLED316_CLK_PIN_BM;
	STLED316_STB_PORT |= STLED316_STB_PIN_BM;
	STLED316_DIO_PORT |= STLED316_DIO_PIN_BM;
}

//=============================================================================

// STLED316_InitializeInt
// Configures INT I/O lines and interrupt
// - intNum - PORT interrupt number (valid values are 0 or 1)
// - return value - configured interrupt number or -1 if the interrupt number is invalid

int8_t STLED316_InitializeInt( uint8_t intNum )
{
#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
	if( (intNum == 0) || (intNum == 1) 
	{
		DDRD &= ~(1 << (intNum + 2));			// Configure INT0/1 pin as input
		MCUCR &= ~(1 << (2 * intNum));			// Trigger INT0/1 on falling edge only
		MCUCR |= (1 << (2 * intNum + 1));
		GICR |= (1 << (intNum + 6));			// Enable INT0/1 interrupt
		return (int8_t)intNum;
	}
#if defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
	else if( intNum == 2 )
	{
		DDRB &= ~(1 << DDB2);					// Configure INT2 pin as input
		MCUCSR &= ~(1 << ICS2);					// Trigger INT2 on falling edge only
		GICR |= (1 << INT2);					// Enable INT2 interrupt
		return 2;
	}
#endif

#elif defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__)
	if( (intNum >= 0) && (intNum <= 3) )
	{
		DDRD &= ~(1 << intNum);					// Configure INTn pin as input
		EICRA &= ~(1 << (2 * intNum));	// Trigger INTn on falling edge only
		EICRA |= (1 << (2 * intNum + 1));
		EIMSK |= (1 << intNum);			// Enable INTn interrupt
	}
	else if( (intNum >= 4) && (intNum <= 7) )
	{
		DDRE &= ~(1 << intNum);			// Configure INTn pin as input
		EICRB &= ~(1 << (2 * (intNum - 4)));		// Trigger INTn on falling edge only
		EICRB |= (1 << (2 * (intNum - 4) + 1));
		EIMSK |= (1 << intNum);			// Enable INTn interrupt
	}

#endif

	return -1;
}

//=============================================================================

void STLED316_Write(uint8_t *Buffer, uint8_t BytesToWrite)
{
	uint8_t		BitNum, ByteNum;

	STLED316_DIO_DDR |= STLED316_DIO_PIN_BM;

	STLED316_STB_PORT &= ~STLED316_STB_PIN_BM;
	_delay_us(1);

	for( ByteNum = 0; ByteNum < BytesToWrite; ByteNum++ )
	{
		for( BitNum = 0; BitNum < 8; BitNum++ )
		{
			STLED316_CLK_PORT &= ~STLED316_CLK_PIN_BM;
			if( Buffer[ByteNum] & (1 << BitNum) )
				STLED316_DIO_PORT |= STLED316_DIO_PIN_BM;
			else
				STLED316_DIO_PORT &= ~STLED316_DIO_PIN_BM;
			_delay_us( 1 );

			STLED316_CLK_PORT |= STLED316_CLK_PIN_BM;
			_delay_us(1);
		}
	}
	_delay_us(1);

	STLED316_STB_PORT |= STLED316_STB_PIN_BM;
}

//=============================================================================

void STLED316_Read(uint8_t Command, uint8_t * Buffer, uint8_t BytesToRead)
{
	uint8_t char BitNum, ByteNum;

//Send command
	STLED316_DIO_DDR |= STLED316_DIO_PIN_BM;

	STLED316_STB_PORT &= ~STLED316_STB_PIN_BM;

	for( BitNum = 0; BitNum < 8; BitNum++ )
	{
		STLED316_CLK_PORT &= ~STLED316_CLK_PIN_BM;
		if( Command & (1 << BitNum) )
			STLED316_DIO_PORT |= STLED316_DIO_PIN_BM;
		else 
			STLED316_DIO_PORT &= ~STLED316_DIO_PIN_BM;
		_delay_us( 1 );

		STLED316_CLK_PORT |= STLED316_CLK_PIN_BM;
		_delay_us(1);
	}

//Read data
	STLED316_DIO_DDR &= (~STLED316_DIO_PIN_BM);

	for( ByteNum = 0; ByteNum < BytesToRead; ByteNum++ )
	{
		Buffer[ByteNum] = 0;
		for( BitNum = 0; BitNum < 8; BitNum++ )
		{
			STLED316_CLK_PORT &= ~STLED316_CLK_PIN_BM;
			_delay_us(1);

			STLED316_CLK_PORT |= STLED316_CLK_PIN_BM;
			_delay_us(1);

			if( STLED316_DIO_PINS & STLED316_DIO_PIN_BM )
				Buffer[ByteNum] |= (1 << BitNum);
		}
	}

	STLED316_STB_PORT |= STLED316_STB_PIN_BM;
}

//=============================================================================



