/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "cmdepub.h"

#include "cmdutil.h"
#include "epubdocument.h"
#include "pageitem.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusdoc.h"

#include <QDir>
#include <QStringList>

#include <cmath>
#include <limits>

namespace
{
const char* statusName(EpubReadingOrder::Status status)
{
	switch (status)
	{
		case EpubReadingOrder::Status::Ready: return "ready";
		case EpubReadingOrder::Status::Empty: return "empty";
		case EpubReadingOrder::Status::Ambiguous: return "ambiguous";
		case EpubReadingOrder::Status::InvalidChain: return "invalid-chain";
		case EpubReadingOrder::Status::InvalidOrder: return "invalid-order";
		case EpubReadingOrder::Status::UnsupportedContent: return "unsupported-content";
	}
	return "unsupported-content";
}

PyObject* unicode(const QString& value)
{
	const QByteArray utf8 = value.toUtf8();
	return PyUnicode_DecodeUTF8(utf8.constData(), utf8.size(), "strict");
}

PyObject* unicodeList(const QStringList& values)
{
	PyObject* list = PyList_New(values.size());
	if (!list)
		return nullptr;
	for (Py_ssize_t index = 0; index < values.size(); ++index)
	{
		PyObject* value = unicode(values.at(index));
		if (!value)
		{
			Py_DECREF(list);
			return nullptr;
		}
		PyList_SET_ITEM(list, index, value);
	}
	return list;
}

const char* inlineKindName(EpubExport::InlineKind kind)
{
	switch (kind)
	{
		case EpubExport::InlineKind::Plain: return "plain";
		case EpubExport::InlineKind::Emphasis: return "emphasis";
		case EpubExport::InlineKind::Strong: return "strong";
	}
	return "plain";
}

const char* blockKindName(EpubExport::BlockKind kind)
{
	switch (kind)
	{
		case EpubExport::BlockKind::Paragraph: return "paragraph";
		case EpubExport::BlockKind::Heading: return "heading";
		case EpubExport::BlockKind::BulletItem: return "bullet-item";
		case EpubExport::BlockKind::OrderedItem: return "ordered-item";
		case EpubExport::BlockKind::Image: return "image";
	}
	return "paragraph";
}

const char* alignmentName(EpubExport::TextAlignment alignment)
{
	switch (alignment)
	{
		case EpubExport::TextAlignment::Left: return "left";
		case EpubExport::TextAlignment::Center: return "center";
		case EpubExport::TextAlignment::Right: return "right";
		case EpubExport::TextAlignment::Justify: return "justify";
	}
	return "left";
}

PyObject* inlineRuns(const EpubExport::Block& block)
{
	const Py_ssize_t count = block.runs.isEmpty() ? (block.text.isEmpty() ? 0 : 1) : block.runs.size();
	PyObject* list = PyList_New(count);
	if (!list)
		return nullptr;
	for (Py_ssize_t index = 0; index < count; ++index)
	{
		const EpubExport::InlineRun run = block.runs.isEmpty()
			? EpubExport::InlineRun { EpubExport::InlineKind::Plain, block.text } : block.runs.at(index);
		PyObject* kind = PyUnicode_FromString(inlineKindName(run.kind));
		PyObject* value = unicode(run.text);
		PyObject* pair = kind && value ? PyTuple_Pack(2, kind, value) : nullptr;
		Py_XDECREF(kind);
		Py_XDECREF(value);
		if (!pair)
		{
			Py_DECREF(list);
			return nullptr;
		}
		PyList_SET_ITEM(list, index, pair);
	}
	return list;
}

bool parseMappings(PyObject* styleMappings, PyObject* characterMappings,
	EpubReadingOrder::ParagraphStyleMap& styles,
	EpubReadingOrder::CharacterStyleMap& characterStyles)
{
	if (styleMappings)
	{
		if (!PyDict_Check(styleMappings))
		{
			PyErr_SetString(PyExc_TypeError, "paragraphStyles must be a dict of style names to heading levels (0-6)");
			return false;
		}
		PyObject* key = nullptr;
		PyObject* value = nullptr;
		Py_ssize_t cursor = 0;
		while (PyDict_Next(styleMappings, &cursor, &key, &value))
		{
			if (!PyUnicode_Check(key) || !PyLong_Check(value) || PyBool_Check(value))
			{
				PyErr_SetString(PyExc_TypeError, "style mappings require string names and integer levels");
				return false;
			}
			Py_ssize_t byteCount = 0;
			const char* utf8 = PyUnicode_AsUTF8AndSize(key, &byteCount);
			const long level = PyLong_AsLong(value);
			if (!utf8 || PyErr_Occurred())
				return false;
			const QString name = QString::fromUtf8(utf8, byteCount);
			if (name.trimmed().isEmpty() || name.contains(QChar::Null) || level < 0 || level > 6)
			{
				PyErr_SetString(PyExc_ValueError, "style names must be non-empty and levels must be 0-6");
				return false;
			}
			styles.insert(name, static_cast<int>(level));
		}
	}
	if (characterMappings)
	{
		if (!PyDict_Check(characterMappings))
		{
			PyErr_SetString(PyExc_TypeError, "characterStyles must be a dict of style names to 'emphasis' or 'strong'");
			return false;
		}
		PyObject* key = nullptr;
		PyObject* value = nullptr;
		Py_ssize_t cursor = 0;
		while (PyDict_Next(characterMappings, &cursor, &key, &value))
		{
			if (!PyUnicode_Check(key) || !PyUnicode_Check(value))
			{
				PyErr_SetString(PyExc_TypeError, "character style mappings require string names and semantics");
				return false;
			}
			Py_ssize_t nameBytes = 0;
			Py_ssize_t semanticBytes = 0;
			const char* nameUtf8 = PyUnicode_AsUTF8AndSize(key, &nameBytes);
			const char* semanticUtf8 = PyUnicode_AsUTF8AndSize(value, &semanticBytes);
			if (!nameUtf8 || !semanticUtf8)
				return false;
			const QString name = QString::fromUtf8(nameUtf8, nameBytes);
			const QString semantic = QString::fromUtf8(semanticUtf8, semanticBytes);
			if (name.trimmed().isEmpty() || name.contains(QChar::Null) ||
				(semantic != QLatin1String("emphasis") && semantic != QLatin1String("strong")))
			{
				PyErr_SetString(PyExc_ValueError, "character styles require non-empty names and 'emphasis' or 'strong'");
				return false;
			}
			characterStyles.insert(name, semantic == QLatin1String("strong")
				? EpubExport::InlineKind::Strong : EpubExport::InlineKind::Emphasis);
		}
	}
	return true;
}

QString pythonText(PyObject* value)
{
	Py_ssize_t length = 0;
	const char* utf8 = PyUnicode_AsUTF8AndSize(value, &length);
	return utf8 ? QString::fromUtf8(utf8, length) : QString();
}

const char* exportStatusName(EpubExport::Status status)
{
	switch (status)
	{
		case EpubExport::Status::Exported: return "exported";
		case EpubExport::Status::InvalidInput: return "invalid-input";
		case EpubExport::Status::OutputExists: return "output-exists";
		case EpubExport::Status::IoError: return "io-error";
		case EpubExport::Status::ArchiveError: return "archive-error";
	}
	return "archive-error";
}

PyObject* statusResult(const char* status, const QString& detail)
{
	PyObject* result = PyDict_New();
	PyObject* pyStatus = PyUnicode_FromString(status);
	PyObject* pyDetail = unicode(detail);
	if (!result || !pyStatus || !pyDetail)
	{
		Py_XDECREF(result);
		Py_XDECREF(pyStatus);
		Py_XDECREF(pyDetail);
		return nullptr;
	}
	const bool ok = PyDict_SetItemString(result, "status", pyStatus) == 0 &&
		PyDict_SetItemString(result, "detail", pyDetail) == 0;
	Py_DECREF(pyStatus);
	Py_DECREF(pyDetail);
	if (!ok)
	{
		Py_DECREF(result);
		return nullptr;
	}
	return result;
}

PyObject* imageFidelityWarnings(const ScribusDoc& document)
{
	PyObject* warnings = PyList_New(0);
	if (!warnings)
		return nullptr;
	for (const PageItem* item : EpubDocument::rankableItems(document))
	{
		if (!item->isImageFrame())
			continue;
		QStringList omitted { QStringLiteral("frame-geometry"), QStringLiteral("colour-management") };
		const bool cropped = EpubDocument::savedUseImageFrameCrop(item);
		if (!cropped && (std::abs(item->imageXOffset()) > 0.000001 || std::abs(item->imageYOffset()) > 0.000001))
			omitted.append(QStringLiteral("image-offset"));
		if (!cropped && (std::abs(item->imageXScale() - 1.0) > 0.000001 ||
			std::abs(item->imageYScale() - 1.0) > 0.000001))
			omitted.append(QStringLiteral("image-scale"));
		if (std::abs(item->imageRotation()) > 0.000001 || std::abs(item->rotation()) > 0.000001)
			omitted.append(QStringLiteral("rotation"));
		if (item->imageFlippedH() || item->imageFlippedV())
			omitted.append(QStringLiteral("image-flip"));
		if (!item->effectsInUse.isEmpty())
			omitted.append(QStringLiteral("image-effects"));
		PyObject* frame = unicode(item->itemName());
		PyObject* categories = PyList_New(omitted.size());
		if (!frame || !categories)
		{
			Py_XDECREF(frame);
			Py_XDECREF(categories);
			Py_DECREF(warnings);
			return nullptr;
		}
		for (Py_ssize_t index = 0; index < omitted.size(); ++index)
		{
			PyObject* category = unicode(omitted.at(index));
			if (!category)
			{
				Py_DECREF(frame);
				Py_DECREF(categories);
				Py_DECREF(warnings);
				return nullptr;
			}
			PyList_SET_ITEM(categories, index, category);
		}
		PyObject* warning = PyDict_New();
		const bool ok = warning && PyDict_SetItemString(warning, "frameName", frame) == 0 &&
			PyDict_SetItemString(warning, "omitted", categories) == 0 &&
			PyList_Append(warnings, warning) == 0;
		Py_DECREF(frame);
		Py_DECREF(categories);
		Py_XDECREF(warning);
		if (!ok)
		{
			Py_DECREF(warnings);
			return nullptr;
		}
	}
	return warnings;
}

bool addLineSpacingWarnings(PyObject* result, const ScribusDoc& document)
{
	PyObject* warnings = PyList_New(0);
	if (!warnings)
		return false;
	for (const QString& warning : EpubDocument::lineSpacingWarnings(document))
	{
		PyObject* message = unicode(warning);
		const bool ok = message && PyList_Append(warnings, message) == 0;
		Py_XDECREF(message);
		if (!ok)
		{
			Py_DECREF(warnings);
			return false;
		}
	}
	const bool ok = PyDict_SetItemString(result, "lineSpacingWarnings", warnings) == 0;
	Py_DECREF(warnings);
	return ok;
}
}

PyObject* scribus_epubmixedreadingorderpreflight(PyObject* /*self*/, PyObject* args)
{
	PyObject* styleMappings = nullptr;
	PyObject* characterMappings = nullptr;
	if (!PyArg_ParseTuple(args, "|OO", &styleMappings, &characterMappings))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	EpubReadingOrder::ParagraphStyleMap styles;
	EpubReadingOrder::CharacterStyleMap characterStyles;
	if (!parseMappings(styleMappings, characterMappings, styles, characterStyles))
		return nullptr;
	const ScribusDoc* document = ScCore->primaryMainWindow()->doc;
	const EpubDocument::PreflightReport preflight = EpubDocument::preflightMixedSavedOrder(*document, {}, {}, false, styles, characterStyles);
	const EpubDocument::Result& extracted = preflight.extraction;
	PyObject* result = statusResult(statusName(extracted.status), extracted.detail);
	PyObject* kinds = PyList_New(0);
	PyObject* texts = PyList_New(0);
	PyObject* imageIndices = PyList_New(0);
	PyObject* mediaTypes = PyList_New(0);
	if (!result || !kinds || !texts || !imageIndices || !mediaTypes)
	{
		Py_XDECREF(result);
		Py_XDECREF(kinds);
		Py_XDECREF(texts);
		Py_XDECREF(imageIndices);
		Py_XDECREF(mediaTypes);
		return nullptr;
	}
	for (const EpubExport::Block& block : extracted.book.blocks)
	{
		PyObject* kind = PyUnicode_FromString(blockKindName(block.kind));
		PyObject* value = unicode(block.text);
		PyObject* imageIndex = PyLong_FromLong(block.imageIndex);
		const bool ok = kind && value && imageIndex &&
			PyList_Append(kinds, kind) == 0 && PyList_Append(texts, value) == 0 &&
			PyList_Append(imageIndices, imageIndex) == 0;
		Py_XDECREF(kind);
		Py_XDECREF(value);
		Py_XDECREF(imageIndex);
		if (!ok)
		{
			Py_DECREF(result);
			Py_DECREF(kinds);
			Py_DECREF(texts);
			Py_DECREF(imageIndices);
			Py_DECREF(mediaTypes);
			return nullptr;
		}
	}
	for (const EpubExport::ImageAsset& image : extracted.book.images)
	{
		PyObject* mediaType = unicode(image.mediaType);
		const bool ok = mediaType && PyList_Append(mediaTypes, mediaType) == 0;
		Py_XDECREF(mediaType);
		if (!ok)
		{
			Py_DECREF(result);
			Py_DECREF(kinds);
			Py_DECREF(texts);
			Py_DECREF(imageIndices);
			Py_DECREF(mediaTypes);
			return nullptr;
		}
	}
	const bool ok = PyDict_SetItemString(result, "blockKinds", kinds) == 0 &&
		PyDict_SetItemString(result, "texts", texts) == 0 &&
		PyDict_SetItemString(result, "imageIndices", imageIndices) == 0 &&
		PyDict_SetItemString(result, "imageMediaTypes", mediaTypes) == 0;
	Py_DECREF(kinds);
	Py_DECREF(texts);
	Py_DECREF(imageIndices);
	Py_DECREF(mediaTypes);
	if (!ok)
	{
		Py_DECREF(result);
		return nullptr;
	}
	PyObject* warnings = imageFidelityWarnings(*document);
	if (!warnings || PyDict_SetItemString(result, "imageFidelityWarnings", warnings) != 0)
	{
		Py_XDECREF(warnings);
		Py_DECREF(result);
		return nullptr;
	}
	Py_DECREF(warnings);
	if (!addLineSpacingWarnings(result, *document))
	{
		Py_DECREF(result);
		return nullptr;
	}
	PyObject* issues = PyList_New(0);
	if (!issues)
	{
		Py_DECREF(result);
		return nullptr;
	}
	for (const EpubDocument::PreflightIssue& issue : preflight.issues)
	{
		PyObject* entry = PyDict_New();
		PyObject* severity = PyUnicode_FromString(issue.severity == EpubDocument::PreflightSeverity::Error ? "error" : "warning");
		PyObject* code = unicode(issue.code);
		PyObject* detail = unicode(issue.detail);
		const bool ok = entry && severity && code && detail &&
			PyDict_SetItemString(entry, "severity", severity) == 0 &&
			PyDict_SetItemString(entry, "code", code) == 0 &&
			PyDict_SetItemString(entry, "detail", detail) == 0 &&
			PyList_Append(issues, entry) == 0;
		Py_XDECREF(entry);
		Py_XDECREF(severity);
		Py_XDECREF(code);
		Py_XDECREF(detail);
		if (!ok)
		{
			Py_DECREF(issues);
			Py_DECREF(result);
			return nullptr;
		}
	}
	const bool issuesAdded = PyDict_SetItemString(result, "issues", issues) == 0;
	Py_DECREF(issues);
	if (!issuesAdded)
	{
		Py_DECREF(result);
		return nullptr;
	}
	const EpubDocument::StyleNames usedStyles = EpubDocument::usedStyleNames(*document);
	PyObject* paragraphNames = unicodeList(usedStyles.paragraph);
	PyObject* characterNames = unicodeList(usedStyles.character);
	const bool namesAdded = paragraphNames && characterNames &&
		PyDict_SetItemString(result, "usedParagraphStyles", paragraphNames) == 0 &&
		PyDict_SetItemString(result, "usedCharacterStyles", characterNames) == 0;
	Py_XDECREF(paragraphNames);
	Py_XDECREF(characterNames);
	if (!namesAdded)
	{
		Py_DECREF(result);
		return nullptr;
	}
	return result;
}

PyObject* scribus_epubimageframepreflight(PyObject* /*self*/, PyObject* args)
{
	PyObject* frameNameObject = nullptr;
	PyObject* altTextObject = nullptr;
	if (!PyArg_ParseTuple(args, "O|O", &frameNameObject, &altTextObject))
		return nullptr;
	if (!PyUnicode_Check(frameNameObject) || (altTextObject && !PyUnicode_Check(altTextObject)))
	{
		PyErr_SetString(PyExc_TypeError, "frameName and optional altText must be strings");
		return nullptr;
	}
	if (!checkHaveDocument())
		return nullptr;
	const QString frameName = pythonText(frameNameObject);
	const QString altText = altTextObject ? pythonText(altTextObject) : QString();
	if (PyErr_Occurred())
		return nullptr;
	PageItem* item = GetUniqueImageItem(frameName);
	if (!item)
		return nullptr;
	const EpubDocument::ImageFrameResult inspected = altTextObject
		? EpubDocument::extractLinkedImageFrame(item, altText)
		: EpubDocument::extractLinkedImageFrame(item);
	PyObject* result = statusResult(statusName(inspected.status), inspected.detail);
	PyObject* mediaType = unicode(inspected.asset.mediaType);
	PyObject* byteSize = PyLong_FromSsize_t(inspected.asset.data.size());
	PyObject* readingOrder = PyLong_FromLong(EpubDocument::savedOrder(item));
	PyObject* caption = unicode(inspected.block.caption);
	const int savedCaptionAlignment = EpubDocument::savedImageCaptionAlignment(item);
	PyObject* captionAlignment = PyUnicode_FromString(savedCaptionAlignment < 0
		? "invalid" : alignmentName(static_cast<EpubExport::TextAlignment>(savedCaptionAlignment)));
	PyObject* decorative = PyBool_FromLong(EpubDocument::savedImageDecorative(item));
	PyObject* widthPercent = PyLong_FromLong(EpubDocument::savedImageWidthPercent(item));
	PyObject* frameCropApplied = PyBool_FromLong(inspected.cropped);
	if (!result || !mediaType || !byteSize || !readingOrder || !caption || !captionAlignment ||
		!decorative || !widthPercent || !frameCropApplied)
	{
		Py_XDECREF(result);
		Py_XDECREF(mediaType);
		Py_XDECREF(byteSize);
		Py_XDECREF(readingOrder);
		Py_XDECREF(caption);
		Py_XDECREF(captionAlignment);
		Py_XDECREF(decorative);
		Py_XDECREF(widthPercent);
		Py_XDECREF(frameCropApplied);
		return nullptr;
	}
	const bool ok = PyDict_SetItemString(result, "mediaType", mediaType) == 0 &&
		PyDict_SetItemString(result, "byteSize", byteSize) == 0 &&
		PyDict_SetItemString(result, "readingOrder", readingOrder) == 0 &&
		PyDict_SetItemString(result, "caption", caption) == 0 &&
		PyDict_SetItemString(result, "captionAlignment", captionAlignment) == 0 &&
		PyDict_SetItemString(result, "decorative", decorative) == 0 &&
		PyDict_SetItemString(result, "widthPercent", widthPercent) == 0 &&
		PyDict_SetItemString(result, "frameCropApplied", frameCropApplied) == 0;
	Py_DECREF(mediaType);
	Py_DECREF(byteSize);
	Py_DECREF(readingOrder);
	Py_DECREF(caption);
	Py_DECREF(captionAlignment);
	Py_DECREF(decorative);
	Py_DECREF(widthPercent);
	Py_DECREF(frameCropApplied);
	if (!ok)
	{
		Py_DECREF(result);
		return nullptr;
	}
	return result;
}

PyObject* scribus_setepubimagealttext(PyObject* /*self*/, PyObject* args)
{
	PyObject* frameNameObject = nullptr;
	PyObject* altTextObject = nullptr;
	if (!PyArg_ParseTuple(args, "OO", &frameNameObject, &altTextObject))
		return nullptr;
	if (!PyUnicode_Check(frameNameObject) || !PyUnicode_Check(altTextObject))
	{
		PyErr_SetString(PyExc_TypeError, "frameName and altText must be strings");
		return nullptr;
	}
	if (!checkHaveDocument())
		return nullptr;
	const QString frameName = pythonText(frameNameObject);
	const QString altText = pythonText(altTextObject);
	if (PyErr_Occurred())
		return nullptr;
	PageItem* item = GetUniqueImageItem(frameName);
	if (!item)
		return nullptr;
	if (!EpubDocument::setSavedImageAltText(item, altText))
	{
		PyErr_SetString(PyExc_ValueError, "A normal-page image frame and valid alt text are required");
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject* scribus_setepubimagedecorative(PyObject* /*self*/, PyObject* args)
{
	PyObject* frameNameObject = nullptr;
	PyObject* decorativeObject = nullptr;
	if (!PyArg_ParseTuple(args, "OO", &frameNameObject, &decorativeObject))
		return nullptr;
	if (!PyUnicode_Check(frameNameObject) || !PyBool_Check(decorativeObject))
	{
		PyErr_SetString(PyExc_TypeError, "frameName must be a string and decorative a bool");
		return nullptr;
	}
	if (!checkHaveDocument())
		return nullptr;
	const QString frameName = pythonText(frameNameObject);
	if (PyErr_Occurred())
		return nullptr;
	PageItem* item = GetUniqueImageItem(frameName);
	if (!item)
		return nullptr;
	if (!EpubDocument::setSavedImageDecorative(item, decorativeObject == Py_True))
	{
		PyErr_SetString(PyExc_ValueError, "Clear the image description and caption before marking it decorative");
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject* scribus_setepubimagecaption(PyObject* /*self*/, PyObject* args)
{
	PyObject* frameNameObject = nullptr;
	PyObject* captionObject = nullptr;
	if (!PyArg_ParseTuple(args, "UU", &frameNameObject, &captionObject))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString frameName = pythonText(frameNameObject);
	const QString caption = pythonText(captionObject);
	if (PyErr_Occurred())
		return nullptr;
	PageItem* item = GetUniqueImageItem(frameName);
	if (!item)
		return nullptr;
	if (!EpubDocument::setSavedImageCaption(item, caption))
	{
		PyErr_SetString(PyExc_ValueError, "A normal-page image frame and valid caption text are required");
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject* scribus_setepubimagecaptionalignment(PyObject* /*self*/, PyObject* args)
{
	PyObject* frameNameObject = nullptr;
	PyObject* alignmentObject = nullptr;
	if (!PyArg_ParseTuple(args, "UU", &frameNameObject, &alignmentObject))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	const QString frameName = pythonText(frameNameObject);
	const QString alignmentName = pythonText(alignmentObject);
	if (PyErr_Occurred())
		return nullptr;
	const QStringList choices { QStringLiteral("left"), QStringLiteral("center"),
		QStringLiteral("right"), QStringLiteral("justify") };
	const int alignment = choices.indexOf(alignmentName);
	if (alignment < 0)
	{
		PyErr_SetString(PyExc_ValueError, "alignment must be left, center, right, or justify");
		return nullptr;
	}
	PageItem* item = GetUniqueImageItem(frameName);
	if (!item)
		return nullptr;
	if (!EpubDocument::setSavedImageCaptionAlignment(item, alignment))
	{
		PyErr_SetString(PyExc_ValueError, "A normal-page image frame with a saved EPUB caption is required");
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject* scribus_setepubimagewidthpercent(PyObject* /*self*/, PyObject* args)
{
	PyObject* frameNameObject = nullptr;
	PyObject* widthObject = nullptr;
	if (!PyArg_ParseTuple(args, "OO", &frameNameObject, &widthObject))
		return nullptr;
	if (!PyUnicode_Check(frameNameObject) || !PyLong_Check(widthObject) || PyBool_Check(widthObject))
	{
		PyErr_SetString(PyExc_TypeError, "frameName must be a string and widthPercent an integer");
		return nullptr;
	}
	const long widthPercent = PyLong_AsLong(widthObject);
	if (PyErr_Occurred())
		return nullptr;
	if (widthPercent < 0 || widthPercent > 100)
	{
		PyErr_SetString(PyExc_ValueError, "widthPercent must be 0 (Auto) or between 1 and 100");
		return nullptr;
	}
	if (!checkHaveDocument())
		return nullptr;
	const QString frameName = pythonText(frameNameObject);
	if (PyErr_Occurred())
		return nullptr;
	PageItem* item = GetUniqueImageItem(frameName);
	if (!item)
		return nullptr;
	if (!EpubDocument::setSavedImageWidthPercent(item, static_cast<int>(widthPercent)))
	{
		PyErr_SetString(PyExc_ValueError, "A normal-page image frame is required");
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject* scribus_setepubuseimageframecrop(PyObject* /*self*/, PyObject* args)
{
	PyObject* frameNameObject = nullptr;
	PyObject* enabledObject = nullptr;
	if (!PyArg_ParseTuple(args, "OO", &frameNameObject, &enabledObject))
		return nullptr;
	if (!PyUnicode_Check(frameNameObject) || !PyBool_Check(enabledObject))
	{
		PyErr_SetString(PyExc_TypeError, "frameName must be a string and enabled a bool");
		return nullptr;
	}
	if (!checkHaveDocument())
		return nullptr;
	const QString frameName = pythonText(frameNameObject);
	if (PyErr_Occurred())
		return nullptr;
	PageItem* item = GetUniqueImageItem(frameName);
	if (!item)
		return nullptr;
	if (!EpubDocument::setSavedUseImageFrameCrop(item, enabledObject == Py_True))
	{
		PyErr_SetString(PyExc_ValueError, "A normal-page image frame is required");
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject* scribus_setepubimagereadingorder(PyObject* /*self*/, PyObject* args)
{
	PyObject* frameNameObject = nullptr;
	PyObject* rankObject = nullptr;
	if (!PyArg_ParseTuple(args, "OO", &frameNameObject, &rankObject))
		return nullptr;
	if (!PyUnicode_Check(frameNameObject) || !PyLong_Check(rankObject) || PyBool_Check(rankObject))
	{
		PyErr_SetString(PyExc_TypeError, "frameName must be a string and rank an integer");
		return nullptr;
	}
	const long rank = PyLong_AsLong(rankObject);
	if (PyErr_Occurred())
		return nullptr;
	if (rank < 0 || rank > std::numeric_limits<int>::max())
	{
		PyErr_SetString(PyExc_ValueError, "rank must be zero or a positive integer");
		return nullptr;
	}
	if (!checkHaveDocument())
		return nullptr;
	const QString frameName = pythonText(frameNameObject);
	if (PyErr_Occurred())
		return nullptr;
	PageItem* item = GetUniqueImageItem(frameName);
	if (!item)
		return nullptr;
	if (!EpubDocument::canRankImageFrame(item) ||
		!EpubDocument::setSavedOrder(item, static_cast<int>(rank)))
	{
		PyErr_SetString(PyExc_ValueError, "A printable normal-page independent image frame is required");
		return nullptr;
	}
	Py_RETURN_NONE;
}

PyObject* scribus_epubreadingorderpreflight(PyObject* /*self*/, PyObject* args)
{
	PyObject* styleMappings = nullptr;
	PyObject* characterMappings = nullptr;
	if (!PyArg_ParseTuple(args, "|OO", &styleMappings, &characterMappings))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	EpubReadingOrder::ParagraphStyleMap styles;
	EpubReadingOrder::CharacterStyleMap characterStyles;
	if (!parseMappings(styleMappings, characterMappings, styles, characterStyles))
		return nullptr;
	const ScribusDoc* document = ScCore->primaryMainWindow()->doc;
	const EpubDocument::Result extracted = EpubDocument::extractSavedOrder(*document, {}, styles, characterStyles);
	PyObject* result = PyDict_New();
	PyObject* status = PyUnicode_FromString(statusName(extracted.status));
	PyObject* detail = unicode(extracted.detail);
	PyObject* paragraphs = PyList_New(extracted.book.blocks.size());
	PyObject* headingLevels = PyList_New(extracted.book.blocks.size());
	PyObject* runs = PyList_New(extracted.book.blocks.size());
	PyObject* blockKinds = PyList_New(extracted.book.blocks.size());
	PyObject* listStarts = PyList_New(extracted.book.blocks.size());
	PyObject* listLevels = PyList_New(extracted.book.blocks.size());
	PyObject* listStartNumbers = PyList_New(extracted.book.blocks.size());
	PyObject* listTypes = PyList_New(extracted.book.blocks.size());
	PyObject* directions = PyList_New(extracted.book.blocks.size());
	PyObject* alignments = PyList_New(extracted.book.blocks.size());
	if (!result || !status || !detail || !paragraphs || !headingLevels || !runs || !blockKinds || !listStarts || !listLevels || !listStartNumbers || !listTypes || !directions || !alignments)
	{
		Py_XDECREF(result);
		Py_XDECREF(status);
		Py_XDECREF(detail);
		Py_XDECREF(paragraphs);
		Py_XDECREF(headingLevels);
		Py_XDECREF(runs);
		Py_XDECREF(blockKinds);
		Py_XDECREF(listStarts);
		Py_XDECREF(listLevels);
		Py_XDECREF(listStartNumbers);
		Py_XDECREF(listTypes);
		Py_XDECREF(directions);
		Py_XDECREF(alignments);
		return nullptr;
	}
	for (Py_ssize_t index = 0; index < extracted.book.blocks.size(); ++index)
	{
		PyObject* value = unicode(extracted.book.blocks.at(index).text);
		PyObject* level = PyLong_FromLong(extracted.book.blocks.at(index).headingLevel);
		PyObject* blockRuns = inlineRuns(extracted.book.blocks.at(index));
		PyObject* kind = PyUnicode_FromString(blockKindName(extracted.book.blocks.at(index).kind));
		PyObject* starts = PyBool_FromLong(extracted.book.blocks.at(index).startsList);
		PyObject* listLevel = PyLong_FromLong(extracted.book.blocks.at(index).listLevel);
		PyObject* firstNumber = PyLong_FromLong(extracted.book.blocks.at(index).kind == EpubExport::BlockKind::OrderedItem &&
			extracted.book.blocks.at(index).startsList
			? extracted.book.blocks.at(index).listStart : 0);
		const auto& block = extracted.book.blocks.at(index);
		const char* listType = block.kind != EpubExport::BlockKind::OrderedItem || !block.startsList ? ""
			: block.orderedStyle == EpubExport::OrderedStyle::LowerRoman ? "i"
			: block.orderedStyle == EpubExport::OrderedStyle::UpperRoman ? "I"
			: block.orderedStyle == EpubExport::OrderedStyle::LowerAlpha ? "a"
			: block.orderedStyle == EpubExport::OrderedStyle::UpperAlpha ? "A" : "1";
		PyObject* type = PyUnicode_FromString(listType);
		PyObject* direction = PyUnicode_FromString(block.direction == EpubExport::TextDirection::Rtl ? "rtl" : "ltr");
		PyObject* alignment = PyUnicode_FromString(alignmentName(block.alignment));
		if (!value || !level || !blockRuns || !kind || !starts || !listLevel || !firstNumber || !type || !direction || !alignment)
		{
			Py_XDECREF(value);
			Py_XDECREF(level);
			Py_XDECREF(blockRuns);
			Py_XDECREF(kind);
			Py_XDECREF(starts);
			Py_XDECREF(listLevel);
			Py_XDECREF(firstNumber);
			Py_XDECREF(type);
			Py_XDECREF(direction);
			Py_XDECREF(alignment);
			Py_DECREF(result);
			Py_DECREF(status);
			Py_DECREF(detail);
			Py_DECREF(paragraphs);
			Py_DECREF(headingLevels);
			Py_DECREF(runs);
			Py_DECREF(blockKinds);
			Py_DECREF(listStarts);
			Py_DECREF(listLevels);
			Py_DECREF(listStartNumbers);
			Py_DECREF(listTypes);
			Py_DECREF(directions);
			Py_DECREF(alignments);
			return nullptr;
		}
		PyList_SET_ITEM(paragraphs, index, value);
		PyList_SET_ITEM(headingLevels, index, level);
		PyList_SET_ITEM(runs, index, blockRuns);
		PyList_SET_ITEM(blockKinds, index, kind);
		PyList_SET_ITEM(listStarts, index, starts);
		PyList_SET_ITEM(listLevels, index, listLevel);
		PyList_SET_ITEM(listStartNumbers, index, firstNumber);
		PyList_SET_ITEM(listTypes, index, type);
		PyList_SET_ITEM(directions, index, direction);
		PyList_SET_ITEM(alignments, index, alignment);
	}
	const bool ok = PyDict_SetItemString(result, "status", status) == 0 &&
		PyDict_SetItemString(result, "detail", detail) == 0 &&
		PyDict_SetItemString(result, "paragraphs", paragraphs) == 0 &&
		PyDict_SetItemString(result, "headingLevels", headingLevels) == 0 &&
		PyDict_SetItemString(result, "inlineRuns", runs) == 0 &&
		PyDict_SetItemString(result, "blockKinds", blockKinds) == 0 &&
		PyDict_SetItemString(result, "listStarts", listStarts) == 0 &&
		PyDict_SetItemString(result, "listLevels", listLevels) == 0 &&
		PyDict_SetItemString(result, "listStartNumbers", listStartNumbers) == 0 &&
		PyDict_SetItemString(result, "listTypes", listTypes) == 0 &&
		PyDict_SetItemString(result, "directions", directions) == 0 &&
		PyDict_SetItemString(result, "alignments", alignments) == 0;
	Py_DECREF(status);
	Py_DECREF(detail);
	Py_DECREF(paragraphs);
	Py_DECREF(headingLevels);
	Py_DECREF(runs);
	Py_DECREF(blockKinds);
	Py_DECREF(listStarts);
	Py_DECREF(listLevels);
	Py_DECREF(listStartNumbers);
	Py_DECREF(listTypes);
	Py_DECREF(directions);
	Py_DECREF(alignments);
	if (!ok)
	{
		Py_DECREF(result);
		return nullptr;
	}
	if (!addLineSpacingWarnings(result, *document))
	{
		Py_DECREF(result);
		return nullptr;
	}
	return result;
}

PyObject* scribus_exportepubtextonly(PyObject* /*self*/, PyObject* args)
{
	PyObject* pathObject = nullptr;
	PyObject* identifierObject = nullptr;
	PyObject* titleObject = nullptr;
	PyObject* languageObject = nullptr;
	PyObject* styleMappings = nullptr;
	PyObject* characterMappings = nullptr;
	PyObject* authorObject = nullptr;
	if (!PyArg_ParseTuple(args, "UUUU|OOO", &pathObject, &identifierObject, &titleObject,
		&languageObject, &styleMappings, &characterMappings, &authorObject))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (authorObject && !PyUnicode_Check(authorObject))
	{
		PyErr_SetString(PyExc_TypeError, "author must be a string");
		return nullptr;
	}
	EpubReadingOrder::ParagraphStyleMap styles;
	EpubReadingOrder::CharacterStyleMap characterStyles;
	if (!parseMappings(styleMappings, characterMappings, styles, characterStyles))
		return nullptr;
	const QString path = pythonText(pathObject);
	EpubExport::Book metadata {
		pythonText(identifierObject), pythonText(titleObject), pythonText(languageObject),
		authorObject ? pythonText(authorObject) : QString(), {}
	};
	if (PyErr_Occurred())
		return nullptr;
	if (path.isEmpty() || path.contains(QChar::Null) || !QDir::isAbsolutePath(path))
		return statusResult("invalid-input", QStringLiteral("EPUB output must be an absolute, non-empty path."));
	const ScribusDoc* document = ScCore->primaryMainWindow()->doc;
	const EpubDocument::Result extracted = EpubDocument::extractSavedOrder(*document, metadata, styles, characterStyles);
	if (!extracted.ready())
		return statusResult(statusName(extracted.status), extracted.detail);
	const EpubExport::Result written = EpubExport::writeBook(extracted.book, path);
	const QStringList leadingWarnings = written.exported() ? EpubDocument::lineSpacingWarnings(*document) : QStringList {};
	if (!leadingWarnings.isEmpty())
		return statusResult("exported", leadingWarnings.join(QLatin1Char(' ')));
	return statusResult(exportStatusName(written.status), written.detail);
}

PyObject* scribus_exportepubtextandimages(PyObject* /*self*/, PyObject* args)
{
	PyObject* pathObject = nullptr;
	PyObject* identifierObject = nullptr;
	PyObject* titleObject = nullptr;
	PyObject* languageObject = nullptr;
	PyObject* styleMappings = nullptr;
	PyObject* characterMappings = nullptr;
	PyObject* authorObject = nullptr;
	if (!PyArg_ParseTuple(args, "UUUU|OOO", &pathObject, &identifierObject, &titleObject,
		&languageObject, &styleMappings, &characterMappings, &authorObject))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	if (authorObject && !PyUnicode_Check(authorObject))
	{
		PyErr_SetString(PyExc_TypeError, "author must be a string");
		return nullptr;
	}
	EpubReadingOrder::ParagraphStyleMap styles;
	EpubReadingOrder::CharacterStyleMap characterStyles;
	if (!parseMappings(styleMappings, characterMappings, styles, characterStyles))
		return nullptr;
	const QString path = pythonText(pathObject);
	EpubExport::Book metadata {
		pythonText(identifierObject), pythonText(titleObject), pythonText(languageObject),
		authorObject ? pythonText(authorObject) : QString(), {}
	};
	if (PyErr_Occurred())
		return nullptr;
	if (path.isEmpty() || path.contains(QChar::Null) || !QDir::isAbsolutePath(path))
		return statusResult("invalid-input", QStringLiteral("EPUB output must be an absolute, non-empty path."));
	const ScribusDoc* document = ScCore->primaryMainWindow()->doc;
	const EpubDocument::Result extracted = EpubDocument::extractMixedSavedOrder(*document, metadata, styles, characterStyles);
	if (!extracted.ready())
		return statusResult(statusName(extracted.status), extracted.detail);
	const EpubExport::Result written = EpubExport::writeBook(extracted.book, path);
	const QStringList leadingWarnings = written.exported() ? EpubDocument::lineSpacingWarnings(*document) : QStringList {};
	if (written.status == EpubExport::Status::Exported && !extracted.book.images.isEmpty())
	{
		bool cropped = false;
		for (const PageItem* item : EpubDocument::rankableItems(*document))
			cropped |= item->isImageFrame() && EpubDocument::savedUseImageFrameCrop(item);
		QString detail = cropped
			? QStringLiteral("Selected exact PNG frame crops were applied; other linked images use original pixels. Page placement, effects, and colour adjustments are not preserved.")
			: QStringLiteral("Linked images use original file pixels; Apscribe frame crop, scale, geometry, and colour adjustments are not preserved.");
		if (!leadingWarnings.isEmpty())
			detail += QLatin1Char(' ') + leadingWarnings.join(QLatin1Char(' '));
		return statusResult("exported", detail);
	}
	if (!leadingWarnings.isEmpty())
		return statusResult("exported", leadingWarnings.join(QLatin1Char(' ')));
	return statusResult(exportStatusName(written.status), written.detail);
}
