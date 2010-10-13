/* bclocklamps.c
 *
 * implementation of the individual lamps for the berlin clock
 * and the display lists containing them
 * by Kelley Nielsen
 */


#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "bclocklamps.h"

static void handleGLerrors(char *guiltyfunction)
{
  GLenum errcode;
  const GLubyte *errstring;

    errcode = glGetError();
    if (GL_NO_ERROR != errcode)
      {
        errstring = gluErrorString(errcode);
        fprintf(stderr, "GL error in %s: %s\n", guiltyfunction, errstring);
      }
}


static void errorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf(stderr, "Quadric Error: %s\n", estring);
   exit(0);
}


GLint makelampdlists(Bool ismono)
{
  GLint mainlistindex;
  int iterator;
  GLint mono_on, mono_off, redon, redoff, yellowon, yellowoff;
  GLint fourbank1, fourbank2, fourbank3, fourbank4;
  GLint elevenbank1, elevenbank2, elevenbank3, elevenbank4;
  GLint elevenbank5, elevenbank6, elevenbank7, elevenbank8;
  GLint elevenbank9, elevenbank10, elevenbank11;
  GLUquadricObj * circlebank;

  GLfloat elevenbankoffset = 0.35;

GLfloat fourbankverts[20][3] = {
{-6.975000, 1.700000, 1.000000}, {-7.491623, 1.597237, 1.000000},
{-7.929594, 1.304594, 1.000000}, {-8.222237, 0.866623, 1.000000}, 
{-8.325000, 0.350000, 1.000000}, {-8.325000, -0.350000, 1.000000},
{-8.222237, -0.866623, 1.000000}, {-7.929594, -1.304594, 1.000000},
{-7.491623, -1.597237, 1.000000}, {-6.975000, -1.700000, 1.000000},
{6.975000, -1.700000, 1.000000}, {7.491623, -1.597237, 1.000000},
{7.929594, -1.304594, 1.000000}, {8.222237, -0.866623, 1.000000},
{8.325000, -0.350000, 1.000000}, {8.325000, 0.350000, 1.000000},
{8.222237, 0.866623, 1.000000}, {7.929594, 1.304594, 1.000000},
{7.491623, 1.597237, 1.000000}, {6.975000, 1.700000, 1.000000}};

GLfloat elevenbankverts[20][3] = {
{-5.650000, 1.700000, 1.000000}, {-6.166623, 1.597237, 1.000000},
{-6.604594, 1.304594, 1.000000}, {-6.897237, 0.866623, 1.000000},
{-7.000000, 0.350000, 1.000000}, {-7.000000, -0.350000, 1.000000},
{-6.897237, -0.866623, 1.000000}, {-6.604594, -1.304594, 1.000000},
{-6.166623, -1.597237, 1.000000}, {-5.650000, -1.700000, 1.000000},
{5.650000, -1.700000, 1.000000}, {6.166623, -1.597237, 1.000000},
{6.604594, -1.304594, 1.000000}, {6.897237, -0.866623, 1.000000},
{7.000000, -0.350000, 1.000000}, {7.000000, 0.350000, 1.000000},
{6.897237, 0.866623, 1.000000}, {6.604594, 1.304594, 1.000000},
{6.166623, 1.597237, 1.000000}, {5.650000, 1.700000, 1.000000}};


   GLfloat mono_on_ambient[]  = {0.0, 0.0, 0.0, 1.0}; 
   GLfloat mono_on_diffuse[]  = {0.0, 0.0, 0.0, 1.0}; 
   GLfloat mono_on_specular[]  = {0., 0., 0., 1.}; 
   GLfloat mono_on_emission[]  = {1.0, 1.0, 1.0, 1.0}; 
   GLfloat mono_on_shininess  = 0.0; 
   GLfloat mono_off_ambient[]  = {0., 0., 0.0, 1.}; 
   GLfloat mono_off_diffuse[]  = {0., 0., 0.0, 1.}; 
   GLfloat mono_off_specular[]  = {0.6, 0.6, 0.6, 1.0}; 
   GLfloat mono_off_emission[]  = {0., 0., 0., 1.}; 
   GLfloat mono_off_shininess = 0.7; 
 
  GLfloat redon_ambient[] = {0.0, 0.0, 0., 1.0};
  GLfloat redon_diffuse[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat redon_specular[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat redon_emission[] = {1.0, 0.0, 0., 1.0};
  GLfloat redon_shininess = 0.0;
  GLfloat redoff_ambient[] = {0.15, 0.05, 0.05, 1.};
  GLfloat redoff_diffuse[] = {0.15, 0.05, 0.05, 1.};
  GLfloat redoff_specular[] = {0.7, 0.7, 0.7, 1.0};
  GLfloat redoff_emission[] = {0., 0., 0., 1.};
  GLfloat redoff_shininess = 0.7;
 
   GLfloat yellowon_ambient[]  = {0.0, 0.0, 0.0, 1.0}; 
   GLfloat yellowon_diffuse[]  = {0.0, 0.0, 0.0, 1.0}; 
   GLfloat yellowon_specular[]  = {0., 0., 0., 1.}; 
   GLfloat yellowon_emission[]  = {1.0, 1.0, 0.0, 1.0}; 
   GLfloat yellowon_shininess  = 0.0; 
   GLfloat yellowoff_ambient[]  = {0.15, 0.15, 0.0, 1.}; 
   GLfloat yellowoff_diffuse[]  = {0.15, 0.15, 0.0, 1.}; 
   GLfloat yellowoff_specular[]  = {0.6, 0.6, 0.2, 1.0}; 
   GLfloat yellowoff_emission[]  = {0., 0., 0., 1.}; 
   GLfloat yellowoff_shininess = 0.7; 
 
  circlebank = gluNewQuadric();
  gluQuadricCallback(circlebank, GLU_ERROR, errorCallback);
   gluQuadricDrawStyle(circlebank, GLU_FILL);
   gluQuadricNormals(circlebank, GLU_SMOOTH);
 

  if (!ismono)
   {
   redon = glGenLists(1);
  glNewList(redon, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT, redon_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, redon_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, redon_specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, redon_emission);
   glMaterialf(GL_FRONT, GL_SHININESS, redon_shininess * 128.0);
   glEndList();
   redoff = glGenLists(1);
  glNewList(redoff, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT, redoff_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, redoff_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, redoff_specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, redoff_emission);
   glMaterialf(GL_FRONT, GL_SHININESS, redoff_shininess * 128.0);
   glEndList();
  yellowon = glGenLists(1);
  glNewList(yellowon, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT, yellowon_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowon_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, yellowon_specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, yellowon_emission);
   glMaterialf(GL_FRONT, GL_SHININESS, yellowon_shininess * 128.0);
   glEndList();
   yellowoff = glGenLists(1);
  glNewList(yellowoff, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT, yellowoff_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowoff_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, yellowoff_specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, yellowoff_emission);
   glMaterialf(GL_FRONT, GL_SHININESS, yellowoff_shininess * 128.0);
   glEndList();
   }
   else
   {
  mono_on = glGenLists(1);
  glNewList(mono_on, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mono_on_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mono_on_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mono_on_specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, mono_on_emission);
   glMaterialf(GL_FRONT, GL_SHININESS, mono_on_shininess * 128.0);
   glEndList();
   mono_off = glGenLists(1);
  glNewList(mono_off, GL_COMPILE);
  glMaterialfv(GL_FRONT, GL_AMBIENT, mono_off_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mono_off_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mono_off_specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, mono_off_emission);
   glMaterialf(GL_FRONT, GL_SHININESS, mono_off_shininess * 128.0);
   glEndList();
  redon = yellowon = mono_on;
  redoff = yellowoff = mono_off;
   }

   fourbank1 = glGenLists(1);
   glNewList(fourbank1, GL_COMPILE);
   glPushMatrix();
  glTranslatef(-0.6, 0.0, 0.0);
   glBegin(GL_POLYGON);
 glVertex3f(-FOURBANKWIDTH + 0.2, LIGHTBANKHEIGHT, 1.);
  for (iterator = 0; iterator < 10; iterator ++)
    {
      glVertex3fv(fourbankverts[iterator]);
    }
  glVertex3f(-FOURBANKWIDTH + 0.2, -LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

   fourbank2 = glGenLists(1);
   glNewList(fourbank2, GL_COMPILE);

   glPushMatrix();
   glTranslatef(-0.2, 0.0, 0.0);

  glBegin(GL_QUADS);
  glVertex3f(-FOURBANKWIDTH + 0.2, LIGHTBANKHEIGHT, 1.);
  glVertex3f(-FOURBANKWIDTH + 0.2, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(0., -LIGHTBANKHEIGHT, 1.);
  glVertex3f(0., LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();


   fourbank3 = glGenLists(1);
  glNewList(fourbank3, GL_COMPILE);
  glPushMatrix();
  glTranslatef(0.2, 0.0, 0.0);

  glBegin(GL_QUADS);
  glVertex3f(0., LIGHTBANKHEIGHT, 1.);
  glVertex3f(0., -LIGHTBANKHEIGHT, 1.);
  glVertex3f(FOURBANKWIDTH - 0.2, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(FOURBANKWIDTH - 0.2, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList(); 

   fourbank4 = glGenLists(1);
  glNewList(fourbank4, GL_COMPILE);
  glPushMatrix();
  glTranslatef(0.6, 0.0, 0.0);
   glBegin(GL_POLYGON);
 glVertex3f(FOURBANKWIDTH - 0.2, -LIGHTBANKHEIGHT, 1.);
  for (iterator = 10; iterator < 20; iterator ++)
    {
      glVertex3fv(fourbankverts[iterator]);
    }
  glVertex3f(FOURBANKWIDTH - 0.2, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList(); 

   elevenbank1 = glGenLists(1);
   glNewList(elevenbank1, GL_COMPILE);
   glPushMatrix();
  glTranslatef((-elevenbankoffset * 5) - 0.15, 0.0, 0.0);
   glBegin(GL_POLYGON);
 glVertex3f((-ELEVENBANKWIDTH * 9) + 0.1, LIGHTBANKHEIGHT, 1.);
  for (iterator = 0; iterator < 10; iterator ++)
    {
      glVertex3fv(elevenbankverts[iterator]);
    }
  glVertex3f((-ELEVENBANKWIDTH * 9) + 0.1, -LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank2 = glGenLists(1);
  glNewList(elevenbank2, GL_COMPILE);
  glPushMatrix();
  glTranslatef(-elevenbankoffset * 4, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(-ELEVENBANKWIDTH * 9, LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 9, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 7, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 7, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank3 = glGenLists(1);
  glNewList(elevenbank3, GL_COMPILE);
  glPushMatrix();
  glTranslatef(-elevenbankoffset * 3, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(-ELEVENBANKWIDTH * 7, LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 7, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 5, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 5, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank4 = glGenLists(1);
  glNewList(elevenbank4, GL_COMPILE);
  glPushMatrix();
   glTranslatef(-elevenbankoffset * 2, 0.0, 0.0);
 glBegin(GL_QUADS);
  glVertex3f(-ELEVENBANKWIDTH * 5, LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 5, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 3, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 3, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank5 = glGenLists(1);
  glNewList(elevenbank5, GL_COMPILE);
  glPushMatrix();
  glTranslatef(-elevenbankoffset, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(-ELEVENBANKWIDTH * 3, LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH * 3, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank6 = glGenLists(1);
  glNewList(elevenbank6, GL_COMPILE);
  glPushMatrix();
  glBegin(GL_QUADS);
  glVertex3f(-ELEVENBANKWIDTH, LIGHTBANKHEIGHT, 1.);
  glVertex3f(-ELEVENBANKWIDTH, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank7 = glGenLists(1);
  glNewList(elevenbank7, GL_COMPILE);
  glPushMatrix();
  glTranslatef(elevenbankoffset, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(ELEVENBANKWIDTH, LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 3, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 3, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank8 = glGenLists(1);
  glNewList(elevenbank8, GL_COMPILE);
  glPushMatrix();
  glTranslatef(elevenbankoffset * 2, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(ELEVENBANKWIDTH * 3, LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 3, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 5, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 5, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank9 = glGenLists(1);
  glNewList(elevenbank9, GL_COMPILE);
  glPushMatrix();
  glTranslatef(elevenbankoffset * 3, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(ELEVENBANKWIDTH * 5, LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 5, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 7, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 7, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

  elevenbank10 = glGenLists(1);
  glNewList(elevenbank10, GL_COMPILE);
  glPushMatrix();
  glTranslatef(elevenbankoffset * 4, 0.0, 0.0);
  glBegin(GL_QUADS);
  glVertex3f(ELEVENBANKWIDTH * 7, LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 7, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 9, -LIGHTBANKHEIGHT, 1.);
  glVertex3f(ELEVENBANKWIDTH * 9, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList();

   elevenbank11 = glGenLists(1);
  glNewList(elevenbank11, GL_COMPILE);
  glPushMatrix();
  glTranslatef((elevenbankoffset * 5) + 0.15, 0.0, 0.0);
   glBegin(GL_POLYGON);
 glVertex3f((ELEVENBANKWIDTH * 9) - 0.1, -LIGHTBANKHEIGHT, 1.);
  for (iterator = 10; iterator < 20; iterator ++)
    {
      glVertex3fv(elevenbankverts[iterator]);
    }
  glVertex3f((ELEVENBANKWIDTH * 9) - 0.1, LIGHTBANKHEIGHT, 1.);
  glEnd();
  glPopMatrix();
  glEndList(); 

/* ----- end setup lists, begin actual time-referenced banks to be used in display function */

    mainlistindex = glGenLists(52);

/* single hour banks (second row) */

  glNewList (mainlistindex + HR1OFFSET + 0, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glCallList(redoff);
  glCallList(fourbank1);
  glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();


  glNewList (mainlistindex + HR1OFFSET + 1, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glCallList(redon);
  glCallList(fourbank1);
  glCallList(redoff);
  glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();


  glNewList (mainlistindex + HR1OFFSET + 2, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glCallList(redon);
  glCallList(fourbank1);
  glCallList(fourbank2);
  glCallList(redoff);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();


  glNewList (mainlistindex + HR1OFFSET + 3, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glCallList(redon);
  glCallList(fourbank1);
  glCallList(fourbank2);
   glCallList(fourbank3);
  glCallList(redoff);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();

  glNewList (mainlistindex + HR1OFFSET + 4, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glCallList(redon);
  glCallList(fourbank1);
 glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();



/* ----- five hour banks (top row)---------------------- */

  glNewList (mainlistindex + HR5OFFSET + 0, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., 5., 0.);
  glCallList(redoff);
  glCallList(fourbank1);
  glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
glPopMatrix();
  glEndList();


  glNewList (mainlistindex + HR5OFFSET + 1, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., 5., 0.);
  glCallList(redon);
  glCallList(fourbank1);
  glCallList(redoff);
  glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();


  glNewList (mainlistindex + HR5OFFSET + 2, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., 5., 0.);
  glCallList(redon);
  glCallList(fourbank1);
  glCallList(fourbank2);
  glCallList(redoff);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();


  glNewList (mainlistindex + HR5OFFSET + 3, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., 5., 0.);
  glCallList(redon);
  glCallList(fourbank1);
  glCallList(fourbank2);
   glCallList(fourbank3);
  glCallList(redoff);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();


  glNewList (mainlistindex + HR5OFFSET + 4, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., 5., 0.);
  glCallList(redon);
  glCallList(fourbank1);
  glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();


/* ------- single minute banks (bottom row)------------------------------ */

  glNewList (mainlistindex + MIN1OFFSET + 0, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., -10., 0.);
  glCallList(yellowoff);
  glCallList(fourbank1);
  glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();

  glNewList (mainlistindex + MIN1OFFSET + 1, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., -10., 0.);
  glCallList(yellowon);
  glCallList(fourbank1);
  glCallList(yellowoff);
  glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();

  glNewList (mainlistindex + MIN1OFFSET + 2, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., -10., 0.);
  glCallList(yellowon);
  glCallList(fourbank1);
  glCallList(fourbank2);
  glCallList(yellowoff);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();

  glNewList (mainlistindex + MIN1OFFSET + 3, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., -10., 0.);
  glCallList(yellowon);
  glCallList(fourbank1);
  glCallList(fourbank2);
   glCallList(fourbank3);
  glCallList(yellowoff);
 glCallList(fourbank4);
  glPopMatrix();
  glEndList();

  glNewList (mainlistindex + MIN1OFFSET + 4, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
 glPushMatrix();
  glTranslatef(0., -10., 0.);
  glCallList(yellowon);
  glCallList(fourbank1);
  glCallList(fourbank2);
   glCallList(fourbank3);
 glCallList(fourbank4);
 glPopMatrix();
  glEndList();

/* -----five minute banks (third row) ----------------------- */

  glNewList(mainlistindex + MIN5OFFSET + 0, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowoff);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redoff);
    glCallList(elevenbank3);
    glCallList(elevenbank6);
    glCallList(elevenbank9);
   glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 1, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
    glCallList(yellowoff);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redoff);
    glCallList(elevenbank3);
    glCallList(elevenbank6);
    glCallList(elevenbank9);
    glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 2, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
    glCallList(yellowoff);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redoff);
    glCallList(elevenbank3);
    glCallList(elevenbank6);
    glCallList(elevenbank9);
    glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 3, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
   glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
    glCallList(yellowoff);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
     glCallList(redoff);
   glCallList(elevenbank6);
    glCallList(elevenbank9);
    glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 4, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
    glCallList(yellowoff);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
     glCallList(redoff);
   glCallList(elevenbank6);
    glCallList(elevenbank9);
    glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 5, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
      glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
   glCallList(yellowoff);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
     glCallList(redoff);
   glCallList(elevenbank6);
    glCallList(elevenbank9);
   glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 6, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
    glCallList(yellowoff);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
   glCallList(elevenbank6);
     glCallList(redoff);
    glCallList(elevenbank9);
    glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 7, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
    glCallList(yellowoff);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
   glCallList(elevenbank6);
     glCallList(redoff);
    glCallList(elevenbank9);
    glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 8, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
    glCallList(yellowoff);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
   glCallList(elevenbank6);
     glCallList(redoff);
    glCallList(elevenbank9);
    glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 9, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
    glCallList(yellowoff);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
   glCallList(elevenbank6);
    glCallList(elevenbank9);
   glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 10, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
    glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
    glCallList(yellowoff);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
   glCallList(elevenbank6);
    glCallList(elevenbank9);
   glPopMatrix();
    glEndList();

  glNewList(mainlistindex + MIN5OFFSET + 11, GL_COMPILE);
    glNormal3f(0.0, 0.0, 1.0);
   glPushMatrix();
    glTranslatef(0., -5., 0.);
    glCallList(yellowon);
    glCallList(elevenbank1);
     glCallList(elevenbank2);
     glCallList(elevenbank4);
     glCallList(elevenbank5);
     glCallList(elevenbank7);
     glCallList(elevenbank8);
     glCallList(elevenbank10);
     glCallList(elevenbank11);
    glCallList(redon);
    glCallList(elevenbank3);
   glCallList(elevenbank6);
    glCallList(elevenbank9);
    glPopMatrix();
    glEndList();

/* -----seconds banks (topknot) ------------------------------- */

  glNewList(mainlistindex + SECOFFSET + 0, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
 glPushMatrix();
  glTranslatef(0., 10., 1.);
  glCallList(yellowoff);
  gluDisk(circlebank, 0, 2.2, 24, 16);
 glPopMatrix();
  glEndList();  

  glNewList(mainlistindex + SECOFFSET + 1, GL_COMPILE);
  glNormal3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0., 10., 1.);
  glCallList(yellowon);
  gluDisk(circlebank, 0, 2.2, 24, 16);
 glPopMatrix();
  glEndList();  

  handleGLerrors("makelampdlists");

  return (mainlistindex);

}



