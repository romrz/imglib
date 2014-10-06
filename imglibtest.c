#include <stdio.h>
#include "imglib.h"

main() {
  
  Image *img = create_image(1, 10, 10, "Imagen");
  int i;
  for(i = 0; i < 100; i++) {
    img->pixels[i].r = 0;
  }

  int x1 = 0, x2 = 5, x3 = 8;
  int y1 = 5, y2 = 5, y3 = 5; 
  img->pixels[y1*img->width + x1].r = 1;
  img->pixels[y2*img->width + x2].r = 1;
  img->pixels[y3*img->width + x3].r = 1;

  Point points[img->width * img->height];

  int n = get_black_points(img, points);
  //print_points(points, n);
  
  //calculate_line(points, n);



  Image *img2 = read_image("img_bin.ppm");

  save_image_svg(img2, "img_svg.svg");

  save_image(img, "img.ppm");
  free_image(img);
  
}
