

#include "ejecta.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "yarandom.h"


void reset_ejparticle(ejparticle * redux, double maxradius)
{
  GLdouble r, theta;
  theta = (double)(random() % 314) / 50.;
  r = (double)(random() % 100) / 100. * maxradius;
  redux->dircos[0] = cos(theta);
  redux->dircos[1] = sin(theta);
  redux->initpos[0] = r * redux->dircos[0];
  redux->initpos[1] = 0.;
    redux->initpos[2] = r * redux->dircos[1];
    redux->initvelocity[0] = ((double)(random() % 200) / 100.)* redux->dircos[0];
    redux->initvelocity[1] = ((double)(random() % 200) / 100. + 8.) ;
    redux->initvelocity[2] = ((double)(random() % 200) / 100.)* redux->dircos[1]  ;

  if (random() % 300 == 0)
    {
      redux->initvelocity[0] += 2.;
      redux->initvelocity[1] += 0.5;
      redux->initvelocity[2] += 2.;
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
