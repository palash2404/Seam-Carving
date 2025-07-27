#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/image.h"

// Add color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Create an image with alternating pixel values (checkerboard pattern)
struct image* create_checkerboard(int width, int height, uint8_t val1,
                                  uint8_t val2) {
    struct image* img = image_init(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            // Alternate between val1 and val2
            uint8_t val = ((x + y) % 2 == 0) ? val1 : val2;
            img->pixels[index].r = val;
            img->pixels[index].g = val;
            img->pixels[index].b = val;
        }
    }

    return img;
}

void test_extreme_values(void) {
    // Test with extreme value checkerboard patterns
    struct image* high_contrast = create_checkerboard(100, 100, 0, 255);
    struct image* subtle = create_checkerboard(100, 100, 127, 128);
    struct image* all_bright = create_checkerboard(100, 100, 254, 255);
    struct image* all_dark = create_checkerboard(100, 100, 0, 1);

    // Calculate brightness for each image
    uint8_t high_contrast_brightness = image_brightness(high_contrast);
    uint8_t subtle_brightness = image_brightness(subtle);
    uint8_t all_bright_brightness = image_brightness(all_bright);
    uint8_t all_dark_brightness = image_brightness(all_dark);

    printf("High contrast brightness: %d\n", high_contrast_brightness);
    printf("Subtle contrast brightness: %d\n", subtle_brightness);
    printf("All bright brightness: %d\n", all_bright_brightness);
    printf("All dark brightness: %d\n", all_dark_brightness);

    // Verify results
    assert(high_contrast_brightness == 127);  // Average of 0 and 255
    assert(subtle_brightness == 127);         // Average of 127 and 128
    assert(all_bright_brightness >= 254);     // Should be ~254-255
    assert(all_dark_brightness <= 1);         // Should be ~0-1

    // Test odd dimensions
    struct image* odd_size = create_checkerboard(101, 99, 0, 255);
    uint8_t odd_size_brightness = image_brightness(odd_size);

    // With odd dimensions, not exactly half pixels are 0 and half are 255
    assert(odd_size_brightness > 126 && odd_size_brightness < 128);

    // Clean up
    image_destroy(high_contrast);
    image_destroy(subtle);
    image_destroy(all_bright);
    image_destroy(all_dark);
    image_destroy(odd_size);

    printf("%sPASSED%s Extreme value test passed!\n", GREEN, RESET);
}

void test_unusual_dimensions(void) {
    // Test with unusual image dimensions
    struct image* very_wide = image_init(1000, 1);
    struct image* very_tall = image_init(1, 1000);
    struct image* single_row = image_init(100, 1);
    struct image* single_column = image_init(1, 100);

    // Fill with a simple pattern
    for (int i = 0; i < 1000; i++) {
        if (i < very_wide->w) {
            very_wide->pixels[i].r = i % 256;
            very_wide->pixels[i].g = i % 256;
            very_wide->pixels[i].b = i % 256;
        }

        if (i < very_tall->h) {
            very_tall->pixels[i].r = i % 256;
            very_tall->pixels[i].g = i % 256;
            very_tall->pixels[i].b = i % 256;
        }

        if (i < single_row->w) {
            single_row->pixels[i].r = i % 256;
            single_row->pixels[i].g = i % 256;
            single_row->pixels[i].b = i % 256;
        }

        if (i < single_column->h) {
            single_column->pixels[i].r = i % 256;
            single_column->pixels[i].g = i % 256;
            single_column->pixels[i].b = i % 256;
        }
    }

    // Test brightness
    uint8_t very_wide_brightness = image_brightness(very_wide);
    uint8_t very_tall_brightness = image_brightness(very_tall);
    uint8_t single_row_brightness = image_brightness(single_row);
    uint8_t single_column_brightness = image_brightness(single_column);

    printf("Very wide image brightness: %d\n", very_wide_brightness);
    printf("Very tall image brightness: %d\n", very_tall_brightness);
    printf("Single row image brightness: %d\n", single_row_brightness);
    printf("Single column image brightness: %d\n", single_column_brightness);

    // All should have valid brightness values
    assert(very_wide_brightness >= 0 && very_wide_brightness <= 255);
    assert(very_tall_brightness >= 0 && very_tall_brightness <= 255);
    assert(single_row_brightness >= 0 && single_row_brightness <= 255);
    assert(single_column_brightness >= 0 && single_column_brightness <= 255);

    // Clean up
    image_destroy(very_wide);
    image_destroy(very_tall);
    image_destroy(single_row);
    image_destroy(single_column);

    printf("%sPASSED%s Unusual dimensions test passed!\n", GREEN, RESET);
}

int main(void) {
    test_extreme_values();
    test_unusual_dimensions();
    return 0;
}
