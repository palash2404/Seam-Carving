#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/image.h"

// Add color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Helper function to create a solid color image
struct image* create_solid_image(int w, int h, uint8_t r, uint8_t g,
                                 uint8_t b) {
    struct image* img = image_init(w, h);
    for (int i = 0; i < w * h; i++) {
        img->pixels[i].r = r;
        img->pixels[i].g = g;
        img->pixels[i].b = b;
    }
    return img;
}

void test_brightness_solid_colors(void) {
    // Test with solid color images
    struct image* white_img = create_solid_image(100, 100, 255, 255, 255);
    struct image* black_img = create_solid_image(100, 100, 0, 0, 0);
    struct image* gray_img = create_solid_image(100, 100, 128, 128, 128);

    uint8_t white_brightness = image_brightness(white_img);
    uint8_t black_brightness = image_brightness(black_img);
    uint8_t gray_brightness = image_brightness(gray_img);

    assert(white_brightness == 255);  // Should be 255
    assert(black_brightness == 0);    // Should be 0
    assert(gray_brightness == 128);   // Should be 128

    image_destroy(white_img);
    image_destroy(black_img);
    image_destroy(gray_img);

    printf("%sPASSED%s Brightness solid colors test passed!\n", GREEN, RESET);
}

int main(void) {
    test_brightness_solid_colors();
    return 0;
}
