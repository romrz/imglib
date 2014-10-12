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
#include <string.h>
#include "imglib/imglib.h"

int main(int nargs, char *args[]) {

  /* Original image name */
  char *img_name = calloc(strlen(args[1]) + 1, sizeof(char));
  strcpy(img_name, args[1]);

  /* Binary image name of the for [image_name]_bin.ppm */
  char *img_name_bin = calloc(strlen(img_name) + 5, sizeof(char));
  strncpy(img_name_bin, img_name, strlen(img_name) - 4);
  strcat(img_name_bin, "_bin.ppm");

  /* Binary image name of the for [image_name]_svg.svg */
  char *img_name_svg = calloc(strlen(img_name) + 5, sizeof(char));
  strncpy(img_name_svg, img_name, strlen(img_name) - 4);
  strcat(img_name_svg, "_svg.svg");
  

  Image *imgsrc = read_image(img_name);
  if(imgsrc == NULL) {
    printf("Unable to read the image: %s\n", img_name);
    return 0;
  }

  Image *img = p3_to_p1(imgsrc, atoi(args[2]));
  if(img == NULL) {
    printf("Unable to convert image P3 to P1\n");
    return 0;
  }

  // Threshold to calculate the image lines
  double t = atof(args[3]);
  
  /* Saves the images */
  save_image(img, img_name_bin);
  save_image_svg(img, img_name_svg, t);

  /* Frees memory */
  free(img_name);
  free(img_name_bin);
  free(img_name_svg);

  free_image(img);
  free_image(imgsrc);

  return 0;
}
