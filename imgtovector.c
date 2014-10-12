/**
 * This program converts an P3 image to binary and from binary to svg.
 * 
 * Arguments
 * 1. Source image
 * 2. Threshold to convert to binary
 * 3. Threshold to convert to svg
 */

#include <stdio.h>
#include <stdlib.h>
#include "imglib/imglib.h"

int main(int nargs, char *args[]) {

  Image *imgsrc = read_image(args[1]);
  Image *img = p3_to_p1(imgsrc, atoi(args[2]));

  // Threshold
  double t = atof(args[3]);
  
  save_image(img, "img_bin.ppm");
  save_image_svg(img, "img_svg.svg", t);

  free_image(img);
  free_image(imgsrc);

  return 0;
}
