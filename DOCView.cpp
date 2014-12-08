// DOCView.cpp
// Copyright (c) 2014 Markus Himmel <markus@himmel-villmar.de>
// This file is distributed under the terms of the MIT license

#include "DOCView.h"

#include <stdio.h>

#include <Alert.h>
#include <Catalog.h>
#include <CheckBox.h>
#include <LayoutBuilder.h>
#include <MenuBar.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <StringView.h>

#include "DOCTranslator.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DOCTranslator"

BMenuItem *
generate_item(uint32 index, uint32 current)
{
	BMessage *message = new BMessage(DOCView::MSG_CHARMAP_CHANGED);
	message->AddInt32("value", index);

	BMenuItem *item = new BMenuItem(mappings[index], message);
	item->SetMarked(current == index);

	return item;
}

DOCView::DOCView(const BRect &frame, const char *name, uint32 resizeMode,
	uint32 flags, TranslatorSettings *settings)
	:
	BView(frame, name, resizeMode, flags| B_FRAME_EVENTS)
{
	(new BAlert("Test", "FromConstructor", "OK"))->Go();
	fSettings = settings;

	fSettings->Acquire();
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetLowColor(ViewColor());

	fTitle = new BStringView("title", B_TRANSLATE("DOC document translator"));
	fTitle->SetFont(be_bold_font);

	char version[256];
	snprintf(version, sizeof(version), B_TRANSLATE("Version %d.%d.%d, %s"),
		static_cast<int>(B_TRANSLATION_MAJOR_VERSION(DOC_TRANSLATOR_VERSION)),
		static_cast<int>(B_TRANSLATION_MINOR_VERSION(DOC_TRANSLATOR_VERSION)),
		static_cast<int>(B_TRANSLATION_REVISION_VERSION(
			DOC_TRANSLATOR_VERSION)),
		__DATE__);
	fInfo = new BStringView("version", version);

	fAuthor = new BStringView(
		"Copyright ",
		B_UTF8_COPYRIGHT "2014 Markus Himmel <markus@himmel-villmar.de>");

	BPopUpMenu *menu = new BPopUpMenu("mapping");

	uint32 currentMapping = fSettings->SetGetInt32(
		DOC_SETTING_CHARACTER_MAPPING);

	size_t numMappings = sizeof(mappings) / sizeof(char *);

	for (int i = 0; i < numMappings; i++)
	{
		menu->AddItem(generate_item(i, currentMapping));
	}

	fCharacterMapping = new BMenuField(B_TRANSLATE("Character mapping:"), menu);

	BMessage *msg = new BMessage(DOCView::MSG_LANDSCAPE_CHANGED);

	fLandscape = new BCheckBox(B_TRANSLATE("Landscape mode"), msg);
	bool current = fSettings->SetGetBool(DOC_SETTING_LANDSCAPE);
	fLandscape->SetValue(current);


	BLayoutBuilder::Group<>(this, B_VERTICAL, 7)
		.SetInsets(5)
		.Add(fTitle)
		.Add(fInfo)
		.AddGlue()
		.AddGroup(B_HORIZONTAL)
			.Add(fCharacterMapping)
			.AddGlue()
			.End()
		.Add(fLandscape)
		.AddGlue()
		.Add(fAuthor)
		.AddGlue();
}


DOCView::~DOCView()
{
	fSettings->Release();
}

void
DOCView::AllAttached()
{
	fLandscape->SetTarget(this);
	fCharacterMapping->Menu()->SetTargetForItems(this);
}

void
DOCView::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case MSG_CHARMAP_CHANGED:
			int32 value;
			if (message->FindInt32("value", &value) == B_OK)
			{
				fSettings->SetGetInt32(DOC_SETTING_CHARACTER_MAPPING, &value);
				fSettings->SaveSettings();
			}
			break;

		case MSG_LANDSCAPE_CHANGED:
			bool boolValue;
			boolValue = fLandscape->Value();
			fSettings->SetGetBool(DOC_SETTING_LANDSCAPE, &boolValue);
			fSettings->SaveSettings();
			break;

		default:
			BView::MessageReceived(message);
	}
}

