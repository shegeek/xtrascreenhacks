/* hhtree.c, Copyright (c) 2015 Kelley Nielsen <kelleynnn@gmail.com>
 *
 * visualizer for the hogeweg-hesper L-systems generator
 * it's impossible to get the visualization exactly like the one
 * in H&H's original PL/1 program,
 * but the generated trees are (mostly) (hopefully) correct and
 * the render is visually pleasing.

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

#define refresh_hhtree 0
#define hhtree_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include "xlockmore.h"
#include <math.h>
#include "hogeweg-hesper.h"

#define DEFAULTS        "*delay:	30000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL			/* whole file */

#define DEF_NCOLORS     "64"
#define DEF_SMOOTH "False"
#define DEF_CLOVER "False"
#define DEF_BUMPYSQUARE "False"

#define ORTHOSIZE 20.0
#define COLORS 0
#define LOWER 1
#define BRANCHONLY 0

enum displaymap
{ CLOVER, STRIPES, CIRCLE, PLASMA, BUMPYROUND, BUMPYSQUARE };

typedef struct
{
  GLXContext *glx_context;
  XColor *colors;
  GLuint *pixcolors;
  int numcolors;
  int coloroffset;
  GLfloat viewangle;
  GLfloat viewheight;
  GLfloat xeye, zeye;
  branchnode *hhtree;
  GLuint branchdlist;
  /* this is never read from ? */
  int maxstackdepth[1];
} hhtree_configuration;

static hhtree_configuration *lps = NULL;

static int ncolors;
static Bool smooth;
static Bool clover;
static Bool bumpysquare;

/* test colors, discard once color array is working */
GLfloat face0_color[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat face1_color[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat face2_color[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat face3_color[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat face4_color[] = { 1.0, 0.0, 1.0, 1.0 };
GLfloat face5_color[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat face6_color[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat final_color[] = { 0.7, 0.8, 0.0, 1.0 };

/* gold and silver are from the teapots demo from the redbook */
GLfloat gold_ambient[] = { 0.24725, 0.1995, 0.0745, 1.0 };
GLfloat gold_diffuse[] = { 0.75164, 0.60648, 0.22648, 1.0 };
GLfloat gold_specular[] = { 0.628281, 0.555802, 0.366065, 1.0, 1.0 };

GLfloat gold_shininess = 0.4;

static XrmOptionDescRec opts[] = {
  {"-ncolors", ".ncolors", XrmoptionSepArg, 0},
  {"-smooth", ".smooth", XrmoptionNoArg, "True"},
  {"+smooth", ".smooth", XrmoptionNoArg, "False"},
  {"-clover", ".clover", XrmoptionNoArg, "True"},
  {"+clover", ".clover", XrmoptionNoArg, "False"},
  {"-bumpysquare", ".bumpysquare", XrmoptionNoArg, "True"},
  {"+bumpysquare", ".bumpysquare", XrmoptionNoArg, "False"},
};

static argtype vars[] = {
  {&ncolors, "ncolors", "NColors", DEF_NCOLORS, t_Int},
  {&smooth, "smooth", "Smooth", DEF_SMOOTH, t_Bool},
  {&clover, "clover", "Clover", DEF_CLOVER, t_Bool},
  {&bumpysquare, "bumpysquare", "BumpySquare", DEF_BUMPYSQUARE, t_Bool},
};

static OptionStruct desc[] = {
  {"-ncolors", "how many different colors the screen will use"},
  {"-smooth", "whether to use smooth color transitions"},
  {"-clover",
   "whether to use the clover (default) colormap (overrides other maps)"},
  {"-bumpysquare", "whether to use the square eggcrate colormap"},
};

ENTRYPOINT ModeSpecOpt hhtree_opts =
  { countof (opts), opts, countof (vars), vars, desc };

#ifdef USE_MODULES
ModStruct hhtree_description = { "hhtree", "init_hhtree", "draw_hhtree",
  "release_hhtree",
  "draw_hhtree", "init_hhtree", (char *) NULL, &hhtree_opts,
  30000, 30, 1, 1, 64, 1.0, "",
  "Simulated plant life", 0, NULL
};
#endif /* USE_MODULES */

static void makebranchlist (hhtree_configuration * lp);
static void rendertree (hhtree_configuration * lp, branchnode * finaltree,
			int order, int axis);
void advance_hhtree_view (hhtree_configuration * lp);

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

/* ===== tree generation =================== */

static void
makebranchlist (hhtree_configuration * lp)
{
  lp->branchdlist = glGenLists (1);
  glNewList (lp->branchdlist, GL_COMPILE);
  glDisable (GL_LIGHTING);
  glBegin (GL_LINES);
  glVertex3f (0., 0., 0.);
  glVertex3f (0., 10., 0.);
  glEnd ();
  glEnable (GL_LIGHTING);
  glTranslatef (0., 10., 0.);
  /* TODO: we don't have glut primitives anymore */
/*   glutSolidDodecahedron(); */
  glEndList ();
}

/* axis param is 0 is for axis, nonzero for side branch */
static void
rendertree (hhtree_configuration * lp, branchnode * finaltree,
	    int order, int axis)
{
  branchnode *base = finaltree;
  int i;
  int colorselect = order % 8;

  if (COLORS)
    {
      /* use the color array, not these test colors */
      switch (colorselect)
	{
	case 0:
	  glColor4fv (face0_color);
	  break;
	case 1:
	  glColor4fv (face1_color);
	  break;
	case 2:
	  glColor4fv (face2_color);
	  break;
	case 3:
	  glColor4fv (face3_color);
	  break;
	case 4:
	  glColor4fv (face4_color);
	  break;
	case 5:
	  glColor4fv (face5_color);
	  break;
	case 6:
	  glColor4fv (face6_color);
	  break;
	case 7:
	  glColor4fv (final_color);
	  break;
	default:
	  break;
	}
    }

  /* render the representation of this branchnode */
  glRotatef (180., 0., -1., 0.);
  glCallList (lp->branchdlist);

  /* handle side branches, if any */
  if (base->sidebranchcount > 0)
    {
      for (i = 0; i < base->sidebranchcount; i++)
	{
	  glPushMatrix ();
	  /* spread branch 45 degrees down from the axis,
	   * then distribute the branches evenly about the axis
	   */
	  glRotatef (base->spreadangle * i, 0., 1., 0.);
	  glRotatef (45., 0., 0., -1.);
/* 	  glRotatef(45., 0., 0., (order % 2 == 0 ? 1. : -1.)); */
	  rendertree (lp, base->branches[i + 1], order + 1, i + 1);
	  glPopMatrix ();
	}
    }

  /* handle next axis branch in chain, if it exists */
  if (base->branches[0] != NULL)
    {
      rendertree (lp, base->branches[0], order + 1, 0);
    }
}

void
advance_hhtree_view (hhtree_configuration * lp)
{
  lp->viewangle += 0.02;
  if (lp->viewangle >= 6.28)
    lp->viewangle -= 6.28;
}

/* ===== end tree generation =============== */

ENTRYPOINT void
reshape_hhtree (ModeInfo * mi, int width, int height)
{
  float aspect = (float) width / (float) height;
  glViewport (0, 0, (GLint) width, (GLint) height);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
/*    height is reversed to make the viewport have
 *    the same coordinates as an x window */
  /* find a better way to do this than "orthosize" */
  glOrtho (-ORTHOSIZE * aspect, ORTHOSIZE * aspect, -ORTHOSIZE, ORTHOSIZE,
	   1.0, 500.0);
  /* gluPerspective (60., width/height, 1., 200.0); */
  glMatrixMode (GL_MODELVIEW);
  handleGLerrors ("reshape");
}

ENTRYPOINT void
init_hhtree (ModeInfo * mi)
{
  hhtree_configuration *lp;
  char *hhstring;
  int loop;

  GLfloat light_position[] = { 5.0, 5.0, 10.0, 0.0 };
  GLfloat amb[4] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat dif[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat spc[4] = { 0.4, 0.4, 0.4, 1.0 };
  GLfloat fogColor[4] = { 0., 0., 0., 1. };

  if (!lps)
    {
      lps = (hhtree_configuration *)
	calloc (MI_NUM_SCREENS (mi), sizeof (hhtree_configuration));
      if (!lps)
	{
	  fprintf (stderr, "%s: out of memory\n", progname);
	  exit (1);
	}
      lp = &lps[MI_SCREEN (mi)];
    }

  lp = &lps[MI_SCREEN (mi)];
  lp->glx_context = init_GL (mi);

  lp->coloroffset = 0;
  if (MI_IS_MONO (mi))
    {
      lp->numcolors = 2;
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
	}
    }

  /* transform x colors into pixel values for gl usage */
  lp->pixcolors = calloc (lp->numcolors, sizeof (GLuint));
  if (lp->pixcolors == NULL)
    {
      fprintf (stderr, "%s: out of memory\n", progname);
      exit (1);
    }
  for (loop = 0; loop < lp->numcolors; loop++)
    {
      GLuint r, g, b;
      r = lp->colors[loop].red / 256;
      g = lp->colors[loop].green / 256;
      b = lp->colors[loop].blue / 256;
      lp->pixcolors[loop] = r << 24 | g << 16 | b << 8 | 255;
    }
/*  free (lp->colors);
  lp->colors = NULL; */

  /* TODO: use fog, or make option? */
  glEnable (GL_FOG);
  glFogi (GL_FOG_MODE, GL_LINEAR);
  glFogf (GL_FOG_DENSITY, 0.10);
  glHint (GL_FOG_HINT, GL_NICEST);
  glFogf (GL_FOG_START, 150);
  glFogf (GL_FOG_END, 400);
  glFogfv (GL_FOG_COLOR, fogColor);

  glLightfv (GL_LIGHT0, GL_POSITION, light_position);
  glLightfv (GL_LIGHT0, GL_AMBIENT, amb);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, dif);
  glLightfv (GL_LIGHT0, GL_SPECULAR, spc);
  lp->viewangle = 0.0;
  lp->viewheight = 0.0;
  glShadeModel (GL_SMOOTH);
  glColor4fv (gold_diffuse);
  glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  glLineWidth (3.);
  glEnable (GL_CULL_FACE);
  glCullFace (GL_BACK);
  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth (1.0f);

  hhstring = malloc (STRINGSIZE);
  generate (hhstring, ITERATIONS, 0);
  lp->hhtree = mkhogewegtree (hhstring);
  free (hhstring);
  makebranchlist (lp);

  handleGLerrors ("init");
  reshape_hhtree (mi, MI_WIDTH (mi), MI_HEIGHT (mi));
  glFlush ();
}

ENTRYPOINT void
release_hhtree (ModeInfo * mi)
{
  if (lps)
    {
      int screen;
      for (screen = 0; screen < MI_NUM_SCREENS (mi); screen++)
	{
	  hhtree_configuration *lp = &lps[screen];
	  free (lp->pixcolors);
	}
      free (lps);
      lps = 0;
    }
  FreeAllGL (mi);
}

ENTRYPOINT void
draw_hhtree (ModeInfo * mi)
{
  hhtree_configuration *lp = &lps[MI_SCREEN (mi)];
  Display *dpy = MI_DISPLAY (mi);
  Window window = MI_WINDOW (mi);
  int i;

  if (!lp->glx_context)
    return;
  glXMakeCurrent (MI_DISPLAY (mi), MI_WINDOW (mi), *(lp->glx_context));
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  advance_hhtree_view (lp);

  if (LOWER)
    glTranslatef (0., -ORTHOSIZE + 10., 0.);
  glTranslatef (0., 0., -100.);
  lp->xeye = 5 * sin (lp->viewangle);
  lp->zeye = 5 * cos (lp->viewangle);
  gluLookAt (lp->xeye, lp->viewheight, lp->zeye, 0.0, 0.0, 0.0, 0.0, 1.0,
	     0.0);

  /*draw tree */
  glMaterialfv (GL_FRONT, GL_AMBIENT, gold_ambient);
  glMaterialfv (GL_FRONT, GL_DIFFUSE, gold_diffuse);
  glMaterialfv (GL_FRONT, GL_SPECULAR, gold_specular);
  glMaterialf (GL_FRONT, GL_SHININESS, gold_shininess * 128.0);

  if (COLORS)
    glEnable (GL_COLOR_MATERIAL);
  if (BRANCHONLY)
    rendertree (lp, lp->hhtree->branches[lp->hhtree->sidebranchcount], 0, 0);
  else
    rendertree (lp, lp->hhtree, 0, 0);
  if (COLORS)
    glDisable (GL_COLOR_MATERIAL);

  glLoadIdentity ();

  if (++lp->coloroffset >= lp->numcolors)
    lp->coloroffset -= lp->numcolors;
  if (mi->fps_p)
    do_fps (mi);
  glFinish ();
  glXSwapBuffers (dpy, window);
  handleGLerrors ("draw");
}

XSCREENSAVER_MODULE ("HHTree", hhtree)
#endif /* USE_GL */
/* =========================================================== */
/* debug functions, held here in case they're needed */
/* put these supporting variables in the configuration struct */
/*
double pointvectors[400][3];
int pointcount = 0;
int pointindices[400][2];
int indexcount = 0;
*/
/* void dumppoints(void){
  int i;
  for (i = 0; i < pointcount; i++){
    printf("\t<item>\"%f %f %f\"</item>\n", pointvectors[i][0],  pointvectors[i][1],  pointvectors[i][2]);
  }
  printf("\n");
  for (i = 0; i < indexcount; i++){
    printf("\t<item>\"%d %d\"</item>\n", pointindices[i][0], pointindices[i][1]);
  }
} */
/* prints screen coordinates rounded to the nearest int */
/*void printcurpointd(void){
  double curmatrix[16];
  int i;
  double magnification = WINYSIZE / ORTHOSIZE;
    glGetDoublev(GL_MODELVIEW_MATRIX, curmatrix);
    printf("%d %d %d %d   ", (int)nearbyint(curmatrix[12] * magnification),
	   (int)nearbyint(curmatrix[13] * magnification),
	   (int)nearbyint(curmatrix[14]), (int)curmatrix[15]);
} */
/* prints screen coordinates left as doubles */
/* void printcurpointf(void){
  double curmatrix[16];
  int i;
  double magnification = WINYSIZE / ORTHOSIZE;
    glGetDoublev(GL_MODELVIEW_MATRIX, curmatrix);
    printf("\t<item>\"%f %f %f\"</item>\n", (curmatrix[12] * magnification),
	   (curmatrix[13] * magnification),  (curmatrix[14] * magnification));
} */
