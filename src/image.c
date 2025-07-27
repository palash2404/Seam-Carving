#include "image.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "energy.h"
#include "indexing.h"
#include "util.h"

/**
 * Initialize the image @p `img` with width @p `w` and height @p `h`.
 */
struct image *image_init(int const w, int const h) {
  // DO NOT EDIT
  struct image *img = malloc(sizeof(struct image));
  img->w = w;
  img->h = h;
  img->pixels = malloc(w * h * sizeof(*img->pixels));
  memset(img->pixels, 0, w * h * sizeof(*img->pixels));
  return img;
}

/**
 * Destroy the image @p `img` by freeing its pixels field and by freeing @p
 * `img` itself. Don't use @p img afterwards.
 */
void image_destroy(struct image *img) {
  free(img->pixels);
  free(img);
}

/**
 * Read an image from the file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 * file format.
 * @returns the image that was read.
 */
struct image *image_read_from_file(const char *filename) {
  // DO NOT EDIT
  FILE *f = fopen(filename, "r");
  if (f == NULL)
    exit(EXIT_FAILURE);

  if (fscanf(f, "P3") == EOF) {
    fclose(f);
    exit(EXIT_FAILURE);
  }

  int w, h;
  if (fscanf(f, "%d %d 255 ", &w, &h) == EOF) {
    fclose(f);
    exit(EXIT_FAILURE);
  }
  if (w <= 0 || h <= 0) {
    fclose(f);
    exit(EXIT_FAILURE);
  }

  struct image *img = image_init(w, h);
  struct pixel *pixels = img->pixels;

  for (int y = 0; y < img->h; ++y) {
    unsigned int r, g, b;
    for (int x = 0; x < img->w; ++x, ++pixels) {
      if (fscanf(f, "%u %u %u ", &r, &g, &b) == EOF) {
        image_destroy(img);
        fclose(f);
        exit(EXIT_FAILURE);
      }
      pixels->r = r;
      pixels->g = g;
      pixels->b = b;
    }
  }

  if (fgetc(f) != EOF) {
    image_destroy(img);
    fclose(f);
    exit(EXIT_FAILURE);
  }

  fclose(f);
  return img;
}

/**
 * Write the image @p `img` to file at @p `filename` in the portable pixmap (P3)
 * format. See http://en.wikipedia.org/wiki/Netpbm_format for details on the
 *
 * file format.
 */
void image_write_to_file(struct image *img, const char *filename) {
  // TODO implement (assignment 3.3)
  FILE *f = fopen(filename, "w");

  if (f == NULL)
    exit(EXIT_FAILURE);

  fprintf(f, "P3 \n");
  fprintf(f, "%d %d \n", img->w, img->h);
  fprintf(f, "255\n");

  for (int y = 0; y < img->h; y++) {   // height
    for (int x = 0; x < img->w; x++) { // weight
      struct pixel pix =
          img->pixels[yx_index(y, x, img->w)]; // getting position of every tile
      fprintf(f, "%u %u %u \n", pix.r, pix.g,
              pix.b); // writing red,blue,green in every tile
    }
  }

  fclose(f);
}

/**
 * Compute the brightness of the image @p `img`.
 */
uint8_t image_brightness(struct image *img) {
  // TODO implement (assignment 3.1)
  uint64_t total = 0;
  int size = img->w * img->h;

  for (int i = 0; i < size; i++) {
    struct pixel p = img->pixels[i];

    uint8_t brightness = (p.r + p.g + p.b) / 3;

    total += brightness;
  }
  if (size == 0) {
    return total;
  }
  return total / size;
}

/**
 * Carve out the path @p `seam` from the image @p `img`,
 * where only the @p `w` left columns are considered.
 * Move all pixels right of it one to the left and fill the rightmost row with
 * black (0,0,0). Columns with index >= `w` are not considered as part of the
 * image.
 */
void carve_path(struct image *const img, int const w,
                uint32_t const *const seam) {
  // TODO implement (assignment 3.3)
  for (int y = 0; y < img->h; y++) { // carving we go down to top so img->h
    int x = seam[y];

    for (int i = x; i < w - 1; i++) {
      img->pixels[yx_index(y, i, img->w)] = img->pixels[yx_index(
          y, i + 1,
          img->w)]; // shift to left i.e. img->w, till second last w-1
    }
    struct pixel *black =
        &img->pixels[yx_index(y, w - 1, img->w)]; // filling her up, so we use &
    black->r = 0;
    black->b = 0;
    black->g = 0;
  }
}
