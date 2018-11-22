#include "common.h"
#include "program_logic.h"

#include "segmentlcd.h"
#include "segmentlcd_individual.h"


#include "stdio.h"
#include "retargetserial.h"
#include "types.h"


layout_t	LayoutStorage [16];

typedef struct{
	uint16_t index;
	uint16_t values;
}lcd_position_t;


int coord_x=0;
int coord_y=0;
SegmentLCD_LowerCharSegments_TypeDef segmentField[7];
SegmentLCD_LowerCharSegments_TypeDef hitField[7];
int lcd_calc_dig(int x, int y){

	//A
	if(x==0&&y==4)
		return 0;
	//B
	if(x==4&&y==3)
		return 1;
	//C
	if(x==4&&y==1)
		return 2;
	//D
	if(x==0&&y==0)
		return 3;
	//E
	if(x==0&&y==1)
		return 4;
	//F
	if(x==0&&y==3)
		return 5;
	//G
	if(x==0&&y==2)
		return 6;
	//H
	if(x==1&&y==3)
		return 7;
	//J
	if(x==2&&y==3)
		return 8;
	//K
	if(x==3&&y==3)
		return 9;
	//M
	if(x==1&&y==2)
		return 10;
	//N
	if(x==3&&y==1)
		return 11;
	//P
	if(x==2&&y==1)
		return 12;
	//Q
	if(x==1&&y==1)
		return 13;
	return 0;
}
lcd_position_t lcd_calc_position_on_lcd(int x,int y){
	lcd_position_t pos;

	if(y==0||y==4){
		pos.index=x/7;
		pos.values=0;
	}
	if(y==1||y==3){
		pos.index=x/7;
		pos.values=x%7;
		if(pos.values>4)
			pos.values=4;
	}
	if(y==2){
		pos.index=x/7;
		pos.values=x%7;
		if(pos.values>1)
			pos.values=1;
	}
	return pos;
}
void aring_circle(){
	int aring_pos=0;
	while(aring_pos!=9){
	if(aring_blink){
		SegmentLCD_ARing(aring_pos,true);
		aring_pos++;
		aring_blink=false;
	}}
}
void clear_aring(){
	for(int i=0;i<9;i++){
		SegmentLCD_ARing(i,false);
	}
}
void set_all_aring(){
	for(int i=0;i<9;i++){
			SegmentLCD_ARing(i,true);
		}
}
void blink_aring(){
	aring_blink=false;
	for(int i = 0;i<3;i++){
		set_all_aring();
		while(!aring_blink);
		aring_blink=false;
		clear_aring();
		while(!aring_blink);
		aring_blink=false;
	}




}



void update_position(){
	if(update_slider)
	{
		if(slider_move_left){
			coord_x--;
			if(coord_x<0){
				coord_x=max_x;
			}
			slider_move_left=false;
		}
		else if(slider_move_right){
			coord_x++;
			if(coord_x>max_x){
				coord_x=0;
			}
			slider_move_right=false;
		}
	}
	update_slider=false;
	if(btn_1_pressed){
		coord_y++;
		if(coord_y>max_y)
			coord_y=0;
		btn_1_pressed=false;
	}
	else if(btn_2_pressed){
			coord_y--;
			if(coord_y<0)
				coord_y=max_y;
			btn_2_pressed=false;
	}
	//printf("x:%d y:%d\n",coord_x,coord_y);
	if(double_btn_pressed){
		//printf("double click\n");
		position_t current_pos =calculate_current_coordinate(coord_x,coord_y);

		aring_circle();
		shot(current_pos);
		clear_aring();
		double_btn_pressed=false;
	}


}
void clear_all_segments(){
	for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_LOWER_CHARS; p++) {
		segmentField[p].raw = 0;
	}
	SegmentLCD_LowerSegments(segmentField);
}
position_t calculate_current_coordinate(){
	position_t returned_value;
	lcd_position_t pos=lcd_calc_position_on_lcd(coord_x,coord_y);
	returned_value.index=pos.index;
	returned_value.values=lcd_calc_dig(pos.values,coord_y);
	return returned_value;
}
position_t prev_pos;
void Display(){
	position_t current_pos =calculate_current_coordinate();

	if(light_blinking_led)
		segmentField[current_pos.index].raw |=1<<current_pos.values;
	else
		segmentField[current_pos.index].raw &=~(1<<current_pos.values);



	if(prev_pos.index!=current_pos.index||prev_pos.values!=current_pos.values){
		segmentField[prev_pos.index].raw &=~(1<<prev_pos.values);
	}
	for(int i=0;i<7;i++){
		segmentField[i].raw |=hitField[i].raw;
	}



	prev_pos= current_pos;
	SegmentLCD_LowerSegments(segmentField);
}

void add_to_hit_block(block_t block){
	for(int i=0;i<block.length_of_block;i++){
		hitField[block.coordinates[i].index].raw|= 1<<(block.coordinates[i].values);
	}
}
void shot(position_t position){
	printf("shot current_pos%d %d\n",position.index,position.values);
	position_t current_pos;
	for(int ship_number=0;ship_number <4;ship_number++){
		 for(int block_number=0;block_number<2;block_number++){
			 for(int coordinates=0;coordinates<LayoutStorage[layout_number].ships[ship_number].blocks[block_number].length_of_block;coordinates++){
				 	current_pos= LayoutStorage[layout_number].ships[ship_number].blocks[block_number].coordinates[coordinates];
			 	 	if(current_pos.index==position.index&&current_pos.values==position.values&&(LayoutStorage[layout_number].ships[ship_number].blocks[block_number].hit==false)){
						printf("hit\n");
						hit_number++;
						blink_aring();
						LayoutStorage[layout_number].ships[ship_number].blocks[block_number].hit=true;
						add_to_hit_block(LayoutStorage[layout_number].ships[ship_number].blocks[block_number]);
						SegmentLCD_Number(hit_number);
						return;
			 	 	}
			 	}
		 }
	}
	printf("not hit!!\n");
}



void storage(){
	LayoutStorage[0].ships[0].blocks[0].coordinates[0].values=1;
	LayoutStorage[0].ships[0].blocks[1].coordinates[0].values=2;
	LayoutStorage[0].ships[0].blocks[0].coordinates[0].index=0;
	LayoutStorage[0].ships[0].blocks[1].coordinates[0].index=0;
	LayoutStorage[0].ships[0].blocks[0].hit=false;
	LayoutStorage[0].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[0].ships[0].blocks[1].hit=false;
	LayoutStorage[0].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[0].ships[1].blocks[0].coordinates[0].values=8;
	LayoutStorage[0].ships[1].blocks[1].coordinates[0].values=12;
	LayoutStorage[0].ships[1].blocks[0].coordinates[0].index=1;
	LayoutStorage[0].ships[1].blocks[1].coordinates[0].index=1;
	LayoutStorage[0].ships[1].blocks[0].hit=false;
	LayoutStorage[0].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[0].ships[1].blocks[1].hit=false;
	LayoutStorage[0].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[0].ships[2].blocks[0].coordinates[0].values=0;
	LayoutStorage[0].ships[2].blocks[1].coordinates[0].values=0;
	LayoutStorage[0].ships[2].blocks[0].coordinates[0].index=2;
	LayoutStorage[0].ships[2].blocks[1].coordinates[0].index=3;
	LayoutStorage[0].ships[2].blocks[0].hit=false;
	LayoutStorage[0].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[0].ships[2].blocks[1].hit=false;
	LayoutStorage[0].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[0].ships[3].blocks[0].coordinates[0].values=6;
	LayoutStorage[0].ships[3].blocks[1].coordinates[0].values=6;
	LayoutStorage[0].ships[3].blocks[0].coordinates[0].index=4;
	LayoutStorage[0].ships[3].blocks[1].coordinates[0].index=5;
	LayoutStorage[0].ships[3].blocks[0].coordinates[1].values=10;
	LayoutStorage[0].ships[3].blocks[1].coordinates[1].values=10;
	LayoutStorage[0].ships[3].blocks[0].coordinates[1].index=4;
	LayoutStorage[0].ships[3].blocks[1].coordinates[1].index=5;
	LayoutStorage[0].ships[3].blocks[0].hit=false;
	LayoutStorage[0].ships[3].blocks[0].length_of_block=2;
	LayoutStorage[0].ships[3].blocks[1].hit=false;
	LayoutStorage[0].ships[3].blocks[1].length_of_block=2;

	LayoutStorage[1].ships[0].blocks[0].coordinates[0].values=1;
	LayoutStorage[1].ships[0].blocks[1].coordinates[0].values=2;
	LayoutStorage[1].ships[0].blocks[0].coordinates[0].index=1;
	LayoutStorage[1].ships[0].blocks[1].coordinates[0].index=1;
	LayoutStorage[1].ships[0].blocks[0].hit=false;
	LayoutStorage[1].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[1].ships[0].blocks[1].hit=false;
	LayoutStorage[1].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[1].ships[1].blocks[0].coordinates[0].values=8;
	LayoutStorage[1].ships[1].blocks[1].coordinates[0].values=12;
	LayoutStorage[1].ships[1].blocks[0].coordinates[0].index=2;
	LayoutStorage[1].ships[1].blocks[1].coordinates[0].index=2;
	LayoutStorage[1].ships[1].blocks[0].hit=false;
	LayoutStorage[1].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[1].ships[1].blocks[1].hit=false;
	LayoutStorage[1].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[1].ships[2].blocks[0].coordinates[0].values=0;
	LayoutStorage[1].ships[2].blocks[1].coordinates[0].values=0;
	LayoutStorage[1].ships[2].blocks[0].coordinates[0].index=3;
	LayoutStorage[1].ships[2].blocks[1].coordinates[0].index=4;
	LayoutStorage[1].ships[2].blocks[0].hit=false;
	LayoutStorage[1].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[1].ships[2].blocks[1].hit=false;
	LayoutStorage[1].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[1].ships[3].blocks[0].coordinates[0].values=6;
	LayoutStorage[1].ships[3].blocks[1].coordinates[0].values=6;
	LayoutStorage[1].ships[3].blocks[0].coordinates[0].index=5;
	LayoutStorage[1].ships[3].blocks[1].coordinates[0].index=6;
	LayoutStorage[1].ships[3].blocks[0].coordinates[1].values=10;
	LayoutStorage[1].ships[3].blocks[1].coordinates[1].values=10;
	LayoutStorage[1].ships[3].blocks[0].coordinates[1].index=5;
	LayoutStorage[1].ships[3].blocks[1].coordinates[1].index=6;
	LayoutStorage[1].ships[3].blocks[0].hit=false;
	LayoutStorage[1].ships[3].blocks[0].length_of_block=2;
	LayoutStorage[1].ships[3].blocks[1].hit=false;
	LayoutStorage[1].ships[3].blocks[1].length_of_block=2;

	LayoutStorage[2].ships[0].blocks[0].coordinates[0].values=9;
	LayoutStorage[2].ships[0].blocks[1].coordinates[0].values=13;
	LayoutStorage[2].ships[0].blocks[0].coordinates[0].index=0;
	LayoutStorage[2].ships[0].blocks[1].coordinates[0].index=0;
	LayoutStorage[2].ships[0].blocks[0].hit=false;
	LayoutStorage[2].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[2].ships[0].blocks[1].hit=false;
	LayoutStorage[2].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[2].ships[1].blocks[0].coordinates[0].values=8;
	LayoutStorage[2].ships[1].blocks[1].coordinates[0].values=12;
	LayoutStorage[2].ships[1].blocks[0].coordinates[0].index=1;
	LayoutStorage[2].ships[1].blocks[1].coordinates[0].index=1;
	LayoutStorage[2].ships[1].blocks[0].hit=false;
	LayoutStorage[2].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[2].ships[1].blocks[1].hit=false;
	LayoutStorage[2].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[2].ships[2].blocks[0].coordinates[0].values=7;
	LayoutStorage[2].ships[2].blocks[1].coordinates[0].values=11;
	LayoutStorage[2].ships[2].blocks[0].coordinates[0].index=3;
	LayoutStorage[2].ships[2].blocks[1].coordinates[0].index=3;
	LayoutStorage[2].ships[2].blocks[0].hit=false;
	LayoutStorage[2].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[2].ships[2].blocks[1].hit=false;
	LayoutStorage[2].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[2].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[2].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[2].ships[3].blocks[0].coordinates[0].index=4;
	LayoutStorage[2].ships[3].blocks[1].coordinates[0].index=4;
	LayoutStorage[2].ships[3].blocks[0].hit=false;
	LayoutStorage[2].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[2].ships[3].blocks[1].hit=false;
	LayoutStorage[2].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[3].ships[0].blocks[0].coordinates[0].values=9;
	LayoutStorage[3].ships[0].blocks[1].coordinates[0].values=13;
	LayoutStorage[3].ships[0].blocks[0].coordinates[0].index=1;
	LayoutStorage[3].ships[0].blocks[1].coordinates[0].index=1;
	LayoutStorage[3].ships[0].blocks[0].hit=false;
	LayoutStorage[3].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[3].ships[0].blocks[1].hit=false;
	LayoutStorage[3].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[3].ships[1].blocks[0].coordinates[0].values=8;
	LayoutStorage[3].ships[1].blocks[1].coordinates[0].values=12;
	LayoutStorage[3].ships[1].blocks[0].coordinates[0].index=2;
	LayoutStorage[3].ships[1].blocks[1].coordinates[0].index=2;
	LayoutStorage[3].ships[1].blocks[0].hit=false;
	LayoutStorage[3].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[3].ships[1].blocks[1].hit=false;
	LayoutStorage[3].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[3].ships[2].blocks[0].coordinates[0].values=7;
	LayoutStorage[3].ships[2].blocks[1].coordinates[0].values=11;
	LayoutStorage[3].ships[2].blocks[0].coordinates[0].index=4;
	LayoutStorage[3].ships[2].blocks[1].coordinates[0].index=4;
	LayoutStorage[3].ships[2].blocks[0].hit=false;
	LayoutStorage[3].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[3].ships[2].blocks[1].hit=false;
	LayoutStorage[3].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[3].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[3].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[3].ships[3].blocks[0].coordinates[0].index=5;
	LayoutStorage[3].ships[3].blocks[1].coordinates[0].index=5;
	LayoutStorage[3].ships[3].blocks[0].hit=false;
	LayoutStorage[3].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[3].ships[3].blocks[1].hit=false;
	LayoutStorage[3].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[4].ships[0].blocks[0].coordinates[0].values=9;
	LayoutStorage[4].ships[0].blocks[1].coordinates[0].values=13;
	LayoutStorage[4].ships[0].blocks[0].coordinates[0].index=2;
	LayoutStorage[4].ships[0].blocks[1].coordinates[0].index=2;
	LayoutStorage[4].ships[0].blocks[0].hit=false;
	LayoutStorage[4].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[4].ships[0].blocks[1].hit=false;
	LayoutStorage[4].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[4].ships[1].blocks[0].coordinates[0].values=8;
	LayoutStorage[4].ships[1].blocks[1].coordinates[0].values=12;
	LayoutStorage[4].ships[1].blocks[0].coordinates[0].index=3;
	LayoutStorage[4].ships[1].blocks[1].coordinates[0].index=3;
	LayoutStorage[4].ships[1].blocks[0].hit=false;
	LayoutStorage[4].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[4].ships[1].blocks[1].hit=false;
	LayoutStorage[4].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[4].ships[2].blocks[0].coordinates[0].values=7;
	LayoutStorage[4].ships[2].blocks[1].coordinates[0].values=11;
	LayoutStorage[4].ships[2].blocks[0].coordinates[0].index=5;
	LayoutStorage[4].ships[2].blocks[1].coordinates[0].index=5;
	LayoutStorage[4].ships[2].blocks[0].hit=false;
	LayoutStorage[4].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[4].ships[2].blocks[1].hit=false;
	LayoutStorage[4].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[4].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[4].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[4].ships[3].blocks[0].coordinates[0].index=6;
	LayoutStorage[4].ships[3].blocks[1].coordinates[0].index=6;
	LayoutStorage[4].ships[3].blocks[0].hit=false;
	LayoutStorage[4].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[4].ships[3].blocks[1].hit=false;
	LayoutStorage[4].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[5].ships[0].blocks[0].coordinates[0].values=9;
	LayoutStorage[5].ships[0].blocks[1].coordinates[0].values=13;
	LayoutStorage[5].ships[0].blocks[0].coordinates[0].index=0;
	LayoutStorage[5].ships[0].blocks[1].coordinates[0].index=0;
	LayoutStorage[5].ships[0].blocks[0].hit=false;
	LayoutStorage[5].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[5].ships[0].blocks[1].hit=false;
	LayoutStorage[5].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[5].ships[1].blocks[0].coordinates[0].values=8;
	LayoutStorage[5].ships[1].blocks[1].coordinates[0].values=12;
	LayoutStorage[5].ships[1].blocks[0].coordinates[0].index=2;
	LayoutStorage[5].ships[1].blocks[1].coordinates[0].index=2;
	LayoutStorage[5].ships[1].blocks[0].hit=false;
	LayoutStorage[5].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[5].ships[1].blocks[1].hit=false;
	LayoutStorage[5].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[5].ships[2].blocks[0].coordinates[0].values=7;
	LayoutStorage[5].ships[2].blocks[1].coordinates[0].values=11;
	LayoutStorage[5].ships[2].blocks[0].coordinates[0].index=4;
	LayoutStorage[5].ships[2].blocks[1].coordinates[0].index=4;
	LayoutStorage[5].ships[2].blocks[0].hit=false;
	LayoutStorage[5].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[5].ships[2].blocks[1].hit=false;
	LayoutStorage[5].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[5].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[5].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[5].ships[3].blocks[0].coordinates[0].index=6;
	LayoutStorage[5].ships[3].blocks[1].coordinates[0].index=6;
	LayoutStorage[5].ships[3].blocks[0].hit=false;
	LayoutStorage[5].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[5].ships[3].blocks[1].hit=false;
	LayoutStorage[5].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[6].ships[0].blocks[0].coordinates[0].values=9;
	LayoutStorage[6].ships[0].blocks[1].coordinates[0].values=13;
	LayoutStorage[6].ships[0].blocks[0].coordinates[0].index=0;
	LayoutStorage[6].ships[0].blocks[1].coordinates[0].index=0;
	LayoutStorage[6].ships[0].blocks[0].hit=false;
	LayoutStorage[6].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[6].ships[0].blocks[1].hit=false;
	LayoutStorage[6].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[6].ships[1].blocks[0].coordinates[0].values=8;
	LayoutStorage[6].ships[1].blocks[1].coordinates[0].values=12;
	LayoutStorage[6].ships[1].blocks[0].coordinates[0].index=1;
	LayoutStorage[6].ships[1].blocks[1].coordinates[0].index=1;
	LayoutStorage[6].ships[1].blocks[0].hit=false;
	LayoutStorage[6].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[6].ships[1].blocks[1].hit=false;
	LayoutStorage[6].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[6].ships[2].blocks[0].coordinates[0].values=7;
	LayoutStorage[6].ships[2].blocks[1].coordinates[0].values=11;
	LayoutStorage[6].ships[2].blocks[0].coordinates[0].index=5;
	LayoutStorage[6].ships[2].blocks[1].coordinates[0].index=5;
	LayoutStorage[6].ships[2].blocks[0].hit=false;
	LayoutStorage[6].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[6].ships[2].blocks[1].hit=false;
	LayoutStorage[6].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[6].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[6].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[6].ships[3].blocks[0].coordinates[0].index=6;
	LayoutStorage[6].ships[3].blocks[1].coordinates[0].index=6;
	LayoutStorage[6].ships[3].blocks[0].hit=false;
	LayoutStorage[6].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[6].ships[3].blocks[1].hit=false;
	LayoutStorage[6].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[7].ships[0].blocks[0].coordinates[0].values=0;
	LayoutStorage[7].ships[0].blocks[1].coordinates[0].values=0;
	LayoutStorage[7].ships[0].blocks[0].coordinates[0].index=0;
	LayoutStorage[7].ships[0].blocks[1].coordinates[0].index=1;
	LayoutStorage[7].ships[0].blocks[0].hit=false;
	LayoutStorage[7].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[7].ships[0].blocks[1].hit=false;
	LayoutStorage[7].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[7].ships[1].blocks[0].coordinates[0].values=3;
	LayoutStorage[7].ships[1].blocks[1].coordinates[0].values=3;
	LayoutStorage[7].ships[1].blocks[0].coordinates[0].index=1;
	LayoutStorage[7].ships[1].blocks[1].coordinates[0].index=2;
	LayoutStorage[7].ships[1].blocks[0].hit=false;
	LayoutStorage[7].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[7].ships[1].blocks[1].hit=false;
	LayoutStorage[7].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[7].ships[2].blocks[0].coordinates[0].values=1;
	LayoutStorage[7].ships[2].blocks[1].coordinates[0].values=2;
	LayoutStorage[7].ships[2].blocks[0].coordinates[0].index=3;
	LayoutStorage[7].ships[2].blocks[1].coordinates[0].index=3;
	LayoutStorage[7].ships[2].blocks[0].hit=false;
	LayoutStorage[7].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[7].ships[2].blocks[1].hit=false;
	LayoutStorage[7].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[7].ships[3].blocks[0].coordinates[0].values=7;
	LayoutStorage[7].ships[3].blocks[1].coordinates[0].values=11;
	LayoutStorage[7].ships[3].blocks[0].coordinates[0].index=4;
	LayoutStorage[7].ships[3].blocks[1].coordinates[0].index=4;
	LayoutStorage[7].ships[3].blocks[0].hit=false;
	LayoutStorage[7].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[7].ships[3].blocks[1].hit=false;
	LayoutStorage[7].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[8].ships[0].blocks[0].coordinates[0].values=0;
	LayoutStorage[8].ships[0].blocks[1].coordinates[0].values=0;
	LayoutStorage[8].ships[0].blocks[0].coordinates[0].index=2;
	LayoutStorage[8].ships[0].blocks[1].coordinates[0].index=3;
	LayoutStorage[8].ships[0].blocks[0].hit=false;
	LayoutStorage[8].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[8].ships[0].blocks[1].hit=false;
	LayoutStorage[8].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[8].ships[1].blocks[0].coordinates[0].values=3;
	LayoutStorage[8].ships[1].blocks[1].coordinates[0].values=3;
	LayoutStorage[8].ships[1].blocks[0].coordinates[0].index=2;
	LayoutStorage[8].ships[1].blocks[1].coordinates[0].index=3;
	LayoutStorage[8].ships[1].blocks[0].hit=false;
	LayoutStorage[8].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[8].ships[1].blocks[1].hit=false;
	LayoutStorage[8].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[8].ships[2].blocks[0].coordinates[0].values=1;
	LayoutStorage[8].ships[2].blocks[1].coordinates[0].values=2;
	LayoutStorage[8].ships[2].blocks[0].coordinates[0].index=1;
	LayoutStorage[8].ships[2].blocks[1].coordinates[0].index=1;
	LayoutStorage[8].ships[2].blocks[0].hit=false;
	LayoutStorage[8].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[8].ships[2].blocks[1].hit=false;
	LayoutStorage[8].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[8].ships[3].blocks[0].coordinates[0].values=7;
	LayoutStorage[8].ships[3].blocks[1].coordinates[0].values=11;
	LayoutStorage[8].ships[3].blocks[0].coordinates[0].index=5;
	LayoutStorage[8].ships[3].blocks[1].coordinates[0].index=5;
	LayoutStorage[8].ships[3].blocks[0].hit=false;
	LayoutStorage[8].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[8].ships[3].blocks[1].hit=false;
	LayoutStorage[8].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[9].ships[0].blocks[0].coordinates[0].values=0;
	LayoutStorage[9].ships[0].blocks[1].coordinates[0].values=0;
	LayoutStorage[9].ships[0].blocks[0].coordinates[0].index=0;
	LayoutStorage[9].ships[0].blocks[1].coordinates[0].index=1;
	LayoutStorage[9].ships[0].blocks[0].hit=false;
	LayoutStorage[9].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[9].ships[0].blocks[1].hit=false;
	LayoutStorage[9].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[9].ships[1].blocks[0].coordinates[0].values=3;
	LayoutStorage[9].ships[1].blocks[1].coordinates[0].values=3;
	LayoutStorage[9].ships[1].blocks[0].coordinates[0].index=4;
	LayoutStorage[9].ships[1].blocks[1].coordinates[0].index=5;
	LayoutStorage[9].ships[1].blocks[0].hit=false;
	LayoutStorage[9].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[9].ships[1].blocks[1].hit=false;
	LayoutStorage[9].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[9].ships[2].blocks[0].coordinates[0].values=1;
	LayoutStorage[9].ships[2].blocks[1].coordinates[0].values=2;
	LayoutStorage[9].ships[2].blocks[0].coordinates[0].index=2;
	LayoutStorage[9].ships[2].blocks[1].coordinates[0].index=2;
	LayoutStorage[9].ships[2].blocks[0].hit=false;
	LayoutStorage[9].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[9].ships[2].blocks[1].hit=false;
	LayoutStorage[9].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[9].ships[3].blocks[0].coordinates[0].values=7;
	LayoutStorage[9].ships[3].blocks[1].coordinates[0].values=11;
	LayoutStorage[9].ships[3].blocks[0].coordinates[0].index=6;
	LayoutStorage[9].ships[3].blocks[1].coordinates[0].index=6;
	LayoutStorage[9].ships[3].blocks[0].hit=false;
	LayoutStorage[9].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[9].ships[3].blocks[1].hit=false;
	LayoutStorage[9].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[10].ships[0].blocks[0].coordinates[0].values=0;
	LayoutStorage[10].ships[0].blocks[1].coordinates[0].values=0;
	LayoutStorage[10].ships[0].blocks[0].coordinates[0].index=6;
	LayoutStorage[10].ships[0].blocks[1].coordinates[0].index=5;
	LayoutStorage[10].ships[0].blocks[0].hit=false;
	LayoutStorage[10].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[10].ships[0].blocks[1].hit=false;
	LayoutStorage[10].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[10].ships[1].blocks[0].coordinates[0].values=3;
	LayoutStorage[10].ships[1].blocks[1].coordinates[0].values=3;
	LayoutStorage[10].ships[1].blocks[0].coordinates[0].index=6;
	LayoutStorage[10].ships[1].blocks[1].coordinates[0].index=5;
	LayoutStorage[10].ships[1].blocks[0].hit=false;
	LayoutStorage[10].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[10].ships[1].blocks[1].hit=false;
	LayoutStorage[10].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[10].ships[2].blocks[0].coordinates[0].values=1;
	LayoutStorage[10].ships[2].blocks[1].coordinates[0].values=2;
	LayoutStorage[10].ships[2].blocks[0].coordinates[0].index=1;
	LayoutStorage[10].ships[2].blocks[1].coordinates[0].index=1;
	LayoutStorage[10].ships[2].blocks[0].hit=false;
	LayoutStorage[10].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[10].ships[2].blocks[1].hit=false;
	LayoutStorage[10].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[10].ships[3].blocks[0].coordinates[0].values=7;
	LayoutStorage[10].ships[3].blocks[1].coordinates[0].values=11;
	LayoutStorage[10].ships[3].blocks[0].coordinates[0].index=3;
	LayoutStorage[10].ships[3].blocks[1].coordinates[0].index=3;
	LayoutStorage[10].ships[3].blocks[0].hit=false;
	LayoutStorage[10].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[10].ships[3].blocks[1].hit=false;
	LayoutStorage[10].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[11].ships[0].blocks[0].coordinates[0].values=0;
	LayoutStorage[11].ships[0].blocks[1].coordinates[0].values=0;
	LayoutStorage[11].ships[0].blocks[0].coordinates[0].index=2;
	LayoutStorage[11].ships[0].blocks[1].coordinates[0].index=3;
	LayoutStorage[11].ships[0].blocks[0].hit=false;
	LayoutStorage[11].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[11].ships[0].blocks[1].hit=false;
	LayoutStorage[11].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[11].ships[1].blocks[0].coordinates[0].values=3;
	LayoutStorage[11].ships[1].blocks[1].coordinates[0].values=3;
	LayoutStorage[11].ships[1].blocks[0].coordinates[0].index=2;
	LayoutStorage[11].ships[1].blocks[1].coordinates[0].index=3;
	LayoutStorage[11].ships[1].blocks[0].hit=false;
	LayoutStorage[11].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[11].ships[1].blocks[1].hit=false;
	LayoutStorage[11].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[11].ships[2].blocks[0].coordinates[0].values=4;
	LayoutStorage[11].ships[2].blocks[1].coordinates[0].values=5;
	LayoutStorage[11].ships[2].blocks[0].coordinates[0].index=1;
	LayoutStorage[11].ships[2].blocks[1].coordinates[0].index=1;
	LayoutStorage[11].ships[2].blocks[0].hit=false;
	LayoutStorage[11].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[11].ships[2].blocks[1].hit=false;
	LayoutStorage[11].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[11].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[11].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[11].ships[3].blocks[0].coordinates[0].index=4;
	LayoutStorage[11].ships[3].blocks[1].coordinates[0].index=4;
	LayoutStorage[11].ships[3].blocks[0].hit=false;
	LayoutStorage[11].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[11].ships[3].blocks[1].hit=false;
	LayoutStorage[11].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[12].ships[0].blocks[0].coordinates[0].values=1;
	LayoutStorage[12].ships[0].blocks[1].coordinates[0].values=2;
	LayoutStorage[12].ships[0].blocks[0].coordinates[0].index=0;
	LayoutStorage[12].ships[0].blocks[1].coordinates[0].index=0;
	LayoutStorage[12].ships[0].blocks[0].hit=false;
	LayoutStorage[12].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[12].ships[0].blocks[1].hit=false;
	LayoutStorage[12].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[12].ships[1].blocks[0].coordinates[0].values=4;
	LayoutStorage[12].ships[1].blocks[1].coordinates[0].values=5;
	LayoutStorage[12].ships[1].blocks[0].coordinates[0].index=0;
	LayoutStorage[12].ships[1].blocks[1].coordinates[0].index=0;
	LayoutStorage[12].ships[1].blocks[0].hit=false;
	LayoutStorage[12].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[12].ships[1].blocks[1].hit=false;
	LayoutStorage[12].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[12].ships[2].blocks[0].coordinates[0].values=4;
	LayoutStorage[12].ships[2].blocks[1].coordinates[0].values=5;
	LayoutStorage[12].ships[2].blocks[0].coordinates[0].index=6;
	LayoutStorage[12].ships[2].blocks[1].coordinates[0].index=6;
	LayoutStorage[12].ships[2].blocks[0].hit=false;
	LayoutStorage[12].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[12].ships[2].blocks[1].hit=false;
	LayoutStorage[12].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[12].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[12].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[12].ships[3].blocks[0].coordinates[0].index=6;
	LayoutStorage[12].ships[3].blocks[1].coordinates[0].index=6;
	LayoutStorage[12].ships[3].blocks[0].hit=false;
	LayoutStorage[12].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[12].ships[3].blocks[1].hit=false;
	LayoutStorage[12].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[13].ships[0].blocks[0].coordinates[0].values=1;
	LayoutStorage[13].ships[0].blocks[1].coordinates[0].values=2;
	LayoutStorage[13].ships[0].blocks[0].coordinates[0].index=2;
	LayoutStorage[13].ships[0].blocks[1].coordinates[0].index=2;
	LayoutStorage[13].ships[0].blocks[0].hit=false;
	LayoutStorage[13].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[13].ships[0].blocks[1].hit=false;
	LayoutStorage[13].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[13].ships[1].blocks[0].coordinates[0].values=4;
	LayoutStorage[13].ships[1].blocks[1].coordinates[0].values=5;
	LayoutStorage[13].ships[1].blocks[0].coordinates[0].index=4;
	LayoutStorage[13].ships[1].blocks[1].coordinates[0].index=4;
	LayoutStorage[13].ships[1].blocks[0].hit=false;
	LayoutStorage[13].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[13].ships[1].blocks[1].hit=false;
	LayoutStorage[13].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[13].ships[2].blocks[0].coordinates[0].values=4;
	LayoutStorage[13].ships[2].blocks[1].coordinates[0].values=5;
	LayoutStorage[13].ships[2].blocks[0].coordinates[0].index=0;
	LayoutStorage[13].ships[2].blocks[1].coordinates[0].index=0;
	LayoutStorage[13].ships[2].blocks[0].hit=false;
	LayoutStorage[13].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[13].ships[2].blocks[1].hit=false;
	LayoutStorage[13].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[13].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[13].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[13].ships[3].blocks[0].coordinates[0].index=6;
	LayoutStorage[13].ships[3].blocks[1].coordinates[0].index=6;
	LayoutStorage[13].ships[3].blocks[0].hit=false;
	LayoutStorage[13].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[13].ships[3].blocks[1].hit=false;
	LayoutStorage[13].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[14].ships[0].blocks[0].coordinates[0].values=0;
	LayoutStorage[14].ships[0].blocks[1].coordinates[0].values=0;
	LayoutStorage[14].ships[0].blocks[0].coordinates[0].index=1;
	LayoutStorage[14].ships[0].blocks[1].coordinates[0].index=2;
	LayoutStorage[14].ships[0].blocks[0].hit=false;
	LayoutStorage[14].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[14].ships[0].blocks[1].hit=false;
	LayoutStorage[14].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[14].ships[1].blocks[0].coordinates[0].values=3;
	LayoutStorage[14].ships[1].blocks[1].coordinates[0].values=3;
	LayoutStorage[14].ships[1].blocks[0].coordinates[0].index=4;
	LayoutStorage[14].ships[1].blocks[1].coordinates[0].index=5;
	LayoutStorage[14].ships[1].blocks[0].hit=false;
	LayoutStorage[14].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[14].ships[1].blocks[1].hit=false;
	LayoutStorage[14].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[14].ships[2].blocks[0].coordinates[0].values=4;
	LayoutStorage[14].ships[2].blocks[1].coordinates[0].values=5;
	LayoutStorage[14].ships[2].blocks[0].coordinates[0].index=0;
	LayoutStorage[14].ships[2].blocks[1].coordinates[0].index=0;
	LayoutStorage[14].ships[2].blocks[0].hit=false;
	LayoutStorage[14].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[14].ships[2].blocks[1].hit=false;
	LayoutStorage[14].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[14].ships[3].blocks[0].coordinates[0].values=1;
	LayoutStorage[14].ships[3].blocks[1].coordinates[0].values=2;
	LayoutStorage[14].ships[3].blocks[0].coordinates[0].index=6;
	LayoutStorage[14].ships[3].blocks[1].coordinates[0].index=6;
	LayoutStorage[14].ships[3].blocks[0].hit=false;
	LayoutStorage[14].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[14].ships[3].blocks[1].hit=false;
	LayoutStorage[14].ships[3].blocks[1].length_of_block=1;

	LayoutStorage[15].ships[0].blocks[0].coordinates[0].values=0;
	LayoutStorage[15].ships[0].blocks[1].coordinates[0].values=0;
	LayoutStorage[15].ships[0].blocks[0].coordinates[0].index=0;
	LayoutStorage[15].ships[0].blocks[1].coordinates[0].index=1;
	LayoutStorage[15].ships[0].blocks[0].hit=false;
	LayoutStorage[15].ships[0].blocks[0].length_of_block=1;
	LayoutStorage[15].ships[0].blocks[1].hit=false;
	LayoutStorage[15].ships[0].blocks[1].length_of_block=1;

	LayoutStorage[15].ships[1].blocks[0].coordinates[0].values=3;
	LayoutStorage[15].ships[1].blocks[1].coordinates[0].values=3;
	LayoutStorage[15].ships[1].blocks[0].coordinates[0].index=0;
	LayoutStorage[15].ships[1].blocks[1].coordinates[0].index=1;
	LayoutStorage[15].ships[1].blocks[0].hit=false;
	LayoutStorage[15].ships[1].blocks[0].length_of_block=1;
	LayoutStorage[15].ships[1].blocks[1].hit=false;
	LayoutStorage[15].ships[1].blocks[1].length_of_block=1;

	LayoutStorage[15].ships[2].blocks[0].coordinates[0].values=6;
	LayoutStorage[15].ships[2].blocks[1].coordinates[0].values=6;
	LayoutStorage[15].ships[2].blocks[0].coordinates[0].index=5;
	LayoutStorage[15].ships[2].blocks[1].coordinates[0].index=6;
	LayoutStorage[15].ships[3].blocks[0].coordinates[1].values=10;
	LayoutStorage[15].ships[3].blocks[1].coordinates[1].values=10;
	LayoutStorage[15].ships[3].blocks[0].coordinates[1].index=5;
	LayoutStorage[15].ships[3].blocks[1].coordinates[1].index=6;
	LayoutStorage[15].ships[2].blocks[0].hit=false;
	LayoutStorage[15].ships[2].blocks[0].length_of_block=1;
	LayoutStorage[15].ships[2].blocks[1].hit=false;
	LayoutStorage[15].ships[2].blocks[1].length_of_block=1;

	LayoutStorage[15].ships[3].blocks[0].coordinates[0].values=6;
	LayoutStorage[15].ships[3].blocks[1].coordinates[0].values=6;
	LayoutStorage[15].ships[3].blocks[0].coordinates[0].index=0;
	LayoutStorage[15].ships[3].blocks[1].coordinates[0].index=1;
	LayoutStorage[15].ships[3].blocks[0].coordinates[1].values=10;
	LayoutStorage[15].ships[3].blocks[1].coordinates[1].values=10;
	LayoutStorage[15].ships[3].blocks[0].coordinates[1].index=0;
	LayoutStorage[15].ships[3].blocks[1].coordinates[1].index=1;
	LayoutStorage[15].ships[3].blocks[0].hit=false;
	LayoutStorage[15].ships[3].blocks[0].length_of_block=1;
	LayoutStorage[15].ships[3].blocks[1].hit=false;
	LayoutStorage[15].ships[3].blocks[1].length_of_block=1;

}
