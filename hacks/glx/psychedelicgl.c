/*psychedelic-gl.c, Copyright (c) 2011 Kelley Nielsen <shegeek-dev@comcast.net>
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

# define refresh_psychedelicgl 0
# define psychedelicgl_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include "xlockmore.h"
#include "clovermap.h"
#include <math.h>


#define DEFAULTS        "*delay:	30000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL /* whole file */

#define MAX_FLYERS      1000
#define MAX_POLYSIZE   200


#define DEF_BITMAP      DEFAULTXBM
#define DEF_NCOLORS     "16"
#define DEF_SIZE        "100"
#define DEF_STEP        "200"
#define DEF_DOTS        "False"



/* typedef struct { */
/*   GLuint texid; */

/*   GLfloat color[4];  */
/*     GLfloat position[3]; */
/* } flyer; */

typedef struct {
  GLXContext *glx_context;
  XColor *colors;
  int numcolors;
  int * pixgrid;
  int gridwidth, gridheight;
  int coloroffset;

/*    flyer **flyers;  */
/*   GLfloat flyersize; */
/*   GLfloat cloudDepth; */
/*   GLfloat cloudInRadius; */
/*   GLfloat cloudOutRadius; */
/*   GLfloat zincrement; */

} psychedelicgl_configuration;

static psychedelicgl_configuration *lps = NULL;

static char * bitmap;
static int ncolors;
static int size;
static int step;
static Bool dots;

static XrmOptionDescRec opts[] = {
/*     { "-bitmap",   ".bitmap",   XrmoptionSepArg, 0 }, */
    { "-ncolors",  ".ncolors",  XrmoptionSepArg, 0 },
    { "-size",     ".size",     XrmoptionSepArg, 0 },
    { "-step",     ".step",     XrmoptionSepArg, 0 },
    {"-dots",      ".dots",     XrmoptionNoArg, "True" },
    {"+dots",      ".dots",     XrmoptionNoArg, "False" },
};

static argtype vars[] = {
/*     {&bitmap,   "bitmap",    "Bitmap",     DEF_BITMAP,    t_String}, */
    {&ncolors,  "ncolors",   "NColors",    DEF_NCOLORS,   t_Int},
    {&size,     "size",      "Size",       DEF_SIZE,      t_Int},
    {&step,     "step",      "Step",       DEF_STEP,      t_Int},
    {&dots,     "dots",      "Dots",       DEF_DOTS,      t_Bool},
};

static OptionStruct desc[] = {
/*   {"-bitmap", "path to <image>.xbm, the image to be flown"}, */
    {"-ncolors", "how many different colors the screen will use"},
    {"-size",  "relative size of the flyers on the screen"},
    {"-step",  "how far the flyers should move forward in one frame"},
};

ENTRYPOINT ModeSpecOpt psychedelicgl_opts = {countof(opts), opts, countof(vars), vars, desc};

#ifdef USE_MODULES
ModStruct flyer_description =
    {"psychedelicgl", "init_psychedelicgl", "draw_psychedelicgl", "release_psychedelicgl",
     "draw_psychedelicgl", "init_psychedelicgl", (char *)NULL, &psychedelicgl_opts,
     40000, 30, 1, 1, 64, 1.0, "",
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


/* static Bool LoadGLTextures(char xbmfile[], GLuint * temptexid) */
/* { */
/*   return False; */
/* } */



/* static void reset_flyer (flyer_configuration *lp, flyer *fl) */
/* { */
/*  } */


ENTRYPOINT void reshape_psychedelicgl (ModeInfo *mi, int width, int height)
{
/*     GLfloat h = (GLfloat) height / (GLfloat) width; */

    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
/*    height is reversed to make the viewport have the same coordinates as an x window */
    gluOrtho2D((float)0, (float)width, (float)height, (float)0); 
    glMatrixMode(GL_MODELVIEW);
    handleGLerrors("reshape");
}



/* ----- init ------------------------------------------------*/

ENTRYPOINT void init_psychedelicgl (ModeInfo *mi)
{
/*     int loop; */
    /* linear coord should be between 0.01 (large dots) and 0.1 (small dots) */
    /* constant and quadratic coords should be zero,*/
    /* too large of dots or a quadratic coord causes an appearance of */
    /* dots taking a curved path and veering away from the eye coordinate */
/*     GLfloat distparams[3]; */
    psychedelicgl_configuration *lp;



    if (!lps) {
      lps = (psychedelicgl_configuration *)
        calloc (MI_NUM_SCREENS(mi), sizeof (psychedelicgl_configuration));
      if (!lps) {
        fprintf(stderr, "%s: out of memory\n", progname);
        exit(1);
      }
      lp = &lps[MI_SCREEN(mi)];
    }

    lp = &lps[MI_SCREEN(mi)];
    lp->glx_context = init_GL(mi);

/*         lp->cloudDepth = -350.0; */
/*     lp->cloudInRadius = 2.0; */
/*     lp->cloudOutRadius = 20.0; */

    lp->gridwidth = MI_WIDTH(mi);
    lp->gridheight = MI_HEIGHT(mi);
    lp->coloroffset = 0;

 /*    if(!dots) */
/*       { */
/*     lp->flyersize = ((size > 0 && size <=  MAX_POLYSIZE) ? */
/* 		     size / 100.0 : 1.0); */
/*       } */
/*     else */
/*       { */
/*       lp->flyersize = ((size > 0 && size <=  MAX_POLYSIZE) ? */
/* 		       ((size * -0.045) + 10) / 100 : 0.05); */
/*     } */

/*     lp->numflyers = ((count > 0 && count <= MAX_FLYERS) ? count : 20 ); */
/*     lp->flyers = calloc (lp->numflyers, sizeof(flyer *)); */

    if (MI_IS_MONO(mi))
      {
	lp->numcolors = 2;
        lp->colors = calloc (1, sizeof (*lp->colors));
        lp->colors[0].pixel = MI_BLACK_PIXEL(mi);
        lp->colors[1].pixel = MI_WHITE_PIXEL(mi);
        XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[0]);
        XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[1]);
      } 
    else
      {
	lp->numcolors = ((ncolors > 0 && ncolors < NUMCOLORS) ? 
                          ncolors : 16);
    lp->colors = calloc ( lp->numcolors,  sizeof (*lp->colors));
    make_random_colormap (MI_DISPLAY(mi),MI_VISUAL(mi), MI_COLORMAP(mi),
                            lp->colors, &lp->numcolors, True, False, 0, True);
   /* add bg color to last slot, overwriting the assigned color */
      lp->colors[lp->numcolors-1].pixel = MI_BLACK_PIXEL(mi);
      XQueryColor(MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[lp->numcolors - 1]);
   }


/*     if (!dots) { */
/*     glShadeModel(GL_SMOOTH); */
/*     glEnable(GL_DEPTH_TEST); */
/*    glEnable(GL_ALPHA_TEST); */
/*    glAlphaFunc(GL_GEQUAL, 0.5); */
/*     } */
/*     else { */
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_DEPTH_TEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
/*       distparams[0] = distparams[2] = 0.0; */
/*       distparams[1] = lp->flyersize; */
/*       glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distparams); */
      glPointSize(1.0);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
/*     } */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);


     handleGLerrors("init");

/*     for (loop = 0; loop < lp->numflyers; loop++) */
/*     { */
/*       lp->flyers[loop] = malloc (sizeof (flyer )); */
/*       if (NULL == lp->flyers[loop]){ */
/* 	if (DEBUGMSG) printf("single flyer didn't get allocated\n"); */
/* 	    lp->numflyers = loop; */
/*             break; */
/* 	  } */
/* 	  reset_flyer(lp, lp->flyers[loop]); */
/*       lp->flyers[loop]->position[2] = loop * zdistribution; */
/*     } */

     lp->pixgrid = malloc(lp->gridwidth * lp->gridheight * sizeof(int));
	if (lp->pixgrid == NULL)
	  {
	    printf("Couldn't allocate memory, exiting\n");
	    exit(EXIT_FAILURE);
	  }
    	memset(lp->pixgrid, 0, lp->gridheight * lp->gridwidth * sizeof(int));
      cloverImage(lp->pixgrid, lp->numcolors, lp->gridwidth, lp->gridheight, R);
 

    reshape_psychedelicgl(mi, MI_WIDTH(mi), MI_HEIGHT(mi));

    glFlush();
}


ENTRYPOINT void release_psychedelicgl (ModeInfo *mi)
{
  if (lps) {
    int screen, loop;
    for (screen = 0; screen < MI_NUM_SCREENS(mi); screen++) {
      psychedelicgl_configuration *lp = &lps[screen];
      /* colors.h call gives bad access xlib error */
      free(lp->colors);
/*       free_colors(MI_DISPLAY(mi), MI_COLORMAP(mi), lp->colors, lp->numcolors); */
/*       for (loop = 0; loop < lp->numflyers; loop++) */
/* 	{ */
/*           free(lp->flyers[loop]); */
/* 	} */
      free(lp->pixgrid);
    }
    free (lps);
    lps = 0;
  }

  FreeAllGL(mi);
}




ENTRYPOINT void
draw_psychedelicgl (ModeInfo *mi)
{
    psychedelicgl_configuration *lp = &lps[MI_SCREEN(mi)];
    Display *dpy = MI_DISPLAY(mi);
    Window window = MI_WINDOW(mi);
/*     flyer *cflyer = NULL; */
    int hloop = 0;
    int wloop = 0;
    int cindex = 0;

    if (!lp->glx_context)
      return;

    glXMakeCurrent(MI_DISPLAY(mi), MI_WINDOW(mi), *(lp->glx_context));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


/*          glLoadIdentity(); */
/* 	 glTranslatef((float)lp->gridwidth/2, (float)(-lp->gridheight/2), 0.); */

/*    for (loop = 0; loop < lp->numflyers; loop++) { */
/*          cflyer = lp->flyers[loop]; */

/*         glPushMatrix(); */
/*         glColor4fv(cflyer->color); */
/*         glTranslatef(cflyer->position[0], cflyer->position[1], */
/*                      cflyer->position[2]); */

/* 	if (!dots) */
/* 	  { */

/*    glBegin(GL_QUADS); */
/*    glTexCoord2f(0.0, 0.0);    glVertex2f(-lp->flyersize, -lp->flyersize); */
/*    glTexCoord2f(1.0, 0.0);   glVertex2f(lp->flyersize, -lp->flyersize); */
/*    glTexCoord2f(1.0, 1.0);   glVertex2f(lp->flyersize, lp->flyersize); */
/*    glTexCoord2f(0.0, 1.0);   glVertex2f(-lp->flyersize, lp->flyersize); */
/*    glEnd(); */
/* 	  } */
/* 	else */
/* 	  { */
/* 	    glBegin(GL_POINTS); */
/*             glVertex2f(0.0, 0.0); */
/*             glEnd(); */
/*           } */

/*         glPopMatrix(); */
/* 	cflyer->position[2] += lp->zincrement; */
/*         if( cflyer->position[2] > -1.0) */
/* 	  { */
/* 	    reset_flyer(lp, lp->flyers[loop]); */
/* 	  } */
/*     } */

	 glBegin(GL_POINTS);
	 for (hloop = 0; hloop < lp->gridheight; hloop++)
	   {
	     for (wloop = 0; wloop < lp->gridwidth; wloop++)
	       {
		 cindex =((int)(lp->pixgrid[ hloop * lp->gridwidth + wloop ]) +
			      lp->coloroffset) % lp->numcolors;
		 glColor3us(lp->colors[cindex].red, lp->colors[cindex].green, lp->colors[cindex].blue);
		   glVertex2i(wloop, hloop);
	       }
	   }

	 glEnd();

	  if (lp->coloroffset++ >= lp->numcolors) lp->coloroffset -= lp->numcolors;

    if (mi->fps_p) do_fps (mi);
    glFinish();
    glXSwapBuffers(dpy, window);
    handleGLerrors("draw");

}

XSCREENSAVER_MODULE ("PsychedelicGL", psychedelicgl)

#endif /* USE_GL */

  /* -------------------------------------------------------- */

/* --------------------------------------------------- */

/* error check for:
 *
 * proper release function
 */

/* make sure all needed parts of option handling are there */

/* related: what is <hack>_description, actually? */


/* ---------------------------------------------------------- */


/* ------------------------------------------------------------ */

