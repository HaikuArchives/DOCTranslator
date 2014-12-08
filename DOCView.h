// DOCView.h
// Copyright (c) 2014 Markus Himmel <markus@himmel-villmar.de>
// This file is distributed under the terms of the MIT license

#ifndef DOC_VIEW_H
#define DOC_VIEW_H

#include <CheckBox.h>
#include <MenuField.h>
#include <StringView.h>
#include <View.h>

#include "TranslatorSettings.h"

#define DOC_VIEW_WIDTH 300
#define DOC_VIEW_HEIGHT 270

class DOCView : public BView  {
public:
	DOCView(const BRect &frame, const char *name, uint32 resizeMode,
		uint32 flags, TranslatorSettings *settings);
	~DOCView();

	enum
	{
		MSG_CHARMAP_CHANGED = 'cmch',
		MSG_LANDSCAPE_CHANGED = 'lsch'
	};

	virtual void AllAttached();
	virtual void MessageReceived(BMessage *message);

private:
	TranslatorSettings *fSettings;

	BStringView *fTitle;
	BStringView *fInfo;
	BStringView *fAuthor;

	BCheckBox *fLandscape;

	BMenuField *fCharacterMapping;
};

#endif // DOC_VIEW_H

