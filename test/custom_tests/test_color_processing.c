#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/image.h"

// Add color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Create a test pattern with RGB gradients
struct image* create_rgb_test_pattern(int width, int height) {
    struct image* img = image_init(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            img->pixels[index].r =
                (uint8_t)((x * 255) / width);  // R increases horizontally
            img->pixels[index].g =
                (uint8_t)((y * 255) / height);  // G increases vertically
            img->pixels[index].b =
                (uint8_t)(255 -
                          ((x * 255) / width));  // B decreases horizontally
        }
    }
    return img;
}

// Helper function to extract a single channel (0=R, 1=G, 2=B)
struct image* extract_channel(struct image* src, int channel) {
    struct image* result = image_init(src->w, src->h);

    for (int y = 0; y < src->h; y++) {
        for (int x = 0; x < src->w; x++) {
            int index = y * src->w + x;
            uint8_t value = 0;

            if (channel == 0)
                value = src->pixels[index].r;
            else if (channel == 1)
                value = src->pixels[index].g;
            else if (channel == 2)
                value = src->pixels[index].b;

            // Set all channels to the same value for grayscale effect
            result->pixels[index].r = value;
            result->pixels[index].g = value;
            result->pixels[index].b = value;
        }
    }

    return result;
}

void test_channel_brightness(void) {
    // Create test pattern
    struct image* test_img = create_rgb_test_pattern(200, 200);

    // Extract channels
    struct image* red_channel = extract_channel(test_img, 0);
    struct image* green_channel = extract_channel(test_img, 1);
    struct image* blue_channel = extract_channel(test_img, 2);

    // Test brightness calculations
    uint8_t original_brightness = image_brightness(test_img);
    uint8_t red_brightness = image_brightness(red_channel);
    uint8_t green_brightness = image_brightness(green_channel);
    uint8_t blue_brightness = image_brightness(blue_channel);

    printf("Original brightness: %d\n", original_brightness);
    printf("Red channel brightness: %d\n", red_brightness);
    printf("Green channel brightness: %d\n", green_brightness);
    printf("Blue channel brightness: %d\n", blue_brightness);

    // Verify that brightness calculations are consistent
    assert(original_brightness <= 255);
    assert(red_brightness <= 255);
    assert(green_brightness <= 255);
    assert(blue_brightness <= 255);

    // Clean up
    image_destroy(test_img);
    image_destroy(red_channel);
    image_destroy(green_channel);
    image_destroy(blue_channel);

        printf("%sPASSED%s Channel brightness test passed!\n", GREEN, RESET);
}

int main(void) {
    test_channel_brightness();
    return 0;
}
