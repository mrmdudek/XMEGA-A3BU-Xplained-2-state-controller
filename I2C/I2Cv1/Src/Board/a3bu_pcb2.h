/*
 * A3BU_PCB2.h
 *
 * Header file for use with A3BU_PCB1 board
 * (R, C measurement)
 */ 

#ifndef A3BU_PCB2_H_INCLUDED
#define A3BU_PCB2_H_INCLUDED

#include "gpio/gpio.h"

//-----------------------------------------------------------------------------

#define PCB2_CTRL_PORT			PORTC
#define PCB2_CTRL_RZ1_PIN		GPIO_PIN_0
#define PCB2_CTRL_RZ2_PIN		GPIO_PIN_1
#define PCB2_CTRL_RZ3_PIN		GPIO_PIN_2
#define PCB2_CTRL_CZ1_PIN		GPIO_PIN_3
#define PCB2_CTRL_CZ2_PIN		GPIO_PIN_4
#define PCB2_CTRL_CZ3_PIN		GPIO_PIN_5
#define PCB2_CTRL_RD_PIN		GPIO_PIN_6

#define PCB2_CTRL_RZ_PINS		(PCB2_CTRL_RZ1_PIN | PCB2_CTRL_RZ2_PIN | PCB2_CTRL_RZ3_PIN)
#define PCB2_CTRL_CZ_PINS		(PCB2_CTRL_CZ1_PIN | PCB2_CTRL_CZ2_PIN | PCB2_CTRL_CZ3_PIN)

//-----------------------------------------------------------------------------

#define PCB2_ANALOG_PORT		PORTB
#define PCB2_ADC0_PIN			GPIO_PIN_0
#define PCB2_ADC1_PIN			GPIO_PIN_1
#define PCB2_ADC2_PIN			GPIO_PIN_2

//-----------------------------------------------------------------------------

#define R8					5100L
#define R9					9100L

#define R10					3000L
#define R11					3600L

#define RR1					1000L
#define RR2					10000L
#define RR3					100000L

#define RC1					10000L
#define RC2					100000L
#define RC3					1000000L

//-----------------------------------------------------------------------------

#endif /* A3BU_PCB2_H_INCLUDED */
