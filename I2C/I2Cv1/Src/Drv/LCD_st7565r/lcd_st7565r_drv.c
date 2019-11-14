/**
 * \file
 *
 * \brief ST7565R display controller driver.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "lcd_st7565r/lcd_st7565r_drv.h"

//=============================================================================

uint8_t		st7565r_FrameBuffer[ST7565R_FRAMEBUFFER_SIZE];

//=============================================================================

#if 0
/**
 * \internal
 * \brief Initialize the hardware interface
 *
 * Depending on what interface used for interfacing the LCD controller this
 * function will initialize the necessary hardware.
 */

void LCD_InitInterface( void )
{
	GPIO_CfgPins( &LCD_CTRL_RST_PORT, LCD_CTRL_RST_PIN, GPIO_OMODE_TOTEM, GPIO_DIR_OUTPUT_1, 0 );

	GPIO_CfgPins( &LCD_CTRL_A0_PORT, LCD_CTRL_A0_PIN, GPIO_OMODE_TOTEM, GPIO_DIR_OUTPUT_0, 0 );

	USART_CfgSpiSSPin( &LCD_CTRL_SS_PORT, LCD_CTRL_SS_PIN );
	USART_CfgSpiSCKPin( &LCD_CTRL_SCK_PORT, LCD_CTRL_SCK_PIN, ST7565R_SPI_MODE );
	USART_CfgSpiMOSIPin( &LCD_CTRL_MOSI_PORT, LCD_CTRL_MOSI_PIN );

//	void USART_CfgSpiMode( USART_t *pUSART, uint8_t spiSckMode, uint8_t spiDORD );
//	void USART_CfgBaudRate( USART_t *pUSART, bool dblSpeed, uint16_t brSel, int8_t brScaleFactor );

	USART_CfgSpiMode( &LCD_CTRL_USART_SPI, ST7565R_SPI_MODE, USART_SPI_DORD_MSB );
	USART_CfgBaudRate( &LCD_CTRL_USART_SPI, false, 7, 1 );
	
	USART_TxEnable( &LCD_CTRL_USART_SPI );
	USART_RxEnable( &LCD_CTRL_USART_SPI );
}
#endif

/**
 * \brief Initialize the LCD controller
 *
 * Call this function to initialize the hardware interface and the LCD
 * controller. When initialization is done the display is turned on and ready
 * to receive data.
 */
void st7565r_init( void )
{
// Do a hard reset of the LCD display controller
	st7565r_hard_reset( );

// Initialize the interface
//	st7565r_interface_init();

// Set the A0 pin to the default state (command)
//	t_pin_low(ST7565R_A0_PIN);
	GPIO_SetPinsLow( &ST7565R_CTRL_A0_PORT, ST7565R_CTRL_A0_PIN );

// The column address is set to increasing
	st7565r_write_command( ST7565R_CMD_ADC_NORMAL );

// Non-inverted display
	st7565r_display_invert_disable( );

// The common mode scan direction is reversed COM31->COM0
	st7565r_write_command( ST7565R_CMD_REVERSE_SCAN_DIRECTION );

// Set the voltage bias ratio to 1/6
	st7565r_write_command( ST7565R_CMD_LCD_BIAS_1_DIV_6_DUTY33 );

// Set booster circuit, voltage regulator and voltage follower all to on
	st7565r_write_command( ST7565R_CMD_POWER_CTRL_ALL_ON );

// Set the booster ratio to 2X,3X,4X
	st7565r_write_command( ST7565R_CMD_BOOSTER_RATIO_SET );
	st7565r_write_command( ST7565R_CMD_BOOSTER_RATIO_2X_3X_4X );

// Set voltage resistor ratio to 1
	st7565r_write_command( ST7565R_CMD_VOLTAGE_RESISTOR_RATIO_1 );

// Set contrast to min value, no need to check return value as the contrast
// is set to the defined minimum
	st7565r_set_contrast( ST7565R_DISPLAY_CONTRAST_MIN );

// Turn on the display
	st7565r_display_on( );

// Set display to output data from line 0
	st7565r_set_display_start_line_address( 0 );
}
//-----------------------------------------------------------------------------

uint8_t st7565r_set_contrast( uint8_t contrast )
{
	if( contrast < ST7565R_DISPLAY_CONTRAST_MIN )
		contrast = ST7565R_DISPLAY_CONTRAST_MIN;

	if ( contrast > ST7565R_DISPLAY_CONTRAST_MAX ) 
		contrast = ST7565R_DISPLAY_CONTRAST_MAX;

	st7565r_write_command( ST7565R_CMD_ELECTRONIC_VOLUME_MODE_SET );
	st7565r_write_command( ST7565R_CMD_ELECTRONIC_VOLUME(contrast) );

	return contrast;
}
//-----------------------------------------------------------------------------

void st7565r_put_framebuffer( uint8_t page0, uint8_t page1, uint8_t x0, uint8_t x1 )
{
	uint16_t	x_ofs, x_ofsMax;

	while( page0 <= page1 )
	{
		st7565r_set_page_address( page0 );
		st7565r_set_column_address( x0 );
		x_ofs = (uint16_t)page0 * LCD_WIDTH + x0;
		x_ofsMax = x_ofs + x1;
		for( ; x_ofs <= x_ofsMax; x_ofs++ )
			st7565r_write_data( st7565r_FrameBuffer[x_ofs] );
		page0++;
	}
}
//-----------------------------------------------------------------------------

uint8_t st7565r_modify_page( uint8_t page, uint8_t x, uint8_t pixel_mask, uint8_t color )
{
	uint16_t	byte_idx;
	uint8_t		byte_value;

	byte_idx = (uint16_t)page * LCD_WIDTH + (uint16_t)x;
	byte_value = st7565r_FrameBuffer[byte_idx];

	switch (color)
	{
		case LCD_PIXEL_CLR:
			byte_value &= ~pixel_mask;
			break;
		case LCD_PIXEL_SET:
			byte_value |= pixel_mask;
			break;
		case LCD_PIXEL_TGL:
			byte_value ^= pixel_mask;
			break;
		default:
			break;
	}
	
	st7565r_FrameBuffer[byte_idx] = byte_value;

	return byte_value;
}

//-----------------------------------------------------------------------------

uint32_t st7565r_read_column( uint8_t page0, uint8_t page1, uint8_t x )
{
	uint32_t	col_value;
	uint16_t	byte_idx;
	uint8_t		shift_cnt;

	col_value = 0;
	shift_cnt = 0;
	byte_idx = (uint16_t)page0 * LCD_WIDTH + (uint16_t)x;

	while( page0 <= page1 )
	{
		col_value |= (uint32_t)(st7565r_FrameBuffer[byte_idx]) << shift_cnt;
		byte_idx += LCD_WIDTH;
		shift_cnt += 8;
		page0++;
	}

	return col_value;
}

//-----------------------------------------------------------------------------

void st7565r_write_column( uint8_t page0, uint8_t page1, uint8_t x, uint32_t column )
{
	uint16_t	byte_idx;

	byte_idx = (uint16_t)page0 * LCD_WIDTH + (uint16_t)x;

	while( page0 <= page1 )
	{
		st7565r_FrameBuffer[byte_idx] = (uint8_t)(column & 0xFF);
		byte_idx += LCD_WIDTH;
		column >>= 8;
		page0++;
	}
}

//-----------------------------------------------------------------------------

