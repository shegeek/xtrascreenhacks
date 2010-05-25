/* xbmread.c  OpenGL read an xbm file                                       */
/*            given a filename.xbm, rgba, back_rgba,                        */
/*            returns width, height, rgba pixels                            */
/*     int width; int height; int RGBA; 0,0,0,0 used for background         */
/*     unsigned char pixels[4*width*height];                                */
/* call: status = xbmread("your-filename", rgba_foreground, rgba_background,*/
/*                         &width, &height, pixels);                        */
/* status==0 for OK                                                         */

/* changes by Kelley Nielsen 2009:
*
*  unused variables commented out
*  accepts an array of GLuint rather than uchar
*  checks for array overflow, size parameter added
*  xbmread.h added
*/


                                                   

#include "xbmread.h"

static FILE *inp;        /* Pointer to the input FILE stream */
static int debug = 0;    /* set to 1 for full debug printout */
static int nrgba = 0;
/* static int lineno = -1; */
/* static int aindex = 0; */
static int lwidth = 0;
static int lheight = 0;
/* static int kwidth = 0; */
static /*  unsigned  */char line[256];
static int hexval(char ch);


/* int xbmread(char filename[], int rgba, int back_rgba, */
/*             int *width, int *height, int rgbapix[])  */
int xbmread(char filename[], GLuint rgba, GLuint back_rgba, 
            int *width, int *height, int maxsize, GLuint  rgbapix[])
{
  int status = 0;
  int lineno = -1;
/*   int i, j, k, nbit; */
  int i, k;
/*   char h1, h2, ch, bits[8]; */
  GLuint h1, h2, ch;
  char * p;
  
  nrgba = 0;
/*   nbit = 0; */

  if(debug)printf("Display the header info within a XBM image file \n");

  /* Open the XBM image file */
  if ((inp = fopen(filename, "r")) == (FILE *)NULL)
  {
    if(debug) printf("xbmread: Cannot open file %s\n", filename);
    return -1;
  }

  /* Read the XBM image file header information */
  while(fgets(line, 255, inp)!=NULL)
  {
    if(debug) printf("%s", line);
    if(strlen(line)<5) continue; /* ignore junk lines */
    if(line[0]=='#')
    {
      if((p=strstr(line, "width"))!=NULL)
      {
        lwidth = atoi(p+5);
        if(debug) printf("found width=%d \n", lwidth);
      }
      else if((p=strstr(line, "height"))!=NULL)
      {
        lheight = atoi(p+6);
        if(debug) printf("found height=%d \n", lheight);
      }
      if ((lwidth * lheight) > (maxsize * maxsize))
	{
	  lwidth = lheight = 0;
          return -1;
        }
    }
    else if((p=strstr(line, "static"))!=NULL)
    {
      lineno = lwidth * (lheight-1);
/*       if(debug) printf("found static \n"); */
    }
    else /* find  0xhh, sets of 8 bits */
    {
      for(i=0; i<strlen(line)-4; i++)
      {
        if(line[i]=='{') continue;
        if(line[i]==' ') continue;
        if(line[i]=='0' && (line[i+1]=='x' || line[i+1]=='X'))
        {
          h1 = hexval(line[i+2]);
          h2 = hexval(line[i+3]);
          ch = h1*16+h2;
          if (debug) printf("ch is %x\n", ch);
          for( k=0; k<8; k++)
          {
            rgbapix[lineno+nrgba] = back_rgba;
            if( ch&(1<<k) ) rgbapix[lineno+nrgba] = rgba;
            nrgba++;
            if((nrgba)==lwidth)
            {
              nrgba = 0;
              lineno = lineno - lwidth;
              break; /* ignore left over bits */
            }
          }
          i = i+4; /* also skip comma */
        }
      }
    }  
  }
  fclose(inp);

  *width  = lwidth;
  *height = lheight; 
  return status;
} /* end xbmread */


static int hexval(char ch)
{
  static char hex[] = "0123456789ABCDEFabcdef";
  char *p;
  int val = 0;

  p = strchr(hex, ch);
  val = p-hex;
  val = (val>15)?val-6:val;
  return val;
} /* end hexval */



/* ------------------------------------------------- */

/* shegeek's goals for xbmread
 *
 * get rid of globals
 * make sure valid xbm is checked for and handled in every possible way
 * make sure is current with current & past xbm specs
 * spin off a version to work on an #included xbm
 *  -> is in flyer.c, can probably stay there
 * possibly pad image with background to make dimensions 2^n
 * possibly/optionally use RA colors rather than RGBA
 * write up credits/changes, contact Prof. Squire about it
 *  ->changed so far: image array is of gluint rather than uchar,
 *    accompanying vars changed to match,
 *    accepts size parameter to check for overflow,
 *    unused vars commented out,
 *    .h file added (no header or anything yet)
 */
