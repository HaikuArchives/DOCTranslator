The translator corretly translates .doc files to plain text files, PostScript files and PDF files.

### The antiword .recipe

The antiword .recipe has some problems:
* The build of some dependencies fails, requiring --no-dependencies
* The .recipe installs a debug version (Prints a lot of garbage to the console)
* The .recipe ~~misses an installation step where a bunch of files have to be copied in a certain directory (as explained on the antiword home page)~~ copies files to the wrong folder
