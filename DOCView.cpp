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
generate_item(uint32 index, uint32 current, uint32 messageID,
	const char * const labels[])
{
	BMessage *message = new BMessage(messageID);
	message->AddInt32("value", index);

	BMenuItem *item = new BMenuItem(labels[index], message);
	item->SetMarked(current == index);

	return item;
}

DOCView::DOCView(const BRect &frame, const char *name, uint32 resizeMode,
	uint32 flags, TranslatorSettings *settings)
	:
	BView(frame, name, resizeMode, flags| B_FRAME_EVENTS)
{
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
		menu->AddItem(generate_item(i, currentMapping, MSG_CHARMAP_CHANGED,
			mappings));
	}

	fCharacterMapping = new BMenuField(B_TRANSLATE("Character mapping:"), menu);

	menu = new BPopUpMenu("paper");

	uint32 currentPaper = fSettings->SetGetInt32(
		DOC_SETTING_PAPER);

	size_t numPaper = sizeof(paper) / sizeof(char *);

	for (int i = 0; i < numPaper; i++)
	{
		menu->AddItem(generate_item(i, currentPaper, MSG_PAPER_CHANGED,
			paper));
	}

	fPaper = new BMenuField(B_TRANSLATE("Paper size:"), menu);


	BMessage *msg = new BMessage(DOCView::MSG_LANDSCAPE_CHANGED);

	fLandscape = new BCheckBox(B_TRANSLATE("Landscape mode"), msg);
	bool current = fSettings->SetGetBool(DOC_SETTING_LANDSCAPE);
	fLandscape->SetValue(current);

	msg = new BMessage(DOCView::MSG_REMOVED_CHANGED);

	fRemoved = new BCheckBox(B_TRANSLATE("Show removed text"), msg);
	current = fSettings->SetGetBool(DOC_SETTING_REMOVED);
	fRemoved->SetValue(current);

	msg = new BMessage(DOCView::MSG_HIDDEN_CHANGED);

	fHidden = new BCheckBox(B_TRANSLATE("Show hidden text"), msg);
	current = fSettings->SetGetBool(DOC_SETTING_HIDDEN);
	fHidden->SetValue(current);


	BLayoutBuilder::Group<>(this, B_VERTICAL, 7)
		.SetInsets(5)
		.Add(fTitle)
		.Add(fInfo)
		.AddGlue()
		.AddGroup(B_HORIZONTAL)
			.Add(fPaper)
			.AddGlue()
			.End()
		.AddGroup(B_HORIZONTAL)
			.Add(fCharacterMapping)
			.AddGlue()
			.End()
		.Add(fLandscape)
		.Add(fHidden)
		.Add(fRemoved)
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
	fRemoved->SetTarget(this);
	fHidden->SetTarget(this);
	fPaper->Menu()->SetTargetForItems(this);
	fCharacterMapping->Menu()->SetTargetForItems(this);
}

void
DOCView::MessageReceived(BMessage *message)
{
	int32 value;
	bool boolValue;
	switch (message->what)
	{
		case MSG_CHARMAP_CHANGED:
			if (message->FindInt32("value", &value) == B_OK)
			{
				fSettings->SetGetInt32(DOC_SETTING_CHARACTER_MAPPING, &value);
				fSettings->SaveSettings();
			}
			break;

		case MSG_LANDSCAPE_CHANGED:
			boolValue = fLandscape->Value();
			fSettings->SetGetBool(DOC_SETTING_LANDSCAPE, &boolValue);
			fSettings->SaveSettings();
			break;

		case MSG_PAPER_CHANGED:
			if (message->FindInt32("value", &value) == B_OK)
			{
				fSettings->SetGetInt32(DOC_SETTING_PAPER, &value);
				fSettings->SaveSettings();
			}

		case MSG_REMOVED_CHANGED:
			boolValue = fRemoved->Value();
			fSettings->SetGetBool(DOC_SETTING_REMOVED, &boolValue);
			fSettings->SaveSettings();
			break;

		case MSG_HIDDEN_CHANGED:
			boolValue = fHidden->Value();
			fSettings->SetGetBool(DOC_SETTING_HIDDEN, &boolValue);
			fSettings->SaveSettings();
			break;


		default:
			BView::MessageReceived(message);
			break;
	}
}

