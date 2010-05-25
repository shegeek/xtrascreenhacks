/* daisy.c--by shegeek (Kelley Nielsen)
 * Copyright (c) 2006 by Kelley Nielsen <shegeek@cruzers.com>
 *
 * based on and steals code from deluxe.c by:

 * xscreensaver, Copyright (c) 1999 Jamie Zawinski <jwz@jwz.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

/*-------------------------------------------------------------*/

#include <math.h>
#include "screenhack.h"
#include "erase.h"

#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
# include "xdbe.h"
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */

#define THETASTEP 0.02
#define THETASTEPS 314


enum displaymode {POP, DROP, ROAM};

struct state {
  Display * dpy;
  Window window;

  int count;
  int daisiesactive;
  unsigned long drawdelay, erasedelay, delay;
  int ncolors;
  XColor *colors;
  int duration;
  int uptime;
  Bool dbuf;
  enum displaymode mode;

  XWindowAttributes xgwa;
  GC erase_gc;
  eraser_state * eraser;

  Pixmap b, ba, bb;
# ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  Bool dbeclear_p;
  XdbeBackBuffer backb;
# endif /* HAVE_DOUBLE_BUFFER_EXTENSION */


  struct daisy **daisies;
};


struct daisy
{
  XPoint outline[314];
  double radii[314];
  XPoint locus;
  GC dgc;
  unsigned int numturns;
  unsigned int radius;
  unsigned int numpetals;
  unsigned int petal_color;
  unsigned int center_color;
  unsigned int drop_increment;
  double spin_increment;
  XPoint roam_increment;
};

static void
calculate_outline(struct daisy *d)
{
  double theta = 0.0;
  double thetaprime;
  int i;
      for (i = 0; i < THETASTEPS; i++)
	{
	  thetaprime = theta + (d->spin_increment * d->numturns);
	  if (thetaprime > 6.28)
	    thetaprime -= 6.28;
	  d->outline[i].x = d->locus.x + (d->radii[i] * cos (thetaprime));
	  d->outline[i].y = d->locus.y + (d->radii[i] * sin (thetaprime));
	  theta += THETASTEP;
	}

  }


/* "constructor" ----------------------------------------------*/

static struct daisy *
new_daisy (struct state *st, Display * display, Drawable window, int mode,
	   int width, int height)
{
  int i;
  struct daisy *newd;
  XGCValues gcv;
  unsigned long flags;
  float theta = 0.0;

  int nvals[] = { 2, 4, 6, 8, 10 };	/* controls number of petals */
  /*even values of n > 10 have too many petals and look crammed */
  int num_nvals = sizeof(nvals) / sizeof(int);;

  newd = (struct daisy *) malloc (sizeof (struct daisy));

  newd->numturns = 0;
  /*radii range from 30 to 200 */
  newd->radius = (random () % 170) + 30;
  /* actual number of petals is 2 * numpetals, of course */
  newd->numpetals = nvals[random () % num_nvals];
  newd->locus.x = random () % width;
  /* if drop code is active, daisies start off the top of the screen */
  if (DROP == mode)
    {
      newd->locus.y = 0 - newd->radius;
    }
  else
    {
      newd->locus.y = random () % height;
    }
  if (ROAM == mode)
    {
      while (((short) (newd->locus.x + newd->radius) < 0) ||
	     ((short) (newd->locus.x - newd->radius) > width))
	{
	  newd->locus.x = random () % width;
	}
      while (((short) (newd->locus.y + newd->radius) < 0) ||
	     ((short) (newd->locus.y - newd->radius) > height))
	{
	  newd->locus.y = random () % height;
	}
    }

  /*set drop and spin increments */
  newd->drop_increment = (random () % 5) + 2;
  newd->spin_increment = ((random () % 5) + 1) * .02;
  if ((random () % 2) == 0)
    newd->spin_increment *= -1;
  newd->roam_increment.x = (random () % 5) + 1;
  if ((random () % 2) == 0)
    newd->roam_increment.x *= -1;
  newd->roam_increment.y = (random () % 5) + 1;
  if ((random () % 2) == 0)
    newd->roam_increment.y *= -1;

       newd->petal_color = st->colors[random () % st->ncolors].pixel;
      newd->center_color = st->colors[random () % st->ncolors].pixel;
		  /* make sure centers stand out against petals */
      while ((newd->petal_color == newd->center_color) && !(st->ncolors < 2))
	{
	  newd->center_color = st->colors[random () % st->ncolors].pixel;
	}


  flags = GCForeground;
  gcv.foreground = newd->petal_color;
  newd->dgc = XCreateGC (display, window, flags, &gcv);

  for (i = 0; i < THETASTEPS; i++)
    {
      newd->radii[i] = newd->radius * cos (newd->numpetals * theta);
      theta += THETASTEP;
    }			

  calculate_outline(newd);

  return newd;
}


/* drawing utility function -----------------------------------*/

static void
draw_one_daisy (Drawable win, Display * display, struct daisy *d)
{

  XSetForeground (display, d->dgc, d->petal_color);
  XFillPolygon (display, win, d->dgc, d->outline, 314,
		Complex, CoordModeOrigin);

  XSetForeground (display, d->dgc, d->center_color);
  XFillArc (display, win, d->dgc, d->locus.x - (d->radius / 4),
	    d->locus.y - (d->radius / 4), d->radius / 2, d->radius / 2, 0,
	    360 * 64);
}				/* draw_one_daisy() */


static Bool
is_screenchangetime (struct state *teststate)
{
  if (teststate->mode != POP) return 0;
  teststate->uptime++;
  return (teststate->uptime == teststate->duration);

    }

static void
clear_daisy_array (void *closure)
{
   struct state *st = (struct state *) closure;
  int i;
  for (i = 0; i < st->count; i++)
    {
      free (st->daisies[i]);
	  }
}

static void
fill_daisy_array (struct state *newstate)
{
  int i;
  for (i = 0; i < newstate->count; i++)
    {
    newstate->daisies[i] = 
		    new_daisy (newstate, newstate->dpy, newstate->window, 
		  newstate->mode, newstate->xgwa.width, newstate->xgwa.height);
	  }
}

/* animator --------------------------------------------------*/
static int
animate (Drawable window, Display * display, int mode, int width, int height,
	 struct daisy *d)
{

  int returncode = 0;

  /*has daisy dropped off bottom of screen? */
  if ((((short) (height + d->radius) - d->locus.y) < 0) && (DROP == mode))
    {
      free (d);
      returncode = -1;
    }
  else
    {
      if (DROP == mode)
	{
	  d->locus.y += d->drop_increment;
	}
      if (ROAM == mode)
	{
	  if (((short) (d->locus.x + d->radius) < 0) ||
	      ((short) (d->locus.x - d->radius) > width))
	    {
	      d->roam_increment.x *= -1;
	    }
	  if (((short) (d->locus.y + d->radius) < 0) ||
	      ((short) (d->locus.y - d->radius) > height))
	    {
	      d->roam_increment.y *= -1;
	    }
	  d->locus.x += d->roam_increment.x;
	  d->locus.y += d->roam_increment.y;
	}

      calculate_outline (d);
      if (++(d->numturns) >= 628)
	d->numturns -= 628;


      draw_one_daisy (window, display, d);
    }
  return returncode;
}

static struct state *
fetch_resources (struct state *inputstate)
{
  struct state * istate = inputstate;
  Bool pop, drop, roam;

  istate->count = get_integer_resource (istate->dpy, "count", "Integer");
    if (istate->count < 1 || 1000 < istate->count ) istate->count = 25;
  istate->delay = get_integer_resource (istate->dpy, "delay", "Integer");
     if (istate->delay < 0 || 1000000 < istate->delay ) istate->delay = 30000;
 istate->duration = get_integer_resource (istate->dpy, "duration", "Integer");
    if (istate->duration < 0 || 1000 < istate->duration ) istate->duration = 20;
  istate->ncolors = get_integer_resource (istate->dpy, "ncolors", "Integer");
    if (istate->ncolors < 1 || 16383 < istate->ncolors ) istate->ncolors = 64;
  istate->dbuf = get_boolean_resource (istate->dpy, "doubleBuffer", "Boolean");

# ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  istate->dbeclear_p = get_boolean_resource (istate->dpy, "useDBEClear", "Boolean");
#endif

# ifdef HAVE_COCOA      /* Don't second-guess Quartz's double-buffering */
  istate->dbuf = False;
# endif

  XGetWindowAttributes (istate->dpy, istate->window, &istate->xgwa);

  pop = get_boolean_resource (istate->dpy, "pop", "Boolean");
  drop = get_boolean_resource (istate->dpy, "drop", "Boolean");
  roam = get_boolean_resource (istate->dpy, "roam", "Boolean");
  istate->mode = POP;
  if (roam)
   istate->mode = ROAM;
  if (drop)
    istate->mode = DROP;
  if (pop)
    istate->mode = POP;

  return istate;
}


/* begin functiontable fcns -------------------------------------------------*/

static void *
daisy_init (Display * dpy, Window window)
{
  struct state *st = (struct state *) calloc (1, sizeof(*st));
  XGCValues gcv;

  st->dpy = dpy;
  st->window = window;
  st->eraser = NULL;

  st = fetch_resources (st);

  st->drawdelay = st->delay;
  st->erasedelay = (long) st->drawdelay  / 2;
  st->uptime = 0;

  st->daisies = (struct daisy **) calloc (st->count, sizeof (struct daisy *));

 /* set up colors */
  st->colors = (XColor *) calloc ( sizeof (*st->colors), st->ncolors);
  if (get_boolean_resource (st->dpy, "mono", "Boolean"))
    {
      st->ncolors = 1;
    }

  if (st->ncolors < 2)
    {
      st->colors[0].pixel = get_pixel_resource (st->dpy, st->xgwa.colormap,
					    "foreground", "Foreground");
    }

  else
    {
      make_random_colormap (st->dpy, st->xgwa.visual, st->xgwa.colormap,
			    st->colors, &st->ncolors, True, True, 0, True);
    }


  /* set up double buffering */

  if (st->dbuf)
    {
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
      if (st->dbeclear_p)
        st->b = xdbe_get_backbuffer (st->dpy, st->window, XdbeBackground);
      else 
        st->b = xdbe_get_backbuffer (st->dpy, st->window, XdbeUndefined);
      st->backb = st->b;xdbe_get_backbuffer (dpy, window, XdbeUndefined);
#endif 

      if (!st->b)
	{
	  st->ba =
	    XCreatePixmap (st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth);
	  st->bb =
	    XCreatePixmap (st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth);
	  st->b = st->ba;
	}
    }
  else
    {
      st->b = st->window;
    }


  gcv.foreground = get_pixel_resource ( st->dpy, st->xgwa.colormap,
					"background", "Background");
  st->erase_gc = XCreateGC (st->dpy, st->b, GCForeground, &gcv);


  if (st->mode == POP) st->daisiesactive = 1;
  else st->daisiesactive = st->count;
  fill_daisy_array(st);
    

  return st;
}



static unsigned long
daisy_draw (Display * dpy, Window window, void *closure)
{
struct state *st = (struct state *) closure;

  int i;
  if (st->eraser)
    {
     st->eraser = erase_window (st->dpy, st->window, st->eraser);
     return (st->erasedelay);
    }

 
	  if (is_screenchangetime(st))
	    {
                  st->uptime = 0;
	      /* clear screen and daisy array for the next pass */
	      if (st->daisiesactive == st->count)
		{
                  clear_daisy_array(st);
                  fill_daisy_array(st);
		  st->daisiesactive = 1;
 		  st->eraser = erase_window (st->dpy, st->window, st->eraser);
                  return (st->erasedelay);
		}		/* if time to clear screen in pop */
	      else
		{
		  /* add a new daisy to the group */
		  st->daisiesactive++;
		}
	    }

  
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  if (!st->dbeclear_p || !st->backb)
#endif 
      XFillRectangle (st->dpy, st->b, st->erase_gc, 0, 0, st->xgwa.width, st ->xgwa.height);


      for (i = 0; i < st->daisiesactive; i++)
	{
	  if (animate (st->b, st->dpy, st->mode, st->xgwa.width, st->xgwa.height, st->daisies[i]) < 0)
	    {
	      st->daisies[i] = new_daisy (st, st->dpy, st->b, st->mode, st->xgwa.width, st->xgwa.height);
	    }
	}


#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
      if (st->backb)
	{
	  XdbeSwapInfo info[1];
	  info[0].swap_window = st->window;
	  info[0].swap_action = (st->dbeclear_p ? XdbeBackground : XdbeUndefined);
	  XdbeSwapBuffers (st->dpy, info, 1);
	}
      else
#endif
	/* HAVE_DOUBLE_BUFFER_EXTENSION */
       if (st->dbuf)
	{
	  XCopyArea (st->dpy, st->b, st->window, st->erase_gc, 0, 0,
		     st->xgwa.width, st->xgwa.height, 0, 0);
	  st->b = (st->b == st->ba ? st->bb : st->ba);
	}

  return st->drawdelay;
}				


static void
daisy_reshape (Display *dpy, Window window, void *closure, 
                 unsigned int w, unsigned int h)
{
   struct state *st = (struct state *) closure;
    XGetWindowAttributes (st->dpy, st->window, &st->xgwa);
  if (! st->dbuf) {   /* #### more complicated if we have a back buffer... */
/*     XGetWindowAttributes (st->dpy, st->window, &st->xgwa); */
    XClearWindow (dpy, window);

  }
}

static Bool
daisy_event (Display *dpy, Window window, void *closure, XEvent *event)
{
  return False;
}


static void
daisy_free (Display *dpy, Window window, void *closure)
{
    struct state *st = (struct state *) closure;
   clear_daisy_array(st);
  free(st);
}


static const char *daisy_defaults[] = {
  ".background:		black",
  ".foreground:		white",
  "*delay:              30000",
  "*duration:           50",
  "*count:	        25",
  "*ncolors:		64",
  "*doubleBuffer:	True",
  "*drop:               False",
  "*roam:               False",
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  "*useDBE:		True",
  "*useDBEClear:        True",
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */
  0
};

static XrmOptionDescRec daisy_options[] = {
  {"-delay", ".delay", XrmoptionSepArg, 0},
  {"-count", ".count", XrmoptionSepArg, 0},
  {"-duration", ".duration", XrmoptionSepArg, 0},
  {"-ncolors", ".ncolors", XrmoptionSepArg, 0},
  {"-pop", ".pop", XrmoptionNoArg, "True"},
  {"-drop", ".drop", XrmoptionNoArg, "True"},
  {"-roam", ".roam", XrmoptionNoArg, "True"},
  {"-db", ".doubleBuffer", XrmoptionNoArg, "True"},
  {"-no-db", ".doubleBuffer", XrmoptionNoArg, "False"},
  {0, 0, 0, 0}
};

XSCREENSAVER_MODULE ("Daisy", daisy)




/* make order of struct declarations and inits agree */
/* reorder helper fcns */
/* make sure each fcn works on a local copy of the struct state --
   in fact, clean up parameters generally */
/* find more error checking opportunities--conditional compilations, etc */
/* check that the conditional compilation for OSX is right*/
/* run splint, clean up memory usage */

/* clear the screen and start over once in awhile in roam */
/* outline is recalculated each frame --
 * should it be calculated once and then just moved? */
