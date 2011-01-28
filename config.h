/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */


/* config.h.in --- xtrascreenhacks, Copyright (c) 2011 Kelley Nielsen.
 *
 *  The best way to set these parameters is by running the included `configure'
 *  script.  That examines your system, and generates `config.h' from 
 *  `config.h.in'.
 *
 *  If something goes very wrong, you can edit `config.h' directly, but beware
 *  that your changes will be lost if you ever run `configure' again.
 */


/* Define this if gettimeofday() takes two arguments. */
#define GETTIMEOFDAY_TWO_ARGS 1

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#define HAVE_DIRENT_H 1

/* Define this if you have the X Double Buffer Extension. */
#define HAVE_DOUBLE_BUFFER_EXTENSION 1

/* Define to 1 if you have the `dup2' function. */
#define HAVE_DUP2 1

/* Define to 1 if you have the `ftime' function. */
#define HAVE_FTIME 1

/* Define this if you have the gettimeofday function. */
#define HAVE_GETTIMEOFDAY 1

/* Define this if you have OpenGL. Some of the demos require it, so if you
   don't have it, then those particular demos won't be built. (This won't
   affect the screen saver as a whole.) */
#define HAVE_GL 1

/* Define this if your version of OpenGL has the glBindTexture() routine. This
   is the case for OpenGL 1.1, but not for OpenGL 1.0. */
#define HAVE_GLBINDTEXTURE 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define to 1 if you have the `X11' library (-lX11). */
#define HAVE_LIBX11 1

/* Define to 1 if you have the `Xext' library (-lXext). */
#define HAVE_LIBXEXT 1

/* Define to 1 if you have the `Xt' library (-lXt). */
#define HAVE_LIBXT 1

/* Define to 1 if you have the <math.h> header file. */
#define HAVE_MATH_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define this if you have OpenGL, but it's the MesaGL variant. (The libraries
   have different names.) (HAVE_GL should be defined too.) */
#define HAVE_MESA_GL 1

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
/* #undef HAVE_NDIR_H */

/* Define to 1 if you have the `putenv' function. */
#define HAVE_PUTENV 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the `sqrt' function. */
#define HAVE_SQRT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasecmp' function. */
#define HAVE_STRCASECMP 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strncasecmp' function. */
#define HAVE_STRNCASECMP 1

/* Define to 1 if you have the `strpbrk' function. */
#define HAVE_STRPBRK 1

/* Define to 1 if you have the `strrchr' function. */
#define HAVE_STRRCHR 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_DIR_H */

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
/* #undef HAVE_SYS_NDIR_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `uname' function. */
#define HAVE_UNAME 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <values.h> header file. */
#define HAVE_VALUES_H 1

/* Define this if you have the Xmu library. This is standard part of X, and if
   your vendor doesn't ship it, you should report that as a bug. */
#define HAVE_XMU 1

/* Define this if you have the X Shared Memory Extension. */
#define HAVE_XSHM_EXTENSION 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "shegeek-dev@comcast.net"

/* Define to the full name of this package. */
#define PACKAGE_NAME "XtraScreenHacks"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "XtraScreenHacks 0.7"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "xtrascreenhacks"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.7"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#define inline __inline__
#endif

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef pid_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */
