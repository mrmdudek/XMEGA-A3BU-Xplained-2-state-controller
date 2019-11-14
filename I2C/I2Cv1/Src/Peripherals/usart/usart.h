/*
 * usart.h
 *
 * Created: 2017-04-24 21:10:37
 *  Author: Zbyszek
 */ 


#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include <stdbool.h>
#include <avr/io.h>

//----------------------------------------------------------------------------

#define	USART_INTLVL_OFF_gc		USART_DREINTLVL_OFF_gc		/* Disable interrupt */
#define USART_INTLVL_LO_gc		USART_DREINTLVL_LO_gc		/* Enable low level interrupt */
#define USART_INTLVL_MED_gc		USART_DREINTLVL_MED_gc		/* Enable medium level interrupt */
#define USART_INTLVL_HI_gc		USART_DREINTLVL_OFF_gc		/* Enable high level interrupt */

//----------------------------------------------------------------------------

/* Communication Mode (mode argument) */
//	USART_CMODE_ASYNCHRONOUS_gc = (0x00<<6),	/* Asynchronous Mode */
//	USART_CMODE_SYNCHRONOUS_gc = (0x01<<6),		/* Synchronous Mode */
//	USART_CMODE_IRDA_gc = (0x02<<6),			/* IrDA Mode */
//	USART_CMODE_MSPI_gc = (0x03<<6),			/* Master SPI Mode */

uint8_t USART_SelectMode( USART_t *pUSART, uint8_t mode );
//----------------------------------------------------------------------------

void USART_SetBSelBScale( USART_t *pUSART, uint8_t clk2x, uint16_t bSel, int8_t bScale );
uint8_t USART_SetBaudRate( USART_t *pUSART, uint8_t clk2x, uint32_t baud, uint32_t cpuFreq );

//----------------------------------------------------------------------------

void USART_CfgUsartRxDPin( PORT_t *port, uint8_t pinMask );
void USART_CfgUsartTxDPin( PORT_t *port, uint8_t pinMask );
void USART_CfgUsartSCKPin( PORT_t *port, uint8_t pinMask, bool masterMode );

uint8_t USART_CfgUsartParam( USART_t *pUSART, uint8_t charSize, uint8_t nStopBits, char parityMode );

static inline void USART_TxEnable( USART_t *pUSART );
static inline void USART_TxDisable( USART_t *pUSART );
static inline void USART_RxEnable( USART_t *pUSART );
static inline void USART_RxDisable( USART_t *pUSART );

static inline void USART_WriteChar( USART_t *pUSART, uint8_t ch );
static inline uint8_t USART_ReadChar( USART_t *pUSART );

void USART_WriteData9( USART_t *pUSART, uint16_t data );
uint16_t USART_ReadData9( USART_t *pUSART );

void USART_WriteBuf( USART_t *pUSART, uint8_t *buf, uint16_t nBytes );
void USART_WriteTxt( USART_t *pUSART, char *txt );

static inline uint8_t USART_RxComplete( USART_t *pUSART );
static inline uint8_t USART_TxComplete( USART_t *pUSART );
static inline uint8_t USART_TxDREmpty( USART_t *pUSART );

static inline void USART_ClrTxCompleteFlag( USART_t *pUSART );
static inline void USART_ClrRxCompleteFlag( USART_t *pUSART );

static inline void USART_CfgTxCompleteInt( USART_t *pUSART, uint8_t level );
static inline void USART_CfgRxCompleteInt( USART_t *pUSART, uint8_t level );
static inline void USART_CfgTxDREmptyInt( USART_t *pUSART, uint8_t level );

static inline void USART_DisableTxCompleteInt( USART_t *pUSART );
static inline void USART_DisableRxCompleteInt( USART_t *pUSART );
static inline void USART_DisableTxDREmptyInt( USART_t *pUSART );

//============================================================================

/* definitions for SPI mode configuration */

/* USART.CTRLC  bit masks and bit positions - additional definitions for SPI mode */
#define USART_UCPHA_bp	1			/* USART SPI Mode Clock Phase bit position. */
#define USART_UCPHA_bm	(1 << 1)	/* USART SPI Mode Clock Phase bit mask. */

#define USART_UDORD_bp	2			/* USART SPI Mode Data Order bit position. */
#define USART_UDORD_bm	(1 << 2)	/* USART SPI Mode Data Order bit mask. */

/*
typedef struct USART_SPI_SSPin_struct
{
	PORT_t	*port;
	uint8_t	pinMask;
} USART_SPI_SSPin_t;
*/

#define USART_SPI_SCK_MODE_0	0					// SCK leading edge - rising, data sample (CPOL=0, CPHA=0)
#define USART_SPI_SCK_MODE_1	1					// SCK leading edge - rising, data setup (CPOL=0, CPHA=1)
#define USART_SPI_SCK_MODE_2	2					// SCK leading edge - falling, data sample (CPOL=1, CPHA=0)
#define USART_SPI_SCK_MODE_3	3					// SCK leading edge - falling, data setup (CPOL=1, CPHA=1)

#define USART_SPI_DORD_MSB		0					// Data order - MSB first
#define USART_SPI_DORD_LSB		USART_UDORD_bm		// Data order - LSB first

//----------------------------------------------------------------------------

static inline void USART_CfgSpiMISOPin( PORT_t *port, uint8_t pinMask );
static inline void USART_CfgSpiMOSIPin( PORT_t *port, uint8_t pinMask );
void USART_CfgSpiSCKPin( PORT_t *port, uint8_t pinMask, uint8_t spiSckMode );
void USART_CfgSpiSSPin( PORT_t *port, uint8_t pinMask );

void USART_CfgSpiParam( USART_t *pUSART, uint8_t spiSckMode, uint8_t spiDORD );

static inline void USART_SpiSelectDevice( PORT_t *port, uint8_t pinMask );
static inline void USART_SpiDeselectDevice( PORT_t *port, uint8_t pinMask );

static inline uint8_t USART_SpiTransmit( USART_t *pUSART, uint8_t txCh );

void USART_SpiWriteBuf( USART_t *pUSART, uint8_t *buf, uint16_t len );
void USART_SpiReadBuf( USART_t *pUSART, uint8_t *buf, uint16_t len );
void USART_SpiTransmitBuf( USART_t *pUSART, uint8_t *txBuf, uint8_t *rxBuf, uint16_t len );

//============================================================================

static inline void USART_CfgSpiMISOPin( PORT_t *port, uint8_t pinMask )
{
	USART_CfgUsartRxDPin( port, pinMask );
}
//----------------------------------------------------------------------------

static inline void USART_CfgSpiMOSIPin( PORT_t *port, uint8_t pinMask )
{
	USART_CfgUsartTxDPin( port, pinMask );
}
//----------------------------------------------------------------------------

static inline void USART_TxEnable( USART_t *pUSART )
{
	pUSART->CTRLB |= USART_TXEN_bm;
}
//----------------------------------------------------------------------------

static inline void USART_TxDisable( USART_t *pUSART )
{
	pUSART->CTRLB &= ~USART_TXEN_bm;
}
//----------------------------------------------------------------------------

static inline void USART_RxEnable( USART_t *pUSART )
{
	pUSART->CTRLB |= USART_RXEN_bm;
}
//----------------------------------------------------------------------------

static inline void USART_RxDisable( USART_t *pUSART )
{
	pUSART->CTRLB &= ~USART_TXEN_bm;
}
//----------------------------------------------------------------------------

static inline void USART_WriteChar( USART_t *pUSART, uint8_t ch )
{
	pUSART->DATA = ch;
}
//----------------------------------------------------------------------------

static inline uint8_t USART_ReadChar( USART_t *pUSART )
{
	return pUSART->DATA;
}
//----------------------------------------------------------------------------

static inline uint8_t USART_RxComplete( USART_t *pUSART )
{
	return (pUSART->STATUS & USART_RXCIF_bm);
}
//----------------------------------------------------------------------------

static inline uint8_t USART_TxComplete( USART_t *pUSART )
{
	return (pUSART->STATUS & USART_TXCIF_bm);
}
//----------------------------------------------------------------------------

static inline uint8_t USART_TxDREmpty( USART_t *pUSART )
{
	return (pUSART->STATUS & USART_DREIF_bm);
}
//----------------------------------------------------------------------------

static inline void USART_ClrTxCompleteFlag( USART_t *pUSART )
{
	pUSART->STATUS |= USART_TXCIF_bm;
}
//----------------------------------------------------------------------------

static inline void USART_ClrRxCompleteFlag( USART_t *pUSART )
{
	pUSART->STATUS |= USART_RXCIF_bm;
}
//----------------------------------------------------------------------------

static inline void USART_CfgTxCompleteInt( USART_t *pUSART, uint8_t level )
{
	(pUSART)->CTRLA = ((pUSART)->CTRLA & ~USART_TXCINTLVL_gm) | (level << USART_TXCINTLVL_gp);
}
//----------------------------------------------------------------------------

static inline void USART_CfgRxCompleteInt( USART_t *pUSART, uint8_t level )
{
	(pUSART)->CTRLA = ((pUSART)->CTRLA & ~USART_RXCINTLVL_gm) |	(level << USART_RXCINTLVL_gp);
}
//----------------------------------------------------------------------------

static inline void USART_CfgTxDREmptyInt( USART_t *pUSART, uint8_t level )
{
	(pUSART)->CTRLA = ((pUSART)->CTRLA & ~USART_DREINTLVL_gm) |	(level << USART_DREINTLVL_gp);
}
//----------------------------------------------------------------------------

static inline void USART_DisableTxCompleteInt( USART_t *pUSART )
{
	(pUSART)->CTRLA &= ~USART_TXCINTLVL_gm;
}
//----------------------------------------------------------------------------

static inline void USART_DisableRxCompleteInt( USART_t *pUSART )
{
	(pUSART)->CTRLA &= ~USART_RXCINTLVL_gm;
}
//----------------------------------------------------------------------------

static inline void USART_DisableTxDREmptyInt( USART_t *pUSART )
{
	(pUSART)->CTRLA &= ~USART_DREINTLVL_gm;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

static inline void USART_SpiSelectDevice( PORT_t *port, uint8_t pinMask )
{
	port->OUTCLR = pinMask;
}
//----------------------------------------------------------------------------

static inline void USART_SpiDeselectDevice( PORT_t *port, uint8_t pinMask )
{
	port->OUTSET = pinMask;
}
//----------------------------------------------------------------------------

static inline uint8_t USART_SpiTransmit( USART_t *pUSART, uint8_t txCh )
{
	while( !USART_TxDREmpty( pUSART ) );
	USART_WriteChar( pUSART, txCh );
	while( !USART_TxComplete( pUSART ) );
	USART_ClrTxCompleteFlag( pUSART );
	return USART_ReadChar( pUSART );
}
//----------------------------------------------------------------------------

#endif /* USART_H_INCLUDED */

