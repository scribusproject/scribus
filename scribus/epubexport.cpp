/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "epubexport.h"

#include "third_party/zip/zip.h"

#include <QDateTime>
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImageReader>
#include <QRegularExpression>
#include <QTemporaryDir>
#include <QXmlStreamWriter>

#include <cmath>

namespace EpubExport
{
namespace
{
constexpr auto xhtmlNamespace = "http://www.w3.org/1999/xhtml";
constexpr auto epubNamespace = "http://www.idpf.org/2007/ops";
constexpr auto opfNamespace = "http://www.idpf.org/2007/opf";
constexpr auto dcNamespace = "http://purl.org/dc/elements/1.1/";
constexpr auto xmlNamespace = "http://www.w3.org/XML/1998/namespace";

bool validXmlText(const QString& value)
{
	for (qsizetype i = 0; i < value.size(); ++i)
	{
		const QChar ch = value.at(i);
		if (ch.isHighSurrogate())
		{
			if (i + 1 >= value.size() || !value.at(i + 1).isLowSurrogate())
				return false;
			++i;
			continue;
		}
		const ushort unit = ch.unicode();
		if (ch.isLowSurrogate() || (unit < 0x20 && unit != 0x09 && unit != 0x0a && unit != 0x0d) ||
			unit == 0xfffe || unit == 0xffff)
			return false;
	}
	return true;
}

QString imageFileName(qsizetype index, const ImageAsset& image)
{
	return QStringLiteral("image-%1.%2").arg(index + 1).arg(image.mediaType == QLatin1String("image/png")
		? QStringLiteral("png") : QStringLiteral("jpg"));
}

bool validImage(const ImageAsset& image)
{
	if (image.data.isEmpty() || (image.mediaType != QLatin1String("image/png") &&
		image.mediaType != QLatin1String("image/jpeg")))
		return false;
	QBuffer buffer;
	buffer.setData(image.data);
	if (!buffer.open(QIODevice::ReadOnly))
		return false;
	QImageReader reader(&buffer);
	if (!reader.canRead())
		return false;
	const QByteArray format = reader.format().toLower();
	if (image.mediaType == QLatin1String("image/png") && format != QByteArrayLiteral("png"))
		return false;
	if (image.mediaType == QLatin1String("image/jpeg") &&
		format != QByteArrayLiteral("jpeg") && format != QByteArrayLiteral("jpg"))
		return false;
	return !reader.read().isNull();
}

bool validParagraphMetrics(const Block& block)
{
	const auto validDistance = [](double value) { return std::isfinite(value) && std::abs(value) <= 10000.0; };
	const ParagraphMetrics& metrics = block.metrics;
	return validDistance(metrics.leftIndent) && validDistance(metrics.rightIndent) &&
		validDistance(metrics.firstLineIndent) && validDistance(metrics.spaceBefore) &&
		validDistance(metrics.spaceAfter) &&
		((block.kind == BlockKind::Paragraph || block.kind == BlockKind::Heading) || metrics.isDefault());
}

bool validBook(const Book& book)
{
	static const QRegularExpression languagePattern(QStringLiteral("^[A-Za-z]{2,8}(?:-[A-Za-z0-9]{1,8})*$"));
	if (book.identifier.trimmed().isEmpty() || book.title.trimmed().isEmpty() ||
		!languagePattern.match(book.language).hasMatch() || book.blocks.isEmpty())
		return false;
	if (!validXmlText(book.identifier) || !validXmlText(book.title) ||
		!validXmlText(book.language) || !validXmlText(book.author))
		return false;
	bool hasContent = false;
	QVector<bool> referencedImages(book.images.size(), false);
	BlockKind previousListKind = BlockKind::Paragraph;
	int previousListLevel = 0;
	int previousListStart = 1;
	OrderedStyle previousOrderedStyle = OrderedStyle::Decimal;
	int orderedRunLength = 0;
	for (const Block& block : book.blocks)
	{
		if (!validXmlText(block.text) || !validXmlText(block.caption) || !validParagraphMetrics(block) ||
			(block.direction != TextDirection::Ltr && block.direction != TextDirection::Rtl) ||
			(block.alignment != TextAlignment::Left && block.alignment != TextAlignment::Center &&
				block.alignment != TextAlignment::Right && block.alignment != TextAlignment::Justify) ||
			(block.kind == BlockKind::Heading && (block.headingLevel < 1 || block.headingLevel > 6 || block.text.trimmed().isEmpty())) ||
			(block.kind == BlockKind::Paragraph && block.headingLevel != 0) ||
			(block.kind == BlockKind::BulletItem && (block.headingLevel != 0 || block.text.trimmed().isEmpty())) ||
			(block.kind == BlockKind::OrderedItem && (block.headingLevel != 0 || block.text.trimmed().isEmpty())) ||
			(block.kind == BlockKind::Image && (block.headingLevel != 0 ||
				(!block.decorative && block.text.trimmed().isEmpty()) ||
				(block.decorative && (!block.text.isEmpty() || !block.caption.isEmpty())) ||
				!block.runs.isEmpty() || block.imageIndex < 0 || block.imageIndex >= book.images.size())) ||
			(block.kind != BlockKind::Paragraph && block.kind != BlockKind::Heading &&
				block.kind != BlockKind::BulletItem && block.kind != BlockKind::OrderedItem &&
				block.kind != BlockKind::Image) ||
			(block.kind != BlockKind::Image && block.imageIndex != -1) ||
			(block.kind != BlockKind::Image && block.decorative) ||
			(block.imageWidthPercent < 0 || block.imageWidthPercent > 100) ||
			(block.kind != BlockKind::Image && block.imageWidthPercent != 0) ||
			(block.captionAlignment != TextAlignment::Left &&
				block.captionAlignment != TextAlignment::Center &&
				block.captionAlignment != TextAlignment::Right &&
				block.captionAlignment != TextAlignment::Justify) ||
			(block.caption.isEmpty() && block.captionAlignment != TextAlignment::Left) ||
			(block.kind != BlockKind::Image && !block.caption.isEmpty()) ||
			(!block.caption.isEmpty() && block.caption.trimmed().isEmpty()) ||
			(block.startsList && block.kind != BlockKind::BulletItem && block.kind != BlockKind::OrderedItem) ||
			(block.kind != BlockKind::BulletItem && block.listLevel != 0) ||
			(block.kind == BlockKind::BulletItem && (block.listLevel < 0 || block.listLevel > 1 ||
				(block.listLevel == 1 && (previousListKind != BlockKind::BulletItem ||
					block.startsList != (previousListLevel == 0))))) ||
			(block.kind != BlockKind::OrderedItem && (block.listStart != 1 || block.orderedStyle != OrderedStyle::Decimal)) ||
			block.listStart < 1 ||
			(block.orderedStyle != OrderedStyle::Decimal && block.orderedStyle != OrderedStyle::LowerRoman &&
				block.orderedStyle != OrderedStyle::UpperRoman && block.orderedStyle != OrderedStyle::LowerAlpha &&
				block.orderedStyle != OrderedStyle::UpperAlpha) ||
			(block.kind == BlockKind::OrderedItem && previousListKind == BlockKind::OrderedItem &&
				!block.startsList && (block.listStart != previousListStart || block.orderedStyle != previousOrderedStyle)))
			return false;
		if (block.kind == BlockKind::Image)
			referencedImages[block.imageIndex] = true;
		orderedRunLength = block.kind == BlockKind::OrderedItem
			? (previousListKind == BlockKind::OrderedItem && !block.startsList ? orderedRunLength + 1 : 1) : 0;
		if (block.kind == BlockKind::OrderedItem && block.orderedStyle != OrderedStyle::Decimal &&
			block.listStart > ((block.orderedStyle == OrderedStyle::LowerAlpha ||
				block.orderedStyle == OrderedStyle::UpperAlpha ? 27 : 4000) - orderedRunLength))
			return false;
		previousListKind = block.kind;
		previousListLevel = block.listLevel;
		previousListStart = block.listStart;
		previousOrderedStyle = block.orderedStyle;
		if (!block.runs.isEmpty())
		{
			QString combined;
			for (const InlineRun& run : block.runs)
			{
				if (run.text.isEmpty() || !validXmlText(run.text) ||
					(run.kind != InlineKind::Plain && run.kind != InlineKind::Emphasis && run.kind != InlineKind::Strong))
					return false;
				combined += run.text;
			}
			if (combined != block.text)
				return false;
		}
		hasContent |= block.kind == BlockKind::Image || !block.text.trimmed().isEmpty();
	}
	if (!hasContent)
		return false;
	for (qsizetype index = 0; index < book.images.size(); ++index)
	{
		if (!referencedImages.at(index) || !validImage(book.images.at(index)))
			return false;
	}
	return true;
}

bool writeFile(const QString& path, const QByteArray& bytes)
{
	QFile file(path);
	return file.open(QIODevice::WriteOnly) && file.write(bytes) == bytes.size() && file.flush();
}

QByteArray containerXml()
{
	QByteArray bytes;
	QXmlStreamWriter xml(&bytes);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement(QStringLiteral("container"));
	xml.writeDefaultNamespace(QStringLiteral("urn:oasis:names:tc:opendocument:xmlns:container"));
	xml.writeAttribute(QStringLiteral("version"), QStringLiteral("1.0"));
	xml.writeStartElement(QStringLiteral("rootfiles"));
	xml.writeEmptyElement(QStringLiteral("rootfile"));
	xml.writeAttribute(QStringLiteral("full-path"), QStringLiteral("OEBPS/content.opf"));
	xml.writeAttribute(QStringLiteral("media-type"), QStringLiteral("application/oebps-package+xml"));
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeEndDocument();
	return bytes;
}

struct Chapter
{
	qsizetype firstBlock { 0 };
	qsizetype endBlock { 0 };
	QString id;
	QString fileName;
};

QVector<Chapter> splitChapters(const Book& book)
{
	QVector<Chapter> chapters;
	qsizetype firstBlock = 0;
	bool seenChapterHeading = false;
	for (qsizetype index = 0; index < book.blocks.size(); ++index)
	{
		const Block& block = book.blocks.at(index);
		if (block.kind != BlockKind::Heading || block.headingLevel != 1)
			continue;
		if (seenChapterHeading)
			chapters.append({ firstBlock, index, {}, {} });
		firstBlock = seenChapterHeading ? index : firstBlock;
		seenChapterHeading = true;
	}
	chapters.append({ firstBlock, book.blocks.size(), {}, {} });
	for (qsizetype index = 0; index < chapters.size(); ++index)
	{
		chapters[index].id = index == 0 ? QStringLiteral("chapter")
			: QStringLiteral("chapter-%1").arg(index + 1);
		chapters[index].fileName = chapters.at(index).id + QStringLiteral(".xhtml");
	}
	return chapters;
}

QByteArray packageXml(const Book& book, const QVector<Chapter>& chapters)
{
	QByteArray bytes;
	QXmlStreamWriter xml(&bytes);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement(QStringLiteral("package"));
	xml.writeDefaultNamespace(QLatin1String(opfNamespace));
	xml.writeNamespace(QLatin1String(dcNamespace), QStringLiteral("dc"));
	xml.writeAttribute(QStringLiteral("version"), QStringLiteral("3.0"));
	xml.writeAttribute(QStringLiteral("unique-identifier"), QStringLiteral("pub-id"));
	xml.writeStartElement(QStringLiteral("metadata"));
	xml.writeStartElement(QLatin1String(dcNamespace), QStringLiteral("identifier"));
	xml.writeAttribute(QStringLiteral("id"), QStringLiteral("pub-id"));
	xml.writeCharacters(book.identifier);
	xml.writeEndElement();
	xml.writeTextElement(QLatin1String(dcNamespace), QStringLiteral("title"), book.title);
	xml.writeTextElement(QLatin1String(dcNamespace), QStringLiteral("language"), book.language);
	if (!book.author.trimmed().isEmpty())
		xml.writeTextElement(QLatin1String(dcNamespace), QStringLiteral("creator"), book.author);
	xml.writeStartElement(QStringLiteral("meta"));
	xml.writeAttribute(QStringLiteral("property"), QStringLiteral("dcterms:modified"));
	xml.writeCharacters(QDateTime::currentDateTimeUtc().toString(QStringLiteral("yyyy-MM-dd'T'HH:mm:ss'Z'")));
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeStartElement(QStringLiteral("manifest"));
	xml.writeEmptyElement(QStringLiteral("item"));
	xml.writeAttribute(QStringLiteral("id"), QStringLiteral("nav"));
	xml.writeAttribute(QStringLiteral("href"), QStringLiteral("nav.xhtml"));
	xml.writeAttribute(QStringLiteral("media-type"), QStringLiteral("application/xhtml+xml"));
	xml.writeAttribute(QStringLiteral("properties"), QStringLiteral("nav"));
	xml.writeEmptyElement(QStringLiteral("item"));
	xml.writeAttribute(QStringLiteral("id"), QStringLiteral("styles"));
	xml.writeAttribute(QStringLiteral("href"), QStringLiteral("styles.css"));
	xml.writeAttribute(QStringLiteral("media-type"), QStringLiteral("text/css"));
	for (const Chapter& chapter : chapters)
	{
		xml.writeEmptyElement(QStringLiteral("item"));
		xml.writeAttribute(QStringLiteral("id"), chapter.id);
		xml.writeAttribute(QStringLiteral("href"), chapter.fileName);
		xml.writeAttribute(QStringLiteral("media-type"), QStringLiteral("application/xhtml+xml"));
	}
	for (qsizetype index = 0; index < book.images.size(); ++index)
	{
		const ImageAsset& image = book.images.at(index);
		xml.writeEmptyElement(QStringLiteral("item"));
		xml.writeAttribute(QStringLiteral("id"), QStringLiteral("image-%1").arg(index + 1));
		xml.writeAttribute(QStringLiteral("href"), QStringLiteral("images/") + imageFileName(index, image));
		xml.writeAttribute(QStringLiteral("media-type"), image.mediaType);
	}
	xml.writeEndElement();
	xml.writeStartElement(QStringLiteral("spine"));
	for (const Chapter& chapter : chapters)
	{
		xml.writeEmptyElement(QStringLiteral("itemref"));
		xml.writeAttribute(QStringLiteral("idref"), chapter.id);
	}
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeEndDocument();
	return bytes;
}

QByteArray stylesheet()
{
	return QByteArrayLiteral(".scribus-align-left { text-align: left; }\n"
		".scribus-align-center { text-align: center; }\n"
		".scribus-align-right { text-align: right; }\n"
		".scribus-align-justify { text-align: justify; }\n"
		"figure img { max-width: 100%; height: auto; }\n");
}

QString alignmentClass(TextAlignment alignment)
{
	switch (alignment)
	{
		case TextAlignment::Left: return QStringLiteral("scribus-align-left");
		case TextAlignment::Center: return QStringLiteral("scribus-align-center");
		case TextAlignment::Right: return QStringLiteral("scribus-align-right");
		case TextAlignment::Justify: return QStringLiteral("scribus-align-justify");
	}
	return {};
}

QString paragraphMetricsStyle(const Block& block)
{
	const ParagraphMetrics& metrics = block.metrics;
	if (metrics.isDefault())
		return {};
	const auto pt = [](double value) { return QString::number(value, 'f', 6) + QStringLiteral("pt"); };
	const double physicalLeft = block.direction == TextDirection::Rtl ? metrics.rightIndent : metrics.leftIndent;
	const double physicalRight = block.direction == TextDirection::Rtl ? metrics.leftIndent : metrics.rightIndent;
	return QStringLiteral("margin-left:%1; margin-right:%2; text-indent:%3; margin-top:%4; margin-bottom:%5")
		.arg(pt(physicalLeft), pt(physicalRight), pt(metrics.firstLineIndent),
			pt(metrics.spaceBefore), pt(metrics.spaceAfter));
}

void beginXhtml(QXmlStreamWriter& xml, const Book& book, const QString& documentTitle, bool contentDocument)
{
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeStartElement(QStringLiteral("html"));
	xml.writeDefaultNamespace(QLatin1String(xhtmlNamespace));
	xml.writeNamespace(QLatin1String(epubNamespace), QStringLiteral("epub"));
	xml.writeAttribute(QStringLiteral("lang"), book.language);
	xml.writeAttribute(QLatin1String(xmlNamespace), QStringLiteral("lang"), book.language);
	xml.writeStartElement(QStringLiteral("head"));
	xml.writeEmptyElement(QStringLiteral("meta"));
	xml.writeAttribute(QStringLiteral("charset"), QStringLiteral("utf-8"));
	xml.writeTextElement(QStringLiteral("title"), documentTitle);
	if (contentDocument)
	{
		xml.writeEmptyElement(QStringLiteral("link"));
		xml.writeAttribute(QStringLiteral("rel"), QStringLiteral("stylesheet"));
		xml.writeAttribute(QStringLiteral("type"), QStringLiteral("text/css"));
		xml.writeAttribute(QStringLiteral("href"), QStringLiteral("styles.css"));
	}
	xml.writeEndElement();
	xml.writeStartElement(QStringLiteral("body"));
}

QByteArray chapterXml(const Book& book, const Chapter& chapter)
{
	QByteArray bytes;
	QXmlStreamWriter xml(&bytes);
	QString documentTitle = book.title;
	for (qsizetype i = chapter.firstBlock; i < chapter.endBlock; ++i)
	{
		if (book.blocks.at(i).kind == BlockKind::Heading)
		{
			documentTitle = book.blocks.at(i).text;
			break;
		}
	}
	beginXhtml(xml, book, documentTitle, true);
	// Pretty-printing can insert whitespace into mixed text and inline markup.
	xml.setAutoFormatting(false);
	xml.writeStartElement(QStringLiteral("main"));
	if (chapter.firstBlock == 0 && book.blocks.first().kind != BlockKind::Heading)
	{
		xml.writeStartElement(QStringLiteral("h1"));
		xml.writeAttribute(QStringLiteral("id"), QStringLiteral("book-title"));
		xml.writeCharacters(book.title);
		xml.writeEndElement();
	}
	bool listOpen = false;
	bool listItemOpen = false;
	bool nestedListOpen = false;
	bool nestedItemOpen = false;
	BlockKind openListKind = BlockKind::Paragraph;
	OrderedStyle openOrderedStyle = OrderedStyle::Decimal;
	TextDirection openListDirection = TextDirection::Ltr;
	TextDirection nestedListDirection = TextDirection::Ltr;
	auto closeNestedList = [&]() {
		if (!nestedListOpen)
			return;
		if (nestedItemOpen)
			xml.writeEndElement(); // nested li
		xml.writeEndElement(); // nested ul
		nestedItemOpen = false;
		nestedListOpen = false;
	};
	auto closeList = [&]() {
		closeNestedList();
		if (!listOpen)
			return;
		if (listItemOpen)
			xml.writeEndElement(); // parent li
		xml.writeEndElement(); // ul or ol
		listItemOpen = false;
		listOpen = false;
	};
	for (qsizetype i = chapter.firstBlock; i < chapter.endBlock; ++i)
	{
		const Block& block = book.blocks[i];
		const bool isListItem = block.kind == BlockKind::BulletItem || block.kind == BlockKind::OrderedItem;
		if (listOpen && (!isListItem || block.kind != openListKind || (block.startsList && block.listLevel == 0) ||
			(block.kind == BlockKind::OrderedItem && block.orderedStyle != openOrderedStyle)))
			closeList();
		if (isListItem && !listOpen)
		{
			xml.writeStartElement(block.kind == BlockKind::BulletItem ? QStringLiteral("ul") : QStringLiteral("ol"));
			if (block.direction == TextDirection::Rtl)
				xml.writeAttribute(QStringLiteral("dir"), QStringLiteral("rtl"));
			if (block.kind == BlockKind::OrderedItem && block.listStart != 1)
				xml.writeAttribute(QStringLiteral("start"), QString::number(block.listStart));
			if (block.kind == BlockKind::OrderedItem && block.orderedStyle != OrderedStyle::Decimal)
			{
				const QString type = block.orderedStyle == OrderedStyle::LowerRoman ? QStringLiteral("i")
					: block.orderedStyle == OrderedStyle::UpperRoman ? QStringLiteral("I")
					: block.orderedStyle == OrderedStyle::LowerAlpha ? QStringLiteral("a") : QStringLiteral("A");
				xml.writeAttribute(QStringLiteral("type"), type);
			}
			listOpen = true;
			openListKind = block.kind;
			openOrderedStyle = block.orderedStyle;
			openListDirection = block.direction;
		}
		if (isListItem && block.listLevel == 1)
		{
			if (!nestedListOpen)
			{
				xml.writeStartElement(QStringLiteral("ul"));
				if (block.direction == TextDirection::Rtl)
					xml.writeAttribute(QStringLiteral("dir"), QStringLiteral("rtl"));
				nestedListOpen = true;
				nestedListDirection = block.direction;
			}
			else if (nestedItemOpen)
				xml.writeEndElement(); // prior nested li
			nestedItemOpen = false;
		}
		else if (isListItem)
		{
			closeNestedList();
			if (listItemOpen)
				xml.writeEndElement(); // prior parent li
			listItemOpen = false;
		}
		if (block.kind == BlockKind::Image)
		{
			xml.writeStartElement(QStringLiteral("figure"));
			if (block.direction == TextDirection::Rtl)
				xml.writeAttribute(QStringLiteral("dir"), QStringLiteral("rtl"));
			if (block.alignment != TextAlignment::Left || block.direction == TextDirection::Rtl)
				xml.writeAttribute(QStringLiteral("class"), alignmentClass(block.alignment));
			if (block.imageWidthPercent > 0)
				xml.writeAttribute(QStringLiteral("style"), QStringLiteral("width: %1%; max-width: 100%;").arg(block.imageWidthPercent));
			xml.writeEmptyElement(QStringLiteral("img"));
			xml.writeAttribute(QStringLiteral("src"), QStringLiteral("images/") +
				imageFileName(block.imageIndex, book.images.at(block.imageIndex)));
			xml.writeAttribute(QStringLiteral("alt"), block.text);
			if (block.imageWidthPercent > 0)
				xml.writeAttribute(QStringLiteral("style"), QStringLiteral("width: 100%;"));
			if (block.decorative)
				xml.writeAttribute(QStringLiteral("role"), QStringLiteral("presentation"));
			if (!block.caption.isEmpty())
			{
				xml.writeStartElement(QStringLiteral("figcaption"));
				if (block.captionAlignment != TextAlignment::Left)
					xml.writeAttribute(QStringLiteral("class"), alignmentClass(block.captionAlignment));
				xml.writeCharacters(block.caption);
				xml.writeEndElement();
			}
			xml.writeEndElement();
			continue;
		}
		xml.writeStartElement(block.kind == BlockKind::Heading ? QStringLiteral("h%1").arg(block.headingLevel)
			: isListItem ? QStringLiteral("li") : QStringLiteral("p"));
		const TextDirection listDirection = block.listLevel == 1 ? nestedListDirection : openListDirection;
		if (block.direction == TextDirection::Rtl || (isListItem && listDirection == TextDirection::Rtl))
			xml.writeAttribute(QStringLiteral("dir"), block.direction == TextDirection::Rtl
				? QStringLiteral("rtl") : QStringLiteral("ltr"));
		if (block.alignment != TextAlignment::Left || block.direction == TextDirection::Rtl)
			xml.writeAttribute(QStringLiteral("class"), alignmentClass(block.alignment));
		const QString spacing = paragraphMetricsStyle(block);
		if (!spacing.isEmpty())
			xml.writeAttribute(QStringLiteral("style"), spacing);
		if (block.kind == BlockKind::Heading)
			xml.writeAttribute(QStringLiteral("id"), QStringLiteral("h%1").arg(i + 1));
		if (block.runs.isEmpty())
			xml.writeCharacters(block.text);
		else
		{
			for (const InlineRun& run : block.runs)
			{
				if (run.kind != InlineKind::Plain)
					xml.writeStartElement(run.kind == InlineKind::Strong ? QStringLiteral("strong") : QStringLiteral("em"));
				xml.writeCharacters(run.text);
				if (run.kind != InlineKind::Plain)
					xml.writeEndElement();
			}
		}
		if (isListItem)
		{
			if (block.listLevel == 1)
				nestedItemOpen = true;
			else
				listItemOpen = true;
		}
		else
			xml.writeEndElement();
	}
	closeList();
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeEndDocument();
	return bytes;
}

QByteArray navigationXml(const Book& book, const QVector<Chapter>& chapters)
{
	QByteArray bytes;
	QXmlStreamWriter xml(&bytes);
	beginXhtml(xml, book, book.title, false);
	xml.writeStartElement(QStringLiteral("nav"));
	xml.writeAttribute(QLatin1String(epubNamespace), QStringLiteral("type"), QStringLiteral("toc"));
	xml.writeAttribute(QStringLiteral("id"), QStringLiteral("toc"));
	xml.writeTextElement(QStringLiteral("h1"), QStringLiteral("Contents"));
	xml.writeStartElement(QStringLiteral("ol"));
	QVector<int> openLevels;
	qsizetype chapterIndex = 0;
	for (qsizetype i = 0; i < book.blocks.size(); ++i)
	{
		while (i >= chapters.at(chapterIndex).endBlock)
			++chapterIndex;
		const Block& block = book.blocks[i];
		if (block.kind != BlockKind::Heading)
			continue;
		if (openLevels.isEmpty())
			openLevels.append(block.headingLevel);
		else if (block.headingLevel > openLevels.last())
		{
			// A deeper heading belongs to the preceding entry. Skipped
			// heading levels do not create empty navigation entries.
			xml.writeStartElement(QStringLiteral("ol"));
			openLevels.append(block.headingLevel);
		}
		else
		{
			xml.writeEndElement(); // Previous li.
			while (openLevels.size() > 1 && block.headingLevel <= openLevels.at(openLevels.size() - 2))
			{
				xml.writeEndElement(); // Nested ol.
				xml.writeEndElement(); // Parent li.
				openLevels.removeLast();
			}
			openLevels.last() = block.headingLevel;
		}
		xml.writeStartElement(QStringLiteral("li"));
		xml.writeAttribute(QStringLiteral("dir"), block.direction == TextDirection::Rtl
			? QStringLiteral("rtl") : QStringLiteral("ltr"));
		xml.writeStartElement(QStringLiteral("a"));
		xml.writeAttribute(QStringLiteral("href"), QStringLiteral("%1#h%2")
			.arg(chapters.at(chapterIndex).fileName).arg(i + 1));
		xml.writeCharacters(block.text);
		xml.writeEndElement();
	}
	if (openLevels.isEmpty())
	{
		xml.writeStartElement(QStringLiteral("li"));
		xml.writeStartElement(QStringLiteral("a"));
		xml.writeAttribute(QStringLiteral("href"), QStringLiteral("chapter.xhtml#book-title"));
		xml.writeCharacters(book.title);
		xml.writeEndElement();
		xml.writeEndElement();
	}
	else
	{
		xml.writeEndElement(); // Final li.
		while (openLevels.size() > 1)
		{
			xml.writeEndElement(); // Nested ol.
			xml.writeEndElement(); // Parent li.
			openLevels.removeLast();
		}
	}
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeEndElement();
	xml.writeEndDocument();
	return bytes;
}
} // namespace

Result writeBook(const Book& book, const QString& outputPath)
{
	if (!validBook(book) || outputPath.isEmpty())
		return { Status::InvalidInput, QStringLiteral("EPUB requires an identifier, title, language, and valid ordered content.") };
	const QVector<Chapter> chapters = splitChapters(book);
	const QFileInfo outputInfo(outputPath);
	if (outputInfo.exists() || outputInfo.isSymLink())
		return { Status::OutputExists, QStringLiteral("The output file already exists.") };
	const QDir outputDir = outputInfo.absoluteDir();
	if (!outputDir.exists())
		return { Status::IoError, QStringLiteral("The output directory does not exist.") };
	QTemporaryDir staging(outputDir.filePath(QStringLiteral(".scribus-epub-XXXXXX")));
	if (!staging.isValid())
		return { Status::IoError, QStringLiteral("Could not create a temporary EPUB directory.") };
	const QDir temp(staging.path());
	if (!temp.mkpath(QStringLiteral("META-INF")) || !temp.mkpath(QStringLiteral("OEBPS")) ||
		(!book.images.isEmpty() && !temp.mkpath(QStringLiteral("OEBPS/images"))) ||
		!writeFile(temp.filePath(QStringLiteral("mimetype")), QByteArrayLiteral("application/epub+zip")) ||
		!writeFile(temp.filePath(QStringLiteral("META-INF/container.xml")), containerXml()) ||
		!writeFile(temp.filePath(QStringLiteral("OEBPS/content.opf")), packageXml(book, chapters)) ||
		!writeFile(temp.filePath(QStringLiteral("OEBPS/nav.xhtml")), navigationXml(book, chapters)) ||
		!writeFile(temp.filePath(QStringLiteral("OEBPS/styles.css")), stylesheet()))
		return { Status::IoError, QStringLiteral("Could not write temporary EPUB content.") };
	for (const Chapter& chapter : chapters)
	{
		if (!writeFile(temp.filePath(QStringLiteral("OEBPS/") + chapter.fileName), chapterXml(book, chapter)))
			return { Status::IoError, QStringLiteral("Could not write temporary EPUB chapter content.") };
	}
	for (qsizetype index = 0; index < book.images.size(); ++index)
	{
		if (!writeFile(temp.filePath(QStringLiteral("OEBPS/images/") + imageFileName(index, book.images.at(index))),
			book.images.at(index).data))
			return { Status::IoError, QStringLiteral("Could not write temporary EPUB image content.") };
	}
	Zip zip;
	const QString archivePath = temp.filePath(QStringLiteral("book.epub"));
	if (zip.createArchive(archivePath) != Zip::Ok)
		return { Status::ArchiveError, QStringLiteral("Could not create the EPUB archive.") };
	bool added =
		zip.addFile(temp.filePath(QStringLiteral("mimetype")), QString(), Zip::IgnorePaths, Zip::Store) == Zip::Ok &&
		zip.addFile(temp.filePath(QStringLiteral("META-INF/container.xml")), QStringLiteral("META-INF"), Zip::IgnoreRoot, Zip::AutoFull) == Zip::Ok &&
		zip.addFile(temp.filePath(QStringLiteral("OEBPS/content.opf")), QStringLiteral("OEBPS"), Zip::IgnoreRoot, Zip::AutoFull) == Zip::Ok &&
		zip.addFile(temp.filePath(QStringLiteral("OEBPS/nav.xhtml")), QStringLiteral("OEBPS"), Zip::IgnoreRoot, Zip::AutoFull) == Zip::Ok &&
		zip.addFile(temp.filePath(QStringLiteral("OEBPS/styles.css")), QStringLiteral("OEBPS"), Zip::IgnoreRoot, Zip::AutoFull) == Zip::Ok;
	for (const Chapter& chapter : chapters)
	{
		added = added && zip.addFile(temp.filePath(QStringLiteral("OEBPS/") + chapter.fileName),
			QStringLiteral("OEBPS"), Zip::IgnoreRoot, Zip::AutoFull) == Zip::Ok;
	}
	for (qsizetype index = 0; index < book.images.size(); ++index)
	{
		added = added && zip.addFile(temp.filePath(QStringLiteral("OEBPS/images/") + imageFileName(index, book.images.at(index))),
			QStringLiteral("OEBPS/images"), Zip::IgnoreRoot, Zip::AutoFull) == Zip::Ok;
	}
	const bool closed = zip.closeArchive() == Zip::Ok;
	if (!added || !closed)
		return { Status::ArchiveError, QStringLiteral("Could not finish the EPUB archive.") };
	const QFileInfo finalOutput(outputPath);
	if (finalOutput.exists() || finalOutput.isSymLink() || !QFile::rename(archivePath, outputPath))
		return { Status::IoError, QStringLiteral("Could not move the EPUB to the requested path.") };
	return { Status::Exported, {} };
}

} // namespace EpubExport
