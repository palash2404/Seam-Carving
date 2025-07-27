#include "energy.h"

#include <stdlib.h>
#include <string.h>

#include "indexing.h"
#include "util.h"

uint32_t max(uint32_t a, uint32_t b) { return a > b ? a : b; }

uint32_t min(uint32_t a, uint32_t b) { return a < b ? a : b; }

/**
 * Calculate the difference of two color values @p a and @p b.
 * The result is the sum of the squares of the differences of the three (red,
 * green and blue) color components.
 * */
inline uint32_t diff_color(struct pixel const a, struct pixel const b) {
  // TODO implement (assignment 3.2)

  int diff_a = a.r - b.r;
  int diff_b = a.b - b.b;
  int diff_g = a.g - b.g;

  return diff_a * diff_a + diff_b * diff_b + diff_g * diff_g;
}

/**
 * Calculate the total energy at every pixel of the image @p `img`,
 * but only considering columns with index less than @p `w`.
 * To this end, first calculate the local energy and use it to calculate the
 * total energy.
 * @p `energy` is expected to have allocated enough space
 * to represent the energy for every pixel of the whole image @p `img.
 * @p `w` is the width up to (excluding) which column in the image the energy
 * should be calculated. The energy is expected to be stored exactly analogous
 * to the image, i.e. you should be able to access the energy of a pixel with
 * the same array index.
 */
void calculate_energy(uint32_t *const energy, struct image *const img,
                      int const w) {
  // TODO implement (assignment 3.2)
  for (int y = 0; y < img->h; y++) {      // height top to down
    for (int x = 0; x < w; x++) {         // column left to right
      int index = yx_index(y, x, img->w); // (y*w0+x)

      uint32_t local_energy = 0;

      struct pixel present = img->pixels[index];

      if (x == 0 && y == 0) {
        energy[index] = 0;
        continue;
      }

      if (y > 0) {
        struct pixel above =
            img->pixels[yx_index(y - 1, x, img->w)]; // getting top value
        local_energy +=
            diff_color(present, above); // adding square of difference of main
                                        // and the top tile to local.
      }
      if (x > 0) {
        struct pixel left = img->pixels[yx_index(y, x - 1, img->w)];
        local_energy += diff_color(present, left);
      }

      energy[index] = local_energy;
    }
  }

  for (int y = 1; y < img->h; y++) {
    for (int x = 0; x < w; x++) {
      int index = yx_index(y, x, img->w);
      uint32_t local_energy = energy[index];

      uint32_t top = energy[yx_index(y - 1, x, img->w)];

      if (x > 0) {
        uint32_t top_left = energy[yx_index(y - 1, x - 1, img->w)];
        if (top_left < top) {
          top = top_left;
        }
      }
      if (x < w - 1) {
        uint32_t top_right = energy[yx_index(y - 1, x + 1, img->w)];
        if (top_right < top) {
          top = top_right;
        }
      }

      energy[index] = local_energy + top;
    }
  }
}

/**
 * Calculate the index of the column with the least energy in bottom row.
 * Expects that @p `energy` holds the energy of every pixel of @p `img` up to
 * column (excluding) @p `w`. Columns with index `>= w` are not considered as
 * part of the image.
 * @p `w0` states the width of the energy matrix @p `energy`
 * @p `h` states the height of the energy matrix @p `energy`
 */
int calculate_min_energy_column(uint32_t const *const energy, int const w0,
                                int const w, int const h) {
  // TODO implement (assignment 3.2)

  int y = h - 1; // last row
  int index = 0; // first column or x=0, will also return the minimum total
                 // energy column.
  uint32_t min_energy =
      energy[yx_index(y, 0, w0)]; // first tile of the last row (x,h-1)
                                  // compare this to the next elemnts

  for (int i = 1; i < w; i++) {
    uint32_t max_energy = energy[yx_index(y, i, w0)];
    if (max_energy < min_energy) { // compare first to the next and update
                                   // the first if the second is smaller
                                   // than first else keep first untouched.
      min_energy = max_energy;
      index = i;
    }
  }
  return index; // return address or say the column number where there is the
                // minimum total enery
}

/**
 * Calculate the optimal path (i.e. least energy), according to the energy
 * entries in @p `energy` up to (excluding) column @p `w`. The path is stored in
 * @p `seam`. Columns with index `>= w` are not considered as part of the image.
 * @p `x` is the index in the bottom row where the seam starts.
 * @p `w0` states the width of the energy matrix @p `energy`
 * @p `h` states the height of the energy matrix @p `energy`
 */
void calculate_optimal_path(uint32_t const *const energy, int const w0,
                            int const w, int const h, int x,
                            uint32_t *const seam) {
  // TODO implement (assignment 3.2)
  seam[h - 1] = x;
  for (int z = h - 2; z >= 0; z--) {
    uint32_t upper = energy[yx_index(z, x, w0)];
    int next = x;

    // uint32_t upper_left = UINT32_MAX;
    // uint32_t upper_right = UINT32_MAX;

    if (x > 0) {
      uint32_t upper_left = energy[yx_index(z, x - 1, w0)];
      if (upper_left < upper) {
        upper = upper_left;
        next = x - 1; // left shift for next step
      }
    }
    if (x < w - 1) {
      uint32_t upper_right = energy[yx_index(z, x + 1, w0)];
      if (upper_right < upper) {
        upper = upper_right;
        next = x + 1; // right make
      }
    }
    // printf(
    //   "Row: %d, Current x: %d, Chosen new_x: %d | Energies - Left: %u, "
    //  "Up: %u, Right: %u\n",
    // z, x, next, upper_left, energy[yx_index(z, x, w0)], upper_right);

    seam[z] = next;
    x = next;
  }
}
