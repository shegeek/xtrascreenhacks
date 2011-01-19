/* todo:
 * no connector (or room for it) between seconds lamp & rest of clock
 * should there be one backdrop strip for each row of lamps w/a connector
   between each, as in the original big clock?
 * ->probably not--orig. is very tall, & would make poor use of screen
 *   maybe make a user option in the future
 * put small black border around lamps?
 * make lamp and background colors user options
 * possibly make % of screen used by clock a user option
 * code is messy!
 *  -> create structs for clock and background
 *  -> functions to break off: init_bg, init_clock, draw_bg, display_time
 * might be worthwhile to check time only once per second 
   after clock gets going
 */


/*berlinuhr.c--by shegeek (Kelley Nielsen)
 *
 * based on and steals code from deluxe.c by:

 * xscreensaver, Copyright (c) 1999 Jamie Zawinski <jwz@jwz.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

/*-------------------------------------------------------------*/

#include "screenhack.h"

#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
# include "xdbe.h"
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */


/* screenhack -------------------------------------------------*/
char *progclass = "BerlinUhr";

char *defaults[] = {
  ".background:		black",
  ".foreground:		gray70",
  ".hourforeground:     red",
  ".minuteforeground:   yellow",
  ".offforeground:      gray30",
  "*doubleBuffer:	True",
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  "*useDBE:		True",
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */
  0
};

XrmOptionDescRec options[] = {
  {"-db", ".doubleBuffer", XrmoptionNoArg, "True"},
  {"-no-db", ".doubleBuffer", XrmoptionNoArg, "False"},
  {0, 0, 0, 0}
};

void
screenhack (Display * dpy, Window window)
{

  Bool dbuf = get_boolean_resource ("doubleBuffer", "Boolean");

  XGCValues gcv;
  XWindowAttributes xgwa;

  GC erase_gc = 0;
  /* for drawing background */
  GC backdrop_gc = 0;

  GC offgc = 0;
  GC ygc = 0;
  GC rgc = 0;


  /* time handling (of course) */
   time_t clock_t;
   struct tm *clock_tm;


  /* check time 4 times per second */
  int delay = 250000;


/* Each row of lamps has
 * an array of rectangles
 * and an integer marker
 * (5-minute row has two of each)
 */
  XRectangle hr5rect[4];
  int hr5pos;
  XRectangle hr1rect[4];
  int hr1pos;
  XRectangle min5rect[11];
  int min5pos;
  XRectangle min15rect[3];
  int min15pos;
  XRectangle min1rect[4];
  int min1pos;

  /* the seconds lamp is handled by itself */
  int secx, secy, sech, secw;

  /* iterators, temporaries, and the like */
  int i, j;
  int tempy, temph, tempx, tempw;
  unsigned long flags;
  /* make this a user option */ 

 /* for background, and fitting the lamps into it */
  int pad;
int leftmargin, rightmargin, workingwidth, workingheight;
 int backdroplinewidth, lamplinewidth, lampheight;

	/* double-buffer to reduce flicker */
 Pixmap b = 0, ba = 0, bb = 0; 
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
  XdbeBackBuffer backb = 0;
#endif  /* HAVE_DOUBLE_BUFFER_EXTENSION */


  XGetWindowAttributes (dpy, window, &xgwa);

  pad = xgwa.width * .02;
  backdroplinewidth =  xgwa.width * .03;
  lamplinewidth = xgwa.width * .02;



  flags = GCForeground;

  gcv.line_style = LineSolid;
  gcv.join_style = JoinRound;

  gcv.line_width = backdroplinewidth;
  flags |= (GCLineWidth | GCLineStyle | GCJoinStyle);
  gcv.foreground = get_pixel_resource ("foreground", "Foreground",
                                         dpy, xgwa.colormap);
  backdrop_gc = XCreateGC (dpy, window, flags, &gcv);


  gcv.line_width = lamplinewidth;
  flags |= (GCLineWidth | GCLineStyle | GCJoinStyle);
  gcv.foreground = get_pixel_resource ("offforeground", "Offforeground",
                                          dpy, xgwa.colormap);
  offgc = XCreateGC (dpy, window, flags, &gcv);


  gcv.foreground = get_pixel_resource ("hourforeground", "Hourforeground",
                                           dpy, xgwa.colormap);
  rgc = XCreateGC (dpy, window, flags, &gcv);


  gcv.foreground = get_pixel_resource ("minuteforeground", "Minuteforeground",
                                           dpy, xgwa.colormap);
  ygc = XCreateGC (dpy, window, flags, &gcv);

  gcv.foreground = get_pixel_resource ("background", "Background",
				       dpy, xgwa.colormap);




  if (dbuf)
    {
#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
      b = backb = xdbe_get_backbuffer (dpy, window, XdbeUndefined);
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */

      if (!b)
	{
	  ba =
	    XCreatePixmap (dpy, window, xgwa.width, xgwa.height, xgwa.depth);
	  bb =
	    XCreatePixmap (dpy, window, xgwa.width, xgwa.height, xgwa.depth);
	  b = ba;
	}
    }
  else
    {
      b = window;
    }



  /* make xrectangles that will be the lit areas */

  /* for the background */
  leftmargin = xgwa.width * 0.20;
  rightmargin = xgwa.width - leftmargin;
  workingwidth = rightmargin - leftmargin;
  workingheight = xgwa.height * 0.74;

tempy = 0;
temph = xgwa.height - workingheight - pad;
 tempx = (xgwa.width / 2) - (temph / 2) - pad / 2;
tempw = temph;

secy = tempy + pad;
sech = temph - pad;
secx = tempx + pad;
secw = tempw - pad;

 lampheight = workingheight - backdroplinewidth;

 tempy = xgwa.height - lampheight * 1 - backdroplinewidth;
temph = xgwa.height * 0.17;
 tempw = workingwidth / 4;

for( i= 0; i < 4; i++ ){
  tempx = leftmargin + ( i * tempw );
 hr5rect[i].y  = tempy + pad;
hr5rect[i].height  = temph - 2 * pad;
hr5rect[i].x  = hr1rect[i].x = min1rect[i].x  = tempx + pad;
 hr5rect[i].width  =  hr1rect[i].width  =  
 min1rect[i].width  = tempw - 2 * pad;
}

 tempy = xgwa.height - lampheight * 0.75 - backdroplinewidth;

for( i= 0; i < 4; i++ ){
 hr1rect[i].y  = tempy + pad;
hr1rect[i].height  = temph - 2 * pad;
}


 tempy = xgwa.height - lampheight * 0.5 - backdroplinewidth;
 tempw = workingwidth / 11;
 j = 0;

for( i= 0; i < 11; i++ ){
  tempx = leftmargin + ( i * tempw );;
 min5rect[i].y  = tempy + pad;
min5rect[i].height  = temph - 2 * pad;
min5rect[i].x  = tempx + pad;
 min5rect[i].width  = tempw - 2 * pad;
 if ((i+1) % 3 == 0){
   min15rect[j].y = min5rect[i].y;
   min15rect[j].height = min5rect[i].height;
   min15rect[j].x = min5rect[i].x;
   min15rect[j].width = min5rect[i].width;
   j++;
 }
}


 tempy = xgwa.height - lampheight * 0.25 - backdroplinewidth;

for( i= 0; i < 4; i++ ){
 min1rect[i].y  = tempy + pad;
min1rect[i].height  = temph - 2 * pad;
}


  erase_gc = XCreateGC (dpy, b, GCForeground, &gcv);

  if (ba)
    XFillRectangle (dpy, ba, erase_gc, 0, 0, xgwa.width, xgwa.height);
  if (bb)
    XFillRectangle (dpy, bb, erase_gc, 0, 0, xgwa.width, xgwa.height);




  while (1)
    {



/*       XFillRectangle (dpy, b, erase_gc, 0, 0, xgwa.width, xgwa.height); */

  clock_t = time(NULL);
  clock_tm = localtime(&clock_t);


      /* draw the backdrop */

  XDrawRectangle (dpy, b, backdrop_gc, leftmargin, 
                  xgwa.height - workingheight, 
                  workingwidth, workingheight - backdroplinewidth);
  XFillRectangle (dpy, b, backdrop_gc, leftmargin, 
                  xgwa.height - workingheight,
                  workingwidth, workingheight - backdroplinewidth);
    XFillArc( dpy, b, backdrop_gc, secx - pad, secy - pad, 
              secw + pad * 2, sech + pad * 2, 0, 360 * 64 );


  /* find row positions */
  hr5pos = clock_tm->tm_hour / 5;
  hr1pos = clock_tm->tm_hour % 5;
  min5pos = clock_tm->tm_min / 5;
  min15pos = min5pos / 3;
  min1pos = clock_tm->tm_min % 5;

   /* handle seconds lamp */
  /* doesn't allow for leap seconds--lamp will be lit for 2 seconds */
  if(( clock_tm->tm_sec % 2 ) == 0 ){
    XFillArc( dpy, b, offgc, secx, secy, secw, sech, 0, 360 * 64 );
  }
  else {
    XFillArc( dpy, b, ygc, secx, secy, secw, sech, 0, 360 * 64 );    
  }


      /* light rows */
XDrawRectangles( dpy, b, offgc, hr5rect, 4  );
XFillRectangles( dpy, b, offgc, hr5rect, 4  );
XDrawRectangles( dpy, b, rgc, hr5rect, hr5pos );
XFillRectangles( dpy, b, rgc, hr5rect, hr5pos );

XDrawRectangles( dpy, b, offgc, hr1rect, 4 );
XFillRectangles( dpy, b, offgc, hr1rect, 4 );
XDrawRectangles( dpy, b, rgc, hr1rect, hr1pos );
XFillRectangles( dpy, b, rgc, hr1rect, hr1pos );

XDrawRectangles( dpy, b, offgc, min5rect, 11 );
XFillRectangles( dpy, b, offgc, min5rect, 11 );
XDrawRectangles( dpy, b, ygc, min5rect, min5pos );
XFillRectangles( dpy, b, ygc, min5rect, min5pos );
XDrawRectangles( dpy, b, rgc, min15rect, min15pos );
XFillRectangles( dpy, b, rgc, min15rect, min15pos );

XDrawRectangles( dpy, b, offgc, min1rect, 4 );
XFillRectangles( dpy, b, offgc, min1rect, 4 );
XDrawRectangles( dpy, b, ygc, min1rect, min1pos );
XFillRectangles( dpy, b, ygc, min1rect, min1pos );

 

#ifdef HAVE_DOUBLE_BUFFER_EXTENSION
      if (backb)
	{
	  XdbeSwapInfo info[1];
	  info[0].swap_window = window;
	  info[0].swap_action = XdbeUndefined;
	  XdbeSwapBuffers (dpy, info, 1);
	}
      else
#endif /* HAVE_DOUBLE_BUFFER_EXTENSION */
	/* FIX: acting strangely with non-xdbe backbuffer */
      if (dbuf)
	{
	  XCopyArea (dpy, b, window, erase_gc, 0, 0,
		     xgwa.width, xgwa.height, 0, 0);
	  b = (b == ba ? bb : ba);
	}


      XSync (dpy, False);
      screenhack_handle_events (dpy);
      usleep (delay);

    }				/*while(1} */

}				/* screenhack() */
