/*  figuretest.c
 *  test driver for the soma cube drawing functions
 *  that will go in the soma screenhack.
 *  includes display lists to draw the seven pieces
 *  and the orientation guide to turn them in the proper direction.
 */

/* the plan is to have the parser
 * generate somafigures.h at compile time 
 * using a (possibly user supplied) solutions file,
 * and possibly make a separate .c/.h file for the piece drawing.
 */

#define FIGURE somafigures[currentfigure]

#define SQRT3 1.732050808
#define COLOROFFSET 10

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>  /* for M_SQRT2 */
#include "bevelcube.h"

typedef struct
{
  GLfloat degrees;
  GLfloat rotvector[3];
} orientationguide;

typedef struct
{
  GLfloat refcubex[8];
  GLfloat refcubey[8];
  GLfloat refcubez[8];
  int     orientation[8];
  GLfloat figlength;
  GLfloat figdepth;
  GLfloat figwidth;
} figure;

#include "somafigures.h"


orientationguide s4[] = {{0., {0., 0., 1.}}, {90., {0., 0., 1.}},
			 {180., {0., 0., 1.}}, {270., {0., 0., 1.}},
			 {90., {0., 1., 0.}}, {180., {0., 1., 0.}},
			 {270., {0., 1., 0.}}, {90., {1., 0., 0.}},
			 {180., {1., 0., 0.}}, {270, {1., 0., 0.}},
			 {180., {M_SQRT2, M_SQRT2, 0.}},
			 {180., {0., M_SQRT2, -M_SQRT2}},
			 {180., {-M_SQRT2, M_SQRT2, 0.}},
			 {180., {0., M_SQRT2, M_SQRT2}},
			 {180., {-M_SQRT2, 0., M_SQRT2}}, 
			 {180., {M_SQRT2, 0., M_SQRT2}},
			 {120., {SQRT3, SQRT3, SQRT3}}, 
			 {240, {SQRT3, SQRT3, SQRT3}},
			 {120., {SQRT3, SQRT3, -SQRT3}}, 
			 {240, {SQRT3, SQRT3, -SQRT3}},
			 {120., {-SQRT3, SQRT3, -SQRT3}}, 
			 {240, {-SQRT3, SQRT3, -SQRT3}},
			 {120., {-SQRT3, SQRT3, SQRT3}}, 
			 {240, {-SQRT3, SQRT3, SQRT3}}};
			 


int currentorientation, previousorientation, temporientation;

GLfloat angle, height;
GLfloat xeye, zeye;
GLuint listindex;
GLuint currentcube[2];
int currentfigure = 0;


/* gold and silver are from the teapots demo from the redbook */
GLfloat gold_ambient[] = {0.24725, 0.1995, 0.0745, 1.0};
GLfloat gold_diffuse[] = {0.75164, 0.60648, 0.22648, 1.0};
GLfloat gold_specular[] = {0.628281, 0.555802, 0.366065, 1,0};
GLfloat gold_shininess = 0.4;

GLfloat silver_ambient[] = {0.19225, 0.19225, 0.19225, 1.0};
GLfloat silver_diffuse[] = {0.50754, 0.50754, 0.50754, 1.0};
GLfloat silver_specular[] = {0.508273, 0.508273, 1,0};
GLfloat silver_shininess = 0.4;

GLfloat red_ambient[] = {1., 0., 0., 1.0};
GLfloat red_diffuse[] = {1., 0., 0., 1.0};
GLfloat red_specular[] = {1., 0., 0., 1.};
GLfloat red_shininess = 0.4;

GLfloat green_ambient[] = {0., 1., 0., 1.0};
GLfloat green_diffuse[] = {0., 1., 0., 1.0};
GLfloat green_specular[] = {0., 1., 0., 1.};
GLfloat green_shininess = 0.4;

GLfloat blue_ambient[] = {0., 0. ,1., 1.0};
GLfloat blue_diffuse[] = {0., 0. ,1., 1.0};
GLfloat blue_specular[] = {0., 0., 1., 1.};
GLfloat blue_shininess = 0.4;

GLfloat cyan_ambient[] = {0., 1., 1., 1.0};
GLfloat cyan_diffuse[] = {0., 1., 1., 1.0};
GLfloat cyan_specular[] = {0., 1., 1., 1.0};
GLfloat cyan_shininess = 0.4;

GLfloat yellow_ambient[] = {1., 1., 0., 1.0};
GLfloat yellow_diffuse[] = {1., 1., 0., 1.0};
GLfloat yellow_specular[] = {1., 1., 0., 1.0};
GLfloat yellow_shininess = 0.4;

GLfloat magenta_ambient[] = {1., 0., 1., 1.0};
GLfloat magenta_diffuse[] = {1., 0., 1., 1.0};
GLfloat magenta_specular[] = {1., 0., 1., 1.};
GLfloat magenta_shininess = 0.4;

GLfloat white_ambient[] = {1., 1., 1., 1.0};
GLfloat white_diffuse[] = {1., 1., 1., 1.0};
GLfloat white_specular[] = {1., 1., 1., 1.};
GLfloat white_shininess = 0.4;



void makedlist(void)
{

  listindex = glGenLists(18);

  glNewList(listindex + 1, GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(-1.0, 1.0, 0.0);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  glNewList(listindex + 2, GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(1., 0., 0.);
   drawbevelcube();
   glTranslatef(-2.0, 1.0, 0.0);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  glNewList(listindex + 3, GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(-2.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(1., 1.0, 0.0);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  glNewList(listindex + 4, GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(-1.0, 1.0, 0.0);
   drawbevelcube();
   glTranslatef(-1., 0., 0.);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  glNewList(listindex + 5, GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(0.0, 1.0, 0.0);
   drawbevelcube();
   glTranslatef(1.0, -1.0, 0.0);
   drawbevelcube();
   glTranslatef(0.0, 0.0, 1.0);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  glNewList(listindex + 6, GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(0.0, 1.0, 0.0);
   drawbevelcube();
   glTranslatef(1.0, -1.0, 0.0);
   drawbevelcube();
   glTranslatef(0.0, 0.0, -1.0);
  drawbevelcube();
   glPopMatrix();
   glEndList();

  glNewList(listindex + 7, GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(-1.0, 1.0, 0.0);
   drawbevelcube();
   glTranslatef(0., -1., -1.);
   drawbevelcube();
   glPopMatrix();
   glEndList();

   glNewList(listindex + COLOROFFSET + 1, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT, red_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, red_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, red_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, red_shininess * 128.0);
   glEndList();

   glNewList(listindex + COLOROFFSET + 2, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT, green_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, green_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, green_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, green_shininess * 128.0);
   glEndList();

   glNewList(listindex + COLOROFFSET + 3, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT, blue_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, blue_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, blue_shininess * 128.0);
   glEndList();

   glNewList(listindex + COLOROFFSET + 4, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT, cyan_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, cyan_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, cyan_shininess * 128.0);
   glEndList();

   glNewList(listindex + COLOROFFSET + 5, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT, yellow_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, yellow_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, yellow_shininess * 128.0);
   glEndList();

   glNewList(listindex + COLOROFFSET + 6, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT, magenta_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, magenta_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, magenta_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, magenta_shininess * 128.0);
   glEndList();

   glNewList(listindex + COLOROFFSET + 7, GL_COMPILE);
   glMaterialfv(GL_FRONT, GL_AMBIENT, white_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, white_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, white_specular);
   glMaterialf(GL_FRONT, GL_SHININESS, white_shininess * 128.0);
   glEndList();

}


void init(void) 
{
  GLfloat light_color[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {0.0, 1.0, 1.0, 0.0};

  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  angle = 0.0;
  height = 0.0;
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   currentorientation = previousorientation = temporientation = 0;
   makedlist();
  currentcube[1] =  listindex + 1;
  currentcube[0] = listindex + COLOROFFSET + 1;

/*    glEnable(GL_CULL_FACE); */
/*    glCullFace(GL_BACK); */
   glEnable(GL_DEPTH_TEST);

   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glColor3f(1.0, 1.0, 1.0);
}

void display(void)
{
  int currentpiece;
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();            
   xeye = 5 * sin(angle);
   zeye = 5 * cos(angle);
   gluLookAt (xeye, height, zeye, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glPushMatrix();
/*    glTranslatef(-FIGURE.figlength/2, -FIGURE.figdepth/2, -FIGURE.figwidth/2); */

   for (currentpiece = 1; currentpiece < 8; currentpiece++)
     {
       currentorientation = FIGURE.orientation[currentpiece];
       currentcube[1] = listindex + currentpiece;
       currentcube[0] = listindex + COLOROFFSET + currentpiece;
   printf("Index: %i; axis: %f, %f, %f; rotation: %f\n", 
           currentorientation,
           s4[currentorientation].rotvector[0],
           s4[currentorientation].rotvector[1], 
           s4[currentorientation].rotvector[2], 
           s4[currentorientation].degrees);

   glPushMatrix();
   glTranslatef(FIGURE.refcubex[currentpiece], FIGURE.refcubey[currentpiece],
                FIGURE.refcubez[currentpiece]);
   glRotatef(s4[currentorientation].degrees,
             s4[currentorientation].rotvector[0],
             s4[currentorientation].rotvector[1],
             s4[currentorientation].rotvector[2] );
   glCallLists(2, GL_INT, currentcube);
   glPopMatrix();
     }
   glBegin(GL_LINES);
   glVertex3f(0., -5.0, 0.);
   glVertex3f(0., 5.0, 0.);
   glEnd();
   glPopMatrix();
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (-5.0, 5.0, -5.0, 5.0, 0.5, 20.0);
/*    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0); */
   glMatrixMode (GL_MODELVIEW);
}

/* assumes angle in radians,
 * for trig fcns and eventually gluLookAt()
 */
/* ARGSUSED5 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
   case 'l':
     angle -= 0.2;
     if (angle <= -6.28) angle += 6.28;
     glutPostRedisplay();
     break;
   case 'r':
     angle += 0.2;
     if (angle >= 6.28) angle -= 6.28;
     glutPostRedisplay();
     break;
   case'u':
     height += 1.0;
     glutPostRedisplay();
     break;
   case 'd':
     height -= 1.0;
     glutPostRedisplay();
     break;
   case 'c':
     angle = 0.0;
     height = 0.0;
     glutPostRedisplay();
     break;
   case 'n':
     currentfigure++;
     if (currentfigure >= somafigurescount) currentfigure = 0;
     glutPostRedisplay();
     break;
      case 27:
         exit(0);
         break;
   }
}

/* assumes angle in degrees
 * for glRotatef()
 */
/* ARGSUSED5 */
/* void keyboard(unsigned char key, int x, int y) */
/* { */
/*    switch (key) { */
/*    case 'l': */
/*      angle -= 10.0; */
/*      if (angle <= -360.0) angle += 360.0; */
/*      glutPostRedisplay(); */
/*      break; */
/*    case 'r': */
/*      angle += 10.0; */
/*      if (angle >= 360.0) angle -= 360.0; */
/*      glutPostRedisplay(); */
/*      break; */
/*    case'u': */
/*      height += 1.0; */
/*      glutPostRedisplay(); */
/*      break; */
/*    case 'd': */
/*      height -= 1.0; */
/*      glutPostRedisplay(); */
/*      break; */
/*    case 'c': */
/*      angle = 0.0; */
/*      height = 0.0; */
/*      glutPostRedisplay(); */
/*      break; */
/*       case 27: */
/*          exit(0); */
/*          break; */
/*    } */
/* } */

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}


