/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef EPUBEXPORT_H
#define EPUBEXPORT_H

#include "scribusapi.h"

#include <QByteArray>
#include <QString>
#include <QVector>

namespace EpubExport
{

enum class BlockKind { Paragraph, Heading, BulletItem, OrderedItem, Image };
enum class InlineKind { Plain, Emphasis, Strong };
enum class OrderedStyle { Decimal, LowerRoman, UpperRoman, LowerAlpha, UpperAlpha };
enum class TextDirection { Ltr, Rtl };
enum class TextAlignment { Left, Center, Right, Justify };

struct ParagraphMetrics
{
	double leftIndent { 0.0 };
	double rightIndent { 0.0 };
	double firstLineIndent { 0.0 };
	double spaceBefore { 0.0 };
	double spaceAfter { 0.0 };

	bool isDefault() const
	{
		return leftIndent == 0.0 && rightIndent == 0.0 && firstLineIndent == 0.0 &&
			spaceBefore == 0.0 && spaceAfter == 0.0;
	}
};

struct InlineRun
{
	InlineKind kind { InlineKind::Plain };
	QString text;
};

struct Block
{
	BlockKind kind { BlockKind::Paragraph };
	QString text;
	int headingLevel { 0 }; // 1-6 for headings; zero for paragraphs.
	QVector<InlineRun> runs; // Empty for legacy plain-text blocks; otherwise concatenates to text.
	bool startsList { false }; // A new flat list begins at this item.
	int listStart { 1 }; // First number of a local ordered list.
	OrderedStyle orderedStyle { OrderedStyle::Decimal };
	TextDirection direction { TextDirection::Ltr };
	TextAlignment alignment { TextAlignment::Left };
	int imageIndex { -1 }; // Zero-based index in Book::images for image blocks only.
	QString caption; // Optional visible figure caption; independent of image alt text.
	int listLevel { 0 }; // Standard bullets may nest one level; other blocks remain level zero.
	ParagraphMetrics metrics; // Scribus paragraph distances in points; text blocks only.
	bool decorative { false }; // Explicitly decorative image; requires empty alt and no caption.
	int imageWidthPercent { 0 }; // 0 = reader default; 1-100 = width of the reading area.
	TextAlignment captionAlignment { TextAlignment::Left }; // Applies only to a visible image caption.
};

struct ImageAsset
{
	QByteArray data;
	QString mediaType; // image/png or image/jpeg; bytes are validated before packaging.
};

// The caller supplies an explicit reading order and image assets. The writer
// does not infer placement from positioned Scribus frames or alter PDF export.
struct Book
{
	QString identifier;
	QString title;
	QString language;
	QString author;
	QVector<Block> blocks;
	QVector<ImageAsset> images;
};

enum class Status { Exported, InvalidInput, OutputExists, IoError, ArchiveError };

struct Result
{
	Status status { Status::InvalidInput };
	QString detail;

	bool exported() const { return status == Status::Exported; }
};

// Writes a reflowable EPUB 3.3 package. Existing output is never
// overwritten; an incomplete archive is confined to a temporary directory.
SCRIBUS_API Result writeBook(const Book& book, const QString& outputPath);

} // namespace EpubExport

#endif
