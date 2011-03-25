

#include <stdlib.h>
#include "clovermap.h"

#define	MIN(x, y)	((x) < (y))?(x):(y)
#define	MAX(x, y)	((x) > (y))?(x):(y)
#define	CEIL(a, b)	((a)+(b)-1)/(b)
#define	FLOOR(a, b)	CEIL((a)-(b), (b))

#define getPixel(x, y)          bits[(y)*width+(x)]
#define putPixel(x, y, v)       bits[(y)*width+(x)] = (v)%nColors;
#define putRawPixel(x, y, v)    bits[(y)*width+(x)] = (v);
#define plot(x, y)      putPixel((x), (y), getPixel((x), (y))+1)

static void 
clipLine(int * bits, int nColors, int width, int height,  
	 int x0, int y0, int xn, int yn, int xe, int ye, int xf, int yf);
static void
line(int * bits, int nColors, int width, int height,  int fun, 
     int x0, int y0, int dx, int dy, int xe, int ye, int xf, int yf);

/*
 * Basically the algorithm is to draw a series of Bresenham lines from the
 * center.  The "interference pattern" is built by incrementing the pixel value
 * of (x,y) every time it's touched; the resulting pattern is a product of the
 * vagaries of integer arithmetic.
 */
void
cloverImage(int * bits, int nColors, int width, int height, int r)
{
        int     maxX, maxY, midX, midY, x, f, y;
        int     val, yy, x1, y1;
        int     i, o;
        int    *b;

        maxX = width - 1;
        maxY = height - 1;
        midX = maxX / 2;
        midY = maxY / 2;

        for (y = 0; y < height; y++) {
                b = &bits[y*width];
                for (x = 0; x < width; x++)
                        *b++ = nColors;         /* fill in background */
        }


        /*
         * Fill in the first semi-quadrant.
         */

        x = r;
        f = 0;
        for (y = 0; y < x; y++) {
                if (f > x) {
                        x--;
                        f = f-x - (x-1);
                }
                clipLine(bits, nColors, width, height,  midX, midY, x+midX, y+midY, 0, 0, maxX, maxY);
                f = f+y + y+1;
        }


      /*
         * Copy to the other seven, adjusting the horizontal and diagonal.
         */

        for (x = midX; x < maxX; x++) {
/*              putPixel(x, midY, (getPixel(x, midY) << 1) - 1);*/
                if (x - midX + midY <= maxY)
                        putPixel(x, x-midX+midY, 
                                (getPixel(x, x-midX+midY) << 1) - 1);
                yy = MIN(maxY, x + midY - midX);
                for (y = midY; y <= yy; y++) {
                        val = getPixel(x, y);
                        x1 = x;
                        y1 = y;
                        for (i = 0; i < 4; i++) {
                                if ((y1 < maxY) && (y1 > 0)) {
                                        putPixel(midX + midX - x1, y1, val);
                                        putPixel(x1, y1, val);
                                }
                                o = x1;
                                x1 = midX + midY - y1;
                                y1 = midY + o - midX;
                        }
                }
        }
}

/*
 * (xe, ye) and (xf, yf) are the corners of a rectangle to clip a line to.
 * (x0, y0) and (xn, yn) are the endpoints of the line to clip.
 * The function argument that's being computed is the semi-quadrant;
 *  dx and dy are used to determine whether we're above or below the diagonal,
 *  since (x0, y0) is always the midpoint of the pattern.
 * (The LispM has the origin at lower left, instead of upper left, so
 * the numbers don't correspond to the normal Cartesian plane quadrants.)
 *
 * This routine is very general, but the calling code only builds lines in the
 * first semi-quadrant and then copies them everywhere else.
 */

static void 
clipLine(int * bits, int nColors, int width, int height,  int x0, int y0, int xn, int yn, int xe, int ye, int xf, int yf)
{
        int     dx, dy;

        dx = abs(xn - x0);
        dy = abs(yn - y0);

        if (xn > x0) {                          /* moving right */
                if (yn >= y0) {                 /* moving up */
                        if (dx > dy)            /* below diagonal */
			  line(bits, nColors, width, height, 0, x0, y0, dx, dy, xe, ye, xf, yf);
                        else
                                line(bits, nColors, width, height,  1, y0, x0, dy, dx, ye, xe, yf, xf);
                } else {
                        if (dx > dy)
                                line(bits, nColors, width, height,  7, x0, -y0, dx, dy, xe, -yf, xf, -ye);
                        else
                                line(bits, nColors, width, height,  6, -y0, x0, dy, dx, -yf, xe, -ye, xf);
                }
        } else {
                if (yn >= y0) {
                        if (dx > dy)
                                line(bits, nColors, width, height,  3, -x0, y0, dx, dy, -xf, ye, -xe, yf);
                        else
                                line(bits, nColors, width, height,  2, y0, -x0, dy, dx, ye, -xf, yf, -xe);
                } else {
                        if (dx > dy)
                                line(bits, nColors, width, height,  4, -x0, -y0, dx, dy, -xf, -yf, -xe, -ye);
                        else
                                line(bits, nColors, width, height,  5, -y0, -x0, dy, dx, -yf, -xf, -ye, -xe);
                }
        }
}


/*
 * Clip symmetric segment (x0, y0) thru (xn, yn) to the rectangle 
 * (xe, ye) < (xf, yf).
 *
 * The original says:
 *
 * "This routine incorrectly assumes that the subsegment starts prior to the
 * midpoint of the supersegment.  The 'divide for nearest integer' (i.e.,
 * divide for remainder of minimum magnitude), which is simulated by the FLOOR
 * and CEIL of num and (dx <<1), always rounds up on the half integer case, but
 * should round down (for symmetry) if startup is in 2nd half. It would be
 * nice to have these other flavors of divide.'
 */
static void
line(int * bits, int nColors, int width, int height,  int fun, int x0, int y0, int dx, int dy, int xe, int ye, int xf, int yf)
{
        int     x, num, lx;
        int     xx, y, x00, f;
        int     x11;

        x = MAX(x0,  MAX(xe, 
                        (dy == 0)? xe :
                                   x0 + CEIL(dx * (((ye - y0)<<1) - 1), 
                                                (dy << 1))));
        num = dx + 2*dy*(x - x0);
        lx = MIN(xf, (dy == 0) ? xf :
                                x0 + CEIL(dx * (((yf - y0)<<1) - 1), 
                                                (dy << 1)));
        xx = MIN(lx, x0 + (dx>>1));
        y = y0 + FLOOR(num, (dx<<1));
        f = (FLOOR(num, (dx<<1)) - dx) >> 1;

        for (x00 = x; x00 < xx; x00++,f+=dy) {
                if (f+f > dx) {
                        f -= dx;
                        y++;
                }
                switch(fun) {
                case 0: plot(x00, y);   break;
                case 1: plot(y, x00);   break;
                case 2: plot(-y, x00);  break;
                case 3: plot(-x00, y);  break;
                case 4: plot(-x00, -y); break;
                case 5: plot(-y, -x00); break;
                case 6: plot(y, -x00);  break;
                case 7: plot(x00, -y);  break;
                }
        }

        for (x11 = x00; x11 < lx; x11++, f+=dy) {
                if (f + f > dx) {
                        f -= dx;
                        y++;
                }
                switch(fun) {
                case 0: plot(x11, y);   break;
                case 1: plot(y, x11);   break;
                case 2: plot(-y, x11);  break;
                case 3: plot(-x11, y);  break;
                case 4: plot(-x11, -y); break;
                case 5: plot(-y, -x11); break;
                case 6: plot(y, -x11);  break;
                case 7: plot(x11, -y);  break;
                }
        }
}

