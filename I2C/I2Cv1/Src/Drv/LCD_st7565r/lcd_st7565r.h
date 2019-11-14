
#ifndef LCD_ST7565R_H_INCLUDED
#define LCD_ST7565R_H_INCLUDED

#include <stdint.h>
#include "LCD_st7565r/lcd_st7565r_drv.h"

//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------

/*
 * Defining LCD_ST7565R_NO_SMALL_FONT, LCD_ST7565R_NO_MEDIUM_FONT or LCD_ST7565R_NO_LARGE_FONT
 * causes that then corresponding font matrix will NOT be loaded.
 * This feature may be used to save SRAM.
 */

#if (defined(LCD_ST7565R_NO_SMALL_FONT) && defined(LCD_ST7565R_NO_MEDIUM_FONT) && defined(LCD_ST7565R_NO_LARGE_FONT))
#define LCD_ST7565R_NO_FONT
#warning "No font defined. Text (character) writing functions will not work."
#endif

//-----------------------------------------------------------------------------

/*
 * LCD_ST7565R_DRAWLINE_ALG selects the algorithm of drawing the line.
 * 1 - selects generic algorithm from ASF library 
 * 2 - selects M. McMillan's algorithm
 * Setting the invalid value (other than 1 or 2) selects algorithm 1
 */

#define LCD_ST7565R_DRAWLINE_ALG	2

#if ((LCD_ST7565R_DRAWLINE_ALG != 1) && (LCD_ST7565R_DRAWLINE_ALG != 2))
#define LCD_ST7565R_DRAWLINE_ALG	1
#endif

//-----------------------------------------------------------------------------

typedef enum 
{
	lcd_st7565r_font_small = 0,
	lcd_st7565r_font_medium = 1,
	lcd_st7565r_font_large = 2
} lcd_st7565r_FontSize_t;

//-----------------------------------------------------------------------------

void lcd_st7565r_InitInterface( void );
void lcd_st7565r_InitCtrl( void );
static inline uint8_t lcd_st7565r_SetContrast( uint8_t contrast );

//-----------------------------------------------------------------------------

void lcd_st7565r_ClrScr( uint8_t color );

//-----------------------------------------------------------------------------

void lcd_st7565r_DrawPixel( uint8_t x, uint8_t y, uint8_t color );

void lcd_st7565r_DrawHorizLine( uint8_t x0, uint8_t x1, uint8_t y, uint8_t color );
void lcd_st7565r_DrawVertLine( uint8_t x, uint8_t y0, uint8_t y1, uint8_t color );
void lcd_st7565r_DrawLine( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color );
void lcd_st7565r_DrawRectangle( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, bool fill );

//void lcd_st7565r_InitText( void );
int8_t lcd_st7565r_SelectFont( lcd_st7565r_FontSize_t font );
uint8_t lcd_st7565r_CharHeight( void );
uint8_t lcd_st7565r_CharWidth( void );
uint8_t lcd_st7565r_DrawChar( char c, uint8_t color );
void lcd_st7565r_DrawText( const char *txt, uint8_t color );

void lcd_st7565r_GotoTextXY( uint8_t x, uint8_t y );
static inline uint8_t lcd_st7565r_GetTextX( void );
static inline uint8_t lcd_st7565r_GetTextY( void );

//=============================================================================

static inline uint8_t lcd_st7565r_SetContrast( uint8_t contrast )
{
	return st7565r_set_contrast( contrast );
}

//-----------------------------------------------------------------------------

extern uint8_t		lcd_st7565r_text_x, lcd_st7565r_text_y;

static inline uint8_t lcd_st7565r_GetTextX( void )
{
	return lcd_st7565r_text_x;
}
//-----------------------------------------------------------------------------

static inline uint8_t lcd_st7565r_GetTextY( void )
{
	return lcd_st7565r_text_y;
}

//-----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif	/* LCD_ST7565R_H_INCLUDED */

