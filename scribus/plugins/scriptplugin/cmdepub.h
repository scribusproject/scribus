/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef CMDEPUB_H
#define CMDEPUB_H

#include "cmdvar.h"

PyDoc_STRVAR(scribus_epubreadingorderpreflight__doc__,
QT_TR_NOOP("epubReadingOrderPreflight([paragraphStyles, characterStyles]) -> dict\n\
\n\
Read the current document's saved EPUB story order without changing it.\n\
paragraphStyles maps named styles to 0 (paragraph) or 1-6 (heading level).\n\
characterStyles maps named styles to 'emphasis' or 'strong'. Unmapped\n\
named styles and unsupported list/drop-cap variants fail explicitly.\n\
Returns status, detail,\n\
ordered paragraphs, matching headingLevels, inlineRuns, blockKinds, and\n\
listStarts, listStartNumbers (zero except at ordered-list starts), and\n\
listTypes ('1', 'i', or 'I' at ordered-list starts; empty otherwise).\n\
Flat standard-bullet and local decimal/Roman lists are supported. Roman\n\
numbers must remain within 1-3999; other numbering, nesting, custom bullets, and drop\n\
caps still fail.\n\
This does not check full visual styling, images, accessibility, or complete\n\
EPUB export readiness.\n\
"));

PyObject* scribus_epubreadingorderpreflight(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_epubmixedreadingorderpreflight__doc__,
QT_TR_NOOP("epubMixedReadingOrderPreflight([paragraphStyles, characterStyles]) -> dict\n\
\n\
Read the saved order of whole text stories and linked PNG/JPEG image frames.\n\
Returns status, detail, ordered blockKinds and texts (image text is alt text),\n\
imageIndices (-1 for text), imageMediaTypes, imageFidelityWarnings, structured\n\
issues, and usedParagraphStyles/usedCharacterStyles. Each fidelity warning\n\
identifies a frame and settings omitted from reflowable output.\n\
Named style mappings use\n\
the same rules as epubReadingOrderPreflight. This does not write an EPUB or\n\
preserve general image-frame transforms, page layout, or colour management;\n\
explicit exact PNG crops are reported when applied.\n\
"));

PyObject* scribus_epubmixedreadingorderpreflight(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_epubimageframepreflight__doc__,
QT_TR_NOOP("epubImageFramePreflight(frameName, [altText]) -> dict\n\
\n\
Read and validate one linked PNG/JPEG image frame without changing the document.\n\
Omitting altText uses the description saved on the frame. Returns status,\n\
detail, mediaType, byteSize, readingOrder, decorative, widthPercent,\n\
frameCropApplied, optional caption, and captionAlignment.\n\
Alt text must be non-empty unless the frame is explicitly decorative. This does\n\
not establish mixed text/image reading order, preserve general frame or colour\n\
transforms, or export the image; an explicit exact PNG crop is supported.\n\
"));

PyObject* scribus_epubimageframepreflight(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_setepubimagealttext__doc__,
QT_TR_NOOP("setEpubImageAltText(frameName, altText) -> None\n\
\n\
Save an EPUB image description on a normal-page image frame. An empty string\n\
clears it. This does not change the image link or its visible caption.\n\
"));

PyObject* scribus_setepubimagealttext(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_setepubimagedecorative__doc__,
QT_TR_NOOP("setEpubImageDecorative(frameName, decorative) -> None\n\
\n\
Save an explicit decorative-image flag. Clear the EPUB image description and\n\
visible caption first. Decorative images export with empty alt text and a\n\
presentation role; other images still require meaningful alt text.\n\
"));

PyObject* scribus_setepubimagedecorative(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_setepubimagecaption__doc__,
QT_TR_NOOP("setEpubImageCaption(frameName, caption) -> None\n\
\n\
Save an optional visible EPUB figure caption on a normal-page image frame.\n\
Pass an empty string to clear it. Captions do not replace required alt text;\n\
decorative images cannot have captions.\n\
"));

PyObject* scribus_setepubimagecaption(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_setepubimagecaptionalignment__doc__,
QT_TR_NOOP("setEpubImageCaptionAlignment(frameName, alignment) -> None\n\
\n\
Align a saved visible EPUB caption without moving its image. alignment is\n\
one of 'left', 'center', 'right', or 'justify'. Left is the default. The image\n\
must already have a caption; clearing that caption also clears this override.\n\
"));

PyObject* scribus_setepubimagecaptionalignment(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_setepubimagewidthpercent__doc__,
QT_TR_NOOP("setEpubImageWidthPercent(frameName, widthPercent) -> None\n\
\n\
Set a reflowable EPUB figure width from 1 to 100 percent of the reading area.\n\
Zero clears the override and keeps the reader default. This does not reproduce\n\
the Scribus image frame crop, rotation, effects, or colour management.\n\
"));

PyObject* scribus_setepubimagewidthpercent(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_setepubuseimageframecrop__doc__,
QT_TR_NOOP("setEpubUseImageFrameCrop(frameName, enabled) -> None\n\
\n\
Opt in to an exact source-pixel crop from a plain rectangular linked PNG frame.\n\
Preflight rejects rotations, flips, masks, effects, fractional crop boundaries,\n\
or a viewport not fully covered by the image. Disabling keeps original bytes.\n\
"));

PyObject* scribus_setepubuseimageframecrop(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_setepubimagereadingorder__doc__,
QT_TR_NOOP("setEpubImageReadingOrder(frameName, rank) -> None\n\
\n\
Store a 1-based EPUB rank on a printable normal-page image frame.\n\
Zero clears the rank. Every text story and image needs a unique contiguous\n\
rank for mixed-content export.\n\
"));

PyObject* scribus_setepubimagereadingorder(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_exportepubtextonly__doc__,
QT_TR_NOOP("exportEpubTextOnly(outputPath, identifier, title, language, [paragraphStyles, characterStyles, author]) -> dict\n\
\n\
Write a limited reflowable EPUB from the current document's saved text-story order.\n\
outputPath must be absolute and must not already exist. Metadata is explicit;\n\
paragraphStyles and characterStyles use epubReadingOrderPreflight mappings.\n\
Returns status ('exported' on success) and detail. Unsupported content is\n\
rejected before writing. This text-only API does not preserve full visual\n\
styling or linked assets and is not a general-purpose EPUB export command.\n\
"));

PyObject* scribus_exportepubtextonly(PyObject* self, PyObject* args);

PyDoc_STRVAR(scribus_exportepubtextandimages__doc__,
QT_TR_NOOP("exportEpubTextAndImages(outputPath, identifier, title, language, [paragraphStyles, characterStyles, author]) -> dict\n\
\n\
Write a limited reflowable EPUB from saved ranks on complete text stories\n\
and linked PNG/JPEG image frames with saved alt text and optional captions.\n\
The output path must be\n\
absolute and new. Unsupported content is rejected before writing. Images\n\
use original file pixels unless an exact PNG frame crop is explicitly selected.\n\
Page placement, effects, and colour adjustments are not reproduced. This is\n\
not a general EPUB export.\n\
"));

PyObject* scribus_exportepubtextandimages(PyObject* self, PyObject* args);

#endif
