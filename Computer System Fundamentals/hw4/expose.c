#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
    long double factor;
};

//Returns plugin name
const char *get_plugin_name(void) {
    return "expose";
}

//Returns plugin description
const char *get_plugin_desc(void) {
    return "adjust the intensity of all pixels";
}

//Parse arguments for later use
void *parse_arguments(int num_args, char *args[]) {
    if (num_args != 1) { //Return null if the argument count is wrong
        return NULL;
    }
    long double argument = strtold(args[0], NULL);
    if(argument < 0) {
        return NULL;
    }
    struct Arguments *factor = calloc(1, sizeof(struct Arguments));
    factor->factor = argument;
    return factor;
}

//Helper function to expose each pixel
static uint32_t expose(uint32_t pixel, long double factor) {
    uint8_t r, g, b, a;
    img_unpack_pixel(pixel, &r, &g, &b, &a);
    uint64_t newr = r * factor; //uint64's make sure overflow doesnt occur
    uint64_t newg = g * factor;
    uint64_t newb = b * factor;
    if(newr > 255) { //Makes sure the value doesnt exceed 255
        r = 255;
    } else {
        r = newr;
    }
    if(newg > 255) {
        g = 255;
    } else {
        g = newg;
    }
    if(newb > 255) {
        b = 255;
    } else {
        b = newb;
    }
    return img_pack_pixel(r, g, b, a); //Return packed pixel
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

    unsigned num_pixels = source->width * source->height;
    for (unsigned i = 0; i < num_pixels; i++) {
        out->data[i] = expose(source->data[i], args->factor); //Expose each pixel in the array using helper function
    }
    free(args); //Free arguments struct
    return out;
}


