#include <stdio.h>
#include "imglib.h"

main() {
  
  Image *img = create_image(1, 10, 10, "Imagen");
  int i;
  for(i = 0; i < 100; i++) {
    img->pixels[i].r = 1;
  }

  Point points[img->width * img->height];

  int n = array_black_points(img, points);
  print_array_points(points, n);

  save_image(img, "img.ppm");

  free_image(img);
  
}
