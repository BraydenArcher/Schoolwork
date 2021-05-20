#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
    // This plugin doesn't accept any command line arguments;
    // just define a single dummy field.
    long n;
};

//Returns plugin name
const char *get_plugin_name(void) {
    return "tile";
}

//Returns plugin description
const char *get_plugin_desc(void) {
    return "tile source image in an NxN arrangement";
}

//Parse arguments for later use
void *parse_arguments(int num_args, char *args[]) {
    if (num_args == 0 || num_args > 1) {
        return NULL;
    }
    long argument = strtol(args[0], NULL, 10);
    if(argument <= 0) {
        return NULL;
    }
    struct Arguments *n = calloc(1, sizeof(struct Arguments));
    n->n = argument;
    return n;
}

//Returns an image with an effect based off of the plugin specification
struct Image *transform_image(struct Image *source, void *arg_data) {
    struct Arguments *args = arg_data;

    // Allocate a result Image
    struct Image *out = img_create(source->width, source->height);
    if (!out) {
        free(args);
        return NULL;
    }
    long n = args->n;
    int rows = (int) source->height; //Set width and height as rows and cols respectively
    int cols = (int) source->width;
    int *tilesizesi = malloc(sizeof(int) * n); //Malloc two arrays that will contain size of each tile
    int *tilesizesj = malloc(sizeof(int) * n);
    int remainderi = rows % (int) n; //Determine the remainder of each axis
    int remainderj = cols % (int) n;

    for(int i = 0; i < n; i++) {
        tilesizesi[i] = rows / (int) n; //Generates initial tile size for i and j
        tilesizesj[i] = cols / (int) n;
        if(remainderi > 0) { //If there is still a remainder add one to the size for i and j
            tilesizesi[i]++;
            remainderi--;
        }
        if(remainderj > 0) {
            tilesizesj[i]++;
            remainderj--;
        }
        if(i != 0) { //If i is not 0, then add the previous index to the current one to get the value the new tile should start for each axis
            tilesizesi[i] += tilesizesi[i-1];
            tilesizesj[i] += tilesizesj[i-1];
        }
    }


    int newi = 0;
    int rowindex = 0;
    for(int i = 0; i < rows; i++) { //Loop through the image
        if(i == tilesizesi[rowindex]) { //If i equals one of the breakpoints, reset and make another mini image
            rowindex++;
            newi = 0;
        }
        int newj = 0; //Reset j values each iteration
        int colindex = 0; //Reset index of tile size each time for y axis
        for(int j = 0; j < cols; j++) {
            if(j == tilesizesj[colindex]) { //If j equals one of the breakpoints, reset and make another mini image
                colindex++;
                newj = 0;
            }
            out->data[(i*cols) + j] = source->data[(newi * cols) + newj]; //Set the output at the current index to the source that is calculated from the nth pixel
            newj += (int) n; //Add n each iteration
        }
    newi += (int) n; //Add n each iteration
    }
    free(tilesizesi);
    free(tilesizesj);
    free(args);
    return out;
}


