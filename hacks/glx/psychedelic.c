/*psychedelic.c, Copyright (c) 2011 Kelley Nielsen <shegeek-dev@comcast.net>
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

#define DEBUG 1
#define DEBUGMSG False

# define refresh_psychedelic 0
# define psychedelic_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include "xlockmore.h"
#include "clovermap.h"
#include <math.h>

#define DEFAULTS        "*delay:	30000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL /* whole file */

/* #define DEF_MAP      CLOVER */
#define DEF_NCOLORS     "64"
/* #define DEF_SIZE        "100" */
/* #define DEF_STEP        "200" */
#define DEF_SMOOTH "False"
#define DEF_CLOVER "False"
#define DEF_SIMPLE "False"
#define DEF_SIMPLE2 "False"

enum displaymap {CLOVER, SIMPLE, SIMPLE2};
/* #define NUMMAPS 3 */

typedef struct {
  GLXContext *glx_context;
  XColor *colors;
  GLuint * pixcolors;
  int numcolors;
  int * pixgrid;
  GLuint * piximage;
  int gridwidth, gridheight;
  int coloroffset;
} psychedelic_configuration;

static psychedelic_configuration *lps = NULL;

static int ncolors;
/* static int size; */
/* static int step; */
static Bool smooth;
static Bool clover;
static Bool simple;
static Bool simple2;

static XrmOptionDescRec opts[] = {
    { "-map",   ".map",   XrmoptionSepArg, 0 },
    { "-ncolors",  ".ncolors",  XrmoptionSepArg, 0 },
/*     { "-size",     ".size",     XrmoptionSepArg, 0 }, */
/*     { "-step",     ".step",     XrmoptionSepArg, 0 }, */
    {"-smooth",      ".smooth",     XrmoptionNoArg, "True" },
    {"+smooth",      ".smooth",     XrmoptionNoArg, "False" },
    {"-clover",      ".clover",     XrmoptionNoArg, "True" },
    {"+clover",      ".clover",     XrmoptionNoArg, "False" },
    {"-simple",      ".simple",     XrmoptionNoArg, "True" },
    {"+simple",      ".simple",     XrmoptionNoArg, "False" },
    {"-simple2",      ".simple2",     XrmoptionNoArg, "True" },
    {"+simple2",      ".simple2",     XrmoptionNoArg, "False" },
};

static argtype vars[] = {
/*     {&map,   "map",    "Map",     DEF_MAP,    t_Int}, */
    {&ncolors,  "ncolors",   "NColors",    DEF_NCOLORS,   t_Int},
/*     {&size,     "size",      "Size",       DEF_SIZE,      t_Int}, */
/*     {&step,     "step",      "Step",       DEF_STEP,      t_Int}, */
    {&smooth,     "smooth",      "Smooth",       DEF_SMOOTH,      t_Bool},
    {&clover,     "clover",      "Clover",       DEF_CLOVER,      t_Bool},
    {&simple,     "simple",      "Simple",       DEF_SIMPLE,      t_Bool},
    {&simple2,     "simple2",      "Simple2",       DEF_SIMPLE2,      t_Bool},
};

static OptionStruct desc[] = {
/*   {"-map", "background pattern to animate"}, */
    {"-ncolors", "how many different colors the screen will use"},
/*     {"-size",  "relative size of the flyers on the screen"}, */
/*     {"-step",  "how far the flyers should move forward in one frame"}, */
    {"-smooth", "whether to use smooth color transitions"},
    {"-clover", "whether to use the clover (default) colormap--overrides other maps"},
  {"-simple", "whether to use the simple colormap"},
  {"-simple2", "whether to use the other simple colormap"},
};

ENTRYPOINT ModeSpecOpt psychedelic_opts = {countof(opts), opts, countof(vars), vars, desc};

#ifdef USE_MODULES
ModStruct flyer_description =
    {"psychedelic", "init_psychedelic", "draw_psychedelic", "release_psychedelic",
     "draw_psychedelic", "init_psychedelic", (char *)NULL, &psychedelic_opts,
     30000, 30, 1, 1, 64, 1.0, "",
     "Various color cycling effects", 0, NULL};
#endif /* USE_MODULES */

static void handleGLerrors(char *guiltyfunction)
{
  GLenum errcode;
  const GLubyte *errstring;

    errcode = glGetError();
    if (GL_NO_ERROR != errcode)
      {
        errstring = gluErrorString(errcode);
        fprintf(stderr, "GL error in %s: %s\n", guiltyfunction, errstring);
      }
}

/* === some simple maps for testing */
static void
simpleImage(int * bits, int width, int height, int numcolors)
{
        int     i, j;

        for (i = 0; i < height; i++) {
          for (j = 0; j< width / 2; j++) {
            bits[ i * width + j ] = bits[i * width + width - j] = j % numcolors;
          }
        }
}

static void
simple2Image(int * bits, int width, int height, int numcolors)
{
        int     i, j;

        for (i = 0; i < height/2; i++) {
          for (j = 0; j< width; j++) {
/*             bits[ i * width + j ] =(i > height/2 ? numcolors -(i % numcolors) - 1 : i % numcolors);; */
            bits[ i * width + j ] = bits[(height - i) * width + j] = i % numcolors;


          }
        }
}
/* ===== end maps ============== */

ENTRYPOINT void reshape_psychedelic (ModeInfo *mi, int width, int height)
{
    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
/*    height is reversed to make the viewport have the same coordinates as an x window */
/* although it doesn't really matter, because the clover map is symmetrical */
/*     gluOrtho2D((float)0, (float)width, (float)height, (float)0);  */
    gluOrtho2D(0.,  (float)width, 0., (float)height ); 
    glMatrixMode(GL_MODELVIEW);
    handleGLerrors("reshape");
}

ENTRYPOINT void init_psychedelic (ModeInfo *mi)
{
    psychedelic_configuration *lp;
    enum displaymap dmap;
    int loop;

    if (!lps) {
      lps = (psychedelic_configuration *)
        calloc (MI_NUM_SCREENS(mi), sizeof (psychedelic_configuration));
      if (!lps) {
        fprintf(stderr, "%s: out of memory\n", progname);
        exit(1);
      }
      lp = &lps[MI_SCREEN(mi)];
    }

    lp = &lps[MI_SCREEN(mi)];
    lp->glx_context = init_GL(mi);

    lp->gridwidth = MI_WIDTH(mi);
    lp->gridheight = MI_HEIGHT(mi);
    lp->coloroffset = 0;

    if (MI_IS_MONO(mi))
      {
	lp->numcolors = 2;
/* 	lp->pixcolors = calloc(2, sizeof(GLuint)); */
/* 	lp->pixcolors[0] = 0xffffff00; */
/* 	lp->pixcolors[1] = 0x0; */
        lp->colors = calloc (2, sizeof (*lp->colors));
        lp->colors[0].pixel = MI_BLACK_PIXEL(mi);
        lp->colors[1].pixel = MI_WHITE_PIXEL(mi);
        XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[0]);
        XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[1]);
      } 
    else
      {
    lp->numcolors = ((ncolors > 1 && ncolors < NUMCOLORS) ? ncolors : 64);
    lp->colors = calloc ( lp->numcolors,  sizeof (*lp->colors));
    if (smooth)
    make_smooth_colormap (MI_DISPLAY(mi),MI_VISUAL(mi), MI_COLORMAP(mi),
                            lp->colors, &lp->numcolors, True, False, 0);
    else
    make_random_colormap (MI_DISPLAY(mi),MI_VISUAL(mi), MI_COLORMAP(mi),
                            lp->colors, &lp->numcolors, True, False, 0, True);
      
   /* add bg color to last slot, overwriting the assigned color */
      lp->colors[lp->numcolors-1].pixel = MI_BLACK_PIXEL(mi);
      XQueryColor(MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[lp->numcolors - 1]);
   }
    lp->pixcolors = calloc(lp->numcolors, sizeof(GLuint));
    if (lp->pixcolors == NULL)
      {
        fprintf(stderr, "%s: out of memory\n", progname);
        exit(1);
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
     free(lp->colors);
    lp->colors = NULL;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

     lp->pixgrid = malloc(lp->gridwidth * lp->gridheight * sizeof(int));
     lp->piximage = malloc(lp->gridwidth * lp->gridheight * sizeof(GLuint));
     if ((lp->pixgrid == NULL) || (lp->piximage == NULL))
	  {
	    printf("Couldn't allocate memory, exiting\n");
	    exit(EXIT_FAILURE);
	  }
       memset(lp->pixgrid, 0, lp->gridheight * lp->gridwidth * sizeof(int));
       dmap = CLOVER;
       if (simple) dmap = SIMPLE;
       if (simple2) dmap = SIMPLE2;
       if (clover) dmap = CLOVER;
       switch(dmap)
	 {
	 case CLOVER:
      cloverImage(lp->pixgrid, lp->numcolors, lp->gridwidth, lp->gridheight, R);
      break;
	 case SIMPLE:
	   simpleImage(lp->pixgrid, lp->gridwidth, lp->gridheight, lp->numcolors );
	   break;
	 case SIMPLE2:
	   simple2Image(lp->pixgrid, lp->gridwidth, lp->gridheight, lp->numcolors );
	   break;
	 default:
      cloverImage(lp->pixgrid, lp->numcolors, lp->gridwidth, lp->gridheight, R);
      break;
	 }

     handleGLerrors("init");
    reshape_psychedelic(mi, MI_WIDTH(mi), MI_HEIGHT(mi));
    glFlush();
}


ENTRYPOINT void release_psychedelic (ModeInfo *mi)
{
  if (lps) {
    int screen;
    for (screen = 0; screen < MI_NUM_SCREENS(mi); screen++) {
      psychedelic_configuration *lp = &lps[screen];
      free(lp->pixcolors);
      free(lp->pixgrid);
      free(lp->piximage);
    }
    free (lps);
    lps = 0;
  }
  FreeAllGL(mi);
}

ENTRYPOINT void
draw_psychedelic (ModeInfo *mi)
{
    psychedelic_configuration *lp = &lps[MI_SCREEN(mi)];
    Display *dpy = MI_DISPLAY(mi);
    Window window = MI_WINDOW(mi);
    int hloop = 0;
    int wloop = 0;
    long imageindex = 0;
    int cindex = 0;

    if (!lp->glx_context)
      return;
    glXMakeCurrent(MI_DISPLAY(mi), MI_WINDOW(mi), *(lp->glx_context));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 for (hloop = 0; hloop < lp->gridheight; hloop++)
	   {
	     for (wloop = 0; wloop < lp->gridwidth; wloop++)
	       {
		 imageindex = (hloop * lp->gridwidth + wloop);
		 cindex = (lp->pixgrid[imageindex] + lp->coloroffset) % lp->numcolors;
		 lp->piximage[imageindex] = lp->pixcolors[cindex];
	       }
	   }
	 glRasterPos2i(0, 0);
	 glDrawPixels (lp->gridwidth, lp->gridheight, GL_RGBA,
		       GL_UNSIGNED_INT_8_8_8_8, lp->piximage);

    if (++lp->coloroffset >= lp->numcolors) lp->coloroffset -= lp->numcolors;
    if (mi->fps_p) do_fps (mi);
    glFinish();
    glXSwapBuffers(dpy, window);
    handleGLerrors("draw");
}

XSCREENSAVER_MODULE ("Psychedelic", psychedelic)

#endif /* USE_GL */
 
/* ========================= */
/* deal with likely non-portability of packed pixels -- maybe MI_VISUAL(mi) will be useful */
/* document clovermap.c/h, finish man, configurator & readme accordingly */

/* make more maps, handle option for them better */
/* make mono a ramp of greys, not just black and white */

/* take black stripe out of smooth? */
/* apparently there is a bug in clovermap that leaves
 * some indices == numcolors, not numcolors -1
 * this can cause an out of range error if not modulo'd again
 */
