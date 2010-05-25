/* twinkle.c--by shegeek (Kelley Nielsen)
 * Copyright (c) 2006 by Kelley Nielsen <shegeek@cruzers.com>
 *
 * based on and steals code from deluxe.c by:
 *
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

#include <math.h>
#include "screenhack.h"

#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
# include "xdbe.h"
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */

#define countof(x) (sizeof(x)/sizeof(*(x)))
#define ABS(x) ((x)<0?-(x):(x))

struct state {
  Display * dpy;
  Window window;

  int density;
  int xinc, yinc;
  int delay;
  int ncolors;
  XColor *colors;
  Bool dbuf;
  XGCValues gcv;
  GC erase_gc;

  

  XWindowAttributes xgwa;
  Pixmap b, ba, bb;	/* double-buffer to reduce flicker */
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  XdbeBackBuffer backb;
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */

  struct twinkler ***twinklers;

};

struct twinkler {
  int x, y;
  int w, h;
  int step;
  int fuse;
  GC gc;
  void (*draw) (Display *, Drawable, struct twinkler *);
};


/* clamp values */
static struct state *
fetch_resources (struct state *inputstate)
{

  struct state * st = inputstate;

  st->density = get_integer_resource (st->dpy, "density", "Integer");
  if ((st->density < 1) || (st->density > 50)) st->density = 8;

  st->delay = get_integer_resource (st->dpy, "delay", "Integer");
    if (st->delay < 0 || 1000000 < st->delay ) st->delay = 30000;
 
  st->ncolors = get_integer_resource (st->dpy, "ncolors", "Integer");
    if (st->ncolors < 1 || 16383 < st->ncolors ) st->ncolors = 64;
 
  st->dbuf = get_boolean_resource (st->dpy, "doubleBuffer", "Boolean");

  XGetWindowAttributes (st->dpy, st->window, &st->xgwa);
  st->gcv.foreground = get_pixel_resource (st->dpy,  st->xgwa.colormap, "background", "Background");


  return st;
  }

/*-- drawing primitives: draw a single frame -----------*/

static void
cross1 (Display *dpy, Drawable w, struct twinkler *t)
{
  XDrawLine (dpy, w, t->gc, t->x-3, t->y-3, t->x+3, t->y+3);
  XDrawLine (dpy, w, t->gc, t->x-3, t->y+3, t->x+3, t->y-3);
}

static void
cross2 (Display *dpy, Drawable w, struct twinkler *t)
{
  XDrawLine (dpy, w, t->gc, t->x, t->y-3, t->x, t->y+3);
  XDrawLine (dpy, w, t->gc, t->x-3, t->y, t->x+3, t->y);
}

static void
box1 (Display *dpy, Drawable w, struct twinkler *t)
{
  XDrawLine (dpy, w, t->gc, t->x-3, t->y-3, t->x-3, t->y+3);
  XDrawLine (dpy, w, t->gc, t->x-3, t->y+3, t->x+3, t->y+3);
  XDrawLine (dpy, w, t->gc, t->x+3, t->y+3, t->x+3, t->y-3);
  XDrawLine (dpy, w, t->gc, t->x+3, t->y-3, t->x-3, t->y-3);
}

static void
diamond1 (Display *dpy, Drawable w, struct twinkler *t)
{
  XDrawLine (dpy, w, t->gc, t->x,   t->y-3, t->x-3, t->y);
  XDrawLine (dpy, w, t->gc, t->x-3, t->y,   t->x,   t->y+3);
  XDrawLine (dpy, w, t->gc, t->x,   t->y+3, t->x+3, t->y);
  XDrawLine (dpy, w, t->gc, t->x+3, t->y,   t->x,   t->y-3);
}



/*-- animators -----------------------------------------------------*/
/*-- for each new one of these added, be sure to assign to the array
  in init and increment the count variable in the state struct */

static void cross_spin_slow( Display *dpy, Drawable w, struct twinkler *t )
{
  if(t->step % 2 == 0){
  cross1 (dpy, w, t);
  }
  else
    {
   cross2 (dpy, w, t);
    }
  t->step = ++t->step % 2;

}

static void cross_spin_fast( Display *dpy, Drawable w, struct twinkler *t )
{
  if(t->step < 2){
  cross1 (dpy, w, t);
  }
  else
    {
   cross2 (dpy, w, t);
    }
  t->step = ++t->step % 4;
}
static void box_spin_slow( Display *dpy, Drawable w, struct twinkler *t )
{
  if(t->step % 2 == 0){
  box1 (dpy, w, t);
  }
  else
    {
   diamond1 (dpy, w, t);
    }
  t->step = ++t->step % 2;

}

static void box_spin_fast( Display *dpy, Drawable w, struct twinkler *t )
{
  if(t->step < 2){
  box1 (dpy, w, t);
  }
  else
    {
   diamond1 (dpy, w, t);
    }
  t->step = ++t->step % 4;
}


/*-- constructor ----------------------------------------------------*/

static struct twinkler *
make_twinkler (Display *dpy, Drawable d, int x, int y, int w, int h, 
               unsigned long pixel)
{
  XGCValues gcv;
  unsigned long flags;
  struct twinkler *t = (struct twinkler *) malloc (sizeof (*t));
  t->x = x + (random() % w);
  t->y = y + (random() % h);
  t->fuse = 10 + (random() % 20);

  flags = GCForeground;
  gcv.foreground = pixel;
 

  gcv.line_width = 1;
  gcv.line_style = LineSolid;
  gcv.cap_style = CapProjecting;
  gcv.join_style = JoinMiter;

  flags |= (GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle);
  t->gc = XCreateGC (dpy, d, flags, &gcv);

  switch (random() % 6) {
  case 0: case 1: t->draw = cross_spin_slow; break;
  case 2: case 3: t->draw = cross_spin_fast; break;
  case 4:         t->draw = box_spin_slow; break;
  case 5:         t->draw = box_spin_fast; break;
  default: abort(); break;
  }

  return t;
}


/*-- animation driver ----------------------------------------*/
static int
twinkle (Display *dpy, Drawable window, struct twinkler *t)
{
  int returncode;

  if (t->fuse <= 0)
    {
      XFreeGC (dpy, t->gc);
      memset (t, 0, sizeof(*t));
      free (t);
      returncode = -1;
    }
  else
    {
      t->draw (dpy, window, t);
      t->fuse--;
      returncode = 0;
    }
  return returncode;
}

/*-- functiontable fcns----------------------------------------------*/

static void *
twinkle_init (Display * dpy, Window window)
{
  struct state *st = (struct state *) calloc (1, sizeof(*st));
  int horiz_itor, vert_itor;
  st->dpy = dpy;
  st->window = window;

  st = fetch_resources (st);

  st->colors = (XColor *) calloc (sizeof(*st->colors), st->ncolors);

  if (get_boolean_resource(st->dpy, "mono", "Boolean"))
    {
    MONO:
      st->ncolors = 1;
      st->colors[0].pixel = get_pixel_resource( st->dpy, st->xgwa.colormap,"foreground", "Foreground" );
    }
  else
    {
      make_random_colormap (st->dpy, st->xgwa.visual, st->xgwa.colormap,
                            st->colors, &st->ncolors, True, True, 0, True);
      if (st->ncolors < 2)
        goto MONO;
    }

  if (st->dbuf)
    {
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
      st->b = st->backb = xdbe_get_backbuffer (st->dpy, st->window, XdbeUndefined);
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */

      if (!st->b)
        {
          st->ba = XCreatePixmap (st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth);
          st->bb = XCreatePixmap (st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth);
          st->b = st->ba;
        }
    }
  else
    {
      st->b = window;
    }

  st->xinc = st->xgwa.width / st->density;
  st->yinc = st->xgwa.height / st->density;

  /* example of malloc'ing 2d array is at 
   * http://www.etnus.com/Support/Tips/DivingOnDynamic.html
   */
  st->twinklers = (struct twinkler ***) calloc (st->density, 
                                    st->density * sizeof(struct twinkler *));

  for (horiz_itor = 0; horiz_itor < st->density; horiz_itor++){
    st->twinklers[horiz_itor] = calloc (st->density, sizeof( struct twinkler *));
  }

  for (horiz_itor = 0; horiz_itor < st->density; horiz_itor++){
    for (vert_itor = 0; vert_itor < st->density; vert_itor++){
    st->twinklers[horiz_itor][vert_itor] = make_twinkler (st->dpy, st->b, horiz_itor * st->xinc, vert_itor * st->yinc, st->xinc, st->yinc, st->colors[random() % st->ncolors].pixel);
    }
   }

  st->erase_gc = XCreateGC (st->dpy, st->b, GCForeground, &st->gcv);

  if (st->ba) XFillRectangle (st->dpy, st->ba, st->erase_gc, 0, 0, st->xgwa.width, st->xgwa.height);
  if (st->bb) XFillRectangle (st->dpy, st->bb, st->erase_gc, 0, 0, st->xgwa.width, st->xgwa.height);

  return st;
}



static unsigned long
twinkle_draw (Display *dpy, Window window, void *closure)
{
struct state *st = (struct state *) closure;

  int i, j;
      XFillRectangle (st->dpy, st->b, st->erase_gc, 0, 0, st->xgwa.width, st->xgwa.height);

      for (i = 0; i < st->density; i++){
       for (j = 0; j < st->density; j++){
        if (twinkle (st->dpy, st->b, st->twinklers[i][j]) < 0)
          st->twinklers[i][j] = make_twinkler (st->dpy, st->b, i * st->xinc, j * st->yinc,  st->xinc, st->yinc, st->colors[random() % st->ncolors].pixel);
    }
    }

#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
      if (st->backb)
        {
          XdbeSwapInfo info[1];
          info[0].swap_window = st->window;
          info[0].swap_action = XdbeUndefined;
          XdbeSwapBuffers (st->dpy, info, 1);
        }
      else
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */
      if (st->dbuf)
        {
          XCopyArea (st->dpy, st->b, st->window, st->erase_gc, 0, 0,
                     st->xgwa.width, st->xgwa.height, 0, 0);
          st->b = (st->b == st->ba ? st->bb : st->ba);
        }

      return (st->delay);
    }


static void
twinkle_reshape (Display *dpy, Window window, void *closure, 
                 unsigned int w, unsigned int h)
{
   struct state *st = (struct state *) closure;
    XGetWindowAttributes (st->dpy, st->window, &st->xgwa);
  if (! st->dbuf) {   /* #### more complicated if we have a back buffer... */
/*     XGetWindowAttributes (st->dpy, st->window, &st->xgwa); */
/* $$$ recalculate twinkler coordinates to reflect new window size */
    XClearWindow (dpy, window);

  }
}

static Bool
twinkle_event (Display *dpy, Window window, void *closure, XEvent *event)
{
  return False;
}


static void
twinkle_free (Display *dpy, Window window, void *closure)
{
    struct state *st = (struct state *) closure;
    int hitor, vitor;
    for (hitor = 0; hitor < st->density; hitor++)
      {
      for (vitor = 0; vitor < st->density; vitor++)
        {
	free(st->twinklers[hitor][vitor]);
	     }
      }
  free(st);
}



static const char *twinkle_defaults [] = {
  ".background:		black",
  ".foreground:		white",
  "*delay:		50000",
  "*density:		8",
  "*ncolors:		64",
  "*doubleBuffer:	True",
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  "*useDBE:		True",
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */
  0
};

static XrmOptionDescRec twinkle_options [] = {
  { "-delay",		".delay",	XrmoptionSepArg, 0 },
  { "-density",		".density",	XrmoptionSepArg, 0 },
  { "-ncolors",		".ncolors",	XrmoptionSepArg, 0 },
  { "-db",		".doubleBuffer", XrmoptionNoArg,  "True" },
  { "-no-db",		".doubleBuffer", XrmoptionNoArg,  "False" },
  { 0, 0, 0, 0 }
};

XSCREENSAVER_MODULE ("Twinkle", twinkle)


/* store fcn pointers to animators in an array,
   assign these pointers to state structs */

/* get rid of zawinski's goto */
/* clean up reshape */
/* break up init--possibly break off creation of twinkler array */
