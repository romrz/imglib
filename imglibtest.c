#include <stdio.h>
#include "imglib.h"

main() {
  
  //Image *imagen = read_image("escudoP3.ppm");
  //Image *grises = p3_to_p2(imagen);
  //Image *binaria = p3_to_p1(imagen, 1);

  //save_image(binaria, "img_bin.ppm");
  //save_image(grises, "img_grises.ppm");
  //save_image(imagen, "img.ppm");

  //Image *igris = read_image("img_bin.ppm");
  
  //save_image(igris, "img_bin_out.ppm");

  Image *img = create_image(1, 10, 10);
  int i;
  for(i = 0; i < 100; i++) {
    img->pixels[i].r = 1;
  }

  Point points[img->width * img->height];

  int n = array_black_points(img, points);

  print_array_points(points, n);

  save_image(img, "img.ppm");

  free_image(img);
  //free_image(igris);
  //free_image(binaria);
  //free_image(grises);
  //free_image(imagen);

}
