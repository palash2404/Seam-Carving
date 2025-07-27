#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../src/image.h"

// Add color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

// Helper function to create a gradient image for testing
struct image* create_gradient_image(int w, int h) {
    struct image* img = image_init(w, h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int idx = y * w + x;
            img->pixels[idx].r = (uint8_t)((x * 255) / w);
            img->pixels[idx].g = (uint8_t)((y * 255) / h);
            img->pixels[idx].b = 128;
        }
    }
    return img;
}

// Helper function to cut the right side of an image
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

void test_image_cutting(void) {
    // Create a test image with a gradient pattern
    struct image* original = create_gradient_image(200, 100);

    // Test cutting 50 pixels from the right
    struct image* cut_image = cut_image_right(original, 50);

    assert(cut_image->w == 150);
    assert(cut_image->h == 100);

    // Check that pixel data was properly preserved
    for (int y = 0; y < cut_image->h; y++) {
        for (int x = 0; x < cut_image->w; x++) {
            int orig_idx = y * original->w + x;
            int cut_idx = y * cut_image->w + x;

            assert(original->pixels[orig_idx].r ==
                   cut_image->pixels[cut_idx].r);
            assert(original->pixels[orig_idx].g ==
                   cut_image->pixels[cut_idx].g);
            assert(original->pixels[orig_idx].b ==
                   cut_image->pixels[cut_idx].b);
        }
    }

    image_destroy(original);
    image_destroy(cut_image);

        printf("%sPASSED%s Image cutting test passed!\n", GREEN, RESET);
}

int main(void) {
    test_image_cutting();
    return 0;
}
