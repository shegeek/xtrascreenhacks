/* bclockhousing.c
 *
 * implementation of the solid housing for the berlin clock
 * by Kelley Nielsen
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "bclockhousing.h"
#include "bclocklightbank.h"
#include "bclockconnector.h"

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



static GLint makebankdlist(void)
{
  int iterator;
  GLint locallistindex;

  locallistindex = glGenLists(1);
  glNewList (locallistindex, GL_COMPILE);

  /* outer band */
  glBegin(GL_QUADS);
  for (iterator = 0; iterator < 39; iterator += 2)
    {
      glNormal3fv(lightbanknorms[(iterator / 2) + 1]);
      glVertex3fv(lightbankverts[iterator + 2]);
      glVertex3fv(lightbankverts[iterator + 3]);
      glNormal3fv(lightbanknorms[iterator / 2]);
      glVertex3fv(lightbankverts[iterator + 1]);
      glVertex3fv(lightbankverts[iterator]);
    }
  glEnd();

  /* front face */
  glBegin(GL_POLYGON);
  glNormal3f(0.0, 0., 1.);
  for (iterator = 1; iterator <= 41; iterator += 2)
    {
      glVertex3fv(lightbankverts[iterator]);
    }
  glEnd();

  glEndList();

  handleGLerrors("makebankdlist");
  return (locallistindex);
}

static GLint makeconnectdlist(void)
{
  int iterator, jterator;
  GLint locallistindex;

  locallistindex = glGenLists(1);
  glNewList (locallistindex, GL_COMPILE);

  /* outer band */
  glBegin(GL_QUADS);
   for (iterator = 0; iterator < 39; iterator += 2)
    {
      glNormal3fv(connectnorms[iterator + 1]);
      glVertex3fv(connectverts[iterator + 2]);
      glVertex3fv(connectverts[iterator + 3]);
       glNormal3fv(connectnorms[iterator]);
     glVertex3fv(connectverts[iterator + 1]);
      glVertex3fv(connectverts[iterator]);
    }
  glEnd();

  /* front face */
  glNormal3f(0.0, 0., 1.);
  glBegin(GL_POLYGON);
  for (iterator = 0; iterator < 4; iterator++)
    {
  glVertex3fv(connectcenterquad[iterator]);
    }
    glEnd();

  for (iterator = 0; iterator < 4; iterator++)
    {
  glBegin(GL_TRIANGLE_FAN);
  glVertex3fv(connectcenterquad[iterator]);
  for (jterator = 1; jterator < 10; jterator+=2)
    {
  glVertex3fv(connectverts[iterator * 10 + jterator]);
    }
   glEnd();
    }
  glEndList();

  handleGLerrors("makeconnectdlist");
  return (locallistindex);
}


/* this seems excessive, but
 * scaling the main connector just doesn't look right.
 * the sides don't scoop inward far enough because the curve
 * flattens as it scales.
 */
static GLint makesecconnectdlist(void)
{
  int iterator, jterator;
  GLint locallistindex;

  locallistindex = glGenLists(1);
  glNewList (locallistindex, GL_COMPILE);

  /* outer band */
  glBegin(GL_QUADS);
  for (iterator = 0; iterator < 39; iterator += 2)
    {
      glNormal3fv(connectnorms[iterator + 1]);
      glVertex3fv(secconnectverts[iterator + 2]);
      glVertex3fv(secconnectverts[iterator + 3]);
       glNormal3fv(connectnorms[iterator]);
     glVertex3fv(secconnectverts[iterator + 1]);
      glVertex3fv(secconnectverts[iterator]);
    }
  glEnd();

  /* front face */
  glNormal3f(0.0, 0., 1.);

  glBegin(GL_POLYGON);
  for (iterator = 0; iterator < 4; iterator++)
    {
  glVertex3fv(secconnectcenterquad[iterator]);
    }
    glEnd();
  for (iterator = 0; iterator < 4; iterator++)
    {
  glBegin(GL_TRIANGLE_FAN);
  glVertex3fv(secconnectcenterquad[iterator]);
  for (jterator = 1; jterator < 10; jterator+=2)
    {
  glVertex3fv(secconnectverts[iterator * 10 + jterator]);
    }
  glEnd();
    }

  glEndList();

  handleGLerrors("makesecconnectdlist");
  return (locallistindex);
}

GLint makebclockhousing(void){
  GLuint dlistindex;
GLUquadricObj * pole;
  int iterator;
GLuint banklistindex;
GLuint connectlistindex;
GLuint secconnectlistindex;

GLfloat steel_ambient[] = {0.19225, 0.19225, 0.19225, 1.0};
GLfloat steel_diffuse[] = {0.50754, 0.50754, 0.50754, 1.0};
GLfloat steel_specular[] = {0.508273, 0.508273, 0.508273, 1.0};
GLfloat steel_emission[] = {0.0, 0.0, 0.0, 1.0};
GLfloat steel_shininess = 0.4;

   banklistindex = makebankdlist();
   connectlistindex = makeconnectdlist();
   secconnectlistindex = makesecconnectdlist();

   pole = gluNewQuadric();
   gluQuadricCallback(pole, GLU_ERROR, (_GLUfuncptr)errorCallback);

   gluQuadricDrawStyle(pole, GLU_FILL); 
   gluQuadricNormals(pole, GLU_SMOOTH);
 
  dlistindex = glGenLists(1);
  glNewList(dlistindex, GL_COMPILE);

   glMaterialfv(GL_FRONT, GL_AMBIENT, steel_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, steel_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, steel_specular);
   glMaterialfv(GL_FRONT, GL_EMISSION, steel_emission);
   glMaterialf(GL_FRONT, GL_SHININESS, steel_shininess * 128.0);

   /* draw banks, including seconds */
   glPushMatrix();
   glTranslatef(0.0, -10., 0.);
   for (iterator = 0; iterator < 4; iterator++)
     {
   glCallList(banklistindex);
  glTranslatef(0.0, 5.0, 0.);
     }
   glTranslatef(0.0, 0., -1.0);
   gluCylinder(pole, 2.5, 2.5, 2, 24, 16);
   glTranslatef( 0.0, 0.0, 2.0);
   gluDisk(pole, 0, 2.5, 24, 16);
   glPopMatrix();

   /* draw connectors */
   glPushMatrix();
   glTranslatef(0., -7.5, 0.);
   for (iterator = 0; iterator < 3; iterator++)
     {
   glPushMatrix();
   glTranslatef(-4.5, 0., 0.);
   glCallList(connectlistindex);
   glPopMatrix();
   glPushMatrix();
   glTranslatef(4.5, 0., 0.);
   glCallList(connectlistindex);
   glPopMatrix();
   glTranslatef(0., 5.0, 0.);
}
   glTranslatef(0., -0.15, 0.);
   glCallList(secconnectlistindex);
   glPopMatrix();

   /* draw pole */
   glPushMatrix();
   glRotatef(90.0, 1.0, 0.0, 0.0);
   glTranslatef(0.0, 0.0, 12.0);
   gluCylinder(pole, 0.75, 0.75, 20, 16, 12);
   glPopMatrix();

  glEndList();

  handleGLerrors("makebclockhousing");
  return dlistindex;


}
