/*
 * program_logic.h
 *
 *  Created on: 2018. nov. 14.
 *      Author: small
 */

#ifndef SRC_PROGRAM_LOGIC_H_
#define SRC_PROGRAM_LOGIC_H_
#include "types.h"

int x;
int y;
void update_position();
void clear_all_segments();
void Display();
void shot(position_t position);
void storage();
position_t calculate_current_coordinate();

#endif /* SRC_PROGRAM_LOGIC_H_ */
