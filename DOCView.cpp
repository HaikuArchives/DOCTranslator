// DOCView.cpp
// Copyright (c) 2014 Markus Himmel
// This file is distributed under the terms of the MIT license

#include "DOCView.h"
#include "DOCTranslator.h"

#include <Catalog.h>
#include <StringView.h>
#include <Alert.h>
#include <stdio.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DOCTranslator"

DOCView::DOCView(const BRect &frame, const char *name, uint32 resizeMode,
                uint32 flags, TranslatorSettings *settings)
        : BView(frame, name, resizeMode, flags| B_FRAME_EVENTS)
{
  (new BAlert("Test", "FromConstructor", "OK"))->Go();
  fSettings = settings;

  SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

  font_height fontHeight;
  be_bold_font->GetHeight(&fontHeight);
  float height = fontHeight.descent + fontHeight.ascent + fontHeight.leading;

  BRect rect(10, 10, 200, 10 + height);
  BStringView *stringView = new BStringView(rect, "title",
                  B_TRANSLATE("DOC document translator"));
  stringView->SetFont(be_bold_font);
  stringView->ResizeToPreferred();
  AddChild(stringView);

  float maxWidth = stringView->Bounds().Width();

  rect.OffsetBy(0, height + 10);
  char version[256];
  snprintf(version, sizeof(version), B_TRANSLATE("Version %d.%d.%d, %s"),
                  int(B_TRANSLATION_MAJOR_VERSION(DOC_TRANSLATOR_VERSION)),
                  int(B_TRANSLATION_MINOR_VERSION(DOC_TRANSLATOR_VERSION)),
                  int(B_TRANSLATION_REVISION_VERSION(DOC_TRANSLATOR_VERSION)),
                  __DATE__);
  stringView = new BStringView(rect, "version", version);
  stringView->ResizeToPreferred();
  AddChild(stringView);

  if (stringView->Bounds().Width() > maxWidth)
  {
    maxWidth = stringView->Bounds().Width();
  }

  GetFontHeight(&fontHeight);
  height = fontHeight.descent + fontHeight.ascent + fontHeight.leading;

  rect.OffsetBy(0, height + 5);
  stringView = new BStringView(rect,
                  "Copyright", B_UTF8_COPYRIGHT "2014 Markus Himmel");
  stringView->ResizeToPreferred();
  AddChild(stringView);
}

DOCView::~DOCView()
{
  fSettings->Release();
}


void
DOCView::AttachedToWindow()
{
  BView::AttachedToWindow();
}

void
DOCView::FrameResized(float width, float height)
{
}

void
DOCView::MessageReceived(BMessage *message)
{
  BView::MessageReceived(message);
}

