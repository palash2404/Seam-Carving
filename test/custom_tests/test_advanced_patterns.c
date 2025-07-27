#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../src/image.h"

// Add color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Create a gradient image that transitions from top-left to bottom-right
struct image* create_diagonal_gradient(int width, int height) {
    struct image* img = image_init(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate position as percentage across diagonal
            float diag_pos = (float)(x + y) / (width + height - 2);
            uint8_t val = (uint8_t)(diag_pos * 255);

            int idx = y * width + x;
            img->pixels[idx].r = val;
            img->pixels[idx].g = val;
            img->pixels[idx].b = val;
        }
    }
    return img;
}

// Create a checkerboard pattern with alternating colors
struct image* create_checkerboard(int width, int height, int square_size) {
    struct image* img = image_init(width, height);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            int is_white = ((x / square_size) + (y / square_size)) % 2 == 0;
            img->pixels[idx].r = is_white ? 255 : 0;
            img->pixels[idx].g = is_white ? 255 : 0;
            img->pixels[idx].b = is_white ? 255 : 0;
        }
    }
    return img;
}

// Create a random noise image
struct image* create_random_noise(int width, int height) {
    struct image* img = image_init(width, height);
    srand(42);  // Fixed seed for reproducibility

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            img->pixels[idx].r = rand() % 256;
            img->pixels[idx].g = rand() % 256;
            img->pixels[idx].b = rand() % 256;
        }
    }
    return img;
}

// Test brightness on different image patterns
void test_brightness_patterns(void) {
    // Test with diagonal gradient
    struct image* gradient = create_diagonal_gradient(200, 200);
    uint8_t gradient_brightness = image_brightness(gradient);

    // Expected brightness should be around 127-128 for a linear gradient
    assert(gradient_brightness > 125 && gradient_brightness < 130);
    printf("%sPASSED%s Diagonal gradient brightness test\n", GREEN, RESET);
    image_destroy(gradient);

    // Test with checkerboard pattern (50% white, 50% black)
    struct image* checkerboard = create_checkerboard(200, 200, 20);
    uint8_t checkerboard_brightness = image_brightness(checkerboard);

    // Expected brightness should be close to 127-128 (average of 0 and 255)
    assert(checkerboard_brightness > 125 && checkerboard_brightness < 130);
    printf("%sPASSED%s Checkerboard pattern brightness test\n", GREEN, RESET);
    image_destroy(checkerboard);

    // Test with random noise
    struct image* noise = create_random_noise(200, 200);
    uint8_t noise_brightness = image_brightness(noise);

    // Random noise should have brightness around 128, but with some variance
    assert(noise_brightness > 115 && noise_brightness < 140);
    printf("%sPASSED%s Random noise brightness test\n", GREEN, RESET);
    image_destroy(noise);
}

// Test images with extreme aspect ratios
void test_extreme_aspect_ratios(void) {
    // Test with very wide image (1000x10)
    struct image* wide = create_diagonal_gradient(1000, 10);
    uint8_t wide_brightness = image_brightness(wide);
    assert(wide_brightness >= 0 && wide_brightness <= 255);
    printf("%sPASSED%s Very wide image test\n", GREEN, RESET);
    image_destroy(wide);

    // Test with very tall image (10x1000)
    struct image* tall = create_diagonal_gradient(10, 1000);
    uint8_t tall_brightness = image_brightness(tall);
    assert(tall_brightness >= 0 && tall_brightness <= 255);
    printf("%sPASSED%s Very tall image test\n", GREEN, RESET);
    image_destroy(tall);
}

int main(void) {
    test_brightness_patterns();
    test_extreme_aspect_ratios();
    return 0;
}
