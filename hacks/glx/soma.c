/*soma, Copyright (c) 2009 Kelley Nielsen <shegeek-dev@comcast.net>
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

#define refresh_soma 0
#define soma_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))
#define sign(x)  (x)/(fabs(x))

#include "xlockmore.h"
#include <math.h>
#include "bevelcube.h"
#include "flipguide.h"
#include "somafigures.h"


#define DEFAULTS        "*delay:	30000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL /* whole file */

#define MAX_FLYERS      1000
#define MAX_POLYSIZE   200

#define DEF_DURATION    "500"
#define DEF_NCOLORS     "256"
#define DEF_MULTICOLORS "False"
#define DEF_COLORCHANGERATE "10"
#define DEF_SPINRATE        "10"
#define DEF_CLOCKWISE   "False"
#define DEF_CHANGERATE   "100"


static int duration;
static int changerate;
static int spinrate;
static int do_clockwise;
static int ncolors;
static int do_multicolors;
static int colorchangerate;

static XrmOptionDescRec opts[] = {
  { "-duration",   ".duration", XrmoptionSepArg, 0 },
  {"-changerate", ".changerate", XrmoptionSepArg, 0},
    { "-spinrate",     ".spinrate",     XrmoptionSepArg, 0 },
  {"-clockwise", ".clockwise", XrmoptionNoArg, "True"},
  {"+clockwise", ".clockwise", XrmoptionNoArg, "False"},
    { "-ncolors",  ".ncolors",  XrmoptionSepArg, 0 },
  {"-multicolors", ".multicolors", XrmoptionNoArg, "True"},
  {"+multicolors", ".multicolors", XrmoptionNoArg, "False"},
  {"-colorchangerate", ".colorchangerate", XrmoptionSepArg, 0},
};

static argtype vars[] = {
  {&duration,   "duration",  "Duration",   DEF_DURATION,  t_Int},
  {&changerate, "changerate", "Changerate", DEF_CHANGERATE, t_Int},
    {&spinrate,     "spinrate", "Spinrate",  DEF_SPINRATE,   t_Int},
  {&do_clockwise, "clockwise", "Clockwise", DEF_CLOCKWISE, t_Bool},
    {&ncolors,  "ncolors",   "NColors",    DEF_NCOLORS,   t_Int},
  {&do_multicolors, "multicolors", "Multicolors", DEF_MULTICOLORS, t_Bool},
  {&colorchangerate, "colorchangerate", "Colorchangerate", DEF_COLORCHANGERATE, t_Int},
};

static OptionStruct desc[] = {
  {"-duration", "how long each figure should appear before breaking up"},
  {"-changerate", "how fast the figures should break up and reassemble"},
    {"-spinrate",  "how far the pieces should rotate in one frame"},
  {"-clockwise", "whether to make the figure rotate clockwise"},
    {"-ncolors", "how many different colors the screen will use"},
  {"-multicolors", "whether to draw each puzzle piece in a separate color"},
  {"-colorchangerate", "how fast colors should cycle"},
};

ENTRYPOINT ModeSpecOpt soma_opts = {countof(opts), opts, countof(vars), vars, desc};

#ifdef USE_MODULES
ModStruct soma_description =
    {"soma", "init_soma", "draw_soma", "release_soma",
     "draw_soma", "init_soma", (char *)NULL, &soma_opts,
     30000, 30, 1, 1, 64, 1.0, "",
     "Piet Hein's Soma Puzzle", 0, NULL};
#endif /* USE_MODULES */


enum displaystate_t{assembling, resting, exploding};

typedef struct {
  GLXContext *glx_context;
  figure * currentfigure;
  figure *basefigure;
  int ftimer;
  int fduration;
  int ctimer;
  int cduration;
  int tduration;
  double viewangle;
  double vanglestep;
  GLuint piecedlists[8];

  XColor *colors;
  int numcolors;
  GLfloat piececolors[8][3];
  int colorindices[8];

  enum displaystate_t dstate;
  GLfloat xinc[8];
  GLfloat yinc[8];
  GLfloat zinc[8];
} soma_configuration;

static soma_configuration *lps = NULL;



/* ---- main body of code */ 


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


static void makedlists(GLuint * dlists)
{
  dlists[1] = glGenLists(1);
  glNewList(dlists[1], GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(-1.0, 1.0, 0.0);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  dlists[2] = glGenLists(1);
  glNewList(dlists[2], GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(1., 0., 0.);
   drawbevelcube();
   glTranslatef(-2.0, 1.0, 0.0);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  dlists[3] = glGenLists(1);
  glNewList(dlists[3], GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(-2.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(1., 1.0, 0.0);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  dlists[4] = glGenLists(1);
  glNewList(dlists[4], GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(-1.0, 1.0, 0.0);
   drawbevelcube();
   glTranslatef(-1., 0., 0.);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  dlists[5] = glGenLists(1);
  glNewList(dlists[5], GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(0.0, 1.0, 0.0);
   drawbevelcube();
   glTranslatef(1.0, -1.0, 0.0);
   drawbevelcube();
   glTranslatef(0.0, 0.0, 1.0);
   drawbevelcube();
   glPopMatrix();
   glEndList();

  dlists[6] = glGenLists(1);
  glNewList(dlists[6], GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(0.0, 1.0, 0.0);
   drawbevelcube();
   glTranslatef(1.0, -1.0, 0.0);
   drawbevelcube();
   glTranslatef(0.0, 0.0, -1.0);
  drawbevelcube();
   glPopMatrix();
   glEndList();

  dlists[7] = glGenLists(1);
  glNewList(dlists[7], GL_COMPILE);
  glPushMatrix();  
   drawbevelcube();
   glTranslatef(1.0, 0.0, 0.0);
   drawbevelcube();
   glTranslatef(-1.0, 1.0, 0.0);
   drawbevelcube();
   glTranslatef(0., -1., -1.);
   drawbevelcube();
   glPopMatrix();
   glEndList();
}

/* rotate among display states,
 * setting assembled and exploded coordinates for each piece
 * and the increments to move between each;
 * changing figure at beginning of each cycle.
 */
static void reset_figure (soma_configuration *lp, figure *fl)
{
  int newfigure;
  int itor;
  int jitterval;
  GLfloat outfactor = 13.0;

  if (lp->dstate == exploding)
    {
      lp->dstate = assembling;
      lp->ftimer = lp->tduration;
 newfigure = random() % somafigurescount;
  for (itor = 1; itor < 8; itor++)
    {
      jitterval = (random() % 2 == 0) ? -1 : 1;
  lp->basefigure->refcubex[itor] = somafigures[newfigure].refcubex[itor];
  fl->refcubex[itor] = (somafigures[newfigure].refcubex[itor] * outfactor);
  lp->xinc[itor] = (lp->basefigure->refcubex[itor] - fl->refcubex[itor]) / lp->tduration;

  lp->basefigure->refcubey[itor] = somafigures[newfigure].refcubey[itor];
  fl->refcubey[itor] = somafigures[newfigure].refcubey[itor] * (outfactor);
  if ((fl->refcubey[itor] < 2.0) /* && (fl->refcubez[itor] < 2.0) && (fl->refcubex[itor] < 2.0) */)
     {
       if (fl->refcubey[itor] == 0) 
	 fl->refcubey[itor] = (outfactor) * jitterval;
       else fl->refcubey[itor] = (outfactor) * sign(fl->refcubey[itor]);
     }
  lp->yinc[itor] = (lp->basefigure->refcubey[itor] - fl->refcubey[itor]) / lp->tduration;

  lp->basefigure->refcubez[itor] = somafigures[newfigure].refcubez[itor];
  fl->refcubez[itor] = (somafigures[newfigure].refcubez[itor] * outfactor);
  lp->zinc[itor] = (lp->basefigure->refcubez[itor] - fl->refcubez[itor]) / lp->tduration;

  fl->orientation[itor] = somafigures[newfigure].orientation[itor];
  lp->basefigure->orientation[itor] = somafigures[newfigure].orientation[itor];
   }
}

  else if (lp->dstate == assembling)
    {
      lp->dstate = resting;
      lp->ftimer = lp->fduration;
      for (itor = 1; itor < 8; itor++)
	{
	  fl->refcubex[itor] = lp->basefigure->refcubex[itor];
	  fl->refcubey[itor] = lp->basefigure->refcubey[itor];
	  fl->refcubez[itor] = lp->basefigure->refcubez[itor];
	}
 }

  else /* if (lp->dstate == resting) */
    {
      lp->dstate = exploding;
       lp->ftimer = lp->tduration;
       for (itor = 0; itor < 8; itor++)
	 {
	   lp->xinc[itor] = -lp->xinc[itor];
	   lp->yinc[itor] = -lp->yinc[itor];
	   lp->zinc[itor] = -lp->zinc[itor];
	 }
    }
}

static void reset_color (soma_configuration *lp, figure *fl)
{
  XColor tempcolor;
  int itor;

  lp->ctimer = lp->cduration;
  for (itor = 1; itor < 8; itor++)
    {
  lp->colorindices[itor]++;
  if (lp->colorindices[itor] >= lp->numcolors )
    {
      lp->colorindices[itor] = 0;
    }
  tempcolor = (lp->colors[lp->colorindices[itor]]);
  lp->piececolors[itor][0] = tempcolor.red / 65536.0;
  lp->piececolors[itor][1] = tempcolor.green / 65536.0;
  lp->piececolors[itor][2] = tempcolor.blue / 65536.0;
    }
}

static void advance (soma_configuration *lp, figure *fl)
{
  int itor;
   lp->ftimer--;
	if (lp->numcolors > 1)  lp->ctimer--;
        if( lp->ftimer <= 0 ) reset_figure(lp, fl);
        if ( lp->ctimer <= 0) reset_color(lp, fl);

	if ( lp->dstate != resting)
	  {
	    for (itor = 1; itor < 8; itor++)
	      {
		fl->refcubex[itor] += lp->xinc[itor];
		fl->refcubey[itor] += lp->yinc[itor];
		fl->refcubez[itor] += lp->zinc[itor];
	      }
          }
	else
	  {
	lp->viewangle += lp->vanglestep;
        if (lp->viewangle >= 6.28) lp->viewangle = 0.0;
	  }
}

ENTRYPOINT void reshape_soma (ModeInfo *mi, int width, int height)
{
  GLfloat aspect = (GLfloat) width / (GLfloat) height;
    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
   gluPerspective(45.0, aspect, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    handleGLerrors("reshape");
}



/* ----- init ------------------------------------------------*/

ENTRYPOINT void init_soma (ModeInfo *mi)
{
    int loop;
    int colorincrement;
    GLfloat fogColor[4] = {0., 0., 0., 1.};
     GLfloat pos[4] = {0.0, 1.0, 1.0, 0.0};
      GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0};
      GLfloat dif[4] = {1.0, 1.0, 1.0, 1.0};
      GLfloat spc[4] = {0.4, 0.4, 0.4, 1.0};
    GLfloat shininess[] = {75.};
    GLfloat specular[] = {1., 1., 1., 1.};
    soma_configuration *lp;

    if (!lps) {
      lps = (soma_configuration *)
        calloc (MI_NUM_SCREENS(mi), sizeof (soma_configuration));
      if (!lps) {
        fprintf(stderr, "%s: out of memory\n", progname);
        exit(1);
      }
      lp = &lps[MI_SCREEN(mi)];
    }

    lp = &lps[MI_SCREEN(mi)];
    lp->glx_context = init_GL(mi);

    lp->currentfigure = malloc (sizeof (figure));
    lp->basefigure = malloc (sizeof (figure));

    lp->fduration = ((duration > 0 && duration <= 5000) ? duration : 500);
    lp->cduration = ((colorchangerate > 0 && colorchangerate <= 1000) ? colorchangerate: 10);
    lp->tduration = ((changerate > 0 && changerate <= 500) ? changerate : 100);
    lp->viewangle = 0.;
     lp->vanglestep = ((spinrate >= -100 && spinrate <= 100) ? (spinrate / 500.0) : 0.02);
     if (do_clockwise) lp->vanglestep *= -1;
   lp->dstate = exploding;

     makedlists(lp->piecedlists);


     if(!MI_IS_MONO(mi))
      {
	lp->numcolors = ((ncolors > 0 && ncolors < 2049) ? 
                          ncolors : 256);
    lp->colors = calloc ( lp->numcolors,  sizeof (*lp->colors));
    make_smooth_colormap (MI_DISPLAY(mi),MI_VISUAL(mi), MI_COLORMAP(mi),
                            lp->colors, &lp->numcolors, False, 0, True);
     }
     if( (MI_IS_MONO(mi)) || (lp->numcolors < 1))
      {
/* 	GLfloat white[] = {1., 1., 1., 1.}; */
	lp->numcolors = 1;
        lp->colors = calloc (1, sizeof (*lp->colors));
        lp->colors[0].pixel = MI_WHITE_PIXEL(mi);
        XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[0]);
/*          glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white); */
     } 
    colorincrement = (int)lp->numcolors / 7;
    for (loop = 1; loop < 8; loop++)
      {
	if (do_multicolors && lp->numcolors > 6)
	  {
	    lp->colorindices[loop] = colorincrement * loop;
          }
	else
          {
	    lp->colorindices[loop] = 0;
          }
      }
    lp->ftimer = lp->ctimer = 1;
    if (lp->numcolors < 2) reset_color(lp, lp->currentfigure);

      glEnable(GL_FOG);
      glFogi(GL_FOG_MODE, GL_LINEAR);
      glFogf(GL_FOG_DENSITY, 0.50);
      glHint(GL_FOG_HINT, GL_NICEST);
      glFogf(GL_FOG_START, 15);
      glFogf(GL_FOG_END, 25);
      glFogfv(GL_FOG_COLOR, fogColor);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);


      glLightfv(GL_LIGHT0, GL_POSITION, pos);
      glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);
      glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
      glLightfv(GL_LIGHT0, GL_SPECULAR, spc);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
   glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    handleGLerrors("init");
    reshape_soma(mi, MI_WIDTH(mi), MI_HEIGHT(mi));
    glFlush();
}


ENTRYPOINT void release_soma (ModeInfo *mi)
{
  if (lps) {
    int screen, loop;
    for (screen = 0; screen < MI_NUM_SCREENS(mi); screen++) {
      soma_configuration *lp = &lps[screen];
      /* colors.h call gives bad access xlib error */
      free(lp->colors);
/*       free_colors(MI_DISPLAY(mi), MI_COLORMAP(mi), lp->colors, lp->numcolors); */
      for (loop = 1; loop < 8; loop++)
	{
	  glDeleteLists(lp->piecedlists[loop], 1);
	}
      free(lp->currentfigure);
      free(lp->basefigure);
    }
    free (lps);
    lps = 0;
  }

  FreeAllGL(mi);
}




ENTRYPOINT void
draw_soma (ModeInfo *mi)
{
    soma_configuration *lp = &lps[MI_SCREEN(mi)];
    Display *dpy = MI_DISPLAY(mi);
    Window window = MI_WINDOW(mi);
    GLfloat xeye, zeye;
    int loop = 0;

    if (!lp->glx_context)
      return;
    glXMakeCurrent(MI_DISPLAY(mi), MI_WINDOW(mi), *(lp->glx_context));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	 xeye = 10.0 * cos(lp->viewangle);
         zeye = 10.0 * sin(lp->viewangle);

   advance(lp, lp->currentfigure);

         glLoadIdentity();
   gluLookAt (xeye, 5., zeye, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);

   for (loop = 1; loop < 8; loop++) {
        glPushMatrix();
	if (lp->numcolors > 1)
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lp->piececolors[loop]);
       glTranslatef(lp->currentfigure->refcubex[loop],
		     lp->currentfigure->refcubey[loop],
		     lp->currentfigure->refcubez[loop]);
        glRotatef(s4[lp->currentfigure->orientation[loop]].degrees,
		  s4[lp->currentfigure->orientation[loop]].rotvector[0],
		  s4[lp->currentfigure->orientation[loop]].rotvector[1],
		  s4[lp->currentfigure->orientation[loop]].rotvector[2]);

	glCallList(lp->piecedlists[loop]);
 
        glPopMatrix();
   }
    if (mi->fps_p) do_fps (mi);
    glFinish();
    glXSwapBuffers(dpy, window);
    handleGLerrors("draw");
}

XSCREENSAVER_MODULE ("Soma", soma)

#endif /* USE_GL */

  /* -------------------------------------------------------- */


/* --------------------------------------------------- */

/* improve exploding/assembling, so that no pieces wink in/out or collide
 *   maybe make them fly to their new out positions as per original plan
 */

/* configure should take option for solution file,
 * make should prepare somafigures.h from it at compile time
 */

     /* checkerboard coloring */
/* two-set and partial set figures */

/* sometimes the system doesn't allocate the number of colors requested.
 * automatically make mono if not enough colors get allocated?
 * => yes--break off mono handling, call if mono or if allocation fails
 */
