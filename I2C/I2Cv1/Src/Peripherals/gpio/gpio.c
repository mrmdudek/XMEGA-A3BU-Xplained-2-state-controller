/*
 * gpio.c
 *
 * Created: 2017-04-18 22:47:33
 *  Author: Zbyszek
 */ 

//----------------------------------------------------------------------------

#include "gpio/gpio.h"

//----------------------------------------------------------------------------

void GPIO_CfgPins( PORT_t *pPort, uint8_t pinMask, uint8_t pinIOMode, uint8_t pinDirOutput, uint8_t pinInitState, uint8_t pinAttr )
{
	PORTCFG.MPCMASK = pinMask;
	pPort->PIN0CTRL = pinIOMode | pinAttr;
	
	if( pinDirOutput != 0 )				// output
		pPort->DIRSET = pinMask;
	else								// input
		pPort->DIRCLR = pinMask;

	if( pinInitState != 0 )
		pPort->OUTSET = pinMask;
	else
		pPort->OUTCLR = pinMask;

}
//----------------------------------------------------------------------------
/*
void GPIO_IntEnable( PORT_t *pPort, uint8_t intNum, uint8_t intLevel )
{
	uint8_t		tmp;

	tmp = pPort->INTCTRL;
	if( intNum == 0 )
		pPort->INTCTRL = (tmp & ~PORT_INT0LVL_gm) | intLevel;
	else
		pPort->INTCTRL = (tmp & ~PORT_INT1LVL_gm) | (intLevel << 2);
}
*/
//----------------------------------------------------------------------------

/*
void GPIO_IntDisable( PORT_t *pPort, uint8_t intNum )
{
	if( intNum == 0 )
		pPort->INTCTRL &=  ~PORT_INT0LVL_gm;
	else
		pPort->INTCTRL &=  ~PORT_INT1LVL_gm;
}
*/
//----------------------------------------------------------------------------

