#include <stdio.h>
#include "imglib/imglib.h"

void change_pixel(Image *img, int x, int y) {
  img->pixels[img->width * y + x].r = 1;
}

main() {

  Image *img = create_image(1, 100, 100, "imagen");

  change_pixel(img, 10, 10);
  change_pixel(img, 20, 80);
  
  save_image(img, "imgtest.ppm");
  save_image_svg(img, "imgtestSVG.svg");

  free_image(img);
}
