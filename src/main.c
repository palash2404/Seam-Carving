#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "argparser.h"
#include "energy.h"
#include "image.h"
#include "util.h"

/**
 * Compute the brightness and print the statistics of @p `img`,
 * i.e. width, height & brightness.
 */
void statistics(struct image *img) {
  // TODO implement (assignment 3.1)
  /* implement and use the function:
   * `image_brightness`
   */
  printf("width: %u\n", img->w);
  printf("height: %u\n", img->h);
  printf("brightness: %u\n", image_brightness(img));
}

/**
 * Find & print the minimal path of @p `img`.
 */
void find_print_min_path(struct image *img) {
  // TODO implement (assignment 3.2)
  /* implement and use the functions:
   * - `calculate_energy`
   * - `calculate_min_energy_column`
   * - `calculate_optimal_path`
   * in `energy.c`
   */

  uint32_t *energy =
      malloc(img->w * img->h * sizeof(uint32_t)); // w+y*x ->energy[h*w]
  if (!energy) {
    fprintf(stderr, "Memory allocation failed for energy\n");
    exit(EXIT_FAILURE);
  }
  calculate_energy(energy, img, img->w); // local and total fuck

  int x = calculate_min_energy_column(
      energy, img->w, img->w,
      img->h); // get x i.e. the index of the minimum tile

  uint32_t *seam =
      malloc(img->h * sizeof(uint32_t)); // seam[y]=x seam needs just the height
  if (!seam) {
    fprintf(stderr, "Memory allocation failed for seam\n");
    free(energy);
    exit(EXIT_FAILURE);
  }

  calculate_optimal_path(energy, img->w, img->w, img->h, x, seam);

  for (int i = 0; i < img->h; i++) { // itirate over the height and not width
                                     // as we only need vertical seam;
    printf("%u\n", seam[i]);
  }

  free(energy);
  free(seam);
}

/**
 * Find & carve out @p `n` minimal paths in @p `img`.
 * The image size stays the same, instead for every carved out path there is a
 * column of black pixels appended to the right.
 */
void find_and_carve_path(struct image *const img, int n) {
  // TODO implement (assignment 3.3)
  /* implement and use the functions from assignment 3.2 and:
   * - `carve_path`
   * - `image_write_to_file`
   * in `image.c`.
   */
  if (n >= 0 && n <= img->w) {
    int width = img->w;
    for (int i = 0; i < n; i++) {
      uint32_t *energy = malloc(img->w * img->h * sizeof(uint32_t));
      if (!energy) {
        fprintf(stderr, "Memory allocation failed for energy\n");
        exit(EXIT_FAILURE);
      }

      calculate_energy(energy, img, width);

      int x = calculate_min_energy_column(energy, img->w, width, img->h);

      uint32_t *seam = malloc(img->h * sizeof(uint32_t));
      if (!seam) {
        fprintf(stderr, "Memory allocation failed for seam\n");
        free(energy);
        exit(EXIT_FAILURE);
      }

      calculate_optimal_path(energy, img->w, width, img->h, x, seam);

      carve_path(img, width, seam);

      width--;

      free(energy);
      free(seam);
    }
  }

  image_write_to_file(img, "out.ppm");
}

/**
 * Parse the arguments and call the appropriate functions as specified by the
 * arguments.
 */
int main(int const argc, char **const argv) {
  // DO NOT EDIT
  bool show_min_path = false;
  bool show_statistics = false;
  int n_steps = -1;

  char const *const filename =
      parse_arguments(argc, argv, &show_min_path, &show_statistics, &n_steps);
  if (!filename)
    return EXIT_FAILURE;

  struct image *img = image_read_from_file(filename);

  if (show_statistics) {
    statistics(img);
    image_destroy(img);
    return EXIT_SUCCESS;
  }

  if (show_min_path) {
    find_print_min_path(img);
  } else {
    if (n_steps < 0 || n_steps > img->w)
      n_steps = img->w;

    find_and_carve_path(img, n_steps);
  }

  image_destroy(img);
  return EXIT_SUCCESS;
}
