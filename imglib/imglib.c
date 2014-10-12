/**
 * Imglib implementations
 */

#include <stdio.h>
#include <math.h>
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
  if(type != 1)
    fscanf(fp, "%d ", &maxcolor);

  // Creates the image
  img = create_image(type, width, height, comment);

  if(img != NULL) {
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
  }

  return img;
}

/**
 * Copies an image
 */
Image *copy_image(Image *imgsrc) {
  
  Image *img = create_image(imgsrc->type, imgsrc->width, imgsrc->height, imgsrc->comment);
  
  if(img != NULL) {
    int pixels = imgsrc->width * imgsrc->height;
    int i;
    for(i = 0; i < pixels; i++)
      img->pixels[i] = imgsrc->pixels[i];
  }

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
  if(img->type != 1)
    fprintf(fp, "%d\n", img->maxcolor);

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
void save_image_svg(Image *img, char *file, double t) {

  FILE *fp;
  fp = fopen(file, "w");

  int max_lines = 1000;
  Line lines[max_lines];
  int n = get_lines(img, lines, max_lines, t);

  // Prints the image
  fprintf(fp,"<svg width=\"%d\" height=\"%d\">\n",img->width,img->height);
  
  int i;
  for(i = 0; i < n; i++)
    fprintf(
        fp,
	"\t<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke=\"black\" />\n",
	lines[i].p1.x, lines[i].p1.y, lines[i].p2.x, lines[i].p2.y);
  
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
  if(imgsrc->type != 3) return NULL;

  Image *img = create_image(2, imgsrc->width, imgsrc->height, imgsrc->comment);

  if(img != NULL) {
    int i, pixels = img->width * img->height;
    for(i = 0; i < pixels; i++)
      img->pixels[i].r = average_pixel(&imgsrc->pixels[i]);
  }

  return img;
}

/**
 * Converts a P3 image to a P1 image
 */
Image *p3_to_p1(Image *imgsrc, int umbral) {
  if(imgsrc->type != 3) return NULL;

  Image *img = create_image(1, imgsrc->width, imgsrc->height, imgsrc->comment);

  if(img != NULL) {
    int i, pixels = img->width * img->height;
    for(i = 0; i < pixels; i++)
      img->pixels[i].r = average_pixel(&imgsrc->pixels[i])<umbral ? 1 : 0;
  }

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

/**
 * Prints the points
 */
void print_points(Point points[], int n) {
  int i;
  for(i = 0; i < n; i++)
    printf("(%d, %d) ", points[i].x, points[i].y);
  printf("\n");
}

/**
 * Gets the approximate lines of the image, using the given threshold
 * Returns the number of lines
 */
int get_lines(Image *img, Line lines[], int max_lines, double t) {  
  // Gets the points of the black pixels
  Point points[img->width * img->height];
  int num_points = get_black_points(img, points);

  // Error
  double em = 0.0;
  double eb = 0.0;

  // Aux line
  Line line;
  
  // Initial and final point
  int p1 = 0;
  int p2 = 3;

  int i;
  for(i = 0; i < max_lines && p2 < num_points; i++) {
    do {
      line = calculate_line(points, p1, p2, &em, &eb);
      
      // If the error of the next line is greater than the threshold
      // breaks the loop
      if(p2 + 1 < num_points) {
	double aux_em;
	calculate_line(points, p1, p2+1, &aux_em, &eb);
	if(fabs(aux_em) > t)
	  break;
      }
    } while(fabs(em) <= t && p2++ < num_points);

    lines[i] = line;
    
    p1 = p2 - 1;
    p2 = p1 + 3;
  }

  return i;
}

/**
 * Gets the absolute value of the number
 */
double fabs(double n) {
  return n > 0 ? n : -n;
}

/**
 * Gets the m value of the line
 */
double get_m(Line line) {
  return (double) (line.p2.y - line.p1.y) / (double) (line.p2.x - line.p1.x);
}

/**
 * Gets the b value of the line
 */
double get_b(Line line) {
  return (double) line.p1.y;
}

/**
 * Aproximates a line between the initial and final point,
 * storing the error in the passed parameters
 */
Line calculate_line(Point points[], int ip, int fp, double *me, double *be) {

  int Sx = 0;
  int Sy = 0;
  int Sxy = 0;
  int Sxx = 0;
  int Syy = 0;

  double Serror = 0.0;
  double m_error = 0.0;
  double b_error = 0.0;

  double m = 0.0;
  double b = 0.0;

  Line line;

  int n = fp - ip;
  int i;
  for(i = ip; i < fp; i++) {
    Sx += points[i].x;
    Sy += points[i].y;
    Sxy += points[i].x * points[i].y;
    Sxx += points[i].x * points[i].x;
    Syy += points[i].y * points[i].y;
  }

  m = (double) (n * Sxy - Sx * Sy) / (n * Sxx - Sx * Sx);
  b = (double) (Sxx * Sy - Sx * Sxy) / (n * Sxx - Sx * Sx);

  // Calculates the error (b + m*xi - yi)2
  for(i = ip; i < fp; i++)
    Serror += pow((double) (b + m * points[i].x - points[i].y), 2);

  m_error = sqrt(((double) n / (n * Sxx - Sx * Sx))*(Serror) / (n - 2) );
  b_error = sqrt(((double) Sxx / (n * Sxx - Sx * Sx))*(Serror) / (n - 2));

  // Stores the error
  *me = m_error;
  *be = b_error;

  // Gets the left and right most position point
  // between the initial and final point
  int lp = find_leftmost_point(points, ip, fp);
  int rp = find_rightmost_point(points, ip, fp);

  // Creates a line between the left and right points
  Point p1 = {points[lp].x, m * points[lp].x + b};
  Point p2 = {points[rp].x, m * points[rp].x + b};
  line.p1 = p1;
  line.p2 = p2;

  return line;
}

/**
 * Finds the position of the point in left most position 
 */
int find_leftmost_point(Point points[], int p1, int p2) {
  int i, p = p1;
  for(i = p1 + 1; i < p2; i++)
    if(points[i].x < points[p].x)
      p = i;
  
  return p;
}

/**
 * Finds the position of the point in right most position 
 */
int find_rightmost_point(Point points[], int p1, int p2) {
  int i, p = p1;
  for(i = p1 + 1; i < p2; i++)
    if(points[i].x > points[p].x)
      p = i;

  return p;
}
