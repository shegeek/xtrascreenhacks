$! Xscreensaver - definition of various DCL symbols
$ set NOON
$ set def [.HACKS]
$ mydisk = f$trnlmn("SYS$DISK")
$ mydir  = mydisk+f$directory()
$ daisy		:== $'mydir'daisy
$ twinkle	:== $'mydir'twinkle
$ set def [-.DRIVER]
$ mydir  = mydisk+f$directory()
$ xscreensaver :== $'mydir'xscreensaver
$ xscreen*command :== $'mydir'xscreensaver-command
$ set def [-]
$ exit
