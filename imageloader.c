/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
        //read from the ppm file the row and col number
        FILE *fp = fopen(filename, "r");
        char buf[2];// this should have the "P3" at the end
        fscanf(fp, "%s",buf);
        uint32_t rows;
        uint32_t cols;
        fscanf(fp, "%u %u", &cols, &rows);
        uint8_t max_color_val;
        fscanf(fp, "%hhu", &max_color_val);

        // printf("buf reads = %s, rows = %u, cols = %u, max_color_val = %u\n", buf , rows, cols, max_color_val);

        //allocate storage space
        Color** image = malloc(sizeof(Color*));
        Color* pixels = malloc(sizeof(Color)*rows*cols);
        // check for malloc integrity
        if (image == NULL || pixels == NULL){
            exit(-1);
        }
        *image = pixels;

        

        uint8_t temp_R;
        uint8_t temp_G;
        uint8_t temp_B;
        uint64_t curr_index = 0;

        while (fscanf(fp, "%hhu %hhu %hhu", &temp_R, &temp_G, &temp_B)==3){
        	(*(pixels+curr_index)).R = temp_R;
        	(*(pixels+curr_index)).G = temp_G;
        	(*(pixels+curr_index)).B = temp_B;
			curr_index = curr_index + 1;
        }

		fclose(fp);

        Image* result = malloc(sizeof(Image));
        // check for malloc integrity
        if (result == NULL){
            exit(-1);
        }
        (*result).image = image;
        (*result).rows = rows;
        (*result).cols = cols;

        return result;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//first print the P3 format
	printf("P3\n");
	uint32_t rows = (*image).rows;
	uint32_t cols = (*image).cols;
	// print the row sna docls for sizes
	printf("%u %u\n", cols, rows);

    // print the max value
    printf("255\n");

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
		printf("%3hhu %3hhu %3hhu", temp_R, temp_G,temp_B );

        if (counter_row_limit == cols){
            counter_row_limit = 0;
            printf("\n");
        }
        else{
            printf("   "); // the spaces between columns
        }
	}

}

//Frees an image
void freeImage(Image *image)
{
    free(*((*image).image)); //free the pixels
	free((*image).image); 
	free(image);
}