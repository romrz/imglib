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
Image *copy_image(Image *imgsrc);
void save_image(Image *image, char *file);
void save_image_svg(Image *image, char *file, double t);
void free_image(Image * image);
Image *p3_to_p2(Image *imgsrc);
Image *p3_to_p1(Image *imgsrc, int umbral);
int average_pixel(Pixel *pixel);

double fabs(double n);

int get_black_points(Image *image, Point points[]);
void print_points(Point points[], int n);
Line calculate_line(Point points[], int ip, int fp, double *me, double *be);
double get_m(Line line);
double get_b(Line line);
int get_lines(Image *img, Line lines[], int max_lines, double t);
int find_leftmost_point(Point points[], int p1, int p2);
int find_rightmost_point(Point points[], int p1, int p2);
