/*
 * common.h
 *
 *  Created on: 2018. nov. 14.
 *      Author: small
 */


#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

#define max_x 48
#define max_y 4




//for communication
volatile bool btn_1_pressed;
volatile bool btn_2_pressed;
volatile bool double_btn_pressed;

volatile bool slider_move_right;
volatile bool slider_move_left;

volatile bool  update_slider;

//IT
volatile bool button_1_rising_edge;
volatile bool button_2_rising_edge;

volatile int elapsed_time;

// timing
volatile bool light_blinking_led;
volatile bool aring_blink;
//
int layout_number;
int hit_number;







#endif /* SRC_COMMON_H_ */
