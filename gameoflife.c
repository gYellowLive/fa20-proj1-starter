/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

#define LSB_MASK_8BITS 1


//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	uint32_t max_rows = image->rows;
	uint32_t max_cols = image->cols;

	Color* new_color = malloc(sizeof(Color));
	if (new_color == NULL){
            exit(-1);
    }

	uint16_t R_life_counter = 0;
	uint16_t G_life_counter = 0;
	uint16_t B_life_counter = 0;
	for (int i = -1; i<2; i++){
		for (int j = -1; j<2; j++){
			int curr_row = (row + max_rows + i) % max_rows;
			int curr_col = (col + max_cols + j) % max_cols;

			Color* cell_to_check = (*(image->image)) + curr_row*(max_cols) + curr_col; 

			uint8_t R_value = cell_to_check->R;
			uint8_t G_value = cell_to_check->G;
			uint8_t B_value = cell_to_check->B;
			// printf("row = %u, col=%u, mask is: %u", row, col, (B_value & 1) );
			if (R_value >0){
				R_life_counter += 1;
			}
			if (G_value >0){
				G_life_counter += 1;
			}
			if (B_value >0){
				B_life_counter += 1;
			}	

		}
	}

	Color* nominal_cell_to_check = (*(image->image)) + row*(max_cols) + col;
	uint8_t nominal_cell_R_value = nominal_cell_to_check->R;
	uint8_t nominal_cell_G_value = nominal_cell_to_check->G;
	uint8_t nominal_cell_B_value = nominal_cell_to_check->B;

	uint32_t R_key = 1 << R_life_counter;
	uint32_t G_key = 1 << G_life_counter;
	uint32_t B_key = 1 << B_life_counter;


	if (nominal_cell_R_value  > 0){
		R_key = R_key >> 1; //in the for loop, the own cell is counted
		R_key = R_key << 9;
	}
	if (nominal_cell_G_value  > 0){
		G_key = G_key >> 1; //in the for loop, the own cell is counted
		G_key = G_key << 9;
	}
	if (nominal_cell_B_value  > 0){
		B_key = B_key >> 1; //in the for loop, the own cell is counted
		B_key = B_key << 9;
	}

	if (R_key & rule){
		new_color->R = 255;
	}
	else{
		new_color->R = 0;
	}

	if (G_key & rule){
		new_color->G = 255;
	}
	else{
		new_color->G = 0;
	}

	if (B_key & rule){
		new_color->B = 255;
	}
	else{
		new_color->B = 0;
	}


	return new_color;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image* result = malloc(sizeof(Image));
	Color** new_image = malloc(sizeof(Color*));
	Color* pixels = malloc(sizeof(Color)* (image->rows) *(image->cols));
	if (result == NULL || new_image == NULL || pixels == NULL){
            exit(-1);
    }
    *new_image = pixels;

	(*result).image = new_image;
    (*result).rows = image->rows;
    (*result).cols = image->cols;

	uint64_t curr_index = 0;
	uint32_t curr_row = curr_index/ (image->cols);
	uint32_t curr_col = curr_index % (image->cols);

	while(curr_index < ((image->rows) *(image->cols)) ){
		curr_row = curr_index/ (image->cols);
		curr_col = curr_index % (image->cols);
		Color* generated_color = evaluateOneCell(image, curr_row, curr_col, rule);
		(*(pixels+curr_index)) = *generated_color;
		free(generated_color);
		curr_index = curr_index + 1;
	}

	return result;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
//process command line input
void processCLI(int argc, char **argv, char **filename) 
{
	if (argc != 3) {
		printf("usage: %s filename\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	*filename = argv[1];
}

int main(int argc, char **argv)
{
	Image *image;
	char *filename;
	processCLI(argc,argv,&filename);

	image = readData(filename);
	uint32_t rule = 0x1808;
	Image* new_image = life(image, rule);

	writeData(new_image);

	// printf(new_image);
	// printPixels(new_image);

	freeImage(image);
	freeImage(new_image);

	return 0;
}

