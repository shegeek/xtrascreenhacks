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

  unsigned long delay;
  int ncolors;
  XColor *colors;
  Pixmap pixmap;
  int * bits;
  int uptime;
  Bool dbuf;
  enum displaymode mode;
  int pixsize;
  Bool first;
  int coloroffset;

  XWindowAttributes xgwa;
  int width;
  GC maingc;
/*   GC erase_gc; */

/*   Pixmap b, ba, bb; */
/*   Pixmap b; */
/* # ifdef HAVE_DOUBLE_BUFFER_EXTENSION */
/*   Bool dbeclear_p; */
/*   XdbeBackBuffer backb; */
/* # endif    */         /* HAVE_DOUBLE_BUFFER_EXTENSION */

};


static void
simpleImage(struct state *st, int width, int height)
{
	int	i, j;
/* 	int offset =  0; int offset2 =0; */
/* should do this in init, so it's coded only once */
	st->bits = (int *) malloc(height *  width * sizeof(int *));

	if (st->bits == NULL)
	  {
	    printf("Couldn't allocate memory for bits, exiting\n");
	    exit(1);
	  }
/* 	else printf("malloc'd successfully\n"); */

	for (i = 0; i < height; i++) {
/* 	  offset = offset2; */
	  for (j = 0; j< width; j++) {
			st->bits[ i * width + j ] = i % st->ncolors;
/* 			st->bits[ i * width + j ] = i + offset % st->ncolors; */
/* 	  offset = (offset + 1) % 4; */

/* 			printf("bit at %d, %d assigned number %d\n", i, j, i % st->ncolors); */
	  }
/* 	  offset2++; */
	}
}




static struct state *
fetch_resources (struct state *inputstate)
{
  struct state * istate = inputstate;
/*   Bool pop, drop, roam; */
  Bool bigpixels, hugepixels;

  istate->delay = get_integer_resource (istate->dpy, "delay", "Integer");
     if (istate->delay < 0 || 100000000 < istate->delay ) istate->delay = 30000;
  istate->ncolors = get_integer_resource (istate->dpy, "ncolors", "Integer");
    if (istate->ncolors < 2 || 255 < istate->ncolors ) istate->ncolors = 16;
  istate->dbuf = get_boolean_resource (istate->dpy, "doubleBuffer", "Boolean");

/* # ifdef HAVE_DOUBLE_BUFFER_EXTENSION */
/*   istate->dbeclear_p = get_boolean_resource (istate->dpy, "useDBEClear", "Boolean"); */
/* #endif */

/* # ifdef HAVE_COCOA    */   /* Don't second-guess Quartz's double-buffering */
/*   istate->dbuf = False; */
/* # endif */

  XGetWindowAttributes (istate->dpy, istate->window, &istate->xgwa);

bigpixels = get_boolean_resource(istate->dpy, "big", "Boolean");
hugepixels = get_boolean_resource(istate->dpy, "huge", "Boolean");
 istate->pixsize = 1;
 if (bigpixels)   istate->pixsize = 2;
 if (hugepixels)   istate->pixsize = 4;


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

  st->dpy = dpy;
  st->window = window;

  st = fetch_resources (st);

  st->uptime = 0;
  st->first = True;
  st->width = st->xgwa.width;
  st->coloroffset = 0;

 
 /* set up colors */
   gcv.foreground = get_pixel_resource ( st->dpy, st->xgwa.colormap,
					"foreground", "Foreground");
  gcv.background = get_pixel_resource ( st->dpy, st->xgwa.colormap,
					"background", "Background");
 st->colors = (XColor *) calloc ( sizeof (*st->colors), st->ncolors);
  if (get_boolean_resource (st->dpy, "mono", "Boolean"))
    {
      st->ncolors = 2;
      st->colors[0].pixel = gcv.background;
      st->colors[1].pixel = gcv.foreground;
    }
  else
    {
      /* the classic version of this hack works by cycling the colormap.
       * Writable color cells are necessary to do this. However, a lot of people
       * use some form of GL desktop now, and the best visual for GL is TrueColor;
       * hence, we are not likely to get writable cells, so re-design the hack as if we don't..
       */
      make_random_colormap (st->dpy, st->xgwa.visual, st->xgwa.colormap,
			    st->colors, &st->ncolors, True, True, False, True);
 
  /* add bg color to last slot */
/*       st->colors[st->ncolors+1].pixel = get_pixel_resource (st->dpy, st->xgwa.colormap, */
/* 					    "background", "Background"); */
/*          XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &st->colors[st->ncolors+1]); */
/*       XQueryColor(st->dpy, st->xgwa.colormap, &st->colors[st->ncolors+1]); */
   }


  /* set up double buffering */

 /*  if (st->dbuf) */
/*     { */
/* #ifdef HAVE_DOUBLE_BUFFER_EXTENSION */
/*       printf("inside dbe ifdef, dbeclear_p is %d\n", st->dbeclear_p); */
/*       if (st->dbeclear_p){ */
/* 	printf("in dbeclear\n"); */
 /*        st->b = xdbe_get_backbuffer (st->dpy, st->window, XdbeBackground);  */
	/* 	printf("st->b init'd successfully\n");*/      /*     }  */
/*       else  */
/*         st->b = xdbe_get_backbuffer (st->dpy, st->window, XdbeUndefined); */
/*       st->backb = st->b; */
/*       printf("backb assigned\n"); */
/*      xdbe_get_backbuffer (dpy, window, XdbeUndefined); */
/*   printf("dbe init'd successfully\n"); */
/* #endif  */

 /*      if (!st->b) */
/* 	{ */
/* 	  st->ba = */
/* 	    XCreatePixmap (st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth); */
/* 	  st->bb = */
/* 	    XCreatePixmap (st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth); */
/* 	  st->b = st->ba; */
/*   printf("st->b init'd successfully in dbuf\n"); */
/* 	} */
/*     } */
/*   else */
/*     { */
/*       st->b = st->window; */
/*    printf("st->b init'd successfully in no dbuf\n"); */
/*    } */

  switch(st->mode)
    {
    case SIMPLE:
    simpleImage(st, st->xgwa.width, st->xgwa.height);
  break;
 default:
      simpleImage(st, st->xgwa.width, st->xgwa.height);
      break;
}

/*   st->erase_gc = XCreateGC (st->dpy, st->b, GCForeground, &gcv); */
/*   printf("erase_gc init'd successfully\n"); */

  st->maingc = XCreateGC (st->dpy, st->window, GCForeground, &gcv);

  st->pixmap = XCreatePixmap(st->dpy, st->window, st->xgwa.width, st->xgwa.height, st->xgwa.depth);
/*   printf("pixmap created successfully\n"); */

/*   printf("init'd successfully\n");     */

  return st;
}



static unsigned long
psychedelic_draw (Display * dpy, Window window, void *closure)
{
struct state *st = (struct state *) closure;

 int i, j;
long index1, index;

  
/* #ifdef HAVE_DOUBLE_BUFFER_EXTENSION */
/*   if (!st->dbeclear_p || !st->backb) */
/* #endif  */
/*       XFillRectangle (st->dpy, st->b, st->erase_gc, 0, 0, st->xgwa.width, st ->xgwa.height); */


     for (i = 0; i < st->xgwa.height / st->pixsize; i++)
      {
	for (j = 0; j < st->width / st->pixsize; j++)
	  {
	    index1=( i * st->xgwa.width) + j;
	    index =((int)(st->bits[ i * st->xgwa.width + j ]) + st->coloroffset) % st->ncolors;;
  
  if(st->first == True)
/*   if(True) */
    {
/* 	    printf("coordinates are %4d, %4d, index is %8ld, color is %3ld\n", j, i, index1, index); */
    }
	    XSetForeground(st->dpy, st->maingc, st->colors[index].pixel );
	    if (st->pixsize > 1){
	    XFillRectangle(st->dpy, st->pixmap, st->maingc, j*st->pixsize, i*st->pixsize, 
			   st->pixsize, st->pixsize);
	    }
	    else
	      {
	    XDrawPoint (st->dpy, st->pixmap, st->maingc, j, i);
	      }
 	  }
      }
/*     if (st->first == True) st->first = False; */
/*     printf("image pixels filled successfully\n"); */
	  XCopyArea (st->dpy, st->pixmap, st->window, st->maingc, 0, 0,
		     st->xgwa.width, st->xgwa.height, 0, 0);


	  if (st->coloroffset++ >= st->ncolors) st->coloroffset -= st->ncolors;

/*      XFlush(st->dpy); */

/* #ifdef HAVE_DOUBLE_BUFFER_EXTENSION */
/*       if (st->backb) */
/* 	{ */
/* 	  XdbeSwapInfo info[1]; */
/* 	  info[0].swap_window = st->window; */
/* 	  info[0].swap_action = (st->dbeclear_p ? XdbeBackground : XdbeUndefined); */
/* 	  XdbeSwapBuffers (st->dpy, info, 1); */
/* 	} */
/*       else */
/* #endif */
	/* HAVE_DOUBLE_BUFFER_EXTENSION */
/*        if (st->dbuf) */
/* 	{ */
/* 	  XCopyArea (st->dpy, st->b, st->window, st->erase_gc, 0, 0, */
/* 		     st->xgwa.width, st->xgwa.height, 0, 0); */
/* 	  st->b = (st->b == st->ba ? st->bb : st->ba); */
/* 	} */

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
    /* valgrind doesn't notice the call to free_colors(), so call free() directly */
/*     free_colors(dpy, st->xgwa.colormap, st->colors, st->ncolors); */
    free(st->colors);
    XFreeGC(dpy, st->maingc);
/*     XFreeGC(dpy, st->erase_gc); */
    XFreePixmap(dpy, st->pixmap);
    free (st->bits);
  free(st);
}


static const char *psychedelic_defaults[] = {
  ".background:		black",
  ".foreground:		white",
  "*delay:              30000",
  "*ncolors:		16",
  "*big:                      False",
  "*huge:                      False",
/*   "*doubleBuffer:	False", */
/*   "*drop:               False", */
/*   "*roam:               False", */
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  "*useDBE:		False",
  "*useDBEClear:        False",
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */
  0
};

static XrmOptionDescRec psychedelic_options[] = {
  {"-delay", ".delay", XrmoptionSepArg, 0},
  {"-ncolors", ".ncolors", XrmoptionSepArg, 0},
  {"-big", ".big", XrmoptionNoArg, "True"},
  {"-huge", ".huge", XrmoptionNoArg, "True"},
/*   {"-pop", ".pop", XrmoptionNoArg, "True"}, */
/*   {"-drop", ".drop", XrmoptionNoArg, "True"}, */
/*   {"-roam", ".roam", XrmoptionNoArg, "True"}, */
/*   {"-db", ".doubleBuffer", XrmoptionNoArg, "False"}, */
/*   {"-no-db", ".doubleBuffer", XrmoptionNoArg, "True"}, */
  {0, 0, 0, 0}
};

XSCREENSAVER_MODULE ("Psychedelic", psychedelic)

/* ============================================== */

/* add background color (supposed to be black) to the color array */
/*  fix reshape, and make use of new screen dimensions */

/* remove double buffering completely, free other pixmaps if they're left when finished
   -> is presently commented out, make sure we wouldn't be better off with some form of it */
/* either assign and use screen variables (width, depth, etc) to struct members and
 * use consistently, or remove completely */
/* struct members are passed around and returned flakily, do it right */




