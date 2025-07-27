#include <stdlib.h>

#include "../../src/image.h"

// Forward declarations for your existing functions
void calculate_energy(uint32_t* energy, struct image* img, int width);
int calculate_min_energy_column(uint32_t* energy, int img_width, int width,
                                int height);
void calculate_optimal_path(uint32_t* energy, int img_width, int width,
                            int height, int min_col, uint32_t* path);
void carve_path(struct image* image, int w, uint32_t const* seam);

// Adapter function that mimics the interface expected by the tests
struct image* image_carve_seam(struct image* img) {
    if (img == NULL || img->w <= 1 || img->h <= 0) {
        return NULL;
    }

    // Create a copy of the original image
    struct image* result = image_init(img->w, img->h);
    for (int i = 0; i < img->w * img->h; i++) {
        result->pixels[i] = img->pixels[i];
    }

    // Allocate memory for energy and seam arrays
    uint32_t* energy = malloc(img->w * img->h * sizeof(uint32_t));
    uint32_t* seam = malloc(img->h * sizeof(uint32_t));

    if (!energy || !seam) {
        free(energy);
        free(seam);
        image_destroy(result);
        return NULL;
    }

    // Calculate energy for the current width
    calculate_energy(energy, result, result->w);

    // Find the column with minimum energy
    int min_col =
        calculate_min_energy_column(energy, result->w, result->w, result->h);

    // Calculate the optimal path
    calculate_optimal_path(energy, result->w, result->w, result->h, min_col,
                           seam);

    // Carve out the seam
    carve_path(result, result->w, seam);

    // IMPORTANT: Update the width of the image after carving
    result->w = result->w - 1;

    // Clean up
    free(energy);
    free(seam);

    return result;
}
