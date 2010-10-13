/*craters, Copyright (c) 2010 Kelley Nielsen <shegeek-dev@comcast.net>
 *
 * Based on:
 * various things found in the original code from the xscreensaver hacks
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

#include "crater4.h"

# define refresh_craters 0
# define craters_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include "xlockmore.h"
#include <ctype.h>
#include <math.h>



#define DEFAULTS        "*delay:	30000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL /* whole file */

#define MAX_CRATERS     100
#define MIN_CRATERSIZE  0.5
#define MAX_CRATERSIZE   5


#define DEF_COUNT       "20"
#define DEF_SIZE        "100"
#define DEF_STEP        "200"
#define DEF_FOG         "False"



typedef struct {

  GLfloat sizefactor; 
    GLfloat position[3];
} crater;

typedef struct {
  GLXContext *glx_context;
   crater **craters; 
  char *userxbm;
  int numcraters;
  int countcraters;
  GLfloat cratersize;

  GLfloat fogDepth;
  GLfloat groundRadius;
  GLfloat groundlevel;

  int timer;
  int ctimer;

} crater_configuration;

static crater_configuration *lps = NULL;

static int count;
static int size;
static int step;
static int do_fog;

static XrmOptionDescRec opts[] = {
    { "-count",    ".count",    XrmoptionSepArg, 0 },
    { "-size",     ".size",     XrmoptionSepArg, 0 },
    { "-step",     ".step",     XrmoptionSepArg, 0 },
    {"-fog",       ".fog",      XrmoptionNoArg, "True" },
    {"+fog",       ".fog",      XrmoptionNoArg, "False" },
};

static argtype vars[] = {
    {&count,    "count",     "Count",      DEF_COUNT,     t_Int},
    {&size,     "size",      "Size",       DEF_SIZE,      t_Int},
    {&step,     "step",      "Step",       DEF_STEP,      t_Int},
    {&do_fog,   "fog",       "Fog",        DEF_FOG,       t_Bool},
};

static OptionStruct desc[] = {
    {"-count", "maximum number of craters to form before clearing the screen"},
    {"-size",  "relative size of the craters on the screen"},
    {"-step",  "how far the ground should rotate in one frame"},
    /* put fog here? */
};

ENTRYPOINT ModeSpecOpt craters_opts = {countof(opts), opts, countof(vars), vars, desc};

#ifdef USE_MODULES
ModStruct craters_description =
    {"craters", "init_craters", "draw_craters", "release_craters",
     "draw_craters", "init_craters", (char *)NULL, &craters_opts,
     40000, 30, 1, 1, 64, 1.0, "",
     "A lunar surface being cratered", 0, NULL};
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


static crater * new_crater(crater_configuration *lp)
{
  crater * newcrater;
  float r, theta;

/*   newcrater = (struct crater *) malloc (sizeof (struct crater)); */
  newcrater =  malloc (sizeof (newcrater));
  if (NULL == newcrater) return NULL;

  r = (float)(random() % (int) lp->groundRadius);
  theta = ((float)(random() % 314)) / 50;
  newcrater->position[0] = r * cos(theta);
  newcrater->position[1] = lp->groundlevel;
  newcrater->position[2] = r * sin(theta);
  newcrater->sizefactor = 1.;

  return newcrater;
  }


ENTRYPOINT void reshape_craters (ModeInfo *mi, int width, int height)
{
    GLfloat h = (GLfloat) height / (GLfloat) width;
    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1/h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    handleGLerrors("reshape");
}



/* ----- init ------------------------------------------------*/

ENTRYPOINT void init_craters (ModeInfo *mi)
{
      GLfloat pos[4] = {0.0, 1.0, 1.0, 0.0};
      GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0};
      GLfloat dif[4] = {1.0, 1.0, 1.0, 1.0};
      GLfloat spc[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat fogColor[4] = {0., 0., 0., 1.};
    /* linear coord should be between 0.01 (large dots) and 0.1 (small dots) */
    /* constant and quadratic coords should be zero,*/
    /* too large of dots or a quadratic coord causes an appearance of */
    /* dots taking a curved path and veering away from the eye coordinate */
    GLfloat distparams[3];
    crater_configuration *lp;

    if (!lps) {
      lps = (crater_configuration *)
        calloc (MI_NUM_SCREENS(mi), sizeof (crater_configuration));
      if (!lps) {
        fprintf(stderr, "%s: out of memory\n", progname);
        exit(1);
      }
      lp = &lps[MI_SCREEN(mi)];
    }

    lp = &lps[MI_SCREEN(mi)];
    lp->glx_context = init_GL(mi);

    lp->fogDepth = -350.0;
    lp->groundRadius = 25.0;
    lp->groundlevel = -8.;

    lp->cratersize = ((size > 0 && size <=  MAX_CRATERSIZE) ?
		     size / 100.0 : 1.0);

    lp->numcraters = ((count > 0 && count <= MAX_CRATERS) ? count : 20 );
    lp->countcraters = 0;
    lp->craters = calloc (lp->numcraters, sizeof(crater *));

    lp->ctimer = lp->timer = 500;


    /* kick on the fog machine */
    if (do_fog) {
      glEnable(GL_FOG);
      glFogi(GL_FOG_MODE, GL_EXP2);
      glFogf(GL_FOG_DENSITY, 0.004);
      glHint(GL_FOG_HINT, GL_NICEST);
      glFogf(GL_FOG_START, lp->fogDepth * 0.2);
      glFogf(GL_FOG_END, lp->fogDepth);
      glFogfv(GL_FOG_COLOR, fogColor);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK); 
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_DEPTH_TEST);
      distparams[0] = distparams[2] = 0.0;
      distparams[1] = lp->cratersize;
      glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distparams);
      glPointSize(6.0);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);


/* these need refinement -- are still left from blocktube */
/* also add material properties here -- everything is lunar gray */
/* move declarations to top of fcn, else make a block */
      glLightfv(GL_LIGHT0, GL_POSITION, pos);
      glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
      glLightfv(GL_LIGHT0, GL_SPECULAR, spc);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);

    handleGLerrors("init");
    reshape_craters(mi, MI_WIDTH(mi), MI_HEIGHT(mi));
    glFlush();
}


ENTRYPOINT void release_craters (ModeInfo *mi)
{
  if (lps) {
    int screen, loop;
    for (screen = 0; screen < MI_NUM_SCREENS(mi); screen++) {
      crater_configuration *lp = &lps[screen];
      for (loop = 0; loop < lp->numcraters; loop++)
	{
          free(lp->craters[loop]);
	}
      free(lp->craters);
    }
    free (lps);
    lps = 0;
  }
  FreeAllGL(mi);
}




ENTRYPOINT void
draw_craters (ModeInfo *mi)
{
    crater_configuration *lp = &lps[MI_SCREEN(mi)];
    Display *dpy = MI_DISPLAY(mi);
    Window window = MI_WINDOW(mi);
    crater *ccrater = NULL;
    int loop = 0;

    if (!lp->glx_context)
      return;

    glXMakeCurrent(MI_DISPLAY(mi), MI_WINDOW(mi), *(lp->glx_context));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


         glLoadIdentity();


	 /* draw the ground */

	 /* loop over the craters */
   for (loop = 0; loop < lp->countcraters; loop++) {
         ccrater = lp->craters[loop];

        glPushMatrix();
	 glRotatef(90., -1., 0., 0.);
        glTranslatef(ccrater->position[0], ccrater->position[1],
                     ccrater->position[2]);

	/* draw the crater */
	drawcrater4();

        glPopMatrix();


   }
	/* advance ground rotation */

   lp->ctimer--;
        if( lp->ctimer < 0)
	  {
	    lp->craters[lp->countcraters] = new_crater(lp);
	    lp->countcraters++;
	    /* check allocation, count of craters against max */
	    lp->ctimer = lp->timer;
	  }
    

    if (mi->fps_p) do_fps (mi);
    glFinish();
    glXSwapBuffers(dpy, window);
    handleGLerrors("draw");

}

XSCREENSAVER_MODULE ("CRATERS", craters)

#endif /* USE_GL */

  /* -------------------------------------------------------- */

/* --------------------------------------------------- */

