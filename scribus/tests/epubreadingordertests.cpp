/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "epubreadingorder.h"

#include <QtTest>

#include <limits>

class EpubReadingOrderTests : public QObject
{
	Q_OBJECT

private slots:
	void oneLinkedStoryIsReadOnce();
	void explicitOrderPlacesIndependentStories();
	void rejectsAmbiguousOrBrokenOrder();
	void rejectsIncompleteOrInvalidExplicitOrder();
	void savedRanksSurviveFrameListReordering();
	void rejectsInvalidSavedRanks();
	void scansNestedGroupsWithoutDuplicatingFrames();
	void rejectsMalformedOrUnsupportedHierarchy();
	void rejectsUnsupportedAndEmptyContent();
	void mapsNamedParagraphStylesOnlyWhenExplicit();
	void mapsCharacterRunsOnlyWhenExplicit();
	void groupsFlatBulletItemsWithoutCrossingStories();
	void nestsOneLevelOfStandardBullets();
	void groupsLocalDecimalListsAndPreservesStart();
	void keepsRomanRunsSeparateAndBounded();
	void keepsAlphabeticRunsSeparateAndBounded();
	void preservesParagraphDirections();
	void preservesParagraphAlignments();
	void preservesParagraphMetrics();
	void interleavesRankedStoriesAndImages();
	void permitsOneUnrankedImage();
	void rejectsIncompleteMixedOrderAndBadImages();
};

void EpubReadingOrderTests::oneLinkedStoryIsReadOnce()
{
	using namespace EpubReadingOrder;
	// Deliberately shuffled frame-list order: chain links, not list order, win.
	const QVector<Frame> frames {
		{ 2, 1, -1, QStringLiteral("duplicate view must not be read"), false },
		{ 0, -1, 1, QStringLiteral("First paragraph\nతెలుగు & English\nThird"), false },
		{ 1, 0, 2, QStringLiteral("duplicate view must not be read"), false }
	};
	const Result result = extractSingleStory(frames);
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 3);
	QCOMPARE(result.blocks.at(0).text, QStringLiteral("First paragraph"));
	QCOMPARE(result.blocks.at(1).text, QStringLiteral("తెలుగు & English"));
	QCOMPARE(result.blocks.at(2).text, QStringLiteral("Third"));
	for (const auto& block : result.blocks)
	{
		QCOMPARE(block.kind, EpubExport::BlockKind::Paragraph);
		QCOMPARE(block.headingLevel, 0);
	}
}

void EpubReadingOrderTests::explicitOrderPlacesIndependentStories()
{
	using namespace EpubReadingOrder;
	// Canvas order is B, A-next, A-root. Explicit roots choose A then B.
	const QVector<Frame> frames {
		{ 20, -1, -1, QStringLiteral("Second story"), false },
		{ 11, 10, -1, QStringLiteral("repeated view"), false },
		{ 10, -1, 11, QStringLiteral("First story\ncontinued"), false }
	};
	const Result result = extractStories(frames, { 10, 20 });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 3);
	QCOMPARE(result.blocks.at(0).text, QStringLiteral("First story"));
	QCOMPARE(result.blocks.at(1).text, QStringLiteral("continued"));
	QCOMPARE(result.blocks.at(2).text, QStringLiteral("Second story"));
	const Result reversed = extractStories(frames, { 20, 10 });
	QVERIFY2(reversed.ready(), qPrintable(reversed.detail));
	QCOMPARE(reversed.blocks.at(0).text, QStringLiteral("Second story"));
	QCOMPARE(reversed.blocks.at(1).text, QStringLiteral("First story"));
}

void EpubReadingOrderTests::rejectsAmbiguousOrBrokenOrder()
{
	using namespace EpubReadingOrder;
	QCOMPARE(extractSingleStory({ { 0, -1, -1, QStringLiteral("A"), false },
		{ 1, -1, -1, QStringLiteral("B"), false } }).status, Status::Ambiguous);
	QCOMPARE(extractSingleStory({ { 0, -1, 1, QStringLiteral("A"), false },
		{ 1, 0, -1, {}, false }, { 1, 0, -1, {}, false } }).status, Status::InvalidChain);
	QCOMPARE(extractSingleStory({ { 0, -1, 1, QStringLiteral("A"), false },
		{ 1, -2, -1, {}, false } }).status, Status::InvalidChain);
	QCOMPARE(extractSingleStory({ { 0, 1, 1, QStringLiteral("A"), false },
		{ 1, 0, 0, {}, false } }).status, Status::InvalidChain);
	QCOMPARE(extractSingleStory({ { 0, -1, 1, QStringLiteral("A"), false },
		{ 1, 0, 2, {}, false } }).status, Status::InvalidChain);
}

void EpubReadingOrderTests::rejectsIncompleteOrInvalidExplicitOrder()
{
	using namespace EpubReadingOrder;
	const QVector<Frame> frames {
		{ 0, -1, 1, QStringLiteral("A"), false },
		{ 1, 0, -1, {}, false },
		{ 2, -1, -1, QStringLiteral("B"), false }
	};
	QCOMPARE(extractStories(frames, {}).status, Status::Ambiguous);
	QCOMPARE(extractStories(frames, { 0 }).status, Status::InvalidOrder);
	QCOMPARE(extractStories(frames, { 0, 0 }).status, Status::InvalidOrder);
	QCOMPARE(extractStories(frames, { 0, 1 }).status, Status::InvalidOrder);
	QCOMPARE(extractStories(frames, { 0, 99 }).status, Status::InvalidOrder);
	QCOMPARE(extractStories({ { 0, -1, 1, QStringLiteral("A"), false },
		{ 1, -2, -1, {}, false }, { 2, -1, -1, QStringLiteral("B"), false } },
		{ 0, 2 }).status, Status::InvalidChain);
}

void EpubReadingOrderTests::savedRanksSurviveFrameListReordering()
{
	using namespace EpubReadingOrder;
	const QVector<Frame> frames {
		{ 7, -1, -1, QStringLiteral("Second"), false, 2 },
		{ 4, 3, -1, QStringLiteral("repeated view"), false, -1 },
		{ 3, -1, 4, QStringLiteral("First"), false, 1 }
	};
	const Result result = extractSavedOrder(frames);
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 2);
	QCOMPARE(result.blocks.at(0).text, QStringLiteral("First"));
	QCOMPARE(result.blocks.at(1).text, QStringLiteral("Second"));
	QCOMPARE(extractSavedOrder({ { 3, -1, -1, QStringLiteral("Only"), false, -1 } }).status,
		Status::Ready);
}

void EpubReadingOrderTests::preservesParagraphMetrics()
{
	using namespace EpubReadingOrder;
	Frame frame { 0, -1, -1, QStringLiteral("First\nSecond"), false };
	frame.paragraphMetrics = { { 18.0, 7.0, -4.0, 6.0, 3.0 }, {} };
	const Result result = extractSingleStory({ frame });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.at(0).metrics.leftIndent, 18.0);
	QCOMPARE(result.blocks.at(0).metrics.firstLineIndent, -4.0);
	QVERIFY(result.blocks.at(1).metrics.isDefault());
	frame.paragraphMetrics.removeLast();
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.paragraphMetrics.append(EpubExport::ParagraphMetrics {});
	frame.paragraphMetrics[0].spaceBefore = std::numeric_limits<double>::infinity();
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.paragraphMetrics[0].spaceBefore = 6.0;
	frame.bulletParagraphs = { true, false };
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
}

void EpubReadingOrderTests::interleavesRankedStoriesAndImages()
{
	using namespace EpubReadingOrder;
	const QVector<Frame> frames {
		{ 20, -1, -1, QStringLiteral("After image"), false, 3 },
		{ 11, 10, -1, QStringLiteral("linked view must not repeat"), false, -1 },
		{ 10, -1, 11, QStringLiteral("Before image\nSecond paragraph"), false, 1 }
	};
	const Image illustration { 7, 2, QStringLiteral("Blue illustration"),
		{ QByteArrayLiteral("image bytes"), QStringLiteral("image/png") },
		EpubExport::TextDirection::Rtl, EpubExport::TextAlignment::Center };
	const Result result = extractMixedSavedOrder(frames, { illustration });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 4);
	QCOMPARE(result.blocks.at(0).text, QStringLiteral("Before image"));
	QCOMPARE(result.blocks.at(1).text, QStringLiteral("Second paragraph"));
	QCOMPARE(result.blocks.at(2).kind, EpubExport::BlockKind::Image);
	QCOMPARE(result.blocks.at(2).text, QStringLiteral("Blue illustration"));
	QCOMPARE(result.blocks.at(2).imageIndex, 0);
	QCOMPARE(result.blocks.at(2).direction, EpubExport::TextDirection::Rtl);
	QCOMPARE(result.blocks.at(2).alignment, EpubExport::TextAlignment::Center);
	QCOMPARE(result.blocks.at(3).text, QStringLiteral("After image"));
	QCOMPARE(result.images.size(), 1);
	QCOMPARE(result.images.first().data, illustration.asset.data);
	// Text-only extraction must not reinterpret the new cross-type ranks.
	QCOMPARE(extractSavedOrder(frames).status, Status::InvalidOrder);

	Image second = illustration;
	second.id = 8;
	second.savedOrder = 3;
	second.altText = QStringLiteral("Second illustration");
	QVector<Frame> twoStories = frames;
	twoStories[0].savedOrder = 4;
	const Result twoImages = extractMixedSavedOrder(twoStories, { illustration, second });
	QVERIFY2(twoImages.ready(), qPrintable(twoImages.detail));
	QCOMPARE(twoImages.blocks.at(3).kind, EpubExport::BlockKind::Image);
	QCOMPARE(twoImages.blocks.at(3).imageIndex, 1);
	QCOMPARE(twoImages.images.size(), 2);
}

void EpubReadingOrderTests::permitsOneUnrankedImage()
{
	using namespace EpubReadingOrder;
	const Image image { 1, -1, QStringLiteral("A map"),
		{ QByteArrayLiteral("image bytes"), QStringLiteral("image/jpeg") },
		EpubExport::TextDirection::Ltr, EpubExport::TextAlignment::Left,
		QStringLiteral("Map of the coast") };
	const Result result = extractMixedSavedOrder({}, { image });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 1);
	QCOMPARE(result.blocks.first().kind, EpubExport::BlockKind::Image);
	QCOMPARE(result.blocks.first().imageIndex, 0);
	QCOMPARE(result.blocks.first().caption, QStringLiteral("Map of the coast"));
	Image centeredCaption = image;
	centeredCaption.captionAlignment = EpubExport::TextAlignment::Center;
	const Result centered = extractMixedSavedOrder({}, { centeredCaption });
	QVERIFY(centered.ready());
	QCOMPARE(centered.blocks.first().captionAlignment, EpubExport::TextAlignment::Center);
	centeredCaption.caption.clear();
	QCOMPARE(extractMixedSavedOrder({}, { centeredCaption }).status, Status::UnsupportedContent);
	QCOMPARE(result.images.first().mediaType, QStringLiteral("image/jpeg"));
	Image sized = image;
	sized.widthPercent = 55;
	const Result sizedResult = extractMixedSavedOrder({}, { sized });
	QVERIFY(sizedResult.ready());
	QCOMPARE(sizedResult.blocks.first().imageWidthPercent, 55);
	sized.widthPercent = 101;
	QCOMPARE(extractMixedSavedOrder({}, { sized }).status, Status::UnsupportedContent);
	Image decorative = image;
	decorative.altText.clear();
	decorative.caption.clear();
	decorative.decorative = true;
	const Result decoration = extractMixedSavedOrder({}, { decorative });
	QVERIFY2(decoration.ready(), qPrintable(decoration.detail));
	QVERIFY(decoration.blocks.first().decorative);
	QVERIFY(decoration.blocks.first().text.isEmpty());
	decorative.caption = QStringLiteral("Visible caption");
	QCOMPARE(extractMixedSavedOrder({}, { decorative }).status, Status::UnsupportedContent);
	QCOMPARE(extractMixedSavedOrder({}, {}).status, Status::Empty);
}

void EpubReadingOrderTests::rejectsIncompleteMixedOrderAndBadImages()
{
	using namespace EpubReadingOrder;
	const Frame story { 1, -1, -1, QStringLiteral("Story"), false, 1 };
	const Image image { 2, 2, QStringLiteral("Illustration"),
		{ QByteArrayLiteral("image bytes"), QStringLiteral("image/png") } };
	QCOMPARE(extractMixedSavedOrder({ story }, { image }).status, Status::Ready);
	Frame unranked = story;
	unranked.savedOrder = -1;
	QCOMPARE(extractMixedSavedOrder({ unranked }, { image }).status, Status::InvalidOrder);
	Image duplicateRank = image;
	duplicateRank.savedOrder = 1;
	QCOMPARE(extractMixedSavedOrder({ story }, { duplicateRank }).status, Status::InvalidOrder);
	Image gap = image;
	gap.savedOrder = 3;
	QCOMPARE(extractMixedSavedOrder({ story }, { gap }).status, Status::InvalidOrder);
	Image invalid = image;
	invalid.savedOrder = -2;
	QCOMPARE(extractMixedSavedOrder({ story }, { invalid }).status, Status::InvalidOrder);
	invalid = image;
	invalid.id = -1;
	QCOMPARE(extractMixedSavedOrder({ story }, { invalid }).status, Status::InvalidOrder);
	QCOMPARE(extractMixedSavedOrder({}, { image, image }).status, Status::InvalidOrder);
	invalid = image;
	invalid.altText = QStringLiteral("  ");
	QCOMPARE(extractMixedSavedOrder({ story }, { invalid }).status, Status::UnsupportedContent);
	invalid.altText = QStringLiteral("bad") + QChar::Null;
	QCOMPARE(extractMixedSavedOrder({ story }, { invalid }).status, Status::UnsupportedContent);
	invalid = image;
	invalid.caption = QStringLiteral("bad") + QChar::Null;
	QCOMPARE(extractMixedSavedOrder({ story }, { invalid }).status, Status::UnsupportedContent);
	invalid = image;
	invalid.asset.data.clear();
	QCOMPARE(extractMixedSavedOrder({ story }, { invalid }).status, Status::UnsupportedContent);
	invalid = image;
	invalid.asset.mediaType = QStringLiteral("image/tiff");
	QCOMPARE(extractMixedSavedOrder({ story }, { invalid }).status, Status::UnsupportedContent);
	invalid = image;
	invalid.direction = static_cast<EpubExport::TextDirection>(123);
	QCOMPARE(extractMixedSavedOrder({ story }, { invalid }).status, Status::UnsupportedContent);
	Frame nonRoot { 3, 1, -1, {}, false, 2 };
	Frame linkedRoot = story;
	linkedRoot.nextId = 3;
	QCOMPARE(extractMixedSavedOrder({ linkedRoot, nonRoot }, { image }).status, Status::InvalidOrder);
	nonRoot.savedOrder = -1;
	nonRoot.previousId = 999;
	QCOMPARE(extractMixedSavedOrder({ linkedRoot, nonRoot }, { image }).status, Status::InvalidChain);
}

void EpubReadingOrderTests::rejectsInvalidSavedRanks()
{
	using namespace EpubReadingOrder;
	QCOMPARE(extractSavedOrder({ { 0, -1, -1, QStringLiteral("A"), false, 1 },
		{ 1, -1, -1, QStringLiteral("B"), false, -1 } }).status, Status::InvalidOrder);
	QCOMPARE(extractSavedOrder({ { 0, -1, -1, QStringLiteral("A"), false, 1 },
		{ 1, -1, -1, QStringLiteral("B"), false, 1 } }).status, Status::InvalidOrder);
	QCOMPARE(extractSavedOrder({ { 0, -1, -1, QStringLiteral("A"), false, 1 },
		{ 1, -1, -1, QStringLiteral("B"), false, 3 } }).status, Status::InvalidOrder);
	QCOMPARE(extractSavedOrder({ { 0, -1, 1, QStringLiteral("A"), false, 1 },
		{ 1, 0, -1, {}, false, 2 } }).status, Status::InvalidOrder);
	QCOMPARE(extractSavedOrder({ { 0, -1, -1, QStringLiteral("A"), false, -2 } }).status,
		Status::InvalidOrder);
}

void EpubReadingOrderTests::scansNestedGroupsWithoutDuplicatingFrames()
{
	using namespace EpubReadingOrder;
	const QVector<ContentNode> nodes {
		{ ContentKind::Group, true, { 1, 2, 6 } },
		{ ContentKind::TextFrame, true, {} },
		{ ContentKind::Group, true, { 3, 1 } },
		{ ContentKind::TextFrame, true, {} },
		{ ContentKind::Group, false, { 5 } },
		{ ContentKind::ImageFrame, true, {} },
		{ ContentKind::ImageFrame, true, {} }
	};
	const ContentScan scan = scanContent(nodes, { 0, 1, 4 });
	QVERIFY2(scan.valid(), qPrintable(scan.detail));
	QCOMPARE(scan.textFrameIndices, (QVector<int> { 1, 3 }));
	QCOMPARE(scan.imageFrameIndices, (QVector<int> { 6 }));
	const Result ordered = extractSavedOrder({
		{ 1, -1, -1, QStringLiteral("Second"), false, 2 },
		{ 3, -1, -1, QStringLiteral("First"), false, 1 }
	});
	QVERIFY(ordered.ready());
	QCOMPARE(ordered.blocks.at(0).text, QStringLiteral("First"));
	QCOMPARE(ordered.blocks.at(1).text, QStringLiteral("Second"));
}

void EpubReadingOrderTests::rejectsMalformedOrUnsupportedHierarchy()
{
	using namespace EpubReadingOrder;
	QCOMPARE(scanContent({ { ContentKind::Group, true, { 1 } },
		{ ContentKind::Group, true, { 0 } } }, { 0 }).status, Status::InvalidChain);
	QCOMPARE(scanContent({ { ContentKind::Group, true, { 42 } } }, { 0 }).status,
		Status::InvalidChain);
	const ContentScan unsupported = scanContent({ { ContentKind::Group, true, { 1, 2 } },
		{ ContentKind::TextFrame, true, {} }, { ContentKind::Unsupported, true, {} } }, { 0 });
	QCOMPARE(unsupported.status, Status::UnsupportedContent);
	QCOMPARE(unsupported.textFrameIndices, (QVector<int> { 1 }));
	QCOMPARE(unsupported.unsupportedIndices, (QVector<int> { 2 }));
	QCOMPARE(scanContent({ { ContentKind::Group, false, { 1 } },
		{ ContentKind::Unsupported, true, {} } }, { 0 }).status, Status::Ready);
}

void EpubReadingOrderTests::rejectsUnsupportedAndEmptyContent()
{
	using namespace EpubReadingOrder;
	QCOMPARE(extractSingleStory({}).status, Status::Empty);
	QCOMPARE(extractSingleStory({ { 0, -1, -1, QStringLiteral("  "), false } }).status, Status::Empty);
	QCOMPARE(extractSingleStory({ { 0, -1, 1, QStringLiteral("A"), false },
		{ 1, 0, -1, {}, true } }).status, Status::UnsupportedContent);
	QCOMPARE(extractStories({ { 0, -1, -1, {}, false },
		{ 1, -1, -1, QStringLiteral("Body"), false } }, { 0, 1 }).status, Status::Ready);
	QCOMPARE(extractStories({ { 0, -1, -1, {}, false },
		{ 1, -1, -1, QStringLiteral("  "), false } }, { 0, 1 }).status, Status::Empty);
}

void EpubReadingOrderTests::mapsNamedParagraphStylesOnlyWhenExplicit()
{
	using namespace EpubReadingOrder;
	Frame frame { 0, -1, -1, QStringLiteral("Chapter One\nOpening paragraph"), false };
	frame.paragraphStyleNames = { QStringLiteral("ChapterTitle"), QStringLiteral("BodyCopy") };
	const Result unmapped = extractSingleStory({ frame });
	QCOMPARE(unmapped.status, Status::UnsupportedContent);
	QVERIFY(unmapped.detail.contains(QStringLiteral("ChapterTitle")));
	QCOMPARE(extractSingleStory({ frame }, { { QStringLiteral("ChapterTitle"), 1 } }).status,
		Status::UnsupportedContent);
	const Result mapped = extractSingleStory({ frame }, {
		{ QStringLiteral("ChapterTitle"), 1 }, { QStringLiteral("BodyCopy"), 0 }
	});
	QVERIFY2(mapped.ready(), qPrintable(mapped.detail));
	QCOMPARE(mapped.blocks.size(), 2);
	QCOMPARE(mapped.blocks.at(0).kind, EpubExport::BlockKind::Heading);
	QCOMPARE(mapped.blocks.at(0).headingLevel, 1);
	QCOMPARE(mapped.blocks.at(1).kind, EpubExport::BlockKind::Paragraph);
	QCOMPARE(mapped.blocks.at(1).headingLevel, 0);
	QCOMPARE(extractSingleStory({ frame }, { { QStringLiteral("ChapterTitle"), 7 },
		{ QStringLiteral("BodyCopy"), 0 } }).status, Status::UnsupportedContent);
	frame.paragraphStyleNames.removeLast();
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
}

void EpubReadingOrderTests::mapsCharacterRunsOnlyWhenExplicit()
{
	using namespace EpubReadingOrder;
	Frame frame { 0, -1, -1, QStringLiteral("A bold and italic word"), false };
	frame.characterStyleNames.resize(frame.rootText.size());
	for (int index = 2; index < 6; ++index)
		frame.characterStyleNames[index] = QStringLiteral("BoldText");
	for (int index = 11; index < 17; ++index)
		frame.characterStyleNames[index] = QStringLiteral("ItalicText");
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	QCOMPARE(extractSingleStory({ frame }, {}, {
		{ QStringLiteral("BoldText"), EpubExport::InlineKind::Strong }
	}).status, Status::UnsupportedContent);
	const Result mapped = extractSingleStory({ frame }, {}, {
		{ QStringLiteral("BoldText"), EpubExport::InlineKind::Strong },
		{ QStringLiteral("ItalicText"), EpubExport::InlineKind::Emphasis }
	});
	QVERIFY2(mapped.ready(), qPrintable(mapped.detail));
	QCOMPARE(mapped.blocks.size(), 1);
	const auto& runs = mapped.blocks.first().runs;
	QCOMPARE(runs.size(), 5);
	QCOMPARE(runs.at(0).text, QStringLiteral("A "));
	QCOMPARE(runs.at(1).kind, EpubExport::InlineKind::Strong);
	QCOMPARE(runs.at(1).text, QStringLiteral("bold"));
	QCOMPARE(runs.at(3).kind, EpubExport::InlineKind::Emphasis);
	QCOMPARE(runs.at(3).text, QStringLiteral("italic"));
	frame.characterStyleNames.removeLast();
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
}

void EpubReadingOrderTests::groupsFlatBulletItemsWithoutCrossingStories()
{
	using namespace EpubReadingOrder;
	Frame first { 0, -1, -1, QStringLiteral("One\nTwo\nBody\nThree"), false, 1 };
	first.bulletParagraphs = { true, true, false, true };
	Frame second { 1, -1, -1, QStringLiteral("Another"), false, 2 };
	second.bulletParagraphs = { true };
	const Result result = extractSavedOrder({ second, first });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 5);
	QCOMPARE(result.blocks.at(0).kind, EpubExport::BlockKind::BulletItem);
	QVERIFY(result.blocks.at(0).startsList);
	QVERIFY(!result.blocks.at(1).startsList);
	QCOMPARE(result.blocks.at(2).kind, EpubExport::BlockKind::Paragraph);
	QVERIFY(result.blocks.at(3).startsList);
	QVERIFY(result.blocks.at(4).startsList);
	first.bulletParagraphs.removeLast();
	QCOMPARE(extractSavedOrder({ first, second }).status, Status::UnsupportedContent);
}

void EpubReadingOrderTests::nestsOneLevelOfStandardBullets()
{
	using namespace EpubReadingOrder;
	Frame frame { 0, -1, -1, QStringLiteral("Parent\nChild one\nChild two\nNext parent"), false };
	frame.bulletParagraphs = { true, true, true, true };
	frame.bulletParagraphLevels = { 0, 1, 1, 0 };
	const Result result = extractSingleStory({ frame });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 4);
	QCOMPARE(result.blocks.at(1).listLevel, 1);
	QVERIFY(result.blocks.at(1).startsList);
	QVERIFY(!result.blocks.at(2).startsList);
	QCOMPARE(result.blocks.at(3).listLevel, 0);
	QVERIFY(!result.blocks.at(3).startsList);
	frame.bulletParagraphLevels[0] = 1;
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.bulletParagraphLevels = { 0, 2, 1, 0 };
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.bulletParagraphLevels.removeLast();
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
}

void EpubReadingOrderTests::groupsLocalDecimalListsAndPreservesStart()
{
	using namespace EpubReadingOrder;
	Frame first { 0, -1, -1, QStringLiteral("One\nTwo\nBody\nThree"), false, 1 };
	first.orderedParagraphStarts = { 3, 3, 0, 1 };
	const Result result = extractSavedOrder({ first });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 4);
	QCOMPARE(result.blocks.at(0).kind, EpubExport::BlockKind::OrderedItem);
	QVERIFY(result.blocks.at(0).startsList);
	QCOMPARE(result.blocks.at(0).listStart, 3);
	QVERIFY(!result.blocks.at(1).startsList);
	QCOMPARE(result.blocks.at(2).kind, EpubExport::BlockKind::Paragraph);
	QVERIFY(result.blocks.at(3).startsList);
	QCOMPARE(result.blocks.at(3).listStart, 1);
	first.orderedParagraphStarts[1] = 4;
	QCOMPARE(extractSavedOrder({ first }).status, Status::UnsupportedContent);
	first.orderedParagraphStarts.removeLast();
	QCOMPARE(extractSavedOrder({ first }).status, Status::UnsupportedContent);
}

void EpubReadingOrderTests::keepsRomanRunsSeparateAndBounded()
{
	using namespace EpubReadingOrder;
	Frame frame { 0, -1, -1, QStringLiteral("One\nTwo\nThree"), false };
	frame.orderedParagraphStarts = { 3, 3, 7 };
	frame.orderedParagraphStyles = { EpubExport::OrderedStyle::LowerRoman,
		EpubExport::OrderedStyle::LowerRoman, EpubExport::OrderedStyle::UpperRoman };
	const Result result = extractSingleStory({ frame });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.size(), 3);
	QVERIFY(result.blocks.at(0).startsList);
	QVERIFY(!result.blocks.at(1).startsList);
	QVERIFY(result.blocks.at(2).startsList);
	QCOMPARE(result.blocks.at(2).orderedStyle, EpubExport::OrderedStyle::UpperRoman);
	frame.orderedParagraphStarts = { 3999, 3999, 7 };
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.orderedParagraphStarts = { 3998, 3998, 7 };
	QCOMPARE(extractSingleStory({ frame }).status, Status::Ready);
	frame.orderedParagraphStyles.removeLast();
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.orderedParagraphStyles.append(EpubExport::OrderedStyle::UpperRoman);
	frame.orderedParagraphStarts = { 0, 0, 0 };
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
}

void EpubReadingOrderTests::keepsAlphabeticRunsSeparateAndBounded()
{
	using namespace EpubReadingOrder;
	Frame frame { 0, -1, -1, QStringLiteral("Y\nZ\nUpper"), false };
	frame.orderedParagraphStarts = { 25, 25, 1 };
	frame.orderedParagraphStyles = { EpubExport::OrderedStyle::LowerAlpha,
		EpubExport::OrderedStyle::LowerAlpha, EpubExport::OrderedStyle::UpperAlpha };
	const Result result = extractSingleStory({ frame });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QVERIFY(result.blocks.at(0).startsList);
	QVERIFY(!result.blocks.at(1).startsList);
	QVERIFY(result.blocks.at(2).startsList);
	QCOMPARE(result.blocks.at(2).orderedStyle, EpubExport::OrderedStyle::UpperAlpha);
	frame.orderedParagraphStarts = { 26, 26, 1 };
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.orderedParagraphStarts = { 25, 25, 1 };
	QCOMPARE(extractSingleStory({ frame }).status, Status::Ready);
}

void EpubReadingOrderTests::preservesParagraphDirections()
{
	using namespace EpubReadingOrder;
	Frame frame { 0, -1, -1, QStringLiteral("שלום\nEnglish"), false };
	frame.paragraphDirections = { EpubExport::TextDirection::Rtl, EpubExport::TextDirection::Ltr };
	const Result result = extractSingleStory({ frame });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.at(0).direction, EpubExport::TextDirection::Rtl);
	QCOMPARE(result.blocks.at(1).direction, EpubExport::TextDirection::Ltr);
	frame.paragraphDirections.removeLast();
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.paragraphDirections.append(static_cast<EpubExport::TextDirection>(-1));
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
}

void EpubReadingOrderTests::preservesParagraphAlignments()
{
	using namespace EpubReadingOrder;
	Frame frame { 0, -1, -1, QStringLiteral("Left\nCenter\nRight\nJustify"), false };
	frame.paragraphAlignments = { EpubExport::TextAlignment::Left,
		EpubExport::TextAlignment::Center, EpubExport::TextAlignment::Right,
		EpubExport::TextAlignment::Justify };
	const Result result = extractSingleStory({ frame });
	QVERIFY2(result.ready(), qPrintable(result.detail));
	QCOMPARE(result.blocks.at(0).alignment, EpubExport::TextAlignment::Left);
	QCOMPARE(result.blocks.at(1).alignment, EpubExport::TextAlignment::Center);
	QCOMPARE(result.blocks.at(2).alignment, EpubExport::TextAlignment::Right);
	QCOMPARE(result.blocks.at(3).alignment, EpubExport::TextAlignment::Justify);
	frame.paragraphAlignments.removeLast();
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
	frame.paragraphAlignments.append(static_cast<EpubExport::TextAlignment>(-1));
	QCOMPARE(extractSingleStory({ frame }).status, Status::UnsupportedContent);
}

QTEST_APPLESS_MAIN(EpubReadingOrderTests)
#include "epubreadingordertests.moc"
