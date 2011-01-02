/* ejecta.h, Copyright (c) 2010 Kelley Nielsen <shegeek-dev@comcast.net>
 * part of the xtrascreenhacks package
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */


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

#include <X11/Xlib.h>
#include <GL/gl.h> 
#include <GL/glu.h> 

typedef struct {
  GLdouble initpos[3]; /* local xyz coordinates of initial location */
  GLdouble initvelocity[3];
  GLdouble currpos[3];
} ejparticle;
  

typedef struct {
  ejparticle ** ejectum;
  int numejparticles;
  double radius;
  double taccum;
  double tincrement;
  double grav;
  int numsteps;
} ejemitter;

ejemitter * init_ejemitter(int count, double timestep, double gravconstant);

int reset_ejemitter(ejemitter * redux, GLdouble newradius);


Bool spew_ejecta(ejemitter * ejspew);

void delete_ejemitter(ejemitter * doomed);


#endif
