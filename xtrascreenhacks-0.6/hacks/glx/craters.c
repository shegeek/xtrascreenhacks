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
#define DEF_SIZE        "100"  /* needed? */
#define DEF_STEP        "200"  /* needed? */
#define DEF_FOG         "False"



typedef struct
{

  GLfloat sizefactor;
  GLfloat position[3];
} crater;

typedef struct
{
  GLXContext *glx_context;
  crater **craters;
  char *userxbm;
  int numcraters;
  int countcraters;
  GLfloat cratersize;

  GLfloat fogDepth;
  GLfloat groundRadius;
  GLfloat groundlevel;
  GLuint craterdlist, grounddlist;

  int timer;
  int ctimer;

  eraser_state *eraser;
  bool erasing;
  ejemitter * eruption;
  bool erupting;

} crater_configuration;

static crater_configuration *lps = NULL;

static int count;
static int size;
static int step;
static int do_fog;

static XrmOptionDescRec opts[] = {
  {"-count", ".count", XrmoptionSepArg, 0},
  {"-size", ".size", XrmoptionSepArg, 0},
  {"-step", ".step", XrmoptionSepArg, 0},
  {"-fog", ".fog", XrmoptionNoArg, "True"},
  {"+fog", ".fog", XrmoptionNoArg, "False"},
};

static argtype vars[] = {
  {&count, "count", "Count", DEF_COUNT, t_Int},
  {&size, "size", "Size", DEF_SIZE, t_Int},
  {&step, "step", "Step", DEF_STEP, t_Int},
  {&do_fog, "fog", "Fog", DEF_FOG, t_Bool},
};

static OptionStruct desc[] = {
  {"-count", "maximum number of craters to form before clearing the screen"},
  {"-size", "relative size of the craters on the screen"},
  {"-step", "how far the ground should rotate in one frame"},
  /* put fog here? */
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

static void errorCallback(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf(stderr, "Quadric Error: %s\n", estring);
   exit(0);
}


static crater *
new_crater (crater_configuration * lp)
{
  crater *newcrater;
  float r, theta;

/*   newcrater = (struct crater *) malloc (sizeof (struct crater)); */
  newcrater = malloc (sizeof (newcrater));
  if (NULL == newcrater)
    return NULL;

  r = (float) (random () % (int) (lp->groundRadius - 10)) + 7.;
  /* 90 +/- 45 degrees, converted to radians, 
   * minus a little to keep the craters fully in the frame
   */
  theta = ((float) (random () % 96) / 100.) + (1.047197551);
  newcrater->position[0] = r * cos (theta);
  newcrater->position[2] = lp->groundlevel;
  newcrater->position[1] = r * sin (theta);
  newcrater->sizefactor = ((float) (random () % 200) / 100.) + 4.;

/*   { */
/*     float distance = sqrt(newcrater->position[0] * newcrater->position[0] +newcrater->position[1] * newcrater->position[1] + newcrater->position[2] * newcrater->position[2]); */
/*   printf ("New crater: radius = %f, angle = %f, position = %f, %f  distance from origin = %f\n", */
/* 	  r, theta * 180. / M_PI, newcrater->position[0], newcrater->position[1], distance); */
/*   } */

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
  GLfloat specular[] = { 0.5, 0.5, 0.5, 1. };
  GLfloat color[4] = { 0.5, 0.5, 0.5, 1. };
  /* change light position back to coming from side when done with ejecta */
  GLfloat pos[4] = { 5.0, 5.0, 0., 0.0 };
  GLfloat amb[4] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat dif[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat spc[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat fogColor[4] = { 0., 0., 0., 1. };
  /* linear coord should be between 0.01 (large dots) and 0.1 (small dots) */
  /* constant and quadratic coords should be zero, */
  /* too large of dots or a quadratic coord causes an appearance of */
  /* dots taking a curved path and veering away from the eye coordinate */
  GLfloat distparams[3];
  GLUquadricObj * groundquad;
  crater_configuration *lp;
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

  lp->fogDepth = -350.0;
  lp->groundRadius = 50.0;
  lp->groundlevel = -6.;

  /* I don't think this is needed */
  lp->cratersize = ((size > 0 && size <= MAX_CRATERSIZE) ?
		    size / 100.0 : 1.0);

  lp->numcraters = ((count > 0 && count <= MAX_CRATERS) ? count : 20);
  lp->countcraters = 0;
  lp->craters = calloc (lp->numcraters, sizeof (crater *));
  for (loop = 0; loop < lp->numcraters; loop++) lp->craters[loop] = NULL;

  lp->ctimer = lp->timer = 10;
  lp->erasing = false;
  lp->eruption = init_ejemitter(5000, 0.01, 25.);
  lp->erupting = false;

  groundquad = gluNewQuadric();
  gluQuadricCallback(groundquad, GLU_ERROR, errorCallback);
  gluQuadricDrawStyle(groundquad, GLU_FILL); 
  gluQuadricNormals(groundquad, GLU_SMOOTH);
  lp->craterdlist = glGenLists(1);
  glNewList(lp->craterdlist, GL_COMPILE);
  drawcrater4();
  glEndList();
  lp->grounddlist = glGenLists(1);
  glNewList(lp->grounddlist, GL_COMPILE);
  gluDisk(groundquad, 0., lp->groundRadius, 45, 15);
  glEndList();
  gluDeleteQuadric(groundquad);
  

  if (do_fog)
    {
      glEnable (GL_FOG);
      glFogi (GL_FOG_MODE, GL_EXP2);
      glFogf (GL_FOG_DENSITY, 0.004);
      glHint (GL_FOG_HINT, GL_NICEST);
      glFogf (GL_FOG_START, lp->fogDepth * 0.2);
      glFogf (GL_FOG_END, lp->fogDepth);
      glFogfv (GL_FOG_COLOR, fogColor);
      glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }

  glShadeModel (GL_SMOOTH);
  glEnable (GL_DEPTH_TEST);
/*     glEnable(GL_CULL_FACE);  */
/*     glCullFace(GL_BACK);  */
  glEnable (GL_POINT_SMOOTH);
  glEnable (GL_DEPTH_TEST);
  distparams[0] = distparams[2] = 0.0;
  distparams[1] = 0.3;
  glPointParameterfv (GL_POINT_DISTANCE_ATTENUATION, distparams);
  glPointSize (10.0);
  glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glClearDepth (1.0);


/* these need refinement -- are still left from blocktube */
/* also add material properties here -- everything is lunar gray */
/* move declarations to top of fcn, else make a block */
  glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  glMaterialfv (GL_FRONT, GL_SPECULAR, specular);
  glMaterialf (GL_FRONT, GL_SHININESS, 0.3 * 128.0);

  glLightfv (GL_LIGHT0, GL_POSITION, pos);
  glLightfv (GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv (GL_LIGHT0, GL_SPECULAR, spc);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);

  handleGLerrors ("init");
  reshape_craters (mi, MI_WIDTH (mi), MI_HEIGHT (mi));
  glFlush ();
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
	  for (loop = 0; loop < lp->numcraters; loop++)
	    {
	      if (lp->craters[loop] != NULL)
		{
		free (lp->craters[loop]);
		}
	    }
	  free (lp->craters);
	  if (lp->eraser != NULL) free (lp->eraser);
/*       delete_ejemitter(lp->eruption); */
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
  int loop = 0;

/*     printf("Entering draw, timer reads %d\n", lp->ctimer); */

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
      glTranslatef (0., 0., -20.);


      /* draw the ground */
      glPushMatrix();
	  glRotatef (90., -1., 0., 0.);
	  glTranslatef(0., 0., lp->groundlevel);
	glCallList(lp->grounddlist);
	glPopMatrix();

      /* loop over the craters */
      for (loop = 0; loop < lp->countcraters; loop++)
	{
	  ccrater = lp->craters[loop];

	  glPushMatrix ();
	  glRotatef (90., -1., 0., 0.);
	  glTranslatef (ccrater->position[0], ccrater->position[1],
			ccrater->position[2]);
	  glScalef (ccrater->sizefactor, ccrater->sizefactor, ccrater->sizefactor);

	  /* draw the crater */
	  glCallList(lp->craterdlist);
/* 	printf("Just drew crater number %d at %f, %f, %f\n", loop, ccrater->position[0], ccrater->position[1], ccrater->position[2]); */
	  
	  if ((loop == lp->countcraters - 1) && (lp->erupting == true))
	    {
	      glRotatef (-90., -1., 0., 0.);
	      lp->erupting = spew_ejecta(lp->eruption);
	    }


	  glPopMatrix ();


	}

      if (lp->erupting == false) lp->ctimer--;

      if (lp->ctimer < 0)
	{
	  lp->ctimer = lp->timer;
	  lp->countcraters++;
	  if (lp->countcraters > lp->numcraters)
	    {
/* 	      printf("max craters reached, freeing and erasing\n"); */
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
/* 	      printf("room for another crater at %d\n", lp->countcraters - 1); */
	      lp->craters[lp->countcraters - 1] = new_crater (lp);
	      reset_ejemitter(lp->eruption, 0.6);
	      lp->erupting = true;
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

/* ***yet to be done (list not complete)***
 *
 * make crater mesh gracefully enter during ejecting
 *  -> possibly have mesh rise up from underneath,
 *     or make multiple meshes and display them in sequence
 *
 * reconcile size of ejecta cloud with size of mesh
 * get ejecta to look more realistic, presently is a big cloud
 *
 * set lighting positon appropriately 
 *   (remember ejecta must show against other objects)
 *
 * put in meteor strike
 *
 * create starfield 
 *
 * reconcile normal and erasing delays, add usleep code if necessary
 *  -> rotating old craters offscreen and freeing is still an option
 *
 * refine options and defaults
 *
 * modelview matrix manipulation is inefficient
 *
 *
 * get craters to instantiate in the viewable area at the proper size
 *  -> determine desired size, particle system will influence this
 *     (tenatively done)
 *
 * solve the problem of craters intersecting
 *
 * refine sizefactor--presently all craters are between 2. and 4.
 */


/* minimum distance from eye point that crater needs to be seen:
 * abs(groundlevel) / tan (0.5 * angle specified in gluPerspective()) +
 *   distance translated back 
 */

/* --------------------------------------------------- */
