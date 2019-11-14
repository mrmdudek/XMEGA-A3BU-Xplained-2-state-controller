//-----------------------------------------------------------------------------
//    KAmodCOMBO library for ATmega and ATxmega microcontrollers
//
//    Modified by ZM based on KAmodCOMBO library for ATmega32
//    Ver. 1.0 - 2017.03 
//    Ver. 2.0 - 2018.02
//-----------------------------------------------------------------------------
//
//    THE SOFTWARE INCLUDED IN THIS FILE IS FOR GUIDANCE ONLY.
//    BTC KORPORACJA SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
//    OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//    FROM USE OF THIS SOFTWARE.
//
//-----------------------------------------------------------------------------


#include "KAmodCOMBO/KAmodCOMBO.h"

//=============================================================================

extern uint8_t STLED316_7Seg_SegMap[];

//=============================================================================

// STLED316_DisplayOn
// This function turns display on, you MUST call it before STLED316 can display anything.

void STLED316_DisplayOn( void )
{
	uint8_t		Buffer = 0xD;
	
	STLED316_Write( &Buffer, 1 );
}

//=============================================================================

// STLED316_DisplayOff
// Turns display off.

void STLED316_DisplayOff( void )
{
	uint8_t		Buffer = 0xE;
	
	STLED316_Write( &Buffer, 1 );
}

//=============================================================================

// STLED316_LEDs_Write
// This function turns on/off LEDs depending on LEDs variable bits, bit 0 = 0 turns off D1, bit 0 = 1 turns on D1,
// bit 1 = 0 turns off D2, bit 1 = 1 turns on D2 and so on.

void STLED316_LEDs_Write( uint8_t LEDs )
{
	uint8_t		Buffer[2] = { 0x8, LEDs };
	
	STLED316_Write( Buffer, 2 );
}

//=============================================================================

// STLED316_SetBrghtCtrlModeAndDigitCnt
// Set brightness control mode
// Mode:
//   - STLED316_BRGHT_MODE_CONST - constant mode, all segments and LEDs will have equal brightness
//     passed in ConstBrightness  parameter (values 0..7)
//   - STLED316_BRGHT_MODE_VAR - brightness can be set individually
// DigitCount: 1..6 - number of enabled displays

void STLED316_SetBrghtCtrlModeAndDigitCnt( uint8_t BrghtCtrlMode, uint8_t ConstBrght, uint8_t DigitCount )
{
	uint8_t		Buffer[2] = { 0x10, BrghtCtrlMode | ((DigitCount > 0) & (DigitCount < 7) ? (DigitCount - 1) : 5) | ((ConstBrght & 0x07) << 5) };
	
	STLED316_Write( Buffer, 2 );
}

//=============================================================================

// STLED316_7Seg_Write
// This functions sets what LED display segments will be on.
// - DisplayNumber - number of display, values 0..5.
// - Segments - which segments will be on, combination of STLED316_xxx_SEG constants.
//   For example STLED316_RT_SEG | STLED316_RB_SEG gives '1' (both right segments on).

void STLED316_7Seg_Write( uint8_t DisplayNum, uint8_t Segments )
{
	uint8_t		Buffer[2] = { DisplayNum, Segments };

	STLED316_Write( Buffer, 2 );
}

//=============================================================================

// STLED316_7Seg_WriteTxt
// Writes text on 7-segment display starting from given 7-segment display.
// This function displays decimal digits (0 ... 9), hexadecimal digits (A ... F, a ... f),
// minus sign (-), decimal point (.) and space ( ).
// Other characters are displayed as 3 horizontal segments.
// - DisplayNum - 7-segment display number of the text beginning
// - Txt - pointer to the C-style text

void STLED316_7Seg_WriteTxt( uint8_t DisplayNum, const char *Txt )
{
	int8_t			idx;
	register char	c;
	uint8_t			digitMap;

	while( *Txt != '\0' &&  DisplayNum < 6 )
	{
		c = *Txt;
		if( c >= '0' && c <= '9' )
		idx = c - '0';
		else if( c >= 'A' && c <= 'F' )
		idx = c - 'A' + 10;
		else if( c >= 'a' && c <= 'f' )
		idx = c - 'a' + 10;
		else if( c == '-' )
		idx = 16;
		else if( c == ' ' )
		idx = 17;
		else
		idx = 18;
		digitMap = STLED316_7Seg_SegMap[idx];
		
		if( *(Txt + 1) == '.' )
		{
			digitMap |= STLED316_DOT_SEG;
			Txt++;
		}
		
		STLED316_7Seg_Write( DisplayNum, digitMap );
		
		DisplayNum--;
		Txt++;
	}
}

//=============================================================================

// STLED316_LEDsAndButs_Read
// Reads LED states (into LEDs) and buttons states (state of S1..S8 buttons in low byte and S9..S16 in high byte of Buttons variable)

void STLED316_LEDsAndButs_Read( uint8_t *LEDs, uint16_t *Buttons )
{
	uint8_t		Buffer[3];

	STLED316_Read( 0x48, Buffer, 3 );
	*LEDs = Buffer[0];
	*Buttons = ((uint16_t)Buffer[2] << 8) | Buffer[1];
}

//=============================================================================

// STLED316_Buts_Read
// Reads buttons states (state of S1..S8 buttons in low byte and S9..S16 in high byte)

uint16_t STLED316_Buts_Read( void )
{
	uint16_t		buttons;

	STLED316_Read( 0x49, (uint8_t*)(&buttons), 2 );
	
	return buttons;
}

//=============================================================================

// STLED316_LEDs_Read
// Reads D1..D8 LEDs states

uint8_t STLED316_LEDs_Read( void )
{
	uint8_t		LEDs;

	STLED316_Read( 0x48, &LEDs, 1 );
	
	return LEDs;
}

//=============================================================================

// STLED316_LEDs_SetBrghtInd
// BrightnessTab - 8-element array, every element is brightness of single LED, you should pass
// values in range 0..7. Do not expect that 0 turns off LED, it will be just less bright.

void STLED316_LEDs_SetBrghtInd( uint8_t *BrightnessTab )
{
	uint8_t		Buffer[5], i;
	
	Buffer[0] = 0x18;
	for( i = 0; i < 4; i++ )
		Buffer[i + 1] = (BrightnessTab[2 * i] & 0x07) | ((BrightnessTab[2 * i + 1] & 0x07) << 4);
	STLED316_Write( Buffer, 5 );
}

//=============================================================================

// STLED316_LEDs_SetBrght
// Brightness - brightness of all LEDs, you should pass value in range 0..7.
// Do not expect that 0 turns off LED, it will be just less bright.

void STLED316_LEDs_SetBrght( uint8_t Brightness )
{
	uint8_t		Buffer[5], tmp;
	int8_t		i;

	Buffer[0] = 0x18;
	tmp = ((Brightness & 0x07) << 4) | (Brightness & 0x07);
	for( i = 1; i < 5; i++ )
		Buffer[i] = tmp;
	STLED316_Write( Buffer, 5 );
}

//=============================================================================

// STLED316_LEDs_GetBrghtInd
// BrightnessTab - 8-element array, every element is brightness of single LED, you should pass
// values in range 0..7. Do not expect that 0 turns off LED, it will be just less bright.

void STLED316_LEDs_GetBrghtInd( uint8_t *BrightnessTab )
{
	int8_t		i;

	STLED316_Read( 0x58, BrightnessTab, 4 );

	for( i = 3; i >= 0; i-- )
	{
		BrightnessTab[2 * i + 1] = (BrightnessTab[i] >> 4) & 0x07;
		BrightnessTab[2 * i] = BrightnessTab[i] & 0x07;
	}
}

//=============================================================================

// STLED316_7Seg_SetBrghtInd
// BrightnessTab - 6-element array, every element is brightness of the single 7-segment display, you should pass
// values in range 0..7. Do not expect that 0 turns off LED, it will be just less bright.
//
// To use this function you need to call STLED316_SetBrghtCtrlModeAndDigitCnt(STLED316_BRGHT_MODE_VAR... before!!!

void STLED316_7Seg_SetBrghtInd( uint8_t *BrightnessTab )
{
	uint8_t		Buffer[4];
	int8_t		i;

	Buffer[0] = 0x11;
	for( i = 0; i < 3; i++ )
		Buffer[i + 1] = ((BrightnessTab[2 * i + 1] & 0x07) << 4) | (BrightnessTab[2 * i] & 0x07);
	
	STLED316_Write( Buffer, 4 );
}

//=============================================================================

// STLED316_7Seg_SetBrght
// Set brightness of all 7-segment displays.
// - Brightness - brightness values (0..7)
//
// To use this function you need to call STLED316_SetBrghtCtrlModeAndDigitCnt(STLED316_BRGHT_MODE_VAR... before!!!

void STLED316_7Seg_SetBrght( uint8_t Brightness )
{
	uint8_t		Buffer[4], tmp;
	int8_t		i;

	Buffer[0] = 0x11;
	tmp = ((Brightness & 0x07) << 4) | (Brightness & 0x07);
	for( i = 1; i < 4; i++ )
		Buffer[i] = tmp;
	STLED316_Write( Buffer, 4 );
}

//=============================================================================

// STLED316_7Seg_GetBrghtInd
// BrightnessTab - 6-element array, every element is brightness of single 7-segment display, you should pass
// values in range 0..7. Do not expect that 0 turns off LED, it will be just less bright.
//
// To use this function you need to call STLED316_SetBrghtCtrlModeAndDigitCnt(STLED316_BRGHT_MODE_VAR... before!!!

void STLED316_7Seg_GetBrghtInd( uint8_t *BrightnessTab )
{
	int8_t		i;

	STLED316_Read( 0x51, BrightnessTab, 3 );

	for( i = 2; i >= 0; i-- )
	{
		BrightnessTab[2 * i + 1] = (BrightnessTab[i] >> 4) & 0x07;
		BrightnessTab[2 * i] = BrightnessTab[i] & 0x07;
	}
}

//=============================================================================

uint8_t STLED316_7Seg_SegMap[] =
{
	( STLED316_TOP_SEG | STLED316_BOT_SEG | STLED316_LT_SEG | STLED316_LB_SEG | STLED316_RT_SEG | STLED316_RB_SEG ),						// 0
	( STLED316_RT_SEG  | STLED316_RB_SEG ),																									// 1
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_LB_SEG | STLED316_RT_SEG ),											// 2
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_RT_SEG | STLED316_RB_SEG ),											// 3
	( STLED316_MID_SEG | STLED316_LT_SEG  | STLED316_RT_SEG  | STLED316_RB_SEG ),															// 4
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_LT_SEG | STLED316_RB_SEG ),											// 5
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_LT_SEG | STLED316_LB_SEG | STLED316_RB_SEG ),						// 6
	( STLED316_TOP_SEG | STLED316_RT_SEG  | STLED316_RB_SEG ),																				// 7
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_LT_SEG | STLED316_LB_SEG | STLED316_RT_SEG | STLED316_RB_SEG ),		// 8
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_LT_SEG | STLED316_RT_SEG | STLED316_RB_SEG ),						// 9
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_LT_SEG	 | STLED316_LB_SEG | STLED316_RT_SEG | STLED316_RB_SEG ),						// A
	( STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_LT_SEG	 | STLED316_LB_SEG | STLED316_RB_SEG ),											// b
	( STLED316_TOP_SEG | STLED316_BOT_SEG | STLED316_LT_SEG  | STLED316_LB_SEG ),															// C
	( STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_LB_SEG  | STLED316_RT_SEG | STLED316_RB_SEG ),											// d
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_BOT_SEG | STLED316_LT_SEG | STLED316_LB_SEG ),											// E
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_LT_SEG  | STLED316_LB_SEG ),															// F
	( STLED316_MID_SEG ),																													// -
	( 0 ),																																	// space
	( STLED316_TOP_SEG | STLED316_MID_SEG | STLED316_BOT_SEG )																				// Not a char
};


