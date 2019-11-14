/*
 * twi.c
 *
 * Created: 2018-05-10 21:10:16
 *  Author: Zbyszek
 */ 

//----------------------------------------------------------------------------

#include "twi/twi.h"

//----------------------------------------------------------------------------

void TWI_CfgPins( PORT_t *port, uint8_t pinMask, bool pullUp )
{
	PORTCFG.MPCMASK	= pinMask;
	if( pullUp )
		port->PIN0CTRL = PORT_ISC_BOTHEDGES_gc | PORT_OPC_WIREDANDPULL_gc;
	else
		port->PIN0CTRL = PORT_ISC_BOTHEDGES_gc | PORT_OPC_WIREDAND_gc;
	port->DIRSET = pinMask;

}
//----------------------------------------------------------------------------

uint8_t TWI_SetBaudRate( TWI_t *pTWI, uint32_t twiFreq, uint32_t cpuFreq )
{
	uint32_t	tmp;
	
	tmp = cpuFreq / (2 * twiFreq) - 5;
	pTWI->MASTER.BAUD = (uint8_t)tmp;
	
	return (uint8_t)tmp;
}
//----------------------------------------------------------------------------

int8_t TWIM_WriteBuf( TWI_t *pTWI, uint8_t slaveAddr, uint8_t *data, uint8_t dataSize )
{
	uint8_t		status;
	uint8_t		i;
	bool		ack;
	
	TWIM_WriteAddr( pTWI, slaveAddr, false );
	while( ((status = TWIM_ReadStatus( pTWI )) & TWI_MASTER_WIF_bm) == 0 );
	if( (status & TWI_MASTER_BUSERR_bm) != 0 )
	{														// Bus Error
		return TWI_ERR_BUSERR;		
	}
	
	if( (status & TWI_MASTER_ARBLOST_bm) != 0 )
	{														// Bus Error
		return TWI_ERR_ARBLOST;
	}

	if( (status & TWI_MASTER_RXACK_bm) != 0 )
	{														// NACK
		TWIM_Send_STOP( pTWI );
		return TWI_ERR_NOSLAVE;
	}

	ack = true;
	for( i = 0; (i < dataSize) && ack; i++ )
	{
		TWIM_WriteData( pTWI, data[i] );
		while( ((status = TWIM_ReadStatus( pTWI )) & TWI_MASTER_WIF_bm) == 0 );
		ack = (status & TWI_MASTER_RXACK_bm) == 0;
	}
	TWIM_Send_STOP( pTWI );

	return i;
}
//----------------------------------------------------------------------------

int8_t TWIM_ReadBuf( TWI_t *pTWI, uint8_t slaveAddr, uint8_t *cmd, uint8_t cmdSize, uint8_t *data, uint8_t dataSize )
{
	uint8_t		status;
	uint8_t		i;
	bool		ack;
	

	if( cmdSize != 0 )
	{
		TWIM_WriteAddr( pTWI, slaveAddr, false );
		while( ((status = TWIM_ReadStatus( pTWI )) & TWI_MASTER_WIF_bm) == 0 );
		if( (status & TWI_MASTER_BUSERR_bm) != 0 )
		{														// Bus Error
			return TWI_ERR_BUSERR;
		}
		
		if( (status & TWI_MASTER_ARBLOST_bm) != 0 )
		{														// Bus Error
			return TWI_ERR_ARBLOST;
		}

		if( (status & TWI_MASTER_RXACK_bm) != 0 )
		{														// NACK
			TWIM_Send_STOP( pTWI );
			return TWI_ERR_NOSLAVE;
		}

		ack = true;
		for( i = 0; (i < dataSize) && ack; i++ )
		{
			TWIM_WriteData( pTWI, data[i] );
			while( ((status = TWIM_ReadStatus( pTWI )) & TWI_MASTER_WIF_bm) == 0 );
			ack = (status & TWI_MASTER_RXACK_bm) == 0;
		}
	}

	TWIM_WriteAddr( pTWI, slaveAddr, true );
	while( ((status = TWIM_ReadStatus( pTWI )) & (TWI_MASTER_WIF_bm | TWI_MASTER_RIF_bm)) == 0 );
	
	if( (status & TWI_MASTER_WIF_bm) != 0 )
	{
		if( (status & TWI_MASTER_BUSERR_bm) != 0 )
		{														// Bus Error
			return TWI_ERR_BUSERR;
		}
		
		if( (status & TWI_MASTER_ARBLOST_bm) != 0 )
		{														// Bus Error
			return TWI_ERR_ARBLOST;
		}

		if( (status & TWI_MASTER_RXACK_bm) != 0 )
		{														// NACK
			TWIM_Send_STOP( pTWI );
			return TWI_ERR_NOSLAVE;
		}
	}

	i = 0;
	dataSize--;

	TWIM_EnableSmartMode( &TWIC );
	TWIM_SetACKAction( &TWIC );

	while( i <= dataSize )
	{
		if( i < dataSize )
		{
			data[i] = TWIM_ReadData( pTWI );
			if( i < dataSize )
				while( ((status = TWIM_ReadStatus( pTWI )) & TWI_MASTER_RIF_bm) == 0 );
		}
		else
		{
			TWIM_SetNACKAction( &TWIC );
			data[i] = TWIM_ReadData( pTWI );
		}
	
		i++;
	} 
	
	TWIM_Send_STOP( pTWI );
		
	return i;
	
}
//----------------------------------------------------------------------------

