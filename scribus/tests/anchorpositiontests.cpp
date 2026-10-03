/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */

#include <QtTest>

#include "anchorposition.h"

class AnchorPositionTests : public QObject
{
	Q_OBJECT

private slots:
	void defaultsPreserveLegacyInlineBehavior();
	void resolvesColumnAlignmentAndOffsets();
	void resolvesSpineRelativeAlignment();
	void resolvesAboveLinePlacement();
	void expandsWrapBoundsIndependently();
	void keepsObjectInsideReferenceBounds();
	void movesInlineAndAboveLineWithoutLosingAnchor();
	void resizesAlignedAnchorsWithoutJumping();
};

void AnchorPositionTests::defaultsPreserveLegacyInlineBehavior()
{
	AnchorPosition anchor;
	QVERIFY(anchor.isDefault());
	QVERIFY(anchor.isInline());
	QVERIFY(!anchor.hasTextWrap());
	QCOMPARE(anchor.resolvedRect(QRectF(), QRectF(), QPointF(12.0, 34.0), QSizeF(20.0, 10.0)),
		QRectF(12.0, 34.0, 20.0, 10.0));
}

void AnchorPositionTests::resolvesColumnAlignmentAndOffsets()
{
	AnchorPosition anchor;
	anchor.mode = AnchorPosition::Mode::Custom;
	anchor.horizontalReference = AnchorPosition::HorizontalReference::TextColumn;
	anchor.horizontalAlignment = AnchorPosition::HorizontalAlignment::Center;
	anchor.verticalReference = AnchorPosition::VerticalReference::TextFrame;
	anchor.verticalAlignment = AnchorPosition::VerticalAlignment::Top;
	anchor.xOffset = 3.0;
	anchor.yOffset = 5.0;

	QCOMPARE(anchor.resolvedRect(QRectF(10.0, 0.0, 100.0, 200.0),
		QRectF(0.0, 20.0, 100.0, 200.0), QPointF(40.0, 50.0), QSizeF(20.0, 10.0)),
		QRectF(53.0, 25.0, 20.0, 10.0));
}

void AnchorPositionTests::resolvesSpineRelativeAlignment()
{
	AnchorPosition anchor;
	anchor.mode = AnchorPosition::Mode::Custom;
	anchor.horizontalReference = AnchorPosition::HorizontalReference::Page;
	anchor.horizontalAlignment = AnchorPosition::HorizontalAlignment::Spine;
	anchor.verticalAlignment = AnchorPosition::VerticalAlignment::Baseline;

	QRectF reference(10.0, 0.0, 100.0, 200.0);
	QCOMPARE(anchor.resolvedRect(reference, reference, QPointF(20.0, 30.0), QSizeF(20.0, 10.0), true).left(), 90.0);
	QCOMPARE(anchor.resolvedRect(reference, reference, QPointF(20.0, 30.0), QSizeF(20.0, 10.0), false).left(), 10.0);
}

void AnchorPositionTests::resolvesAboveLinePlacement()
{
	AnchorPosition anchor;
	anchor.mode = AnchorPosition::Mode::AboveLine;
	anchor.yOffset = 4.0;

	QCOMPARE(anchor.resolvedRect(QRectF(), QRectF(), QPointF(25.0, 50.0), QSizeF(20.0, 10.0)),
		QRectF(25.0, 36.0, 20.0, 10.0));
}

void AnchorPositionTests::expandsWrapBoundsIndependently()
{
	AnchorPosition anchor;
	anchor.mode = AnchorPosition::Mode::Custom;
	anchor.wrapMode = AnchorPosition::WrapMode::BoundingBox;
	anchor.wrapOffsets = QMarginsF(1.0, 2.0, 3.0, 4.0);

	QVERIFY(anchor.hasTextWrap());
	QCOMPARE(anchor.wrapRect(QRectF(10.0, 20.0, 30.0, 40.0)), QRectF(9.0, 18.0, 34.0, 46.0));
}

void AnchorPositionTests::keepsObjectInsideReferenceBounds()
{
	AnchorPosition anchor;
	anchor.mode = AnchorPosition::Mode::Custom;
	anchor.horizontalReference = AnchorPosition::HorizontalReference::TextFrame;
	anchor.horizontalAlignment = AnchorPosition::HorizontalAlignment::Custom;
	anchor.verticalReference = AnchorPosition::VerticalReference::TextFrame;
	anchor.verticalAlignment = AnchorPosition::VerticalAlignment::Custom;
	anchor.xOffset = 95.0;
	anchor.yOffset = -25.0;
	anchor.keepWithinBounds = true;

	QRectF bounds(10.0, 20.0, 100.0, 80.0);
	QCOMPARE(anchor.resolvedRect(bounds, bounds, QPointF(), QSizeF(30.0, 20.0)),
		QRectF(80.0, 20.0, 30.0, 20.0));
}

void AnchorPositionTests::movesInlineAndAboveLineWithoutLosingAnchor()
{
	AnchorPosition inlineAnchor;
	const AnchorPosition movedInline = inlineAnchor.movedOnCanvas(QPointF(12.0, -7.0));
	QCOMPARE(movedInline.mode, AnchorPosition::Mode::Custom);
	QCOMPARE(movedInline.resolvedRect(QRectF(), QRectF(), QPointF(25.0, 50.0), QSizeF(20.0, 10.0)),
		QRectF(37.0, 33.0, 20.0, 10.0));

	AnchorPosition aboveLine;
	aboveLine.mode = AnchorPosition::Mode::AboveLine;
	aboveLine.yOffset = 4.0;
	const AnchorPosition movedAbove = aboveLine.movedOnCanvas(QPointF(-3.0, 8.0));
	QCOMPARE(movedAbove.mode, AnchorPosition::Mode::Custom);
	QCOMPARE(movedAbove.resolvedRect(QRectF(), QRectF(), QPointF(25.0, 50.0), QSizeF(20.0, 10.0)),
		QRectF(22.0, 44.0, 20.0, 10.0));
}

void AnchorPositionTests::resizesAlignedAnchorsWithoutJumping()
{
	AnchorPosition anchor;
	anchor.mode = AnchorPosition::Mode::Custom;
	anchor.horizontalReference = AnchorPosition::HorizontalReference::TextColumn;
	anchor.horizontalAlignment = AnchorPosition::HorizontalAlignment::Right;
	anchor.verticalReference = AnchorPosition::VerticalReference::TextFrame;
	anchor.verticalAlignment = AnchorPosition::VerticalAlignment::Center;
	const QRectF horizontal(10.0, 0.0, 100.0, 100.0);
	const QRectF vertical(0.0, 20.0, 100.0, 80.0);
	const QRectF before = anchor.resolvedRect(horizontal, vertical, QPointF(25.0, 50.0), QSizeF(20.0, 10.0));
	const QRectF target(before.left() - 5.0, before.top() - 4.0, 30.0, 16.0);
	const AnchorPosition resized = anchor.resizedOnCanvas(before, target);
	QCOMPARE(resized.resolvedRect(horizontal, vertical, QPointF(25.0, 50.0), target.size()), target);

	AnchorPosition inlineAnchor;
	const QRectF inlineBefore(25.0, 40.0, 20.0, 10.0);
	const QRectF inlineTarget(19.0, 35.0, 26.0, 15.0);
	const AnchorPosition resizedInline = inlineAnchor.resizedOnCanvas(inlineBefore, inlineTarget);
	QCOMPARE(resizedInline.resolvedRect(QRectF(), QRectF(), QPointF(25.0, 50.0), inlineTarget.size()),
		inlineTarget);
}

QTEST_APPLESS_MAIN(AnchorPositionTests)

#include "anchorpositiontests.moc"
