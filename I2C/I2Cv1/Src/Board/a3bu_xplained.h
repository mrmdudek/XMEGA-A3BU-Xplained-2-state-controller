/*
 * a3bu_xplained.h
 *
 * Created: 2017-05-10 19:58:27
 *  Author: Zbyszek
 */ 


#ifndef A3BU_XPLAINED_H_INCLUDED
#define A3BU_XPLAINED_H_INCLUDED

#include <avr/io.h>
#include "gpio/gpio.h"

//-----------------------------------------------------------------------------

// LCD screen

#define LCD_WIDTH				128
#define LCD_HEIGHT				32

// ST7565R controller USART SPI interface
#define ST7565R_CTRL_USART_SPI		USARTD0

// ST7565R controller Port and Pin mapping
#define ST7565R_CTRL_SCK_PORT		PORTD					// SPI SCK line
#define ST7565R_CTRL_SCK_PIN		GPIO_PIN_1

#define ST7565R_CTRL_MOSI_PORT		PORTD					// SPI MOSI line
#define ST7565R_CTRL_MOSI_PIN		GPIO_PIN_3

#define ST7565R_CTRL_SS_PORT		PORTF					// SPI SS line
#define ST7565R_CTRL_SS_PIN			GPIO_PIN_3

#define ST7565R_CTRL_RS_PORT		PORTD					// Register Select (RS) line
#define ST7565R_CTRL_RS_PIN			GPIO_PIN_0	
#define ST7565R_CTRL_A0_PORT		ST7565R_CTRL_RS_PORT	// Alternate names
#define ST7565R_CTRL_A0_PIN			ST7565R_CTRL_RS_PIN

#define ST7565R_CTRL_RST_PORT		PORTA					// Hardware reset line
#define ST7565R_CTRL_RST_PIN		GPIO_PIN_3

// Backlight Port and Pin mapping
#define LCD_BACKLIGHT_PORT			PORTE					// Backlight control line
#define LCD_BACKLIGHT_PIN			GPIO_PIN_4
#define LCD_BACKLIGHT_TIMER			TCE1

static inline void LCD_Backlight_CfgPin( void );

static inline void LCD_Backlight_On( void );
static inline void LCD_Backlight_Off( void );
static inline void LCD_Backlight_Toggle( void );

void LCD_Backlight_CfgPWM( void );
void LCD_Backlight_Intensity( uint8_t intensity );

//-----------------------------------------------------------------------------

#define SW0_PORT				PORTE
#define SW0_PIN					GPIO_PIN_5

#define SW1_PORT				PORTF
#define SW1_PIN					GPIO_PIN_1

#define SW2_PORT				PORTF
#define SW2_PIN					GPIO_PIN_2

//-----------------------------------------------------------------------------

#define LED01_PORT				PORTR

#define LED0_PORT				LED01_PORT
#define LED0_PIN				GPIO_PIN_0

#define LED1_PORT				LED01_PORT
#define LED1_PIN				GPIO_PIN_1

#define LED2RG_PORT				PORTD

#define LED2R_PORT				LED2RG_PORT
#define LED2R_PIN				GPIO_PIN_4

#define LED2G_PORT				LED2RG_PORT
#define LED2G_PIN				GPIO_PIN_5

//-----------------------------------------------------------------------------

static inline void LCD_Backlight_CfgPin( void )
{
	GPIO_CfgPins( &LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN, PORT_OPC_TOTEM_gc, 1, 0, 0 );
}
//-----------------------------------------------------------------------------

static inline void LCD_Backlight_Off( void )
{
	GPIO_SetPinsLow( &LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN );
}
//-----------------------------------------------------------------------------

static inline void LCD_Backlight_On( void )
{
	GPIO_SetPinsHigh( &LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN );
}
//-----------------------------------------------------------------------------

static inline void LCD_Backlight_Toggle( void )
{
	GPIO_TogglePins( &LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN );
}
//-----------------------------------------------------------------------------


#endif /* A3BU_XPLAINED_H_INCLUDED */