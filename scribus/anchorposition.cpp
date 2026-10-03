/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include "anchorposition.h"

#include <QtGlobal>

namespace
{
bool nearlyEqual(double lhs, double rhs)
{
	return qAbs(lhs - rhs) <= 0.000001;
}
}

bool AnchorPosition::isDefault() const
{
	return *this == AnchorPosition();
}

QRectF AnchorPosition::resolvedRect(const QRectF& horizontalReferenceRect,
									const QRectF& verticalReferenceRect,
									const QPointF& anchorPoint,
									const QSizeF& objectSize,
									bool leftPage) const
{
	if (mode == Mode::Inline)
		return QRectF(anchorPoint, objectSize);

	double x = anchorPoint.x();
	double y = anchorPoint.y();

	HorizontalAlignment effectiveHorizontalAlignment = horizontalAlignment;
	if (horizontalAlignment == HorizontalAlignment::Spine)
		effectiveHorizontalAlignment = leftPage ? HorizontalAlignment::Right : HorizontalAlignment::Left;
	else if (horizontalAlignment == HorizontalAlignment::AwayFromSpine)
		effectiveHorizontalAlignment = leftPage ? HorizontalAlignment::Left : HorizontalAlignment::Right;

	const bool relativeToAnchor = horizontalReference == HorizontalReference::AnchorCharacter;
	const QRectF horizontalRect = relativeToAnchor
		? QRectF(anchorPoint, QSizeF(0.0, 0.0))
		: horizontalReferenceRect;
	switch (effectiveHorizontalAlignment)
	{
		case HorizontalAlignment::Left:
			x = horizontalRect.left();
			break;
		case HorizontalAlignment::Center:
			x = horizontalRect.center().x() - objectSize.width() / 2.0;
			break;
		case HorizontalAlignment::Right:
			x = horizontalRect.right() - objectSize.width();
			break;
		case HorizontalAlignment::Custom:
			x = relativeToAnchor ? anchorPoint.x() : horizontalRect.left();
			break;
		case HorizontalAlignment::Spine:
		case HorizontalAlignment::AwayFromSpine:
			break;
	}
	x += xOffset;

	if (mode == Mode::AboveLine)
		y = anchorPoint.y() - objectSize.height() - yOffset;
	else
	{
		const bool relativeToLine = verticalReference == VerticalReference::AnchorLine;
		const QRectF verticalRect = relativeToLine
			? QRectF(anchorPoint, QSizeF(0.0, 0.0))
			: verticalReferenceRect;
		switch (verticalAlignment)
		{
			case VerticalAlignment::Top:
				y = verticalRect.top();
				break;
			case VerticalAlignment::Center:
				y = verticalRect.center().y() - objectSize.height() / 2.0;
				break;
			case VerticalAlignment::Bottom:
				y = verticalRect.bottom() - objectSize.height();
				break;
			case VerticalAlignment::Baseline:
				y = anchorPoint.y() - objectSize.height();
				break;
			case VerticalAlignment::Custom:
				y = relativeToLine ? anchorPoint.y() : verticalRect.top();
				break;
		}
		y += yOffset;
	}

	QRectF result(QPointF(x, y), objectSize);
	if (keepWithinBounds)
	{
		if (result.width() <= horizontalReferenceRect.width())
			result.moveLeft(qBound(horizontalReferenceRect.left(), result.left(), horizontalReferenceRect.right() - result.width()));
		if (result.height() <= verticalReferenceRect.height())
			result.moveTop(qBound(verticalReferenceRect.top(), result.top(), verticalReferenceRect.bottom() - result.height()));
	}
	return result;
}

QRectF AnchorPosition::wrapRect(const QRectF& objectRect) const
{
	return objectRect.adjusted(-wrapOffsets.left(), -wrapOffsets.top(),
		wrapOffsets.right(), wrapOffsets.bottom());
}

AnchorPosition AnchorPosition::movedOnCanvas(const QPointF& delta) const
{
	AnchorPosition result(*this);
	if (mode == Mode::Inline)
	{
		// The inline object's bottom-left is the anchor point in the text line.
		// Convert it to a movable object without changing its initial position.
		result.mode = Mode::Custom;
		result.horizontalReference = HorizontalReference::AnchorCharacter;
		result.horizontalAlignment = HorizontalAlignment::Custom;
		result.verticalReference = VerticalReference::AnchorLine;
		result.verticalAlignment = VerticalAlignment::Baseline;
	}
	else if (mode == Mode::AboveLine)
	{
		// Above-line Y offsets run upwards; custom baseline offsets run downwards.
		result.mode = Mode::Custom;
		result.verticalReference = VerticalReference::AnchorLine;
		result.verticalAlignment = VerticalAlignment::Baseline;
		result.yOffset = -yOffset;
	}
	result.xOffset += delta.x();
	result.yOffset += delta.y();
	return result;
}

AnchorPosition AnchorPosition::resizedOnCanvas(const QRectF& oldRect, const QRectF& newRect, bool leftPage) const
{
	AnchorPosition result(*this);
	const double widthChange = newRect.width() - oldRect.width();
	const double heightChange = newRect.height() - oldRect.height();
	if (mode == Mode::Inline)
	{
		result.mode = Mode::Custom;
		result.horizontalReference = HorizontalReference::AnchorCharacter;
		result.horizontalAlignment = HorizontalAlignment::Custom;
		result.verticalReference = VerticalReference::AnchorLine;
		result.verticalAlignment = VerticalAlignment::Baseline;
		result.xOffset = newRect.left() - oldRect.left();
		result.yOffset = newRect.top() - (oldRect.bottom() - newRect.height());
		return result;
	}

	HorizontalAlignment effectiveHorizontalAlignment = horizontalAlignment;
	if (horizontalAlignment == HorizontalAlignment::Spine)
		effectiveHorizontalAlignment = leftPage ? HorizontalAlignment::Right : HorizontalAlignment::Left;
	else if (horizontalAlignment == HorizontalAlignment::AwayFromSpine)
		effectiveHorizontalAlignment = leftPage ? HorizontalAlignment::Left : HorizontalAlignment::Right;
	result.xOffset += newRect.left() - oldRect.left();
	if (effectiveHorizontalAlignment == HorizontalAlignment::Center)
		result.xOffset += widthChange / 2.0;
	else if (effectiveHorizontalAlignment == HorizontalAlignment::Right)
		result.xOffset += widthChange;

	if (mode == Mode::AboveLine)
	{
		result.mode = Mode::Custom;
		result.verticalReference = VerticalReference::AnchorLine;
		result.verticalAlignment = VerticalAlignment::Baseline;
		result.yOffset = -yOffset;
	}
	result.yOffset += newRect.top() - oldRect.top();
	if (result.verticalAlignment == VerticalAlignment::Center)
		result.yOffset += heightChange / 2.0;
	else if (result.verticalAlignment == VerticalAlignment::Bottom
		|| result.verticalAlignment == VerticalAlignment::Baseline)
		result.yOffset += heightChange;
	return result;
}

bool AnchorPosition::operator==(const AnchorPosition& other) const
{
	return mode == other.mode
		&& horizontalReference == other.horizontalReference
		&& verticalReference == other.verticalReference
		&& horizontalAlignment == other.horizontalAlignment
		&& verticalAlignment == other.verticalAlignment
		&& wrapMode == other.wrapMode
		&& nearlyEqual(xOffset, other.xOffset)
		&& nearlyEqual(yOffset, other.yOffset)
		&& nearlyEqual(wrapOffsets.left(), other.wrapOffsets.left())
		&& nearlyEqual(wrapOffsets.top(), other.wrapOffsets.top())
		&& nearlyEqual(wrapOffsets.right(), other.wrapOffsets.right())
		&& nearlyEqual(wrapOffsets.bottom(), other.wrapOffsets.bottom())
		&& keepWithinBounds == other.keepWithinBounds
		&& preventManualPositioning == other.preventManualPositioning;
}
