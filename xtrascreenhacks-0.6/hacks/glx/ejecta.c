/*ejecta.c, Copyright (c) 2010 Kelley Nielsen <shegeek-dev@comcast.net>
 * part of the xtrascreenhacks package
 *
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */


#include "ejecta.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "yarandom.h"

#define EJECTSPREAD 100

static void
handleGLerrors (char *guiltyfunction)
{
  GLenum errcode;
  const GLubyte *errstring;

  errcode = glGetError ();
  if (GL_NO_ERROR != errcode)
    {
      errstring = gluErrorString (errcode);
      fprintf (stderr, "GL error in %s: %s\n", guiltyfunction, errstring);
    }
}

void reset_ejparticle(ejparticle * redux, double maxradius)
{
  double r, theta;
  double xcomp, zcomp;
  theta = (double)(random() % 314) / 50.;
  r = (double)(random() % 100) / 100. * maxradius;
  xcomp = cos(theta);
  zcomp = sin(theta);
  redux->initpos[0] = r * xcomp;
  redux->initpos[1] = 0.;
    redux->initpos[2] = r * zcomp;
    redux->initvelocity[0] = xcomp * 3 * (maxradius - r);
    redux->initvelocity[1] = exp((maxradius - r) + 1) * 
      ((double)(random() % EJECTSPREAD) / 100.);
    redux->initvelocity[2] = zcomp * 3 *(maxradius - r);

    /* wildly scatter a few particles */
    if ((random() % 20 == 0) && (r < maxradius * 0.2))
    {
      redux->initvelocity[0] += 2. * xcomp;
      redux->initvelocity[1] += 0.5;
      redux->initvelocity[2] += 2. * zcomp;
    }
} 



Bool advance_ejparticle(ejparticle * updated, double elapsed, double gconst)
{
  Bool islive = True;
  updated->currpos[0] = updated->initvelocity[0] * elapsed 
    + updated->initpos[0];
  updated->currpos[1] = (gconst * elapsed * elapsed) + 
    updated->initvelocity[1] * elapsed + updated->initpos[1];
  if (updated->currpos[1] < 0.) islive = False;
  updated->currpos[2] = updated->initvelocity[2] * elapsed + updated->initpos[2];
  return islive;
}

int reset_ejemitter(ejemitter * redux, GLdouble newradius)
{
    double maxvnought;
    double t2maxh;
  int itor;
  redux->taccum = 0;
  redux->radius = newradius;
  for (itor = 0; itor < redux->numejparticles; itor++)
    {
      reset_ejparticle(redux->ejectum[itor], newradius);
    }
  maxvnought = (exp(newradius + 1.) * EJECTSPREAD / 100.);
    t2maxh = maxvnought / -redux->grav;
    redux->numsteps = t2maxh / redux->tincrement;
  return redux->numsteps;
}

ejemitter * init_ejemitter(int count, double timestep, double gravconstant)
{
  int itor;
  ejemitter * newemitter;

  newemitter = malloc(sizeof(ejemitter));
  newemitter->ejectum = calloc (count, sizeof(ejparticle *));
  for (itor = 0; itor < count; itor++)
    {
      newemitter->ejectum[itor] = malloc(sizeof(ejparticle));
    }
  newemitter->numejparticles = count;
  newemitter->tincrement = timestep;
  newemitter->grav = -0.5 * gravconstant;
  return newemitter;
}  


/* the first of these uses points; the second, little solids
 * the first one runs faster, of course
 * the ground is darker with the second, not yet sure why 
 * at a scalefactor of 0.05, the solids look just like the points
 * WARNING: the one with solids uses a glut primitive
 */
void render_ejparticles(ejparticle ** ejectoids, int numejectoids)
{
  int iterator;
  glBegin(GL_POINTS);
  for (iterator = 0; iterator < numejectoids; iterator++)
    {
    glVertex3f(ejectoids[iterator]->currpos[0],
	       ejectoids[iterator]->currpos[1],
	       ejectoids[iterator]->currpos[2]); 
    }
  glEnd();
  handleGLerrors("ejecta: render_ejparticles");
}


/* void render_ejparticles(ejparticle ** ejectoids, int numejectoids) */
/* { */
/*   int iterator; */
/*   for (iterator = 0; iterator < numejectoids; iterator++) */
/*     { */
/*       glPushMatrix(); */
/*     glTranslatef(ejectoids[iterator]->currpos[0], */
/* 	       ejectoids[iterator]->currpos[1], */
/* 	       ejectoids[iterator]->currpos[2]);  */
/*     glScalef(0.1, 0.1, 0.1); */
/*     glutSolidOctahedron(); */
/*     glPopMatrix(); */
/*     } */
/* } */

Bool spew_ejecta(ejemitter * ejspew)
{
  Bool emitterislive = False;
  Bool particleislive;
  int itor;
  ejspew->taccum += ejspew->tincrement;
  for (itor = 0; itor < ejspew->numejparticles; itor++)
    {
      particleislive = advance_ejparticle(ejspew->ejectum[itor], 
					  ejspew->taccum, ejspew->grav);
      if (particleislive) emitterislive = True;
    }
  render_ejparticles(ejspew->ejectum, ejspew->numejparticles);
  return emitterislive;
}


void delete_ejemitter(ejemitter * doomed)
{
  int itor;
  for (itor = 0; itor < doomed->numejparticles; itor++)
    {
      free (doomed->ejectum[itor]);
    }
  free(doomed->ejectum);
  free(doomed);
  doomed = NULL;
}


/* =================================== */

/* points comprising crater outline, ordered inward to outward:
 *
 *
v7 0.000145 -0.002416 0.000034
v6 0.156413 -0.002416 0.007709
v5 0.208796 -0.002416 0.023462
v4 0.271345 -0.002416 0.057688
v3 0.355245 -0.002416 0.179949
v2 0.427474 -0.002416 0.115025
v8 0.538759 -0.002416 0.061143
v9 0.688434 -0.002416 0.000061
v1 0.699854 -0.002416 0.000174
*
* v9x / v3x = 1.938 (ratio of outside of berm to top of lip)
*/
