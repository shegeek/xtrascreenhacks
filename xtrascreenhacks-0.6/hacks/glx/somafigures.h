#ifndef SOMAFIGURES_H
#define SOMAFIGURES_H
typedef struct {
  GLfloat refcubex[8];
  GLfloat refcubey[8];
  GLfloat refcubez[8];
  int     orientation[8];
} figure;

figure somafigures[] = {
{{0.0, 1.000000, 0.000000, 0.000000, -1.000000, -1.000000, 1.000000, 1.000000}, {0.0, 2.000000, 2.000000, 0.000000, 1.000000, 2.000000, 1.000000, 0.000000}, {0.0, 0.000000, -1.000000, -1.000000, 0.000000, 1.000000, -1.000000, 1.000000}, {-1, 15, 21, 7, 14, 20, 5, 1}},
{{0.0, 1.000000, 0.000000, 1.000000, -1.000000, 1.000000, 0.000000, -1.000000}, {0.0, 1.000000, 2.000000, 2.000000, 1.000000, 0.000000, 2.000000, 0.000000}, {0.0, 1.000000, -1.000000, 0.000000, -1.000000, -1.000000, 0.000000, 1.000000}, {-1, 2, 12, 14, 23, 6, 15, 0}},
{{0.0, 0.000000, 1.000000, 0.000000, -1.000000, 0.000000, 1.000000, -1.000000}, {0.0, 2.000000, 2.000000, 0.000000, 2.000000, 1.000000, 0.000000, 0.000000}, {0.0, 1.000000, 1.000000, 1.000000, 0.000000, -1.000000, 0.000000, -1.000000}, {-1, 11, 14, 0, 14, 0, 11, 6}},
{{0.0, -1.000000, 1.000000, 1.000000, 0.000000, 0.000000, 0.000000, -1.000000}, {0.0, 2.000000, 0.000000, 2.000000, 2.000000, 1.000000, 0.000000, 0.000000}, {0.0, 0.000000, -1.000000, 0.000000, 1.000000, -1.000000, 1.000000, -1.000000}, {-1, 9, 6, 14, 2, 6, 11, 6}},
{{0.0, 0.000000, 1.000000, -1.000000, 0.000000, -1.000000, -1.000000, 1.000000}, {0.0, 2.000000, 2.000000, 1.000000, 0.000000, 1.000000, 2.000000, 1.000000}, {0.0, -1.000000, 1.000000, -1.000000, -1.000000, 1.000000, 1.000000, 0.000000}, {-1, 7, 20, 3, 7, 20, 9, 2}},
{{0.0, -1.000000, -1.000000, -1.000000, 0.000000, 0.000000, 1.000000, 1.000000}, {0.0, 2.000000, 0.000000, 1.000000, 1.000000, 1.000000, 2.000000, 2.000000}, {0.0, 0.000000, 0.000000, 1.000000, -1.000000, 1.000000, -1.000000, 1.000000}, {-1, 9, 9, 19, 2, 20, 12, 2}},
{{0.0, -3.000000, 3.000000, 0.000000, -2.000000, -3.000000, -3.000000, -3.000000}, {0.0, 0.500000, 0.500000, 1.500000, 1.500000, 1.500000, 1.500000, 0.500000}, {0.0, -1.000000, 0.000000, 0.000000, 0.000000, -2.000000, 2.000000, 1.000000}, {-1, 0, 5, 2, 2, 20, 23, 0}},
{{0.0, -1.500000, -0.500000, 1.500000, 0.500000, 2.500000, -1.500000, -1.500000}, {0.0, 1.500000, 0.500000, -0.500000, -0.500000, -0.500000, 0.500000, -0.500000}, {0.0, 0.000000, 0.000000, 1.000000, -1.000000, 0.000000, 1.000000, -1.000000}, {-1, 1, 10, 9, 7, 19, 20, 5}},
{{0.0, -1.000000, 0.000000, 1.000000, -1.000000, 1.000000, -1.000000, 0.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, -1.500000, -1.500000, 0.500000, 1.500000, 3.500000, 3.500000, -3.500000}, {-1, 13, 7, 21, 18, 9, 11, 5}},
{{0.0, -1.000000, 1.000000, -1.000000, -1.000000, -1.000000, 1.000000, -1.000000}, {0.0, 0.000000, -1.000000, -1.000000, 0.000000, 1.000000, 1.000000, 2.000000}, {0.0, 1.000000, 1.000000, 0.000000, -1.000000, 0.000000, -1.000000, -1.000000}, {-1, 9, 22, 17, 10, 7, 12, 6}},
{{0.0, 1.000000, 2.000000, 0.000000, -1.000000, 2.000000, -2.000000, -2.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, 2.000000, 0.000000, -2.000000, 0.000000, -2.000000, -2.000000, 2.000000}, {-1, 11, 13, 7, 13, 6, 6, 0}},
{{0.0, 0.000000, 0.000000, -2.000000, 2.000000, -2.000000, -1.000000, 2.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, -1.000000, 0.000000, 0.000000, 1.000000, -2.000000, 2.000000, -2.000000}, {-1, 9, 17, 17, 22, 0, 11, 5}},
{{0.0, 3.000000, 1.000000, 0.000000, -1.000000, -3.000000, 3.000000, -3.000000}, {0.0, 1.500000, 0.500000, 0.500000, 0.500000, 1.500000, 0.500000, 0.500000}, {0.0, 0.000000, 1.000000, -1.000000, 0.000000, 1.000000, -1.000000, -1.000000}, {-1, 15, 9, 7, 13, 20, 5, 6}},
{{0.0, 0.500000, 0.500000, 0.500000, -0.500000, 0.500000, -0.500000, -0.500000}, {0.0, -1.000000, 3.000000, 2.000000, 1.000000, 0.000000, 4.000000, -2.000000}, {0.0, -0.500000, -0.500000, 0.500000, -0.500000, -0.500000, -0.500000, 0.500000}, {-1, 2, 12, 1, 16, 21, 17, 0}},
{{0.0, 1.000000, 1.000000, -1.000000, 0.000000, 1.000000, -1.000000, -1.000000}, {0.0, -1.000000, 3.000000, 2.000000, 0.000000, 0.000000, 0.000000, -1.000000}, {0.0, 0.000000, -1.000000, -1.000000, -1.000000, 1.000000, 0.000000, -1.000000}, {-1, 13, 12, 3, 10, 11, 17, 6}},
{{0.0, 2.000000, 0.000000, 1.000000, 0.000000, -2.000000, -1.000000, 2.000000}, {0.0, 1.500000, 1.500000, 0.500000, 1.500000, 0.500000, 0.500000, 1.500000}, {0.0, 1.000000, -1.000000, 0.000000, 1.000000, -1.000000, 1.000000, -1.000000}, {-1, 2, 2, 9, 2, 17, 11, 12}},
{{0.0, -1.000000, 2.000000, 0.000000, 1.000000, 2.000000, -2.000000, -2.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, 1.000000, 2.000000, -2.000000, 0.000000, -2.000000, -2.000000, 2.000000}, {-1, 9, 5, 7, 13, 6, 6, 0}},
{{0.0, 4.500000, -3.500000, 1.500000, -2.500000, 0.500000, -1.500000, 2.500000}, {0.0, -0.500000, 2.500000, -0.500000, 0.500000, 0.500000, -0.500000, 0.500000}, {0.0, 1.500000, -1.500000, 0.500000, -1.500000, -0.500000, -0.500000, 1.500000}, {-1, 1, 12, 0, 8, 23, 19, 3}},
{{0.0, 2.000000, 1.000000, 1.000000, 0.000000, -1.000000, -1.000000, -1.000000}, {0.0, 0.000000, 0.000000, 1.000000, 2.000000, 0.000000, 0.000000, 2.000000}, {0.0, 0.000000, 1.000000, -1.000000, -1.000000, 0.000000, -1.000000, 1.000000}, {-1, 11, 10, 16, 13, 6, 5, 3}},
{{0.0, -1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000}, {0.0, -1.500000, 3.500000, -1.500000, -0.500000, -0.500000, -1.500000, -0.500000}, {0.0, -2.000000, -1.000000, 1.000000, 0.000000, -2.000000, -1.000000, -1.000000}, {-1, 6, 23, 4, 1, 23, 21, 1}},
{{0.0, 1.000000, -2.000000, -2.000000, -2.000000, -1.000000, 0.000000, -2.000000}, {0.0, -1.000000, 1.000000, -1.000000, -1.000000, 1.000000, -1.000000, 2.000000}, {0.0, -2.000000, -2.000000, 1.000000, -1.000000, -2.000000, -2.000000, -2.000000}, {-1, 0, 15, 4, 4, 3, 5, 6}},
{{0.0, -2.000000, -2.000000, -1.000000, 1.000000, -2.000000, -2.000000, -2.000000}, {0.0, -1.000000, 1.000000, -1.000000, 0.000000, -1.000000, 1.000000, 2.000000}, {0.0, 1.000000, -2.000000, -2.000000, -2.000000, -1.000000, -1.000000, -2.000000}, {-1, 6, 8, 0, 2, 17, 23, 6}},
{{0.0, 0.000000, -2.000000, -1.000000, 1.000000, 2.000000, 2.000000, -2.000000}, {0.0, 0.000000, 0.000000, 2.000000, 2.000000, 0.000000, 0.000000, 0.000000}, {0.0, 1.000000, -1.000000, -1.000000, -1.000000, 0.000000, -1.000000, 0.000000}, {-1, 11, 0, 2, 8, 6, 5, 6}},
{{0.0, 0.000000, 2.000000, 1.000000, 0.000000, -2.000000, -1.000000, 0.000000}, {0.0, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000}, {0.0, -1.000000, 2.000000, 0.000000, 1.000000, 0.000000, -2.000000, 0.000000}, {-1, 9, 11, 7, 8, 9, 21, 0}},
{{0.0, -1.000000, 1.000000, -1.000000, 0.000000, 1.000000, 0.000000, 1.000000}, {0.0, 2.000000, 2.000000, 1.000000, 1.000000, 0.000000, 0.000000, 2.000000}, {0.0, -2.000000, -2.000000, 0.000000, 1.000000, -1.000000, -2.000000, 0.000000}, {-1, 15, 12, 19, 14, 6, 5, 2}},
{{0.0, 1.000000, -2.000000, -1.000000, -2.000000, 0.000000, -1.000000, -1.000000}, {0.0, -0.500000, 0.500000, -0.500000, 0.500000, 0.500000, -0.500000, -0.500000}, {0.0, -2.000000, -2.000000, -2.000000, 1.000000, -1.000000, -1.000000, 0.000000}, {-1, 0, 16, 0, 14, 20, 5, 6}},
{{0.0, 1.000000, 0.000000, 0.000000, 0.000000, -1.000000, 1.000000, -1.000000}, {0.0, -0.500000, -0.500000, 2.500000, -0.500000, -0.500000, -1.500000, -1.500000}, {0.0, -2.000000, -2.000000, -2.000000, 1.000000, 0.000000, -1.000000, -2.000000}, {-1, 15, 16, 0, 14, 20, 21, 6}},
{{0.0, -1.000000, 1.000000, 1.000000, -1.000000, -1.000000, -1.000000, -1.000000}, {0.0, 2.500000, 2.500000, -0.500000, 1.500000, 1.500000, 0.500000, -0.500000}, {0.0, 0.000000, -1.000000, 0.000000, -1.000000, 0.000000, -1.000000, 1.000000}, {-1, 7, 12, 21, 10, 7, 23, 0}},
{{0.0, -1.000000, -1.000000, 0.000000, -1.000000, 0.000000, 0.000000, 0.000000}, {0.0, 3.000000, 0.000000, -1.000000, 1.000000, 1.000000, -1.000000, 2.000000}, {0.0, -1.000000, 1.000000, -1.000000, 0.000000, -1.000000, 0.000000, -1.000000}, {-1, 15, 4, 0, 4, 7, 5, 6}},
{{0.0, 0.000000, 1.000000, -1.000000, 0.000000, -1.000000, 0.000000, 0.000000}, {0.0, 0.000000, 0.000000, 0.000000, 1.000000, 2.000000, 0.000000, 1.000000}, {0.0, 1.000000, 0.000000, 0.000000, -1.000000, 0.000000, -1.000000, 1.000000}, {-1, 7, 10, 7, 10, 3, 11, 0}},
{{0.0, 2.000000, -1.000000, -2.000000, 1.000000, 0.000000, 0.000000, 0.000000}, {0.0, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000}, {0.0, -1.000000, -2.000000, 0.000000, 1.000000, 0.000000, 1.000000, 0.000000}, {-1, 13, 7, 17, 7, 9, 5, 1}},
{{0.0, -2.000000, 2.000000, 0.000000, 1.000000, 0.000000, -1.000000, 0.000000}, {0.0, 0.000000, 0.000000, 1.000000, 1.000000, 2.000000, 1.000000, 0.000000}, {0.0, -1.000000, -1.000000, -1.000000, 0.000000, 1.000000, 1.000000, 0.000000}, {-1, 7, 21, 1, 14, 20, 20, 5}},
{{0.0, -1.000000, 0.000000, 0.000000, -1.000000, 0.000000, -1.000000, -1.000000}, {0.0, 1.000000, 2.000000, -2.000000, 1.000000, -2.000000, -2.000000, 0.000000}, {0.0, -1.000000, -1.000000, -1.000000, 0.000000, 0.000000, 0.000000, -1.000000}, {-1, 0, 16, 0, 10, 0, 6, 8}},
{{0.0, -1.000000, 0.000000, -1.000000, 0.000000, 0.000000, 0.000000, -1.000000}, {0.0, 1.000000, 2.000000, -2.000000, 1.000000, -2.000000, -2.000000, 0.000000}, {0.0, -1.000000, 0.000000, 0.000000, -1.000000, -1.000000, 0.000000, -1.000000}, {-1, 6, 1, 4, 16, 0, 6, 8}},
{{0.0, 2.000000, 0.000000, -1.000000, 1.000000, -1.000000, 0.000000, 0.000000}, {0.0, 2.000000, 1.000000, 2.000000, 1.000000, 0.000000, 2.000000, 0.000000}, {0.0, -1.000000, 1.000000, 1.000000, -1.000000, 0.000000, 0.000000, 0.000000}, {-1, 2, 2, 9, 2, 6, 9, 0}},
{{0.0, 0.000000, 3.000000, -3.000000, -1.000000, 1.000000, 1.000000, -1.000000}, {0.0, 1.000000, 2.000000, 1.000000, 2.000000, 0.000000, 2.000000, 0.000000}, {0.0, 0.000000, -1.000000, 1.000000, 0.000000, -1.000000, -1.000000, 1.000000}, {-1, 2, 12, 3, 7, 17, 23, 1}},
{{0.0, 2.000000, 0.000000, -2.000000, 1.000000, -1.000000, 1.000000, -1.000000}, {0.0, 1.000000, 2.000000, 1.000000, 0.000000, 0.000000, 2.000000, 2.000000}, {0.0, -2.000000, 0.000000, 2.000000, -2.000000, 0.000000, -1.000000, 1.000000}, {-1, 1, 20, 19, 7, 6, 20, 2}},
{{0.0, -1.500000, 2.500000, 1.500000, -0.500000, -2.500000, -0.500000, -2.500000}, {0.0, 1.500000, 0.500000, 0.500000, 1.500000, 1.500000, 0.500000, 0.500000}, {0.0, 0.500000, 0.500000, -1.500000, -0.500000, 1.500000, 0.500000, -0.500000}, {-1, 7, 11, 7, 2, 20, 21, 6}},
{{0.0, -1.500000, 2.500000, -0.500000, 1.500000, -0.500000, -2.500000, -2.500000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 1.500000, 0.500000, 1.500000}, {0.0, 1.000000, -2.000000, 0.000000, -1.000000, 1.000000, 2.000000, 0.000000}, {-1, 7, 13, 0, 13, 21, 19, 8}},
{{0.0, 1.500000, -1.500000, -1.500000, 1.500000, -2.500000, -0.500000, 2.500000}, {0.0, 0.500000, 0.500000, 0.500000, 1.500000, 1.500000, 1.500000, 0.500000}, {0.0, -1.500000, 0.500000, 1.500000, -1.500000, 2.500000, -0.500000, -2.500000}, {-1, 13, 0, 0, 22, 20, 23, 5}},
{{0.0, -1.000000, 1.000000, -1.000000, 1.000000, 0.000000, -2.000000, 2.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, 2.000000, 1.000000, 0.000000, -1.000000, -3.000000, -2.000000, -2.000000}, {-1, 7, 21, 7, 13, 6, 6, 5}},
{{0.0, 1.500000, -0.500000, -0.500000, -0.500000, 0.500000, 1.500000, -0.500000}, {0.0, -0.500000, -1.500000, -0.500000, 2.500000, 0.500000, 2.500000, 2.500000}, {0.0, -0.500000, -0.500000, 1.500000, 0.500000, -0.500000, -0.500000, -0.500000}, {-1, 1, 7, 19, 23, 6, 5, 1}},
{{0.0, 1.000000, 0.000000, 0.000000, 0.000000, -1.000000, 1.000000, -1.000000}, {0.0, -1.500000, 3.500000, 0.500000, 1.500000, -0.500000, -0.500000, -0.500000}, {0.0, 0.000000, 1.000000, 0.000000, -2.000000, 0.000000, -2.000000, -2.000000}, {-1, 6, 23, 4, 23, 3, 15, 8}},
{{0.0, -1.000000, 0.000000, 0.000000, 1.000000, 0.000000, -1.000000, 1.000000}, {0.0, -0.500000, 3.500000, -0.500000, -0.500000, -1.500000, -1.500000, -1.500000}, {0.0, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, -2.000000, -2.000000}, {-1, 14, 23, 0, 14, 6, 6, 5}},
{{0.0, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, -1.000000, -1.000000}, {0.0, -1.000000, 3.000000, -1.000000, 0.000000, 0.000000, 0.000000, 0.000000}, {0.0, -2.000000, 1.000000, 0.000000, -1.000000, 0.000000, -2.000000, 1.000000}, {-1, 6, 23, 7, 8, 21, 23, 3}},
{{0.0, 1.000000, 0.000000, 0.000000, -1.000000, -1.000000, 1.000000, 1.000000}, {0.0, 0.500000, 2.500000, 0.500000, -0.500000, -0.500000, -0.500000, -0.500000}, {0.0, 0.000000, 1.000000, -2.000000, -1.000000, 0.000000, -1.000000, 1.000000}, {-1, 11, 23, 2, 4, 17, 11, 1}},
{{0.0, -1.000000, 0.000000, 0.000000, 0.000000, -1.000000, 1.000000, 1.000000}, {0.0, 0.000000, 4.000000, 0.000000, -1.000000, -1.000000, 1.000000, -2.000000}, {0.0, 1.000000, 1.000000, 0.000000, -1.000000, 1.000000, 1.000000, 1.000000}, {-1, 6, 23, 16, 14, 3, 12, 5}},
{{0.0, 0.000000, 0.000000, 1.000000, 0.000000, -1.000000, -1.000000, 0.000000}, {0.0, 1.000000, 0.000000, 2.000000, 0.000000, 2.000000, 3.000000, 3.000000}, {0.0, 1.000000, 0.000000, 0.000000, -1.000000, 0.000000, 0.000000, 1.000000}, {-1, 0, 23, 16, 4, 3, 17, 6}},
{{0.0, -0.500000, 0.500000, -0.500000, 0.500000, -0.500000, 0.500000, 0.500000}, {0.0, -2.500000, -2.500000, 2.500000, 0.500000, -1.500000, -1.500000, 4.500000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, -1.500000, -1.500000, 0.500000}, {-1, 1, 10, 3, 1, 23, 23, 12}},
{{0.0, -3.000000, 0.000000, 3.000000, 1.000000, -2.000000, -2.000000, -1.000000}, {0.0, 1.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 0.000000}, {0.0, 0.000000, -1.000000, 0.000000, 0.000000, 1.000000, -2.000000, 0.000000}, {-1, 3, 21, 1, 2, 6, 21, 5}},
{{0.0, 1.500000, -0.500000, -1.500000, 0.500000, -1.500000, 0.500000, -1.500000}, {0.0, 1.000000, 1.000000, 2.000000, 1.000000, 0.000000, 0.000000, 0.000000}, {0.0, 0.000000, 1.000000, 0.000000, -1.000000, -1.000000, 0.000000, 1.000000}, {-1, 14, 9, 14, 2, 0, 6, 0}},
{{0.0, -0.500000, 1.500000, -1.500000, -0.500000, -1.500000, 0.500000, -1.500000}, {0.0, 2.000000, 0.000000, 2.000000, 1.000000, 0.000000, 1.000000, 0.000000}, {0.0, 0.000000, 1.000000, 0.000000, -1.000000, -1.000000, 0.000000, 1.000000}, {-1, 15, 22, 14, 10, 0, 15, 0}},
{{0.0, 0.000000, -1.000000, 1.000000, -1.000000, -1.000000, 0.000000, 1.000000}, {0.0, 0.500000, -0.500000, 1.500000, 1.500000, -0.500000, 1.500000, -0.500000}, {0.0, 0.000000, -1.000000, -1.000000, 0.000000, 0.000000, 0.000000, 1.000000}, {-1, 9, 0, 1, 23, 17, 17, 1}},
{{0.0, -2.000000, 2.000000, 0.000000, 0.000000, -2.000000, 0.000000, 1.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, 0.500000, -0.500000, -1.500000, -0.500000, 2.500000, 1.500000, 2.500000}, {-1, 9, 21, 9, 13, 9, 21, 0}},
{{0.0, -2.500000, 2.500000, 0.500000, 1.500000, -1.500000, -1.500000, -0.500000}, {0.0, 1.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000}, {0.0, 0.000000, 0.000000, -1.000000, 0.000000, 1.000000, -2.000000, 0.000000}, {-1, 3, 5, 7, 7, 6, 21, 5}},
{{0.0, -1.000000, 1.000000, 0.000000, 0.000000, -1.000000, 0.000000, 1.000000}, {0.0, 0.500000, -1.500000, 3.500000, 1.500000, -0.500000, 0.500000, -1.500000}, {0.0, -1.000000, -1.000000, 0.000000, -1.000000, 0.000000, -1.000000, 0.000000}, {-1, 15, 5, 14, 16, 3, 8, 5}},
{{0.0, 1.000000, -2.000000, 0.000000, -1.000000, 2.000000, 2.000000, -1.000000}, {0.0, 2.000000, 0.000000, 2.000000, 2.000000, 1.000000, 1.000000, 1.000000}, {0.0, 1.000000, -1.000000, -1.000000, 1.000000, 0.000000, -1.000000, 0.000000}, {-1, 14, 6, 7, 8, 21, 12, 1}},
{{0.0, 1.000000, 2.000000, 0.000000, 1.000000, -1.000000, -2.000000, -2.000000}, {0.0, 2.000000, 0.000000, 2.000000, 1.000000, 2.000000, 0.000000, 1.000000}, {0.0, 1.000000, -1.000000, -1.000000, 0.000000, 0.000000, 1.000000, -1.000000}, {-1, 14, 6, 7, 18, 15, 19, 8}},
{{0.0, -1.000000, 1.000000, 0.000000, 0.000000, 0.000000, -1.000000, 1.000000}, {0.0, 0.500000, 0.500000, 0.500000, 1.500000, 1.500000, 0.500000, 0.500000}, {0.0, -0.500000, -1.500000, -2.500000, 0.500000, -1.500000, 2.500000, 2.500000}, {-1, 7, 6, 7, 14, 11, 19, 1}},
{{0.0, 1.000000, -1.000000, -1.000000, 0.000000, 1.000000, 1.000000, -1.000000}, {0.0, 2.500000, 3.500000, -0.500000, 1.500000, -1.500000, -1.500000, -1.500000}, {0.0, -1.000000, -1.000000, -1.000000, -1.000000, 1.000000, -1.000000, 1.000000}, {-1, 2, 8, 3, 2, 19, 5, 0}},
{{0.0, -1.000000, -1.000000, 1.000000, 0.000000, 0.000000, -1.000000, 0.000000}, {0.0, -0.500000, -0.500000, 0.500000, 0.500000, 1.500000, 2.500000, -0.500000}, {0.0, 1.000000, -1.000000, 0.000000, -1.000000, 0.000000, -1.000000, 1.000000}, {-1, 4, 10, 16, 10, 23, 17, 0}},
{{0.0, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, -1.000000, -1.000000}, {0.0, 0.500000, 0.500000, 1.500000, -0.500000, -0.500000, 0.500000, -0.500000}, {0.0, 1.500000, 1.500000, -1.500000, 0.500000, -1.500000, -0.500000, -1.500000}, {-1, 11, 14, 16, 7, 6, 17, 6}},
{{0.0, -1.000000, 0.000000, 1.000000, 0.000000, -1.000000, -1.000000, 1.000000}, {0.0, -0.500000, 2.500000, 1.500000, 0.500000, 1.500000, 2.500000, -0.500000}, {0.0, 0.000000, 1.000000, 0.000000, -1.000000, -1.000000, -1.000000, 1.000000}, {-1, 9, 3, 16, 23, 3, 17, 1}},
{{0.0, -1.500000, -1.500000, -0.500000, -1.500000, -1.500000, 1.500000, 1.500000}, {0.0, 2.000000, 2.000000, 1.000000, 0.000000, 0.000000, 0.000000, 2.000000}, {0.0, 1.500000, -1.500000, -1.500000, -0.500000, 1.500000, -1.500000, -1.500000}, {-1, 3, 15, 3, 4, 9, 21, 12}},
{{0.0, -4.000000, 4.000000, 0.000000, 2.000000, 0.000000, -1.000000, -2.000000}, {0.0, -0.500000, -0.500000, 1.500000, 0.500000, -0.500000, -0.500000, -0.500000}, {0.0, 0.500000, 0.500000, -0.500000, -0.500000, -0.500000, -0.500000, -0.500000}, {-1, 9, 11, 0, 2, 0, 6, 5}},
{{0.0, 0.500000, -0.500000, 0.500000, 0.500000, -0.500000, 0.500000, -0.500000}, {0.0, -1.000000, 2.000000, 1.000000, 2.000000, -1.000000, -1.000000, -1.000000}, {0.0, 0.500000, -1.500000, 0.500000, -1.500000, -1.500000, -1.500000, 0.500000}, {-1, 7, 15, 1, 23, 6, 6, 5}},
{{0.0, -1.000000, 1.000000, 0.000000, 1.000000, -1.000000, -1.000000, 1.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 1.500000, 1.500000, 1.500000}, {0.0, 0.500000, 2.500000, -1.500000, 0.500000, 1.500000, -2.500000, -2.500000}, {-1, 9, 11, 7, 4, 20, 23, 12}},
{{0.0, -1.000000, 0.000000, -1.000000, 1.000000, 1.000000, -1.000000, 1.000000}, {0.0, 1.500000, 0.500000, -0.500000, 1.500000, -0.500000, 0.500000, -0.500000}, {0.0, -1.500000, -1.500000, 0.500000, -1.500000, -1.500000, -1.500000, 0.500000}, {-1, 6, 16, 4, 16, 6, 23, 5}},
{{0.0, 3.000000, -3.000000, -1.000000, 1.000000, 2.000000, -2.000000, 0.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, 0.500000, 0.500000, -0.500000, -0.500000, 2.500000, 2.500000, -1.500000}, {-1, 13, 7, 9, 7, 9, 11, 0}},
{{0.0, 1.000000, 1.000000, -1.000000, 0.000000, 0.000000, 0.000000, 1.000000}, {0.0, 0.500000, 0.500000, 0.500000, 1.500000, 1.500000, 1.500000, 0.500000}, {0.0, 2.000000, -1.000000, 0.000000, 3.000000, 2.000000, -1.000000, 1.000000}, {-1, 13, 22, 4, 8, 11, 17, 1}},
{{0.0, -1.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 1.500000, 0.500000, 1.500000}, {0.0, 0.500000, 1.500000, -2.500000, -0.500000, 2.500000, 3.500000, 3.500000}, {-1, 9, 13, 9, 18, 11, 11, 2}},
{{0.0, -1.000000, 2.000000, 0.000000, 1.000000, 2.000000, -2.000000, -2.000000}, {0.0, 0.500000, -0.500000, 2.500000, 0.500000, 1.500000, 1.500000, -0.500000}, {0.0, -0.500000, -0.500000, -0.500000, -0.500000, -0.500000, -0.500000, -0.500000}, {-1, 7, 13, 2, 7, 21, 17, 6}},
{{0.0, 1.000000, 1.000000, 0.000000, -1.000000, 0.000000, -1.000000, -1.000000}, {0.0, 0.500000, 1.500000, 1.500000, 0.500000, -0.500000, -0.500000, -0.500000}, {0.0, -0.500000, -1.500000, -1.500000, -0.500000, 0.500000, 0.500000, -1.500000}, {-1, 15, 23, 1, 10, 0, 6, 6}},
{{0.0, -1.000000, -1.000000, 0.000000, 1.000000, -1.000000, 1.000000, 0.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, -2.500000, 1.500000, 2.500000, -3.500000, -0.500000, 0.500000, -1.500000}, {-1, 11, 9, 7, 13, 9, 11, 0}},
{{0.0, 1.000000, 0.000000, 1.000000, -1.000000, -2.000000, 2.000000, -1.000000}, {0.0, 0.500000, -0.500000, 1.500000, 1.500000, -0.500000, 0.500000, 0.500000}, {0.0, -0.500000, -0.500000, -0.500000, -0.500000, -0.500000, 0.500000, 0.500000}, {-1, 13, 7, 0, 1, 17, 20, 3}},
{{0.0, 1.000000, 0.000000, 0.000000, -1.000000, -1.000000, -1.000000, 1.000000}, {0.0, 0.000000, 0.000000, 1.000000, 2.000000, -1.000000, 0.000000, -1.000000}, {0.0, -1.000000, 0.000000, -1.000000, -1.000000, 0.000000, -1.000000, 1.000000}, {-1, 15, 16, 1, 16, 17, 23, 1}},
{{0.0, -1.500000, -1.500000, -0.500000, -0.500000, -1.500000, 1.500000, 1.500000}, {0.0, 2.500000, -0.500000, -0.500000, 1.500000, 0.500000, 0.500000, -0.500000}, {0.0, -0.500000, -0.500000, -1.500000, -1.500000, 1.500000, -1.500000, 1.500000}, {-1, 9, 16, 0, 1, 20, 12, 1}},
{{0.0, -1.500000, -1.500000, 1.500000, 0.500000, -0.500000, -0.500000, -1.500000}, {0.0, 2.000000, 2.000000, 1.000000, 0.000000, 2.000000, 0.000000, 0.000000}, {0.0, -1.500000, 0.500000, -1.500000, -1.500000, -1.500000, 1.500000, -0.500000}, {-1, 15, 23, 1, 13, 3, 11, 0}},
{{0.0, 0.000000, 2.000000, 1.000000, -1.000000, -1.000000, -1.000000, 1.000000}, {0.0, 1.000000, 0.000000, 0.000000, 0.000000, 2.000000, 0.000000, 2.000000}, {0.0, -1.000000, 1.000000, -1.000000, 0.000000, 0.000000, -1.000000, 0.000000}, {-1, 0, 11, 7, 7, 20, 5, 2}},
{{0.0, 0.000000, -2.000000, -1.000000, 0.000000, 1.000000, 1.000000, -2.000000}, {0.0, 1.500000, 0.500000, 1.500000, 0.500000, 0.500000, 1.500000, 0.500000}, {0.0, 1.000000, 1.000000, 0.000000, -1.000000, 0.000000, 0.000000, -1.000000}, {-1, 9, 8, 14, 10, 21, 19, 8}},
{{0.0, 1.000000, 0.000000, 0.000000, 0.000000, -1.000000, 1.000000, -1.000000}, {0.0, 1.000000, -1.000000, 2.000000, 0.000000, 1.000000, -1.000000, -1.000000}, {0.0, 0.000000, 1.000000, -1.000000, -1.000000, 0.000000, 0.000000, -1.000000}, {-1, 11, 11, 0, 10, 20, 6, 6}},
{{0.0, -2.000000, 0.000000, -2.000000, 0.000000, -1.000000, 2.000000, 2.000000}, {0.0, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 1.000000, 0.000000}, {0.0, 1.000000, -1.000000, -1.000000, 0.000000, -1.000000, -1.000000, 0.000000}, {-1, 9, 0, 16, 13, 15, 5, 5}},
{{0.0, 0.500000, -1.500000, 0.500000, -1.500000, 0.500000, -0.500000, -0.500000}, {0.0, 2.000000, 2.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000}, {0.0, -1.500000, -1.500000, -0.500000, -0.500000, 0.500000, -1.500000, 0.500000}, {-1, 15, 17, 17, 4, 19, 12, 5}},
{{0.0, 0.000000, 1.000000, 1.000000, -1.000000, -1.000000, 0.000000, 1.000000}, {0.0, 2.500000, 1.500000, -0.500000, 1.500000, -0.500000, -0.500000, 1.500000}, {0.0, 0.000000, 1.000000, 0.000000, -1.000000, -1.000000, 1.000000, -1.000000}, {-1, 3, 2, 4, 23, 17, 11, 12}},
{{0.0, 1.500000, -1.500000, 0.500000, 0.500000, -0.500000, 1.500000, -1.500000}, {0.0, 0.000000, 0.000000, 1.000000, 0.000000, 1.000000, 2.000000, 2.000000}, {0.0, -1.500000, 1.500000, -0.500000, 0.500000, 0.500000, -1.500000, 0.500000}, {-1, 13, 0, 4, 18, 23, 12, 8}},
{{0.0, -0.500000, 1.500000, -1.500000, -0.500000, -0.500000, 0.500000, -0.500000}, {0.0, 1.000000, 2.000000, 1.000000, 0.000000, 2.000000, 2.000000, 0.000000}, {0.0, -1.500000, 0.500000, 0.500000, -0.500000, 1.500000, -0.500000, 1.500000}, {-1, 6, 12, 3, 18, 11, 12, 1}},
{{0.0, -1.000000, 2.000000, 0.000000, 0.000000, 1.000000, 0.000000, -2.000000}, {0.0, -0.500000, -0.500000, 1.500000, 0.500000, 0.500000, 0.500000, -0.500000}, {0.0, 1.000000, 1.000000, -1.000000, 0.000000, -1.000000, -1.000000, -1.000000}, {-1, 11, 22, 0, 16, 3, 12, 6}},
{{0.0, -1.000000, 1.000000, 1.000000, -1.000000, 1.000000, 0.000000, -1.000000}, {0.0, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 2.000000}, {0.0, 1.000000, 1.000000, -1.000000, -1.000000, 0.000000, -1.000000, -1.000000}, {-1, 3, 11, 1, 23, 21, 21, 6}},
{{0.0, 4.000000, -3.000000, 2.000000, 0.000000, -4.000000, -4.000000, -2.000000}, {0.0, 0.500000, 0.500000, 1.500000, 1.500000, 0.500000, 0.500000, 0.500000}, {0.0, 0.000000, -1.000000, 0.000000, 0.000000, -2.000000, 1.000000, 0.000000}, {-1, 1, 17, 2, 2, 17, 6, 6}},
{{0.0, 1.500000, -0.500000, 0.500000, -1.500000, -1.500000, -0.500000, -1.500000}, {0.0, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 1.000000, 1.000000}, {0.0, -0.500000, -1.500000, -1.500000, -1.500000, 1.500000, -0.500000, 0.500000}, {-1, 4, 21, 3, 16, 9, 9, 6}},
{{0.0, -0.500000, -1.500000, -1.500000, -1.500000, 1.500000, 0.500000, 1.500000}, {0.0, 0.000000, -1.000000, 2.000000, 0.000000, 0.000000, 0.000000, -1.000000}, {0.0, -1.000000, 1.000000, 0.000000, 0.000000, -1.000000, 0.000000, 1.000000}, {-1, 3, 18, 4, 10, 23, 15, 1}},
{{0.0, -1.500000, -0.500000, -1.500000, -0.500000, -0.500000, 0.500000, 1.500000}, {0.0, 1.000000, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000}, {0.0, -1.500000, -0.500000, 0.500000, -1.500000, 1.500000, -0.500000, -1.500000}, {-1, 6, 7, 4, 8, 19, 21, 5}},
{{0.0, 1.500000, -0.500000, 0.500000, -0.500000, -1.500000, -0.500000, 0.500000}, {0.0, -0.500000, 0.500000, -0.500000, 1.500000, -0.500000, 2.500000, 0.500000}, {0.0, 1.500000, -0.500000, -0.500000, 0.500000, 0.500000, -0.500000, -0.500000}, {-1, 11, 15, 17, 10, 0, 8, 0}},
{{0.0, -1.000000, -2.000000, 0.000000, 1.000000, 0.000000, 2.000000, -1.000000}, {0.0, 2.000000, 0.000000, 0.000000, 2.000000, 1.000000, 0.000000, 1.000000}, {0.0, -1.000000, -1.000000, 1.000000, -1.000000, -1.000000, -1.000000, 0.000000}, {-1, 13, 0, 9, 7, 0, 5, 3}},
{{0.0, 0.000000, 2.000000, 0.000000, 2.000000, -2.000000, 0.000000, -2.000000}, {0.0, 0.000000, 1.000000, 2.000000, 0.000000, 1.000000, 0.000000, 0.000000}, {0.0, -1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, -1.000000}, {-1, 0, 14, 14, 22, 20, 21, 6}},
{{0.0, -1.500000, 0.500000, 1.500000, -0.500000, -1.500000, 1.500000, 0.500000}, {0.0, 1.000000, 1.000000, 0.000000, 0.000000, 2.000000, 2.000000, 0.000000}, {0.0, 0.500000, 1.500000, 0.500000, 0.500000, 1.500000, -1.500000, -1.500000}, {-1, 3, 14, 4, 7, 20, 12, 6}},
{{0.0, 1.000000, -1.000000, -1.000000, 0.000000, 0.000000, 1.000000, 1.000000}, {0.0, 2.500000, 0.500000, -0.500000, 0.500000, 0.500000, 0.500000, -0.500000}, {0.0, 0.000000, 0.000000, 0.000000, -1.000000, 1.000000, 0.000000, 1.000000}, {-1, 2, 1, 9, 2, 11, 9, 1}},
{{0.0, 0.000000, -1.000000, 0.000000, 0.000000, 0.000000, -1.000000, 1.000000}, {0.0, 1.000000, 2.000000, 2.000000, 1.000000, 0.000000, 0.000000, 0.000000}, {0.0, 0.000000, -1.000000, 1.000000, -1.000000, 1.000000, 0.000000, 0.000000}, {-1, 13, 8, 9, 8, 9, 21, 0}},
{{0.0, 0.000000, 1.000000, 0.000000, 1.000000, -1.000000, -1.000000, 0.000000}, {0.0, 1.500000, 0.500000, 3.500000, -1.500000, -1.500000, -1.500000, -0.500000}, {0.0, -1.000000, -1.000000, -1.000000, 0.000000, -1.000000, 0.000000, 0.000000}, {-1, 6, 2, 7, 18, 0, 5, 0}},
{{0.0, 0.000000, 1.000000, -1.000000, -2.000000, 0.000000, -1.000000, 0.000000}, {0.0, 2.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.000000, 2.000000}, {0.0, 1.000000, 0.000000, 1.000000, 0.000000, 0.000000, -1.000000, -1.000000}, {-1, 3, 10, 0, 4, 19, 12, 8}},
{{0.0, 2.000000, 0.000000, -1.000000, 1.000000, -1.000000, 0.000000, -2.000000}, {0.0, 0.500000, 1.500000, 0.500000, 0.500000, 1.500000, 1.500000, 0.500000}, {0.0, 0.000000, 0.000000, -1.000000, -2.000000, 2.000000, 1.000000, 1.000000}, {-1, 13, 2, 17, 13, 20, 23, 0}},
{{0.0, 2.000000, -2.000000, 2.000000, -2.000000, -1.000000, 1.000000, 0.000000}, {0.0, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000, 0.500000}, {0.0, 2.000000, 2.000000, -1.000000, -2.000000, -1.000000, -2.000000, 0.000000}, {-1, 11, 9, 17, 7, 6, 21, 0}},
{{0.0, 0.500000, 1.500000, -1.500000, 0.500000, 0.500000, -0.500000, 1.500000}, {0.0, 0.000000, 1.000000, 1.000000, 1.000000, 1.000000, 0.000000, 0.000000}, {0.0, 0.500000, 1.500000, 0.500000, -1.500000, 0.500000, 1.500000, -1.500000}, {-1, 7, 14, 3, 16, 6, 11, 5}},
{{0.0, 0.500000, -1.500000, 0.500000, -0.500000, 1.500000, -1.500000, 1.500000}, {0.0, 2.000000, 2.000000, 1.000000, 1.000000, 1.000000, 0.000000, 1.000000}, {0.0, -1.500000, 0.500000, -0.500000, 0.500000, -1.500000, 1.500000, 1.500000}, {-1, 7, 7, 14, 2, 23, 19, 2}},
{{0.0, -1.500000, 0.500000, -0.500000, -0.500000, -1.500000, -1.500000, -1.500000}, {0.0, -2.000000, -2.000000, -1.000000, -2.000000, 1.000000, 2.000000, 3.000000}, {0.0, -0.500000, -1.500000, -1.500000, 0.500000, -1.500000, -1.500000, -1.500000}, {-1, 4, 17, 1, 22, 3, 17, 6}}};
int somafigurescount = 105;

#endif