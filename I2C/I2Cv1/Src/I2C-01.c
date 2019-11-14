#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define A3BU_PCB1_CON1J2_CON2J1
#include "board.h"
#include "a3bu_pcb1.h"
#include "LCD_st7565r/lcd_st7565r.h"
#include "KAmodCOMBO/KAmodCOMBO.h"
#include "gpio/gpio.h"
#include "osc/osc.h"
#include "pit/pit.h"
#include "twi/twi.h"

//--------------------------------------------------------

// Clock options
//	#define		OSC_RC2M_SELECT
	#define		OSC_RC32M_SELECT
//	#define		DFLL_RC32K_ENABLE
	#define		DFLL_TOSC32K_ENABLE

//--------------------------------------------------------

#define		DFLLRC2M_RATIO		0x7A1	
#define		DFLLRC32M_RATIO		0x7A12

//=============================================================================

// DS1621 Commands
#define DS1621_TEMP				0xAA
#define DS1621_TH				0xA1
#define DS1621_TL				0xA2
#define DS1621_CFG				0xAC
#define DS1621_COUNTER			0xA8
#define DS1621_SLOPE			0xA9
#define DS1621_START			0xEE
#define DS1621_STOP				0x22

// DS1621 Configuration Byte bit masks
#define DS1621_CFG_1SHOT_bm 	0x01
#define	DS1621_CFG_HPOL_bm		0x02
#define	DS1621_CFG_NVB_bm		0x10
#define	DS1621_CFG_TLF_bm		0x20
#define	DS1621_CFG_THF_bm		0x40
#define	DS1621_CFG_DONE_bm		0x80

#define DS1621_ADDR_A2A1A0		0x7				// set by jumpers
#define DS1621_ADDR				(0x48 + DS1621_ADDR_A2A1A0)

//=============================================================================

#define S1 0x0001
#define	S2 0x0002
#define S3 0x0004
#define	S4 0x0008
#define S5 0x0010
#define	S6 0x0020
#define S7 0x0040
#define	S8 0x0080
#define S9 0x0100
#define	S10 0x0200
#define S11 0x0400
#define	S12 0x0800
#define S13 0x1000
#define	S14 0x2000
#define S15 0x4000
#define S16 0x8000

// Definicje menu:

#define UP			S8
#define DOWN		S16
#define OK			S7
#define BACK		S15
#define UUP			S5 // ustaw temperature szybko do góry
#define DDOWN		S13// ustaw temperature szybko na dó³

//=============================================================================
//=============================================================================

void UInt8_to_Txt( uint8_t x, char *txt )
{
	uint8_t		i;

	for( i = 0; i < 8; i++ )
	{
		txt[i] = ((x & 0x80) != 0 ) ? '1' : '0';
		x <<= 1;
	}
	txt[8] = '\0';
}
//=============================================================================
//=============================================================================

int main(void)
{
	uint16_t		But0, But1, But2,But3,  ButREdge;
	uint8_t			cmd, buf[2];
	int16_t			temp;

//------------------------------------------
	sei();
#ifdef OSC_RC2M_SELECT
	OSC_PLLCfg( OSC_PLLSRC_RC2M_gc, 16, 1 );				// PLL output = 32 MHz
	OSC_Enable( OSC_PLLEN_bm );
	while( OSC_Ready( OSC_PLLRDY_bm ) == 0 );

	CLK_PerClkCfg( CLK_PSADIV_2_gc, CLK_PSBCDIV_1_1_gc );	// CPU, PER, PER2, PER4 clock = 16 MHz
	CLK_SysClkSel( CLK_SCLKSEL_PLL_gc );

#if defined(DFLL_RC32K_ENABLE)

	OSC_Enable( OSC_RC32KEN_bm );							// Enable external oscillator RC32K - OK
	while( OSC_Ready( OSC_RC32KRDY_bm ) == 0 );

	DFLL_Cfg( &DFLLRC2M, DFLLRC2M_RATIO );					// Load DFLL ratio
	DFLL_RC2M_RefClkSel( OSC_RC2MCREF_RC32K_gc );			// Select 32 kHz internal clock as DFLL reference
	DFLL_Enable( &DFLLRC2M );								// Enable DFLL

#elif defined(DFLL_TOSC32K_ENABLE)

	OSC_TOSC32K_Start( );

	OSC_XOsc_TOSC32K_Sel( 0 );								// Select 32 kHz TOSC as external clock
	OSC_Enable( OSC_XOSCEN_bm );							// Enable external oscillator
	while( OSC_Ready( OSC_XOSCRDY_bm ) == 0 );

	DFLL_Cfg( &DFLLRC2M, DFLLRC2M_RATIO );					// Load DFLL ratio
	DFLL_RC2M_RefClkSel( OSC_RC2MCREF_XOSC32K_gc );			// Select 32 kHz external clock as DFLL reference
	DFLL_Enable( &DFLLRC2M );								// Enable DFLL

#endif

#endif

#ifdef OSC_RC32M_SELECT

	OSC_Enable( OSC_RC32MEN_bm );							// Enable 32 MHz RC oscillator
	while( OSC_Ready( OSC_RC32MRDY_bm ) == 0 );

	CLK_PerClkCfg( CLK_PSADIV_2_gc, CLK_PSBCDIV_1_1_gc );	// CPU, PER, PER2, PER4 clock = 16 MHz
	CLK_SysClkSel( CLK_SCLKSEL_RC32M_gc );

#if defined(DFLL_RC32K_ENABLE)

	OSC_Enable( OSC_RC32KEN_bm );							// Enable external oscillator RC32K - OK
	while( OSC_Ready( OSC_RC32KRDY_bm ) == 0 );

	DFLL_Cfg( &DFLLRC32M, DFLLRC32M_RATIO );				// Load DFLL ratio
	DFLL_RC32M_RefClkSel( OSC_RC32MCREF_RC32K_gc );			// Select 32 kHz internal clock as DFLL reference
	DFLL_Enable( &DFLLRC32M );								// Enable DFLL

#elif defined(DFLL_TOSC32K_ENABLE)

	OSC_TOSC32K_Start( );

	OSC_XOsc_TOSC32K_Sel( 0 );								// Select 32 kHz TOSC as external clock
	OSC_Enable( OSC_XOSCEN_bm );							// Enable external oscillator
	while( OSC_Ready( OSC_XOSCRDY_bm ) == 0 );

	DFLL_Cfg( &DFLLRC32M, DFLLRC32M_RATIO );				// Load DFLL ratio
	DFLL_RC32M_RefClkSel( OSC_RC32MCREF_XOSC32K_gc );		// Select 32 kHz external clock as DFLL reference
	DFLL_Enable( &DFLLRC32M );								// Enable DFLL

#endif

	OSC_Disable( OSC_RC2MEN_bm );							// For power saving

#endif

//--------------------------------------------------------------

// Konfiguracja modu³u KAmodCOMBO

	STLED316_Initialize( );
//	STLED316_InitializeInt( 0 );
	STLED316_DisplayOn( );

	STLED316_SetBrghtCtrlModeAndDigitCnt(STLED316_BRGHT_MODE_VAR, 0, 6);

	STLED316_7Seg_SetBrght( 4 );
	STLED316_LEDs_SetBrght( 2 );
	STLED316_7Seg_WriteTxt( 5, "      " );

//--------------------------------------------------------------

// Konfiguracja podœwietlenia i wyœwietlacza LCD

	LCD_Backlight_CfgPin( );
//	LCD_Backlight_CfgPWM( );
	LCD_Backlight_On( );
	lcd_st7565r_InitInterface( );
	lcd_st7565r_InitCtrl( );
	lcd_st7565r_ClrScr( LCD_PIXEL_CLR );
	lcd_st7565r_SetContrast( ST7565R_DISPLAY_CONTRAST_MIN );
	lcd_st7565r_SelectFont( lcd_st7565r_font_medium );
	lcd_st7565r_GotoTextXY( 0, 0 );

//--------------------------------------------------------------
	
	PIT_Cfg( TC_CLKSEL_DIV8_gc, 40000,	5 );			// TCF0 Clk = 2 MHz, OVF Int period - 20 ms, Loop period - 5*20 ms
	PIT_Start( );

//--------------------------------------------------------------

// Konfiguracja portów do odczytu prze³¹czników i przycisków na p³ytce A3BU-PCB1

	GPIO_CfgPins( &PCB1_SW_BUT_PORT_LO, PCB1_SW_BUT_PIN_LO, (PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc), 0, 0, PORT_INVEN_bm );
	GPIO_CfgPins( &PCB1_SW_BUT_PORT_HI, PCB1_SW_BUT_PIN_HI, (PORT_OPC_TOTEM_gc | PORT_ISC_BOTHEDGES_gc), 0, 0, PORT_INVEN_bm );

//--------------------------------------------------------------

// Konfiguracja wyjœcia TWI 
	
	TWI_CfgPins( &PORTC, (GPIO_PIN_0 | GPIO_PIN_1), true );
	TWI_SetBaudRate( &TWIC, 100000, F_CPU );
	TWIM_Enable( &TWIC );
	TWIM_SetIdleBusState( &TWIC );

//--------------------------------------------------------------

// Inicjowanie zmiennych

	But0 = STLED316_Buts_Read( );
	But1 = But0;
	But2 = But0;
	But3 = But0;
	int a=0;
	int16_t Thz;
	int16_t Tlz;
	int nz=0,inf=1;
	char txt1[16];
	int8_t m=0; // zmienna do licznika menu

//--------------------------------------------------------------

//sygnalizacja dzia³ania termostatu
void termostat ()
{
	if (!(PORTC.IN & PIN3_bm))
	{
		sprintf( txt1, " ON " );
		lcd_st7565r_GotoTextXY( 100,20 );
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}
	if (PORTC.IN & PIN3_bm)
	{
		sprintf( txt1, "OFF" );
		lcd_st7565r_GotoTextXY( 100,20 );
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}
}

void error( int8_t inf ) //funkcja ta sprawdza czy nie wyst¹pi³ b³¹d
{
	if (inf<0)
	{
		if (inf==-1)
		{
			sprintf( txt1, "NOSLAVE             "); 
			lcd_st7565r_GotoTextXY( 0, 20 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
		}
		
		if (inf==-11)
		{
			sprintf( txt1, "ARBLOST             ");
			lcd_st7565r_GotoTextXY( 0, 20 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
		}
		
		if (inf==-12)
		{
			sprintf( txt1, "BUSERR              ");
			lcd_st7565r_GotoTextXY( 0, 20 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
		}
		
	}
	else
	{
		sprintf( txt1, "       "); //czyszczenie ekranu
		lcd_st7565r_GotoTextXY( 0, 20 );
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}
}
//--------------------------------------------------------------
//--------------------------------------------------------------

	while(1) //petla glowna programu
	{
			
	But0 = STLED316_Buts_Read( );//odczytaj stan przycisków
	ButREdge = ~But1 & But0; //sprawdz czy by³a zmiana stanu
	if (ButREdge & BACK){lcd_st7565r_ClrScr(LCD_PIXEL_CLR);}//po powrocie do menu wyczyœæ ekran
	termostat();
	
//--------------------------------------------------------------
	
if (inf<0) //jeœli wystapi³ b³ad w po³¹czeniu z kontrolerem 
{
	nz=0;	//zmienna pomocnicza do ponownego zainicjowania
}

//--------------------------------------------------------------
	
	if(nz++<1)		// inicjowanie 
{
	cmd = DS1621_START; 
	TWIM_WriteBuf( &TWIC, DS1621_ADDR, &cmd, 1 ); //start komunikacji
		
	cmd = DS1621_TH;					//odczytaj temperature TH w czujniku aby ustawiac od tej temp. w Ustaw TH  
	TWIM_ReadBuf( &TWIC, DS1621_ADDR, &cmd, 1, buf, 2 );
	Thz = ((int16_t)buf[0] << 1) | (int16_t)(buf[1] >> 7);
	Thz *= 5;
	sprintf( txt1, "TH =%3d.%1d", (Thz / 10), (abs( Thz ) % 10));
		
	cmd = DS1621_TL;				//odczytaj temperature TL w czujniku aby ustawiac od tej temp. w Ustaw TH  
	TWIM_ReadBuf( &TWIC, DS1621_ADDR, &cmd, 1, buf, 2 );
	Tlz = ((int16_t)buf[0] << 1) | (int16_t)(buf[1] >> 7);
	Tlz *= 5;
	sprintf( txt1, "TL =%3d.%1d", (Tlz / 10), (abs( Tlz ) % 10));
								
	nz=31; //zmienna pomocnicza ¿eby nie powtarza³o inicjalizacji
}

	//---------------------------
	
	//Menu graficzne programu
	//licznik menu programu
	if ((ButREdge & DOWN) != 0)		//licznik menu góra
	{
		m++;
		if (m>5) m=0;
	}

	if ((ButREdge & UP) != 0)		//licznik menu dó³
	{
		m--;
		if (m<0) m=5;
	}
	
	//wyœwietlanie menu programu
	
	if (m==0)
	{
		sprintf( txt1, "Pomiar reczny       ");
		lcd_st7565r_GotoTextXY( 0, 0 );
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}

	if (m==1)
	{
		sprintf( txt1, "Pomiar automatyczny ");
		lcd_st7565r_GotoTextXY( 0, 0 );
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}

	if (m==2)
	{
		sprintf( txt1, "Odczyt TH i TL      ");
		lcd_st7565r_GotoTextXY( 0, 0 );
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}

	if (m==3)
	{
		sprintf( txt1, "Odczyt CFG          ");
		lcd_st7565r_GotoTextXY( 0, 0 );
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}

	if (m==4)
	{
		sprintf( txt1, "Ustaw TH          ");
		lcd_st7565r_GotoTextXY( 0, 0 );
		
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}

	if (m==5)
	{
		sprintf( txt1, "Ustaw TL            ");
		lcd_st7565r_GotoTextXY( 0, 0 );
		lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
	}
	
	//podmenu programu
	
if ((m==0) && ((ButREdge & OK) != 0))		// odczyt reczny
{
	while (1)
	{
		But0 = STLED316_Buts_Read( );
		ButREdge = ~But1 & But0;
		termostat();
		
		if(((ButREdge & OK) != 0))		
		{
			cmd = DS1621_TEMP;				//komenda odczytu temperatury
			inf=TWIM_ReadBuf( &TWIC, DS1621_ADDR, &cmd, 1, buf, 2 ); 
			temp = ((int16_t)buf[0] << 1) | (int16_t)(buf[1] >> 7);
			temp *= 5;
			sprintf( txt1, "T =%3d.%1d", (temp / 10), (abs( temp ) % 10)); // Wyœwietl temperaturê
			lcd_st7565r_GotoTextXY( 0, 10 );      
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			error(inf); //wywo³aj funkcje error 
			}
							
		if (inf<0) //powrót do menu po b³êdzie
		{
			sprintf( txt1, "              ");
			lcd_st7565r_GotoTextXY( 0, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			break;
		}
		else
		{sprintf( txt1, "Odczyt-ok (S7)");
			lcd_st7565r_GotoTextXY( 0, 20 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );}
							
		if (STLED316_Buts_Read( ) == BACK) //jesli przycisk back wcisniety powrót do menu g³ównego
		{
			break;
		}
		But1=But0;
	}
}

	if ((m==1) && ((ButREdge & OK) != 0))		//odczyt automatyczny
	{
		a=7000; // zmienna pomocnicza
		lcd_st7565r_ClrScr(LCD_PIXEL_CLR);	//wyczyœæ ca³y ekran
		
		while (1)
		{
			
		if(a++>6000)	
		{
			cmd = DS1621_TEMP;
			inf=TWIM_ReadBuf( &TWIC, DS1621_ADDR, &cmd, 1, buf, 2 );
			temp = ((int16_t)buf[0] << 1) | (int16_t)(buf[1] >> 7);
			temp *= 5;
			sprintf( txt1, "T =%3d.%1d", (temp / 10), (abs( temp ) % 10));
			lcd_st7565r_GotoTextXY( 0, 10 );       // Wyœwietl temperaturê
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			a=0;
			termostat();
			error(inf);
		}
			
		if (inf<0) //powrót do menu po b³êdzie
		{
			sprintf( txt1, "              ");
			lcd_st7565r_GotoTextXY( 0, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			nz=0;
			break;
		}
		
		if (STLED316_Buts_Read( ) == BACK)
		{
			break;
		}
			
	}
	}

	if ((m==2) && ((ButREdge & OK) != 0))									//odczyt TH i TL
	{
		lcd_st7565r_ClrScr(LCD_PIXEL_CLR);
		a=7000;
		
		while (1)
		{	
						
		if(a++>6000)		
			{
			cmd = DS1621_TH;
			inf=TWIM_ReadBuf( &TWIC, DS1621_ADDR, &cmd, 1, buf, 2 );
			temp = ((int16_t)buf[0] << 1) | (int16_t)(buf[1] >> 7);
			temp *= 5;
			sprintf( txt1, "TH =%3d.%1d", (temp / 10), (abs( temp ) % 10));
			lcd_st7565r_GotoTextXY( 0, 10 );       
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
														
			cmd = DS1621_TL;
			inf=TWIM_ReadBuf( &TWIC, DS1621_ADDR, &cmd, 1, buf, 2 );
			temp = ((int16_t)buf[0] << 1) | (int16_t)(buf[1] >> 7);
			temp *= 5;
			sprintf( txt1, "TL =%3d.%1d", (temp / 10), (abs( temp ) % 10));
			lcd_st7565r_GotoTextXY( 70, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			
			termostat();
			error(inf);	
			a=0;					
			}
		
		if (inf<0) //powrót do menu po b³êdzie
		{
			sprintf( txt1, "                     ");
			lcd_st7565r_GotoTextXY( 0, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			nz=0;
			break;
		}
				
		if (STLED316_Buts_Read( ) == BACK)		
		{
			break;
		}
		}
	}

	
	if ((m==3) && ((ButREdge & OK) != 0)) // odczyt cfg
	{
		a=7000;
		lcd_st7565r_ClrScr(LCD_PIXEL_CLR);
		
	while (1)
	{
		if(a++>6000)		
		{
			cmd = DS1621_CFG;
			inf=TWIM_ReadBuf( &TWIC, DS1621_ADDR, &cmd, 1, buf, 1 );
			sprintf( txt1, "cfg%3x", (uint16_t) buf[0]);
			lcd_st7565r_GotoTextXY( 0, 10 );       
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			a=0;
			termostat();
			error(inf);
		}
		
		if (inf<0) //powrót do menu po b³êdzie
		{
			sprintf( txt1, "              ");
			lcd_st7565r_GotoTextXY( 0, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			nz=0;
			break;
		}
		if (STLED316_Buts_Read( ) == BACK)
		
		{
			break;
		}
		
	}
	}

	if ((m==4) && ((ButREdge & OK) != 0))           //Ustaw THz
	{lcd_st7565r_ClrScr(LCD_PIXEL_CLR);
	
	while (1)
{
		But0 = STLED316_Buts_Read( );
		ButREdge = ~But2 & But0;
		if((ButREdge & UP) != 0)		
		{	Thz+=5;
			if (Thz>1250)
			{	Thz = 1250;	}
		}
				
			
		if((ButREdge & DOWN) != 0)		
		{	Thz-=5;
			if (Thz<Tlz)
			{	Thz = Tlz;	}
		}
		if((STLED316_Buts_Read( ) & S5 ) != 0)
		{				
				Thz+=5;
				if (Thz>1250)
				{	Thz = 1250;	}
		}
						
		if((STLED316_Buts_Read( ) & S13 ) != 0)
		{
			
				Thz-=5;
				if (Thz<Tlz)
				{	Thz = Tlz;	}
		}
		
			if (Thz >=0)
			{
			sprintf( txt1, "Thz = %3d.%1d", (Thz / 10), (abs( Thz ) % 10));
			lcd_st7565r_GotoTextXY( 0, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			termostat();
			}
										
		if ((ButREdge & OK) != 0)
		{
			if (Thz >=0)
			{	uint8_t tabTH[3];
				tabTH[0] = DS1621_TH;          
				tabTH[1] = Thz / 10;
				tabTH[2] = (Thz % 10) < 5 ? 0 : 0x80;
				inf=TWIM_WriteBuf( &TWIC, DS1621_ADDR, tabTH, 3 );
				error(inf);
			}
		}		
		
		if (inf<0) //powrót do menu po b³êdzie
		{
			sprintf( txt1, "              ");
			lcd_st7565r_GotoTextXY( 0, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			nz=0;
			break;
		}
		
		But2=But0;


		if (STLED316_Buts_Read( ) == BACK)
		{		break;		}
}
	}
				
	if ((m==5) && ((ButREdge & OK) != 0))  //ustaw TL
	{lcd_st7565r_ClrScr(LCD_PIXEL_CLR);
		while (1)
		{
			But0 = STLED316_Buts_Read( );
			ButREdge = ~But3 & But0;
			
			if((ButREdge & UP) != 0)		
			{	Tlz+=5;
				if (Tlz>Thz)
				{  Tlz = Thz;	}
			}
			if((ButREdge & DOWN) != 0)		
			{	Tlz-=5;
				if (Tlz<0)
				{	Tlz = 0;	}
			}			
				if((STLED316_Buts_Read( ) & UUP ) != 0)
				{
					Tlz+=5;
					if (Tlz>Thz)
					{  Tlz = Thz;	}
				}
				
				if((STLED316_Buts_Read( ) & DDOWN ) != 0)
				{
					
					Tlz-=5;
					if (Tlz<0)
					{	Tlz = 0;	}
				}
			sprintf( txt1, "Tlz =%3d.%1d", (Tlz / 10), (abs( Tlz ) % 10));
			lcd_st7565r_GotoTextXY( 0, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			termostat();
			
			if ((ButREdge & OK) != 0)
			{
				uint8_t tabTL[3];
				tabTL[0] = DS1621_TL;          
				tabTL[1] = Tlz / 10;
				tabTL[2] = (Tlz % 10) < 5 ? 0 : 0x80;
				inf=TWIM_WriteBuf( &TWIC, DS1621_ADDR, tabTL, 3 );
				error(inf);
			}
			
		if (inf<0) //powrót do menu po b³êdzie
		{
			sprintf( txt1, "              ");
			lcd_st7565r_GotoTextXY( 0, 10 );
			lcd_st7565r_DrawText( txt1, LCD_PIXEL_SET );
			nz=0;
			break;
		}
			
			But3=But0;
			
	if (STLED316_Buts_Read( ) == BACK)
		{
			break;
		}
}
}		
					
	But1 = But0;
	PIT_Wait( );
	
	}
	}
	