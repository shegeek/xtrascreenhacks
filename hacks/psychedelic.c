/* psychedelic.c--by shegeek (Kelley Nielsen)
 * Copyright (c) 2011 by Kelley Nielsen <shegeek-dev@comcast.net>
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

#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
# include "xdbe.h"
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */

#define THETASTEP 0.02
#define THETASTEPS 314

/* from the original =======================*/
#define	NCOLORS	16
#define	R	5432
#define S	4321

#define	MIN(x, y)	((x) < (y))?(x):(y)
#define	MAX(x, y)	((x) > (y))?(x):(y)
#define	CEIL(a, b)	((a)+(b)-1)/(b)
#define	FLOOR(a, b)	CEIL((a)-(b), (b))
/* =====================================*/

enum displaymode {SIMPLE};

struct state {
  Display * dpy;
  Window window;

  int count;
/*   int daisiesactive; */
  unsigned long delay;
  int ncolors;
  XColor *colors;
  XImage * image;
  Pixmap pixmap;
  char * bits;
  int imagedepth;
  int duration;
  int uptime;
  Bool dbuf;
  enum displaymode mode;
  Bool first;
  int coloroffset;

  XWindowAttributes xgwa;
  int width;
  GC maingc;
  GC erase_gc;

  Pixmap b, ba, bb;
# ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  Bool dbeclear_p;
  XdbeBackBuffer backb;
# endif /* HAVE_DOUBLE_BUFFER_EXTENSION */

};





/* static XImage * */ static void
simpleImage(struct state *st, Display * display, Drawable window, int width, int height)
{
	int	i, j;
/* 	XImage	*im; */
  char * stbits = NULL;
  st->bits = (char *) malloc(width * height * sizeof(char));
  stbits = (char *) malloc(width * height * sizeof(char));
	if( (st->bits == NULL) || (stbits == NULL))
	  {
	    printf("Couldn't allocate memory for bits, exiting\n");
	    exit(1);
	  }
	else printf("malloc'd successfully\n");

	/* there is an xshm routine for this too, used in swirl */
/* 	im = XCreateImage(display, st->xgwa.visual, st->xgwa.depth,  */
/* 			ZPixmap, 0, st->bits, width, height, 8, width); */
	st->image = XCreateImage(display, st->xgwa.visual, st->xgwa.depth, ZPixmap, 0,
		       (char *)(calloc(width * height * sizeof(char), 1)), width, height, 8, 0);
/* 	printf("Image created, is %p\n", st->image); */

	for (i = 0; i < height; i++) {
	  for (j = 0; j< width; j++) {
			st->bits[i * width + j] = i % st->ncolors;
/* 			stbits[i * width + j] = i % st->ncolors; */
/* 			printf("bit at %d, %d assigned number %d\n", i, j, i % st->ncolors); */
	  }
	}

/* 	free(stbits); */
/* 	return im; */
}




static struct state *
fetch_resources (struct state *inputstate)
{
  struct state * istate = inputstate;
/*   Bool pop, drop, roam; */

  istate->count = get_integer_resource (istate->dpy, "count", "Integer");
    if (istate->count < 1 || 1000 < istate->count ) istate->count = 25;
  istate->delay = get_integer_resource (istate->dpy, "delay", "Integer");
     if (istate->delay < 0 || 100000000 < istate->delay ) istate->delay = 30000;
 istate->duration = get_integer_resource (istate->dpy, "duration", "Integer");
    if (istate->duration < 0 || 1000 < istate->duration ) istate->duration = 20;
  istate->ncolors = get_integer_resource (istate->dpy, "ncolors", "Integer");
    if (istate->ncolors < 1 || 255 < istate->ncolors ) istate->ncolors = 64;
  istate->dbuf = get_boolean_resource (istate->dpy, "doubleBuffer", "Boolean");

# ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  istate->dbeclear_p = get_boolean_resource (istate->dpy, "useDBEClear", "Boolean");
#endif

# ifdef HAVE_COCOA      /* Don't second-guess Quartz's double-buffering */
  istate->dbuf = False;
# endif

  XGetWindowAttributes (istate->dpy, istate->window, &istate->xgwa);

/*   pop = get_boolean_resource (istate->dpy, "pop", "Boolean"); */
/*   drop = get_boolean_resource (istate->dpy, "drop", "Boolean"); */
/*   roam = get_boolean_resource (istate->dpy, "roam", "Boolean"); */
/*   istate->mode = POP; */
/*   if (roam) */
/*    istate->mode = ROAM; */
/*   if (drop) */
/*     istate->mode = DROP; */
/*   if (pop) */
/*     istate->mode = POP; */
  istate->mode = SIMPLE;
  return istate;
}


/* begin functiontable fcns -------------------------------------------------*/

static void *
psychedelic_init (Display * dpy, Window window)
{
  struct state *st = (struct state *) calloc (1, sizeof(*st));
  XGCValues gcv;

  Bool writable = True;

  st->dpy = dpy;
  st->window = window;

  st = fetch_resources (st);

  st->uptime = 0;
  st->first = True;
  st->width = st->xgwa.width;
  st->coloroffset = 0;

 /* set up colors */
   gcv.foreground = get_pixel_resource ( st->dpy, st->xgwa.colormap,
					"background", "Background");
  gcv.background = get_pixel_resource ( st->dpy, st->xgwa.colormap,
					"foreground", "Foreground");
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
			    st->colors, &st->ncolors, True, True, &writable, True);
/*       if (writable != True) */
/* 	{ printf("Non-writable colors, exiting\n"); exit(1); } */

  /* add bg color to last slot */
/*       st->colors[st->ncolors+1].pixel = get_pixel_resource (st->dpy, st->xgwa.colormap, */
/* 					    "background", "Background"); */
/*          XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &st->colors[st->ncolors+1]); */
/*       XQueryColor(st->dpy, st->xgwa.colormap, &st->colors[st->ncolors+1]); */
   }

  printf("colors init'd successfully\n");

  /* set up double buffering */

  if (st->dbuf)
    {
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
      printf("inside dbe ifdef, dbeclear_p is %d\n", st->dbeclear_p);
      if (st->dbeclear_p){
	printf("in dbeclear\n");
        st->b = xdbe_get_backbuffer (st->dpy, st->window, XdbeBackground);
	printf("st->b init'd successfully\n");}
      else 
        st->b = xdbe_get_backbuffer (st->dpy, st->window, XdbeUndefined);
      st->backb = st->b;
      printf("backb assigned\n");
     xdbe_get_backbuffer (dpy, window, XdbeUndefined);
  printf("dbe init'd successfully\n");
#endif 

      if (!st->b)
	{
	  st->ba =
	    XCreatePixmap (st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth);
	  st->bb =
	    XCreatePixmap (st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth);
	  st->b = st->ba;
  printf("st->b init'd successfully in dbuf\n");
	}
    }
  else
    {
      st->b = st->window;
   printf("st->b init'd successfully in no dbuf\n");
   }

/*   st->image = NULL; */
/*   st->image =  *//* simpleImage(st, st->dpy, st->window, st->xgwa.width, st->xgwa.height); */
/*   printf("back from simpleImage, image is %p, bits is %p\n", (void *)st->image, (void *)st->bits); */
	st->image = XCreateImage(st->dpy, st->xgwa.visual, st->xgwa.depth, ZPixmap, 0,
				 calloc(st->xgwa.width * sizeof(int), 1) , st->xgwa.width, 1, 8, 0);

/* 	st->image->data = calloc(st->xgwa.width, 1); */
	if (st->image->data == NULL) {
	  printf("malloc failed, exiting\n");
	  exit(1);
	}

  st->erase_gc = XCreateGC (st->dpy, st->b, GCForeground, &gcv);
  printf("erase_gc init'd successfully\n");

  st->maingc = XCreateGC (st->dpy, st->window, GCForeground, &gcv);

  st->pixmap = XCreatePixmap(st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth);
/*   printf("pixmap created successfully\n"); */

/*     XDestroyImage(st->image); */

  printf("init'd successfully\n");    

  return st;
}



static unsigned long
psychedelic_draw (Display * dpy, Window window, void *closure)
{
struct state *st = (struct state *) closure;

 int i, j;
long index1, index2;
   unsigned long pointcolor;
 
  
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  if (!st->dbeclear_p || !st->backb)
#endif 
/*       XFillRectangle (st->dpy, st->b, st->erase_gc, 0, 0, st->xgwa.width, st ->xgwa.height); */


/* rotate colormap -- most important part of display loop */
/*     printf("rotating colors\n"); */
/*     rotate_colors(st->dpy,  st->xgwa.colormap, st->colors,  st->ncolors, 1); */

#define PIXSIZE 1
    for (i = 0; i < st->xgwa.height; i++)
      {
	for (j = 0; j < st->width; j++)
	  {
	    index1=( i * st->xgwa.width) + j;
/* 	    index2 = (int)(st->bits[index1]); */
	    index2 = (i + st->coloroffset )% st->ncolors;
  
  if(st->first == True)
/*   if(True) */
    {
	    printf("coordinates are %4d, %4d, index is %8ld, color is %3ld\n", j, i, index1, index2);
    }
/* 	    printf("blah blah blah"); printf ("boo bar bazs"); printf("fdjlfjoei fjlsdrjfoeijdff fd"); */
/* 	    printf("fldkfjldkfjl lfjdlfj;lfjf");printf("ldfjeoriueo ldjl fjld");printf("a;oidrjeljldfj"); */
/* 	    pointcolor = st->colors[index2].pixel;  */
	    XSetForeground(st->dpy, st->maingc, st->colors[index2].pixel );
	    XDrawPoint (st->dpy, st->pixmap, st->maingc, j, i);
/* 	    XPutPixel(st->image, j, 0, st->colors[index2].pixel); */
	    /*     XFillRectangle(st->dpy, st->window, st->maingc, j*PIXSIZE, i*PIXSIZE, PIXSIZE, PIXSIZE);*/;
 	  }
/*    XPutImage (st->dpy, st->pixmap, st->maingc, st->image, 0, 0, 0, i, st->width, 1 ); */
/*     printf("image put'd successfully\n"); */
      }
/*     if (st->first == True) st->first = False; */
/*     printf("image pixels filled successfully\n"); */
	  XCopyArea (st->dpy, st->pixmap, st->window, st->maingc, 0, 0,
		     st->xgwa.width, st->xgwa.height, 0, 0);


	  if (st->coloroffset++ > 1000) st->coloroffset -= 1000;

/*      XFlush(st->dpy); */

#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
      if (st->backb)
	{
/* 	  XdbeSwapInfo info[1]; */
/* 	  info[0].swap_window = st->window; */
/* 	  info[0].swap_action = (st->dbeclear_p ? XdbeBackground : XdbeUndefined); */
/* 	  XdbeSwapBuffers (st->dpy, info, 1); */
	}
      else
#endif
	/* HAVE_DOUBLE_BUFFER_EXTENSION */
       if (st->dbuf)
	{
/* 	  XCopyArea (st->dpy, st->b, st->window, st->erase_gc, 0, 0, */
/* 		     st->xgwa.width, st->xgwa.height, 0, 0); */
/* 	  st->b = (st->b == st->ba ? st->bb : st->ba); */
	}

  return st->delay;
}				


static void
psychedelic_reshape (Display *dpy, Window window, void *closure, 
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
psychedelic_event (Display *dpy, Window window, void *closure, XEvent *event)
{
  return False;
}


static void
psychedelic_free (Display *dpy, Window window, void *closure)
{
    struct state *st = (struct state *) closure;
/*    clear_daisy_array(st); */
  free(st);
}


static const char *psychedelic_defaults[] = {
  ".background:		black",
  ".foreground:		white",
  "*delay:              30000",
  "*duration:           50",
  "*count:	        25",
  "*ncolors:		16",
  "*doubleBuffer:	True",
/*   "*drop:               False", */
/*   "*roam:               False", */
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  "*useDBE:		True",
  "*useDBEClear:        True",
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */
  0
};

static XrmOptionDescRec psychedelic_options[] = {
  {"-delay", ".delay", XrmoptionSepArg, 0},
  {"-count", ".count", XrmoptionSepArg, 0},
  {"-duration", ".duration", XrmoptionSepArg, 0},
  {"-ncolors", ".ncolors", XrmoptionSepArg, 0},
/*   {"-pop", ".pop", XrmoptionNoArg, "True"}, */
/*   {"-drop", ".drop", XrmoptionNoArg, "True"}, */
/*   {"-roam", ".roam", XrmoptionNoArg, "True"}, */
  {"-db", ".doubleBuffer", XrmoptionNoArg, "True"},
  {"-no-db", ".doubleBuffer", XrmoptionNoArg, "False"},
  {0, 0, 0, 0}
};

XSCREENSAVER_MODULE ("Psychedelic", psychedelic)

/* technically working with simple image, but the code is a total mess.
   * flickers once or twice on startup.
   * Must be called with -no-db or else there is just a black screen,
   * even with db code commented out. 
   * also, don't forget to redirect output because it will spew a bunch of debug statements.
*/

