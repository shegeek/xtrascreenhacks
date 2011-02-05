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

/* from the original Clover =======================*/
#define	NCOLORS	16
#define	R	5432
#define S	4321

#define	MIN(x, y)	((x) < (y))?(x):(y)
#define	MAX(x, y)	((x) > (y))?(x):(y)
#define	CEIL(a, b)	((a)+(b)-1)/(b)
#define	FLOOR(a, b)	CEIL((a)-(b), (b))

#define getPixel(x, y)          bits[(y)*width+(x)]
#define putPixel(x, y, v)       bits[(y)*width+(x)] = (v)%nColors;
#define putRawPixel(x, y, v)    bits[(y)*width+(x)] = (v);

/* change these to reflect names used in line(), clipline(), and/or cloverImage as needed */
/* #define getPixel(x, y)          st->pixgrid[(y)*st->gridwidth+(x)] */
/* #define putPixel(x, y, v)       st->pixgrid[(y)*st->gridwidth+(x)] = (v)%st->ncolors; */
/* #define putRawPixel(x, y, v)    st->pixgrid[(y)*st->gridwidth+(x)] = (v); */

#define plot(x, y)      putPixel((x), (y), getPixel((x), (y))+1)

/* =====================================*/

enum displaymode {CLOVER, SIMPLE, SIMPLE2};

struct state {
  Display * dpy;
  Window window;

  unsigned long delay;
  int ncolors;
  XColor *colors;
  Pixmap pixmap;
  int * pixgrid;
/*   Bool dbuf; */
  enum displaymode mode;
  int pixsize;
  Bool first;
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


static void
simpleImage(int * bits, int width, int height, int numcolors)
{
	int	i, j;
/* 	int offset =  0; int offset2 =0; */

	for (i = 0; i < height; i++) {
/* 	  offset = offset2; */
	  for (j = 0; j< width; j++) {
/* 	    bits[ i * width + j ] =numcolors -(j % numcolors); */
	    bits[ i * width + j ] =(j > width/2 ? numcolors -(j % numcolors) : j % numcolors);;


/* 			bits[ i * width + j ] = i + offset2 % numcolors; */
/* 	  offset = (offset + 1) % 4; */
	  }
/* 	  offset2++; */
	}
}

static void
simple2Image(int * bits, int width, int height, int numcolors)
{
	int	i, j;
/* 	int offset =  0; int offset2 =0; */

	for (i = 0; i < height; i++) {
/* 	  offset = offset2; */
	  for (j = 0; j< width; j++) {
/* 	    bits[ i * width + j ] =numcolors -(j % numcolors); */
	    bits[ i * width + j ] =(i > height/2 ? numcolors -(i % numcolors) : i % numcolors);;


/* 			bits[ i * width + j ] = i + offset2 % numcolors; */
/* 	  offset = (offset + 1) % 4; */
	  }
/* 	  offset2++; */
	}
}


/* todo:
 *
 * make sure vars that are global in the original are passed down, 
 * esp. the pixgrid, dimensions, and numcolors =>done
 * => what's basepixel? => assumed good for nothing, commented out
 *
 * make sure pointers/references to the above agree, see esp. line 148
 *
 * make sure prototypes are done correctly
 *
 * final check
 *
 *
 * Determine the correct radius to use when screen width > RADIUS * 2
 *
 * background is filled in assuming the last color in the colors array is the background color
 */

/* functions from the original Clover =================================== */
static void 
clipLine(int * bits, int nColors, int width, int height,  int x0, int y0, int xn, int yn, int xe, int ye, int xf, int yf);
static void
line(int * bits, int nColors, int width, int height,  int fun, int x0, int y0, int dx, int dy, int xe, int ye, int xf, int yf);
/*
 * Basically the algorithm is to draw a series of Bresenham lines from the
 * center.  The "interference pattern" is built by incrementing the pixel value
 * of (x,y) every time it's touched; the resulting pattern is a product of the
 * vagaries of integer arithmetic.
 */

static void
cloverImage(int * bits, int nColors, int width, int height, int r)
{
        int     maxX, maxY, midX, midY, x, f, y;
        int     val, yy, x1, y1;
        int     i, o;
        int    *b;

        maxX = width - 1;
        maxY = height - 1;
        midX = maxX / 2;
        midY = maxY / 2;

        for (y = 0; y < height; y++) {
                b = &bits[y*width];
                for (x = 0; x < width; x++)
                        *b++ = nColors;         /* fill in background */
        }


        /*
         * Fill in the first semi-quadrant.
         */

        x = r;
        f = 0;
        for (y = 0; y < x; y++) {
                if (f > x) {
                        x--;
                        f = f-x - (x-1);
                }
                clipLine(bits, nColors, width, height,  midX, midY, x+midX, y+midY, 0, 0, maxX, maxY);
                f = f+y + y+1;
        }

        /*
         * Copy to the other seven, adjusting the horizontal and diagonal.
         */

      /*
         * Copy to the other seven, adjusting the horizontal and diagonal.
         */

        for (x = midX; x < maxX; x++) {
/*              putPixel(x, midY, (getPixel(x, midY) << 1) - 1);*/
                if (x - midX + midY <= maxY)
                        putPixel(x, x-midX+midY, 
                                (getPixel(x, x-midX+midY) << 1) - 1);
                yy = MIN(maxY, x + midY - midX);
                for (y = midY; y <= yy; y++) {
                        val = getPixel(x, y);
                        x1 = x;
                        y1 = y;
                        for (i = 0; i < 4; i++) {
                                if ((y1 < maxY) && (y1 > 0)) {
                                        putPixel(midX + midX - x1, y1, val);
                                        putPixel(x1, y1, val);
                                }
                                o = x1;
                                x1 = midX + midY - y1;
                                y1 = midY + o - midX;
                        }
                }
        }

/*         if (basePixel != 0)  */    /* bias by the first pixel value */
/*                 for (y = 0; y < maxY; y++) */
/*                         for (x = 0; x < maxX; x++) */
/*                                 putRawPixel(x, y, getPixel(x, y) + basePixel); */
                                
}

/*
 * (xe, ye) and (xf, yf) are the corners of a rectangle to clip a line to.
 * (x0, y0) and (xn, yn) are the endpoints of the line to clip.
 * The function argument that's being computed is the semi-quadrant;
 *  dx and dy are used to determine whether we're above or below the diagonal,
 *  since (x0, y0) is always the midpoint of the pattern.
 * (The LispM has the origin at lower left, instead of upper left, so
 * the numbers don't correspond to the normal Cartesian plane quadrants.)
 *
 * This routine is very general, but the calling code only builds lines in the
 * first semi-quadrant and then copies them everywhere else.
 */
static void 
clipLine(int * bits, int nColors, int width, int height,  int x0, int y0, int xn, int yn, int xe, int ye, int xf, int yf)
{
        int     dx, dy;

        dx = abs(xn - x0);
        dy = abs(yn - y0);

        if (xn > x0) {                          /* moving right */
                if (yn >= y0) {                 /* moving up */
                        if (dx > dy)            /* below diagonal */
			  line(bits, nColors, width, height, 0, x0, y0, dx, dy, xe, ye, xf, yf);
                        else
                                line(bits, nColors, width, height,  1, y0, x0, dy, dx, ye, xe, yf, xf);
                } else {
                        if (dx > dy)
                                line(bits, nColors, width, height,  7, x0, -y0, dx, dy, xe, -yf, xf, -ye);
                        else
                                line(bits, nColors, width, height,  6, -y0, x0, dy, dx, -yf, xe, -ye, xf);
                }
        } else {
                if (yn >= y0) {
                        if (dx > dy)
                                line(bits, nColors, width, height,  3, -x0, y0, dx, dy, -xf, ye, -xe, yf);
                        else
                                line(bits, nColors, width, height,  2, y0, -x0, dy, dx, ye, -xf, yf, -xe);
                } else {
                        if (dx > dy)
                                line(bits, nColors, width, height,  4, -x0, -y0, dx, dy, -xf, -yf, -xe, -ye);
                        else
                                line(bits, nColors, width, height,  5, -y0, -x0, dy, dx, -yf, -xf, -ye, -xe);
                }
        }
}


/*
 * Clip symmetric segment (x0, y0) thru (xn, yn) to the rectangle 
 * (xe, ye) < (xf, yf).
 *
 * The original says:
 *
 * "This routine incorrectly assumes that the subsegment starts prior to the
 * midpoint of the supersegment.  The 'divide for nearest integer' (i.e.,
 * divide for remainder of minimum magnitude), which is simulated by the FLOOR
 * and CEIL of num and (dx <<1), always rounds up on the half integer case, but
 * should round down (for symmetry) if startup is in 2nd half. It would be
 * nice to have these other flavors of divide.'
 */
static void
line(int * bits, int nColors, int width, int height,  int fun, int x0, int y0, int dx, int dy, int xe, int ye, int xf, int yf)
{
        int     x, num, lx;
        int     xx, y, x00, f;
        int     x11;

        x = MAX(x0,  MAX(xe, 
                        (dy == 0)? xe :
                                   x0 + CEIL(dx * (((ye - y0)<<1) - 1), 
                                                (dy << 1))));
        num = dx + 2*dy*(x - x0);
        lx = MIN(xf, (dy == 0) ? xf :
                                x0 + CEIL(dx * (((yf - y0)<<1) - 1), 
                                                (dy << 1)));
        xx = MIN(lx, x0 + (dx>>1));
        y = y0 + FLOOR(num, (dx<<1));
        f = (FLOOR(num, (dx<<1)) - dx) >> 1;

        for (x00 = x; x00 < xx; x00++,f+=dy) {
                if (f+f > dx) {
                        f -= dx;
                        y++;
                }
                switch(fun) {
                case 0: plot(x00, y);   break;
                case 1: plot(y, x00);   break;
                case 2: plot(-y, x00);  break;
                case 3: plot(-x00, y);  break;
                case 4: plot(-x00, -y); break;
                case 5: plot(-y, -x00); break;
                case 6: plot(y, -x00);  break;
                case 7: plot(x00, -y);  break;
                }
        }

        for (x11 = x00; x11 < lx; x11++, f+=dy) {
                if (f + f > dx) {
                        f -= dx;
                        y++;
                }
                switch(fun) {
                case 0: plot(x11, y);   break;
                case 1: plot(y, x11);   break;
                case 2: plot(-y, x11);  break;
                case 3: plot(-x11, y);  break;
                case 4: plot(-x11, -y); break;
                case 5: plot(-y, -x11); break;
                case 6: plot(y, -x11);  break;
                case 7: plot(x11, -y);  break;
                }
        }
}


/* end functions from the original Clover ================================ */


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
  struct state *st = (struct state *) calloc (1, sizeof(*st));
  XGCValues gcv;

  st->dpy = dpy;
  st->window = window;

  st = fetch_resources (st);

  st->first = True;
  st->gridwidth = st->xgwa.width / st->pixsize;
  st->gridheight = st->xgwa.height / st->pixsize;
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
 
  /* add bg color to last slot, overwriting the assigned color */
      st->colors[st->ncolors-1].pixel = gcv.background;
/*          XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &st->colors[st->ncolors+1]); */
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

  
/* #ifdef HAVE_DOUBLE_BUFFER_EXTENSION */
/*   if (!st->dbeclear_p || !st->backb) */
/* #endif  */
/*       XFillRectangle (st->dpy, st->b, st->erase_gc, 0, 0, st->xgwa.width, st ->xgwa.height); */


     for (i = 0; i < st->gridheight; i++)
      {
	for (j = 0; j < st->gridwidth; j++)
	  {
	    index1=( i * st->gridwidth) + j;
	    index =((int)(st->pixgrid[ i * st->gridwidth + j ]) + st->coloroffset) % st->ncolors;;
  
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
    struct state *st = (struct state *) closure;
    /* valgrind doesn't notice the call to free_colors(), so call free() directly */
/*     free_colors(dpy, st->xgwa.colormap, st->colors, st->ncolors); */
    free(st->colors);
    XFreeGC(dpy, st->maingc);
/*     XFreeGC(dpy, st->erase_gc); */
    XFreePixmap(dpy, st->pixmap);
    free (st->pixgrid);
  free(st);
}


static const char *psychedelic_defaults[] = {
  ".background:		black",
  ".foreground:		white",
  "*delay:              10000",
  "*ncolors:		16",
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

/* add background color (supposed to be black) to the color array */
/*  fix reshape, and make use of new screen dimensions */

/* remove double buffering completely, free other pixmaps if they're left when finished
   -> is presently commented out, make sure we wouldn't be better off with some form of it */
/* either assign and use screen variables (width, depth, etc) to struct members and
 * use consistently, or remove completely */
/* struct members are passed around and returned flakily, do it right */

/* original clover allowed the user to adjust speed (with +/- keys) -- implement? */

/* ouch this thing is slow. Is there any way to speed it up at all? */




