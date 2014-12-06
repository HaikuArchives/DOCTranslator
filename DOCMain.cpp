// DOCMain.cpp
// Copyright (c) 2014 Markus Himmel
// This file is distributed under the terms of the MIT license.

#include <Application.h>
#include <Catalog.h>
#include "DOCTranslator.h"
#include "DOCView.h"
#include "TranslatorWindow.h"
#include "Alert.h"
#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DOCTranslator"

int
main()
{
  BApplication app("application/x-vnd.Haiku-DOCTranslator");
  (new BAlert("PostMain", "PostMain", "OK"))->Go();
  status_t result;
  result = LaunchTranslatorWindow(new DOCTranslator,
                  B_TRANSLATE("DOC Settings"), BRect(0, 0, DOC_VIEW_WIDTH,
                  DOC_VIEW_HEIGHT));
  (new BAlert("Main", "Main 2", "OK"))->Go();
  if (result == B_OK)
  {
    app.Run();
    return 0;
  }
  else
  {
    return 1;
  }
  return 0;
}
