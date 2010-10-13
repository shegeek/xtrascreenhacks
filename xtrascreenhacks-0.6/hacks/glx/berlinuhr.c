/*berlinuhr, Copyright (c) 2009 Kelley Nielsen <shegeek-dev@comcast.net>
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

#define release_berlinuhr 0
# define refresh_berlinuhr 0
# define berlinuhr_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include <math.h>
#include "xlockmore.h"
#include "bclockhousing.h"
#include "bclocklamps.h"

#define MINRADIUS 25
#define MAXRADIUS 35
#define MININCLINATION (30 * M_PI/180)
#define MAXINCLINATION (150 * M_PI/180)
#define MINAZIMUTH (-30 * M_PI/180)
#define MAXAZIMUTH (30 * M_PI/180)

#define DEFAULTS        "*delay:	50000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL /* whole file */

#define DEF_SIZE        "10"
#define DEF_FOG         "False"


typedef struct {
  GLXContext *glx_context;
  GLint housingindex;
  GLint lamplistindex;

  GLfloat radius, radiusincrement;
  GLfloat inclination, inclinationincrement;
  GLfloat azimuth, azimuthincrement;
  GLfloat xeye, yeye, zeye;
  GLfloat sizefactor;
  GLfloat fogdepth;
} berlinuhr_configuration;

static berlinuhr_configuration *lps = NULL;

static int size;
static int do_fog;

static XrmOptionDescRec opts[] = {
    { "-size",     ".size",     XrmoptionSepArg, 0 },
    {"-fog",       ".fog",      XrmoptionNoArg, "True" },
    {"+fog",       ".fog",      XrmoptionNoArg, "False" },
};

static argtype vars[] = {
    {&size,     "size",      "Size",       DEF_SIZE,      t_Int},
    {&do_fog,   "fog",       "Fog",        DEF_FOG,       t_Bool},
};

static OptionStruct desc[] = {
    {"-size",  "relative size of the clock on the screen"},
    {"-fog",   "Whether to make the clock darker when it is farther away"},
};

ENTRYPOINT ModeSpecOpt berlinuhr_opts = {countof(opts), opts, countof(vars), vars, desc};

#ifdef USE_MODULES
ModStruct berlinuhr_description =
    {"berlinuhr", "init_berlinuhr", "draw_berlinuhr", "release_berlinuhr",
     "draw_berlinuhr", "init_berlinuhr", (char *)NULL, &berlinuhr_opts,
     50000, 10, 1, 1, 64, 1.0, "",
     "The Berlin Quantity Didactics Clock", 0, NULL};
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

static void advance_berlinuhrview (berlinuhr_configuration *lp)
{
  if ((lp->radius < MINRADIUS * lp->sizefactor) || (lp->radius > MAXRADIUS * lp->sizefactor))
  {
    lp->radiusincrement = -lp->radiusincrement;
  }
  if ((lp->inclination <= MININCLINATION) || (lp->inclination >= MAXINCLINATION))
  {
    lp->inclinationincrement = -lp->inclinationincrement;
  }
  if ((lp->azimuth <= MINAZIMUTH) || (lp->azimuth >= MAXAZIMUTH))
  {
    lp->azimuthincrement = -lp->azimuthincrement;
  }
  lp->inclination += lp->inclinationincrement;
  lp->azimuth += lp->azimuthincrement;
  lp->radius += lp->radiusincrement;
  lp->zeye = lp->radius * sin(lp->inclination) * cos(lp->azimuth);
  lp->yeye = lp->radius * sin(lp->inclination) * sin(lp->azimuth);
  lp->xeye = lp->radius * cos(lp->inclination);  
}





ENTRYPOINT void reshape_berlinuhr (ModeInfo *mi, int width, int height)
{
    GLfloat h = (GLfloat) width / (GLfloat) height;
    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0*h, 1.5, 600.0);
    glMatrixMode(GL_MODELVIEW);
    handleGLerrors("reshape");
}



/* ----- init ------------------------------------------------*/

ENTRYPOINT void init_berlinuhr (ModeInfo *mi)
{
    float tempradius;
    int loop;
    berlinuhr_configuration *lp;
  GLint buf, samples;
    GLfloat fogColor[4] = {0,0,0,1};
  GLfloat light_color[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light_position[] = {0.0, 1.0, 1.0, 0.0};
  GLfloat lightmodel_amb[] = {0.7, 0.7, 0.7, 1.0};


    if (!lps) {
      lps = (berlinuhr_configuration *)
        calloc (MI_NUM_SCREENS(mi), sizeof (berlinuhr_configuration));
      if (!lps) {
        fprintf(stderr, "%s: out of memory\n", progname);
        exit(1);
      }
      lp = &lps[MI_SCREEN(mi)];
    }

    lp = &lps[MI_SCREEN(mi)];
    lp->glx_context = init_GL(mi);
    
    lp->sizefactor = (size >= 0 && size <= 10) ? 2.0 - (size * 0.1): 1.0;

/*    lp-> radius = ((random() % 10) + 25) * lp->sizefactor;*/
    tempradius = (random() % 10) + 25;
    lp->radius = tempradius * lp->sizefactor;
    lp->radiusincrement = (random() % 2) ? 0.1 : -0.1;
    lp->inclination = ((random() % 120) + 30) * M_PI/180;
    lp->inclinationincrement = (random() % 2) ? 0.005 : -0.005;
    lp->azimuth = ((float)(random() % 60) - 30) * M_PI/180;
    lp->azimuthincrement =  (random() % 2) ? 0.002 : -0.002;

    /* kick on the fog machine */
    lp-> fogdepth = 50.;
    if (do_fog) {
      glEnable(GL_FOG);
      glFogi(GL_FOG_MODE, GL_LINEAR);
      glFogf(GL_FOG_DENSITY, 0.02);
      glHint(GL_FOG_HINT, GL_NICEST);
      glFogf(GL_FOG_START, lp->fogdepth * 0.3 * lp->sizefactor);
     glFogf(GL_FOG_END, lp->fogdepth * lp->sizefactor);
      glFogfv(GL_FOG_COLOR, fogColor);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 
     glCullFace(GL_BACK);  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
/* this still isn't enough factor to eliminate the artifacts,
   but more just makes the aliasing and rippling worse */
    glPolygonOffset(-2., -1.);

   glGetIntegerv(GL_SAMPLE_BUFFERS, &buf);
   glGetIntegerv(GL_SAMPLES, &samples);
/*   printf("Testing for multisampling: %d, %d\n", buf, samples);*/
   if ((1 == buf) && (samples > 1))
     {
/*       printf("Multisampling on!\n");*/
       glEnable(GL_MULTISAMPLE);
     }

   lp->housingindex = makebclockhousing();
   lp->lamplistindex = makelampdlists(MI_IS_MONO(mi));
   glListBase(lp->lamplistindex);

 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lightmodel_amb);
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);

    handleGLerrors("init");
    reshape_berlinuhr(mi, MI_WIDTH(mi), MI_HEIGHT(mi));
    glFlush();
}


/* Make this be actually meaningful, rather than just letting everything get killed on exit.
 * This will involve getting access to the child lists in the helper files,
 * or finding some other solution.
 */
/*ENTRYPOINT void release_berlinuhr (ModeInfo *mi)*/
/*{*/
/*  if (lps) {*/
/*    int screen, loop;*/
/*   for (screen = 0; screen < MI_NUM_SCREENS(mi); screen++) {*/
/*      berlinuhr_configuration *lp = &lps[screen];*/
/*    }*/
/*    free (lps);*/
/*    lps = 0;*/
/*  }*/

/*  FreeAllGL(mi);*/
/*}*/




ENTRYPOINT void
draw_berlinuhr (ModeInfo *mi)
{
    berlinuhr_configuration *lp = &lps[MI_SCREEN(mi)];
    Display *dpy = MI_DISPLAY(mi);
    Window window = MI_WINDOW(mi);
  time_t rawtime;
  struct tm * displaytime;
  GLint timelamps[5];

   if (!lp->glx_context)
      return;
    glXMakeCurrent(MI_DISPLAY(mi), MI_WINDOW(mi), *(lp->glx_context));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

  advance_berlinuhrview(lp);
  gluLookAt(lp->xeye, lp->yeye, lp->zeye, 0., 0., 0., 0., 1., 0.);

  time(&rawtime);
  displaytime = localtime(&rawtime);
  timelamps[0] = (displaytime->tm_sec % 2) + SECOFFSET;
  timelamps[1] = (displaytime->tm_hour / 5) + HR5OFFSET;
  timelamps[2] = (displaytime->tm_hour % 5) + HR1OFFSET;
  timelamps[3] = (displaytime->tm_min / 5) + MIN5OFFSET;
  timelamps[4] = (displaytime->tm_min % 5) + MIN1OFFSET;
  
   glCallList(lp->housingindex);
   glEnable(GL_POLYGON_OFFSET_FILL);
  glCallLists(5, GL_INT, timelamps);
  glDisable(GL_POLYGON_OFFSET_FILL);

    if (mi->fps_p) do_fps (mi);
    glFinish();
    glXSwapBuffers(dpy, window);
    handleGLerrors("draw");
}

XSCREENSAVER_MODULE ("Berlinuhr", berlinuhr)

#endif /* USE_GL */

/* make release function meaningful */

/* looks right at about 10 fps--will probably need to
 * slow it down to make it look right with a *real* graphics card
 */

/* polish and refine panning some more */

/* how are material properties and xcolors reconciled? */

/* ? differentiate red and yellow in mono option (is currently all white/black)*/

