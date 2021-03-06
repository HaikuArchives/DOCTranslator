// DOCTranslator
// Copyright (c) 2014 Markus Himmel <markus@himmel-villmar.de>
// This file is distributed under the terms of the MIT license.

#ifndef DOC_TRANSLATOR_H
#define DOC_TRANSLATOR_H

#include "BaseTranslator.h"
#include "DOCView.h"

#define DOC_TRANSLATOR_VERSION B_TRANSLATION_MAKE_VERSION(0,1,0)

#define B_DOC_FORMAT 'DOC '
#define B_PS_FORMAT 'PS  '
#define B_PDF_FORMAT 'PDF '

#define DOC_IN_QUALITY 0.8
#define DOC_IN_CAPABILITY 0.8

#define TEXT_OUT_QUALITY 0.4
#define TEXT_OUT_CAPABILITY 0.5

#define PS_OUT_QUALITY 0.7
#define PS_OUT_CAPABILITY 0.5

#define PDF_OUT_QUALITY 0.8
#define PDF_OUT_CAPABILITY 0.5

#define DOC_SETTING_CHARACTER_MAPPING "charactermapping"
#define DOC_SETTING_LANDSCAPE "landscape"
#define DOC_SETTING_PAPER "papersize"
#define DOC_SETTING_REMOVED "removed"
#define DOC_SETTING_HIDDEN "hidden"

// We have to declare the size so that TUs including this header can determine
// it via sizeof.
extern const char * const mappings[30];
extern const char * const paper[3];

class DOCTranslator : public BaseTranslator {
public:

	DOCTranslator();

	virtual status_t DerivedIdentify(BPositionIO *inSource,
		const translation_format *inFormat, BMessage *ioExtension,
		translator_info *outInfo, uint32 outType);

	virtual status_t DerivedTranslate(BPositionIO *inSource,
		const translator_info *inInfo, BMessage *ioExtension, uint32 outType,
		BPositionIO *outDestination, int32 baseType);

	virtual BView *NewConfigView(TranslatorSettings *settings);

protected:
	virtual ~DOCTranslator();
	// Protected since the object is only destroyed when reference count
	// reaches zero

};

#endif // DOC_TRANSLATOR_H

