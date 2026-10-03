/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "epubdocument.h"

#include "marks.h"
#include "numeration.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "styles/paragraphstyle.h"
#include "text/specialchars.h"

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QImageReader>
#include <QImage>
#include <QRegularExpression>
#include <QSet>

#include <functional>
#include <cmath>

namespace EpubDocument
{
namespace
{
bool validAltText(const QString& value)
{
	if (value.trimmed().isEmpty())
		return false;
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

const QString& orderAttributeName()
{
	static const QString name = QStringLiteral("scribus:epub-reading-order");
	return name;
}

const QString& imageAltAttributeName()
{
	static const QString name = QStringLiteral("scribus:epub-image-alt-text");
	return name;
}

const QString& imageCaptionAttributeName()
{
	static const QString name = QStringLiteral("scribus:epub-image-caption");
	return name;
}

const QString& imageCaptionAlignmentAttributeName()
{
	static const QString name = QStringLiteral("scribus:epub-image-caption-alignment");
	return name;
}

const QString& imageDecorativeAttributeName()
{
	static const QString name = QStringLiteral("scribus:epub-image-decorative");
	return name;
}

const QString& imageWidthAttributeName()
{
	static const QString name = QStringLiteral("scribus:epub-image-width-percent");
	return name;
}

const QString& imageFrameCropAttributeName()
{
	static const QString name = QStringLiteral("scribus:epub-image-frame-crop");
	return name;
}

bool hasRankableAncestors(const PageItem* root)
{
	QSet<const PageItem*> ancestors;
	for (const PageItem* item = root; item; item = item->Parent)
	{
		if (ancestors.contains(item))
			return false;
		ancestors.insert(item);
		if (item->isMasterItem() || !item->printEnabled())
			return false;
		if (item != root && !item->isGroup())
			return false;
	}
	return true;
}

void writeSavedOrder(PageItem* root, int rank)
{
	ObjAttrVector attributes = *root->getObjectAttributes();
	for (auto it = attributes.begin(); it != attributes.end();)
	{
		if (it->name == orderAttributeName())
			it = attributes.erase(it);
		else
			++it;
	}
	if (rank > 0)
	{
		ObjectAttribute orderAttribute;
		orderAttribute.name = orderAttributeName();
		orderAttribute.type = QStringLiteral("Integer");
		orderAttribute.value = QString::number(rank);
		attributes.append(orderAttribute);
	}
	root->setObjectAttributes(&attributes);
}

struct DocumentContent
{
	EpubReadingOrder::ContentScan scan;
	QVector<PageItem*> textFrames;
	QVector<PageItem*> imageFrames;
	QVector<PageItem*> orderedFrames;
};

QVector<PageItem*> rankableItemsFromContent(const DocumentContent& content)
{
	QVector<PageItem*> result;
	if (content.scan.status == EpubReadingOrder::Status::InvalidChain)
		return result;
	for (PageItem* item : content.orderedFrames)
	{
		if (canRankStoryRoot(item) || canRankImageFrame(item))
			result.append(item);
	}
	return result;
}

DocumentContent collectContent(const ScribusDoc& document, QVector<PreflightIssue>* issues = nullptr)
{
	QVector<PageItem*> items;
	QVector<EpubReadingOrder::ContentNode> nodes;
	QHash<PageItem*, int> indices;
	std::function<int(PageItem*)> snapshot = [&](PageItem* item) -> int
	{
		if (!item)
			return -1;
		if (indices.contains(item))
			return indices.value(item);
		const int index = items.size();
		indices.insert(item, index);
		items.append(item);
		EpubReadingOrder::ContentNode node;
		node.included = !item->isMasterItem() && item->printEnabled();
		if (item->isGroup())
			node.kind = EpubReadingOrder::ContentKind::Group;
		else if (item->isTextFrame() && !item->isAnnotation() && !item->isNoteFrame() &&
			!item->isAutoNoteFrame() && !item->isTableCell())
			node.kind = EpubReadingOrder::ContentKind::TextFrame;
		else if (item->isImageFrame())
			node.kind = EpubReadingOrder::ContentKind::ImageFrame;
		nodes.append(node);
		if (item->isGroup())
		{
			for (PageItem* child : item->getChildren())
			{
				// snapshot() grows nodes and can invalidate a reference to
				// nodes[index] before append() runs.
				const int childIndex = snapshot(child);
				nodes[index].children.append(childIndex);
			}
		}
		return index;
	};
	QVector<int> topLevelIndices;
	for (PageItem* item : document.DocItems)
		topLevelIndices.append(snapshot(item));
	DocumentContent result;
	result.scan = EpubReadingOrder::scanContent(nodes, topLevelIndices);
	if (issues)
	{
		for (int index : result.scan.unsupportedIndices)
			issues->append({ PreflightSeverity::Error, QStringLiteral("object-unsupported"),
				QStringLiteral("Page object '%1' cannot be preserved in reflowable EPUB.").arg(items.at(index)->itemName()) });
	}
	for (int index : result.scan.textFrameIndices)
		result.textFrames.append(items.at(index));
	for (int index : result.scan.imageFrameIndices)
		result.imageFrames.append(items.at(index));
	QSet<int> includedFrames;
	for (int index : result.scan.textFrameIndices)
		includedFrames.insert(index);
	for (int index : result.scan.imageFrameIndices)
		includedFrames.insert(index);
	for (int index = 0; index < items.size(); ++index)
	{
		if (includedFrames.contains(index))
			result.orderedFrames.append(items.at(index));
	}
	return result;
}

QString savedImageText(const PageItem* item, const QString& attributeName)
{
	if (!item || !item->isImageFrame())
		return {};
	const auto attributes = item->getObjectAttributes(attributeName);
	if (attributes.size() != 1 || attributes.first().type != QLatin1String("String"))
		return {};
	return validAltText(attributes.first().value) ? attributes.first().value : QString();
}

bool setSavedImageText(PageItem* item, const QString& value, const QString& attributeName)
{
	if (!item || !item->isImageFrame() || item->isMasterItem() ||
		(!value.isEmpty() && (!validAltText(value) || savedImageDecorative(item))))
		return false;
	const auto previous = item->getObjectAttributes(attributeName);
	if ((value.isEmpty() && previous.isEmpty()) ||
		(previous.size() == 1 && previous.first().type == QLatin1String("String") &&
			previous.first().value == value))
		return true;
	ObjAttrVector attributes = *item->getObjectAttributes();
	for (auto it = attributes.begin(); it != attributes.end();)
	{
		if (it->name == attributeName)
			it = attributes.erase(it);
		else
			++it;
	}
	if (!value.isEmpty())
	{
		ObjectAttribute attribute;
		attribute.name = attributeName;
		attribute.type = QStringLiteral("String");
		attribute.value = value;
		attributes.append(attribute);
	}
	item->setObjectAttributes(&attributes);
	item->doc()->changed();
	return true;
}
}

namespace
{
bool exactPixelCoordinate(double value, int& result)
{
	if (!std::isfinite(value) || value < -10000000.0 || value > 10000000.0)
		return false;
	const double rounded = std::round(value);
	if (std::abs(value - rounded) > 0.00001)
		return false;
	result = static_cast<int>(rounded);
	return true;
}

ImageFrameResult extractLinkedImageFrameImpl(const PageItem* item, const QString& altText, bool decorative)
{
	if (!item || !item->isImageFrame() || item->isLatexFrame() || item->isOSGFrame() ||
		item->isImageInline() || item->isMasterItem() || !item->printEnabled())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("Choose a printable, normal-page linked image frame."), {}, {} };
	if ((!decorative && !validAltText(altText)) || (decorative && !altText.isEmpty()))
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("A linked image requires valid alt text, unless it is explicitly decorative."), {}, {} };
	const QString caption = savedImageCaption(item);
	const int captionAlignment = savedImageCaptionAlignment(item);
	if (captionAlignment < 0 || (caption.isEmpty() && captionAlignment != 0))
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The image frame has invalid EPUB caption alignment."), {}, {} };
	const int widthPercent = savedImageWidthPercent(item);
	const auto cropAttributes = item->getObjectAttributes(imageFrameCropAttributeName());
	if (!cropAttributes.isEmpty() && !savedUseImageFrameCrop(item))
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The image frame has an invalid EPUB crop flag."), {}, {} };
	const bool useFrameCrop = savedUseImageFrameCrop(item);
	if (widthPercent < 0)
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The image frame has an invalid EPUB width percentage."), {}, {} };
	if (decorative && !caption.isEmpty())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("A decorative image cannot have a visible caption."), {}, {} };
	if (!item->getObjectAttributes(imageCaptionAttributeName()).isEmpty() && caption.isEmpty())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The image frame has an invalid EPUB caption."), {}, {} };
	if (!item->imageIsAvailable || item->externalFile().isEmpty())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The image frame has no available linked file."), {}, {} };
	QFile source(item->externalFile());
	if (!source.open(QIODevice::ReadOnly))
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The linked image file cannot be read."), {}, {} };
	const QByteArray bytes = source.readAll();
	if (bytes.isEmpty() || bytes.size() != source.size())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The linked image file is empty or could not be read completely."), {}, {} };
	QBuffer buffer;
	buffer.setData(bytes);
	if (!buffer.open(QIODevice::ReadOnly))
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The linked image could not be inspected."), {}, {} };
	QImageReader reader(&buffer);
	reader.setDecideFormatFromContent(true);
	if (!reader.canRead())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The linked image is not a readable PNG or JPEG."), {}, {} };
	const QByteArray format = reader.format().toLower();
	const QString mediaType = format == QByteArrayLiteral("png") ? QStringLiteral("image/png")
		: format == QByteArrayLiteral("jpeg") || format == QByteArrayLiteral("jpg")
			? QStringLiteral("image/jpeg") : QString();
	const QImage decoded = reader.read();
	if (mediaType.isEmpty() || decoded.isNull())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The linked image is not a decodable PNG or JPEG."), {}, {} };
	QByteArray outputBytes = bytes;
	if (useFrameCrop)
	{
		if (mediaType != QLatin1String("image/png") || item->frameType() != PageItem::Rectangle ||
			!item->hasDefaultShape() || !item->imageClip.empty() || item->maskType() != 0 ||
			!item->effectsInUse.isEmpty() || item->imageFlippedH() || item->imageFlippedV() ||
			std::abs(item->imageRotation()) > 0.000001 || std::abs(item->rotation()) > 0.000001)
			return { EpubReadingOrder::Status::UnsupportedContent,
				QStringLiteral("Exact EPUB frame crop currently requires a plain, unrotated rectangular PNG image frame without flips, clipping paths, masks, or effects."), {}, {} };
		const double xScale = item->imageXScale();
		const double yScale = item->imageYScale();
		if (!std::isfinite(xScale) || !std::isfinite(yScale) || xScale <= 0.0 ||
			std::abs(xScale - yScale) > 0.000001)
			return { EpubReadingOrder::Status::UnsupportedContent,
				QStringLiteral("Exact EPUB frame crop requires a positive, uniform image scale."), {}, {} };
		int x = 0, y = 0, width = 0, height = 0;
		if (!exactPixelCoordinate(-item->imageXOffset(), x) ||
			!exactPixelCoordinate(-item->imageYOffset(), y) ||
			!exactPixelCoordinate(item->width() / xScale, width) ||
			!exactPixelCoordinate(item->height() / yScale, height) ||
			x < 0 || y < 0 || width <= 0 || height <= 0 ||
			width > 8192 || height > 8192 || qint64(width) * height > 16000000 ||
			qint64(x) + width > decoded.width() || qint64(y) + height > decoded.height())
			return { EpubReadingOrder::Status::UnsupportedContent,
				QStringLiteral("Exact EPUB frame crop requires an integer-pixel viewport fully covered by the linked PNG (maximum 8192 px per side and 16 million pixels)."), {}, {} };
		if (x != 0 || y != 0 || width != decoded.width() || height != decoded.height())
		{
			QBuffer croppedBuffer(&outputBytes);
			outputBytes.clear();
			if (!croppedBuffer.open(QIODevice::WriteOnly) ||
				!decoded.copy(x, y, width, height).save(&croppedBuffer, "PNG"))
				return { EpubReadingOrder::Status::UnsupportedContent,
					QStringLiteral("The exact EPUB frame crop could not be encoded as PNG."), {}, {} };
		}
	}
	EpubExport::Block block;
	block.kind = EpubExport::BlockKind::Image;
	block.text = altText;
	block.imageIndex = 0; // Caller replaces this when appending to a Book.
	block.caption = caption;
	block.captionAlignment = static_cast<EpubExport::TextAlignment>(captionAlignment);
	block.decorative = decorative;
	block.imageWidthPercent = widthPercent;
	return { EpubReadingOrder::Status::Ready, {}, block, { outputBytes, mediaType }, useFrameCrop };
}
}

ImageFrameResult extractLinkedImageFrame(const PageItem* item, const QString& altText)
{
	return extractLinkedImageFrameImpl(item, altText, false);
}

ImageFrameResult extractLinkedImageFrame(const PageItem* item)
{
	if (item && !item->getObjectAttributes(imageDecorativeAttributeName()).isEmpty() && !savedImageDecorative(item))
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The image frame has an invalid EPUB decorative flag."), {}, {} };
	if (item && !item->getObjectAttributes(imageAltAttributeName()).isEmpty() && savedImageAltText(item).isEmpty())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The image frame has an invalid EPUB description."), {}, {} };
	return extractLinkedImageFrameImpl(item, savedImageAltText(item), savedImageDecorative(item));
}

QString savedImageAltText(const PageItem* item)
{
	return savedImageText(item, imageAltAttributeName());
}

bool setSavedImageAltText(PageItem* item, const QString& altText)
{
	return setSavedImageText(item, altText, imageAltAttributeName());
}

bool savedImageDecorative(const PageItem* item)
{
	if (!item || !item->isImageFrame())
		return false;
	const auto attributes = item->getObjectAttributes(imageDecorativeAttributeName());
	return attributes.size() == 1 && attributes.first().type == QLatin1String("Boolean") &&
		attributes.first().value == QLatin1String("true");
}

bool setSavedImageDecorative(PageItem* item, bool decorative)
{
	if (!item || !item->isImageFrame() || item->isMasterItem() ||
		(decorative && (!item->getObjectAttributes(imageAltAttributeName()).isEmpty() ||
			!item->getObjectAttributes(imageCaptionAttributeName()).isEmpty())))
		return false;
	const auto previous = item->getObjectAttributes(imageDecorativeAttributeName());
	if ((decorative && savedImageDecorative(item)) || (!decorative && previous.isEmpty()))
		return true;
	ObjAttrVector attributes = *item->getObjectAttributes();
	for (auto it = attributes.begin(); it != attributes.end();)
	{
		if (it->name == imageDecorativeAttributeName())
			it = attributes.erase(it);
		else
			++it;
	}
	if (decorative)
	{
		ObjectAttribute attribute;
		attribute.name = imageDecorativeAttributeName();
		attribute.type = QStringLiteral("Boolean");
		attribute.value = QStringLiteral("true");
		attributes.append(attribute);
	}
	item->setObjectAttributes(&attributes);
	item->doc()->changed();
	return true;
}

QString savedImageCaption(const PageItem* item)
{
	return savedImageText(item, imageCaptionAttributeName());
}

bool setSavedImageCaption(PageItem* item, const QString& caption)
{
	if (!setSavedImageText(item, caption, imageCaptionAttributeName()))
		return false;
	return !caption.isEmpty() || setSavedImageCaptionAlignment(item, 0);
}

int savedImageCaptionAlignment(const PageItem* item)
{
	if (!item || !item->isImageFrame())
		return -1;
	const auto attributes = item->getObjectAttributes(imageCaptionAlignmentAttributeName());
	if (attributes.isEmpty())
		return 0;
	if (attributes.size() != 1 || attributes.first().type != QLatin1String("Integer"))
		return -1;
	bool valid = false;
	const int value = attributes.first().value.toInt(&valid);
	return valid && value >= 1 && value <= 3 ? value : -1;
}

bool setSavedImageCaptionAlignment(PageItem* item, int alignment)
{
	if (!item || !item->isImageFrame() || item->isMasterItem() ||
		alignment < 0 || alignment > 3 ||
		(alignment > 0 && savedImageCaption(item).isEmpty()))
		return false;
	const auto previous = item->getObjectAttributes(imageCaptionAlignmentAttributeName());
	if ((alignment == 0 && previous.isEmpty()) ||
		(alignment > 0 && previous.size() == 1 &&
			previous.first().type == QLatin1String("Integer") &&
			previous.first().value == QString::number(alignment)))
		return true;
	ObjAttrVector attributes = *item->getObjectAttributes();
	for (auto it = attributes.begin(); it != attributes.end();)
	{
		if (it->name == imageCaptionAlignmentAttributeName())
			it = attributes.erase(it);
		else
			++it;
	}
	if (alignment > 0)
	{
		ObjectAttribute attribute;
		attribute.name = imageCaptionAlignmentAttributeName();
		attribute.type = QStringLiteral("Integer");
		attribute.value = QString::number(alignment);
		attributes.append(attribute);
	}
	item->setObjectAttributes(&attributes);
	item->doc()->changed();
	return true;
}

int savedImageWidthPercent(const PageItem* item)
{
	if (!item || !item->isImageFrame())
		return -1;
	const auto attributes = item->getObjectAttributes(imageWidthAttributeName());
	if (attributes.isEmpty())
		return 0;
	if (attributes.size() != 1 || attributes.first().type != QLatin1String("Integer"))
		return -1;
	bool valid = false;
	const int value = attributes.first().value.toInt(&valid);
	return valid && value >= 1 && value <= 100 ? value : -1;
}

bool setSavedImageWidthPercent(PageItem* item, int widthPercent)
{
	if (!item || !item->isImageFrame() || item->isMasterItem() ||
		widthPercent < 0 || widthPercent > 100)
		return false;
	const auto previous = item->getObjectAttributes(imageWidthAttributeName());
	if ((widthPercent == 0 && previous.isEmpty()) ||
		(widthPercent > 0 && previous.size() == 1 &&
			previous.first().type == QLatin1String("Integer") &&
			previous.first().value == QString::number(widthPercent)))
		return true;
	ObjAttrVector attributes = *item->getObjectAttributes();
	for (auto it = attributes.begin(); it != attributes.end();)
	{
		if (it->name == imageWidthAttributeName())
			it = attributes.erase(it);
		else
			++it;
	}
	if (widthPercent > 0)
	{
		ObjectAttribute attribute;
		attribute.name = imageWidthAttributeName();
		attribute.type = QStringLiteral("Integer");
		attribute.value = QString::number(widthPercent);
		attributes.append(attribute);
	}
	item->setObjectAttributes(&attributes);
	item->doc()->changed();
	return true;
}

bool savedUseImageFrameCrop(const PageItem* item)
{
	if (!item || !item->isImageFrame())
		return false;
	const auto attributes = item->getObjectAttributes(imageFrameCropAttributeName());
	return attributes.size() == 1 && attributes.first().type == QLatin1String("Boolean") &&
		attributes.first().value == QLatin1String("true");
}

bool setSavedUseImageFrameCrop(PageItem* item, bool enabled)
{
	if (!item || !item->isImageFrame() || item->isMasterItem())
		return false;
	const auto previous = item->getObjectAttributes(imageFrameCropAttributeName());
	if ((enabled && savedUseImageFrameCrop(item)) || (!enabled && previous.isEmpty()))
		return true;
	ObjAttrVector attributes = *item->getObjectAttributes();
	for (auto it = attributes.begin(); it != attributes.end();)
	{
		if (it->name == imageFrameCropAttributeName())
			it = attributes.erase(it);
		else
			++it;
	}
	if (enabled)
	{
		ObjectAttribute attribute;
		attribute.name = imageFrameCropAttributeName();
		attribute.type = QStringLiteral("Boolean");
		attribute.value = QStringLiteral("true");
		attributes.append(attribute);
	}
	item->setObjectAttributes(&attributes);
	item->doc()->changed();
	return true;
}

int savedOrder(const PageItem* root)
{
	if (!root)
		return -1;
	const auto attributes = root->getObjectAttributes(orderAttributeName());
	if (attributes.isEmpty())
		return -1;
	if (attributes.size() != 1)
		return -2;
	bool valid = false;
	const int rank = attributes.first().value.toInt(&valid);
	return valid && rank > 0 ? rank : -2;
}

bool setSavedOrder(PageItem* root, int rank)
{
	if ((!canRankStoryRoot(root) && !canRankImageFrame(root)) || rank < 0)
		return false;
	writeSavedOrder(root, rank);
	root->doc()->changed();
	return true;
}

bool canRankStoryRoot(const PageItem* root)
{
	if (!root || !root->isTextFrame() || root->prevInChain())
		return false;
	return hasRankableAncestors(root);
}

bool canRankImageFrame(const PageItem* item)
{
	return item && item->isImageFrame() && !item->isLatexFrame() &&
		!item->isOSGFrame() && !item->isImageInline() && hasRankableAncestors(item);
}

bool hasRankedImageOrder(const ScribusDoc& document)
{
	QSet<PageItem*> visited;
	std::function<bool(PageItem*)> visit = [&](PageItem* item) -> bool
	{
		if (!item || visited.contains(item) || item->isMasterItem() || !item->printEnabled())
			return false;
		visited.insert(item);
		if (item->isImageFrame() && savedOrder(item) != -1)
			return true;
		if (item->isGroup())
		{
			for (PageItem* child : item->getChildren())
			{
				if (visit(child))
					return true;
			}
		}
		return false;
	};
	for (PageItem* item : document.DocItems)
	{
		if (visit(item))
			return true;
	}
	return false;
}

QVector<PageItem*> storyRoots(ScribusDoc& document)
{
	QVector<PageItem*> roots;
	const DocumentContent content = collectContent(document);
	if (content.scan.status == EpubReadingOrder::Status::InvalidChain)
		return roots;
	for (PageItem* item : content.textFrames)
	{
		if (canRankStoryRoot(item))
			roots.append(item);
	}
	return roots;
}

bool assignSavedOrder(ScribusDoc& document, const QVector<PageItem*>& orderedRoots)
{
	if (hasRankedImageOrder(document))
		return false;
	const DocumentContent content = collectContent(document);
	if (content.scan.status == EpubReadingOrder::Status::InvalidChain)
		return false;
	QVector<PageItem*> allRoots;
	for (PageItem* item : content.textFrames)
	{
		if (canRankStoryRoot(item))
			allRoots.append(item);
	}
	if (allRoots.isEmpty() || orderedRoots.size() != allRoots.size())
		return false;
	QSet<PageItem*> expected;
	for (PageItem* root : allRoots)
		expected.insert(root);
	QSet<PageItem*> selected;
	for (PageItem* root : orderedRoots)
	{
		if (!expected.contains(root) || selected.contains(root))
			return false;
		selected.insert(root);
	}
	// A former chain root may retain a rank after relinking. Clear only our
	// reserved attribute on non-root text frames while keeping all other data.
	for (PageItem* item : content.textFrames)
	{
		if (item->prevInChain() && savedOrder(item) != -1)
			writeSavedOrder(item, 0);
	}
	for (int index = 0; index < orderedRoots.size(); ++index)
		writeSavedOrder(orderedRoots.at(index), index + 1);
	document.changed();
	return true;
}

QVector<PageItem*> rankableItems(const ScribusDoc& document)
{
	return rankableItemsFromContent(collectContent(document));
}

StyleNames usedStyleNames(const ScribusDoc& document)
{
	QSet<QString> paragraphNames;
	QSet<QString> characterNames;
	for (const PageItem* item : collectContent(document).textFrames)
	{
		if (item->prevInChain())
			continue;
		auto collectParagraph = [&](int position) {
			const ParagraphStyle& style = item->itemText.paragraphStyle(position);
			const BaseStyle* parent = style.hasParent() ? style.parentStyle() : nullptr;
			if (parent && !parent->isDefaultStyle())
				paragraphNames.insert(parent->name());
		};
		int paragraphStart = 0;
		for (int position = 0; position < item->itemText.length(); ++position)
		{
			if (item->itemText.text(position) == SpecialChars::PARSEP)
			{
				collectParagraph(paragraphStart);
				paragraphStart = position + 1;
				continue;
			}
			const CharStyle& style = item->itemText.charStyle(position);
			const BaseStyle* parent = style.hasParent() ? style.parentStyle() : nullptr;
			if (parent && !parent->isDefaultStyle())
				characterNames.insert(parent->name());
		}
		collectParagraph(paragraphStart);
	}
	StyleNames names;
	names.paragraph = paragraphNames.values();
	names.character = characterNames.values();
	names.paragraph.sort(Qt::CaseInsensitive);
	names.character.sort(Qt::CaseInsensitive);
	return names;
}

QStringList lineSpacingWarnings(const ScribusDoc& document)
{
	int fixedStories = 0;
	for (const PageItem* item : rankableItems(document))
	{
		if (!item->isTextFrame() || item->prevInChain() || item->itemText.length() == 0)
			continue;
		bool hasFixedLeading = false;
		for (int position = 0; position < item->itemText.length(); ++position)
		{
			if (position != 0 && item->itemText.text(position - 1) != SpecialChars::PARSEP)
				continue;
			if (item->itemText.paragraphStyle(position).lineSpacingMode() == ParagraphStyle::FixedLineSpacing)
			{
				hasFixedLeading = true;
				break;
			}
		}
		fixedStories += hasFixedLeading ? 1 : 0;
	}
	if (fixedStories == 0)
		return {};
	return { QStringLiteral("Fixed Apscribe line spacing occurs in %1 text stories; EPUB readers may use different leading.")
		.arg(fixedStories) };
}

bool assignMixedSavedOrder(ScribusDoc& document, const QVector<PageItem*>& orderedItems)
{
	const DocumentContent content = collectContent(document);
	const QVector<PageItem*> expectedItems = rankableItemsFromContent(content);
	if (expectedItems.isEmpty() || orderedItems.size() != expectedItems.size())
		return false;
	QSet<PageItem*> expected;
	for (PageItem* item : expectedItems)
		expected.insert(item);
	QSet<PageItem*> selected;
	for (PageItem* item : orderedItems)
	{
		if (!expected.contains(item) || selected.contains(item))
			return false;
		selected.insert(item);
	}
	// Relinking or embedding may leave ranks on units that are no longer
	// independently rankable. Clear only the reserved EPUB rank attribute.
	for (PageItem* item : content.orderedFrames)
	{
		if (!expected.contains(item) && savedOrder(item) != -1)
			writeSavedOrder(item, 0);
	}
	for (int index = 0; index < orderedItems.size(); ++index)
		writeSavedOrder(orderedItems.at(index), index + 1);
	document.changed();
	return true;
}

namespace
{
bool standardBullet(const ParagraphStyle& paragraph)
{
	return paragraph.hasBullet() && !paragraph.hasNum() && paragraph.numLevel() >= 0 && paragraph.numLevel() <= 1 &&
		paragraph.bulletStr() == QString(QChar(0x2022));
}

bool localOrderedNumber(const ParagraphStyle& paragraph)
{
	const int format = paragraph.numFormat();
	return paragraph.hasNum() && !paragraph.hasBullet() &&
		paragraph.numName() == QLatin1String("<local block>") &&
		(format == Type_1_2_3 || format == Type_i_ii_iii || format == Type_I_II_III ||
			format == Type_a_b_c || format == Type_A_B_C) &&
		paragraph.numLevel() == 0 &&
		paragraph.numPrefix().isEmpty() && paragraph.numSuffix() == QLatin1String(".") &&
		paragraph.numStart() > 0 && paragraph.numRestart() == NSRdocument && !paragraph.numOther();
}

EpubExport::OrderedStyle orderedStyle(const ParagraphStyle& paragraph)
{
	if (paragraph.numFormat() == Type_i_ii_iii)
		return EpubExport::OrderedStyle::LowerRoman;
	if (paragraph.numFormat() == Type_I_II_III)
		return EpubExport::OrderedStyle::UpperRoman;
	if (paragraph.numFormat() == Type_a_b_c)
		return EpubExport::OrderedStyle::LowerAlpha;
	if (paragraph.numFormat() == Type_A_B_C)
		return EpubExport::OrderedStyle::UpperAlpha;
	return EpubExport::OrderedStyle::Decimal;
}

Result extractDocument(const ScribusDoc& document, EpubExport::Book metadata,
	const QVector<const PageItem*>& orderedStoryRoots, bool useSavedOrder, bool includeImages,
	const EpubReadingOrder::ParagraphStyleMap& styles,
	const EpubReadingOrder::CharacterStyleMap& characterStyles,
	QVector<PreflightIssue>* issues = nullptr)
{
	if (!metadata.blocks.isEmpty() || !metadata.images.isEmpty())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("Document extraction requires an empty publication body."), {} };

	const DocumentContent content = collectContent(document, issues);
	const bool unsupportedObjects = content.scan.status == EpubReadingOrder::Status::UnsupportedContent;
	if (!content.scan.valid() && !(issues && unsupportedObjects))
		return { content.scan.status, content.scan.detail, {} };
	if (!includeImages && !content.imageFrames.isEmpty())
		return { EpubReadingOrder::Status::UnsupportedContent,
			QStringLiteral("The text-only EPUB path cannot preserve image frames."), {} };
	const QVector<PageItem*>& textFrames = content.textFrames;

	QHash<const PageItem*, int> ids;
	for (int i = 0; i < textFrames.size(); ++i)
		ids.insert(textFrames.at(i), i);

	QVector<EpubReadingOrder::Frame> frames;
	for (int i = 0; i < textFrames.size(); ++i)
	{
		const PageItem* item = textFrames.at(i);
		EpubReadingOrder::Frame frame;
		frame.id = i;
		frame.previousId = item->prevInChain() ? ids.value(item->prevInChain(), -2) : -1;
		frame.nextId = item->nextInChain() ? ids.value(item->nextInChain(), -2) : -1;
		frame.savedOrder = savedOrder(item);
		const qsizetype issuesBefore = issues ? issues->size() : 0;
		if (!item->prevInChain())
		{
			int paragraphNumber = 1;
			auto reportUnsupported = [&](const QString& detail, bool updateExtractionDetail = true) {
				frame.unsupportedContent = true;
				if (updateExtractionDetail)
					frame.unsupportedDetail = detail;
				if (issues)
					issues->append({ PreflightSeverity::Error, QStringLiteral("story-unsupported"),
						QStringLiteral("Text story '%1', paragraph %2: %3")
							.arg(item->itemName()).arg(paragraphNumber).arg(detail) });
			};
			auto appendParagraph = [&](int start)
			{
				const ParagraphStyle& paragraph = item->itemText.paragraphStyle(start);
				if (paragraph.lineSpacingMode() == ParagraphStyle::BaselineGridLineSpacing)
					reportUnsupported(QStringLiteral("Baseline-grid line spacing cannot be represented in reflowable EPUB."));
				else if (paragraph.lineSpacingMode() != ParagraphStyle::AutomaticLineSpacing &&
					(paragraph.lineSpacingMode() != ParagraphStyle::FixedLineSpacing ||
						!std::isfinite(paragraph.lineSpacing()) || paragraph.lineSpacing() <= 0.0))
					reportUnsupported(QStringLiteral("The paragraph has invalid or unsupported line spacing."));
				const bool bullet = standardBullet(paragraph);
				const bool number = localOrderedNumber(paragraph);
				if (paragraph.hasDropCap() || (paragraph.hasNum() && !number) || (paragraph.hasBullet() && !bullet))
					reportUnsupported(QStringLiteral("Only standard bullets to depth one and flat local decimal, Roman, or A–Z numbering are supported; custom or restarted lists and drop caps need separate EPUB semantics."));
				frame.bulletParagraphs.append(bullet);
				frame.bulletParagraphLevels.append(bullet ? paragraph.numLevel() : 0);
				const EpubExport::ParagraphMetrics metrics { paragraph.leftMargin(), paragraph.rightMargin(),
					paragraph.firstIndent(), paragraph.gapBefore(), paragraph.gapAfter() };
				if ((bullet || number) && !metrics.isDefault())
					reportUnsupported(QStringLiteral("EPUB list items with custom paragraph indents or spacing need separate list CSS mapping."));
				frame.paragraphMetrics.append(metrics);
				frame.orderedParagraphStarts.append(number ? paragraph.numStart() : 0);
				frame.orderedParagraphStyles.append(number ? orderedStyle(paragraph) : EpubExport::OrderedStyle::Decimal);
				if (paragraph.direction() != ParagraphStyle::LTR && paragraph.direction() != ParagraphStyle::RTL)
					reportUnsupported(QStringLiteral("The paragraph has an unsupported base direction."));
				frame.paragraphDirections.append(paragraph.direction() == ParagraphStyle::RTL
					? EpubExport::TextDirection::Rtl : EpubExport::TextDirection::Ltr);
				switch (paragraph.alignment())
				{
					case ParagraphStyle::LeftAligned:
						frame.paragraphAlignments.append(EpubExport::TextAlignment::Left);
						break;
					case ParagraphStyle::Centered:
						frame.paragraphAlignments.append(EpubExport::TextAlignment::Center);
						break;
					case ParagraphStyle::RightAligned:
						frame.paragraphAlignments.append(EpubExport::TextAlignment::Right);
						break;
					case ParagraphStyle::Justified:
						frame.paragraphAlignments.append(EpubExport::TextAlignment::Justify);
						break;
					default:
						reportUnsupported(QStringLiteral("Extended or unknown paragraph alignment needs separate EPUB styling."));
						frame.paragraphAlignments.append(EpubExport::TextAlignment::Left);
						break;
				}
				const BaseStyle* parent = paragraph.hasParent() ? paragraph.parentStyle() : nullptr;
				frame.paragraphStyleNames.append(parent && !parent->isDefaultStyle() ? parent->name() : QString());
			};
			int paragraphStart = 0;
			for (int position = 0; position < item->itemText.length(); ++position)
			{
				if (item->itemText.hasMark(position, MARKBullNumType))
				{
					const ParagraphStyle& paragraph = item->itemText.paragraphStyle(position);
					if (position == paragraphStart && (standardBullet(paragraph) || localOrderedNumber(paragraph)))
						continue; // Generated visual marker; ul/ol supplies it in EPUB.
					reportUnsupported(QStringLiteral("A bullet/number mark is not a supported list marker."));
					if (!issues)
						break;
					if (item->itemText.text(position) == SpecialChars::PARSEP)
					{
						appendParagraph(paragraphStart);
						paragraphStart = position + 1;
						++paragraphNumber;
					}
					continue;
				}
				const QChar ch = item->itemText.text(position);
				if (ch == SpecialChars::PARSEP)
				{
					frame.characterStyleNames.append(QString());
					appendParagraph(paragraphStart);
					paragraphStart = position + 1;
				}
				else
				{
					const CharStyle& character = item->itemText.charStyle(position);
					const BaseStyle* parent = character.hasParent() ? character.parentStyle() : nullptr;
					frame.characterStyleNames.append(parent && !parent->isDefaultStyle() ? parent->name() : QString());
				}
				if (item->itemText.hasObject(position) || item->itemText.hasMark(position) ||
					ch == SpecialChars::LINEBREAK || ch == SpecialChars::COLBREAK ||
					ch == SpecialChars::FRAMEBREAK || ch == SpecialChars::PAGENUMBER ||
					ch == SpecialChars::PAGECOUNT || (ch.unicode() < 0x20 && ch != SpecialChars::PARSEP))
				{
					reportUnsupported(QStringLiteral("An inline object, mark, or control character is not supported in reflowable EPUB."), false);
					if (!issues)
						break;
					if (ch == SpecialChars::PARSEP)
						++paragraphNumber;
					continue;
				}
				frame.rootText.append(ch == SpecialChars::PARSEP ? QLatin1Char('\n') : ch);
				if (ch == SpecialChars::PARSEP)
					++paragraphNumber;
			}
			appendParagraph(paragraphStart);
		}
		if (issues && frame.unsupportedContent && issues->size() == issuesBefore)
		{
			const QString detail = frame.unsupportedDetail.isEmpty()
				? QStringLiteral("This story contains an inline object, mark, control character, or other unsupported text content.")
				: frame.unsupportedDetail;
			issues->append({ PreflightSeverity::Error, QStringLiteral("story-unsupported"),
				QStringLiteral("Text story '%1': %2").arg(item->itemName(), detail) });
		}
		frames.append(frame);
	}
	if (unsupportedObjects)
		return { content.scan.status, content.scan.detail, {} };

	QVector<int> orderedRootIds;
	for (const PageItem* root : orderedStoryRoots)
		orderedRootIds.append(ids.value(root, -2));
	QVector<EpubReadingOrder::Image> images;
	if (includeImages)
	{
		for (int index = 0; index < content.imageFrames.size(); ++index)
		{
			const PageItem* item = content.imageFrames.at(index);
			const ImageFrameResult inspected = extractLinkedImageFrame(item);
			if (!inspected.ready())
				return { inspected.status,
					QStringLiteral("Image frame '%1': %2").arg(item->itemName(), inspected.detail), {} };
			images.append({ index, savedOrder(item), inspected.block.text, inspected.asset,
				EpubExport::TextDirection::Ltr, EpubExport::TextAlignment::Left,
				inspected.block.caption, inspected.block.decorative,
				inspected.block.imageWidthPercent, inspected.block.captionAlignment });
		}
	}
	const auto extraction = includeImages
		? EpubReadingOrder::extractMixedSavedOrder(frames, images, styles, characterStyles)
		: useSavedOrder ? EpubReadingOrder::extractSavedOrder(frames, styles, characterStyles)
			: EpubReadingOrder::extractStories(frames, orderedRootIds, styles, characterStyles);
	if (!extraction.ready())
		return { extraction.status, extraction.detail, {} };
	metadata.blocks = extraction.blocks;
	metadata.images = extraction.images;
	return { EpubReadingOrder::Status::Ready, {}, metadata };
}
} // namespace

Result extractStories(const ScribusDoc& document, EpubExport::Book metadata,
	const QVector<const PageItem*>& orderedStoryRoots,
	const EpubReadingOrder::ParagraphStyleMap& styles,
	const EpubReadingOrder::CharacterStyleMap& characterStyles)
{
	return extractDocument(document, metadata, orderedStoryRoots, false, false, styles, characterStyles);
}

Result extractSavedOrder(const ScribusDoc& document, EpubExport::Book metadata,
	const EpubReadingOrder::ParagraphStyleMap& styles,
	const EpubReadingOrder::CharacterStyleMap& characterStyles)
{
	return extractDocument(document, metadata, {}, true, false, styles, characterStyles);
}

Result extractMixedSavedOrder(const ScribusDoc& document, EpubExport::Book metadata,
	const EpubReadingOrder::ParagraphStyleMap& styles,
	const EpubReadingOrder::CharacterStyleMap& characterStyles)
{
	return extractDocument(document, metadata, {}, true, true, styles, characterStyles);
}

PreflightReport preflightMixedSavedOrder(const ScribusDoc& document, EpubExport::Book metadata,
	const QString& outputPath, bool validateDestination,
	const EpubReadingOrder::ParagraphStyleMap& styles,
	const EpubReadingOrder::CharacterStyleMap& characterStyles)
{
	PreflightReport report;
	report.extraction = extractDocument(document, metadata, {}, true, true, styles, characterStyles, &report.issues);
	auto add = [&](PreflightSeverity severity, const QString& code, const QString& detail) {
		report.issues.append({ severity, code, detail });
	};
	const bool checkMetadata = validateDestination || !metadata.title.isEmpty() || !metadata.identifier.isEmpty() ||
		!metadata.language.isEmpty() || !metadata.author.isEmpty();
	if (checkMetadata)
	{
		if (!validAltText(metadata.title))
			add(PreflightSeverity::Error, QStringLiteral("title"), QStringLiteral("A valid publication title is required."));
		if (!validAltText(metadata.identifier))
			add(PreflightSeverity::Error, QStringLiteral("identifier"), QStringLiteral("A valid publication identifier is required."));
		static const QRegularExpression languagePattern(QStringLiteral("^[A-Za-z]{2,8}(?:-[A-Za-z0-9]{1,8})*$"));
		if (!languagePattern.match(metadata.language).hasMatch())
			add(PreflightSeverity::Error, QStringLiteral("language"), QStringLiteral("Enter a valid language tag, such as en or en-US."));
		if (!metadata.author.isEmpty() && !validAltText(metadata.author))
			add(PreflightSeverity::Error, QStringLiteral("author"), QStringLiteral("The author contains invalid text."));
	}
	if (validateDestination)
	{
		const QFileInfo destination(outputPath);
		if (outputPath.isEmpty() || !QDir::isAbsolutePath(outputPath))
			add(PreflightSeverity::Error, QStringLiteral("output-path"), QStringLiteral("Choose an absolute output path."));
		else
		{
			if (destination.suffix().compare(QLatin1String("epub"), Qt::CaseInsensitive) != 0)
				add(PreflightSeverity::Error, QStringLiteral("output-extension"), QStringLiteral("The output file must have an .epub extension."));
			if (destination.exists() || destination.isSymLink())
				add(PreflightSeverity::Error, QStringLiteral("output-exists"), QStringLiteral("The output file already exists and will not be overwritten."));
			if (!destination.absoluteDir().exists() || !QFileInfo(destination.absolutePath()).isWritable())
				add(PreflightSeverity::Error, QStringLiteral("output-directory"), QStringLiteral("The output directory does not exist or is not writable."));
		}
	}
	const QVector<PageItem*> items = rankableItems(document);
	if (!items.isEmpty())
	{
		QSet<int> ranks;
		bool invalid = false;
		for (const PageItem* item : items)
		{
			const int rank = savedOrder(item);
			if ((items.size() > 1 && (rank < 1 || rank > items.size() || ranks.contains(rank))) ||
				(items.size() == 1 && rank != -1 && rank != 1))
				invalid = true;
			ranks.insert(rank);
			if (item->isImageFrame() && !report.extraction.ready())
			{
				const ImageFrameResult image = extractLinkedImageFrame(item);
				if (!image.ready())
					add(PreflightSeverity::Error, QStringLiteral("image-invalid"),
						QStringLiteral("Image frame '%1': %2").arg(item->itemName(), image.detail));
			}
			else if (item->isImageFrame())
				add(PreflightSeverity::Warning, QStringLiteral("image-fidelity"),
					savedUseImageFrameCrop(item)
						? QStringLiteral("Image '%1': exact PNG frame crop applied; page placement, effects, and colour adjustments are not reproduced.").arg(item->itemName())
						: QStringLiteral("Image '%1': original pixels only; frame crop, scale, rotation, and colour adjustments are not reproduced.").arg(item->itemName()));
		}
		if (invalid)
			add(PreflightSeverity::Error, QStringLiteral("reading-order"), QStringLiteral("Assign each story or image a unique, contiguous reading-order rank."));
	}
	for (const QString& warning : lineSpacingWarnings(document))
		add(PreflightSeverity::Warning, QStringLiteral("line-spacing"), warning);
	if (!report.extraction.ready())
	{
		bool duplicate = false;
		for (const PreflightIssue& issue : report.issues)
			duplicate |= issue.severity == PreflightSeverity::Error &&
				(issue.detail == report.extraction.detail || issue.detail.endsWith(report.extraction.detail) ||
					(issue.code == QLatin1String("object-unsupported") &&
						report.extraction.status == EpubReadingOrder::Status::UnsupportedContent));
		if (!duplicate)
			add(PreflightSeverity::Error, QStringLiteral("document"), report.extraction.detail);
	}
	return report;
}

Result extractSingleStory(const ScribusDoc& document, EpubExport::Book metadata,
	const EpubReadingOrder::ParagraphStyleMap& styles,
	const EpubReadingOrder::CharacterStyleMap& characterStyles)
{
	return extractDocument(document, metadata, {}, false, false, styles, characterStyles);
}

} // namespace EpubDocument
