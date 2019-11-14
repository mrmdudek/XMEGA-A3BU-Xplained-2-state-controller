/*
 * a3bu_xplained.c
 *
 * Created: 2017-05-16 21:41:50
 *  Author: Zbyszek
 */ 

#include "a3bu_xplained.h"

//-----------------------------------------------------------------------------

void LCD_Backlight_CfgPWM( void )
{
	LCD_BACKLIGHT_TIMER.CTRLB = TC_WGMODE_SINGLESLOPE_gc;
	LCD_BACKLIGHT_TIMER.CTRLC = 0;
	LCD_BACKLIGHT_TIMER.CTRLD = 0;
	LCD_BACKLIGHT_TIMER.CTRLE = 0;

	LCD_BACKLIGHT_TIMER.INTCTRLA = 0;
	LCD_BACKLIGHT_TIMER.INTCTRLB = 0;

	LCD_BACKLIGHT_TIMER.PERBUF = 255;
	LCD_BACKLIGHT_TIMER.CCABUF = 255;

	LCD_BACKLIGHT_TIMER.CTRLA = TC_CLKSEL_DIV256_gc;
}
//-----------------------------------------------------------------------------

void LCD_Backlight_Intensity( uint8_t intensity )
{
	if( intensity == 0 )
	{
		LCD_BACKLIGHT_TIMER.CTRLB &= ~TC1_CCAEN_bm;
		GPIO_SetPinsLow( &LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN );
	}
	else if( intensity == 255 )
	{
		LCD_BACKLIGHT_TIMER.CTRLB &= ~TC1_CCAEN_bm;
		GPIO_SetPinsHigh( &LCD_BACKLIGHT_PORT, LCD_BACKLIGHT_PIN );
	}
	else
	{
		LCD_BACKLIGHT_TIMER.CCABUF = intensity;
		LCD_BACKLIGHT_TIMER.CTRLB |= TC1_CCAEN_bm;
	}
}
//-----------------------------------------------------------------------------
