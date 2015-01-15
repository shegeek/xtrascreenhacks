/* plasmamap.c - Copyright (c) 2011 Kelley Nielsen <shegeek-dev@comcast.net>
 * 
 * Creates a plasma fractal height map, using the Diamond Square algorithm
 * as described by Paul Martz at http://www.gameprogrammer.com/fractal.html
 * It is intended to be used by the Psychedelic screen hack.
 * The idea came from Patrick Hahn, whose Fractint version can be found at
 * http://www2.vo.lu/homepages/phahn/fractals/cycling.htm
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "plasmamap.h"
#include "yarandom.h"

#define MIN(x, y)       ((x) < (y))?(x):(y)
#define MAX(x, y)       ((x) > (y))?(x):(y)
#define MID(x, y) ((x) + (y)) / 2
#define AVG(x, y, q, r)  (((x) + (y) + (q) + (r))  / 4)
#define RANDOMIZE(x) (((float)(random() % 200) - 100 ) * (x) )

/* for the points marking the corners of diamonds--
 * top, left, bottom, right
 */
#define T 0
#define L 1
#define B 2
#define R 3

/* for the points marking the corners of squares--
 * top left, bottom left, bottom right, top right
 */
#define TL 0
#define BL 1
#define BR 2
#define TR 3

static int gpow2(int ref)
{
  int base = 2;
  while (base + 1 < ref)
    {
      base = base * 2;
    }
  return base + 1;
}

static int diamondsquare(int * map, int dim, float M, int lowval)
{
  long cornerindices[4];
  int cornervals[4];
  long diamondindex;
/*   int diamondval; */
  long squareindices[4];
/*   int squarevals[4] */
  long dcornerindices[4];
  int divisions;
  int span;
  int wloop, hloop;
  float m = M;
  int lowestval = lowval;
  for (divisions = 1; divisions < dim - 1; divisions *= 2)
    {
      span = (dim - 1) / divisions;
      for (hloop = 0; hloop < divisions; hloop++)
	{
	  for (wloop = 0; wloop < divisions; wloop++)
	    {
	      cornerindices[TL] = (hloop * dim + wloop) * span;
	      cornerindices[BL] = ((hloop + 1) * dim + wloop) * span;
	      cornerindices[BR] = cornerindices[BL] + span;
	      cornerindices[TR] = cornerindices[TL] + span;
	      cornervals[TL] = map[cornerindices[TL]];
	      cornervals[BL] = map[cornerindices[BL]];
	      cornervals[BR] = map[cornerindices[BR]];
	      cornervals[TR] = map[cornerindices[TR]];
	      diamondindex = MID(cornerindices[TL], cornerindices[BR]);
	      map[diamondindex] = AVG(cornervals[TL], cornervals[TR], cornervals[BL], 
				      cornervals[BR]) + RANDOMIZE(m);
	      if (map[diamondindex] < lowestval) lowestval = map[diamondindex];
	    }
	}
      for (hloop = 0; hloop < divisions; hloop++)
	{
	  for (wloop = 0; wloop < divisions; wloop++)
	    {
	      cornerindices[TL] = (hloop * dim + wloop) * span;
	      cornerindices[BL] = ((hloop + 1) * dim + wloop) * span;
	      cornerindices[BR] = cornerindices[BL] + span;
	      cornerindices[TR] = cornerindices[TL] + span;
	      diamondindex = MID(cornerindices[TL], cornerindices[BR]);

	      if (hloop == 0) dcornerindices[T] = diamondindex +((divisions - 1) * dim * span);
	      else dcornerindices[T] = diamondindex - dim * span;
	      dcornerindices[L] = diamondindex - span;
	      if (wloop == 0) dcornerindices[L] += dim - 1;
	      if (hloop == divisions - 1) dcornerindices[B] =  diamondindex -
		((divisions - 1) * dim * span);
	      else dcornerindices[B] = diamondindex + dim * span;; 
	      dcornerindices[R] = diamondindex + span;
	      if (wloop == divisions - 1) dcornerindices[R] -= dim - 1;

	      squareindices[T] = MID(cornerindices[TL], cornerindices[TR]);
	      squareindices[L] = MID(cornerindices[TL], cornerindices[BL]);
	      squareindices[B] = MID(cornerindices[BL], cornerindices[BR]);
	      squareindices[R] = MID(cornerindices[TR], cornerindices[BR]);

	      map[squareindices[T]] = AVG(map[dcornerindices[T]], map[cornerindices[TL]], 
				  map[diamondindex], map[cornerindices[TR]])
		                                  + RANDOMIZE(m);
	      if (map[squareindices[T]] < lowestval) lowestval = map[squareindices[T]];
	      map[squareindices[L]] = AVG(map[cornerindices[TL]], map[dcornerindices[L]], 
				  map[cornerindices[BL]], map[diamondindex])
		                                 + RANDOMIZE(m);
	      if (map[squareindices[L]] < lowestval) lowestval = map[squareindices[L]];
	      map[squareindices[B]] = AVG(map[diamondindex], map[cornerindices[BL]], 
				 map[dcornerindices[B]], map[cornerindices[BR]])
		                                + RANDOMIZE(m);
	      if (map[squareindices[B]] < lowestval) lowestval = map[squareindices[B]];
	      map[squareindices[R]] = AVG(map[cornerindices[TR]], map[diamondindex], 
				  map[cornerindices[BR]], map[dcornerindices[R]])
		                                 + RANDOMIZE(m);
	      if (map[squareindices[R]] < lowestval) lowestval = map[squareindices[R]];
	    }
	}
      m *= 0.5;
    }
  return lowestval;
}


void plasmaImage(int * plasmamap, int plasmawidth, int plasmaheight, int ncolors)
{
  int * dsmap;
  int dimension;
  int itor, jtor;
  int lowestval;
  dimension = gpow2(MAX(plasmawidth, plasmaheight));
  dsmap = malloc(dimension * dimension * sizeof(int));
  if (dsmap == NULL)
    {
        fprintf(stderr, "Out of memory, exiting\n");
        exit(1);
    }
  memset(dsmap, 0, dimension * dimension * sizeof(int));

  dsmap[0] = 50;
  dsmap[dimension - 1] = 50;
  dsmap[dimension * (dimension - 1)] = 50;
  dsmap[dimension * dimension -1] = 50;
  lowestval = 50;
  lowestval = diamondsquare(dsmap, dimension, 0.5, lowestval);
  for (itor = 0; itor < plasmaheight; itor++)
    {
      for (jtor = 0; jtor < plasmawidth; jtor++)
	{
	  plasmamap[itor * plasmawidth + jtor] = (dsmap[itor * dimension + jtor] - lowestval) 
	    % ncolors;
	}
    }
  free(dsmap);
}


/* decide what to do about pixels being assigned twice -> leave in--looks fine, code is simpler */

/* pare down variables and assignments to what's truly necessary while keeping things clear */
/* clear up confusion between diamonds and squares */
/* arrange assignments, calls to AVG(), etc so that everything goes counterclockwise */
