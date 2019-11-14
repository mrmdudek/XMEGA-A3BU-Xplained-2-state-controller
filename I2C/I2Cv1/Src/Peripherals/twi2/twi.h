/*
 * twi.h
 *
 * Created: 2018-05-10 21:10:37
 *  Author: Zbyszek
 */ 


#ifndef TWI_H_INCLUDED
#define TWI_H_INCLUDED

#include <stdbool.h>
#include <avr/io.h>

//----------------------------------------------------------------------------

#define	TWI_INTLVL_OFF_gc		TWI_MASTER_INTLVL_OFF_gc		/* Disable interrupt */
#define TWI_INTLVL_LO_gc		TWI_MASTER_INTLVL_LO_gc			/* Enable low level interrupt */
#define TWI_INTLVL_MED_gc		TWI_MASTER_INTLVL_MED_gc		/* Enable medium level interrupt */
#define TWI_INTLVL_HI_gc		TWI_MASTER_INTLVL_OFF_gc		/* Enable high level interrupt */

//----------------------------------------------------------------------------

#define TWI_ERR_NOSLAVE		(-1)
#define TWI_ERR_ARBLOST		(-11)
#define TWI_ERR_BUSERR		(-12)

//----------------------------------------------------------------------------

void TWI_CfgPins( PORT_t *port, uint8_t pinMask, bool pullUp );

uint8_t TWI_SetBaudRate( TWI_t *pTWI, uint32_t baud, uint32_t cpuFreq );

//============================================================================
// TWI Master
//============================================================================

static inline void TWIM_Enable( TWI_t *pTWI );
static inline void TWIM_Disable( TWI_t *pTWI );

static inline void TWIM_WriteAddr( TWI_t *pTWI, uint8_t addr, bool masterRead );
static inline void TWIM_WriteData( TWI_t *pTWI, uint8_t data );
static inline uint8_t TWIM_ReadData( TWI_t *pTWI );

static inline void TWIM_EnableQuickCmd( TWI_t *pTWI );
static inline void TWIM_DisableQuickCmd( TWI_t *pTWI );
static inline void TWIM_EnableSmartMode( TWI_t *pTWI );
static inline void TWIM_DisableSmartMode( TWI_t *pTWI );

static inline void TWIM_SetACKAction( TWI_t *pTWI );
static inline void TWIM_SetNACKAction( TWI_t *pTWI );

static inline void TWIM_Send_RSTART( TWI_t *pTWI );
static inline void TWIM_Send_ACK( TWI_t *pTWI );
static inline void TWIM_Send_NACK( TWI_t *pTWI );
static inline void TWIM_Send_STOP( TWI_t *pTWI );

/* Bus state constants */
//	TWI_MASTER_BUSSTATE_UNKNOWN_gc = (0x00<<0),		/* Unknown Bus State */
//	TWI_MASTER_BUSSTATE_IDLE_gc = (0x01<<0),		/* Bus is Idle */
//	TWI_MASTER_BUSSTATE_OWNER_gc = (0x02<<0),		/* This Module Controls The Bus */
//	TWI_MASTER_BUSSTATE_BUSY_gc = (0x03<<0),		/* The Bus is Busy */

/* STATUS bit masks */
#define TWI_MASTER_BUSSTATE_gm  0x03				/* Bus State group mask. */
#define TWI_MASTER_BUSERR_bm  0x04					/* Bus Error bit mask. */
#define TWI_MASTER_ARBLOST_bm  0x08					/* Arbitration Lost bit mask. */
#define TWI_MASTER_RXACK_bm  0x10					/* Received Acknowledge bit mask. */
#define TWI_MASTER_CLKHOLD_bm  0x20					/* Clock Hold bit mask. */
#define TWI_MASTER_WIF_bm  0x40						/* Write Interrupt Flag bit mask. */
#define TWI_MASTER_RIF_bm  0x80						/* Read Interrupt Flag bit mask. */

static inline uint8_t TWIM_ReadStatus( TWI_t *pTWI );

static inline void TWIM_CfgIntLvl( TWI_t *pTWI, uint8_t level );
static inline void TWIM_EnableWriteInt( TWI_t *pTWI );
static inline void TWIM_DisableWriteInt( TWI_t *pTWI );
static inline void TWIM_EnableReadInt( TWI_t *pTWI );
static inline void TWIM_DisableReadInt( TWI_t *pTWI );

static inline void TWIM_ClrReadIntFlag( TWI_t *pTWI );
static inline void TWIM_ClrWriteIntFlag( TWI_t *pTWI );
static inline void TWIM_ClrArbLostFlag( TWI_t *pTWI );
static inline void TWIM_ClrBusErrFlag( TWI_t *pTWI );
static inline void TWIM_SetIdleBusState( TWI_t *pTWI );

//----------------------------------------------------------------------------

int8_t TWIM_WriteBuf( TWI_t *pTWI, uint8_t slaveAddr, uint8_t *data, uint8_t dataSize );
int8_t TWIM_ReadBuf( TWI_t *pTWI, uint8_t slaveAddr, uint8_t *cmd, uint8_t cmdSize, uint8_t *data, uint8_t dataSize );

//============================================================================
// TWI Slave
//============================================================================



//===========================================================================

static inline void TWIM_Enable( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLA |= TWI_MASTER_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_Disable( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLA &= ~TWI_MASTER_ENABLE_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_WriteAddr( TWI_t *pTWI, uint8_t addr, bool masterRead )
{
	pTWI->MASTER.ADDR = (addr << 1) | (masterRead ? 1 : 0);
}
//----------------------------------------------------------------------------

static inline void TWIM_WriteData( TWI_t *pTWI, uint8_t data )
{
	pTWI->MASTER.DATA = data;
}
//----------------------------------------------------------------------------

static inline uint8_t TWIM_ReadData( TWI_t *pTWI )
{
	return pTWI->MASTER.DATA;
}
//----------------------------------------------------------------------------

static inline void TWIM_EnableQuickCmd( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLB |= TWI_MASTER_QCEN_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_DisableQuickCmd( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLB &= ~TWI_MASTER_QCEN_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_EnableSmartMode( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLB |= TWI_MASTER_SMEN_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_DisableSmartMode( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLB &= ~TWI_MASTER_SMEN_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_SetACKAction( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLC &= ~TWI_MASTER_ACKACT_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_SetNACKAction( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLC |= TWI_MASTER_ACKACT_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_Send_RSTART( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLC |= TWI_MASTER_CMD_REPSTART_gc;
}
//----------------------------------------------------------------------------

static inline void TWIM_Send_ACK( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
}
//----------------------------------------------------------------------------

static inline void TWIM_Send_NACK( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLC = TWI_MASTER_ACKACT_bm | TWI_MASTER_CMD_RECVTRANS_gc;
}
//----------------------------------------------------------------------------

static inline void TWIM_Send_STOP( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLC |= TWI_MASTER_CMD_STOP_gc;
}
//----------------------------------------------------------------------------

static inline uint8_t TWIM_ReadStatus( TWI_t *pTWI )
{
	return pTWI->MASTER.STATUS;
}
//----------------------------------------------------------------------------

static inline void TWIM_CfgIntLvl( TWI_t *pTWI, uint8_t level )
{
	pTWI->MASTER.CTRLA = (pTWI->MASTER.CTRLA & ~TWI_MASTER_INTLVL_gm) | level;
}
//----------------------------------------------------------------------------

static inline void TWIM_EnableWriteInt( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLA |= TWI_MASTER_WIEN_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_DisableWriteInt( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLA &= ~TWI_MASTER_WIEN_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_EnableReadInt( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLA |= TWI_MASTER_RIEN_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_DisableReadInt( TWI_t *pTWI )
{
	pTWI->MASTER.CTRLA &= ~TWI_MASTER_RIEN_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_ClrReadIntFlag( TWI_t *pTWI )
{
	pTWI->MASTER.STATUS |= TWI_MASTER_RIF_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_ClrWriteIntFlag( TWI_t *pTWI )
{
	pTWI->MASTER.STATUS |= TWI_MASTER_WIF_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_ClrArbLostFlag( TWI_t *pTWI )
{
	pTWI->MASTER.STATUS |= TWI_MASTER_ARBLOST_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_ClrBusErrFlag( TWI_t *pTWI )
{
	pTWI->MASTER.STATUS |= TWI_MASTER_BUSERR_bm;
}
//----------------------------------------------------------------------------

static inline void TWIM_SetIdleBusState( TWI_t *pTWI )
{
	pTWI->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

#endif /* TWI_H_INCLUDED */

