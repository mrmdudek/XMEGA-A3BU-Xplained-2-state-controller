/*
 * usart.c
 *
 * Created: 2017-04-24 21:10:16
 *  Author: Zbyszek
 */ 

//----------------------------------------------------------------------------

#include "usart/usart.h"

//----------------------------------------------------------------------------

uint8_t USART_SelectMode( USART_t *pUSART, uint8_t mode )
{
	mode &= USART_CMODE_gm;
	pUSART->CTRLC = mode;

	return mode;
}
//----------------------------------------------------------------------------

void USART_CfgUsartRxDPin( PORT_t *port, uint8_t pinMask )
{
	PORTCFG.MPCMASK	= pinMask;
	port->PIN0CTRL = PORT_ISC_BOTHEDGES_gc | PORT_OPC_TOTEM_gc;
	port->DIRCLR = pinMask;
}
//----------------------------------------------------------------------------

void USART_CfgUsartTxDPin( PORT_t *port, uint8_t pinMask )
{
	PORTCFG.MPCMASK	= pinMask;
	port->PIN0CTRL = PORT_ISC_BOTHEDGES_gc | PORT_OPC_TOTEM_gc;
	port->DIRSET = pinMask;
	port->OUTSET = pinMask;
}
//----------------------------------------------------------------------------

void USART_CfgUsartSCKPin( PORT_t *port, uint8_t pinMask, bool masterMode )
{
	PORTCFG.MPCMASK	= pinMask;
	port->PIN0CTRL = PORT_ISC_BOTHEDGES_gc | PORT_OPC_TOTEM_gc;
	if( masterMode )
	{
		port->DIRSET = pinMask;
		port->OUTCLR = pinMask;
	}
	else
		port->DIRCLR = pinMask;
}
//----------------------------------------------------------------------------

uint8_t USART_CfgUsartParam( USART_t *pUSART, uint8_t charSize, uint8_t nStopBits, char parityMode )
{
	switch( charSize )
	{
		case 5:
			charSize = USART_CHSIZE_5BIT_gc;
			break;
		case 6:
			charSize = USART_CHSIZE_6BIT_gc;
			break;
		case 7:
			charSize = USART_CHSIZE_7BIT_gc;
			break;
		case 8:
			charSize = USART_CHSIZE_8BIT_gc;
			break;
		case 9:
			charSize = USART_CHSIZE_9BIT_gc;
			break;
		default:
			return 1;
	}

	switch( nStopBits )
	{
		case 1:
			nStopBits = 0;
			break;
		case 2:
			nStopBits = USART_SBMODE_bm;
			break;
		default:
			return 2;
	}

	switch( parityMode )
	{
		case 'N':
		case 'n':
			parityMode = USART_PMODE_DISABLED_gc;
			break;
		case 'E':
		case 'e':
			parityMode = USART_PMODE_EVEN_gc;
			break;
		case 'O':
		case 'o':
			parityMode = USART_PMODE_ODD_gc;
			break;
		default:
			return 3;
	}
	
	pUSART->CTRLC = (pUSART->CTRLC & ~(USART_CHSIZE_gm | USART_SBMODE_bm | USART_PMODE_gm)) | (charSize | nStopBits | parityMode);

	return 0;

}
//----------------------------------------------------------------------------

void USART_SetBSelBScale( USART_t *pUSART, uint8_t clk2x, uint16_t bSel, int8_t bScale )
{
	if( clk2x != 0 )
		pUSART->CTRLB |= USART_CLK2X_bm;
	else
		pUSART->CTRLB &= ~USART_CLK2X_bm;
	pUSART->BAUDCTRLA = bSel & 0xFF;
	pUSART->BAUDCTRLB = ((bScale << 4) & 0xF0) | ((bSel >> 8) & 0x0F);
}
//----------------------------------------------------------------------------

uint8_t USART_SetAsyncBaudRate( USART_t *pUSART, uint8_t clk2x, uint32_t baud, uint32_t cpuFreq )
{
	int8_t exp;
	uint32_t div;
	uint32_t limit;
	uint32_t ratio;
	uint32_t min_rate;
	uint32_t max_rate;

	if( clk2x != 0 )
		pUSART->CTRLB |= USART_CLK2X_bm;
	else
		pUSART->CTRLB &= ~USART_CLK2X_bm;

/*
 * Check if the hardware supports the given baud rate
 */
/* 8 = (2^0) * 8 * (2^0) = (2^BSCALE_MIN) * 8 * (BSEL_MIN) */
	max_rate = cpuFreq / 8;
/* 4194304 = (2^7) * 8 * (2^12) = (2^BSCALE_MAX) * 8 * (BSEL_MAX+1) */
	min_rate = cpuFreq / 4194304;

	if((pUSART->CTRLB & USART_CLK2X_bm) == 0 )
	{
		max_rate /= 2;
		min_rate /= 2;
	}

	if( (baud > max_rate) || (baud < min_rate) )
		return 1;

/* Check if double speed is enabled. */
	if( (pUSART->CTRLB & USART_CLK2X_bm) == 0 )
		baud *= 2;

/* Find the lowest possible exponent. */
	limit = 0xfffU >> 4;
	ratio = cpuFreq / baud;

	for( exp = -7; exp < 7; exp++ )
	{
		if (ratio < limit)
			break;

		limit <<= 1;

		if (exp < -3)
			limit |= 1;
	}

/*
 * Depending on the value of exp, scale either the input frequency or
 * the target baud rate. By always scaling upwards, we never introduce
 * any additional inaccuracy.
 *
 * We are including the final divide-by-8 (aka. right-shift-by-3) in
 * this operation as it ensures that we never exceeed 2**32 at any
 * point.
 *
 * The formula for calculating BSEL is slightly different when exp is
 * negative than it is when exp is positive.
 */
	if (exp < 0)
	{
/* We are supposed to subtract 1, then apply BSCALE. We want to
 * apply BSCALE first, so we need to turn everything inside the
 * parenthesis into a single fractional expression.
 */
		cpuFreq -= 8 * baud;

/* If we end up with a left-shift after taking the final
 * divide-by-8 into account, do the shift before the divide.
 * Otherwise, left-shift the denominator instead (effectively
 * resulting in an overall right shift.)
 */
		if( exp <= -3 )
			div = ((cpuFreq << (-exp - 3)) + baud / 2) / baud;
		else
		{
			baud <<= exp + 3;
			div = (cpuFreq + baud / 2) / baud;
		}
	}
	else
	{
/* We will always do a right shift in this case, but we need to
 * shift three extra positions because of the divide-by-8.
 */
		baud <<= exp + 3;
		div = (cpuFreq + baud / 2) / baud - 1;
	}

	pUSART->BAUDCTRLB = (uint8_t)(((div >> 8) & 0X0F) | (exp << 4));
	pUSART->BAUDCTRLA = (uint8_t)div;

	return 0;
}
//----------------------------------------------------------------------------

uint8_t USART_SetSyncBaudRate( USART_t *pUSART, uint32_t baud, uint32_t cpuFreq )
{
	uint16_t bsel_value;

	pUSART->CTRLB &= ~USART_CLK2X_bm;


/* Check if baudrate is less than the maximum limit specified in datasheet
 */
	if( baud < (cpuFreq / 2) ) 
		bsel_value = ( cpuFreq / (baud * 2)) - 1;
	else
/* If baudrate is not within the specfication in datasheet,
 * assign maximum baudrate possible for the current CPU frequency
 */
		bsel_value = 0;

	pUSART->BAUDCTRLB = (uint8_t)((~USART_BSCALE_gm) & (bsel_value >> 8));
	pUSART->BAUDCTRLA = (uint8_t)(bsel_value);

	return 0;
}
//----------------------------------------------------------------------------

uint8_t USART_SetBaudRate( USART_t *pUSART, uint8_t clk2x, uint32_t baud, uint32_t cpuFreq )
{
	switch( pUSART->CTRLC & USART_CMODE_gm )
	{
		case USART_CMODE_ASYNCHRONOUS_gc :
			return USART_SetAsyncBaudRate( pUSART, clk2x, baud, cpuFreq );
			break;
		case USART_CMODE_IRDA_gc :
			return USART_SetAsyncBaudRate( pUSART, 0, baud, cpuFreq );
			break;
		case USART_CMODE_SYNCHRONOUS_gc :
		case USART_CMODE_MSPI_gc :
			return USART_SetSyncBaudRate( pUSART, baud, cpuFreq );
			break;
	}
	return 1;
}
//----------------------------------------------------------------------------

void USART_WriteData9( USART_t *pUSART, uint16_t data )
{
	if( (data & 0x0100) != 0 )
		pUSART->CTRLB |= USART_TXB8_bm;
	else
		pUSART->CTRLB &= ~USART_TXB8_bm;
	pUSART->DATA = data & 0xFF;
}

//----------------------------------------------------------------------------

uint16_t USART_ReadData9( USART_t *pUSART )
{
	uint16_t	data;
	
	data = ((pUSART->STATUS & USART_RXB8_bm) != 0) ? 0x0100 : 0;
	data |= pUSART->DATA;
	return data;
}
//----------------------------------------------------------------------------

void USART_WriteBuf( USART_t *pUSART, uint8_t *buf, uint16_t nBytes )
{
	uint16_t	i;
	
	for( i = 0; i < nBytes; i++ )
	{
		while( !USART_TxDREmpty( pUSART ) );
		USART_WriteChar( pUSART, buf[i] );
	}
}
//----------------------------------------------------------------------------

void USART_WriteTxt( USART_t *pUSART, char *txt )
{
	while( *txt != '\0' )
	{
		while( !USART_TxDREmpty( pUSART ) );
		USART_WriteChar( pUSART, *txt++ );
	}
}

//============================================================================

void USART_CfgSpiSCKPin( PORT_t *port, uint8_t pinMask, uint8_t spiSckMode )
{
	uint8_t		cpol = 0;

	if( (spiSckMode == USART_SPI_SCK_MODE_2) || (spiSckMode == USART_SPI_SCK_MODE_3))
		cpol = PORT_INVEN_bm;

	PORTCFG.MPCMASK	= pinMask;
	port->PIN0CTRL = PORT_ISC_BOTHEDGES_gc | PORT_OPC_TOTEM_gc | cpol;
	port->DIRSET = pinMask;
	port->OUTCLR = pinMask;
}
//----------------------------------------------------------------------------

void USART_CfgSpiSSPin( PORT_t *port, uint8_t pinMask )
{
	PORTCFG.MPCMASK	= pinMask;
	port->PIN0CTRL = PORT_ISC_BOTHEDGES_gc | PORT_OPC_TOTEM_gc;
	port->DIRSET = pinMask;
	port->OUTSET = pinMask;
}
//----------------------------------------------------------------------------

void USART_CfgSpiParam( USART_t *pUSART, uint8_t spiSckMode, uint8_t spiDORD )
{
	uint8_t		cpha = 0;

	if( (spiSckMode == USART_SPI_SCK_MODE_1) || (spiSckMode == USART_SPI_SCK_MODE_3))
		cpha = USART_UCPHA_bm;

	pUSART->CTRLC = (pUSART->CTRLC & ~(USART_UDORD_bm | USART_UCPHA_bm)) | spiDORD | cpha;
}
//----------------------------------------------------------------------------

/*
status_code_t usart_spi_write_packet(USART_t *usart,const uint8_t *data, size_t len)
{
	size_t i=0;
	while(len) {
		usart_spi_transmit(usart,*(data+i));
		len--;
		i++;
	}
	return STATUS_OK;
}
*/
void USART_SpiWriteBuf( USART_t *pUSART, uint8_t *buf, uint16_t len )
{
	uint16_t	i;

	for( i = 0; i < len; i++ )
		USART_SpiTransmit( pUSART, buf[i] );
}
//----------------------------------------------------------------------------

/*
status_code_t usart_spi_read_packet(USART_t *usart, uint8_t *data, size_t len)
{
	while(len) {
		*data = usart_spi_transmit(usart, CONFIG_USART_SPI_DUMMY);
		len--;
		data++;
	}
	return STATUS_OK;
}
*/
void USART_SpiReadBuf( USART_t *pUSART, uint8_t *buf, uint16_t len )
{
	uint16_t	i;

	for( i = 0; i < len; i++ )
		buf[i] = USART_SpiTransmit( pUSART, 0 );
}

//----------------------------------------------------------------------------

void USART_SpiTransmitBuf( USART_t *pUSART, uint8_t *txBuf, uint8_t *rxBuf, uint16_t len )
{
	uint16_t	i;

	for( i = 0; i < len; i++ )
		rxBuf[i] = USART_SpiTransmit( pUSART, txBuf[i] );
}
//----------------------------------------------------------------------------


