/* to use:
 *
 * call init_ejemitter() to initialize
 * call reset_ejemitter() before first use
 *   and before every new impact thereafter
 * call spew_ejecta() once for every frame during ejection
 * call delete_ejemitter() on exit
 */


#ifndef _EJECTA_H_
#define _EJECTA_H_

#include <stdbool.h>
#include <GL/gl.h> 
#include <GL/glu.h> 

typedef struct {
  GLdouble initpos[3]; /* local xyz coordinates of initial location */
  GLdouble dircos[2];  /* direction cosines, used to update trajectory */ 
  GLdouble initvelocity[3];
  GLdouble currpos[3];
} ejparticle;
  

typedef struct {
  ejparticle ** ejectum;
  int numejparticles;
  double radius;
  double tcounter;
  double tincrement;
  double grav;
} ejemitter;

ejemitter * init_ejemitter(int count, double timestep, double gravconstant);

void reset_ejemitter(ejemitter * redux, GLdouble newradius);


bool spew_ejecta(ejemitter * ejspew);

void delete_ejemitter(ejemitter * doomed);


#endif
