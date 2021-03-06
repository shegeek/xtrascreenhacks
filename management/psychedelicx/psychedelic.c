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

#include <math.h>
#include "screenhack.h"
#include "clovermap.h"

#define DO_FPS
#ifdef DO_FPS
#include <sys/time.h>
#endif


#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
# include "xdbe.h"
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */

#define REPORT(s) printf("Location: %s\n", (s));

enum displaymode {CLOVER, SIMPLE, SIMPLE2};

struct pixelnode {
  int icoord;
  int jcoord;
  struct pixelnode * next;
};

struct state {
  Display * dpy;
  Window window;

  unsigned long delay;
  int ncolors;
  XColor *colors;
  Pixmap pixmap;
  int * pixgrid;
  struct pixelnode ** pixellist;
/*   Bool dbuf; */
  enum displaymode mode;
  int pixsize;
  int coloroffset;

  XWindowAttributes xgwa;
  int gridwidth, gridheight;
  GC maingc;
/*   GC erase_gc; */

/*   Pixmap b, ba, bb; */
/*   Pixmap b; */
/* # ifdef HAVE_DOUBLE_BUFFER_EXTENSION */
/*   Bool dbeclear_p; */
/*   XdbeBackBuffer backb; */
/* # endif    */         /* HAVE_DOUBLE_BUFFER_EXTENSION */
};

#ifdef DO_FPS
/* taken from glxgears */
static double current_time(void)
{
  struct timeval tv;
  struct timezone tz;
  (void) gettimeofday(&tv, &tz);
  return (double) tv.tv_sec + tv.tv_usec /  1000000.0;
}
#endif

static struct pixelnode * new_pixelnode(void){
  struct pixelnode * newnode;
  newnode = NULL;
  newnode = (struct pixelnode *) malloc(sizeof(struct pixelnode)); 
  if (NULL != newnode)
    {
      newnode->icoord = -1;
      newnode->jcoord = -1;
      newnode->next = NULL;
    }
  return newnode;
}

static struct pixelnode * insert_pixelnode(struct pixelnode * head, int icoord, int jcoord)
{
  struct pixelnode * newnode;
  newnode = NULL;
  newnode = new_pixelnode();
  if (NULL != newnode)
    {
      newnode->icoord = icoord;
      newnode->jcoord = jcoord;
      newnode->next = head;
    }
  else printf("Bad node!\n");
  return newnode;
}

static void free_pixellist(struct pixelnode * doomed)
{
  struct pixelnode * current;
  struct pixelnode * next;
  current = next = doomed;
  while (NULL != current)
    {
     next = current->next;
      free(current);
      current = next;
    }
  doomed = NULL;
}

/* ======= end linked list routines ==================== */

static void
simpleImage(int * bits, int width, int height, int numcolors)
{
	int	i, j;

	for (i = 0; i < height; i++) {
	  for (j = 0; j< width / 2; j++) {
	    bits[ i * width + j ] = bits[i * width + width - j] = j % numcolors;
	  }
	}
}

static void
simple2Image(int * bits, int width, int height, int numcolors)
{
	int	i, j;

	for (i = 0; i < height/2; i++) {
	  for (j = 0; j< width; j++) {
	    bits[ i * width + j ] =(i > height/2 ? numcolors -(i % numcolors) - 1 : i % numcolors);;
	    bits[ i * width + j ] = bits[(height - i) * width + j] = i % numcolors;


	  }
	}
}


/* end maps =================================== */


static struct state *
fetch_resources (struct state *inputstate)
{
  struct state * istate = inputstate;
  Bool clover, simple, simple2;
  Bool bigpixels, hugepixels;

  istate->delay = get_integer_resource (istate->dpy, "delay", "Integer");
     if (istate->delay < 0 || 100000000 < istate->delay ) istate->delay = 30000;
  istate->ncolors = get_integer_resource (istate->dpy, "ncolors", "Integer");
    if (istate->ncolors < 2 || 255 < istate->ncolors ) istate->ncolors = 16;
/*   istate->dbuf = get_boolean_resource (istate->dpy, "doubleBuffer", "Boolean"); */

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

  clover = get_boolean_resource (istate->dpy, "clover", "Boolean");
  simple = get_boolean_resource (istate->dpy, "simple", "Boolean");
  simple2 = get_boolean_resource (istate->dpy, "simple2", "Boolean");
  istate->mode = CLOVER;
  if (simple2)
   istate->mode = SIMPLE2;
  if (simple)
    istate->mode = SIMPLE;
  if (clover)
    istate->mode = CLOVER;

  return istate;
}


/* begin functiontable fcns -------------------------------------------------*/

static void *
psychedelic_init (Display * dpy, Window window)
{
  struct state *st = (struct state *) calloc (1, sizeof(struct state));
  XGCValues gcv;
  int i, j;
  long index;
  int colorindex;

  st->dpy = dpy;
  st->window = window;

  st = fetch_resources (st);

  st->gridwidth = st->xgwa.width / st->pixsize;
  st->gridheight = st->xgwa.height / st->pixsize;
  st->coloroffset = 0;

  st->pixellist = (struct pixelnode **) calloc(st->ncolors, sizeof(struct pixelnode *));
 for (i = 0; i < st->ncolors; i++)
    {
      st->pixellist[i] = NULL;
    }
 
 /* set up colors */
   gcv.foreground = get_pixel_resource ( st->dpy, st->xgwa.colormap,
					"foreground", "Foreground");
  gcv.background = get_pixel_resource ( st->dpy, st->xgwa.colormap,
					"background", "Background");
  st->colors = (XColor *) calloc ( st->ncolors, sizeof (XColor) );
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
      make_smooth_colormap (st->dpy, st->xgwa.visual, st->xgwa.colormap,
			    st->colors, &st->ncolors, True, False, False);
 
  /* add bg color to last slot, overwriting the assigned color */
      st->colors[st->ncolors-1].pixel = gcv.background;
      XQueryColor(st->dpy, st->xgwa.colormap, &st->colors[st->ncolors-1]);
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

	st->pixgrid = (int *) malloc(st->gridheight *  st->gridwidth * sizeof(int *));
	memset(st->pixgrid, 0, st->gridheight * st->gridwidth * sizeof(int));
	if (st->pixgrid == NULL)
	  {
	    printf("Couldn't allocate memory, exiting\n");
	    exit(EXIT_FAILURE);
	  }

  switch(st->mode)
    {
    case CLOVER:
      cloverImage(st->pixgrid, st->ncolors, st->gridwidth, st->gridheight, R);
      break;
    case SIMPLE:
      simpleImage(st->pixgrid, st->gridwidth, st->gridheight, st->ncolors);
  break;
    case SIMPLE2:
      simple2Image(st->pixgrid, st->gridwidth, st->gridheight, st->ncolors);
  break;
 default:
      cloverImage(st->pixgrid, st->ncolors, st->gridwidth, st->gridheight, R);
      /*    printf("Bad mode, exiting\n"); */
/*    exit(EXIT_FAILURE); */
      break;
}

  for (i = 0; i < st->gridheight; i++)
    {
      for (j = 0; j < st->gridwidth; j++)
	{
	  index = (long)i * (long)st->gridwidth + (long)j;
     /* this modulo shouldn't be necessary, but clovermap isn't contraining the indices correctly */
	  colorindex = st->pixgrid[index] % st->ncolors;
/* 	  colorindex = st->pixgrid[index]; */
	  if ((colorindex < 0) || (colorindex >= st->ncolors))
	    {
	      printf ("Colormap index out of range: %d\n", colorindex);
	      exit(1);
	    }
	  st->pixellist[colorindex] = insert_pixelnode(st->pixellist[colorindex], j, i);
	  if (st->pixellist[colorindex] == NULL)
	    {
	      printf ("List node not allocated, aborting\n");
	      exit(1);
	    }
	}
    }



/*   st->erase_gc = XCreateGC (st->dpy, st->b, GCForeground, &gcv); */
/*   printf("erase_gc init'd successfully\n"); */

  st->maingc = XCreateGC (st->dpy, st->window, GCForeground, &gcv);
  st->pixmap = XCreatePixmap(st->dpy, st->window, st->xgwa.width, 
			     st->xgwa.height, st->xgwa.depth);
  return st;
}



static unsigned long
psychedelic_draw (Display * dpy, Window window, void *closure)
{
struct state *st = (struct state *) closure;
 int i, j;
long index1, index;
 struct pixelnode * current;
 
/* taken from glxgears */
#ifdef DO_FPS
  static int frames = 0;
  static double tRate0 = -1.;
  double t = current_time();

  frames++;
  if (tRate0 < 0.) tRate0 = t;
  if (t - tRate0 > 5.0)
    {
      float seconds = t - tRate0;
      float fps = frames / seconds;
      printf("%d frames in %3.1f seconds = %6.3f FPS\n", frames, seconds,
             fps);
      tRate0 = t;
      frames = 0;
    }
#endif


/* #ifdef HAVE_DOUBLE_BUFFER_EXTENSION */
/*   if (!st->dbeclear_p || !st->backb) */
/* #endif  */
/*       XFillRectangle (st->dpy, st->b, st->erase_gc, 0, 0, st->xgwa.width, st ->xgwa.height); */

#define USE_LINKED_LIST

#ifndef USE_LINKED_LIST
      for (i = 0; i < st->gridheight; i++)
      {
	for (j = 0; j < st->gridwidth; j++)
	  {
	    index1=( i * st->gridwidth) + j;
	    index =((int)(st->pixgrid[index1]) + st->coloroffset) % st->ncolors;

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
#else
 for (i = 0; i < st->ncolors; i++)
   {
     index = (i + st->coloroffset) % st->ncolors;
     XSetForeground(st->dpy, st->maingc, st->colors[index].pixel);
     current = st->pixellist[i];
     while (current != NULL)
       {
	 XDrawPoint(st->dpy, st->pixmap, st->maingc, current->icoord, current->jcoord);
	 current = current->next;
       }
   }
#endif
	  XCopyArea (st->dpy, st->pixmap, st->window, st->maingc, 0, 0,
		     st->xgwa.width, st->xgwa.height, 0, 0);

	  if (++st->coloroffset >= st->ncolors) st->coloroffset -= st->ncolors;


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
/*   if (! st->dbuf) {  */           /* #### more complicated if we have a back buffer... */
    XClearWindow (dpy, window);
/*   } */
}

static Bool
psychedelic_event (Display *dpy, Window window, void *closure, XEvent *event)
{
  return False;
}


static void
psychedelic_free (Display *dpy, Window window, void *closure)
{
  int i;
    struct state *st = (struct state *) closure;
    /* valgrind doesn't notice the call to free_colors(), so call free() directly */
/*     free_colors(dpy, st->xgwa.colormap, st->colors, st->ncolors); */
    free(st->colors);
    XFreeGC(dpy, st->maingc);
/*     XFreeGC(dpy, st->erase_gc); */
    XFreePixmap(dpy, st->pixmap);
    free (st->pixgrid);
    for (i = 0; i < st->ncolors; i++)
      free_pixellist(st->pixellist[i]);
    free(st->pixellist);
  free(st);
}


static const char *psychedelic_defaults[] = {
  ".background:		black",
  ".foreground:		white",
  "*delay:              30000",
  "*ncolors:		128",
  "*big:                      False",
  "*huge:                      False",
/*   "*doubleBuffer:	False", */
  "*simple:               False",
  "*simple2:               False",
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
 {"-clover", ".clover", XrmoptionNoArg, "True"},
  {"-simple", ".simple", XrmoptionNoArg, "True"},
  {"-simple2", ".simple2", XrmoptionNoArg, "True"},
/*   {"-db", ".doubleBuffer", XrmoptionNoArg, "False"}, */
/*   {"-no-db", ".doubleBuffer", XrmoptionNoArg, "True"}, */
  {0, 0, 0, 0}
};

XSCREENSAVER_MODULE ("Psychedelic", psychedelic)

/* ============================================== */

/* add header, notes, etc to clovermap.c/h */
/* also fix the bug that causes it to contstrain the index to %ncolors incorrectly */

/*  fix reshape, and make use of new screen dimensions */

/* remove double buffering completely, free other pixmaps if they're left when finished
   -> is presently commented out, make sure we wouldn't be better off with some form of it */

/* either assign and use screen variables (width, depth, etc) to struct members and
 * use consistently, or remove completely */

/* struct members are passed around and returned flakily, do it right */

/* ouch this thing is slow. Is there any way to speed it up at all? */
/* why does using the linked lists speed up the simple maps but slow down clover? */

/* to run in small window: -geom =200x150+0+0 */


