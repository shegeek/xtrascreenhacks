

#include "ejecta.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "yarandom.h"


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
    redux->initvelocity[0] =/*  ((double)(random() % 20) / 100.)* */ xcomp * 3 * (maxradius - r);
/*     redux->initvelocity[1] = ((double)(random() % 200) / 100. + 5.) ; */
    redux->initvelocity[1] = exp((maxradius - r) + 1) * ((double)(random() % 130) / 100.);
    redux->initvelocity[2] = /* ((double)(random() % 20) / 100.)*  */ zcomp * 3 *(maxradius - r);

    if ((random() % 20 == 0) && (r < maxradius * 0.2))
    {
      redux->initvelocity[0] += 2. * xcomp;
      redux->initvelocity[1] += 0.5;
      redux->initvelocity[2] += 2. * zcomp;
    }
} 


bool advance_ejparticle(ejparticle * updated, double elapsed, double gconst)
{
  bool islive = true;
  updated->currpos[0] = updated->initvelocity[0] * elapsed 
    + updated->initpos[0];
  updated->currpos[1] = (gconst * elapsed * elapsed) + 
    updated->initvelocity[1] * elapsed + updated->initpos[1];
  if (updated->currpos[1] < 0.) islive = false;
  updated->currpos[2] = updated->initvelocity[2] * elapsed + updated->initpos[2];
  return islive;
}

void reset_ejemitter(ejemitter * redux, GLdouble newradius)
{
  int itor;
  redux->tcounter = 0;
  redux->radius = newradius;
  for (itor = 0; itor < redux->numejparticles; itor++)
    {
      reset_ejparticle(redux->ejectum[itor], newradius);
    }
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
 * the solids can't be casting the entire ground in shadow
 * at a scalefactor of 0.05, the solids look just like the points
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

bool spew_ejecta(ejemitter * ejspew)
{
  bool emitterislive = false;
  bool particleislive;
  int itor;
  ejspew->tcounter += ejspew->tincrement;
  for (itor = 0; itor < ejspew->numejparticles; itor++)
    {
      particleislive = advance_ejparticle(ejspew->ejectum[itor], 
					  ejspew->tcounter, ejspew->grav);
      if (particleislive) emitterislive = true;
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

/* left to do:
 *
 * at the beginning of the eruption, it should look more like
 * the ground is swelling and bursting, and less like a mushroom cloud
 * translating to slightly below ground level and starting from there helps
 *
 * decide how to send message to caller that it's safe to draw mesh
 *
 * possibly put in another flag so that 
 *  emitter won't run if not properly reset
 *
 * add error checking and other correct usage constraints, as mentioned above
 *
 * points blend into ground when they are of the same material
 * make them stand out, and possibly add shadows
 */


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
