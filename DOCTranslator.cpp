// DOCTranslator
// Copyright (c) 2014 Markus Himmel <markus@himmel-villmar.de>
// This file is distributed under the terms of the MIT license.

#include "DOCTranslator.h"

#include <cstring>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>

#include <Alert.h>
#include <Catalog.h>
#include <File.h>
#include <FindDirectory.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "DOCTranslator"

// 8859-12 intentionally left out because it does not exist.
const char * const mappings[] = { "8859-1", "8859-2", "8859-3", "8859-4",
	"8859-5", "8859-6", "8859-7", "8859-8", "8859-9", "8859-10", "8859-11",
	"8859-13", "8859-14", "8859-15", "8859-16", "cp437", "cp850", "cp852",
	"cp862", "cp864", "cp866", "cp1250", "cp1251", "cp1252", "koi8-r",
	"koi8-u", "MacCyrillic", "MacRoman", "roman", "UTF-8" };

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
	},
	{
		B_PS_FORMAT,
		B_TRANSLATOR_TEXT,
		PS_OUT_QUALITY,
		PS_OUT_CAPABILITY,
		"application/postscript",
		"PostScript file"
	}
};


static const TranSetting sDefaultSettings[] = {
	{ DOC_SETTING_CHARACTER_MAPPING, TRAN_SETTING_INT32, 0}
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
	BaseTranslator(B_TRANSLATE("DOC documents"),
		B_TRANSLATE("DOC document translator"),
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

	// Get the tmp folder name
	BPath tmpDir;
	if (find_directory(B_SYSTEM_TEMP_DIRECTORY, &tmpDir) != B_OK)
	{
		return B_ERROR;
	}

	BString tmpPath;

	// Template for a temporary file name
	tmpPath.Append(tmpDir.Path()).Append("/DOCTranslator.XXXXXX");

	(new BAlert("T", "Before file get", "OK"))->Go();
	// This is just to get a temporary file name, we use an ofstream
	int tempFileHandle = mkstemp(tmpPath.LockBuffer(0));
	tmpPath.UnlockBuffer();

	if (tempFileHandle == -1)
	{
		return B_ERROR;
	}

	// Get rid of the the C style file stream
	close(tempFileHandle);

	// Read the whole input into a buffer
	off_t *bufferSize;
	inSource->GetSize(bufferSize);
	uint8 fileBuffer[*bufferSize];
	inSource->Read(&fileBuffer, *bufferSize);

	(new BAlert("", "Before open output file", "OK"))->Go();

	// And dump it into a file
	std::ofstream inputFile;
	inputFile.open(tmpPath, ios::out | ios::binary);
	if (!inputFile)
	{
		return B_ERROR;
	}
	inputFile.write(&fileBuffer, *bufferSize);
	inputFile.close();

	// Now execute antiword and have the shell dump stdout into a file
	(new BAlert("", "Before system call", "OK"))->Go();
	BString cmd = BString("antiword ");

	cmd << "-m " // Mapping
		<< mappings[fSettings->SetGetInt32(DOC_SETTING_CHARACTER_MAPPING)]
		<< " "
		<< tmpPath // Source
		<< " > " // Redirect
		<< tmpPath << "1"; // Destination is source with appended 1

	if (system(cmd.String()) == -1)
	{
		return B_ERROR;
	}

	// Read the whole output into a buffer
	BFile returned(tmpPath << "1", O_RDONLY);
	off_t fileSize;
	returned.GetSize(&fileSize);
	uint8 outputBuffer[fileSize];
	returned.Read(outputBuffer, fileSize);

	// And finally feed it to the destination
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

