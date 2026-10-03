/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#ifndef ANCHORPOSITION_H
#define ANCHORPOSITION_H

#include <QMarginsF>
#include <QPointF>
#include <QRectF>
#include <QSizeF>

#include "scribusapi.h"

/**
 * Positioning and text-wrap settings for a PageItem stored in a text story.
 *
 * The default value deliberately describes Scribus' historic inline-object
 * behaviour. This makes old SLA files and objects created by old scripts retain
 * their layout without needing a file-format migration.
 */
class SCRIBUS_API AnchorPosition
{
public:
	enum class Mode
	{
		Inline = 0,
		AboveLine = 1,
		Custom = 2
	};

	enum class HorizontalReference
	{
		AnchorCharacter = 0,
		TextColumn = 1,
		TextFrame = 2,
		Page = 3,
		Spread = 4
	};

	enum class VerticalReference
	{
		AnchorLine = 0,
		Paragraph = 1,
		TextFrame = 2,
		Page = 3
	};

	enum class HorizontalAlignment
	{
		Left = 0,
		Center = 1,
		Right = 2,
		Spine = 3,
		AwayFromSpine = 4,
		Custom = 5
	};

	enum class VerticalAlignment
	{
		Top = 0,
		Center = 1,
		Bottom = 2,
		Baseline = 3,
		Custom = 4
	};

	enum class WrapMode
	{
		None = 0,
		BoundingBox = 1,
		FrameShape = 2,
		Contour = 3,
		ImageClipPath = 4
	};

	Mode mode { Mode::Inline };
	HorizontalReference horizontalReference { HorizontalReference::AnchorCharacter };
	VerticalReference verticalReference { VerticalReference::AnchorLine };
	HorizontalAlignment horizontalAlignment { HorizontalAlignment::Left };
	VerticalAlignment verticalAlignment { VerticalAlignment::Baseline };
	WrapMode wrapMode { WrapMode::None };
	double xOffset { 0.0 };
	double yOffset { 0.0 };
	QMarginsF wrapOffsets;
	bool keepWithinBounds { false };
	bool preventManualPositioning { false };

	bool isInline() const { return mode == Mode::Inline; }
	bool hasTextWrap() const { return mode == Mode::Custom && wrapMode != WrapMode::None; }
	bool isDefault() const;

	QRectF resolvedRect(const QRectF& horizontalReferenceRect,
					const QRectF& verticalReferenceRect,
					const QPointF& anchorPoint,
					const QSizeF& objectSize,
					bool leftPage = false) const;
	QRectF wrapRect(const QRectF& objectRect) const;
	AnchorPosition movedOnCanvas(const QPointF& delta) const;
	AnchorPosition resizedOnCanvas(const QRectF& oldRect, const QRectF& newRect, bool leftPage = false) const;

	bool operator==(const AnchorPosition& other) const;
	bool operator!=(const AnchorPosition& other) const { return !(*this == other); }
};

#endif
