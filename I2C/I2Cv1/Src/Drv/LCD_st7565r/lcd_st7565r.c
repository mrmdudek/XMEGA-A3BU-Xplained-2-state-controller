

#include "LCD_st7565r/lcd_st7565r.h"

//-----------------------------------------------------------------------------

typedef struct
{
	uint8_t		chWidth, chHeight;
	char		chFirst, chLast;
	uint8_t		*chMatrix;
} lcd_st7565r_Font_t;

//-----------------------------------------------------------------------------

#ifndef LCD_ST7565R_NO_SMALL_FONT
#include "LCD_st7565r/lcd_st7565r_font_6_7.inc"
#endif

#ifndef LCD_ST7565R_NO_MEDIUM_FONT
#include "LCD_st7565r/lcd_st7565r_font_6_10.inc"
#endif

#ifndef LCD_ST7565R_NO_LARGE_FONT
#include "LCD_st7565r/lcd_st7565r_font_10_15.inc"
#endif

//-----------------------------------------------------------------------------

void lcd_st7565r_InitText( void );

//-----------------------------------------------------------------------------

void lcd_st7565r_InitCtrl( void )
{
	st7565r_init( );
	lcd_st7565r_InitText( );
}

//-----------------------------------------------------------------------------

void lcd_st7565r_InitInterface( void )
{
	GPIO_CfgPins( &ST7565R_CTRL_RST_PORT, ST7565R_CTRL_RST_PIN, PORT_OPC_TOTEM_gc, 1, 1, 0 );

	GPIO_CfgPins( &ST7565R_CTRL_A0_PORT, ST7565R_CTRL_A0_PIN, PORT_OPC_TOTEM_gc, 1, 0, 0 );

	USART_CfgSpiSSPin( &ST7565R_CTRL_SS_PORT, ST7565R_CTRL_SS_PIN );
	USART_CfgSpiSCKPin( &ST7565R_CTRL_SCK_PORT, ST7565R_CTRL_SCK_PIN, ST7565R_SPI_MODE );
	USART_CfgSpiMOSIPin( &ST7565R_CTRL_MOSI_PORT, ST7565R_CTRL_MOSI_PIN );

//	void USART_CfgSpiMode( USART_t *pUSART, uint8_t spiSckMode, uint8_t spiDORD );
//	void USART_CfgBaudRate( USART_t *pUSART, bool dblSpeed, uint16_t brSel, int8_t brScaleFactor );

//	USART_CfgSpiMode( &ST7565R_CTRL_USART_SPI, ST7565R_SPI_MODE, USART_SPI_DORD_MSB );
//	USART_CfgBaudRate( &ST7565R_CTRL_USART_SPI, false, 7, 1 );
	
//	uint8_t USART_SelectMode( USART_t *pUSART, uint8_t mode );
//	void USART_CfgSpiParam( USART_t *pUSART, uint8_t spiSckMode, uint8_t spiDORD );
//	uint8_t USART_SetBaudRate( USART_t *pUSART, uint8_t clk2x, uint32_t baud, uint32_t cpuFreq );

	USART_SelectMode( &ST7565R_CTRL_USART_SPI, USART_CMODE_MSPI_gc );
	USART_CfgSpiParam( &ST7565R_CTRL_USART_SPI, ST7565R_SPI_MODE, USART_SPI_DORD_MSB );
	USART_SetBaudRate( &ST7565R_CTRL_USART_SPI, 0, ST7565R_CTRL_CLOCK_SPEED, F_CPU );

	USART_TxEnable( &ST7565R_CTRL_USART_SPI );
	USART_RxEnable( &ST7565R_CTRL_USART_SPI );
}

//-----------------------------------------------------------------------------

void lcd_st7565r_ClrScr( uint8_t color )
{
	uint16_t		i;
	uint8_t			pattern = 0;

	if( color == LCD_PIXEL_SET )
		pattern = 0xFF;

	for( i = 0; i < ST7565R_FRAMEBUFFER_SIZE; i++ )
		st7565r_FrameBuffer[i] = pattern;

	st7565r_put_framebuffer( 0, ST7565R_PAGES_PER_COLUMN - 1, 0, LCD_WIDTH - 1 );
}

//-----------------------------------------------------------------------------

void lcd_st7565r_DrawPixel( uint8_t x, uint8_t y, uint8_t color)
{
	uint8_t page;
	uint8_t pixel_mask;
	uint8_t byte_value;

/* Discard pixels drawn outside the screen */
	if( (x > LCD_WIDTH - 1) || (y > LCD_HEIGHT - 1) )
		return;

	page = y / ST7565R_PIXELS_PER_BYTE;
//	pixel_mask = (1 << (y - (page * 8)));
	pixel_mask = (1 << (y & 0x07));

	byte_value = st7565r_modify_page( page, x, pixel_mask, color );

	st7565r_set_page_address( page );
	st7565r_set_column_address( x );
	st7565r_write_data( byte_value );
}

//-----------------------------------------------------------------------------

void lcd_st7565r_DrawHorizLine( uint8_t x0, uint8_t x1, uint8_t y, uint8_t color )
{
	uint8_t page;
	uint8_t pixel_mask;
	uint8_t byte_value;

/* Discard pixels drawn outside the screen */
	if( (y > LCD_HEIGHT - 1) || ((x0 > LCD_WIDTH - 1) && (x1 > LCD_WIDTH - 1)) )
		return;

	if( x0 > LCD_WIDTH - 1 )
		x0 = LCD_WIDTH - 1;

	if( x1 > LCD_WIDTH - 1 )
		x1 = LCD_WIDTH - 1;

	if( x0 > x1 )
	{
		page = x0;
		x0 = x1;
		x1 = page;
	}
	
	page = y / ST7565R_PIXELS_PER_BYTE;
//	pixel_mask = (1 << (y - (page * 8)));
	pixel_mask = (1 << (y & 0x07));

	st7565r_set_page_address( page );
	st7565r_set_column_address( x0 );

	for( ; x0 <= x1; x0++ )
	{
		byte_value = st7565r_modify_page( page, x0, pixel_mask, color );
		st7565r_write_data( byte_value );
	}
}

//-----------------------------------------------------------------------------

void lcd_st7565r_DrawVertLine( uint8_t x, uint8_t y0, uint8_t y1, uint8_t color )
{
	uint8_t page0, page1, page;
	uint8_t pixel_mask;
	uint8_t byte_value;

/* Discard pixels drawn outside the screen */
	if( (x > LCD_WIDTH - 1) || ((y0 > LCD_HEIGHT - 1) && (y1 > LCD_HEIGHT - 1)) )
		return;

	if( y0 > LCD_HEIGHT - 1 )
		y0 = LCD_HEIGHT - 1;

	if( y1 > LCD_HEIGHT - 1 )
		y1 = LCD_HEIGHT - 1;

	if( y0 > y1 )
	{
		page = y0;
		y0 = y1;
		y1 = page;
	}
	
	page0 = y0 / ST7565R_PIXELS_PER_BYTE;
	page1 = y1 / ST7565R_PIXELS_PER_BYTE;
	
	for( page = page0; page <= page1; page++ )
	{
		if( page == page0 )
			pixel_mask = 0xFF << (y0 & 0x07);
		else if( page == page1 )
			pixel_mask = 0xFF >> (7 - (y1 & 0x07)) ;
		else
			pixel_mask = 0xFF;

		byte_value = st7565r_modify_page( page, x, pixel_mask, color );

		st7565r_set_page_address( page );
		st7565r_set_column_address( x );
		st7565r_write_data( byte_value );
	}
}

//-----------------------------------------------------------------------------

#if (LCD_ST7565R_DRAWLINE_ALG == 1)

/*
 * LCDDrawLine.c
 *
 * \brief Draw a line between two arbitrary points (generic implementation from ASF).
 *
 * \param  x1          Start X coordinate.
 * \param  y1          Start Y coordinate.
 * \param  x2          End X coordinate.
 * \param  y2          End Y coordinate.
 * \param  color       Pixel operation of the line.
 */

void lcd_st7565r_DrawLine( uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color )
{
	uint8_t	i;
	uint8_t	x, y;
	int8_t	xinc, yinc;
	int8_t	dx, dy;
	int8_t	e;

/* swap x1,y1  with x2,y2 */
	if( x1 > x2 )
	{
		dx = x1;
		x1 = x2;
		x2 = dx;
		dy = y1;
		y1 = y2;
		y2 = dy;
	}

	dx = x2 - x1;
	dy = y2 - y1;

	x = x1;
	y = y1;

	if( dx < 0 )
	{
		xinc = -1;
		dx = -dx;
	} 
	else 
	{
		xinc = 1;
	}

//	xinc = 1;

	if( dy < 0 ) 
	{
		yinc = -1;
		dy = -dy;
	} 
	else 
	{
		yinc = 1;
	}

	if(dx > dy) 
	{
		e = dy - dx;
		for( i = 0; i <= dx; i++ ) 
		{
			lcd_st7565r_DrawPixel( x, y, color );
			if(e >= 0) 
			{
				e -= dx;
				y += yinc;
			}
			e += dy;
			x += xinc;
		}
	} 
	else 
	{
		e = dx - dy;
		for( i = 0; i <= dy; i++ ) 
		{
			lcd_st7565r_DrawPixel( x, y, color );
			if( e >= 0 ) 
			{
				e -= dy;
				x += xinc;
			}
			e += dx;
			y += yinc;
		}
	}
}

#endif

//-----------------------------------------------------------------------------

#if (LCD_ST7565R_DRAWLINE_ALG == 2)

/*
 * LCDDrawLine.c
 *
 * Draws a line in the specified color from (x0,y0) to (x1,y1)
 *
 * Note: good write-up on this algorithm in Wikipedia (search for Bresenham's line algorithm)
 *
 * Authors: Dr. Leonard McMillan, Associate Professor UNC
 * Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962)
 *
 */

void lcd_st7565r_DrawLine( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color )
{
	int16_t		dx, dy;
	int8_t		stepx, stepy;
	int16_t		fraction;

	dx = (int8_t)x1 - (int8_t)x0;
	dy = (int8_t)y1 - (int8_t)y0;

	if( dy < 0 )
	{
		dy = -dy; stepy = -1;
	}
	else stepy = 1;

	if( dx < 0 )
	{
		dx = -dx; stepx = -1;
	}
	else stepx = 1;

	lcd_st7565r_DrawPixel( x0, y0, color );

	dy <<= 1;							// dy is now 2*dy
	dx <<= 1;							// dx is now 2*dx

	if( dx > dy )
	{
		fraction = dy - (dx >> 1);		// same as 2*dy - dx
		while( x0 != x1 )
		{
			if( fraction >= 0 )
			{
				y0 += stepy;
				fraction -= dx;			// same as fraction -= 2*dx
			}
			x0 += stepx;
			fraction += dy;				// same as fraction -= 2*dy
			lcd_st7565r_DrawPixel( x0, y0, color );
		}
	}
	else
	{
		fraction = dx - (dy >> 1);
		while( y0 != y1 )
		{
			if( fraction >= 0 )
			{
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;
			lcd_st7565r_DrawPixel( x0, y0, color );
		}
	}
}

#endif

//-----------------------------------------------------------------------------

void lcd_st7565r_DrawRectangle( uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, bool fill )
{
	uint8_t	x;

	if( x0 > x1 )
	{
		x = x0;
		x0 = x1;
		x1 = x;
	}

	if( y0 > y1 )
	{
		x = y0;
		y0 = y1; 
		y1 = x;
	}

	if( (x0 > LCD_WIDTH - 1) || (y0 > LCD_HEIGHT - 1) )
		return;

	if( fill )
	{
		for( x = x0; x <= x1; x++ )
			lcd_st7565r_DrawVertLine( x, y0, y1, color );
	}
	else
	{
		lcd_st7565r_DrawHorizLine( x0, x1, y0, color );
		lcd_st7565r_DrawHorizLine( x0, x1, y1, color );
		lcd_st7565r_DrawVertLine( x0, y0, y1, color );
		lcd_st7565r_DrawVertLine( x1, y0, y1, color );
	}

}

//=============================================================================

lcd_st7565r_Font_t		*lcd_st7565r_curFont;
uint8_t					lcd_st7565r_text_x, lcd_st7565r_text_y;

//-----------------------------------------------------------------------------

void lcd_st7565r_InitText( void )
{
	lcd_st7565r_text_x = 0;
	lcd_st7565r_text_y = 0;

#if !defined(LCD_ST7565R_NO_SMALL_FONT)

	lcd_st7565r_curFont = &lcd_st7565r_font_6_7;

#elif !defined(LCD_ST7565R_NO_MEDIUM_FONT)

	lcd_st7565r_curFont = &lcd_st7565r_font_6_10;

#elif !defined(LCD_ST7565R_NO_LARGE_FONT)

	lcd_st7565r_curFont = &lcd_st7565r_font_10_15;

#endif

}
//-----------------------------------------------------------------------------

int8_t lcd_st7565r_SelectFont( lcd_st7565r_FontSize_t font )
{
	switch( font )
	{

#ifndef LCD_ST7565R_NO_SMALL_FONT

	case lcd_st7565r_font_small:
		lcd_st7565r_curFont = &lcd_st7565r_font_6_7;
		return (int8_t)lcd_st7565r_font_small;
#endif

#ifndef LCD_ST7565R_NO_MEDIUM_FONT

	case lcd_st7565r_font_medium:
		lcd_st7565r_curFont = &lcd_st7565r_font_6_10;
		return (int8_t)lcd_st7565r_font_medium;
#endif
		
#ifndef LCD_ST7565R_NO_LARGE_FONT

	case lcd_st7565r_font_large:
		lcd_st7565r_curFont = &lcd_st7565r_font_10_15;
		return (int8_t)lcd_st7565r_font_large;
#endif

	default:
		return -1;
	}
}

//-----------------------------------------------------------------------------

void lcd_st7565r_GotoTextXY( uint8_t x, uint8_t y)
{
	if( x < LCD_WIDTH )
		lcd_st7565r_text_x = x;
	if( y < LCD_HEIGHT )
		lcd_st7565r_text_y = y;
}

//-----------------------------------------------------------------------------

uint8_t lcd_st7565r_CharHeight( void )
{
	return lcd_st7565r_curFont->chHeight;
}

//-----------------------------------------------------------------------------

uint8_t lcd_st7565r_CharWidth( void )
{
	return lcd_st7565r_curFont->chWidth;
}

//-----------------------------------------------------------------------------

#if !defined(LCD_ST7565R_NO_FONT)

uint8_t lcd_st7565r_DrawChar( char c, uint8_t color )
{
	uint8_t		page0, page1;
	uint8_t		x1, y1, x;
	uint32_t	col_value, fgnd_mask, bgnd_mask;
	uint8_t		mask_shift;
	uint16_t	matrix_idx;

	if( (c < lcd_st7565r_curFont->chFirst) || (c > lcd_st7565r_curFont->chLast) )
		return 1;

	x1 = lcd_st7565r_text_x + lcd_st7565r_curFont->chWidth - 1;
	y1 = lcd_st7565r_text_y + lcd_st7565r_curFont->chHeight - 1;

	if( y1 > LCD_HEIGHT - 1 )
		return 2;

	if( x1 > LCD_WIDTH - 1 )
		return 3;

	page0 = lcd_st7565r_text_y / ST7565R_PIXELS_PER_BYTE;
	page1 = y1 / ST7565R_PIXELS_PER_BYTE;
	mask_shift = lcd_st7565r_text_y % ST7565R_PIXELS_PER_BYTE;

	matrix_idx = (c - lcd_st7565r_curFont->chFirst) * (lcd_st7565r_curFont->chHeight / 8 + 1) * lcd_st7565r_curFont->chWidth;

	bgnd_mask = 0xFFFFFFFF >> ((uint8_t)32 - lcd_st7565r_curFont->chHeight);
	bgnd_mask <<= mask_shift;

	for( x = lcd_st7565r_text_x; x <= x1; x++ )
	{
		col_value = st7565r_read_column( page0, page1, x );
		
		fgnd_mask = lcd_st7565r_curFont->chMatrix[matrix_idx];
		matrix_idx++;
		if( lcd_st7565r_curFont->chHeight > 8 )
		{
			fgnd_mask |= (uint16_t)lcd_st7565r_curFont->chMatrix[matrix_idx] << 8;
			matrix_idx++;
		}
		fgnd_mask <<= mask_shift;

		switch (color)
		{
		case LCD_PIXEL_CLR:
			col_value |= bgnd_mask;
			col_value &= ~fgnd_mask;
			break;
		case LCD_PIXEL_SET:
			col_value &= ~bgnd_mask;
			col_value |= fgnd_mask;
			break;
		case LCD_PIXEL_TGL:
			col_value ^= bgnd_mask;
			col_value ^= fgnd_mask;
			break;
		default:
			break;
		}
		st7565r_write_column( page0, page1, x, col_value );
	}

	st7565r_put_framebuffer( page0, page1, lcd_st7565r_text_x, x1 );

	lcd_st7565r_text_x += lcd_st7565r_curFont->chWidth;
	
	return 0;
}

//-----------------------------------------------------------------------------

void lcd_st7565r_DrawText( const char *txt, uint8_t color )
{
	int8_t		ret_value;

	while( *txt != '\0' )
	{
		if( *txt == '\r' )
			lcd_st7565r_text_x = 0;
		else if( *txt == '\n' )
			lcd_st7565r_text_y += lcd_st7565r_curFont->chHeight;
		else
		{
			ret_value = lcd_st7565r_DrawChar( *txt, color );
			if( ret_value == 3 )
			{
				lcd_st7565r_text_x = 0;
				lcd_st7565r_text_y += lcd_st7565r_curFont->chHeight;
				txt--;
			}
		}
		txt++;
	}
}

//-----------------------------------------------------------------------------

#else	// !defined(LCD_ST7565R_NO_FONT)

uint8_t lcd_st7565r_DrawChar( char c, uint8_t color )
{
	return 4;
}
//-----------------------------------------------------------------------------

void lcd_st7565r_DrawText( const char *txt, uint8_t color )
{
	return;
}
//-----------------------------------------------------------------------------

#endif	// !defined(LCD_ST7565R_NO_FONT)

