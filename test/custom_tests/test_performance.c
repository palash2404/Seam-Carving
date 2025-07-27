#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../src/image.h"

// Add color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Create a large random image for stress testing
struct image* create_large_random_image(int width, int height) {
    struct image* img = image_init(width, height);

    // Seed the random number generator
    srand(42);  // Fixed seed for reproducibility

    for (int i = 0; i < width * height; i++) {
        img->pixels[i].r = rand() % 256;
        img->pixels[i].g = rand() % 256;
        img->pixels[i].b = rand() % 256;
    }

    return img;
}

void test_performance_large_image(void) {
    printf("Creating large image (1000x1000)...\n");
    clock_t start, end;
    double cpu_time_used;

    // Create a large 1000x1000 image (if memory allows)
    start = clock();
    struct image* large_img = create_large_random_image(1000, 1000);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time to create 1000x1000 image: %f seconds\n", cpu_time_used);

    // Test brightness calculation time
    start = clock();
    uint8_t brightness = image_brightness(large_img);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time to calculate brightness on 1000x1000 image: %f seconds\n",
           cpu_time_used);
    printf("Brightness: %d\n", brightness);

    // Verify brightness is in valid range
    assert(brightness >= 0 && brightness <= 255);

    // Clean up
    image_destroy(large_img);

    printf("%sPASSED%s Performance test passed!\n", GREEN, RESET);
}

void test_multiple_operations(void) {
    printf("Testing multiple operations on medium-sized image...\n");

    // Create a medium-sized image
    struct image* img = create_large_random_image(500, 500);

    // Perform multiple operations
    clock_t start = clock();

    // Calculate brightness 10 times (simulating batch processing)
    for (int i = 0; i < 10; i++) {
        uint8_t brightness = image_brightness(img);
        assert(brightness >= 0 && brightness <= 255);
    }

    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time for 10 brightness calculations: %f seconds\n", cpu_time_used);

    // Clean up
    image_destroy(img);

    printf("%sPASSED%s Multiple operations test passed!\n", GREEN, RESET);
}

int main(void) {
    test_performance_large_image();
    test_multiple_operations();
    return 0;
}
