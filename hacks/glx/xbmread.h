#ifndef _XBMREAD_H_
#define _XBMREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* put guards */
#include <GL/gl.h>



int xbmread(char filename[], GLuint rgba, GLuint back_rgba, 
            int *width, int *height, int maxsize, GLuint  rgbapix[]);

#endif
