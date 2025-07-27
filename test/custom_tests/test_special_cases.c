#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/image.h"

// Add color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Create a border-only image (black with white border)
struct image* create_border_image(int width, int height, int border_size) {
    struct image* img = image_init(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;

            // Set white for borders, black for interior
            bool is_border = x < border_size || y < border_size ||
                             x >= width - border_size ||
                             y >= height - border_size;

            img->pixels[idx].r = is_border ? 255 : 0;
            img->pixels[idx].g = is_border ? 255 : 0;
            img->pixels[idx].b = is_border ? 255 : 0;
        }
    }
    return img;
}

// Create an image with a single bright pixel in the center
struct image* create_hot_pixel(int width, int height) {
    struct image* img = image_init(width, height);

    // Fill with black
    for (int i = 0; i < width * height; i++) {
        img->pixels[i].r = 0;
        img->pixels[i].g = 0;
        img->pixels[i].b = 0;
    }

    // Set center pixel to white
    int center_x = width / 2;
    int center_y = height / 2;
    int center_idx = center_y * width + center_x;
    img->pixels[center_idx].r = 255;
    img->pixels[center_idx].g = 255;
    img->pixels[center_idx].b = 255;

    return img;
}

void test_border_images(void) {
    // Test with different border sizes
    int sizes[] = {1, 5, 10, 25, 50};
    char test_msg[100];

    for (int i = 0; i < 5; i++) {
        int border_size = sizes[i];
        struct image* border_img = create_border_image(100, 100, border_size);

        uint8_t brightness = image_brightness(border_img);

        // Calculate expected brightness - percentage of white pixels
        float border_percentage = 1.0f - ((100.0f - 2.0f * border_size) *
                                          (100.0f - 2.0f * border_size)) /
                                             (100.0f * 100.0f);
        uint8_t expected = (uint8_t)(border_percentage * 255.0f);

        // Allow small margin of error
        assert(abs((int)brightness - (int)expected) <= 5);

        // Replace sprintf with snprintf to avoid the warning
        snprintf(test_msg, sizeof(test_msg),
                 "Border image test with %d pixel border", border_size);
        printf("%sPASSED%s %s\n", GREEN, RESET, test_msg);

        image_destroy(border_img);
    }
}

void test_hot_pixel(void) {
    // Test with a single bright pixel in different sized images
    int sizes[] = {3, 10, 100, 500};

    for (int i = 0; i < 4; i++) {
        int size = sizes[i];
        struct image* hot_img = create_hot_pixel(size, size);

        uint8_t brightness = image_brightness(hot_img);

        // Expected brightness is 255 / (size*size)
        uint8_t expected = (uint8_t)(255.0f / (size * size));

        // Allow small margin of error
        assert(abs((int)brightness - (int)expected) <= 1);

        printf("%sPASSED%s Hot pixel test with %dx%d image\n", GREEN, RESET,
               size, size);

        image_destroy(hot_img);
    }
}

int main(void) {
    test_border_images();
    test_hot_pixel();
    return 0;
}
