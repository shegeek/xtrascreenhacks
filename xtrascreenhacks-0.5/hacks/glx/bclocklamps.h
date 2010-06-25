#ifndef lamps_H
#define lamps_H

#define MIN1OFFSET 0
#define MIN5OFFSET 10
#define HR1OFFSET 30
#define HR5OFFSET 40
#define SECOFFSET 50

#define FOURBANKWIDTH 9.25/2
#define ELEVENBANKWIDTH 6.8/11
#define LIGHTBANKHEIGHT 1.7

/* for the Boolean type */
/* #include "screenhackI.h" */
#include <X11/Xdefs.h>

GLint makelampdlists(Bool ismono);

#endif

