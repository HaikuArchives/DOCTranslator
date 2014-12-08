The translator corretly translates .doc files to plain text files. There are
some settings which have yet to be added to the settings dialog. The output
format is also not yet used when building the command (so when outType is set to
PostScript, it actually outputs plain text)

### The antiword .recipe

The antiword .recipe has some problems:
* The build of some dependencies fails, requiring --no-dependencies
* The .recipe installs a debug version (Prints a lot of garbage to the console)
* The .recipe misses an installation step where a bunch of files have to be copied in a certain directory (as explained on the antiword home page)
