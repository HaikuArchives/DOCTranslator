// DOCTranslator
// Copyright (c) 2014 Markus Himmel
// This file is distributed under the terms of the MIT license.

#include "DOCTranslator.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <FindDirectory.h>
#include <Catalog.h>
#include <Alert.h>
#include <File.h>

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

static const TranSetting sDefaultSettings[] = {
	{ "Test setting", TRAN_SETTING_INT32, 2}
};

const uint32 kNumInputFormats = sizeof(sInputFormats) /
	sizeof(translation_format);

const uint32 kNumOutputFormats = sizeof(sOutputFormats) /
	sizeof(translation_format);

const uint32 kNumDefaultSettings = sizeof(sDefaultSettings) /
	sizeof(TranSetting);

BTranslator*
make_nth_translator(int32 n, image_id you, uint32 flags, ...)
{
	if (!n)
	{
		return new DOCTranslator();
	}
	else
	{
		return NULL;
	}
}

DOCTranslator::DOCTranslator()
	:
	BaseTranslator("DOC documents",
		"DOC document translator",
		DOC_TRANSLATOR_VERSION,
		sInputFormats, kNumInputFormats,
		sOutputFormats, kNumOutputFormats,
		"DOCTranslator_Settings",
		sDefaultSettings, kNumDefaultSettings,
		B_TRANSLATOR_TEXT, B_DOC_FORMAT)
{
	(new BAlert("Construct DOCTranslator", "Constructor1", "OK"))->Go();
}

DOCTranslator::~DOCTranslator()
{
}

int32
msoffice_sig_cmp(const uint8 *bytes)
{
	uint8 msoffice_signature[8] = { 0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A,
		0xE1 };

	size_t signature_length = 8;
	return ((int32)(memcmp(msoffice_signature, bytes, signature_length)));
}

status_t
identify_msoffice_header(BPositionIO *inSource, translator_info *outInfo)
{
	const int32 signatureSize = 8;
	uint8 buffer[signatureSize];

	if (inSource->Read(buffer, signatureSize) != signatureSize)
	{
		// Something went wrong reading the signature
		return B_NO_TRANSLATOR;
	}

	if (msoffice_sig_cmp(buffer) != 0)
	{
		return B_NO_TRANSLATOR;
	}

	if (outInfo != NULL)
	{
		outInfo->type = B_DOC_FORMAT;
		outInfo->group = B_TRANSLATOR_TEXT;
		outInfo->quality = DOC_IN_QUALITY;
		outInfo->capability = DOC_IN_QUALITY;
		strcpy(outInfo->MIME, "application/msword");
		strlcpy(outInfo->name, B_TRANSLATE("Microsoft Word document"),
			sizeof(outInfo->name));
	}

	return B_OK;
}

status_t
DOCTranslator::DerivedIdentify(BPositionIO *source,
	const translation_format *inFormat, BMessage *ioExtension,
	translator_info *outInfo, uint32 outType)
{
	(new BAlert("!", "Identifying...", "OK"))->Go();
	// This only checks if a given file has the header shared by all binary
	// Microsoft office formats. I do not think it is easily possible to detect
	// Word documents in particular.
	return identify_msoffice_header(source, outInfo);
}

status_t
DOCTranslator::DerivedTranslate(BPositionIO *inSource,
	const translator_info *inInfo, BMessage *ioExtension,
	uint32 outType, BPositionIO *outDestination, int32 baseType)
{
	// Note: BaseType will always be -1 because this is a B_TRANSLATOR_TEXT
	(new BAlert("a", "Translate", "OK"))->Go();

	if (identify_msoffice_header(inSource, NULL) != B_OK)
	{
		return B_NO_TRANSLATOR;
	}

	// Reset the cursor as it was progressed while identifying the header.
	inSource->Seek(0, SEEK_SET);

	// Antiword needs the input to be present as a file
	BPath tmpDir;
	if (find_directory(B_SYSTEM_TEMP_DIRECTORY, &tmpDir) != B_OK)
	{
		return B_ERROR;
	}

	BString tmpPath;

	tmpPath.Append(tmpDir.Path()).Append("/DOCTranslator.XXXXXX");
	(new BAlert("T", "Before file get", "OK"))->Go();
	// This is just to get a temporary file name, we use an ofstream
	int tempFileHandle = mkstemp(tmpPath.LockBuffer(0));
	tmpPath.UnlockBuffer();

	if (tempFileHandle == -1)
	{
		return B_ERROR;
	}

	close(tempFileHandle);

	off_t *bufferSize;
	inSource->GetSize(bufferSize);

	uint8 fileBuffer[*bufferSize];

	inSource->Read(&fileBuffer, *bufferSize);
	(new BAlert("", "Before open output file", "OK"))->Go();
	std::ofstream inputFile;

	inputFile.open(tmpPath, ios::out | ios::binary);

	if (!inputFile)
	{
		return B_ERROR;
	}

	inputFile.write(&fileBuffer, *bufferSize);

	inputFile.close();
	// Now execute antiword
	(new BAlert("", "Before system call", "OK"))->Go();

	BString cmdName = BString("antiword ");

	if (system((cmdName << tmpPath << " > " << tmpPath << "1").String()) == -1)
	{
		return B_ERROR;
	}

	BFile returned(tmpPath << "1", O_RDONLY);

	off_t fileSize;

	returned.GetSize(&fileSize);

	uint8 outputBuffer[fileSize];

	returned.Read(outputBuffer, fileSize);

	outDestination->Write(outputBuffer, fileSize);
	(new BAlert("", "Exiting...", "OK"))->Go();
}


BView*
DOCTranslator::NewConfigView(TranslatorSettings *settings)
{
	(new BAlert("Test", "Test", "Test"))->Go();
	return new DOCView(BRect(0, 0, DOC_VIEW_WIDTH, DOC_VIEW_HEIGHT),
	B_TRANSLATE("DOCTranslator Settings"), 0, 0, settings);
}

