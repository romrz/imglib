/**
 * Imglib implementations
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "imglib.h"

/**
 * Creates a new image.
 */
Image *create_image(int type, int w, int h, char *comment) {

  Image *img = (Image *) malloc(sizeof(Image));

  img->type = type;
  img->comment = (char *) calloc(strlen(comment) + 1, sizeof(char));
  strcpy(img->comment, comment);
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
  // Image data
  Image *img = NULL;
  int type, width, height, maxcolor;
  char comment[255];

  // Open the image file
  FILE *fp;
  fp = fopen(file, "r");

  // Reads the image data
  fscanf(fp, "P%d ", &type);
  fscanf(fp, "#%[^\n]", comment);
  fscanf(fp, "%d %d ", &width, &height);
  fscanf(fp, "%d ", &maxcolor);

  // Creates the image
  img = create_image(type, width, height, comment);

  // Reads the pixels
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
 * Saves the image in the given file name
 */
void save_image(Image *img, char *file) {

  FILE *fp;
  fp = fopen(file, "w");

  fprintf(fp, "P%d\n", img->type);
  fprintf(fp, "#%s\n", img->comment);
  fprintf(fp, "%d %d\n", img->width, img->height);
  //fprintf(fp, "%d\n", img->maxcolor);

  int i, pixels = img->width * img->height;
  for(i = 0; i < pixels; i++) {

    /* The pixel data of images P2 and P1 is stored
       only in pixel.r */
    if(img->type == 3)
      fprintf(fp, "%d %d %d",
	    img->pixels[i].r, img->pixels[i].g, img->pixels[i].b);
    else if(img->type == 2 || img->type == 1)
      fprintf(fp, "%d", img->pixels[i].r);

    // Prints a new line character when it's end of line or a space if not
    if((i + 1) % img->width == 0)
      fprintf(fp, "\n");
    else
      fprintf(fp, " ");
  }

  fclose(fp);
}


/**
 * Converts the image to an svg image using lines
 */
void save_image_svg(Image *img, char *file) {

  FILE *fp;
  fp = fopen(file, "w");

  // Gets the points of the black pixels
  Point points[img->width * img->height];
  int n = get_black_points(img, points);

  // Gets the lines representing the image
  Line line = calculate_line(points, n);

  fprintf(fp,"<svg width=\"%d\" height=\"%d\">\n", img->width, img->height);
  
  fprintf(fp, "\t<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n", line.p1.x, line.p1.y, line.p2.x, line.p2.y);
  
  fprintf(fp, "</svg>");

  fclose(fp);
}


/**
 * Frees the image memory
 */
void free_image(Image *imagen) {

  free(imagen->comment);
  free(imagen->pixels);
  free(imagen);

}

/**
 * Obtains the average value of the RGB pixel values
 */
int average_pixel(Pixel *pixel) {
  return (pixel->r + pixel->g + pixel->b) / 3;
}

/**
 * Converts a P3 image to a P2 image
 */
Image *p3_to_p2(Image *imgsrc) {
  
  Image *img = create_image(2, imgsrc->width, imgsrc->height, imgsrc->comment);

  int i, pixels = img->width * img->height;
  for(i = 0; i < pixels; i++)
    img->pixels[i].r = average_pixel(&imgsrc->pixels[i]);

  return img;
}

/**
 * Converts a P3 image to a P1 image
 */
Image *p3_to_p1(Image *imgsrc, int umbral) {

  Image *img = create_image(1, imgsrc->width, imgsrc->height, imgsrc->comment);

  int i, pixels = img->width * img->height;
  for(i = 0; i < pixels; i++)
    img->pixels[i].r = average_pixel(&imgsrc->pixels[i]) < umbral ? 1 : 0;

  return img;
}

/**
 * Returns an array with the black pixels of the image
 */
int get_black_points(Image *img, Point points[]) {
  if(img->type != 1) return 0;

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

void print_points(Point points[], int n) {
  int i;
  for(i = 0; i < n; i++)
    printf("(%d, %d) ", points[i].x, points[i].y);
  printf("\n");
}

Line calculate_line(Point points[], int n) {

  int Sx = 0;
  int Sy = 0;
  int Sxy = 0;
  int Sxx = 0;
  int Syy = 0;
  
  double m;
  double b;

  Line line;

  int i;
  for(i = 0; i < n; i++) {
    Sx += points[i].x;
    Sy += points[i].y;
    Sxy += points[i].x * points[i].y;
    Sxx += points[i].x * points[i].x;
    Syy += points[i].y * points[i].y;
  }

  m = (double) (n * Sxy - Sx * Sy) / (n * Sxx - Sx * Sx);
  b = (double) (Sxx * Sy - Sx * Sxy) / (n * Sxx - Sx * Sx);

  Point p1 = {0, b};
  Point p2 = {100, m * 100 + b};
  line.p1 = p1;
  line.p2 = p2;

  return line;
}
