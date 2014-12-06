# Current problems

* Translator does not show up in DataTranslations (or work at all), but makes it crash on exit
* Substituted B_DOC_FORMAT for 99
* Debug alerts are not shown at all
* Makefile only works if haiku source was cloned to ~/haiku and the files are located in src/add-ons/translators/doc

# The antiword .recipe

The antiword .recipe has some problems, too:
* The build of some dependencies fails, requiring --no-dependencies
* The checksum in the .recipe differs from the file downloaded
* The .recipe installs a debug version (Prints a lot of garbage to the console)
* The .recipe misses an installation step where a bunch of files have to be copied in a certain directory (as explained on the antiword home page)
