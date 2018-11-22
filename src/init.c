/*
 * init.c
 *
 *  Created on: 2018. nov. 14.
 *      Author: small
 */


#include "init.h"
#include "common.h"
#include "IT_handlers.h"

#include "stdio.h"
#include "retargetserial.h"

#include "em_device.h"

#include "em_cmu.h"
#include "em_emu.h"
#include "em_timer.h"
#include "em_gpio.h"

#include "gpiointerrupt.h"
#include "caplesense.h"

#include "segmentlcd.h"
#include "segmentlcd_individual.h"



#define TIMER_0_TOP 2734 // TOP=t/(PRESCALE*(1/14MHZ))
#define TIMER_1_TOP 1000

void init_clock(){

	//GPIO
	CMU_ClockEnable(cmuClock_GPIO,true);

	//TIMER0
	CMU_ClockEnable(cmuClock_TIMER0,true);
	CMU_ClockEnable(cmuClock_TIMER1,true);

	//RTC
	// Enable the oscillator for the RTC
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

	// Turn on the clock for Low Energy clocks
	CMU_ClockEnable(cmuClock_HFLE, true);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

	// Turn on the RTC clock
	CMU_ClockEnable(cmuClock_RTC, true);
	//RTC END

}
void gpio_init(){
	// set output
	  GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
	  GPIO_PinModeSet(gpioPortE, 3, gpioModePushPull, 0);


	 //set input
	  GPIO_PinModeSet(gpioPortB, 9, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortB, 10, gpioModeInput, 0);

	  GPIOINT_Init();
	   /* Register callbacks before setting up and enabling pin interrupt. */
	   GPIOINT_CallbackRegister(9, gpio_callback);
	   GPIOINT_CallbackRegister(10, gpio_callback);

	   /* Set rising edge interrupt for both ports */
	   GPIO_IntConfig(gpioPortB, 9, true, false, true);
	   GPIO_IntConfig(gpioPortB, 10, true, false, true);
}

void init_timer()
{

	// INIT TIMER0 AS ONE SHOT TIMER
	TIMER_Init_TypeDef TIMER0_init = TIMER_INIT_DEFAULT;
	TIMER0_init.enable=false;
	TIMER0_init.oneShot=true;
	TIMER0_init.prescale = timerPrescale1024; // timerPrescale1...timerPrescale1024
	 TIMER_Init(TIMER0, &TIMER0_init);
	 TIMER_CounterSet(TIMER0, 0); //
	 TIMER_TopSet(TIMER0, TIMER_0_TOP); // 14MHz/presc/TOP
	 TIMER_IntClear(TIMER0, TIMER_IF_OF);
	 TIMER_IntEnable(TIMER0, TIMER_IF_OF);
	 NVIC_EnableIRQ(TIMER0_IRQn);

	 // INIT TIMER1 AS REPETATIVE TIMER
	TIMER_Init_TypeDef TIMER1_init = TIMER_INIT_DEFAULT;
	TIMER1_init.enable=true;
	TIMER1_init.oneShot=false;
	TIMER1_init.prescale = timerPrescale1024; // timerPrescale1...timerPrescale1024
	 TIMER_Init(TIMER1, &TIMER1_init);
	 TIMER_CounterSet(TIMER1, 0); //
	 TIMER_TopSet(TIMER1 ,TIMER_1_TOP); // 14MHz/presc/TOP
	 TIMER_IntClear(TIMER1, TIMER_IF_OF);
	 TIMER_IntEnable(TIMER1, TIMER_IF_OF);
	 NVIC_EnableIRQ(TIMER1_IRQn);
}

void init_serial(){
	RETARGET_SerialInit();
	RETARGET_SerialCrLf(true);
	printf("Hello!\n");
}

void init(){


	//initialize global variables
	aring_blink=false;
	hit_number=0;
	light_blinking_led=false;
	layout_number=0;
	btn_1_pressed=false;
	btn_2_pressed=false;
	double_btn_pressed=false;
	slider_move_right=false;
	slider_move_left=false;
	button_1_rising_edge=false;
	button_2_rising_edge=false;
	update_slider=false;


	init_clock();
	gpio_init();
	init_serial();
	init_timer();
	CAPLESENSE_Init(false);
	SegmentLCD_Init(false);
	SegmentLCD_Number(0);
}
