/*craters.c, Copyright (c) 2010 Kelley Nielsen <shegeek-dev@comcast.net>
 * part of the xtrascreenhacks package
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


# define refresh_craters 0
# define craters_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include "xlockmore.h"
#include <math.h>
#include <stdbool.h>
#include "erase.h"
#include "crater4.h"
#include "ejecta.h"



#define DEFAULTS        "*delay:	50000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL			/* whole file */

#define MAX_CRATERS     100
#define MIN_CRATERSIZE  0.5
#define MAX_CRATERSIZE   5


#define DEF_COUNT       "20"
#define DEF_STEP        "10"
#define DEF_WAITTIME    "10"



typedef struct
{
  GLfloat sizefactor;
  GLfloat position[3];
} crater;

typedef struct
{
  GLXContext *glx_context;
  crater **craters;
  int numcraters;
  int countcraters;

  GLfloat fogDepth;
  GLfloat groundRadius;
  GLfloat groundlevel;
  GLuint craterdlist, grounddlist;
  GLuint stardlist;

  int timer;
  int wtimer;

  eraser_state *eraser;
  bool erasing;
  ejemitter *eruption;
  bool erupting;
  int eruptframes;
  double eruptincrement;
  bool impacting;
  int impactframes, impactframecount;

} crater_configuration;

static crater_configuration *lps = NULL;

static int count;
static int step;
static int waittime;

static XrmOptionDescRec opts[] = {
  {"-count", ".count", XrmoptionSepArg, 0},
  {"-step", ".step", XrmoptionSepArg, 0},
  {"-waittime", ".waittime", XrmoptionSepArg, 0},
};

static argtype vars[] = {
  {&count, "count", "Count", DEF_COUNT, t_Int},
  {&step, "step", "Step", DEF_STEP, t_Int},
  {&waittime, "waittime", "waittime", DEF_WAITTIME, t_Int},
};

static OptionStruct desc[] = {
  {"-count", "maximum number of craters to form before clearing the screen"},
  {"-step", "how far the ejecta should fly in one frame"},
  {"-waittime", "how long to wait between each crater"},
};

ENTRYPOINT ModeSpecOpt craters_opts =
  { countof (opts), opts, countof (vars), vars, desc };

#ifdef USE_MODULES
ModStruct craters_description =
  { "craters", "init_craters", "draw_craters", "release_craters",
  "draw_craters", "init_craters", (char *) NULL, &craters_opts,
  40000, 30, 1, 1, 64, 1.0, "",
  "A lunar surface being cratered", 0, NULL
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

static void
errorCallback (GLenum errorCode)
{
  const GLubyte *estring;

  estring = gluErrorString (errorCode);
  fprintf (stderr, "Quadric Error: %s\n", estring);
  exit (0);
}


GLuint
make_starfield (double distance)
{
  GLfloat xpos, ypos;
  int itor, numstars = 80;
  GLuint stf;
  GLdouble stdistance = -60.;

  stf = glGenLists (1);
  glNewList (stf, GL_COMPILE);
  glPointSize (2.0);
   glBegin (GL_POINTS);
  for (itor = 0; itor < numstars; itor++)
  {
    xpos = (float)(random() % 400) / 10. - 20.;
    ypos = (float)(random() % 190) / 10. - 4.;
  glVertex3f (xpos, ypos, stdistance);
  }
  glEnd ();
  glPointSize (10.0);
  glEndList ();
  return stf;
}


static crater *
new_crater (crater_configuration * lp)
{
  crater *newcrater;
  float r, theta;

  newcrater = malloc (sizeof (newcrater));
  if (NULL == newcrater)
    return NULL;

  r = (float) (random () % (int) (lp->groundRadius - 10)) + 7.;
  /* 90 +/- 45 degrees, converted to radians, 
   * minus a little to keep the craters more fully in the frame
   */
  theta = ((float) (random () % 96) / 100.) + (1.047197551);
  newcrater->position[0] = r * cos (theta);
  newcrater->position[2] = lp->groundlevel;
  newcrater->position[1] = r * sin (theta);
  newcrater->sizefactor = ((float) (random () % 200) / 100.) + 4.;

  return newcrater;
}


ENTRYPOINT void
reshape_craters (ModeInfo * mi, int width, int height)
{
  GLfloat h = (GLfloat) height / (GLfloat) width;
  glViewport (0, 0, (GLint) width, (GLint) height);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (30.0, 1 / h, 1.0, 500.0);
  glMatrixMode (GL_MODELVIEW);
  handleGLerrors ("reshape");
}



/* ----- init ------------------------------------------------*/

ENTRYPOINT void
init_craters (ModeInfo * mi)
{
  GLfloat specular[] = { 1., 1., 1., 1. };
  GLfloat color[4] = { 0.5, 0.5, 0.5, 1. };
  GLfloat pos[4] = { 5.0, 1.2, 5., 0.0 };
  GLfloat amb[4] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat dif[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat spc[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat pointdistparams[3];
  GLUquadricObj *groundquad;
  crater_configuration *lp;
  double tstep;
  int loop;

  if (!lps)
    {
      lps = (crater_configuration *)
	calloc (MI_NUM_SCREENS (mi), sizeof (crater_configuration));
      if (!lps)
	{
	  fprintf (stderr, "%s: out of memory\n", progname);
	  exit (1);
	}
      lp = &lps[MI_SCREEN (mi)];
    }

  lp = &lps[MI_SCREEN (mi)];
  lp->glx_context = init_GL (mi);

  lp->groundRadius = 50.0;
  lp->groundlevel = -6.;


  lp->numcraters = ((count > 0 && count <= MAX_CRATERS) ? count : 20);
  lp->countcraters = 0;
  lp->craters = calloc (lp->numcraters, sizeof (crater *));
  for (loop = 0; loop < lp->numcraters; loop++)
    lp->craters[loop] = NULL;

  lp->wtimer = lp->timer = ((waittime > 0 && waittime <= 200) ? waittime : 10);
  lp->erasing = false;
  tstep = ((step > 0 && step <= 20) ? ((float)step / 1000.) : 0.01);
  lp->eruption = init_ejemitter (5000, tstep, 25.);
  lp->erupting = false;
  lp->impacting = false;
  lp->impactframes = lp->impactframecount = 2;

  groundquad = gluNewQuadric ();
  gluQuadricCallback (groundquad, GLU_ERROR, errorCallback);
  gluQuadricDrawStyle (groundquad, GLU_FILL);
  gluQuadricNormals (groundquad, GLU_SMOOTH);
  lp->craterdlist = glGenLists (1);
  glNewList (lp->craterdlist, GL_COMPILE);
  drawcrater4 ();
  glEndList ();
  lp->grounddlist = glGenLists (1);
  glNewList (lp->grounddlist, GL_COMPILE);
  gluDisk (groundquad, 0., lp->groundRadius, 45, 15);
  glEndList ();
  gluDeleteQuadric (groundquad);

  glShadeModel (GL_SMOOTH);
  glEnable (GL_CULL_FACE);
  glCullFace (GL_BACK);
  glEnable (GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable (GL_DEPTH_TEST);
  pointdistparams[0] = pointdistparams[2] = 0.0;
  pointdistparams[1] = 0.3;
  glPointParameterfv (GL_POINT_DISTANCE_ATTENUATION, pointdistparams);
/*  glPointSize (10.0);*/
  glLineWidth(2.);
  glEnable(GL_LINE_STIPPLE);
  glLineStipple(1, 43690);
  glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

  lp->stardlist = make_starfield (lp->groundRadius);

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glClearDepth (1.0);

  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialf (GL_FRONT, GL_SHININESS, 0.3 * 128.0);

  glLightfv (GL_LIGHT0, GL_POSITION, pos);
  glLightfv (GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv (GL_LIGHT0, GL_SPECULAR, spc);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);

  reshape_craters (mi, MI_WIDTH (mi), MI_HEIGHT (mi));
  glFlush ();
  handleGLerrors ("init");
}


ENTRYPOINT void
release_craters (ModeInfo * mi)
{
  if (lps)
    {
      int screen, loop;
      for (screen = 0; screen < MI_NUM_SCREENS (mi); screen++)
	{
	  crater_configuration *lp = &lps[screen];
	  if ((lp != NULL) && (lp->numcraters != 0))
	    {
	      for (loop = 0; loop < lp->numcraters; loop++)
		{
		  if (lp->craters[loop] != NULL)
		    {
		      free (lp->craters[loop]);
		    }
		}
	      if (lp->craters != NULL) free (lp->craters);
	      delete_ejemitter (lp->eruption);
	    }
	}
      free (lps);
      lps = 0;
    }
  FreeAllGL (mi);
}



ENTRYPOINT void
draw_craters (ModeInfo * mi)
{
  crater_configuration *lp = &lps[MI_SCREEN (mi)];
  Display *dpy = MI_DISPLAY (mi);
  Window window = MI_WINDOW (mi);
  crater *ccrater = NULL;
  GLfloat starglow[] = { 1., 1., 1., 1. };
  GLfloat normal[] = { 0., 0., 0., 1. };
  int loop = 0;

  if (!lp->glx_context)
    return;

  if ((lp->erasing) || (lp->eraser))
    {
      lp->eraser = erase_window (dpy, window, lp->eraser);
      lp->erasing = false;
      glXWaitX ();
    }

  else
    {
      glXMakeCurrent (MI_DISPLAY (mi), MI_WINDOW (mi), *(lp->glx_context));

      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      glLoadIdentity ();
       glMaterialfv(GL_FRONT, GL_EMISSION, starglow);
             glCallList (lp->stardlist);
      glTranslatef (0., 0., -20);


      /* loop over the craters */
      for (loop = 0; loop < lp->countcraters; loop++)
	{
	  ccrater = lp->craters[loop];

	  glPushMatrix ();
	  glRotatef (90., -1., 0., 0.);
	  glTranslatef (ccrater->position[0], ccrater->position[1],
			ccrater->position[2]);
   	  glScalef (ccrater->sizefactor, ccrater->sizefactor,
		    ccrater->sizefactor);

       /* draw the streak of the impacting meteor */
      if ((lp->impacting == true) && (loop == lp->countcraters - 1))
      {
        glMaterialfv(GL_FRONT, GL_EMISSION, starglow);
        glBegin(GL_LINES);
        glVertex3f(0., 0., 0.);
        glVertex3f(2., 0., 5.);
         glEnd();
        lp->impactframecount--;
         if (lp->impactframecount == 0) lp->impacting = false; 
        }
        
       glMaterialfv(GL_FRONT, GL_EMISSION, normal);


  	  if (loop == lp->countcraters - 1)
	    {
	      glPushMatrix ();
	      glTranslatef (0., 0., lp->eruptframes * lp->eruptincrement);
	      glCallList (lp->craterdlist);
	      glPopMatrix ();
	      if (lp->erupting == true)
		{
		  glRotatef (-90., -1., 0., 0.);
		  lp->erupting = spew_ejecta (lp->eruption);
		  if (lp->eruptframes > 0)
		    lp->eruptframes--;
		}
	    }
	  else
	    glCallList (lp->craterdlist);
	  glPopMatrix ();
}
      /* draw the ground */
        glMaterialfv(GL_FRONT, GL_EMISSION, normal);
             glPushMatrix ();
      glRotatef (90., -1., 0., 0.);
      glTranslatef (0., 0., lp->groundlevel);
      glCallList (lp->grounddlist);
      glPopMatrix ();
      




      if (lp->erupting == false)
	lp->wtimer--;

      if (lp->wtimer < 0)
	{
	  lp->wtimer = lp->timer;
	  lp->countcraters++;
	  lp->impacting = true;
	  lp->impactframecount = lp->impactframes;
	  if (lp->countcraters > lp->numcraters)
	    {
	      for (loop = 0; loop < lp->countcraters - 1; loop++)
		{
		  free (lp->craters[loop]);
		  lp->craters[loop] = NULL;
		}
	      lp->countcraters = 0;
	      lp->erasing = true;
	      glXWaitGL ();
	    }
	  else
	    {
	      lp->craters[lp->countcraters - 1] = new_crater (lp);
	      lp->eruptframes = reset_ejemitter (lp->eruption, 0.6);
	      lp->erupting = true;
	      lp->eruptincrement = -0.18 / (double) lp->eruptframes;
	    }
	}
	
      if (mi->fps_p)
	do_fps (mi);
      glFinish ();
      glXSwapBuffers (dpy, window);
      handleGLerrors ("draw");

    }				/* drawing (i.e. not erasing) case */

}

XSCREENSAVER_MODULE ("CRATERS", craters)
#endif /* USE_GL */
  /* -------------------------------------------------------- */
/* modelview matrix manipulation is inefficient,
 *  probably because crater and ejecta are oriented 90 degrees from each other
 *
 * pass in MI for star and crater distribution
 *
 * solve the problem of craters intersecting
 *
 * make shadows for the craters
 *
 * there is an inherent problem with what I am trying to display...
 * if there is any difference in final color between ground, crater walls and ejecta
 * it looks funny, but if there isn't, nothing can be seen.
 * With real moon craters, there are shadows, but it's still difficult for the eye
 * and doesn't lend itself to an attractive animation.
 */
