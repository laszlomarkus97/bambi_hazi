/*
 * types.h
 *
 *  Created on: 2018. nov. 14.
 *      Author: small
 */

#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_
#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint16_t index;
	uint16_t values;
}position_t;

typedef struct{
	int length_of_block;
	position_t coordinates[2];
	bool hit;
}block_t;

typedef struct{
	block_t blocks[2]; //always two item
}ship_t;

typedef struct{
	ship_t ships[4];
}layout_t;



#endif /* SRC_TYPES_H_ */
