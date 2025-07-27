#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/image.h"

// Add color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Helper function to cut the right side of an image (same as in
// test_image_cutting.c)
struct image* cut_image_right(struct image* original, int cut_amount) {
    int new_width = original->w - cut_amount;
    if (new_width <= 0) {
        // Return empty image if cutting everything
        return image_init(0, original->h);
    }

    struct image* result = image_init(new_width, original->h);

    for (int y = 0; y < original->h; y++) {
        for (int x = 0; x < new_width; x++) {
            int orig_idx = y * original->w + x;
            int new_idx = y * new_width + x;
            result->pixels[new_idx] = original->pixels[orig_idx];
        }
    }

    return result;
}

void test_empty_image(void) {
    // Test with 0x0 image
    struct image* empty_img = image_init(0, 0);

    // These functions should handle empty images gracefully
    uint8_t brightness = image_brightness(empty_img);
    assert(brightness == 0);  // Expected behavior for empty image

    // Test cutting an empty image
    struct image* cut_empty = cut_image_right(empty_img, 10);
    assert(cut_empty->w == 0);
    assert(cut_empty->h == 0);

    image_destroy(empty_img);
    image_destroy(cut_empty);

    printf("%sPASSED%s Empty image test\n", GREEN, RESET);
}

void test_single_pixel_image(void) {
    // Test with 1x1 image
    struct image* single_pixel = image_init(1, 1);
    single_pixel->pixels[0].r = 255;
    single_pixel->pixels[0].g = 128;
    single_pixel->pixels[0].b = 64;

    // Test brightness calculation
    uint8_t brightness = image_brightness(single_pixel);
    assert(brightness == 149);  // (255 + 128 + 64)/3 = 149

    // Test cutting the single pixel image
    struct image* cut_result = cut_image_right(single_pixel, 1);
    assert(cut_result->w == 0);
    assert(cut_result->h == 1);

    image_destroy(single_pixel);
    image_destroy(cut_result);

    printf("%sPASSED%s Single pixel image test\n", GREEN, RESET);
}

int main(void) {
    test_empty_image();
    test_single_pixel_image();
    return 0;
}
