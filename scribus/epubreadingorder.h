/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef EPUBREADINGORDER_H
#define EPUBREADINGORDER_H

#include "epubexport.h"

#include <QHash>

namespace EpubReadingOrder
{

// A frame is a view of a story, not a separate copy of its text. The story
// belongs to the chain root; links provide the only inferred reading order.
struct Frame
{
	int id { -1 };
	int previousId { -1 };
	int nextId { -1 };
	QString rootText;
	bool unsupportedContent { false };
	int savedOrder { -1 }; // -1 = unset; other non-positive values are invalid.
	QVector<QString> paragraphStyleNames; // One name per paragraph; empty name = default body.
	QString unsupportedDetail;
	QVector<QString> characterStyleNames; // One name per UTF-16 unit in rootText.
	QVector<bool> bulletParagraphs; // One flag per paragraph; standard bullets support one nested level.
	QVector<int> orderedParagraphStarts; // Positive start for local flat lists; zero otherwise.
	QVector<EpubExport::OrderedStyle> orderedParagraphStyles; // One style per paragraph when present.
	QVector<EpubExport::TextDirection> paragraphDirections; // One base direction per paragraph when present.
	QVector<EpubExport::TextAlignment> paragraphAlignments; // One supported physical alignment per paragraph.
	QVector<int> bulletParagraphLevels; // Zero for non-bullets/outer bullets; one for nested standard bullets.
	QVector<EpubExport::ParagraphMetrics> paragraphMetrics; // One resolved Scribus style per paragraph.
};

// Explicit semantic assignments: 0 = paragraph, 1-6 = heading level.
// Unknown named styles fail extraction; no style-name guessing is performed.
using ParagraphStyleMap = QHash<QString, int>;
using CharacterStyleMap = QHash<QString, EpubExport::InlineKind>;

enum class Status { Ready, Empty, Ambiguous, InvalidChain, InvalidOrder, UnsupportedContent };

struct Result
{
	Status status { Status::Empty };
	QString detail;
	QVector<EpubExport::Block> blocks;
	QVector<EpubExport::ImageAsset> images;

	bool ready() const { return status == Status::Ready; }
};

// One image frame is one reading-order unit; its rank places it between
// complete text stories, never inside a paragraph or linked story.
struct Image
{
	int id { -1 };
	int savedOrder { -1 };
	QString altText;
	EpubExport::ImageAsset asset;
	EpubExport::TextDirection direction { EpubExport::TextDirection::Ltr };
	EpubExport::TextAlignment alignment { EpubExport::TextAlignment::Left };
	QString caption; // Optional visible caption, separate from required alt text.
	bool decorative { false }; // Explicit choice; empty alt is otherwise invalid.
	int widthPercent { 0 }; // 0 = automatic; 1-100 = explicit reflowable figure width.
	EpubExport::TextAlignment captionAlignment { EpubExport::TextAlignment::Left };
};

enum class ContentKind { Group, TextFrame, ImageFrame, Unsupported };

// Neutral hierarchy snapshot. Node indices are local to one scan; the caller
// maps them back to document items after validation.
struct ContentNode
{
	ContentKind kind { ContentKind::Unsupported };
	bool included { true }; // false for master or non-printing subtrees.
	QVector<int> children;
};

struct ContentScan
{
	Status status { Status::Ready };
	QString detail;
	QVector<int> textFrameIndices;
	QVector<int> imageFrameIndices;
	QVector<int> unsupportedIndices;

	bool valid() const { return status == Status::Ready; }
};

// Traverses groups without using their child-list order as ebook order.
// Repeated references are visited once; cycles and bad references fail.
ContentScan scanContent(const QVector<ContentNode>& nodes, const QVector<int>& topLevelIndices);

// With multiple independent chains, orderedRootIds must contain every chain
// root exactly once. No geometric or z-order inference is made.
Result extractStories(const QVector<Frame>& frames, const QVector<int>& orderedRootIds,
	const ParagraphStyleMap& styles = {}, const CharacterStyleMap& characterStyles = {});

// Uses 1-based saved ranks on every chain root. An unranked single story is
// allowed; multi-story documents require complete, unique, contiguous ranks.
Result extractSavedOrder(const QVector<Frame>& frames, const ParagraphStyleMap& styles = {},
	const CharacterStyleMap& characterStyles = {});

// Validates a single contiguous rank sequence across text-chain roots and
// independent image frames. A sole unranked unit is allowed. Every returned
// image block indexes Result::images; callers may copy both into a Book.
Result extractMixedSavedOrder(const QVector<Frame>& frames, const QVector<Image>& images,
	const ParagraphStyleMap& styles = {}, const CharacterStyleMap& characterStyles = {});

// Convenience path for documents containing exactly one linked story.
Result extractSingleStory(const QVector<Frame>& frames, const ParagraphStyleMap& styles = {},
	const CharacterStyleMap& characterStyles = {});

} // namespace EpubReadingOrder

#endif
