/*
 * gpio.h
 *
 * Created: 2017-04-18 22:48:08
 *  Author: Zbyszek
 */ 


#ifndef GPIO_H_INCLUDED
#define GPIO_H_INCLUDED

#include <avr/io.h>

#define GPIO_PIN_0					0x01						/* Port pin 0 mask */
#define GPIO_PIN_1					0x02						/* Port pin 1 mask */
#define GPIO_PIN_2					0x04						/* Port pin 2 mask */
#define GPIO_PIN_3					0x08						/* Port pin 3 mask */
#define GPIO_PIN_4					0x10						/* Port pin 4 mask */
#define GPIO_PIN_5					0x20						/* Port pin 5 mask */
#define GPIO_PIN_6					0x40						/* Port pin 6 mask */
#define GPIO_PIN_7					0x80						/* Port pin 7 mask */

#define GPIO_PIN_ALL				0xFF						/* Port all pins mask */
#define GPIO_PIN_0123				0x0F						/* Port pins 0, 1, 2, 3 mask */
#define GPIO_PIN_4567				0xF0						/* Port pins 4, 5, 6, 7 mask */

//-----------------------------------------------------------------------------

/* pinIOMode argument should be:
*  one of the PORT_OPC_xxxx and PORT_ISC_xxxx constants
*  OR
*  bit-or sum of one of the PORT_OPC_xxxx constants and one of the PORT_ISC_xxxx constants.
*/
/* pinIOMode output configuration constants */
//	PORT_OPC_TOTEM_gc			/* Totempole = 0 (default value)*/
//	PORT_OPC_BUSKEEPER_gc		/* Totempole w/ Bus keeper on Input and Output */
//	PORT_OPC_PULLDOWN_gc		/* Totempole w/ Pull-down on Input */
//	PORT_OPC_PULLUP_gc			/* Totempole w/ Pull-up on Input */
//	PORT_OPC_WIREDOR_gc			/* Wired OR */
//	PORT_OPC_WIREDAND_gc		/* Wired AND */
//	PORT_OPC_WIREDORPULL_gc		/* Wired OR w/ Pull-down */
//	PORT_OPC_WIREDANDPULL_gc	/* Wired AND w/ Pull-up */

/* pinIOMode input sensing constants */
//	PORT_ISC_BOTHEDGES_gc		/* Sense Both Edges = 0 (default value) */
//	PORT_ISC_RISING_gc			/* Sense Rising Edge */
//	PORT_ISC_FALLING_gc			/* Sense Falling Edge */
//	PORT_ISC_LEVEL_gc			/* Sense Level (Transparent For Events) */
//	PORT_ISC_INPUT_DISABLE_gc	/* Disable Digital Input Buffer */

/* pinDirOutput argument. Pin direction */
// 0							/* pin(s) sets as input */
// non zero						/* pin(s) sets as output */

/* pinInitState argument. Pin initial state. This argument is taken into account if pin is set as output (pinDirOutput != 0) */
// 0							/* pin(s) sets low */
// non zero						/* pin(s) sets high */

/* pinAttr argument constants or their bit-or sum */
//	0							/* No I/O inversion, no slew rate limit */
//	PORT_INVEN_bm				/* Enable I/O inversion */
//	PORT_SRLEN_bm				/* Enable slew rate limit */

void GPIO_CfgPins( PORT_t *pPort, uint8_t pinMask, uint8_t pinIOMode, uint8_t pinDirOutput, uint8_t pinInitState, uint8_t pinAttr );

//-----------------------------------------------------------------------------

static inline void GPIO_DirInput( PORT_t *pPort, uint8_t pinMask );
static inline void GPIO_DirOutput( PORT_t *pPort, uint8_t pinMask );
static inline void GPIO_ToggleDir( PORT_t *pPort, uint8_t pinMask );
static inline void GPIO_WriteDir( PORT_t *pPort, uint8_t pinMask );

//-----------------------------------------------------------------------------

static inline void GPIO_SetPinsHigh( PORT_t *pPort, uint8_t pinMask );
static inline void GPIO_SetPinsLow( PORT_t *pPort, uint8_t pinMask );
static inline void GPIO_TogglePins( PORT_t *pPort, uint8_t pinMask );
static inline void GPIO_WriteOut( PORT_t *pPort, uint8_t pinMask );
static inline uint8_t GPIO_ReadOut( PORT_t *pPort );

//-----------------------------------------------------------------------------

static inline uint8_t GPIO_ReadPins( PORT_t *pPort );

//-----------------------------------------------------------------------------

#define PORT_INTLVL_OFF		PORT_INT0LVL_OFF_gc
#define PORT_INTLVL_LO		PORT_INT0LVL_LO_gc
#define PORT_INTLVL_MED		PORT_INT0LVL_MED_gc
#define PORT_INTLVL_HI		PORT_INT0LVL_HI_gc

/* intLevel argument constants */
//	PORT_INTLVL_OFF				/* Disable interrupt */
//	PORT_INTLVL_LO				/* Enable low level interrupt */
//	PORT_INTLVL_MED				/* Enable medium level interrupt */
//	PORT_INTLVL_HI				/* Enable high level interrupt */

static inline void GPIO_CfgInt0( PORT_t *pPort, uint8_t intLevel );
static inline void GPIO_DisableInt0( PORT_t *pPort );
static inline void GPIO_CfgInt1( PORT_t *pPort, uint8_t intLevel );
static inline void GPIO_DisableInt1( PORT_t *pPort );

static inline void GPIO_SetInt0PinMask( PORT_t *pPort, uint8_t pinMask );
static inline void GPIO_ClrInt0PinMask( PORT_t *pPort, uint8_t pinMask );
static inline uint8_t GPIO_GetInt0PinMask( PORT_t *pPort );
static inline void GPIO_SetInt1PinMask( PORT_t *pPort, uint8_t pinMask );
static inline void GPIO_ClrInt1PinMask( PORT_t *pPort, uint8_t pinMask );
static inline uint8_t GPIO_GetInt1PinMask( PORT_t *pPort );

static inline uint8_t GPIO_GetIntFlags( PORT_t *pPort );
static inline void GPIO_ClrIntFlags( PORT_t *pPort, uint8_t intFMask );

//=============================================================================

static inline void GPIO_DirInput( PORT_t *pPort, uint8_t pinMask )
{
	pPort->DIRCLR = pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_DirOutput( PORT_t *pPort, uint8_t pinMask )
{
	pPort->DIRSET = pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_ToggleDir( PORT_t *pPort, uint8_t pinMask )
{
	pPort->DIRTGL = pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_WriteDir( PORT_t *pPort, uint8_t pinMask )
{
	pPort->DIR = pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_SetPinsHigh( PORT_t *pPort, uint8_t pinMask )
{
	pPort->OUTSET = pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_SetPinsLow( PORT_t *pPort, uint8_t pinMask )
{
	pPort->OUTCLR = pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_TogglePins( PORT_t *pPort, uint8_t pinMask )
{
	pPort->OUTTGL = pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_WriteOut( PORT_t *pPort, uint8_t pinMask )
{
	pPort->OUT = pinMask;
}
//-----------------------------------------------------------------------------

static inline uint8_t GPIO_ReadOut( PORT_t *pPort )
{
	return pPort->OUT;
}
//-----------------------------------------------------------------------------

static inline uint8_t GPIO_ReadPins( PORT_t *pPort )
{
	return pPort->IN;
}
//-----------------------------------------------------------------------------

static inline void GPIO_CfgInt0( PORT_t *pPort, uint8_t intLevel )
{
	pPort->INTCTRL = (pPort->INTCTRL & ~PORT_INT0LVL_gm) | intLevel;
}
//-----------------------------------------------------------------------------

static inline void GPIO_DisableInt0( PORT_t *pPort )
{
	pPort->INTCTRL = (pPort->INTCTRL & ~PORT_INT0LVL_gm);
}
//-----------------------------------------------------------------------------

static inline void GPIO_CfgInt1( PORT_t *pPort, uint8_t intLevel )
{
	pPort->INTCTRL = (pPort->INTCTRL & ~PORT_INT1LVL_gm) | (intLevel << 2);
}
//-----------------------------------------------------------------------------

static inline void GPIO_DisableInt1( PORT_t *pPort )
{
	pPort->INTCTRL = (pPort->INTCTRL & ~PORT_INT1LVL_gm);
}
//-----------------------------------------------------------------------------

static inline void GPIO_SetInt0PinMask( PORT_t *pPort, uint8_t pinMask )
{
	pPort->INT0MASK	|= pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_ClrInt0PinMask( PORT_t *pPort, uint8_t pinMask )
{
	pPort->INT0MASK	&= ~pinMask;
}
//-----------------------------------------------------------------------------

static inline uint8_t GPIO_GetInt0PinMask( PORT_t *pPort )
{
	return pPort->INT0MASK;
}
//-----------------------------------------------------------------------------

static inline void GPIO_SetInt1PinMask( PORT_t *pPort, uint8_t pinMask )
{
	pPort->INT1MASK	|= pinMask;
}
//-----------------------------------------------------------------------------

static inline void GPIO_ClrInt1PinMask( PORT_t *pPort, uint8_t pinMask )
{
	pPort->INT1MASK	&= ~pinMask;
}
//-----------------------------------------------------------------------------

static inline uint8_t GPIO_GetInt1PinMask( PORT_t *pPort )
{
	return pPort->INT1MASK;
}
//-----------------------------------------------------------------------------

static inline uint8_t GPIO_GetIntFlags( PORT_t *pPort )
{
	return pPort->INTFLAGS;
}
//-----------------------------------------------------------------------------

static inline void GPIO_ClrIntFlags( PORT_t *pPort, uint8_t intFMask )
{
	pPort->INTFLAGS |= intFMask;
}
//-----------------------------------------------------------------------------


#endif /* GPIO_H_INCLUDED */