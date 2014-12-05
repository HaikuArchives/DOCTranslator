// DOCTranslator
// Copyright (c) 2014 Markus Himmel
// This file is distributed under the terms of the MIT license.

#ifndef DOC_TRANSLATOR_H
#define DOC_TRANSLATOR_H

#include <Translator.h>
#include <TranslatorFormats.h>
#include <TranslationDefs.h>
#include "BaseTranslator.h"

#define DOC_TRANSLATOR_VERSION B_TRANSLATION_MAKE_VERSION(0,1,0)

#define B_DOC_FORMAT = 'DOC ';

#define DOC_IN_QUIALITY 0.8
#define DOC_IN_CAPABILITY 0.8

#define TEXT_OUT_QUALITY 0.4
#define TEXT_OUT_CAPABILITY 0.5

class DOCTranslator : public BaseTranslator {
public:

  DOCTranslator();

  virtual status_t DerivedIdentify(BPositionIO *inSource,
                  const translation_format *inFormat, BMessage *ioExtension,
                  translator_info *outInfo, uint32 outType);

  virtual status_t DerivedTranslate(BPositionIO *inSource,
                  const translator_info *inInfo, BMessage *ioExtension,
                  uint32 outType, BPositionIO *outDestination, int32 baseType);

  virtual BView *NewConfigWindow(TranslatorSettings *settings);

protected:
  virtual ~DOCTranslator();
  // Protected since the object is only destroyed when reference count
  // reaches zero

private:

};

#endif // DOC_TRANSLATOR_H
