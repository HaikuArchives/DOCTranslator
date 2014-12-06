// DOCTranslator
// Copyright (c) 2014 Markus Himmel
// This file is distributed under the terms of the MIT license.

#include "DOCTranslator.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <FindDirectory.h>

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

int32
msoffice_signature_cmp(const uint8 *bytes)
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

  if (msoffice_sig_cmp(buffer))
  {
    return B_NO_TRANSLATOR;
  }

  if (outInfo)
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


  if (identify_ms_office_header(inSource, NULL) != B_OK)
  {
    return B_NO_TRANSLATOR;
  }

  // Reset the cursor as it was progressed while identifying the header.
  inSource->Seek(0, SEEK_SET);

  // Antiword needs the input to be present as a file
  BPath tmpDir;
  if (find_directory(B_COMMON_TEMP_DIRECTORY, &tmpDir) != B_OK)
  {
    return B_ERROR;
  }

  tmpDir.Append("doctranslator_tmp.doc");

  size_t bufferSize = inSource->GetSize();

  const uint8 fileBuffer[bufferSize];

  Read(fileBuffer, bufferSize);

  ofstream inputFile;

  inputFile.open(tmpDir.Path(), ios::out | ios::binary);

  if (!inputFile)
  {
    return B_ERROR;
  }

  inputFile.write(fileBuffer, bufferSize);

  // Now execute antiword

  FILE *antiwordHandle = popen("antiword " << tmpDir.Path(), "r");

  if (!antiwordHandle)
  {
    return B_ERROR;
  }

  fseek(antiwordHandle, 0, SEEK_END);
  size_t outputLength = ftell(antiwordHandle);
  fseek(f, 0, SEEK_SET);

  uint8 outputBuffer[outputLength];

  fread(outputBuffer, 1, outputLength, antiwordHandle);

  pclose(antiwordHandle);

  outDestination->Write(outputBuffer, outputLength);
}


BView*
DOCTranslator::NewConfigView(TranslatorSettings *settings)
{
  return new DOCView(BRect(0, 0, DOC_VIEW_WIDTH, PNG_VIEW_HEIGHT),
                  B_TRANSLATE("DOCTranslator Settings"), B_FOLLOW_ALL,
                  B_WILL_DRAW, settings);
}
