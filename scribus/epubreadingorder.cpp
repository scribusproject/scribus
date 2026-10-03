/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "epubreadingorder.h"

#include <QHash>
#include <QSet>

#include <algorithm>
#include <cmath>
#include <functional>

namespace EpubReadingOrder
{
namespace
{
bool validImageAltText(const QString& value)
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
}

ContentScan scanContent(const QVector<ContentNode>& nodes, const QVector<int>& topLevelIndices)
{
	ContentScan result;
	QSet<int> visited;
	QSet<int> active;
	std::function<bool(int)> visit = [&](int index) -> bool
	{
		if (index < 0 || index >= nodes.size())
		{
			result.status = Status::InvalidChain;
			result.detail = QStringLiteral("A group references a missing document item.");
			return false;
		}
		if (active.contains(index))
		{
			result.status = Status::InvalidChain;
			result.detail = QStringLiteral("The document item hierarchy contains a cycle.");
			return false;
		}
		if (visited.contains(index))
			return true;
		visited.insert(index);
		const ContentNode& node = nodes.at(index);
		if (!node.included)
			return true;
		if (node.kind == ContentKind::TextFrame)
			result.textFrameIndices.append(index);
		else if (node.kind == ContentKind::ImageFrame)
			result.imageFrameIndices.append(index);
		else if (node.kind == ContentKind::Unsupported)
		{
			result.status = Status::UnsupportedContent;
			result.detail = QStringLiteral("The document contains non-text content that EPUB extraction cannot preserve yet.");
			result.unsupportedIndices.append(index);
		}
		if (node.kind == ContentKind::Group)
		{
			active.insert(index);
			for (int child : node.children)
			{
				if (!visit(child))
					return false;
			}
			active.remove(index);
		}
		return true;
	};
	for (int root : topLevelIndices)
	{
		if (!visit(root))
			break;
	}
	return result;
}

Result extractStories(const QVector<Frame>& frames, const QVector<int>& orderedRootIds,
	const ParagraphStyleMap& styles, const CharacterStyleMap& characterStyles)
{
	if (frames.isEmpty())
		return { Status::Empty, QStringLiteral("No text frames are available for EPUB reading order."), {} };

	QHash<int, const Frame*> byId;
	for (const Frame& frame : frames)
	{
		if (frame.id < 0 || byId.contains(frame.id) || frame.id == frame.previousId || frame.id == frame.nextId)
			return { Status::InvalidChain, QStringLiteral("The text-frame chain has invalid or duplicate links."), {} };
		byId.insert(frame.id, &frame);
	}

	QVector<const Frame*> roots;
	for (const Frame& frame : frames)
	{
		if (frame.unsupportedContent)
			return { Status::UnsupportedContent, frame.unsupportedDetail.isEmpty()
				? QStringLiteral("A text frame contains content this EPUB slice cannot preserve.")
				: frame.unsupportedDetail, {} };
		if (frame.previousId == -1)
			roots.append(&frame);
		else if (!byId.contains(frame.previousId) || byId.value(frame.previousId)->nextId != frame.id)
			return { Status::InvalidChain, QStringLiteral("A text-frame link is missing or asymmetric."), {} };
		if (frame.nextId != -1 && (!byId.contains(frame.nextId) || byId.value(frame.nextId)->previousId != frame.id))
			return { Status::InvalidChain, QStringLiteral("A text-frame link is missing or asymmetric."), {} };
	}
	if (roots.isEmpty())
		return { Status::InvalidChain, QStringLiteral("The text-frame chain has no starting frame."), {} };

	QSet<int> visited;
	for (const Frame* root : roots)
	{
		for (const Frame* frame = root; frame; frame = byId.value(frame->nextId, nullptr))
		{
			if (visited.contains(frame->id))
				return { Status::InvalidChain, QStringLiteral("The text-frame chain contains a cycle or shared frame."), {} };
			visited.insert(frame->id);
		}
	}
	if (visited.size() != frames.size())
		return { Status::InvalidChain, QStringLiteral("The text-frame chain has disconnected frames."), {} };
	if (orderedRootIds.isEmpty() && roots.size() != 1)
		return { Status::Ambiguous, QStringLiteral("Multiple independent text stories need an explicit reading order."), {} };

	QVector<const Frame*> orderedRoots;
	if (orderedRootIds.isEmpty())
		orderedRoots = roots;
	else
	{
		if (orderedRootIds.size() != roots.size())
			return { Status::InvalidOrder, QStringLiteral("The EPUB reading order must include every story once."), {} };
		QSet<int> selected;
		for (int id : orderedRootIds)
		{
			const Frame* root = byId.value(id, nullptr);
			if (!root || root->previousId != -1 || selected.contains(id))
				return { Status::InvalidOrder, QStringLiteral("The EPUB reading order contains an unknown, repeated, or non-root frame."), {} };
			selected.insert(id);
			orderedRoots.append(root);
		}
	}

	Result result { Status::Ready, {}, {} };
	for (const Frame* root : orderedRoots)
	{
		if (root->rootText.isEmpty())
			continue;
		const QStringList paragraphs = root->rootText.split(QLatin1Char('\n'), Qt::KeepEmptyParts);
		if (!root->paragraphStyleNames.isEmpty() && root->paragraphStyleNames.size() != paragraphs.size())
			return { Status::UnsupportedContent, QStringLiteral("Paragraph style data does not match the extracted text."), {} };
		if (!root->bulletParagraphs.isEmpty() && root->bulletParagraphs.size() != paragraphs.size())
			return { Status::UnsupportedContent, QStringLiteral("List data does not match the extracted text."), {} };
		if (!root->bulletParagraphLevels.isEmpty() && root->bulletParagraphLevels.size() != paragraphs.size())
			return { Status::UnsupportedContent, QStringLiteral("List-depth data does not match the extracted text."), {} };
		if (!root->orderedParagraphStarts.isEmpty() && root->orderedParagraphStarts.size() != paragraphs.size())
			return { Status::UnsupportedContent, QStringLiteral("Numbering data does not match the extracted text."), {} };
		if (!root->orderedParagraphStyles.isEmpty() && root->orderedParagraphStyles.size() != paragraphs.size())
			return { Status::UnsupportedContent, QStringLiteral("Numbering style data does not match the extracted text."), {} };
		if (!root->paragraphDirections.isEmpty() && root->paragraphDirections.size() != paragraphs.size())
			return { Status::UnsupportedContent, QStringLiteral("Paragraph direction data does not match the extracted text."), {} };
		if (!root->paragraphAlignments.isEmpty() && root->paragraphAlignments.size() != paragraphs.size())
			return { Status::UnsupportedContent, QStringLiteral("Paragraph alignment data does not match the extracted text."), {} };
		if (!root->paragraphMetrics.isEmpty() && root->paragraphMetrics.size() != paragraphs.size())
			return { Status::UnsupportedContent, QStringLiteral("Paragraph metric data does not match the extracted text."), {} };
		if (!root->characterStyleNames.isEmpty() && root->characterStyleNames.size() != root->rootText.size())
			return { Status::UnsupportedContent, QStringLiteral("Character style data does not match the extracted text."), {} };
		int textOffset = 0;
		EpubExport::BlockKind previousKind = EpubExport::BlockKind::Paragraph;
		int previousListLevel = 0;
		int previousOrderedStart = 1;
		EpubExport::OrderedStyle previousOrderedStyle = EpubExport::OrderedStyle::Decimal;
		int orderedRunLength = 0;
		for (int index = 0; index < paragraphs.size(); ++index)
		{
			const QString styleName = root->paragraphStyleNames.isEmpty()
				? QString() : root->paragraphStyleNames.at(index);
			if (!styleName.isEmpty() && !styles.contains(styleName))
				return { Status::UnsupportedContent,
					QStringLiteral("Paragraph style '%1' needs an explicit EPUB semantic mapping.").arg(styleName), {} };
			const int level = styleName.isEmpty() ? 0 : styles.value(styleName);
			const bool bullet = !root->bulletParagraphs.isEmpty() && root->bulletParagraphs.at(index);
			const int bulletLevel = root->bulletParagraphLevels.isEmpty() ? 0 : root->bulletParagraphLevels.at(index);
			const int orderedStart = root->orderedParagraphStarts.isEmpty() ? 0
				: root->orderedParagraphStarts.at(index);
			const EpubExport::OrderedStyle listStyle = root->orderedParagraphStyles.isEmpty()
				? EpubExport::OrderedStyle::Decimal : root->orderedParagraphStyles.at(index);
			if (level < 0 || level > 6 || (level > 0 && paragraphs.at(index).trimmed().isEmpty()))
				return { Status::UnsupportedContent,
					QStringLiteral("The EPUB semantic mapping for paragraph style '%1' is invalid here.").arg(styleName), {} };
			if ((bullet || orderedStart != 0) && ((bullet && orderedStart != 0) || orderedStart < 0 ||
				level != 0 || paragraphs.at(index).trimmed().isEmpty()))
				return { Status::UnsupportedContent, QStringLiteral("A list item must have valid metadata and non-empty paragraph text."), {} };
			if (bulletLevel < 0 || bulletLevel > 1 || (!bullet && bulletLevel != 0) ||
				(bulletLevel == 1 && previousKind != EpubExport::BlockKind::BulletItem))
				return { Status::UnsupportedContent, QStringLiteral("A nested bullet needs a preceding parent bullet and may be only one level deep."), {} };
			if (listStyle != EpubExport::OrderedStyle::Decimal &&
				listStyle != EpubExport::OrderedStyle::LowerRoman &&
				listStyle != EpubExport::OrderedStyle::UpperRoman &&
				listStyle != EpubExport::OrderedStyle::LowerAlpha &&
				listStyle != EpubExport::OrderedStyle::UpperAlpha)
				return { Status::UnsupportedContent, QStringLiteral("The EPUB ordered-list style is invalid."), {} };
			if (orderedStart == 0 && listStyle != EpubExport::OrderedStyle::Decimal)
				return { Status::UnsupportedContent, QStringLiteral("A non-decimal list style has no ordered-list item."), {} };
			if (orderedStart > 0 && previousKind == EpubExport::BlockKind::OrderedItem &&
				previousOrderedStyle == listStyle && previousOrderedStart != orderedStart)
				return { Status::UnsupportedContent, QStringLiteral("A local ordered list changes its configured start within one run."), {} };
			EpubExport::Block block { bullet ? EpubExport::BlockKind::BulletItem
				: orderedStart > 0 ? EpubExport::BlockKind::OrderedItem
				: level > 0 ? EpubExport::BlockKind::Heading : EpubExport::BlockKind::Paragraph,
				paragraphs.at(index), level };
			block.startsList = (bullet || orderedStart > 0) &&
				(block.kind != previousKind || (bullet && bulletLevel > previousListLevel) ||
					(orderedStart > 0 && listStyle != previousOrderedStyle));
			block.listLevel = bulletLevel;
			block.listStart = orderedStart > 0 ? orderedStart : 1;
			block.orderedStyle = orderedStart > 0 ? listStyle : EpubExport::OrderedStyle::Decimal;
			block.direction = root->paragraphDirections.isEmpty()
				? EpubExport::TextDirection::Ltr : root->paragraphDirections.at(index);
			if (block.direction != EpubExport::TextDirection::Ltr &&
				block.direction != EpubExport::TextDirection::Rtl)
				return { Status::UnsupportedContent, QStringLiteral("The EPUB paragraph direction is invalid."), {} };
			block.alignment = root->paragraphAlignments.isEmpty()
				? EpubExport::TextAlignment::Left : root->paragraphAlignments.at(index);
			if (block.alignment != EpubExport::TextAlignment::Left &&
				block.alignment != EpubExport::TextAlignment::Center &&
				block.alignment != EpubExport::TextAlignment::Right &&
				block.alignment != EpubExport::TextAlignment::Justify)
				return { Status::UnsupportedContent, QStringLiteral("The EPUB paragraph alignment is invalid."), {} };
			block.metrics = root->paragraphMetrics.isEmpty()
				? EpubExport::ParagraphMetrics {} : root->paragraphMetrics.at(index);
			const auto validDistance = [](double value) { return std::isfinite(value) && std::abs(value) <= 10000.0; };
			if (!validDistance(block.metrics.leftIndent) || !validDistance(block.metrics.rightIndent) ||
				!validDistance(block.metrics.firstLineIndent) || !validDistance(block.metrics.spaceBefore) ||
				!validDistance(block.metrics.spaceAfter) ||
				((bullet || orderedStart > 0) && !block.metrics.isDefault()))
				return { Status::UnsupportedContent, QStringLiteral("The EPUB paragraph has unsupported spacing or indent values."), {} };
			orderedRunLength = orderedStart > 0 ? (block.startsList ? 1 : orderedRunLength + 1) : 0;
			if (orderedStart > 0 && listStyle != EpubExport::OrderedStyle::Decimal &&
				orderedStart > ((listStyle == EpubExport::OrderedStyle::LowerAlpha ||
					listStyle == EpubExport::OrderedStyle::UpperAlpha ? 27 : 4000) - orderedRunLength))
				return { Status::UnsupportedContent, QStringLiteral("Roman EPUB lists must stay within 1–3999 and alphabetic lists within A–Z."), {} };
			bool hasInlineSemantics = false;
			for (int position = 0; position < block.text.size() && !root->characterStyleNames.isEmpty(); ++position)
			{
				const QString charStyle = root->characterStyleNames.at(textOffset + position);
				if (!charStyle.isEmpty() && !characterStyles.contains(charStyle))
					return { Status::UnsupportedContent,
						QStringLiteral("Character style '%1' needs an explicit EPUB semantic mapping.").arg(charStyle), {} };
				const EpubExport::InlineKind kind = charStyle.isEmpty()
					? EpubExport::InlineKind::Plain : characterStyles.value(charStyle);
				if (kind != EpubExport::InlineKind::Plain && kind != EpubExport::InlineKind::Emphasis &&
					kind != EpubExport::InlineKind::Strong)
					return { Status::UnsupportedContent, QStringLiteral("The EPUB character style mapping is invalid."), {} };
				hasInlineSemantics |= kind != EpubExport::InlineKind::Plain;
				if (block.runs.isEmpty() || block.runs.last().kind != kind)
					block.runs.append({ kind, {} });
				block.runs.last().text.append(block.text.at(position));
			}
			if (!hasInlineSemantics)
				block.runs.clear();
			result.blocks.append(block);
			previousKind = block.kind;
			previousListLevel = block.listLevel;
			previousOrderedStart = block.listStart;
			previousOrderedStyle = block.orderedStyle;
			textOffset += block.text.size() + 1; // Skip the paragraph separator, if present.
		}
	}
	if (result.blocks.isEmpty() || std::all_of(result.blocks.cbegin(), result.blocks.cend(),
		[](const EpubExport::Block& block) { return block.text.trimmed().isEmpty(); }))
		return { Status::Empty, QStringLiteral("The ordered stories have no text."), {} };
	return result;
}

Result extractSingleStory(const QVector<Frame>& frames, const ParagraphStyleMap& styles,
	const CharacterStyleMap& characterStyles)
{
	return extractStories(frames, {}, styles, characterStyles);
}

Result extractSavedOrder(const QVector<Frame>& frames, const ParagraphStyleMap& styles,
	const CharacterStyleMap& characterStyles)
{
	QHash<int, int> rootIdByRank;
	int rootCount = 0;
	for (const Frame& frame : frames)
	{
		if (frame.previousId == -1)
			++rootCount;
		if (frame.savedOrder == -1)
			continue;
		if (frame.savedOrder < 1 || frame.previousId != -1 || rootIdByRank.contains(frame.savedOrder))
			return { Status::InvalidOrder, QStringLiteral("Saved EPUB ranks must be positive, unique, and placed on story roots."), {} };
		rootIdByRank.insert(frame.savedOrder, frame.id);
	}
	if (rootIdByRank.isEmpty())
		return extractSingleStory(frames, styles, characterStyles);
	if (rootIdByRank.size() != rootCount)
		return { Status::InvalidOrder, QStringLiteral("Every text story needs a saved EPUB rank."), {} };
	QVector<int> orderedRootIds;
	for (int rank = 1; rank <= rootCount; ++rank)
	{
		if (!rootIdByRank.contains(rank))
			return { Status::InvalidOrder, QStringLiteral("Saved EPUB ranks must be contiguous from 1."), {} };
		orderedRootIds.append(rootIdByRank.value(rank));
	}
	return extractStories(frames, orderedRootIds, styles, characterStyles);
}

Result extractMixedSavedOrder(const QVector<Frame>& frames, const QVector<Image>& images,
	const ParagraphStyleMap& styles, const CharacterStyleMap& characterStyles)
{
	struct Unit
	{
		int rank { -1 };
		const Frame* story { nullptr };
		const Image* image { nullptr };
	};
	QVector<Unit> units;
	for (const Frame& frame : frames)
	{
		if (frame.previousId == -1)
			units.append({ frame.savedOrder, &frame, nullptr });
		else if (frame.savedOrder != -1)
			return { Status::InvalidOrder, QStringLiteral("An EPUB rank is stored on a non-root text frame."), {} };
	}
	QSet<int> imageIds;
	for (const Image& image : images)
	{
		if (image.id < 0 || imageIds.contains(image.id))
			return { Status::InvalidOrder, QStringLiteral("EPUB image frames need distinct valid identifiers."), {} };
		imageIds.insert(image.id);
		if ((!image.decorative && !validImageAltText(image.altText)) ||
			(image.decorative && (!image.altText.isEmpty() || !image.caption.isEmpty())) ||
			image.widthPercent < 0 || image.widthPercent > 100 ||
			(image.captionAlignment != EpubExport::TextAlignment::Left &&
				image.captionAlignment != EpubExport::TextAlignment::Center &&
				image.captionAlignment != EpubExport::TextAlignment::Right &&
				image.captionAlignment != EpubExport::TextAlignment::Justify) ||
			(image.caption.isEmpty() && image.captionAlignment != EpubExport::TextAlignment::Left) ||
			(!image.caption.isEmpty() && !validImageAltText(image.caption)) ||
			image.asset.data.isEmpty() ||
			(image.asset.mediaType != QLatin1String("image/png") &&
				image.asset.mediaType != QLatin1String("image/jpeg")) ||
			(image.direction != EpubExport::TextDirection::Ltr &&
				image.direction != EpubExport::TextDirection::Rtl) ||
			(image.alignment != EpubExport::TextAlignment::Left &&
				image.alignment != EpubExport::TextAlignment::Center &&
				image.alignment != EpubExport::TextAlignment::Right &&
				image.alignment != EpubExport::TextAlignment::Justify))
			return { Status::UnsupportedContent, QStringLiteral("An EPUB image has missing or unsupported content."), {} };
		units.append({ image.savedOrder, nullptr, &image });
	}
	if (units.isEmpty())
		return { Status::Empty, QStringLiteral("No text stories or image frames are available."), {} };
	if (units.size() == 1 && units.first().rank == -1)
		units.first().rank = 1;
	QSet<int> ranks;
	for (const Unit& unit : units)
	{
		if (unit.rank < 1 || unit.rank > units.size() || ranks.contains(unit.rank))
			return { Status::InvalidOrder,
				QStringLiteral("Text stories and images need unique, contiguous EPUB ranks from 1."), {} };
		ranks.insert(unit.rank);
	}
	std::sort(units.begin(), units.end(), [](const Unit& a, const Unit& b) { return a.rank < b.rank; });

	QVector<int> orderedStoryIds;
	QHash<int, const Frame*> frameById;
	for (const Frame& frame : frames)
		frameById.insert(frame.id, &frame);
	for (const Unit& unit : units)
	{
		if (unit.story)
			orderedStoryIds.append(unit.story->id);
	}
	if (!frames.isEmpty())
	{
		const Result validation = extractStories(frames, orderedStoryIds, styles, characterStyles);
		if (!validation.ready() && !(validation.status == Status::Empty && !images.isEmpty()))
			return validation;
	}

	Result result { Status::Ready, {}, {}, {} };
	for (const Unit& unit : units)
	{
		if (unit.image)
		{
			EpubExport::Block block;
			block.kind = EpubExport::BlockKind::Image;
			block.text = unit.image->altText;
			block.direction = unit.image->direction;
			block.alignment = unit.image->alignment;
			block.caption = unit.image->caption;
			block.decorative = unit.image->decorative;
			block.imageWidthPercent = unit.image->widthPercent;
			block.captionAlignment = unit.image->captionAlignment;
			block.imageIndex = result.images.size();
			result.images.append(unit.image->asset);
			result.blocks.append(block);
			continue;
		}
		QVector<Frame> chain;
		for (const Frame* frame = unit.story; frame; frame = frameById.value(frame->nextId, nullptr))
			chain.append(*frame);
		const Result story = extractSingleStory(chain, styles, characterStyles);
		if (story.status == Status::Empty)
			continue;
		if (!story.ready())
			return story;
		result.blocks += story.blocks;
	}
	if (result.blocks.isEmpty())
		return { Status::Empty, QStringLiteral("The ordered content is empty."), {} };
	return result;
}

} // namespace EpubReadingOrder
