#ifndef FLIPGUIDE_H
#define FLIPGUIDE_H

#define SQRT2_2 0.707106781
#define SQRT3_3 0.577350269

typedef struct
{
  GLfloat degrees;
  GLfloat rotvector[3];
} flipguide;

static
flipguide s4[] = {{0., {0., 0., 1.}}, {90., {0., 0., 1.}},
                         {180., {0., 0., 1.}}, {270., {0., 0., 1.}},
                         {90., {0., 1., 0.}}, {180., {0., 1., 0.}},
                         {270., {0., 1., 0.}}, {90., {1., 0., 0.}},
                         {180., {1., 0., 0.}}, {270., {1., 0., 0.}},
                         {180., {SQRT2_2, SQRT2_2, 0.}},
                         {180., {0., SQRT2_2, -SQRT2_2}},
                         {180., {-SQRT2_2, SQRT2_2, 0.}},
                         {180., {0., SQRT2_2, SQRT2_2}},
                         {180., {-SQRT2_2, 0., SQRT2_2}}, 
                         {180., {SQRT2_2, 0., SQRT2_2}},
                         {120., {SQRT3_3, SQRT3_3, SQRT3_3}}, 
                         {240., {SQRT3_3, SQRT3_3, SQRT3_3}},
                         {120., {SQRT3_3, SQRT3_3, -SQRT3_3}}, 
                         {240., {SQRT3_3, SQRT3_3, -SQRT3_3}},
                         {120., {-SQRT3_3, SQRT3_3, -SQRT3_3}}, 
                         {240., {-SQRT3_3, SQRT3_3, -SQRT3_3}},
                         {120., {-SQRT3_3, SQRT3_3, SQRT3_3}}, 
                         {240., {-SQRT3_3, SQRT3_3, SQRT3_3}}};


#endif
