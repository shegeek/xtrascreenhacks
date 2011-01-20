 # configure.in --- xtrascreenhacks, Copyright (c) 2009 by Kelley Nielsen
#                  adapted from the original (c) 1997-2009 by Jamie Zawinski.
#

# there is a bunch of stuff left in here from xscreensaver that
# are not presently being used, but that may be used in the future.
# gdk-pixbuf (for xpm-pixmap/ximage), gle, jpeg libs, xpm libs.
# if xpm libs are restored, make sure tests and warning messages work.
# also, there is a bunch of xpm related stuff still in the makefiles.

# remaining problems:
#
# does not detect mesa version number correctly,
# probably because "preferred_mesagl" is 3.4 but 7.9 is current
# (does "version" refer to a mesa specific thing, or opengl spec?)
#
# what else in this file is moldy old?
#
# should fail immediately, not warn and continue, if ansi c check fails
#
# what is the deal with bc?
#
# don't think anything having to do with x app defaults is needed--
# is only for the xscreensaver.ad file
# (in case it is, help string was removed from with-configdir and placed
# in AC_ARG_WITH(x-app-defaults), where for some reason,
# it does not get printed with configure --help)
#
# configure.scan suggests AC_PROG_CXX, but AFAIK there is no c++ in here
# the OSX stuff wants AC_HEADER_STDBOOL, when that gets dealt with
# utils/grabscreen.c wants AC_FUNC_FORK
# generally wanted: AC_FUNC_MALLOC and AC_FUNC_REALLOC


AC_PREREQ(2.67)
AC_INIT([XtraScreenHacks], [0.7], [shegeek-dev@comcast.net])
AC_CONFIG_SRCDIR(hacks/screenhack.c)
AC_CONFIG_HEADERS(config.h)

echo "current directory: `pwd`"
echo "command line was: $0 $@"

###############################################################################
#
#       Autoheader stuff
#
###############################################################################

AH_TOP([
/* config.h.in --- xtrascreenhacks, Copyright (c) 2011 Kelley Nielsen.
 *
 *  The best way to set these parameters is by running the included `configure'
 *  script.  That examines your system, and generates `config.h' from 
 *  `config.h.in'.
 *
 *  If something goes very wrong, you can edit `config.h' directly, but beware
 *  that your changes will be lost if you ever run `configure' again.
 */
])


#AH_TEMPLATE([HAVE_XPM],
#	    [Define this if you have the XPM library installed.  Some of the
#	    demos can make use of this if it is available.])

#AH_TEMPLATE([HAVE_GDK_PIXBUF],
#	    [Define this if you have the GDK_Pixbuf library installed.  Some
#	    of the demos can make use of this if it is available.])

#AH_TEMPLATE([HAVE_JPEGLIB],
#	    [Define this if you have the Independent JPEG Group's JPEG
#	    library installed.  Some of the demos can make use of this if it
#	    is available.])

AH_TEMPLATE([HAVE_XMU],
	    [Define this if you have the Xmu library.  This is standard part
	    of X, and if your vendor doesn't ship it, you should report that
	    as a bug.])

AH_TEMPLATE([HAVE_GL],
	    [Define this if you have OpenGL.  Some of the demos require it,
	    so if you don't have it, then those particular demos won't be
	    built.  (This won't affect the screen saver as a whole.)])

AH_TEMPLATE([HAVE_MESA_GL],
	    [Define this if you have OpenGL, but it's the MesaGL variant. 
	    (The libraries have different names.) (HAVE_GL should be defined
	    too.)])

AH_TEMPLATE([HAVE_GLBINDTEXTURE],
	    [Define this if your version of OpenGL has the glBindTexture()
	    routine.  This is the case for OpenGL 1.1, but not for OpenGL
	    1.0.])

#AH_TEMPLATE([HAVE_GLE],
#	    [Define this if you have the -lgle and -lmatrix libraries (GL 
#	    extrusion.)])

#AH_TEMPLATE([HAVE_GLE3],
#        [Define this if you have the -lgle from GLE version 3])

AH_TEMPLATE([HAVE_XSHM_EXTENSION],
	    [Define this if you have the X Shared Memory Extension.])

AH_TEMPLATE([HAVE_DOUBLE_BUFFER_EXTENSION],
	    [Define this if you have the X Double Buffer Extension.])

AH_TEMPLATE([HAVE_GETTIMEOFDAY],
	    [Define this if you have the gettimeofday function.])

AH_TEMPLATE([GETTIMEOFDAY_TWO_ARGS],
	    [Define this if gettimeofday() takes two arguments.])


# After checking to see that --srcdir is correct (which AC_INIT does)
# check for some random other files that come later in the tar file,
# to make sure everything is here.
#
for d in utils hacks hacks/glx ; do
  f=$srcdir/$d/Makefile.in
  if test \! -r $f ; then
    echo ""
    echo "ERROR: The package is incomplete: $f does not exist."
    echo "       This probably means that your download was truncated."
    echo ""
    exit 1
  fi
done

###############################################################################
#
#       Function to figure out how to run the compiler.
#
###############################################################################

AC_DEFUN([AC_PROG_CC_ANSI],
 [AC_REQUIRE([AC_PROG_CC])

#  echo "Hostname is: $host"
#  echo "ac prog cc results in $GCC"
  if test -z "$GCC"; then
    # not using GCC
    AC_MSG_CHECKING(how to request ANSI compilation)
    case "$host" in
      *-hpux* )
        AC_MSG_RESULT(HPUX: adding -Ae)
        CC="$CC -Ae"
      ;;
      *-aix* )
        AC_MSG_RESULT(AIX: adding -qlanglvl=ansi -qhalt=e)
        CC="$CC -qlanglvl=ansi -qhalt=e"
      ;;
      *-dec-* )
        AC_MSG_RESULT(DEC: adding -std1 -ieee)
        CC="$CC -std1"
      ;;
      *)
        AC_MSG_RESULT(no idea)
      ;;
    esac
  else
    # using GCC
    case "$host" in
      *-solaris*)
        AC_MSG_RESULT(Solaris: adding -D__EXTENSIONS__)
        CC="$CC -D__EXTENSIONS__"
      ;;
    esac
  fi

  OBJCC="$CC"

  AC_MSG_CHECKING([whether the compiler works on ANSI C])
  AC_RUN_IFELSE(AC_LANG_SOURCE([ main(int ac, char **av) { return 0; } ]),
     [AC_MSG_RESULT(yes)],
# AC_MSG_FAILURE is supposed to be better for this
     [AC_MSG_RESULT(no)
     AC_MSG_ERROR(Couldn't build even a trivial ANSI C program: check CC.)],
     [AC_MSG_ERROR(Couldn't build even a trivial ANSI C program: check CC.)])

  if test -n "$GCC"; then
    AC_MSG_RESULT(Turning on gcc compiler warnings.)
    CC="$CC -pedantic -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes"
    OBJCC="$OBJCC -Wall"
    # supposedly gcc 3.4 will have "-Wdeclaration-after-statement"
    # and then perhaps we can do without -pedantic?
  else
    case "$host" in
      *-irix5* |*-irix6.[0-3]* )
        AC_MSG_RESULT(Turning on SGI compiler warnings.)
        CC="$CC -fullwarn -use_readonly_const -rdata_shared -g3"
      ;;
#     *-dec-osf* )
#       if test -z "$GCC"; then
#         AC_MSG_RESULT(Turning on DEC C compiler warnings.)
#         CC="$CC -migrate -w0 -verbose -warnprotos"
#       fi
#     ;;
    esac
  fi
])


###############################################################################
#
#       Functions to figure out how to disable // comments in ANSI C code.
#
#       (With recent gcc, this is done with "-std=c89".  With older gcc, this
#       is done by passing "-lang-c89" to cpp, by passing "-Wp,-lang-c89" to
#       gcc.  Old gcc doesn't support -std, and new gcc doesn't support -lang.
#       so much for compatibility!)
#
#       UPDATE: apparently there is NO WAY to tell gcc 3.2.2 to require that
#       declarations preceed statements, without resorting to "-pedantic".
#       This means that there is no way to get gcc3 to issue warnings that
#       ensure that your code complies with the ANSI/ISO C89 standard, without
#       also drowning in totally useless warnings.  Thank you master may I
#       have another.
#
#       So, I give up, let's just use -pedantic.
#
###############################################################################

AC_DEFUN([AC_GCC_ACCEPTS_STD],
 [if test -n "$GCC"; then
   AC_CACHE_CHECK([whether gcc accepts -std],
     ac_cv_gcc_accepts_std,
    [if ( ( gcc -E -std=c89 - </dev/null >/dev/null ) 2>&1 | \
          grep unrecognized >/dev/null ); then
       ac_cv_gcc_accepts_std=no
     else
       ac_cv_gcc_accepts_std=yes
     fi])
   ac_gcc_accepts_std="$ac_cv_gcc_accepts_std"
  fi
])

AC_DEFUN([AC_NO_CPLUSPLUS_COMMENTS_IN_C_CODE],
 [if test -n "$GCC"; then
   AC_GCC_ACCEPTS_STD
   AC_MSG_RESULT(Disabling C++ comments in ANSI C code.)
   #
   # The reason that // comments are banned from xscreensaver is that gcc is
   # basically the only compiler in the world that supports them in C code.
   # All other vendors support them only in their C++ compilers, not in their
   # ANSI C compilers.  This means that it's a portability problem: every time
   # these comments have snuck into the xscreensaver source code, I've gotten
   # complaints about it the next day.  So we turn off support for them in gcc
   # as well to prevent them from accidentially slipping in.
   #
   if test "$ac_gcc_accepts_std" = yes ; then
     #
     # -std=c89 defines __STRICT_ANSI__, which we don't want.
     # (That appears to be the only additional preprocessor symbol
     # it defines, in addition to the syntax changes it makes.)
     #
     # -std=gnu89 is no good, because // comments were a GNU extension
     # before they were in the ANSI C 99 spec...  (gcc 2.96 permits //
     # with -std=gnu89 but not with -std=c89.)
     #
     CC="$CC -std=c89 -U__STRICT_ANSI__"
   else
     # The old way:
     CC="$CC -Wp,-lang-c89"
   fi
  fi
])


###############################################################################
#
#       Function to figure out how to turn off Objective C on MacOS X.
#       (We have to do this to work around an Apple-specific gcc bug.)
#
###############################################################################

AC_DEFUN([AC_GCC_ACCEPTS_NO_CPP_PRECOMP],
 [if test -n "$GCC"; then
   AC_CACHE_CHECK([whether gcc accepts -no-cpp-precomp],
     ac_cv_gcc_accepts_no_cpp_precomp,
    [if ( ( gcc -E -no-cpp-precomp - </dev/null >/dev/null ) 2>&1 | \
          grep unrecognized >/dev/null ); then
       ac_cv_gcc_accepts_no_cpp_precomp=no
     else
       ac_cv_gcc_accepts_no_cpp_precomp=yes
     fi])
   ac_gcc_accepts_no_cpp_precomp="$ac_cv_gcc_accepts_no_cpp_precomp"
  fi
])

AC_DEFUN([AC_NO_OBJECTIVE_C],
 [if test -n "$GCC"; then
   AC_GCC_ACCEPTS_NO_CPP_PRECOMP
   if test "$ac_gcc_accepts_no_cpp_precomp" = yes ; then
     AC_MSG_RESULT(Disabling Objective C extensions in ANSI C code.)
     CC="$CC -no-cpp-precomp"
   fi
  fi
])


###############################################################################
#
#       Function to figure out how to create directory trees.
#
###############################################################################

AC_DEFUN([AC_PROG_INSTALL_DIRS],
 [AC_CACHE_CHECK([whether "\${INSTALL} -d" creates intermediate directories],
    ac_cv_install_d_creates_dirs,
    [ac_cv_install_d_creates_dirs=no
     rm -rf conftestdir
     if mkdir conftestdir; then
       cd conftestdir 2>/dev/null
       ${INSTALL} -d `pwd`/dir1/dir2 >/dev/null 2>&1
       if test -d dir1/dir2/. ; then
         ac_cv_install_d_creates_dirs=yes
       fi
       cd .. 2>/dev/null
       rm -rf conftestdir
     fi
    ])

  if test "$ac_cv_install_d_creates_dirs" = no ; then
    AC_CACHE_CHECK([whether "mkdir -p" creates intermediate directories],
      ac_cv_mkdir_p_creates_dirs,
      [ac_cv_mkdir_p_creates_dirs=no
       rm -rf conftestdir
       if mkdir conftestdir; then
         cd conftestdir 2>/dev/null
         mkdir -p dir1/dir2 >/dev/null 2>&1
         if test -d dir1/dir2/. ; then
           ac_cv_mkdir_p_creates_dirs=yes
         fi
         cd .. 2>/dev/null
         rm -rf conftestdir
       fi
      ])
  fi

  if test "$ac_cv_install_d_creates_dirs" = yes ; then
    INSTALL_DIRS='${INSTALL} -d'
  elif test "$ac_cv_mkdir_p_creates_dirs" = yes ; then
    INSTALL_DIRS='mkdir -p'
  else
    # any other ideas?
    INSTALL_DIRS='${INSTALL} -d'
  fi
])


###############################################################################
#
#       Function to check whether gettimeofday() exists, and how to call it.
#	This may define HAVE_GETTIMEOFDAY and GETTIMEOFDAY_TWO_ARGS.
#
###############################################################################

AC_DEFUN([AC_GETTIMEOFDAY_ARGS],
 [AC_MSG_CHECKING(how to call gettimeofday)
  AC_CACHE_VAL(ac_cv_gettimeofday_args,
   [AC_TRY_COMPILE([#include <stdlib.h>
                    #include <sys/time.h>],
                   [struct timeval tv; struct timezone tzp;
                    gettimeofday(&tv, &tzp);],
                   [ac_gettimeofday_args=2],
                   [AC_TRY_COMPILE([#include <stdlib.h>
                                    #include <sys/time.h>],
                                   [struct timeval tv; gettimeofday(&tv);],
                                   [ac_gettimeofday_args=1],
                                   [ac_gettimeofday_args=0])])
    ac_cv_gettimeofday_args=$ac_gettimeofday_args])
  ac_gettimeofday_args=$ac_cv_gettimeofday_args
  if test "$ac_gettimeofday_args" = 1 ; then
    AC_DEFINE(HAVE_GETTIMEOFDAY)
    AC_MSG_RESULT(one argument)
  elif test "$ac_gettimeofday_args" = 2 ; then
    AC_DEFINE(HAVE_GETTIMEOFDAY)
    AC_DEFINE(GETTIMEOFDAY_TWO_ARGS)
    AC_MSG_RESULT(two arguments)
  else
    AC_MSG_RESULT(unknown)
  fi
])


###############################################################################
#
#       Function to demand "bc".  Losers.
#
###############################################################################

#AC_DEFUN([AC_DEMAND_BC],
# [ac_bc_result=`echo 6+9 | bc 2>/dev/null`
#  AC_MSG_CHECKING([for bc])
#  if test "$ac_bc_result" = "15" ; then
#    AC_MSG_RESULT(yes)
#  else
#    AC_MSG_RESULT(no)
#    echo ''
#    AC_MSG_ERROR([Your system doesn't have \"bc\", which has been a standard
#                  part of Unix since the 1970s.  Come back when your vendor
#                  has grown a clue.])
#  fi
# ])



###############################################################################
#
#       Functions to check for various X11 crap.
#
###############################################################################

# Try and find the app-defaults directory.
# It sucks that autoconf doesn't do this already...
#
AC_DEFUN([AC_PATH_X_APP_DEFAULTS_XMKMF],[
  rm -fr conftestdir
  if mkdir conftestdir; then
    cd conftestdir 2>/dev/null
    # Make sure to not put "make" in the Imakefile rules, since we grep it out.
    cat > Imakefile <<'EOF'
acfindx:
	@echo 'ac_x_app_defaults="${XAPPLOADDIR}"'
EOF
    if (xmkmf) >/dev/null 2>&1 && test -f Makefile; then
      # GNU make sometimes prints "make[1]: Entering...", which'd confuse us.
      eval `${MAKE-make} acfindx 2>/dev/null | grep -v make`
    fi
    cd .. 2>/dev/null
    rm -fr conftestdir
  fi])

###############################################################################
#
#       Handle the --with-x-app-defaults option HERE
#
###############################################################################

AC_ARG_WITH([x-app-defaults],
[  --with-x-app-defaults=DIR
			   Where to install xscreensaver configuration file.
],
	[ac_cv_x_app_defaults="$withval"],
	[eval ac_x_app_defaults="$withval"])


AC_DEFUN([AC_PATH_X_APP_DEFAULTS_DIRECT],[
  # Look for the directory under a standard set of common directories.
  # Check X11 before X11Rn because it's often a symlink to the current release.
  for ac_dir in                                 \
    /usr/X11/lib/app-defaults                   \
    /usr/X11R6/lib/app-defaults                 \
    /usr/X11R6/lib/X11/app-defaults             \
    /usr/X11R5/lib/app-defaults                 \
    /usr/X11R5/lib/X11/app-defaults             \
    /usr/X11R4/lib/app-defaults                 \
    /usr/X11R4/lib/X11/app-defaults             \
                                                \
    /usr/lib/X11/app-defaults                   \
    /usr/lib/X11R6/app-defaults                 \
    /usr/lib/X11R5/app-defaults                 \
    /usr/lib/X11R4/app-defaults                 \
                                                \
    /usr/local/X11/lib/app-defaults             \
    /usr/local/X11R6/lib/app-defaults           \
    /usr/local/X11R5/lib/app-defaults           \
    /usr/local/X11R4/lib/app-defaults           \
                                                \
    /usr/local/lib/X11/app-defaults             \
    /usr/local/lib/X11R6/app-defaults           \
    /usr/local/lib/X11R6/X11/app-defaults       \
    /usr/local/lib/X11R5/app-defaults           \
    /usr/local/lib/X11R5/X11/app-defaults       \
    /usr/local/lib/X11R4/app-defaults           \
    /usr/local/lib/X11R4/X11/app-defaults       \
                                                \
    /usr/X386/lib/X11/app-defaults              \
    /usr/x386/lib/X11/app-defaults              \
    /usr/XFree86/lib/X11/app-defaults           \
                                                \
    /usr/lib/X11/app-defaults                   \
    /usr/local/lib/X11/app-defaults             \
    /usr/unsupported/lib/X11/app-defaults       \
    /usr/athena/lib/X11/app-defaults            \
    /usr/local/x11r5/lib/X11/app-defaults       \
    /usr/lpp/Xamples/lib/X11/app-defaults       \
    /lib/usr/lib/X11/app-defaults               \
                                                \
    /usr/openwin/lib/app-defaults               \
    /usr/openwin/lib/X11/app-defaults           \
    /usr/openwin/share/lib/app-defaults         \
    /usr/openwin/share/lib/X11/app-defaults     \
                                                \
    /X11R6/lib/app-defaults                     \
    /X11R5/lib/app-defaults                     \
    /X11R4/lib/app-defaults                     \
    ; \
  do
    if test -d "$ac_dir"; then
      ac_x_app_defaults=$ac_dir
      break
    fi
  done
])

AC_DEFUN([AC_PATH_X_APP_DEFAULTS],
  [AC_REQUIRE_CPP()
    AC_CACHE_CHECK([for X app-defaults directory], ac_cv_x_app_defaults,
     [AC_PATH_X_APP_DEFAULTS_XMKMF
      if test x"$ac_x_app_defaults" = x; then
        AC_PATH_X_APP_DEFAULTS_DIRECT
      fi
      if test x"$ac_x_app_defaults" = x; then
        ac_cv_x_app_defaults="/usr/lib/X11/app-defaults"
      else
        # Record where we found app-defaults for the cache.
        ac_cv_x_app_defaults="$ac_x_app_defaults"
      fi])
    eval ac_x_app_defaults="$ac_cv_x_app_defaults"])



# Random special-cases for X on certain pathological OSes.
# You know who you are.
#
AC_DEFUN([AC_X_RANDOM_PATHS],
 [case "$host" in
    *-hpux*)

      # The following arcana was gleaned from conversations with
      # Eric Schwartz <erics@col.hp.com>:
      #
      # On HPUX 10.x, the parts of X that HP considers "standard" live in
      # /usr/{include,lib}/X11R6/.  The parts that HP doesn't consider
      # "standard", notably, Xaw and Xmu, live in /usr/contrib/X11R6/.
      # Yet /usr/contrib/X11R6/ comes preinstalled on all HPUX systems.
      # Also, there are symlinks from /usr/include/ and /usr/lib/ into
      # /usr/{include,lib}/X11R6/, so that (if you don't use Xmu at all)
      # you don't need any -I or -L arguments.
      #
      # On HPUX 9.x, /usr/{include,lib}/X11R5/ and /usr/contrib/X11R5/
      # are the same division as 10.x.  However, there are no symlinks to
      # the X stuff from /usr/include/ and /usr/lib/, so -I and -L
      # arguments are always necessary.
      #
      # However, X11R6 was available on HPUX 9.x as a patch: if that
      # patch was installed, then all of X11R6 went in to
      # /usr/contrib/X11R6/ (there was no /usr/{include,lib}/X11R6/.)
      #
      # HPUX 8.x was the same as 9.x, but was X11R4 instead (I don't know
      # whether R5 was available as a patch; R6 undoubtedly was not.)
      #
      # So.  We try and use the highest numbered pair of
      # /usr/{include,lib}/X11R?/ and /usr/contrib/X11R?/{include,lib}/
      # that are available.  We do not mix and match different versions
      # of X.
      #
      # Question I still don't know the answer to: (do you?)
      #
      #   * On HPUX 9.x, where /usr/include/X11R5/ was standard, and
      #     /usr/contrib/X11R6/ could be installed as a patch, what was in
      #     that contrib directory?  Did it contain so-called "standard"
      #     X11R6, or did it include Xaw and Xmu as well?  If the former,
      #     where did one find Xaw and Xmu on 9.x R6 systems?  Would this
      #     be a situation where one had to reach into the R5 headers and
      #     libs to find Xmu?  That is, must both R6 and R5 directories
      #     be on the -I and -L lists in that case?
      #
      for version in X11R6 X11R5 X11R4 ; do
        # if either pair of directories exists...
        if test -d /usr/include/$version || test -d /usr/contrib/$version/include
        then
           # if contrib exists, use it...
           if test -d /usr/contrib/$version/include ; then
             X_CFLAGS="$X_CFLAGS -I/usr/contrib/$version/include"
             X_LIBS="$X_LIBS -L/usr/contrib/$version/lib"
           fi
           # if the "standard" one exists, use it.
           if test -d /usr/include/$version ; then
             X_CFLAGS="$X_CFLAGS -I/usr/include/$version"
             X_LIBS="$X_LIBS -L/usr/lib/$version"
           fi
           # since at least one of the pair exists, go no farther.
           break
        fi
      done


      # Now let's check for the pseudo-standard locations for OpenGL and XPM.
      #
      if test -d /opt/graphics/OpenGL/include ; then
        # HP-UX 10.20 puts it here
        X_CFLAGS="-I/opt/graphics/OpenGL/include $X_CFLAGS"
        X_LIBS="-L/opt/graphics/OpenGL/lib $X_LIBS"
      elif test -d /opt/Mesa/lib ; then
        X_CFLAGS="-I/opt/Mesa/include $X_CFLAGS"
        X_LIBS="-L/opt/Mesa/lib $X_LIBS"
      fi


#      if test -d /opt/xpm/lib/X11 ; then
#        X_CFLAGS="-I/opt/xpm/include $X_CFLAGS"
#        X_LIBS="-L/opt/xpm/lib/X11 $X_LIBS"
#      fi

      # On HPUX, default to installing in /opt/xscreensaver/ instead of
      # in /usr/local/, unless there is already an xscreensaver in
      # /usr/local/bin/.  This can be overridden with the --prefix arg
      # to configure.  I'm not sure this is the right thing to do, but
      # Richard Lloyd says so...
      #
      if test \! -x /usr/local/bin/xscreensaver ; then
        ac_default_prefix=/opt/xscreensaver
      fi

    ;;
    *-solaris*)

      # Thanks for not making xmkmf find this by default, pinheads.
      # And thanks for moving things around again, too.  Is this
      # really the standard location now?  What happened to the
      # joke that this kind of thing went in /opt?
      # cthomp says "answer: CDE (Common Disorganized Environment)"
      #
      if test -f /usr/dt/include/Xm/Xm.h ; then
        X_CFLAGS="$X_CFLAGS -I/usr/dt/include"
      fi

    ;;
    *-darwin*)

      # On MacOS X (10.x with "fink"), many things are under /sw/.
      #
      if test -d /sw/include ; then
        X_CFLAGS="-I/sw/include $X_CFLAGS"
        X_LIBS="-L/sw/lib $X_LIBS"
      fi
    ;;
  esac])



###############################################################################
#
#       Some utility functions to make checking for X things easier.
#
###############################################################################

# Like AC_CHECK_HEADER, but it uses the already-computed -I directories.
#
AC_DEFUN([AC_CHECK_X_HEADER], [
  ac_save_CPPFLAGS="$CPPFLAGS"
  if test \! -z "$includedir" ; then 
    CPPFLAGS="$CPPFLAGS -I$includedir"
  fi
  CPPFLAGS="$CPPFLAGS $X_CFLAGS"
  CPPFLAGS=`eval eval eval eval eval eval eval eval eval echo $CPPFLAGS`
  AC_CHECK_HEADER([$1],[$2],[$3],[$4])
  CPPFLAGS="$ac_save_CPPFLAGS"])

# Like AC_EGREP_HEADER, but it uses the already-computed -I directories.
#
AC_DEFUN([AC_EGREP_X_HEADER], [
  ac_save_CPPFLAGS="$CPPFLAGS"
  if test \! -z "$includedir" ; then 
    CPPFLAGS="$CPPFLAGS -I$includedir"
  fi
  CPPFLAGS="$CPPFLAGS $X_CFLAGS"
  CPPFLAGS=`eval eval eval eval eval eval eval eval eval echo $CPPFLAGS`
  AC_EGREP_HEADER([$1], [$2], [$3], [$4])
  CPPFLAGS="$ac_save_CPPFLAGS"])

# Like AC_TRY_COMPILE, but it uses the already-computed -I directories.
#
AC_DEFUN([AC_TRY_X_COMPILE], [
  ac_save_CPPFLAGS="$CPPFLAGS"
  if test \! -z "$includedir" ; then 
    CPPFLAGS="$CPPFLAGS -I$includedir"
  fi
  CPPFLAGS="$CPPFLAGS $X_CFLAGS"
  CPPFLAGS=`eval eval eval eval eval eval eval eval eval echo $CPPFLAGS`
  AC_TRY_COMPILE([$1], [$2], [$3], [$4])
  CPPFLAGS="$ac_save_CPPFLAGS"])


# Like AC_CHECK_LIB, but it uses the already-computed -I and -L directories.
# Use this sparingly; it probably doesn't work very well on X programs.
#
AC_DEFUN([AC_CHECK_X_LIB], [
  ac_save_CPPFLAGS="$CPPFLAGS"
  ac_save_LDFLAGS="$LDFLAGS"
#  ac_save_LIBS="$LIBS"

  if test \! -z "$includedir" ; then 
    CPPFLAGS="$CPPFLAGS -I$includedir"
  fi
  # note: $X_CFLAGS includes $x_includes
  CPPFLAGS="$CPPFLAGS $X_CFLAGS"

  if test \! -z "$libdir" ; then
    LDFLAGS="$LDFLAGS -L$libdir"
  fi
  # note: $X_LIBS includes $x_libraries
  LDFLAGS="$LDFLAGS $X_LIBS $X_EXTRA_LIBS"

  CPPFLAGS=`eval eval eval eval eval eval eval eval eval echo $CPPFLAGS`
  LDFLAGS=`eval eval eval eval eval eval eval eval eval echo $LDFLAGS`
  AC_CHECK_LIB([$1], [$2], [$3], [$4], [$5])
  CPPFLAGS="$ac_save_CPPFLAGS"
  LDFLAGS="$ac_save_LDFLAGS"
#  LIBS="$ac_save_LIBS"
  ])

# Like AC_TRY_RUN, but it uses the already-computed -I directories.
# (But not the -L directories!)
#
AC_DEFUN([AC_TRY_X_RUN], [
  ac_save_CPPFLAGS="$CPPFLAGS"
  if test \! -z "$includedir" ; then 
    CPPFLAGS="$CPPFLAGS -I$includedir"
  fi
  CPPFLAGS="$CPPFLAGS $X_CFLAGS"
  CPPFLAGS=`eval eval eval eval eval eval eval eval eval echo $CPPFLAGS`
  AC_TRY_RUN([$1], [$2], [$3], [$4])
  CPPFLAGS="$ac_save_CPPFLAGS"])



# Usage: HANDLE_X_PATH_ARG([variable_name],
#                          [--command-line-option],
#                          [descriptive string])
#
# All of the --with options take three forms:
#
#   --with-foo (or --with-foo=yes)
#   --without-foo (or --with-foo=no)
#   --with-foo=/DIR
#
# This function, HANDLE_X_PATH_ARG, deals with the /DIR case.  When it sees
# a directory (string beginning with a slash) it checks to see whether
# /DIR/include and /DIR/lib exist, and adds them to $X_CFLAGS and $X_LIBS
# as appropriate.
#
AC_DEFUN([HANDLE_X_PATH_ARG], [
   case "$[$1]" in
    yes) ;;
    no)  ;;

    /*)
     AC_MSG_CHECKING([for [$3] headers])
     d=$[$1]/include
     if test -d $d; then
       X_CFLAGS="-I$d $X_CFLAGS"
       AC_MSG_RESULT($d)
     else
       AC_MSG_RESULT(not found ($d: no such directory))
     fi

     AC_MSG_CHECKING([for [$3] libs])
     d=$[$1]/lib
     if test -d $d; then
       X_LIBS="-L$d $X_LIBS"
       AC_MSG_RESULT($d)
     else
       AC_MSG_RESULT(not found ($d: no such directory))
     fi

     # replace the directory string with "yes".
     [$1]_req="yes"
     [$1]=$[$1]_req
     ;;

    *)
     echo ""
     echo "error: argument to [$2] must be \"yes\", \"no\", or a directory."
     echo "       If it is a directory, then \`DIR/include' will be added to"
     echo "       the -I list, and \`DIR/lib' will be added to the -L list."
     exit 1
     ;;
   esac
  ])



###############################################################################
###############################################################################
#
#       End of function definitions.  Now start actually executing stuff.
#
###############################################################################
###############################################################################

# WTF!  autoconf emits this *way* too late.  Do it earlier.
test "x$prefix" = xNONE && prefix=$ac_default_prefix
test "x$exec_prefix" = xNONE && exec_prefix='${prefix}'


# random compiler setup
AC_CANONICAL_HOST
AC_PROG_CC_ANSI
AC_NO_CPLUSPLUS_COMMENTS_IN_C_CODE
AC_NO_OBJECTIVE_C
AC_PROG_CPP
AC_C_CONST
AC_C_INLINE
AC_EXEEXT
#AC_DEMAND_BC

# stuff for Makefiles
AC_PROG_INSTALL
AC_PROG_INSTALL_DIRS
AC_PROG_AWK
AC_PROG_LN_S
AC_PROG_MAKE_SET

# are FG and Fd real libraries, or did autoscan pick them up from ls args?
#AC_CHECK_LIB([FG], [main])
#AC_CHECK_LIB([Fd], [main])
AC_CHECK_LIB([X11], [XCreateGC])
AC_CHECK_LIB([Xext], [XdbeSwapBuffers])
AC_CHECK_LIB([Xt], [XtRealizeWidget])
AC_CHECK_LIB([m], [sqrt])


# By default, autoconf sets INSTALL_SCRIPT to '${INSTALL_PROGRAM}'.
# That's wrong: it should be set to '${INSTALL}', so that one can
# implement the "install-strip" target properly (strip executables,
# but do not try to strip scripts.)
#
INSTALL_SCRIPT='${INSTALL}'

# random libc stuff
AC_HEADER_STDC
AC_CHECK_HEADERS_ONCE([math.h sys/time.h unistd.h values.h])
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SIGNAL
# these two are supposedly obsolescent,
# although configure.scan suggests the sys_wait check.
#AC_HEADER_TIME
#AC_HEADER_SYS_WAIT
AC_HEADER_DIRENT
AC_GETTIMEOFDAY_ARGS
AC_CHECK_FUNCS([select uname putenv])
AC_CHECK_FUNCS([dup2 ftime memset sqrt strcasecmp strchr strdup ])
AC_CHECK_FUNCS([strncasecmp strpbrk strrchr strstr])

AC_PATH_XTRA

if test "$have_x" != yes; then
  AC_MSG_ERROR(Couldn't find X11 headers/libs.  Try `$0 --help'.)
fi

AC_PATH_X_APP_DEFAULTS
AC_X_RANDOM_PATHS
#AC_XPOINTER

AC_MSG_CHECKING(whether this is MacOS X)
  ac_macosx=no
  case "$host" in
    *-apple-darwin* )
      ac_macosx=yes
    ;;
  esac
AC_MSG_RESULT($ac_macosx)




###############################################################################
#
#       Check for -lXmu (some fucked up vendors don't ship it...)
#
###############################################################################

have_xmu=no
AC_CHECK_X_HEADER(X11/Xmu/Error.h, [have_xmu=yes],,
                  [#include <stdlib.h>
                   #include <stdio.h>
                   #include <X11/Intrinsic.h>])
if test "$have_xmu" = no ; then
  XMU_SRCS='$(UTILS_SRC)/xmu.c'
  XMU_OBJS='$(UTILS_BIN)/xmu.o'
  XMU_LIBS=''
else
  XMU_SRCS=''
  XMU_OBJS=''
  XMU_LIBS='-lXmu'
  AC_DEFINE(HAVE_XMU)
fi


###############################################################################
#
#       Check for the SunOS 4.1.x _get_wmShellWidgetClass bug.
#       See comp.windows.x FAQ question 124.  The right fix is to
#       get OpenWindows 3.0 patches 100512-02 and 100573-03.
#
###############################################################################

if test "$have_xmu" = yes ; then
  case "$host" in
    *-sunos4*)
    AC_CACHE_CHECK([for the SunOS 4.1.x _get_wmShellWidgetClass bug],
                   ac_cv_sunos_xmu_bug,
                   [ac_save_LDFLAGS="$LDFLAGS"
                    if test \! -z "$x_libraries" ; then
                      LDFLAGS="$LDFLAGS -L$x_libraries"
                    fi
                    # Note: this trick never works!  (Generally.)
                    # We're only getting away with using AC_TRY_LINK
                    # with X libraries because we know it's SunOS.
                    LDFLAGS="$LDFLAGS -lXmu -lXt -lX11 -lXext -lm"
                    AC_TRY_LINK(,,
                                [ac_cv_sunos_xmu_bug=no],
                                [ac_cv_sunos_xmu_bug=yes])
                    LDFLAGS="$ac_save_LDFLAGS"])
    if test "$ac_cv_sunos_xmu_bug" = yes ; then
      AC_CACHE_CHECK([whether the compiler understands -static], 
                     ac_cv_ld_static,
                     [ac_save_LDFLAGS="$LDFLAGS"
                      LDFLAGS="$LDFLAGS -static"
                      AC_TRY_LINK(,,[ac_cv_ld_static=yes],[ac_cv_ld_static=no])
                    LDFLAGS="$ac_save_LDFLAGS"])
      if test "$ac_cv_ld_static" = yes ; then
        LDFLAGS="$LDFLAGS -static"
      else
        LDFLAGS="$LDFLAGS -Bstatic"
      fi
    fi
    ;;
  esac
fi


###############################################################################
#
#       Handle the --with-hackdir option
#
###############################################################################

have_hackdir=yes
with_hackdir_req=unspecified
AC_ARG_WITH(hackdir,[

Installation options:
  --with-hackdir=DIR      Where to install the hundreds of demo executables.
                          Default: `PREFIX/libexec/xscreensaver/'],
  [with_hackdir="$withval"; with_hackdir_req="$withval"],[with_hackdir=yes])

if test x"$with_hackdir" = xyes; then
  HACKDIR='${libexecdir}/xscreensaver'
elif test x"$with_hackdir" = xno; then
  HACKDIR='${bindir}'
else
  HACKDIR=$with_hackdir
fi

# some of this stuff is repeated below...necessary?
# canonicalize slashes.
#HACKDIR=`echo "${HACKDIR}" | sed 's@/$@@;s@//*@/@g'`

# Expand HACKDIR as HACKDIR_FULL
#HACKDIR_FULL=`eval eval eval eval eval eval eval eval eval echo $HACKDIR`


###############################################################################
#
#       Handle the --with-configdir option
#       Help for --with-x-app-defaults option added..
#
###############################################################################

have_configdir=yes
with_configdir_req=unspecified
AC_ARG_WITH([configdir],
[  --with-configdir=DIR    Where to install the data files that describe each
                          of the display modes to the GUI.
                          Default: `PREFIX/share/xscreensaver/config/'
],
  [with_configdir="$withval"; with_configdir_req="$withval"],
  [with_configdir=yes])

if test x"$with_configdir" = xyes; then
  HACK_CONF_DIR='${datadir}/xscreensaver/config'
elif test x"$with_configdir" = xno; then
  echo "error: must be yes, or a pathname: --with-configdir=$with_configdir"
  exit 1
else
  # there must be a better way than this...
  if test -z "`echo $with_configdir | sed 's@^/.*@@'`" ; then
    # absolute path
    HACK_CONF_DIR=$with_configdir
  else
    # relative path
    HACK_CONF_DIR="\${exec_prefix}$with_configdir"
  fi
fi


###############################################################################
#
#       Handle the --with-imagedir option
#       Help for --with-x-app-defaults option added..
#       ...and removed, was the same as in configdir option above.
#
###############################################################################

have_imagedir=yes
with_imagedir_req=unspecified
AC_ARG_WITH([imagedir],
[  --with-imagedir=DIR    Where to install the image files that may be
                          used with the various hacks.
                          Default: `PREFIX/share/xtrascreenhacks/images/'
],
  [with_imagedir="$withval"; with_imagedir_req="$withval"],
  [with_imagedir=yes])

if test x"$with_imagedir" = xyes; then
  IMAGE_DIR='${datadir}/xtrascreenhacks/images'
elif test x"$with_imagedir" = xno; then
  echo "error: must be yes, or a pathname: --with-imagedir=$with_imagedir"
  exit 1
else
  # there must be a better way than this...
  if test -z "`echo $with_imagedir | sed 's@^/.*@@'`" ; then
    # absolute path
    IMAGE_DIR=$with_imagedir
  else
    # relative path
    IMAGE_DIR="\${exec_prefix}$with_imagedir"
  fi
fi

###############################################################################
#
#       Check for -lgtk (and Gnome stuff)
#
###############################################################################

# Find pkg-config... (need this for both gtk and gdk_pixbuf.)
# if the user specified --with-gtk=/foo/ then look there.
#
gtk_path="$PATH"
if test ! -z "$gtk_dir"; then
  # canonicalize slashes.
  foo=`echo "${gtk_dir}/bin" | sed 's@//*@/@g'`
  gtk_path="$foo:$gtk_path"
fi

AC_PATH_PROGS(pkg_config, pkg-config,, $gtk_path)

if test -z "$pkg_config" ; then
  AC_MSG_WARN([pkg-config not found!])
  pkg_config="false"
fi


# Utility function for running pkg-config-based tests...
#
pkgs=''
pkg_check_version() {
  if test "$ok" = yes ; then
    req="$1"
    min="$2"
    AC_MSG_CHECKING(for $req)
    if $pkg_config --exists "$req" ; then
      vers=`$pkg_config --modversion "$req"`
      if $pkg_config --exists "$req >= $min" ; then
        AC_MSG_RESULT($vers)
        pkgs="$pkgs $req"
        return 1
      else
        AC_MSG_RESULT($vers (wanted >= $min))
        ok=no
        return 0
      fi
    else
      AC_MSG_RESULT(no)
      ok=no
      return 0
    fi
  fi
}

  pkg_check_version      gdk-pixbuf-2.0  2.0.0
  pkg_check_version gdk-pixbuf-xlib-2.0  2.0.0


###############################################################################
#
#       Check for -lGL or -lMesaGL.
#
###############################################################################

have_gl=no
ac_have_mesa_gl=no
with_gl_req=unspecified
gl_halfassed=no
AC_ARG_WITH(gl,[
Graphics options:

  --with-gl               Build those demos which depend on OpenGL.],
  [with_gl="$withval"; with_gl_req="$withval"],[with_gl=yes])

HANDLE_X_PATH_ARG(with_gl, --with-gl, GL)

ac_mesagl_version=unknown
ac_mesagl_version_string=unknown

if test "$with_gl" = yes; then
  AC_CHECK_X_HEADER(GL/gl.h, have_gl=yes, have_gl=no)
  if test "$have_gl" = yes ; then
    AC_CHECK_X_HEADER(GL/glx.h, have_gl=yes, have_gl=no,
                      [#include <GL/gl.h>])
  fi

  # If we have the headers, try and figure out which vendor it's from.
  #
  if test "$have_gl" = yes ; then

    # We need to know whether it's MesaGL so that we know which libraries
    # to link against.
    #
    AC_CACHE_CHECK([whether GL is really MesaGL], ac_cv_have_mesa_gl,
      [ac_cv_have_mesa_gl=no
       if test "$ac_macosx" = no; then
         # WTF! MacOS 10.5.0 ships the Mesa GL headers!
         # It's not really Mesa, is it?
         AC_EGREP_X_HEADER(Mesa|MESA, GL/glx.h, [ac_cv_have_mesa_gl=yes])
       fi])
    ac_have_mesa_gl=$ac_cv_have_mesa_gl

    gl_lib_1=""
    GL_LIBS=""

    if test "$ac_macosx" = yes; then

      # Without these, every link against libGL gets a bunch of useless
      # warnings.
      #
      osx_crud="-bind_at_load -multiply_defined suppress"
      AC_MSG_RESULT(adding "$osx_crud" to GL_LIBS)
      GL_LIBS="$GL_LIBS $osx_crud"
      unset osx_crud

      # New lossage in 10.5.0: without this, we get:
      #   ld: cycle in dylib re-exports with /usr/X11/lib/libGL.dylib
      #
      osx_crud="/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib"
      osx_crud="-Wl,-dylib_file,${osx_crud}:${osx_crud}"

      AC_MSG_RESULT(adding "$osx_crud" to GL_LIBS)
      GL_LIBS="$GL_LIBS $osx_crud"
      unset osx_crud
    fi


    # Some versions of MesaGL are compiled to require -lpthread.
    # So if the Mesa headers exist, and -lpthread exists, then always
    # link -lpthread after the Mesa libs (be they named -lGL or -lMesaGL.)
    #
    if test "$ac_have_mesa_gl" = yes; then
      AC_CHECK_LIB(pthread, pthread_create, [GL_LIBS="-lpthread"], [],)
    fi


    # If we have Mesa headers, check to see if we can link against -lMesaGL.
    # If we don't have Mesa headers, or we don't have -lMesaGL, try -lGL.
    # Else, warn that GL is busted.  (We have the headers, but no libs.)
    #

    if test "$ac_have_mesa_gl" = yes ; then
      AC_CHECK_X_LIB(MesaGL, glXCreateContext, 
                     [gl_lib_1="MesaGL"
                      GL_LIBS="-lMesaGL -lMesaGLU $VIDMODE_LIBS $GL_LIBS"],
                     [], -lMesaGLU $GL_LIBS -lX11 -lXext $VIDMODE_LIBS -lm)
    fi

    if test "$gl_lib_1" = "" ; then
      AC_CHECK_X_LIB(GL, glXCreateContext, 
                     [gl_lib_1="GL"
                      GL_LIBS="-lGL -lGLU $VIDMODE_LIBS $GL_LIBS"],
                     [], -lGLU $GL_LIBS -lX11 -lXext $VIDMODE_LIBS -lm)
    fi

    if test "$gl_lib_1" = "" ; then
      # we have headers, but no libs -- bail.
      have_gl=no
      ac_have_mesa_gl=no
      gl_halfassed=yes
    else
      # linking works -- we can build the GL hacks.
      AC_DEFINE(HAVE_GL)
      if test "$ac_have_mesa_gl" = yes ; then
        AC_DEFINE(HAVE_MESA_GL)
      fi
    fi
  fi


  # Now that we know we have GL headers and libs, do some more GL testing.
  #

  if test "$have_gl" = yes ; then
    # If it's MesaGL, we'd like to issue a warning if the version number
    # is less than or equal to 2.6, because that version had a security bug.
    #
    if test "$ac_have_mesa_gl" = yes; then

      AC_CACHE_CHECK([MesaGL version number], ac_cv_mesagl_version_string,
        [cat > conftest.$ac_ext <<EOF
#line __oline__ "configure"
#include "confdefs.h"
#include <GL/gl.h>
#ifndef MESA_MAJOR_VERSION
# include <GL/xmesa.h>
# ifdef XMESA_MAJOR_VERSION
   /* Around Mesa 3.2, they took out the Mesa version number, so instead,
      we have to check the XMesa version number (the number of the X protocol
      support, which seems to be the same as the Mesa version number.)
    */
#  define MESA_MAJOR_VERSION XMESA_MAJOR_VERSION
#  define MESA_MINOR_VERSION XMESA_MINOR_VERSION
# else
   /* Oh great.  Some time after 3.4, they took out the xmesa.h header file,
      so we have no way of telling what version of Mesa this is at all.
      So, we'll guess that the osmesa version (the "offscreen protocol")
      is less than or equal to the real mesa version number.  Except that
      if OSmesa is 3.3, assume at least Mesa 3.4, since OSmesa was 3.3 in
      Mesa 3.4.  And Mesa 3.3 had xmesa.h.  What a complete load of shit!
    */
# include <GL/osmesa.h>
#  define MESA_MAJOR_VERSION OSMESA_MAJOR_VERSION
#  define MESA_MINOR_VERSION OSMESA_MINOR_VERSION or newer, probably?
#  if OSMESA_MAJOR_VERSION == 3 && OSMESA_MINOR_VERSION == 3
#   undef MESA_MINOR_VERSION
#   define MESA_MINOR_VERSION 4 or newer, probably?
#  endif
# endif
#endif
configure: MESA_MAJOR_VERSION MESA_MINOR_VERSION
EOF

         ac_save_CPPFLAGS="$CPPFLAGS"
         if test \! -z "$includedir" ; then 
           CPPFLAGS="$CPPFLAGS -I$includedir"
         fi
         CPPFLAGS="$CPPFLAGS $X_CFLAGS"

         mglv=`(eval "$ac_cpp conftest.$ac_ext") 2>&AC_FD_CC | grep configure:`

         # M4 sucks!!
         changequote(X,Y)
          mglv=`echo "$mglv" | sed -n \
             's/^configure: *\([0-9][0-9]*\)  *\([0-9].*\)$/\1.\2/p'`
         changequote([,])

         rm -f conftest.$ac_ext

         CPPFLAGS="$ac_save_CPPFLAGS"

         if test "$mglv" = ""; then
           ac_mesagl_version=unknown
           ac_mesagl_version_string=unknown
         else
           ac_mesagl_version_string="$mglv"
           # M4 sucks!!
           changequote(X,Y)
           maj=`echo "$mglv" | sed -n 's/^\([0-9][0-9]*\)\..*$/\1/p'`
           min=`echo "$mglv" | sed -n 's/^.*\.\([0-9][0-9]*\).*$/\1/p'`
           changequote([,])
           ac_mesagl_version=`echo "$maj * 1000 + $min" | bc`
           if test -z "$ac_mesagl_version"; then
             ac_mesagl_version=unknown
             ac_mesagl_version_string=unknown
           fi
         fi
         ac_cv_mesagl_version=$ac_mesagl_version
         ac_cv_mesagl_version_string=$ac_mesagl_version_string
      ])
      ac_mesagl_version=$ac_cv_mesagl_version
      ac_mesagl_version_string=$ac_cv_mesagl_version_string
    fi


    # Check for OpenGL 1.1 features.
    #
    AC_CHECK_X_LIB($gl_lib_1, glBindTexture, [AC_DEFINE(HAVE_GLBINDTEXTURE)],
                   [true], $GL_LIBS -lX11 -lXext -lm)
  fi

elif test "$with_gl" != no; then
  echo "error: must be yes or no: --with-gl=$with_gl"
  exit 1
fi


###############################################################################
#
#       Check for -lgle.
#
###############################################################################

#have_gle=no
#with_gle_req=unspecified
#gle_halfassed=no
#AC_ARG_WITH(gle,
#[  --with-gle              Build those demos which depend on GLE
#                          (the OpenGL "extrusion" library.)],
#  [with_gle="$withval"; with_gle_req="$withval"],[with_gle=yes])

#HANDLE_X_PATH_ARG(with_gle, --with-gle, GLE)

#GLE_LIBS=""

#if test "$have_gl" = no ; then
# true
#elif test "$with_gle" = yes; then

#  AC_CHECK_X_HEADER(GL/gle.h, have_gle3=yes, have_gle3=no,
#                    [#include <GL/gl.h>])
#  if test "$have_gle3" = yes ; then
#    have_gle=yes;
#  else
#    AC_CHECK_X_HEADER(GL/gutil.h, have_gle=yes, have_gle=no,
#                    [#include <GL/gl.h>])
#    if test "$have_gle" = yes ; then
#      AC_CHECK_X_HEADER(GL/tube.h, have_gle=yes, have_gle=no,
#                        [#include <GL/gl.h>])
#    fi
#  fi

#  if test "$have_gle" = yes ; then
#    have_gle=no
#    gle_halfassed=yes
#    AC_CHECK_X_LIB(gle, gleCreateGC, 
#                   [have_gle=yes; gle_halfassed=no; GLE_LIBS="-lgle"],
#                   [], $GL_LIBS -lX11 -lXext -lm)
#  fi
#  if test "$have_gle" = yes ; then
#    have_gle=no
#    gle_halfassed=yes

# ---------------------------
    # sometimes the libmatrix stuff is included in libgle.  look there first.
#
# I don't get it.  For some reason, this test passes on SGI, as if
# uview_direction_d() was in libgle -- but it's not, it's in libmatrix.
# Yet the link is succeeding.  Why???
#
#    AC_CHECK_X_LIB(gle, uview_direction_d, 
#                   [have_gle=yes; gle_halfassed=no],
#                   [], $GL_LIBS -lX11 -lXext -lm)

    # As of GLE 3 this is in libgle, and has changed name to uview_direction!
    # *sigh*
# -------------------------------
#    if test "$have_gle3" = yes ; then
#      AC_CHECK_X_LIB(gle, uview_direction, 
#                     [have_gle=yes; gle_halfassed=no],
#                    [], $GL_LIBS -lX11 -lXext -lm)
#    fi
#    # if it wasn't in libgle, then look in libmatrix.
#    if test "$have_gle" = no ; then
#      AC_CHECK_X_LIB(matrix, uview_direction_d, 
#                     [have_gle=yes; gle_halfassed=no;
#                      GLE_LIBS="$GLE_LIBS -lmatrix"],
#                    [], $GL_LIBS -lX11 -lXext -lm)
#    fi
#  fi

#  if test "$have_gle" = yes ; then
#    AC_DEFINE(HAVE_GLE)
#    if test "$have_gle3" = yes ; then
#      AC_DEFINE(HAVE_GLE3)
#    fi
#  fi

#elif test "$with_gle" != no; then
#  echo "error: must be yes or no: --with-gle=$with_gle"
#  exit 1

#fi


###############################################################################
#
#       Check for -lgdk_pixbuf.
#       These tests are for gdk_pixbuf usage of the hacks, 
#       not xscreensaver-demo (thus we have to test again to get
#       the libraries right: don't want to pull in all of GTK
#       for the hacks.)
#
###############################################################################

#have_gdk_pixbuf=no
#with_gdk_pixbuf_req=unspecified
#AC_ARG_WITH(pixbuf,
#[  --with-pixbuf           Include support for the GDK-Pixbuf library in some
#                          demos, which will make it possible for them to read
#                          GIF, JPEG, and PNG files as well.],
#  [with_gdk_pixbuf="$withval"; with_gdk_pixbuf_req="$withval"],
#  [with_gdk_pixbuf=yes])

## if --with-pixbuf=/directory/ was specified, remember that directory so that
## we can also look for the `gdk-pixbuf-config' program in that directory.
#case "$with_gdk_pixbuf" in
#  /*)
#    gdk_pixbuf_dir="$with_gdk_pixbuf"
#    ;;
#  *)
#    gdk_pixbuf_dir=""
#    ;;
#esac

#HANDLE_X_PATH_ARG(with_gdk_pixbuf, --with-pixbuf, GDK_PIXBUF)

#if test "$with_gdk_pixbuf" != yes -a "$with_gdk_pixbuf" != no ; then
#  echo "error: must be yes or no: --with-pixbuf=$with_gdk_pixbuf"
#  exit 1
#fi

#if test "$with_gdk_pixbuf" = yes; then
#  have_gdk_pixbuf=no

#  pkgs=''
#  ok="yes"

#  pkg_check_version gdk-pixbuf-2.0      2.0.0
#  pkg_check_version gdk-pixbuf-xlib-2.0 2.0.0
#  have_gdk_pixbuf="$ok"

#  if test "$have_gdk_pixbuf" = yes; then
#    AC_CACHE_CHECK([for gdk-pixbuf includes], ac_cv_gdk_pixbuf_config_cflags,
#               [ac_cv_gdk_pixbuf_config_cflags=`$pkg_config --cflags $pkgs`])
#    AC_CACHE_CHECK([for gdk-pixbuf libs], ac_cv_gdk_pixbuf_config_libs,
#               [ac_cv_gdk_pixbuf_config_libs=`$pkg_config --libs $pkgs`])
#  fi

#  ac_gdk_pixbuf_config_cflags=$ac_cv_gdk_pixbuf_config_cflags
#  ac_gdk_pixbuf_config_libs=$ac_cv_gdk_pixbuf_config_libs


#  if test "$have_gdk_pixbuf" = yes; then
#    #
#    # we appear to have pixbuf; check for headers/libs to be sure.
#    #
#    ac_save_gdk_pixbuf_CPPFLAGS="$CPPFLAGS"
#    CPPFLAGS="$CPPFLAGS $ac_gdk_pixbuf_config_cflags"

#    have_gdk_pixbuf=no

#    # check for header A...
#    AC_CHECK_X_HEADER(gdk-pixbuf/gdk-pixbuf.h, [have_gdk_pixbuf=yes])

#    # if that worked, check for header B...
#    if test "$have_gdk_pixbuf" = yes; then
#      have_gdk_pixbuf=no
#      gdk_pixbuf_halfassed=yes
#      AC_CHECK_X_HEADER(gdk-pixbuf/gdk-pixbuf-xlib.h,
#                        [have_gdk_pixbuf=yes
#                         gdk_pixbuf_halfassed=no])

#      # yay, it has a new name in Gtk 2.x...
#      if test "$have_gdk_pixbuf" = no; then
#        have_gdk_pixbuf=no
#        gdk_pixbuf_halfassed=yes
#        AC_CHECK_X_HEADER(gdk-pixbuf-xlib/gdk-pixbuf-xlib.h,
#                          [have_gdk_pixbuf=yes
#                           gdk_pixbuf_halfassed=no])
#      fi
#    fi
#    CPPFLAGS="$ac_save_gdk_pixbuf_CPPFLAGS"
#  fi

#  if test "$have_gdk_pixbuf" = yes; then
#    # we have the headers, now check for the libraries
#    have_gdk_pixbuf=no
#    gdk_pixbuf_halfassed=yes

#    AC_MSG_RESULT(checking for gdk_pixbuf usability...)

#    # library A...
#    AC_CHECK_X_LIB(c, gdk_pixbuf_new_from_file, [have_gdk_pixbuf=yes],,
#                   $ac_gdk_pixbuf_config_libs -lX11 -lXext -lm)
#    # library B...
#    if test "$have_gdk_pixbuf" = yes; then
#      have_gdk_pixbuf=no
#      AC_CHECK_X_LIB(c, gdk_pixbuf_xlib_init,
#                     [have_gdk_pixbuf=yes
#                      gdk_pixbuf_halfassed=no],,
#                     $ac_gdk_pixbuf_config_libs -lX11 -lXext -lm)
#    fi
#  fi

#  if test "$have_gdk_pixbuf" = yes; then
#    INCLUDES="$INCLUDES $ac_gdk_pixbuf_config_cflags"
#    XPM_LIBS="$ac_gdk_pixbuf_config_libs"
#    AC_DEFINE(HAVE_GDK_PIXBUF)
#  else
#    AC_MSG_RESULT(checking for gdk_pixbuf usability... no)
#  fi
#fi


###############################################################################
#
#       Check for -lXpm.
#
###############################################################################

#have_xpm=no
#with_xpm_req=unspecified
#AC_ARG_WITH(xpm,
#[  --with-xpm              Include support for XPM files in some demos.
#                          (Not needed if Pixbuf is used.)],
#  [with_xpm="$withval"; with_xpm_req="$withval"],[with_xpm=yes])

#HANDLE_X_PATH_ARG(with_xpm, --with-xpm, XPM)

#if test "$with_xpm" = yes; then
#  AC_CHECK_X_HEADER(X11/xpm.h,
#                   [have_xpm=yes
#                    AC_DEFINE(HAVE_XPM)
#                    XPM_LIBS="-lXpm $XPM_LIBS"],,
#                    [#include <X11/Xlib.h>])
#elif test "$with_xpm" != no; then
#  echo "error: must be yes or no: --with-xpm=$with_xpm"
#  exit 1
#fi

# -----------------------------------------------
# See comment near $motif_requires_xpm, above.
# Need to do this here, after both Motif and XPM have been checked for.
#
#if test "$have_motif" = yes -a "$have_xpm" = yes ; then
#  if test "$motif_requires_xpm" = yes ; then
#    MOTIF_LIBS="$MOTIF_LIBS $XPM_LIBS"
#  fi
#fi

#if test "$XPM_LIBS" = "" ; then
#  XPM_LIBS=$(MINIXPM)
#fi


###############################################################################
#
#       Check for -ljpeg
#
###############################################################################

#have_jpeg=no
#with_jpeg_req=unspecified
#jpeg_halfassed=no
#AC_ARG_WITH(jpeg,
#[  --with-jpeg             Include support for the JPEG library.],
#  [with_jpeg="$withval"; with_jpeg_req="$withval"],
#  [with_jpeg=yes])

#HANDLE_X_PATH_ARG(with_jpeg, --with-jpeg, JPEG)

#if test "$with_jpeg" != yes -a "$with_jpeg" != no ; then
#  echo "error: must be yes or no: --with-jpeg=$with_jpeg"
#  exit 1
#fi

#if test "$with_jpeg" = yes; then

#  have_jpeg=no
#  AC_CHECK_X_HEADER(jpeglib.h, [have_jpeg=yes])

#  if test "$have_jpeg" = yes; then
#    # we have the header, now check for the library
#    have_jpeg=no
#    jpeg_halfassed=yes
#    AC_CHECK_X_LIB(jpeg, jpeg_start_compress,
#                   [have_jpeg=yes
#                    jpeg_halfassed=no
#                    JPEG_LIBS="-ljpeg"
#                    AC_DEFINE(HAVE_JPEGLIB)])
#  fi
#fi

###############################################################################
#
#       Check for the XSHM server extension.
#
###############################################################################

have_xshm=no
with_xshm_req=unspecified
AC_ARG_WITH(xshm-ext,
[  --with-xshm-ext         Include support for the Shared Memory extension.],
  [with_xshm="$withval"; with_xshm_req="$withval"],[with_xshm=yes])

HANDLE_X_PATH_ARG(with_xshm, --with-xshm-ext, XSHM)

if test "$with_xshm" = yes; then

  # first check for Xshm.h.
  AC_CHECK_X_HEADER(X11/extensions/XShm.h, [have_xshm=yes],,
                    [#include <X11/Xlib.h>])

  # if that succeeded, then check for sys/ipc.h.
  if test "$have_xshm" = yes; then
    have_xshm=no
    AC_CHECK_X_HEADER(sys/ipc.h, [have_xshm=yes])
  fi

  # if that succeeded, then check for sys/shm.h.
  if test "$have_xshm" = yes; then
    have_xshm=no
    AC_CHECK_X_HEADER(sys/shm.h, [have_xshm=yes])
  fi

  # AIX is pathological, as usual: apparently it's normal for the Xshm headers
  # to exist, but the library code to not exist.  And even better, the library
  # code is in its own library: libXextSam.a.  So, if we're on AIX, and that
  # lib doesn't exist, give up.  (This lib gets added to X_EXTRA_LIBS, and
  # that's not quite right, but close enough.)
  #
  case "$host" in
    *-aix*)
      if [ `uname -v` -eq 3 ]; then
        have_xshm=no
        AC_CHECK_X_LIB(XextSam, XShmQueryExtension,
                       [have_xshm=yes; X_EXTRA_LIBS="$X_EXTRA_LIBS -lXextSam"],
                       [true], -lX11 -lXext -lm)
      fi
    ;;
  esac

  # if that succeeded, then we've really got it.
  if test "$have_xshm" = yes; then
    AC_DEFINE(HAVE_XSHM_EXTENSION)
  fi

elif test "$with_xshm" != no; then
  echo "error: must be yes or no: --with-xshm-ext=$with_xshm"
  exit 1
fi


###############################################################################
#
#       Check for the DOUBLE-BUFFER server extension.
#
###############################################################################

have_xdbe=no
with_xdbe_req=unspecified
AC_ARG_WITH(xdbe-ext,
[  --with-xdbe-ext         Include support for the DOUBLE-BUFFER extension.],
  [with_xdbe="$withval"; with_xdbe_req="$withval"],[with_xdbe=yes])

HANDLE_X_PATH_ARG(with_xdbe, --with-xdbe-ext, DOUBLE-BUFFER)

if test "$with_xdbe" = yes; then

  AC_CHECK_X_HEADER(X11/extensions/Xdbe.h, [have_xdbe=yes],,
                    [#include <X11/Xlib.h>])
  if test "$have_xdbe" = yes; then
    AC_DEFINE(HAVE_DOUBLE_BUFFER_EXTENSION)    
  fi

elif test "$with_xdbe" != no; then
  echo "error: must be yes or no: --with-xdbe-ext=$with_xshm"
  exit 1
fi



###############################################################################
#
#       Done testing.  Now, set up the various -I and -L variables
#
###############################################################################

# ccmakedep is supposedly better than makedepend
# (there is also a gccmakedep, which calls gcc -M and is better yet)
# all three are in the xutils-dev package in Maverick,
# where else they may come from, I don't yet know'
# see Dependency Tracking in the Automake manual
DEPEND=makedepend
DEPEND_FLAGS=
DEPEND_DEFINES=


if test \! -z "$includedir" ; then 
  INCLUDES="$INCLUDES -I$includedir"
fi

if test \! -z "$libdir" ; then
  LDFLAGS="$LDFLAGS -L$libdir"
fi



if test "$ac_macosx" = yes; then
  EXES_OSX='$(EXES_OSX)'
  SCRIPTS_OSX='$(SCRIPTS_OSX)'
  MEN_OSX='$(MEN_OSX)'
else
  EXES_OSX=
  SCRIPTS_OSX=
  MEN_OSX=
fi



tab='	'
if test "$have_gl" = yes; then
  GL_EXES='$(GL_EXES)'
  GL_UTIL_EXES='$(GL_UTIL_EXES)'
  GL_MEN='$(GL_MEN)'
  GL_KLUDGE="${tab}  "
else
  GL_KLUDGE="-${tab}  "
fi

#if test "$have_gle" = yes; then
#  GLE_EXES='$(GLE_EXES)'
#  GLE_KLUDGE="${tab}   "
#else
#  GLE_KLUDGE="-${tab}   "
#fi

#if test "$have_jpeg" = yes -a "$have_gdk_pixbuf" = yes; then
# JPEG_EXES='$(JPEG_EXES)'
#fi




# canonicalize slashes.
#HACK_CONF_DIR=`echo "${HACK_CONF_DIR}" | sed 's@/$@@;s@//*@/@g'`

# gcc 3.0 likes to issue this warning for every file:
#
# cc1: warning: changing search order for system directory "/usr/local/include"
# cc1: warning:   as it has already been specified as a non-system directory
#
# Yay.  We can only avoid that by deleting "-I${prefix}/include" from the list.
# Which *should* be totally redundant, and thus an ok thing to delete?
#
INCLUDES=`echo "$INCLUDES" | sed 's@ -I${prefix}/include@@g;'`


###############################################################################
#
#       Perform substitutions and write Makefiles.
#
###############################################################################

AC_SUBST(INCLUDES)

AC_SUBST(SAVER_LIBS)
#AC_SUBST(JPEG_LIBS)
AC_SUBST(HACK_LIBS)
#AC_SUBST(XPM_LIBS)
AC_SUBST(GL_LIBS)
#AC_SUBST(GLE_LIBS)
AC_SUBST(INSTALL_DIRS)

AC_SUBST(OBJCC)
AC_SUBST(EXES_OSX)
AC_SUBST(SCRIPTS_OSX)
AC_SUBST(MEN_OSX)

AC_SUBST(XMU_SRCS)
AC_SUBST(XMU_OBJS)
AC_SUBST(XMU_LIBS)
AC_SUBST(SAVER_GL_SRCS)
AC_SUBST(SAVER_GL_OBJS)
AC_SUBST(SAVER_GL_LIBS)
#AC_SUBST(JPEG_EXES)
AC_SUBST(GL_EXES)
AC_SUBST(GL_UTIL_EXES)
AC_SUBST(GL_MEN)
AC_SUBST(GL_KLUDGE)
#AC_SUBST(GLE_EXES)
#AC_SUBST(GLE_KLUDGE)
AC_SUBST(HACKDIR)
AC_SUBST(HACKDIR_FULL)
AC_SUBST(HACK_CONF_DIR)
AC_SUBST(IMAGE_DIR)

APPDEFAULTS=$ac_x_app_defaults
AC_SUBST(APPDEFAULTS)

AC_SUBST(DEPEND)
AC_SUBST(DEPEND_FLAGS)
AC_SUBST(DEPEND_DEFINES)

AC_OUTPUT(Makefile
          utils/Makefile
          hacks/Makefile
          hacks/glx/Makefile)

###############################################################################
#
#       Print some warnings at the end.
#
###############################################################################

warn_prefix_1="    Warning:"
warn_prefix_2="       Note:"
warn_prefix="$warn_prefix_1"

warning=no
warnsep='    #################################################################'

warnpre() {
  if test "$warning" = no ; then
    echo '' ; echo "$warnsep" ; echo ''
    warning=yes
  fi
}

warn() {
  warnpre
  if test "$warning" = long ; then echo '' ; fi
  warning=yes
  rest="$@"
  echo "$warn_prefix $rest"
}

warnL() {
  was=$warning
  warnpre
  warning=yes
  if test "$was" != no ; then echo '' ; fi
  rest="$@"
  echo "$warn_prefix $rest"
}

warn2() {
  rest="$@"
  echo "             $rest"
  warning=long
}

note() {
  warn_prefix="$warn_prefix_2"
  warn $@
  warn_prefix="$warn_prefix_1"
}

noteL() {
  warn_prefix="$warn_prefix_2"
  warnL $@
  warn_prefix="$warn_prefix_1"
}


if test "$with_xshm_req" = yes -a "$have_xshm" = no ; then
  warn 'The XSHM extension was requested, but was not found.'
fi

if test "$with_xdbe_req" = yes -a "$have_xdbe" = no ; then
  warn 'The DOUBLE-BUFFER extension was requested, but was not found.'
fi


if test "$pkg_config" = false ; then
  warnL 'The "pkg-config" program was not found.  Without that,'
  warn2 "detection of the various GTK libraries won't work."
else
  pkgerr=`$pkg_config --list-all 2>&1 >/dev/null`
  if test "x$pkgerr" != "x" ; then
    warnL 'The "pkg-config" program produces errors.  This often causes'
    warn2 "detection of the various GTK libraries to malfunction."
    warn2 "The errors are:"
    echo ''
    echo "$pkgerr" | sed 's/^/             > /g'
  fi
fi



#if test "$with_xpm_req" = yes -a "$have_xpm" = no; then
#  warnL 'Use of XPM was requested, but it was not found.'
#fi

#if test "$with_gdk_pixbuf_req" = yes  -a "$have_gdk_pixbuf" = no; then
#  warnL 'Use of GDK-Pixbuf was requested, but it was not found.'
#fi

#if test "$have_gdk_pixbuf" = no -o "$gdk_pixbuf_halfassed" = yes || \
#   test "$have_gdk_pixbuf" = no -a "$have_xpm" = no ; then

#  if test "$with_gdk_pixbuf_req" = yes ; then
#    true
#  elif test "$with_gdk_pixbuf_req" = no ; then
#    warnL 'The GDK-Pixbuf library is not being used.'
#  else
#    warnL 'The GDK-Pixbuf library was not found.'
#  fi

#  if test "$with_xpm_req" = yes -o "$have_xpm" = yes ; then
#    true
#  elif test "$with_xpm_req" = no ; then
#    warnL 'The XPM library is not being used.'
#  else
#    warnL 'The XPM library was not found.'
#  fi

#  if test "$have_gdk_pixbuf" = no -a "$have_xpm" = yes ; then
#    warn2 'The XPM library is being used instead.'
#  fi

#  if test "$gdk_pixbuf_halfassed" = yes ; then
#    echo ''
#    warn2 'More specifically, we found the headers, but not the'
#    warn2 'libraries; so either GDK-Pixbuf is half-installed on this'
#    warn2 "system, or something else went wrong.  The \`config.log'"
#    warn2 'file might contain some clues.'
#  fi

#  echo ''
#  warn2 'Some of the demos will not use images as much as they could.'
#  warn2 'You should consider installing GDK-Pixbuf and re-running'
#  warn2 'configure.  (GDK-Pixbuf is recommended over XPM, as it'
#  warn2 'provides support for more image formats.)'
#fi


#if test "$have_jpeg" = no ; then
#  if test "$with_jpeg_req" = yes ; then
#    warnL 'Use of libjpeg was requested, but it was not found.'
#  elif test "$with_jpeg_req" = no ; then
#    noteL 'The JPEG library is not being used.'
#  else
#    noteL 'The JPEG library was not found.'
#  fi

#  if test "$jpeg_halfassed" = yes ; then
#    echo ''
#    warn2 'More specifically, we found the headers, but not the'
#    warn2 'library; so either JPEG is half-installed on this'
#    warn2 "system, or something else went wrong.  The \`config.log'"
#    warn2 'file might contain some clues.'
#    echo ''
#  fi

#  if test "$have_gdk_pixbuf" = no ; then
#    warn2 "This means that it won't be possible for the image-manipulating"
#    warn2 "display modes to load files from disk; and it also means that"
#    warn2 "the \`webcollage' program will be much slower."
#  else
#    warn2 "This means the \`webcollage' program will be much slower."
#  fi
#fi


if test "$have_gl" = yes -a "$ac_have_mesa_gl" = yes ; then
  preferred_mesagl=3.4
  mgv="$ac_mesagl_version_string"
  pgl="$preferred_mesagl"

  if test "$ac_mesagl_version" = unknown; then
    warnL "Unable to determine the MesaGL version number!"
    warn2 "Make sure you are using version $preferred_mesagl or newer."

  elif test \! "$ac_mesagl_version" -gt 2006; then
    warnL "MesaGL version number is $mgv --"
    warn2 "MesaGL 2.6 and earlier have a security bug.  It is strongly"
    warn2 "recommended that you upgrade to at least version $preferred_mesagl."

  elif test \! "$ac_mesagl_version" -gt 3003; then
    warnL "MesaGL version number is $mgv --"
    warn2 "MesaGL 3.3 and earlier have some bugs; it is recommended"
    warn2 "that you upgrade to $pgl or newer."
  fi
fi

if test "$have_gl" = no ; then
  if test "$with_gl_req" = yes ; then
    warnL 'Use of GL was requested, but it was not found.'
  elif test "$with_gl_req" = no ; then
    noteL 'The OpenGL 3D library is not being used.'
  else
    noteL 'The OpenGL 3D library was not found.'
  fi

  if test "$gl_halfassed" = yes ; then
    echo ''
    warn2 'More specifically, we found the headers, but not the'
    warn2 'libraries; so either GL is half-installed on this'
    warn2 "system, or something else went wrong.  The \`config.log'"
    warn2 'file might contain some clues.'
  fi

  echo ''
  warn2 'Those demos which use 3D will not be built or installed.'
  warn2 'You might want to consider installing OpenGL and'
  warn2 "re-running configure.  If your vendor doesn't ship"
  warn2 'their own implementation of OpenGL, you can get a free'
  warn2 'version at <http://www.mesa3d.org/>.  For general OpenGL'
  warn2 'info, see <http://www.opengl.org/>.'

fi


#if test "$have_gl" = yes -a "$have_gle" = no ; then

# # nobody cares about this; don't print the warning unless it was
# # requested and not found, or halfway-found.
# if test "$with_gle_req" = yes -o "$gle_halfassed" = yes ; then

#  if test "$with_gle_req" = yes ; then
#    noteL 'Use of the GLE (GL Extrusion) library was requested, but'
#    warn2 'it was not found (though the OpenGL library was found, and'
#    warn2 'is being used.)'
#  elif test "$with_gle_req" = no ; then
#    noteL 'The OpenGL Library is being used, but the GLE (GL Extrusion)'
#    warn2 'library is not.'
#  else
#    noteL 'The OpenGL Library was found, but the GLE (GL Extrusion)'
#    warn2 'was not.'
#  fi

#  if test "$gle_halfassed" = yes ; then
#    echo ''
#    warn2 'More specifically, we found the headers, but not the'
#    warn2 'libraries; so either GLE is half-installed on this'
#    warn2 "system, or something else went wrong.  The \`config.log'"
#    warn2 'file might contain some clues.'
#  fi

#  echo ''
#  warn2 'Some of the OpenGL (3D) demos (those that depend on GLE)'
#  warn2 'will not be built or installed.  You might want to consider'
#  warn2 'installing GLE and re-running configure.  You can find the'
#  warn2 'GLE library at <http://www.linas.org/gle/>.  For general'
#  warn2 'OpenGL info, see <http://www.opengl.org/>.'

# fi
#fi


# You are in a twisty maze of namespaces and syntaxes, all alike.
# Fuck the skull of Unix.
#
bindir=`eval eval eval eval eval eval eval echo $bindir`
HACKDIR=`eval eval eval eval eval eval eval echo $HACKDIR`
HACK_CONF_DIR=`eval eval eval eval eval eval eval echo $HACK_CONF_DIR`
IMAGE_DIR=`eval eval eval eval eval eval eval echo $IMAGE_DIR`

# canonicalize slashes.
bindir=`echo  "${bindir}"              | sed 's@/$@@;s@//*@/@g'`
HACKDIR=`echo "${HACKDIR}"             | sed 's@/$@@;s@//*@/@g'`
HACK_CONF_DIR=`echo "${HACK_CONF_DIR}" | sed 's@/$@@;s@//*@/@g'`
IMAGE_DIR=`echo "${IMAGE_DIR}" | sed 's@/$@@;s@//*@/@g'`


# Sanity check the hackdir
for bad_choice in xscreensaver xscreensaver-demo xscreensaver-command ; do
  if test "${HACKDIR}" = "${bindir}/${bad_choice}" ; then
    echo ""
    AC_MSG_ERROR([\"--with-hackdir=${bindir}/${bad_choice}\" won't work.
                   There will be an executable installed with that name, so
                   that can't be the name of a directory as well.  Please
                   re-configure with a different directory name.])
  fi
done


do_dir_warning=no


if test "${bindir}" = "${HACKDIR}" ; then
  do_dir_warning=yes
fi

if test "$do_dir_warning" = yes; then
  echo ""
  echo "$warnsep"
  echo ""
  echo "      When you run "make install", the various graphics demos will"
  echo "      be installed in ${HACKDIR}/."
  echo ""
  echo "      If you would prefer the demos to be installed elsewhere,"
  echo "      you should re-run configure with the --with-hackdir=DIR"
  echo "      option.  For more information, run \`./configure --help'."
  warning=yes
fi

if test "$warning" != no; then
  echo '' ; echo "$warnsep" ; echo ''
fi

if test "$do_dir_warning" = no; then
  if test "$warning" = no; then
    echo ''
  fi
  echo "Screen savers will be installed in ${HACKDIR}/"
  echo "Configuration dialogs will be installed in ${HACK_CONF_DIR}/"
  echo "Images will be installed in ${IMAGE_DIR}/"
  echo ''
fi

