/*
 * IT_handlers.c
 *
 *  Created on: 2018. nov. 14.
 *      Author: small
 */

#include "common.h"
#include "stdio.h"
#include "retargetserial.h"

#include "em_device.h"

#include "em_cmu.h"
#include "em_emu.h"
#include "em_timer.h"
#include "em_gpio.h"

#include "gpiointerrupt.h"
#include "caplesense.h"
int slider_position=-1;
void TIMER0_IRQHandler(void) {
	//printf("Timer0 IT called\n");
	TIMER_IntClear(TIMER0, TIMER_IF_OF);      // Clear overflow flag


	if(button_1_rising_edge&&button_2_rising_edge){
		printf("double press\n");
		double_btn_pressed=true;
	}
	else if(button_1_rising_edge){
		btn_1_pressed=true;
		//printf("button 1 pressed\n");
	}
	else if(button_2_rising_edge){
		btn_2_pressed=true;
		//printf("button 2 pressed\n");
	}
	else{
		//printf("Undefined\n");
	}
	TIMER_CounterSet(TIMER0,0);
	 button_1_rising_edge=false;
	 button_2_rising_edge=false;
}


void TIMER1_IRQHandler(void){
	//printf("Timer1 IT called\n");
	TIMER_IntClear(TIMER1, TIMER_IF_OF);      // Clear overflow flag

	update_slider=true;
	slider_position=CAPLESENSE_getSliderPosition();
	if(slider_position>=0){
		if(slider_position>25)
			slider_move_right=true;
		else
			slider_move_left=true;
	}
	light_blinking_led=!light_blinking_led;
	aring_blink =true;
	//printf("slider %d",slider_position);
}


void gpio_callback(uint8_t pin){
	switch(pin){
	case 9:
		//printf("button 1 rising edge");
		/*
		 * BUTTON1 rising edge
		 */
		elapsed_time=TIMER_CounterGet(TIMER0);
		if(!button_1_rising_edge){
			TIMER_CounterSet(TIMER0,0);
			TIMER_Enable(TIMER0,true);
		}



		button_1_rising_edge=true;
		break;
	case 10:
		//printf("button 2 rising edge");
		/*
		 * BUTTON2 rising edge
		 */
		elapsed_time=TIMER_CounterGet(TIMER0);
		if(!button_2_rising_edge){
			TIMER_CounterSet(TIMER0,0);
			TIMER_Enable(TIMER0,true);
		}


		button_2_rising_edge=true;

		break;
	}
}
