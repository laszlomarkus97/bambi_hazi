/***************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for EFM32GG_STK3700
 * @version 5.6.1
 *******************************************************************************
 * # License
 * <b>Copyright 2015 Silicon Labs, Inc. http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include "init.h"
#include "em_chip.h"
#include "stdlib.h"
#include "stdio.h"
#include "retargetserial.h"

#include "common.h"

#include "program_logic.h"
#include "types.h"

#include "segmentlcd.h"
#include "segmentlcd_individual.h"

/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/
int main(void){

  /* Chip errata */
  CHIP_Init();
  init();

  SegmentLCD_Write("SHOT!");

  while(!double_btn_pressed);
  double_btn_pressed=false;


  layout_number=elapsed_time%16;
  printf("layout :  %d \n",layout_number);


  storage();

  /* Infinite blink loop */
  while (1) {



	  update_position();
	  Display();
	  if(hit_number==8){
	  							clear_all_segments();
	  							 while(!double_btn_pressed)
	  							 {
	  								 SegmentLCD_Write("WIN!");
	  							 }
	  						}



  }
}
