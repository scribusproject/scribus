/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef EPUBDOCUMENT_H
#define EPUBDOCUMENT_H

#include "epubreadingorder.h"
#include "scribusapi.h"

#include <QStringList>

class ScribusDoc;
class PageItem;

namespace EpubDocument
{

// Read-only adapter. The caller provides validated publication metadata;
// this does not register an export command or alter document layout.
struct Result
{
	EpubReadingOrder::Status status { EpubReadingOrder::Status::Empty };
	QString detail;
	EpubExport::Book book;

	bool ready() const { return status == EpubReadingOrder::Status::Ready; }
};

enum class PreflightSeverity { Error, Warning };

struct PreflightIssue
{
	PreflightSeverity severity { PreflightSeverity::Error };
	QString code;
	QString detail;
};

struct PreflightReport
{
	Result extraction;
	QVector<PreflightIssue> issues;

	bool ready() const
	{
		if (!extraction.ready())
			return false;
		for (const PreflightIssue& issue : issues)
		{
			if (issue.severity == PreflightSeverity::Error)
				return false;
		}
		return true;
	}
};

struct StyleNames
{
	QStringList paragraph;
	QStringList character;
};

// Read-only conversion of one printable linked image frame. The caller
// supplies descriptive alt text and still decides its position in the book.
// Only explicitly selected exact PNG frame crops are applied; colour and
// other page-layout transforms are not reproduced.
struct ImageFrameResult
{
	EpubReadingOrder::Status status { EpubReadingOrder::Status::UnsupportedContent };
	QString detail;
	EpubExport::Block block;
	EpubExport::ImageAsset asset;
	bool cropped { false }; // True when an explicit exact-pixel frame crop was applied.

	bool ready() const { return status == EpubReadingOrder::Status::Ready; }
};

SCRIBUS_API ImageFrameResult extractLinkedImageFrame(const PageItem* item, const QString& altText);
SCRIBUS_API ImageFrameResult extractLinkedImageFrame(const PageItem* item);

// Stored in the existing SLA object-attribute format. Empty text clears it;
// non-empty text must be valid XML and is not inferred from the file name.
SCRIBUS_API QString savedImageAltText(const PageItem* item);
SCRIBUS_API bool setSavedImageAltText(PageItem* item, const QString& altText);
// An explicit, saved accessibility choice. Decorative images require empty
// description and caption; ordinary images still require valid alt text.
SCRIBUS_API bool savedImageDecorative(const PageItem* item);
SCRIBUS_API bool setSavedImageDecorative(PageItem* item, bool decorative);
// Optional visible caption, stored separately from required accessibility alt text.
SCRIBUS_API QString savedImageCaption(const PageItem* item);
SCRIBUS_API bool setSavedImageCaption(PageItem* item, const QString& caption);
// Caption-only alignment: 0 left (default), 1 center, 2 right, 3 justify.
// -1 indicates malformed saved metadata and blocks export.
SCRIBUS_API int savedImageCaptionAlignment(const PageItem* item);
SCRIBUS_API bool setSavedImageCaptionAlignment(PageItem* item, int alignment);
// Reflowable figure width, independent of Scribus frame geometry. Zero
// removes the override; malformed stored values fail image preflight.
SCRIBUS_API int savedImageWidthPercent(const PageItem* item);
SCRIBUS_API bool setSavedImageWidthPercent(PageItem* item, int widthPercent);
// Opt-in, exact-pixel crop of a plain rectangular linked PNG frame.
SCRIBUS_API bool savedUseImageFrameCrop(const PageItem* item);
SCRIBUS_API bool setSavedUseImageFrameCrop(PageItem* item, bool enabled);

Result extractSingleStory(const ScribusDoc& document, EpubExport::Book metadata,
	const EpubReadingOrder::ParagraphStyleMap& styles = {},
	const EpubReadingOrder::CharacterStyleMap& characterStyles = {});

// orderedStoryRoots is an explicit, immediate-use list of normal-page text
// frames that start independent chains. It must include each story once.
Result extractStories(const ScribusDoc& document, EpubExport::Book metadata,
	const QVector<const PageItem*>& orderedStoryRoots,
	const EpubReadingOrder::ParagraphStyleMap& styles = {},
	const EpubReadingOrder::CharacterStyleMap& characterStyles = {});

// Reads 1-based ranks from the story-root attributes already persisted in SLA.
SCRIBUS_API Result extractSavedOrder(const ScribusDoc& document, EpubExport::Book metadata,
	const EpubReadingOrder::ParagraphStyleMap& styles = {},
	const EpubReadingOrder::CharacterStyleMap& characterStyles = {});

// Read-only document adapter for the shared text-story/image rank sequence.
// It requires linked PNG/JPEG images with saved alt text, unless explicitly decorative.
SCRIBUS_API Result extractMixedSavedOrder(const ScribusDoc& document, EpubExport::Book metadata,
	const EpubReadingOrder::ParagraphStyleMap& styles = {},
	const EpubReadingOrder::CharacterStyleMap& characterStyles = {});

// Independent metadata, destination, rank, and image checks accompany the
// extractor's document error. Read-only probes may omit destination checks;
// entirely empty metadata omits publication-field checks.
SCRIBUS_API PreflightReport preflightMixedSavedOrder(const ScribusDoc& document,
	EpubExport::Book metadata = {}, const QString& outputPath = {}, bool validateDestination = false,
	const EpubReadingOrder::ParagraphStyleMap& styles = {},
	const EpubReadingOrder::CharacterStyleMap& characterStyles = {});

// Named styles actually referenced by printable text-story roots, in a
// stable display order. No EPUB semantics are inferred from the names.
SCRIBUS_API StyleNames usedStyleNames(const ScribusDoc& document);

// Fixed Scribus leading is deliberately not forced onto reflowable reader text.
// Returns a concise warning when any printable text story uses it.
SCRIBUS_API QStringList lineSpacingWarnings(const ScribusDoc& document);

// Zero removes the rank; positive ranks are stored on text-chain roots or
// independent printable image frames. This preserves unrelated attributes.
SCRIBUS_API int savedOrder(const PageItem* root);
bool canRankStoryRoot(const PageItem* root);
SCRIBUS_API bool canRankImageFrame(const PageItem* item);
SCRIBUS_API bool setSavedOrder(PageItem* root, int rank);

// The legacy text-only assignment must not renumber stories while image ranks
// are present.
bool hasRankedImageOrder(const ScribusDoc& document);

// List eligible normal-page chain roots and atomically validate a complete
// reordered list before writing ranks. Unrelated object attributes survive.
QVector<PageItem*> storyRoots(ScribusDoc& document);
bool assignSavedOrder(ScribusDoc& document, const QVector<PageItem*>& orderedRoots);

// Lists rankable text-chain roots and independent images. Assignment verifies
// the exact current set before changing any rank, then writes one shared
// contiguous sequence while preserving unrelated object attributes.
SCRIBUS_API QVector<PageItem*> rankableItems(const ScribusDoc& document);
bool assignMixedSavedOrder(ScribusDoc& document, const QVector<PageItem*>& orderedItems);

} // namespace EpubDocument

#endif
