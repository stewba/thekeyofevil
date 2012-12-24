The Key of Evil
===================

An image display program that uses midi to trigger image transitions.  Inspired (read stolen) from Bill Bailey's stand up 'The Scale of Evil'. This version is still pretty rough around the edges, with no way to configure images to notes easily, but it works well enough for a simple song.

Thanks to the GTK homepage and http://home.roadrunner.com/~jgglatt/tech/arawmidrec.htm for the various bits of info on how to use the GTK and ALSA API's

Issues
===================
The midi message currently picks up on note on and note off messages which makes for bad images transitions when playing overly legato (you get a lot of flicker).

Build
===================
This was built in Code::Blocks but it should build ok with gcc, requires alsa and gtk dev packages.