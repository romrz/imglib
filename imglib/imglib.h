/**
 * Imglib definitions and prototypes
 */

typedef struct {
  int r;
  int g;
  int b;
} Pixel;

typedef struct {
  int type;
  char *comment;
  int width;
  int height;
  int maxcolor;
  Pixel *pixels;
} Image;

typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  Point p1;
  Point p2;
} Line;

Image *create_image(int type, int w, int h, char *comment);
Image *read_image(char *file);
void save_image(Image *image, char *file);
void save_image_svg(Image *image, char *file);
void free_image(Image * image);
Image *p3_to_p2(Image *imgsrc);
Image *p3_to_p1(Image *imgsrc, int umbral);
int average_pixel(Pixel *pixel);

int get_black_points(Image *image, Point points[]);
void print_points(Point points[], int n);
Line calculate_line(Point points[], int n);
