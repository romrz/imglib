/**
 * Este programa convierte una imagen ppm a una imagen vectorial
 * representada por una linea resultante de toda la imagen.
 * 
 * Recibe el nombre de imagen original,
 * el nombre de destino
 * y el umbral para convertir a binario
 */
#include <stdio.h>
#include "imglib/imglib.h"

main(int nargs, char *args[]) {

  //printf("%d\n", atoi(args[3]) + 1);

  Image *img = read_image(args[1]);
  Image *imgbin = p3_to_p1(img, atoi(args[3]));

  save_image(imgbin, "img_bin.ppm");
  save_image_svg(imgbin, args[2]);

  free_image(img);
  free_image(imgbin);
}
