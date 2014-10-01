/**
 * Implementacion las funciones de imglib
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "imglib.h"

Image *create_image(int type, int w, int h) {

  Image *img = (Image *) malloc(sizeof(Image));

  img->name = (char *) calloc(50, sizeof(char));
  //strcpy(img->name, file);
  img->type = type;
  img->comment = (char *) calloc(255, sizeof(char));
  //  strcpy(img->comment, com);
  img->width = w;
  img->height = h;
  img->maxcolor = type == 3 || type == 2 ? 255 : 1;
  
  img->pixels = (Pixel *) calloc(img->width * img->height, sizeof(Pixel));
  
  return img;
}

/**
 * Loads the image specified in file
 */
Image *read_image(char *file) {

  Image *img = (Image *) malloc(sizeof(Image));

  // Establece el name de la imagen como el name de file
  img->name = (char *) calloc(strlen(file) + 1, sizeof(char));
  strcpy(img->name, file);

  // Abre el file de la imagen
  FILE *fp;
  fp = fopen(img->name, "r");
  
  // Lee el type de imagen
  fscanf(fp, "P%d ", &img->type);
  
  // Lee el comment de la imagen
  char com[255];
  fscanf(fp, "#%[^\n]", com);

  img->comment = (char *) calloc(strlen(com) + 1, sizeof(char));
  strcpy(img->comment, com);

  // Lee las dimensiones de la imagen
  fscanf(fp, "%d %d ", &img->width, &img->height);
  fscanf(fp, "%d ", &img->maxcolor);

  // Lee los pixels
  int pixels = img->width * img->height;
  img->pixels = (Pixel *) calloc(pixels, sizeof(Pixel));
  
  Pixel p;
  int i;
  for(i = 0; i < pixels; i++) {
    if(img->type == 3)
      fscanf(fp, "%d %d %d ", &p.r, &p.g, &p.b);
    else
      fscanf(fp, "%d ", &p.r);

    img->pixels[i] = p;
  }

  fclose(fp);

  return img;
}

/**
 * Saves the image in the given file
 */
void save_image(Image *img, char *file) {

  FILE *fp;
  fp = fopen(file, "w");

  fprintf(fp, "P%d\n", img->type);
  fprintf(fp, "#%s\n", img->comment);
  fprintf(fp, "%d %d\n", img->width, img->height);
  fprintf(fp, "%d\n", img->maxcolor);

  int pixels = img->width * img->height;
  int i;
  for(i = 0; i < pixels; i++) {
    if(img->type == 3)
      fprintf(fp, "%d %d %d",
	    img->pixels[i].r,
	    img->pixels[i].g,
	    img->pixels[i].b);
    else if(img->type == 2 || img->type == 1)
      fprintf(fp, "%d", img->pixels[i].r);

    if((i + 1) % img->width == 0)
      fprintf(fp, "\n");
    else
      fprintf(fp, " ");
  }

  fclose(fp);
}

/**
 * Frees the image memory
 */
void free_image(Image *imagen) {

  free(imagen->name);
  free(imagen->comment);
  free(imagen->pixels);
  free(imagen);

}

int pixel_promedio(Pixel *pixel) {
  return (pixel->r + pixel->g + pixel->b) / 3;
}

/**
 * Converts a P3 image to a P2 image
 */
Image *p3_to_p2(Image *imgsrc) {
  
  Image *img = create_image(2, imgsrc->width, imgsrc->height);

  strcpy(img->name, imgsrc->name);
  strcpy(img->comment, imgsrc->comment);

  int pixels = img->width * img->height;
  
  int i;
  for(i = 0; i < pixels; i++)
    img->pixels[i].r = pixel_promedio(&imgsrc->pixels[i]);

  return img;
}

/**
 * Converts a P3 image to a P1 image
 */
Image *p3_to_p1(Image *imgsrc, int umbral) {

  Image *img = create_image(1, imgsrc->width, imgsrc->height);

  strcpy(img->name, imgsrc->name);
  strcpy(img->comment, imgsrc->comment);

  int pixels = img->width * img->height;
  
  int i;
  for(i = 0; i < pixels; i++)
    img->pixels[i].r = pixel_promedio(&imgsrc->pixels[i]) < umbral ? 1 : 0;

  return img;
}

/**
 * Gets an array with the black pixels of the image
 */
int array_black_points(Image *img, Point points[]) {

  int pixels = img->width * img->height;
  int i, n = 0;
  for(i = 0; i < pixels; i++) {
  
    if(img->pixels[i].r == 1) {
      points[n].x = i % img->width;
      points[n].y = i / img->width;
      
      n++;
    }

  }

  return n;
}

void print_array_points(Point points[], int n) {
  int i;
  for(i = 0; i < n; i++) {
    printf("(%d, %d) ", points[i].x, points[i].y);
  }
  printf("\n");
}
