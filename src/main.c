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

#include "program_logic.h"
#include "types.h"


/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/
int main(void){

  /* Chip errata */
  CHIP_Init();
  init();


  storage();

  /* Infinite blink loop */
  while (1) {

	  update_position();
	  Display();



  }
}
