// DOCTranslator
// Copyright (c) 2014 Markus Himmel
// This file is distributed under the terms of the MIT license.

#include "DOCTranslator.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DOCTranslator"

static const translation_format sInputFormats[] = {
  {
    B_DOC_FORMAT,
    B_TRANSLATOR_TEXT,
    DOC_IN_QUALITY,
    DOC_IN_CAPABILITY,
    "application/msword",
    "Microsoft Word document"
  }
};

static const translation_format sOutputFormats[] = {
  {
    B_TRANSLATOR_TEXT,
    B_TRANSLATOR_TEXT,
    TEXT_OUT_QUALITY,
    TEXT_OUT_CAPABILITY,
    "text/plain",
    "Plain text file"
  }
};

const uint32 kNumInputFormats = sizeof(sInputFormats) /
        sizeof(translation_format);

const uint32 kNumOutputFormats = sizeof(sOutputFormats) /
        sizeof(translation_format);

const uint32 kNumDefaultSettings = sizeof(sDefaultSettings) /
        sizeof(TranSetting);

DOCTranslator::DOCTranslator()
        : BaseTranslator(B_TRANSLATE("DOC documents"),
                         B_TRANSLATE("DOC document translator"),
                         DOC_TRANSLATOR_VERSION,
                         sInputFormats, kNumInputFormats,
                         sOutputFormats, kNumOutputFormats,
                         "DOCTranslator_Settings",
                         sDefaultSettings, kNumDefaultSettings,
                         B_TRANSLATOR_TEXT, B_DOC_FORMAT)
{
}

status_t
DOCTranslator::DerivedIdentify(BPositionIO *source,
                const translation_format *inFormat, BMessage *ioExtension,
                translator_info *outInfo, uint32 outType)
{
  // This part needs code!
}

status_t
DOCTranslator::DerivedTranslate(BPositionIO *inSource,
                const translator_info *inInfo, BMessage *ioExtension,
                uint32 outType, BPositionIO *outDestination, int32 baseType)
{
  // Here be dragons.
}

BView*
DOCTranslator::
