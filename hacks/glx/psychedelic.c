/*psychedelic.c, Copyright (c) 2014 Kelley Nielsen <kelleynnn@gmail.com>
 *
 * Based on:
 * blocktube, Copyright (c) 2003 Lars Damerow <lars@oddment.org>
 *
 * [which is in turn] Based on Jamie Zawinski's original dangerball code.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#define refresh_psychedelic 0
#define psychedelic_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include "xlockmore.h"
#include "clovermap.h"
#include "plasmamap.h"
#include <math.h>

#define DEFAULTS        "*delay:	30000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL			/* whole file */

#define DEF_NCOLORS     "64"
#define DEF_SMOOTH "False"
#define DEF_CLOVER "False"
#define DEF_STRIPES "False"
#define DEF_CIRCLE "False"
#define DEF_PLASMA "False"
#define DEF_BUMPYROUND "False"
#define DEF_BUMPYSQUARE "False"

enum displaymap
{ CLOVER, STRIPES, CIRCLE, PLASMA, BUMPYROUND, BUMPYSQUARE };

typedef struct
{
  GLXContext *glx_context;
  XColor *colors;
  GLuint *pixcolors;
  int numcolors;
  int *pixgrid;
  GLuint *piximage;
  int gridwidth, gridheight;
  int coloroffset;
} psychedelic_configuration;

static psychedelic_configuration *lps = NULL;

static int ncolors;
static Bool smooth;
static Bool clover;
static Bool stripes;
static Bool circle;
static Bool plasma;
static Bool bumpy;
static Bool bumpysquare;

static XrmOptionDescRec opts[] = {
  {"-ncolors", ".ncolors", XrmoptionSepArg, 0},
  {"-smooth", ".smooth", XrmoptionNoArg, "True"},
  {"+smooth", ".smooth", XrmoptionNoArg, "False"},
  {"-clover", ".clover", XrmoptionNoArg, "True"},
  {"+clover", ".clover", XrmoptionNoArg, "False"},
  {"-stripes", ".stripes", XrmoptionNoArg, "True"},
  {"+stripes", ".stripes", XrmoptionNoArg, "False"},
  {"-circle", ".circle", XrmoptionNoArg, "True"},
  {"+circle", ".circle", XrmoptionNoArg, "False"},
  {"-plasma", ".plasma", XrmoptionNoArg, "True"},
  {"+plasma", ".plasma", XrmoptionNoArg, "False"},
  {"-bumpy", ".bumpy", XrmoptionNoArg, "True"},
  {"+bumpy", ".bumpy", XrmoptionNoArg, "False"},
  {"-bumpysquare", ".bumpysquare", XrmoptionNoArg, "True"},
  {"+bumpysquare", ".bumpysquare", XrmoptionNoArg, "False"},
};

static argtype vars[] = {
  {&ncolors, "ncolors", "NColors", DEF_NCOLORS, t_Int},
  {&smooth, "smooth", "Smooth", DEF_SMOOTH, t_Bool},
  {&clover, "clover", "Clover", DEF_CLOVER, t_Bool},
  {&stripes, "stripes", "Stripes", DEF_STRIPES, t_Bool},
  {&circle, "circle", "Circle", DEF_CIRCLE, t_Bool},
  {&plasma, "plasma", "Plasma", DEF_PLASMA, t_Bool},
  {&bumpy, "bumpy", "BumpyRound", DEF_BUMPYROUND, t_Bool},
  {&bumpysquare, "bumpysquare", "BumpySquare", DEF_BUMPYSQUARE, t_Bool},
};

static OptionStruct desc[] = {
  {"-ncolors", "how many different colors the screen will use"},
  {"-smooth", "whether to use smooth color transitions"},
  {"-clover",
   "whether to use the clover (default) colormap (overrides other maps)"},
  {"-stripes", "whether to use the striped colormap"},
  {"-circle", "whether to use the circle colormap"},
  {"-plasma", "whether to use the plasma colormap"},
  {"-bumpy", "whether to use the round eggcrate colormap"},
  {"-bumpysquare", "whether to use the square eggcrate colormap"},
};

ENTRYPOINT ModeSpecOpt psychedelic_opts =
  { countof (opts), opts, countof (vars), vars, desc };

#ifdef USE_MODULES
ModStruct psychedelic_description =
  { "psychedelic", "init_psychedelic", "draw_psychedelic",
  "release_psychedelic",
  "draw_psychedelic", "init_psychedelic", (char *) NULL, &psychedelic_opts,
  30000, 30, 1, 1, 64, 1.0, "",
  "Various color cycling effects", 0, NULL
};
#endif /* USE_MODULES */

static void
handleGLerrors (char *guiltyfunction)
{
  GLenum errcode;
  const GLubyte *errstring;

  errcode = glGetError ();
  if (GL_NO_ERROR != errcode)
    {
      errstring = gluErrorString (errcode);
      fprintf (stderr, "GL error in %s: %s\n", guiltyfunction, errstring);
    }
}

/* === some simple maps that are too small to merit their own file */

static void
stripesImage (int *bits, int width, int height, int numcolors)
{
  int orientation = random () % 3;
  int i, j;

  for (i = 0; i < height; i++)
    {
      for (j = 0; j < width; j++)
	{
	  switch (orientation) {
		  case 0:
	    bits[(i + 1) * width - j] = j % numcolors;
	    break;
		  case 1:
	    bits[(i + 1) * width - j] = (i + j) % numcolors;
	    break;
		  case 2:
	    bits[i * width + j] = i % numcolors;
	    break;
	  }
	}
    }
}

static void
circleImage (int *bits, int width, int height, int numcolors)
{
  int halfwidth = width / 2;
  int halfheight = height / 2;
  int halfdist = sqrt(halfwidth * halfwidth + halfheight * halfheight);
  int shrinkx = random() % 2;
  int shrinky = random() % 2;
  int xdist, ydist, distance;
  int currentcolor;
  int i, j;

  for (i = 0; i <= halfheight; i++)
    {
      for (j = 0; j <= halfwidth; j++)
	{
		xdist = (halfwidth - j);
		if (shrinkx)
			xdist /= 2;
		ydist = (halfheight - i);
		if (shrinky)
			ydist /= 2;
		distance = (int)floor(sqrt(xdist * xdist + ydist * ydist));
		currentcolor = (halfdist - distance) % numcolors;
	    bits[i * width + j] = currentcolor;
	    bits[(i + 1) * width - j] = currentcolor;
	    bits[(height - i) * width + j] = currentcolor;
	    bits[(height - i + 1) * width - j] = currentcolor;
	}
    }
}


/* parameter square controls the shape of the sine bumps
 * a 0 value creates egg crate shaped curvy sine bumps,
 * a nonzero value creates checkerboard shaped squarish bumps
 */
static void
sineImage (int *bits, int width, int height, int numcolors, int square)
{
  float sinemap;
  float xexpand, yexpand;
  int itor, jtor;
  xexpand = yexpand = 0.0;
  while (xexpand == 0.0)
    xexpand = (float) (random () % 100) / 1000.;
  while (yexpand == 0.0)
    yexpand = (float) (random () % 100) / 1000.;
  for (itor = 0; itor < height; itor++)
    {
      for (jtor = 0; jtor < width; jtor++)
	{
	  /* factors between 0.1 and 0.01, assign both separately */
	  if (square == 0)
	    {
	      sinemap =
		0.25 * (sin (yexpand * itor) + sin (xexpand * jtor)) + 0.5;
	    }
	  else
	    {
	      sinemap = sin (yexpand * itor) * sin (xexpand * jtor) + 1.;
	    }
	  bits[itor * width + jtor] = sinemap * (numcolors - 1);
	}
    }
}

/* ===== end maps ============== */

ENTRYPOINT void
reshape_psychedelic (ModeInfo * mi, int width, int height)
{
  glViewport (0, 0, (GLint) width, (GLint) height);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
/*    height is reversed to make the viewport have the same coordinates as an x window */
  gluOrtho2D (0., (float) width, 0., (float) height);
  glMatrixMode (GL_MODELVIEW);
  handleGLerrors ("reshape");
}

ENTRYPOINT void
init_psychedelic (ModeInfo * mi)
{
  psychedelic_configuration *lp;
  enum displaymap dmap;
  int loop;

  if (!lps)
    {
      lps = (psychedelic_configuration *)
	calloc (MI_NUM_SCREENS (mi), sizeof (psychedelic_configuration));
      if (!lps)
	{
	  fprintf (stderr, "%s: out of memory\n", progname);
	  exit (1);
	}
      lp = &lps[MI_SCREEN (mi)];
    }

  lp = &lps[MI_SCREEN (mi)];
  lp->glx_context = init_GL (mi);

  lp->gridwidth = MI_WIDTH (mi);
  lp->gridheight = MI_HEIGHT (mi);
  lp->coloroffset = 0;

  if (MI_IS_MONO (mi))
    {
      lp->numcolors = 2;
/* 	lp->pixcolors = calloc(2, sizeof(GLuint)); */
/* 	lp->pixcolors[0] = 0xffffff00; */
/* 	lp->pixcolors[1] = 0x0; */
      lp->colors = calloc (2, sizeof (*lp->colors));
      lp->colors[0].pixel = MI_BLACK_PIXEL (mi);
      lp->colors[1].pixel = MI_WHITE_PIXEL (mi);
      XQueryColor (MI_DISPLAY (mi), MI_COLORMAP (mi), &lp->colors[0]);
      XQueryColor (MI_DISPLAY (mi), MI_COLORMAP (mi), &lp->colors[1]);
    }
  else
    {
      lp->numcolors = ((ncolors > 1 && ncolors <= NUMCOLORS) ? ncolors : 64);
      lp->colors = calloc (lp->numcolors, sizeof (*lp->colors));
      if (smooth)
	make_smooth_colormap (MI_DISPLAY (mi), MI_VISUAL (mi),
			      MI_COLORMAP (mi), lp->colors, &lp->numcolors,
			      True, False, 0);
      else
	{
	  make_random_colormap (MI_DISPLAY (mi), MI_VISUAL (mi),
				MI_COLORMAP (mi), lp->colors, &lp->numcolors,
				True, False, 0, True);

	  /* add bg color to last slot, overwriting the assigned color */
	  lp->colors[lp->numcolors - 1].pixel = MI_BLACK_PIXEL (mi);
	  XQueryColor (MI_DISPLAY (mi), MI_COLORMAP (mi),
		       &lp->colors[lp->numcolors - 1]);
	}
    }
  lp->pixcolors = calloc (lp->numcolors, sizeof (GLuint));
  if (lp->pixcolors == NULL)
    {
      fprintf (stderr, "%s: out of memory\n", progname);
      exit (1);
    }

/*     lp->pixcolors[0] = 0x0; */
  for (loop = 0; loop < lp->numcolors; loop++)
    {
      GLuint r, g, b;
      r = lp->colors[loop].red / 256;
      g = lp->colors[loop].green / 256;
      b = lp->colors[loop].blue / 256;
      lp->pixcolors[loop] = r << 24 | g << 16 | b << 8 | 255;
    }
  free (lp->colors);
  lp->colors = NULL;

  glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth (1.0f);

  lp->pixgrid = malloc (lp->gridwidth * lp->gridheight * sizeof (int));
  lp->piximage = malloc (lp->gridwidth * lp->gridheight * sizeof (GLuint));
  if ((lp->pixgrid == NULL) || (lp->piximage == NULL))
    {
      printf ("Couldn't allocate memory, exiting\n");
      exit (EXIT_FAILURE);
    }
  memset (lp->pixgrid, 0, lp->gridheight * lp->gridwidth * sizeof (int));
  dmap = CLOVER;
  if (stripes)
    dmap = STRIPES;
  if (circle)
	  dmap = CIRCLE;
  if (plasma)
    dmap = PLASMA;
  if (bumpysquare)
    dmap = BUMPYSQUARE;
  if (bumpy)
    dmap = BUMPYROUND;
  if (clover)
    dmap = CLOVER;
  switch (dmap)
    {
    case CLOVER:
      cloverImage (lp->pixgrid, lp->numcolors, lp->gridwidth, lp->gridheight,
		   R);
      break;
    case STRIPES:
      stripesImage (lp->pixgrid, lp->gridwidth, lp->gridheight,
		    lp->numcolors);
      break;
    case CIRCLE:
      circleImage (lp->pixgrid, lp->gridwidth, lp->gridheight,
		    lp->numcolors);
      break;
    case PLASMA:
      plasmaImage (lp->pixgrid, lp->gridwidth, lp->gridheight, lp->numcolors);
      break;
    case BUMPYROUND:
      sineImage (lp->pixgrid, lp->gridwidth, lp->gridheight, lp->numcolors,
		 0);
      break;
    case BUMPYSQUARE:
      sineImage (lp->pixgrid, lp->gridwidth, lp->gridheight, lp->numcolors,
		 1);
      break;
    default:
      cloverImage (lp->pixgrid, lp->numcolors, lp->gridwidth, lp->gridheight,
		   R);
      break;
    }

  handleGLerrors ("init");
  reshape_psychedelic (mi, MI_WIDTH (mi), MI_HEIGHT (mi));
  glFlush ();
}


ENTRYPOINT void
release_psychedelic (ModeInfo * mi)
{
  if (lps)
    {
      int screen;
      for (screen = 0; screen < MI_NUM_SCREENS (mi); screen++)
	{
	  psychedelic_configuration *lp = &lps[screen];
	  free (lp->pixcolors);
	  free (lp->pixgrid);
	  free (lp->piximage);
	}
      free (lps);
      lps = 0;
    }
  FreeAllGL (mi);
}

ENTRYPOINT void
draw_psychedelic (ModeInfo * mi)
{
  psychedelic_configuration *lp = &lps[MI_SCREEN (mi)];
  Display *dpy = MI_DISPLAY (mi);
  Window window = MI_WINDOW (mi);
  int hloop = 0;
  int wloop = 0;
  long imageindex = 0;
  int cindex = 0;

  if (!lp->glx_context)
    return;
  glXMakeCurrent (MI_DISPLAY (mi), MI_WINDOW (mi), *(lp->glx_context));
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (hloop = 0; hloop < lp->gridheight; hloop++)
    {
      for (wloop = 0; wloop < lp->gridwidth; wloop++)
	{
	  imageindex = (hloop * lp->gridwidth + wloop);
	  cindex =
	    (lp->pixgrid[imageindex] + lp->coloroffset) % lp->numcolors;
	  lp->piximage[imageindex] = lp->pixcolors[cindex];
	}
    }
  glRasterPos2i (0, 0);
  /* If this doesn't work for you--and it's bound to not work for somebody,
   * considering that I don't have the equipment to test it on--
   * please contact me at the email address at the top of the file,
   * and let me know what to do to make it portable. Thank you.
   */
/* # ifdef HAVE_COCOA */
/* 	 glDrawPixels (lp->gridwidth, lp->gridheight, GL_BGRA, */
/* 		       GL_UNSIGNED_INT_8_8_8_8_REV, lp->piximage); */
/* #else */
  glDrawPixels (lp->gridwidth, lp->gridheight, GL_RGBA,
		GL_UNSIGNED_INT_8_8_8_8, lp->piximage);
/* #endif */

  if (++lp->coloroffset >= lp->numcolors)
    lp->coloroffset -= lp->numcolors;
  if (mi->fps_p)
    do_fps (mi);
  glFinish ();
  glXSwapBuffers (dpy, window);
  handleGLerrors ("draw");
}

XSCREENSAVER_MODULE ("Psychedelic", psychedelic)
#endif /* USE_GL */
/* ========================= */
/* deal with likely non-portability of packed pixels -- maybe MI_VISUAL(mi) will be useful */
/* make more maps, handle option for them better */
/* come up with a better name for "bumpy", I know I've seen this pattern before */
/* make mono a ramp of greys, not just black and white */
/* save moduloing each pixel by making color array twice the length of ncolors,
 * and repeating the colors in the second half
 */
/* apparently there is a bug in clovermap that leaves
 * some indices == numcolors, not numcolors -1
 * this can cause an out of range error if not modulo'd again
 */
