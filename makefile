NAME=DOCTranslator

TYPE=APP
APP_MIME_SIG=

SRCS=DOCMain.cpp\
		 DOCView.cpp\
		 DOCTranslator.cpp\
		 shared/BaseTranslator.cpp\
		 shared/TranslatorSettings.cpp\
		 shared/TranslatorWindow.cpp\

RDEFS=DOCTranslator.rdef

RSRCS=

LIBS= be localestub translation textencoding $(STDCPPLIBS)
LIBPATHS=
LOCAL_INCLUDE_PATHS= shared

OPTIMIZE=NONE

LOCALES=

SYMBOLS=

DEBUGGER=

COMPILER_FLAGS=

LINKER_FLAGS=

APP_VERSIOn=

DRIVER_PATH=

include $(BUILDHOME)/etc/makefile-engine
