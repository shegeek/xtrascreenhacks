/*flyer, Copyright (c) 2009 Kelley Nielsen <shegeek-dev@comcast.net>
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

#include "xbmread.h"

# define refresh_flyer 0
# define flyer_handle_event 0
#undef countof
#define countof(x) (sizeof((x))/sizeof((*x)))

#include "xlockmore.h"
#include <math.h>

#define DEFAULTXBM "../images/xlogo64"
/*#define DEFAULTXBM "../images/flyer/xlogo64"*/
#include "../images/xlogo64"
#define DEFAULTXBM_WIDTH xlogo64_width
#define DEFAULTXBM_HEIGHT xlogo64_height
#define DEFAULTXBM_BITS xlogo64_bits


#define DEFAULTS        "*delay:	30000           \n" \
			"*showFPS:      False           \n" \

#ifdef USE_GL /* whole file */

#define MAX_FLYERS      1000
#define MAX_TEXTURES    16
#define MAX_TEX_SIZE    1024   
#define MAX_POLYSIZE   200


#define DEF_BITMAP      DEFAULTXBM
#define DEF_COUNT       "20"
#define DEF_NCOLORS     "16"
#define DEF_SIZE        "100"
#define DEF_STEP        "200"
#define DEF_TEXTURE     "True"
#define DEF_FOG         "False"
#define DEF_DOTS        "False"



typedef struct {
  GLuint texid;

  GLfloat color[4]; 
    GLfloat position[3];
} flyer;

typedef struct {
  GLXContext *glx_context;
   flyer **flyers; 
  char *userxbm;
  int numflyers;
  GLuint texturelist[MAX_TEXTURES];
  int numtextures;
  XColor *colors;
  int numcolors;
  GLfloat flyersize;

/*   GLfloat zoom;  */
  GLfloat cloudDepth;
  GLfloat cloudInRadius;
  GLfloat cloudOutRadius;

  GLfloat zincrement;

} flyer_configuration;

static flyer_configuration *lps = NULL;

static char * bitmap;
static int count;
static int ncolors;
static int size;
static int step;
static Bool dots;
static int do_fog;

static XrmOptionDescRec opts[] = {
    { "-bitmap",   ".bitmap",   XrmoptionSepArg, 0 },
    { "-count",    ".count",    XrmoptionSepArg, 0 },
    { "-ncolors",  ".ncolors",  XrmoptionSepArg, 0 },
    { "-size",     ".size",     XrmoptionSepArg, 0 },
    { "-step",     ".step",     XrmoptionSepArg, 0 },
    {"-dots",      ".dots",     XrmoptionNoArg, "True" },
    {"+dots",      ".dots",     XrmoptionNoArg, "False" },
    {"-fog",       ".fog",      XrmoptionNoArg, "True" },
    {"+fog",       ".fog",      XrmoptionNoArg, "False" },
};

static argtype vars[] = {
    {&bitmap,   "bitmap",    "Bitmap",     DEF_BITMAP,    t_String},
    {&count,    "count",     "Count",      DEF_COUNT,     t_Int},
    {&ncolors,  "ncolors",   "NColors",    DEF_NCOLORS,   t_Int},
    {&size,     "size",      "Size",       DEF_SIZE,      t_Int},
    {&step,     "step",      "Step",       DEF_STEP,      t_Int},
    {&dots,     "dots",      "Dots",       DEF_DOTS,      t_Bool},
    {&do_fog,   "fog",       "Fog",        DEF_FOG,       t_Bool},
};

static OptionStruct desc[] = {
  {"-bitmap", "path to <image>.xbm, the image to be flown"},
    {"-count", "how many flyers appear on the screen at the same time"},
    {"-ncolors", "how many different colors the screen will use"},
    {"-size",  "relative size of the flyers on the screen"},
    {"-step",  "how far the flyers should move forward in one frame"},
};

ENTRYPOINT ModeSpecOpt flyer_opts = {countof(opts), opts, countof(vars), vars, desc};

#ifdef USE_MODULES
ModStruct flyer_description =
    {"flyer", "init_flyer", "draw_flyer", "release_flyer",
     "draw_flyer", "init_flyer", (char *)NULL, &flyer_opts,
     40000, 30, 1, 1, 64, 1.0, "",
     "Flying through space with icons zooming by", 0, NULL};
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


static Bool LoadGLTextures(char xbmfile[], GLuint * temptexid)
{
  GLsizei tempwidth = 0;
 GLsizei tempheight = 0;
    GLuint temptexpixels[MAX_TEX_SIZE * MAX_TEX_SIZE];
    GLuint fgcolor = 0xFFFFFFFF;
    GLuint bgcolor = 0x00000000;
    int status = -1;
    GLint errcode = 0;
    Bool success = False;

    /* handling of hard coded default xbm does not check for
     * non power of 2 sized images--
     * we never want to use those as default anyway
     */
    if (strcmp (xbmfile, DEFAULTXBM) == 0)
      {
        int lineindex, byteindex, bitindex;
        int temptexindex;
        GLubyte currentbyte;

	tempwidth = DEFAULTXBM_WIDTH;
        tempheight = DEFAULTXBM_HEIGHT;
	/* outside loop over number of lines, count down from max */
        for (lineindex = DEFAULTXBM_HEIGHT - 1; lineindex >= 0; lineindex--) {
	  /* middle loop, count over bytes in line */
	  for (byteindex = 0; byteindex < (DEFAULTXBM_WIDTH / 8); byteindex++ ) {
            currentbyte = DEFAULTXBM_BITS[lineindex * (DEFAULTXBM_WIDTH / 8)
                                          + byteindex];
	    /* innermost loop, count over bits in byte */
	    for (bitindex = 0; bitindex < 8; bitindex++) {

              temptexindex = ((DEFAULTXBM_HEIGHT - lineindex - 1) *
                              (DEFAULTXBM_WIDTH)) + (byteindex * 8) + bitindex;
	      temptexpixels[temptexindex] = (((currentbyte >> bitindex) & 0x1)
                                               ? fgcolor : bgcolor);
            } /* end of innermost loop */ 
	  } /* end of middle loop */
	    } /* end of outside loop */
	status = 0;
      }
    else
      {
   status = xbmread(xbmfile, fgcolor, bgcolor, 
		    &tempwidth, &tempheight, MAX_TEX_SIZE, temptexpixels);
      }

  if((0 == status) && (tempwidth > 0) && (tempheight > 0))
    {  
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1, temptexid );
    glBindTexture(GL_TEXTURE_2D,  *temptexid);

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                   GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


/*    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempwidth, tempheight, 0, GL_RGBA,  */
/*                 GL_UNSIGNED_INT_8_8_8_8, temptexpixels);  */
    errcode =  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA,  tempwidth, tempheight,
                             GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, temptexpixels); 
   success = True;

    }
  if (0 != errcode )
    {
    fprintf (stderr, "gl error in LoadGLTextures: %s\n", gluErrorString(errcode));
    }

    handleGLerrors("LoadGLTextures");
    return success;
}



static void reset_flyer (flyer_configuration *lp, flyer *fl)
{

  float r, theta;
  XColor tempcolor;
 
  /* use a diameter instead of subtracting every time? */
  r = (float)((random() % (int) (lp->cloudOutRadius - lp->cloudInRadius))
	      + lp->cloudInRadius);
  theta = ((float)(random() % 314)) / 50;
  fl->position[0] = r * cos(theta);
  fl->position[1] = r * sin(theta);
  fl->position[2] = lp->cloudDepth;
 
  if (!dots)
    {
  fl->texid = lp->texturelist[random() % lp->numtextures];
    }
  tempcolor = (lp->colors[random() % lp->numcolors]);
  fl->color[0] = tempcolor.red / 65536.0;
  fl->color[1] = tempcolor.green / 65536.0;
  fl->color[2] = tempcolor.blue / 65536.0;
  fl->color[3] = 1.0;
}


/* need:
 * texture list to assign a decal and poly dimensions (above);
 * color map, to assign a color (above);
 * cloud radii, to assign an xy position (above);
 *   start off with random z positions, or space evenly?
 */ 
/* static flyer new_flyer(flyer_configuration *lp) */
/* { */
/*   flyer * newflyer; */

/*   newflyer = malloc ((sizeof) newflyer); */
/*   if (NULL == newflyer) return NULL; */

/*   should this be called here, or directly after this call in the caller? */ 
/*   randomize_flyer (lp, newflyer); */

/*   return newflyer; */

/*   } */






/* ENTRYPOINT void reshape_flyer (ModeInfo *mi, int width, int height); */
ENTRYPOINT void reshape_flyer (ModeInfo *mi, int width, int height)
{
    GLfloat h = (GLfloat) height / (GLfloat) width;

    glViewport(0, 0, (GLint) width, (GLint) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1/h, 1.0, 500.0);
/*     gluPerspective(45.0, 1/h, 1.0, lps[MI_SCREEN(mi)]->cloudDepth); */
    glMatrixMode(GL_MODELVIEW);
    handleGLerrors("reshape");
}



/* ----- init ------------------------------------------------*/

ENTRYPOINT void init_flyer (ModeInfo *mi)
{
    int loop;
    float zdistribution;
    GLfloat fogColor[4] = {0,0,0,1};
    /* linear coord should be between 0.01 (large dots) and 0.1 (small dots) */
    /* constant and quadratic coords should be zero,*/
    /* too large of dots or a quadratic coord causes an appearance of */
    /* dots taking a curved path and veering away from the eye coordinate */
    GLfloat distparams[3];
    flyer_configuration *lp;



    if (!lps) {
      lps = (flyer_configuration *)
        calloc (MI_NUM_SCREENS(mi), sizeof (flyer_configuration));
      if (!lps) {
        fprintf(stderr, "%s: out of memory\n", progname);
        exit(1);
      }
      lp = &lps[MI_SCREEN(mi)];
    }

    lp = &lps[MI_SCREEN(mi)];
    lp->glx_context = init_GL(mi);

    /*  these will have to be tuned */
/*     lp->zoom = 30.0; */
/*     lp->zoom = 0.0; */
    lp->cloudDepth = -350.0;
    lp->cloudInRadius = 2.0;
    lp->cloudOutRadius = 20.0;
    lp->zincrement = ((step > 0 && step <= 1000) ? step / 100.0 : 2.0);

    if(!dots)
      {
    lp->flyersize = ((size > 0 && size <=  MAX_POLYSIZE) ?
		     size / 100.0 : 1.0);
      }
    else
      {
      lp->flyersize = ((size > 0 && size <=  MAX_POLYSIZE) ?
		       ((size * -0.045) + 10) / 100 : 0.05);
    }

    lp->numflyers = ((count > 0 && count <= MAX_FLYERS) ? count : 20 );
    lp->flyers = calloc (lp->numflyers, sizeof(flyer *));

    if (MI_IS_MONO(mi))
      {
	lp->numcolors = 1;
        lp->colors = calloc (1, sizeof (*lp->colors));
        lp->colors[0].pixel = MI_WHITE_PIXEL(mi);
        XQueryColor( MI_DISPLAY(mi), MI_COLORMAP(mi), &lp->colors[0]);
      } 
    else
      {
	lp->numcolors = ((ncolors > 0 && ncolors < NUMCOLORS) ? 
                          ncolors : 16);
    lp->colors = calloc ( lp->numcolors,  sizeof (*lp->colors));
    make_random_colormap (MI_DISPLAY(mi),MI_VISUAL(mi), MI_COLORMAP(mi),
                            lp->colors, &lp->numcolors, True, False, 0, True);
    }

    /* set up textures --
     * eventually process -dir command line option,
     * looping over the directory and adding as many textures as necessary,
     */
    if (!dots)
      {
    lp->userxbm = get_string_resource (MI_DISPLAY(mi), "bitmap", "Bitmap");
    lp->numtextures = 0;
    if (! LoadGLTextures (lp->userxbm, &lp->texturelist[0]))
      {
	LoadGLTextures (DEFAULTXBM, &lp->texturelist[0]);
      }
    lp->numtextures++;

      glEnable(GL_TEXTURE_2D);
      }
 



    /* kick on the fog machine */
    if (do_fog) {
      glEnable(GL_FOG);
      glFogi(GL_FOG_MODE, GL_EXP2);
      glFogf(GL_FOG_DENSITY, 0.004);
      glHint(GL_FOG_HINT, GL_NICEST);
      glFogf(GL_FOG_START, lp->cloudDepth * 0.2);
      glFogf(GL_FOG_END, lp->cloudDepth);
      glFogfv(GL_FOG_COLOR, fogColor);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    }

    if (!dots) {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 
/*     glCullFace(GL_BACK);  */
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GEQUAL, 0.5);
    }
    else {
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_DEPTH_TEST);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      distparams[0] = distparams[2] = 0.0;
      distparams[1] = lp->flyersize;
      glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distparams);
      glPointSize(6.0);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);


    /* don't need lighting */
/*     if (!do_texture) { */
      /* If there is no texture, the boxes don't show up without a light.
         Though I don't understand why all the blocks come out gray.
       */
/*       GLfloat pos[4] = {0.0, 1.0, 1.0, 0.0}; */
/*       GLfloat amb[4] = {0.2, 0.2, 0.2, 1.0}; */
/*       GLfloat dif[4] = {1.0, 1.0, 1.0, 1.0}; */
/*       GLfloat spc[4] = {1.0, 1.0, 1.0, 1.0}; */
/*       glLightfv(GL_LIGHT0, GL_POSITION, pos); */
/*       glLightfv(GL_LIGHT0, GL_AMBIENT,  amb); */
/*       glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif); */
/*       glLightfv(GL_LIGHT0, GL_SPECULAR, spc); */
/*       glEnable(GL_LIGHTING); */
/*       glEnable(GL_LIGHT0); */
/*     } */

    handleGLerrors("init");

     zdistribution = lp->cloudDepth / lp->numflyers;
    for (loop = 0; loop < lp->numflyers; loop++)
    {
/*       lp->flyers[loop] = new_flyer(lp); */
      lp->flyers[loop] = malloc (sizeof (flyer ));
      if (NULL == lp->flyers[loop]){
	if (DEBUGMSG) printf("single flyer didn't get allocated\n");
	    lp->numflyers = loop;
            break;  /* handle this better */
	  }
	  reset_flyer(lp, lp->flyers[loop]);
      lp->flyers[loop]->position[2] = loop * zdistribution;
    }
    reshape_flyer(mi, MI_WIDTH(mi), MI_HEIGHT(mi));

    /* why is the buffer flushed here? 
     *should i draw, or is there another reason?
     */
    glFlush();
}


ENTRYPOINT void release_flyer (ModeInfo *mi)
{
  if (lps) {
    int screen, loop;
    /* if there might be more than one screen, 
     * do I need to manage multiple state structs throughout the program?
     */
    for (screen = 0; screen < MI_NUM_SCREENS(mi); screen++) {
      flyer_configuration *lp = &lps[screen];
      /* colors.h call gives bad access xlib error */
      free(lp->colors);
/*       free_colors(MI_DISPLAY(mi), MI_COLORMAP(mi), lp->colors, lp->numcolors); */
      for (loop = 0; loop < lp->numflyers; loop++)
	{
          free(lp->flyers[loop]);
	}
      free(lp->flyers);
    }
    free (lps);
    lps = 0;
  }

  FreeAllGL(mi);
}




ENTRYPOINT void
draw_flyer (ModeInfo *mi)
{
    flyer_configuration *lp = &lps[MI_SCREEN(mi)];
    Display *dpy = MI_DISPLAY(mi);
    Window window = MI_WINDOW(mi);
    flyer *cflyer = NULL;
    int loop = 0;

    if (!lp->glx_context)
      return;

    glXMakeCurrent(MI_DISPLAY(mi), MI_WINDOW(mi), *(lp->glx_context));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


         glLoadIdentity();
/*         glTranslatef(0.0f, 0.0f, -(lp->zoom)); */

   for (loop = 0; loop < lp->numflyers; loop++) {
         cflyer = lp->flyers[loop];

        glPushMatrix();
        glColor4fv(cflyer->color);
        glTranslatef(cflyer->position[0], cflyer->position[1],
                     cflyer->position[2]);

	if (!dots)
	  {
      glBindTexture(GL_TEXTURE_2D, cflyer->texid); 

   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0);    glVertex2f(-lp->flyersize, -lp->flyersize);
   glTexCoord2f(1.0, 0.0);   glVertex2f(lp->flyersize, -lp->flyersize);
   glTexCoord2f(1.0, 1.0);   glVertex2f(lp->flyersize, lp->flyersize);
   glTexCoord2f(0.0, 1.0);   glVertex2f(-lp->flyersize, lp->flyersize);
   glEnd();
	  }
	else
	  {
	    glBegin(GL_POINTS);
            glVertex2f(0.0, 0.0);
            glEnd();
          }


        glPopMatrix();
	cflyer->position[2] += lp->zincrement;
        if( cflyer->position[2] > -1.0)
	  {
	    reset_flyer(lp, lp->flyers[loop]);
	  }
    }

    if (mi->fps_p) do_fps (mi);
    glFinish();
    glXSwapBuffers(dpy, window);
    handleGLerrors("draw");

}

XSCREENSAVER_MODULE ("Flyer", flyer)

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

/* make width of cloud and density of fog user controllable */

/* quad size should adjust to texture size automatically? */

/* make length of cloud variable according to size of bitmap
 *  (larger bitmaps will be visible further away) ?
 */

/* set cloud size according to window size (?) */


/* ------------------------------------------------------------ */

/* make a bunch of xbms :
 *
 * dollar sign, sierpinski, smiley face,
 * jolly roger, spider, fly from red book,
 * card suits, shoe, venus/mars, cigar,
 * integral, kitties,
 *
 */

/* make motion blurring option */

/* make smooth color and color cycling option */


/* consider a two component texture (rather than four),
 * which uses R and A values only
 */

/* non-square bitmaps get stretched in the short direction--
 * currently left this as the user's problem, maybe can stay that way
 */
