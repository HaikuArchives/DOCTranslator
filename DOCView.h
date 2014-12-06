// DOCView.h
// Copyright(c) 2014 Markus Himmel
// This file is distributed under the terms of the MIT license

#ifndef DOC_VIEW_H
#define DOC_VIEW_H

#include "TranslatorSettings.h"
#include <View.h>

#define DOC_VIEW_WIDTH 300
#define DOC_VIEW_HEIGHT 270

class DOCView : public BView  {
public:
  PNGView(const BRect &frame, const char *name, uint32 resizeMode,
                  uint32 flags, TranslatorSettings *settings);
  ~PNGView();

  virtual void AttachedToWindow();
  virtual void FrameResized(float width, float height);
  virtual void MessageReceived(BMessage *message);

private:
  TranslatorSettings *fSettings;

};

#endif // DOC_VIEW_H