#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
    // This plugin doesn't accept any command line arguments;
    // just define a single dummy field.
    int dummy;
};

//Returns plugin name
const char *get_plugin_name(void) {
    return "mirrorv";
}

//Returns plugin description
const char *get_plugin_desc(void) {
    return "mirror image vertically";
}

//Parse arguments for later use
void *parse_arguments(int num_args, char *args[]) {
    (void) args; // this is just to avoid a warning about an unused parameter

    if (num_args != 0) {
        return NULL;
    }
    return calloc(1, sizeof(struct Arguments));
}

//Returns an image with an effect based off of the plugin specification
struct Image *transform_image(struct Image *source, void *arg_data) {
    struct Arguments *args = arg_data;

    // Allocate a result Image
    struct Image *out = img_create(source->width, source->height);
    if (!out) { //If out wasn't allocated, free args and return null
        free(args);
        return NULL;
    }
    int rows = (int) source->height;
    int cols = (int) source->width;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            int newi = (rows - 1) - i; //Goes through rows backwards
            out->data[(i * cols) + j] = source->data[(newi * cols) + j]; //Replace topmost pixels with bottom most to mirror
        }
    }

    free(args);

    return out;
}


