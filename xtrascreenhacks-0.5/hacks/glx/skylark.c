/*skylark, Copyright (c) 2010 Kelley Nielsen <shegeek-dev@comcast.net>
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

#define WINSIZE 5

# define refresh_skylark 0
# define skylark_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include "xlockmore.h"
#include <ctype.h>
#include <math.h>

#include "boomerangs.h"



#define DEFAULTS        "*delay:	30000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL /* whole file */

#define MAX_BOOMS      1000
#define MAX_BOOMSIZE   200


#define DEF_COUNT       "100"
#define DEF_NCOLORS     "128"
#define DEF_COLORCHANGERATE "10"
#define DEF_SPINRATE        "10"
#define DEF_SIZE        "100"
#define DEF_STEP        "200"



typedef struct {
/*   GLfloat color[4];  */
  GLfloat pathcontrol[6];
  GLfloat position[3];
  GLfloat tposition;
  GLfloat rotangle;
  GLfloat rotincrement;
  int index;
} boomerang;

typedef struct {
  GLXContext *glx_context;
  GLUnurbsObj *boomnurb;
   boomerang **booms; 
  int numbooms;
  XColor *colors;
  int numcolors;
  GLfloat bgcolor[3];
  int bgcolorindex;
  GLfloat boomsize;

  int ctimer;
  int cduration;
  GLfloat tincrement;
  GLfloat aincrement;
  float winaspect;

} skylark_configuration;

static skylark_configuration *lps = NULL;

static int count;
static int ncolors;
static int colorchangerate;
static int spinrate;
static int size;
static int step;

static XrmOptionDescRec opts[] = {
    { "-count",    ".count",    XrmoptionSepArg, 0 },
    { "-ncolors",  ".ncolors",  XrmoptionSepArg, 0 },
  {"-colorchangerate", ".colorchangerate", XrmoptionSepArg, 0},
    { "-spinrate",     ".spinrate",     XrmoptionSepArg, 0 },
     { "-size",     ".size",     XrmoptionSepArg, 0 },
    { "-step",     ".step",     XrmoptionSepArg, 0 },
};

static argtype vars[] = {
    {&count,    "count",     "Count",      DEF_COUNT,     t_Int},
    {&ncolors,  "ncolors",   "NColors",    DEF_NCOLORS,   t_Int},
  {&colorchangerate, "colorchangerate", "Colorchangerate", DEF_COLORCHANGERATE, t_Int},
    {&spinrate,     "spinrate", "Spinrate",  DEF_SPINRATE,   t_Int},
    {&size,     "size",      "Size",       DEF_SIZE,      t_Int},
    {&step,     "step",      "Step",       DEF_STEP,      t_Int},
};

static OptionStruct desc[] = {
    {"-count", "how many flyers appear on the screen at the same time"},
    {"-ncolors", "how many different colors the screen will use"},
  {"-colorchangerate", "how fast the background color should cycle"},
    {"-spinrate",  "how far the boomerangs should rotate in one frame"},
      {"-size",  "relative size of the flyers on the screen"},
    {"-step",  "how far the flyers should move forward in one frame"},
};

ENTRYPOINT ModeSpecOpt skylark_opts = {countof(opts), opts, countof(vars), vars, desc};

#ifdef USE_MODULES
ModStruct skylark_description =
    {"skylark", "init_skylark", "draw_skylark", "release_skylark",
     "draw_skylark", "init_skylark", (char *)NULL, &skylark_opts,
     40000, 30, 1, 1, 64, 1.0, "",
     "An animated 1950s kitchen table top", 0, NULL};
#endif /* USE_MODULES */

/* -- error handling ---------------------------------------------- */

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

static void nurbsError(GLenum errorcode)
{
  const GLubyte *errorstring;
  errorstring = gluErrorString(errorcode);
  fprintf (stderr, "Nurbs error: %s\n", errorstring);
  exit(0);
}

/* ------------------------------------------------------ */

static void advance_color (skylark_configuration *lp)
{
  XColor tempcolor;

  lp->ctimer = lp->cduration;
  lp->bgcolorindex++;
  if (lp->bgcolorindex >= lp->numcolors )
    {
      lp->bgcolorindex -= (lp->numcolors );
    }
  tempcolor = (lp->colors[lp->bgcolorindex]);
  lp->bgcolor[0] = tempcolor.red / 65536.0;
  lp->bgcolor[1] = tempcolor.green / 65536.0;
  lp->bgcolor[2] = tempcolor.blue / 65536.0;
  glClearColor(lp->bgcolor[0], lp->bgcolor[1], lp->bgcolor[2], 0.0f);
}


static void advance_boomerang (skylark_configuration *lp, boomerang *b)
{

  b->tposition += lp->tincrement;
  if (b->tposition >= 6.28) b->tposition -= 6.28;
  b->position[0] = (b->pathcontrol[0] * cos(b->pathcontrol[1]*b->tposition)) +
		    b->pathcontrol[2];
  b->position[1] = (b->pathcontrol[3] * sin(b->pathcontrol[4]*b->tposition)) +
                   b->pathcontrol[5];

  b->rotangle += b->rotincrement;
  if ((b->rotincrement > 0) && (b->rotangle > 360.0)) b->rotangle -= 360.0;
  if ((b->rotincrement < 0) && (b->rotangle < 360.0)) b->rotangle += 360.0;

}


static boomerang *
new_boomerang(skylark_configuration *lp)
{
  boomerang * newboom;

  newboom =  malloc (sizeof (boomerang));
  if (NULL == newboom) return NULL;

  newboom->index = random() % NUMBOOMS;

  /* Variables controlling the lissajous paths for the boomerangs: 
   * 0 and 3 control scaling in x and y respectively;
   * 1 and 4 are parameters for the sine and cosine functions,
   * 2 and 5 shift the path
   */
  newboom->pathcontrol[0] = 1. * WINSIZE * lp->winaspect;
  newboom->pathcontrol[1] = (GLfloat)((random() % 4) * 2 + 1);
  newboom->pathcontrol[2] = ((float)(random() % 10) - 4.) / 5.;
  newboom->pathcontrol[3] = 1. * WINSIZE;
  newboom->pathcontrol[4] = newboom->pathcontrol[1] + 1.;
  newboom->pathcontrol[5] = ((float)(random() % 10) - 4.) / 5.;
  newboom->tposition = ((random() % 628) / 100.);

  newboom->rotincrement = lp->aincrement * (random() % 5) + 1.;
  if (random() % 2 == 0) newboom->rotincrement *= -1;

  /* get aspect ratio and extend ranges to sides of screen */
/*   newboom->position[0] = (GLfloat)(random() % (WINSIZE * 50)) / 50.0; */
/*   if (random() % 2) newboom->position[0] *= -1; */
/*   newboom->position[1] = (GLfloat)(random() % (WINSIZE * 50)) / 50.0; */
/*   if (random() % 2) newboom->position[1] *= -1; */
  newboom->position[2] = 0.0;
  newboom->rotangle = (GLfloat)(random() % 360);

  advance_boomerang(lp, newboom);

/*   printf("Leaving constructor\n"); */
  printf("Control array: %f, %f, %f, %f, %f, %f\n", newboom->pathcontrol[0], newboom->pathcontrol[1], newboom->pathcontrol[2], newboom->pathcontrol[3], newboom->pathcontrol[4], newboom->pathcontrol[5]);
/*   printf("Position: %f, %f, t: %f\n", newboom->position[0], newboom->position[1], newboom->tposition); */
  return newboom;

  }






/* ENTRYPOINT void reshape_skylark (ModeInfo *mi, int width, int height); */
ENTRYPOINT void reshape_skylark (ModeInfo *mi, int width, int height)
{
    GLfloat aspect = (GLfloat) width / (GLfloat) height;

    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WINSIZE * aspect, WINSIZE * aspect, -WINSIZE, WINSIZE);
    glMatrixMode(GL_MODELVIEW);
    handleGLerrors("reshape");
}



/* ----- init ------------------------------------------------*/

ENTRYPOINT void init_skylark (ModeInfo *mi)
{
    int loop;
/*     GLfloat distparams[3]; */
    skylark_configuration *lp;



    if (!lps) {
      lps = (skylark_configuration *)
        calloc (MI_NUM_SCREENS(mi), sizeof (skylark_configuration));
      if (!lps) {
        fprintf(stderr, "%s: out of memory\n", progname);
        exit(1);
      }
      lp = &lps[MI_SCREEN(mi)];
    }

    lp = &lps[MI_SCREEN(mi)];
    lp->glx_context = init_GL(mi);

  if (!lp->boomnurb)
  {
    lp->boomnurb = gluNewNurbsRenderer();
    gluNurbsProperty(lp->boomnurb, GLU_SAMPLING_TOLERANCE, 5.0f);
/*     gluNurbsProperty(lp->boomnurb, GLU_DISPLAY_MODE, GLU_OUTLINE_POLYGON); */
    gluNurbsProperty(lp->boomnurb, GLU_DISPLAY_MODE, (GLfloat)GLU_FILL);
    gluNurbsCallback(lp->boomnurb, GLU_NURBS_ERROR, (GLvoid (*) ()) nurbsError);
  }
  glLineWidth(3.0);
 
  /* this is awkward--fix */
     lp->tincrement = ((step > 0 && step <= 1000) ? step / 1000000.0 : 0.02);
     printf("tincrement: %f\n", lp->tincrement);
      lp->aincrement = ((spinrate >= 0 && spinrate <= 100) ? (spinrate / 50.0) : 2.0);
      lp->winaspect = (float)MI_WIDTH(mi) / (float)MI_HEIGHT(mi);
 
    lp->boomsize = ((size > 0 && size <=  MAX_BOOMSIZE) ?
		     size / 100.0 : 1.0);

    lp->numbooms = ((count > 0 && count <= MAX_BOOMS) ? count : 100 );
    lp->booms = calloc (lp->numbooms, sizeof(boomerang *));

    if (MI_IS_MONO(mi))
      {
	lp->numcolors = 1;
        lp->colors = calloc (1, sizeof (*lp->colors));
        lp->colors[0].pixel = MI_BLACK_PIXEL(mi);
        XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[0]);
      } 
    else
      {
	lp->numcolors = ((ncolors > 0 && ncolors < NUMCOLORS) ? 
                          ncolors : 128);
    lp->colors = calloc ( lp->numcolors,  sizeof (*lp->colors));
    make_smooth_colormap (MI_DISPLAY(mi),MI_VISUAL(mi), MI_COLORMAP(mi),
                            lp->colors, &lp->numcolors, False, 0, True);
    }
    lp->bgcolorindex = random() % lp->numcolors;
    lp->cduration = ((colorchangerate > 0 && colorchangerate <= 1000) ? 
		     colorchangerate: 10);
    advance_color(lp);


    glShadeModel(GL_SMOOTH);
 /*    glEnable(GL_DEPTH_TEST); */
/*     glEnable(GL_CULL_FACE);  */
/*    glEnable(GL_ALPHA_TEST); */
/*    glAlphaFunc(GL_GEQUAL, 0.5); */
/*       glEnable(GL_POINT_SMOOTH); */
/*       glEnable(GL_DEPTH_TEST); */
/*       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */
/*       distparams[0] = distparams[2] = 0.0; */
/*       distparams[1] = lp->boomsize; */
/*       glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distparams); */
/*       glPointSize(6.0); */
/*       glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); */

    glClearDepth(1.0f);

    handleGLerrors("init");

    for (loop = 0; loop < lp->numbooms; loop++)
    {
      lp->booms[loop] = new_boomerang(lp);
      if (NULL == lp->booms[loop]){
	if (DEBUGMSG) printf("single boomerang didn't get allocated\n");
	    lp->numbooms = loop - 1;
            break;  
	  }
    }
    reshape_skylark(mi, MI_WIDTH(mi), MI_HEIGHT(mi));

    glFlush();

    printf("Leaving init\n");
}


ENTRYPOINT void release_skylark (ModeInfo *mi)
{
/*   if (lps) { */
/*     int screen, loop; */
/*     for (screen = 0; screen < MI_NUM_SCREENS(mi); screen++) { */
/*       skylark_configuration *lp = &lps[screen]; */
      /* colors.h call gives bad access xlib error */
/*       free(lp->colors); */
/*       free_colors(MI_DISPLAY(mi), MI_COLORMAP(mi), lp->colors, lp->numcolors); */
/*       for (loop = 0; loop < lp->numflyers; loop++) */
/* 	{ */
/*           free(lp->flyers[loop]); */
/* 	} */
/*       free(lp->flyers); */
/*     } */
/*     free (lps); */
/*     lps = 0; */
/*   } */

/*   FreeAllGL(mi); */
}




ENTRYPOINT void
draw_skylark (ModeInfo *mi)
{
    skylark_configuration *lp = &lps[MI_SCREEN(mi)];
    Display *dpy = MI_DISPLAY(mi);
    Window window = MI_WINDOW(mi);
    boomerang *cboom = NULL;
    int loop = 0;
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat black[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat grey[] = {1.0, 1.0, 1.0, 1.0};
   if (!MI_IS_MONO(mi)) 
      { 
    black[0] = black[1] = black[2] = 0.; 
    grey[0] = grey[1] = grey[2] = 0.6; 
      } 

    if (!lp->glx_context)
      return;

    glXMakeCurrent(MI_DISPLAY(mi), MI_WINDOW(mi), *(lp->glx_context));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


         glLoadIdentity();

   for (loop = 0; loop < lp->numbooms; loop++) {
     cboom = lp->booms[loop];

        glPushMatrix();
        glTranslatef(cboom->position[0] - 0.1, cboom->position[1],
                     cboom->position[2]);
	glRotatef(cboom->rotangle, 0., 0., 1.);

/* 	if (!dots) */
/* 	  { */
/*       glBindTexture(GL_TEXTURE_2D, cflyer->texid);  */

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

/*         glScalef(1.0, 1.0, 1.); */
         glColor4fv(grey);

       gluBeginCurve(lp->boomnurb);
        gluNurbsCurve(lp->boomnurb, nKnots2, Knots2, kStride,
                      &pointsWeights2[cboom->index][0][0], 
                      order2, GL_MAP1_VERTEX_4);
        gluEndCurve(lp->boomnurb);

	glPopMatrix();
	glPushMatrix();

        glTranslatef(cboom->position[0] + 0.1, cboom->position[1],
                     cboom->position[2]);
	glRotatef(cboom->rotangle, 0., 0., 1.);
	glColor4fv(white);
       gluBeginCurve(lp->boomnurb);
        gluNurbsCurve(lp->boomnurb, nKnots2, Knots2, kStride,
                      &pointsWeights2[cboom->index][0][0], 
                      order2, GL_MAP1_VERTEX_4);
        gluEndCurve(lp->boomnurb);


        glPopMatrix();
	advance_boomerang(lp, cboom);
    }

	lp->ctimer--;
	if (lp->ctimer <= 0) advance_color(lp);
    if (mi->fps_p) do_fps (mi);
    glFinish();
    glXSwapBuffers(dpy, window);
    handleGLerrors("draw");

}

XSCREENSAVER_MODULE ("Skylark", skylark)

#endif /* USE_GL */

  /* -------------------------------------------------------- */

/* --------------------------------------------------- */

/* count of control point sets is same as count of curves on screen--
 * in fact, all terminology from practice piece needs changed 
 */


/* move increments inside individual boomerangs, give each a unique one */

/* cycle drawing order so that the same boomerangs aren't always on top */

/* xlockmore.h restricts number of colors to 256--is this an issue? 
*  (color steps, at present, are visible)
*/

/* is window width and height available outside of modeinfo,
 * to accurately resize paths after a window resize?
 * in other words, can I readjust paths after init has run?
 */

/* refine paths, esp. do offsets--booms are bunching up in corners */

/* screen becomes pixel filled before there is a 
 *high enough boomerang count to look good (probably my machine's problem)
 */
