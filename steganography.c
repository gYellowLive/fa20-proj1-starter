/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

#define LSB_MASK_8BITS 1 

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color* cell_to_check = (*(image->image)) + row*(image->cols) + col; 
	Color* new_color = malloc(sizeof(Color));
	if (new_color == NULL){
            exit(-1);
    }
	
	uint8_t B_value = cell_to_check->B;
	// printf("row = %u, col=%u, mask is: %u", row, col, (B_value & 1) );
	if (B_value & LSB_MASK_8BITS){
		new_color->R = 255;
		new_color->G = 255;
		new_color->B = 255;
	}
	else{
		new_color->R = 0;
		new_color->G = 0;
		new_color->B = 0;
	}

	return new_color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
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
		Color* generated_color = evaluateOnePixel(image, curr_row, curr_col);
		(*(pixels+curr_index)) = *generated_color;
		free(generated_color);
		curr_index = curr_index + 1;
	}


	return result;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/

void printPixels(Image *image)
{

	uint32_t rows = (*image).rows;
	uint32_t cols = (*image).cols;
	// now print the contents
	Color* curr_pixel = (*((*image).image));
	uint64_t curr_index = 0;
	uint8_t temp_R;
    uint8_t temp_G;
    uint8_t temp_B;

    uint32_t counter_row_limit = 0;
	while(curr_index < rows*cols){

        temp_R = (*(curr_pixel+curr_index)).R ;
        temp_G = (*(curr_pixel+curr_index)).G ;
        temp_B = (*(curr_pixel+curr_index)).B ;
		curr_index = curr_index + 1;
		counter_row_limit = counter_row_limit + 1;
		// printf("%3hhu %3hhu %3hhu", temp_R, temp_G,temp_B );

		if (temp_B>0){
			printf("*");
		}
		else{
			printf(" ");
		}

        if (counter_row_limit == rows){
            counter_row_limit = 0;
            printf("\n");
        }
        else{
            // printf("   "); // the spaces between columns
        }
	}

}


//process command line input
void processCLI(int argc, char **argv, char **filename) 
{
	if (argc != 2) {
		printf("usage: %s filename\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
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
	Image* new_image = steganography(image);

	//writeData(new_image);

	// printf(new_image);
	// printPixels(new_image);

	freeImage(image);
	freeImage(new_image);

	return 0;
}
