/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "epubexport.h"
#include "third_party/zip/unzip.h"

#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QTemporaryDir>
#include <QXmlStreamReader>

#include <limits>
#include <QtTest>

namespace
{
EpubExport::Book sampleBook()
{
	return { QStringLiteral("urn:uuid:8f6ea24b-1c8d-4973-878e-ea00431d790a"),
		QStringLiteral("A & B"), QStringLiteral("te-IN"), QStringLiteral("Appaji"),
		{ { EpubExport::BlockKind::Heading, QStringLiteral("First <chapter>"), 1 },
		  { EpubExport::BlockKind::Paragraph, QStringLiteral("తెలుగు & English"), 0 },
		  { EpubExport::BlockKind::Heading, QStringLiteral("Second chapter"), 2 } } };
}

QByteArray archiveEntry(UnZip& archive, const QString& name)
{
	QByteArray bytes;
	QBuffer buffer(&bytes);
	if (!buffer.open(QIODevice::WriteOnly) || archive.extractFile(name, &buffer) != UnZip::Ok)
		return {};
	return bytes;
}

bool isWellFormed(const QByteArray& bytes)
{
	QXmlStreamReader xml(bytes);
	while (!xml.atEnd())
		xml.readNext();
	return !xml.hasError();
}

struct NavigationOutline
{
	QVector<QPair<QString, int>> entries;
	bool listsInsideEntries { true };
};

NavigationOutline navigationOutline(const QByteArray& bytes)
{
	NavigationOutline outline;
	QXmlStreamReader xml(bytes);
	QVector<QString> elements;
	int listDepth = 0;
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isStartElement())
		{
			const QString name = xml.name().toString();
			if (name == QLatin1String("ol"))
			{
				if (listDepth > 0 && (elements.isEmpty() || elements.last() != QLatin1String("li")))
					outline.listsInsideEntries = false;
				++listDepth;
			}
			if (name == QLatin1String("a"))
				outline.entries.append({ xml.attributes().value(QStringLiteral("href")).toString(), listDepth });
			elements.append(name);
		}
		else if (xml.isEndElement())
		{
			if (xml.name() == QLatin1String("ol"))
				--listDepth;
			elements.removeLast();
		}
	}
	if (xml.hasError() || listDepth != 0 || !elements.isEmpty())
		outline.listsInsideEntries = false;
	return outline;
}
}

class EpubExportTests : public QObject
{
	Q_OBJECT

private slots:
	void writesReflowablePackage();
	void rejectsInvalidInputAndPreservesExistingOutput();
	void writesSemanticInlineRuns();
	void writesFlatBulletLists();
	void writesNestedStandardBullets();
	void writesLocalDecimalLists();
	void writesBoundedRomanLists();
	void writesBoundedAlphabeticLists();
	void writesParagraphBaseDirections();
	void writesParagraphAlignmentStyles();
	void preservesParagraphSpacingAndIndents();
	void packagesIllustrationsWithAltText();
	void rejectsInvalidImageAssets();
	void writesHierarchicalNavigation();
	void splitsTopLevelChaptersIntoSpineFiles();
};

void EpubExportTests::writesReflowablePackage()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	const QString output = temp.filePath(QStringLiteral("sample.epub"));
	QVERIFY(EpubExport::writeBook(sampleBook(), output).exported());
	QFile file(output);
	QVERIFY(file.open(QIODevice::ReadOnly));
	const QByteArray prefix = file.read(60);
	QVERIFY(prefix.size() >= 50);
	QCOMPARE(prefix.left(4), QByteArray::fromHex("504b0304"));
	QCOMPARE(quint8(prefix[8]), quint8(0)); // The first entry is stored.
	QCOMPARE(quint8(prefix[9]), quint8(0));
	QCOMPARE(quint8(prefix[28]), quint8(0)); // No local ZIP extra field.
	QCOMPARE(quint8(prefix[29]), quint8(0));
	QCOMPARE(prefix.mid(30, 8), QByteArrayLiteral("mimetype"));
	QCOMPARE(prefix.mid(38, 20), QByteArrayLiteral("application/epub+zip"));
	file.close();

	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QStringList names = archive.fileList();
	QCOMPARE(names.size(), 6);
	QVERIFY(names.contains(QStringLiteral("mimetype")));
	QVERIFY(names.contains(QStringLiteral("META-INF/container.xml")));
	QVERIFY(names.contains(QStringLiteral("OEBPS/content.opf")));
	QVERIFY(names.contains(QStringLiteral("OEBPS/nav.xhtml")));
	QVERIFY(names.contains(QStringLiteral("OEBPS/styles.css")));
	QVERIFY(names.contains(QStringLiteral("OEBPS/chapter.xhtml")));
	for (const QString& name : names)
	{
		if (name == QLatin1String("mimetype") || name == QLatin1String("OEBPS/styles.css"))
			continue;
		QVERIFY2(isWellFormed(archiveEntry(archive, name)), qPrintable(name));
	}
	const QByteArray package = archiveEntry(archive, QStringLiteral("OEBPS/content.opf"));
	QVERIFY(package.contains("version=\"3.0\""));
	QVERIFY(package.contains("dcterms:modified"));
	QVERIFY(package.contains("properties=\"nav\""));
	QVERIFY(package.contains("href=\"styles.css\" media-type=\"text/css\""));
	QVERIFY(package.contains("te-IN"));
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(chapter.contains("<title>First &lt;chapter&gt;</title>"));
	QVERIFY(chapter.contains("href=\"styles.css\""));
	QVERIFY(chapter.contains("First &lt;chapter&gt;"));
	QVERIFY(chapter.contains(QStringLiteral("తెలుగు &amp; English").toUtf8()));
	QVERIFY(chapter.contains("id=\"h1\""));
	QVERIFY(chapter.contains("id=\"h3\""));
	const QByteArray nav = archiveEntry(archive, QStringLiteral("OEBPS/nav.xhtml"));
	QVERIFY(nav.contains("<title>A &amp; B</title>"));
	QVERIFY(nav.contains("chapter.xhtml#h1"));
	QVERIFY(nav.contains("chapter.xhtml#h3"));
	QVERIFY(!nav.contains("chapter.xhtml#h2"));
	archive.closeArchive();
	const QString sampleOutput = qEnvironmentVariable("SCRIBUS_EPUB_SAMPLE_OUTPUT");
	if (!sampleOutput.isEmpty())
	{
		QVERIFY(!QFileInfo::exists(sampleOutput));
		QVERIFY(QFile::copy(output, sampleOutput));
	}
}

void EpubExportTests::rejectsInvalidInputAndPreservesExistingOutput()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	const QString output = temp.filePath(QStringLiteral("sample.epub"));
	auto book = sampleBook();
	book.title.clear();
	QCOMPARE(EpubExport::writeBook(book, output).status, EpubExport::Status::InvalidInput);
	QVERIFY(!QFileInfo::exists(output));
	book = sampleBook();
	book.language = QStringLiteral("not a language tag");
	QCOMPARE(EpubExport::writeBook(book, output).status, EpubExport::Status::InvalidInput);
	book = sampleBook();
	book.blocks[0].headingLevel = 7;
	QCOMPARE(EpubExport::writeBook(book, output).status, EpubExport::Status::InvalidInput);
	book = sampleBook();
	book.blocks[1].text.append(QChar(1));
	QCOMPARE(EpubExport::writeBook(book, output).status, EpubExport::Status::InvalidInput);
	book = sampleBook();
	book.blocks[1].text.append(QChar(0xd800));
	QCOMPARE(EpubExport::writeBook(book, output).status, EpubExport::Status::InvalidInput);
	book = sampleBook();
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("missing/book.epub"))).status,
		EpubExport::Status::IoError);
	QFile existing(output);
	QVERIFY(existing.open(QIODevice::WriteOnly));
	QCOMPARE(existing.write("keep"), qint64(4));
	existing.close();
	QCOMPARE(EpubExport::writeBook(book, output).status, EpubExport::Status::OutputExists);
	QVERIFY(existing.open(QIODevice::ReadOnly));
	QCOMPARE(existing.readAll(), QByteArrayLiteral("keep"));
}

void EpubExportTests::writesSemanticInlineRuns()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks[1].text = QStringLiteral("Before bold & italic after");
	book.blocks[1].runs = {
		{ EpubExport::InlineKind::Plain, QStringLiteral("Before ") },
		{ EpubExport::InlineKind::Strong, QStringLiteral("bold & ") },
		{ EpubExport::InlineKind::Emphasis, QStringLiteral("italic") },
		{ EpubExport::InlineKind::Plain, QStringLiteral(" after") }
	};
	const QString output = temp.filePath(QStringLiteral("inline.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QVERIFY(chapter.contains("Before <strong>bold &amp; </strong><em>italic</em> after"));
	archive.closeArchive();
	book.blocks[1].runs[1].text = QStringLiteral("wrong");
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("invalid.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::writesFlatBulletLists()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Intro"), 0 },
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("First & item"), 0, {}, true },
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("Second item"), 0 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Bridge"), 0 },
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("Third item"), 0, {}, true }
	};
	const QString output = temp.filePath(QStringLiteral("bullets.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QCOMPARE(chapter.count("<ul>"), 2);
	QCOMPARE(chapter.count("<li>"), 3);
	QVERIFY(chapter.contains("<ul><li>First &amp; item</li><li>Second item</li></ul><p>Bridge</p>"));
	archive.closeArchive();
	book.blocks[1].text.clear();
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("invalid-bullets.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::writesNestedStandardBullets()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("Parent"), 0, {}, true },
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("Child & one"), 0, {}, true },
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("Child two"), 0 },
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("Next parent"), 0 }
	};
	book.blocks[1].listLevel = 1;
	book.blocks[2].listLevel = 1;
	const QString output = temp.filePath(QStringLiteral("nested-bullets.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QVERIFY(chapter.contains("<ul><li>Parent<ul><li>Child &amp; one</li><li>Child two</li></ul></li><li>Next parent</li></ul>"));
	archive.closeArchive();
	book.blocks[0].listLevel = 1;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("orphan-nested.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::writesLocalDecimalLists()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Intro"), 0 },
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("First & item"), 0, {}, true, 3 },
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("Second item"), 0, {}, false, 3 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Bridge"), 0 },
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("Fresh item"), 0, {}, true, 1 }
	};
	const QString output = temp.filePath(QStringLiteral("numbers.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QCOMPARE(chapter.count("<ol"), 2);
	QVERIFY(chapter.contains("<ol start=\"3\"><li>First &amp; item</li><li>Second item</li></ol>"));
	QVERIFY(chapter.contains("<p>Bridge</p><ol><li>Fresh item</li></ol>"));
	archive.closeArchive();
	book.blocks[1].listStart = 0;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("invalid-start.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[1].listStart = 3;
	book.blocks[2].listStart = 4;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("inconsistent-start.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::writesBoundedRomanLists()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("Lower first"), 0, {}, true, 3,
			EpubExport::OrderedStyle::LowerRoman },
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("Lower second"), 0, {}, false, 3,
			EpubExport::OrderedStyle::LowerRoman },
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("Upper first"), 0, {}, true, 7,
			EpubExport::OrderedStyle::UpperRoman }
	};
	const QString output = temp.filePath(QStringLiteral("roman.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QVERIFY(chapter.contains("<ol start=\"3\" type=\"i\"><li>Lower first</li><li>Lower second</li></ol>"));
	QVERIFY(chapter.contains("<ol start=\"7\" type=\"I\"><li>Upper first</li></ol>"));
	archive.closeArchive();
	book.blocks[1].orderedStyle = EpubExport::OrderedStyle::UpperRoman;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("mixed-run.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks.removeAt(1);
	book.blocks[0].listStart = 4000;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("roman-overflow.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].listStart = 3999;
	QVERIFY(EpubExport::writeBook(book, temp.filePath(QStringLiteral("roman-edge.epub"))).exported());
}

void EpubExportTests::writesBoundedAlphabeticLists()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("Lower Y"), 0, {}, true, 25,
			EpubExport::OrderedStyle::LowerAlpha },
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("Lower Z"), 0, {}, false, 25,
			EpubExport::OrderedStyle::LowerAlpha },
		{ EpubExport::BlockKind::OrderedItem, QStringLiteral("Upper A"), 0, {}, true, 1,
			EpubExport::OrderedStyle::UpperAlpha }
	};
	const QString output = temp.filePath(QStringLiteral("alphabetic.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QVERIFY(chapter.contains("<ol start=\"25\" type=\"a\"><li>Lower Y</li><li>Lower Z</li></ol>"));
	QVERIFY(chapter.contains("<ol type=\"A\"><li>Upper A</li></ol>"));
	archive.closeArchive();
	const QString sampleOutput = qEnvironmentVariable("SCRIBUS_EPUB_ALPHA_SAMPLE_OUTPUT");
	if (!sampleOutput.isEmpty())
	{
		QVERIFY(!QFileInfo::exists(sampleOutput));
		QVERIFY(QFile::copy(output, sampleOutput));
	}
	book.blocks[0].listStart = 26;
	book.blocks[1].listStart = 26;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("alphabetic-overflow.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::writesParagraphBaseDirections()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::Heading, QStringLiteral("שלום"), 1 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("مرحبا"), 0 },
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("ראשון"), 0, {}, true },
		{ EpubExport::BlockKind::BulletItem, QStringLiteral("Second"), 0 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("English section"), 2 },
	};
	book.blocks[0].direction = EpubExport::TextDirection::Rtl;
	book.blocks[1].direction = EpubExport::TextDirection::Rtl;
	book.blocks[2].direction = EpubExport::TextDirection::Rtl;
	const QString output = temp.filePath(QStringLiteral("direction.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	const QByteArray nav = archiveEntry(archive, QStringLiteral("OEBPS/nav.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QVERIFY(isWellFormed(nav));
	QVERIFY(chapter.contains("<h1 dir=\"rtl\" class=\"scribus-align-left\" id=\"h1\">"));
	QVERIFY(chapter.contains("<p dir=\"rtl\" class=\"scribus-align-left\">"));
	QVERIFY(chapter.contains("<ul dir=\"rtl\"><li dir=\"rtl\" class=\"scribus-align-left\">"));
	QVERIFY(chapter.contains("<li dir=\"ltr\">Second</li>"));
	QVERIFY(nav.contains("<li dir=\"rtl\">"));
	QVERIFY(nav.contains("<li dir=\"ltr\">"));
	QVERIFY(nav.contains("href=\"chapter.xhtml#h1\""));
	archive.closeArchive();
	book.blocks[0].direction = static_cast<EpubExport::TextDirection>(-1);
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("invalid-direction.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::writesParagraphAlignmentStyles()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Left"), 0 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Center"), 0 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Right"), 0 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Justify"), 0 }
	};
	book.blocks[1].alignment = EpubExport::TextAlignment::Center;
	book.blocks[2].alignment = EpubExport::TextAlignment::Right;
	book.blocks[3].alignment = EpubExport::TextAlignment::Justify;
	const QString output = temp.filePath(QStringLiteral("alignment.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QVERIFY(chapter.contains("<p>Left</p>"));
	QVERIFY(chapter.contains("<p class=\"scribus-align-center\">Center</p>"));
	QVERIFY(chapter.contains("<p class=\"scribus-align-right\">Right</p>"));
	QVERIFY(chapter.contains("<p class=\"scribus-align-justify\">Justify</p>"));
	const QByteArray css = archiveEntry(archive, QStringLiteral("OEBPS/styles.css"));
	QVERIFY(css.contains(".scribus-align-left { text-align: left; }"));
	QVERIFY(css.contains(".scribus-align-center { text-align: center; }"));
	QVERIFY(css.contains(".scribus-align-right { text-align: right; }"));
	QVERIFY(css.contains(".scribus-align-justify { text-align: justify; }"));
	archive.closeArchive();
	book.blocks[1].alignment = static_cast<EpubExport::TextAlignment>(-1);
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("invalid-alignment.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::preservesParagraphSpacingAndIndents()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("LTR text"), 0 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("RTL text"), 0 }
	};
	book.blocks[0].metrics = { 18.0, 7.0, -4.0, 6.0, 3.0 };
	book.blocks[1].metrics = book.blocks[0].metrics;
	book.blocks[1].direction = EpubExport::TextDirection::Rtl;
	const QString output = temp.filePath(QStringLiteral("paragraph-metrics.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QByteArray chapter = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(isWellFormed(chapter));
	QVERIFY(chapter.contains("style=\"margin-left:18.000000pt; margin-right:7.000000pt; text-indent:-4.000000pt; margin-top:6.000000pt; margin-bottom:3.000000pt\""));
	QVERIFY(chapter.contains("style=\"margin-left:7.000000pt; margin-right:18.000000pt; text-indent:-4.000000pt; margin-top:6.000000pt; margin-bottom:3.000000pt\""));
	archive.closeArchive();
	book.blocks[0].metrics.leftIndent = std::numeric_limits<double>::infinity();
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("infinite-indent.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].metrics.leftIndent = 18.0;
	book.blocks[0].kind = EpubExport::BlockKind::BulletItem;
	book.blocks[0].startsList = true;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("styled-list.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::packagesIllustrationsWithAltText()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	QImage pixels(2, 2, QImage::Format_RGB32);
	pixels.fill(Qt::red);
	QByteArray png;
	QByteArray jpeg;
	QBuffer pngBuffer(&png);
	QBuffer jpegBuffer(&jpeg);
	QVERIFY(pngBuffer.open(QIODevice::WriteOnly));
	QVERIFY(jpegBuffer.open(QIODevice::WriteOnly));
	QVERIFY(pixels.save(&pngBuffer, "PNG"));
	QVERIFY(pixels.save(&jpegBuffer, "JPEG"));
	auto book = sampleBook();
	book.images = { { png, QStringLiteral("image/png") }, { jpeg, QStringLiteral("image/jpeg") } };
	book.blocks = {
		{ EpubExport::BlockKind::Heading, QStringLiteral("First"), 1 },
		{ EpubExport::BlockKind::Image, QStringLiteral("Red square & sample"), 0 },
		{ EpubExport::BlockKind::Image, QStringLiteral("Another red square"), 0 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Second"), 1 },
		{ EpubExport::BlockKind::Image, QStringLiteral("Repeated PNG"), 0 }
	};
	book.blocks[1].imageIndex = 0;
	book.blocks[1].caption = QStringLiteral("Figure 1 — red & blue");
	book.blocks[1].captionAlignment = EpubExport::TextAlignment::Center;
	book.blocks[2].imageIndex = 1;
	book.blocks[4].imageIndex = 0;
	const QString output = temp.filePath(QStringLiteral("illustrated.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QStringList names = archive.fileList();
	QCOMPARE(names.size(), 9);
	QVERIFY(names.contains(QStringLiteral("OEBPS/images/image-1.png")));
	QVERIFY(names.contains(QStringLiteral("OEBPS/images/image-2.jpg")));
	QCOMPARE(archiveEntry(archive, QStringLiteral("OEBPS/images/image-1.png")), png);
	QCOMPARE(archiveEntry(archive, QStringLiteral("OEBPS/images/image-2.jpg")), jpeg);
	const QByteArray package = archiveEntry(archive, QStringLiteral("OEBPS/content.opf"));
	QVERIFY(isWellFormed(package));
	QVERIFY(package.contains("href=\"images/image-1.png\" media-type=\"image/png\""));
	QVERIFY(package.contains("href=\"images/image-2.jpg\" media-type=\"image/jpeg\""));
	const QByteArray first = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	const QByteArray second = archiveEntry(archive, QStringLiteral("OEBPS/chapter-2.xhtml"));
	QVERIFY(isWellFormed(first));
	QVERIFY(isWellFormed(second));
	QVERIFY(first.contains("<figure><img src=\"images/image-1.png\" alt=\"Red square &amp; sample\""));
	QVERIFY(first.contains("<figcaption class=\"scribus-align-center\">Figure 1 — red &amp; blue</figcaption>"));
	QVERIFY(first.contains("<figure><img src=\"images/image-2.jpg\" alt=\"Another red square\""));
	QCOMPARE(first.count("<figcaption"), 1);
	QVERIFY(second.contains("<figure><img src=\"images/image-1.png\" alt=\"Repeated PNG\""));
	archive.closeArchive();
	const QString sampleOutput = qEnvironmentVariable("SCRIBUS_EPUB_IMAGE_SAMPLE_OUTPUT");
	if (!sampleOutput.isEmpty())
	{
		QVERIFY(!QFileInfo::exists(sampleOutput));
		QVERIFY(QFile::copy(output, sampleOutput));
	}
}

void EpubExportTests::rejectsInvalidImageAssets()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	QImage pixels(1, 1, QImage::Format_RGB32);
	pixels.fill(Qt::blue);
	QByteArray png;
	QBuffer buffer(&png);
	QVERIFY(buffer.open(QIODevice::WriteOnly));
	QVERIFY(pixels.save(&buffer, "PNG"));
	auto book = sampleBook();
	book.blocks = { { EpubExport::BlockKind::Image, QStringLiteral("Blue square"), 0 } };
	book.blocks[0].imageIndex = 0;
	book.images = { { png, QStringLiteral("image/png") } };
	book.blocks[0].text.clear();
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("missing-alt.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].decorative = true;
	const QString decorativePath = temp.filePath(QStringLiteral("decorative.epub"));
	QVERIFY(EpubExport::writeBook(book, decorativePath).exported());
	UnZip decorativeArchive;
	QCOMPARE(decorativeArchive.openArchive(decorativePath), UnZip::Ok);
	const QByteArray decorativeChapter = archiveEntry(decorativeArchive, QStringLiteral("OEBPS/chapter.xhtml"));
	QVERIFY(decorativeChapter.contains("alt=\"\" role=\"presentation\""));
	decorativeArchive.closeArchive();
	book.blocks[0].caption = QStringLiteral("Visible caption");
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("decorative-caption.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].caption.clear();
	book.blocks[0].text = QStringLiteral("Blue square");
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("decorative-alt.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].decorative = false;
	book.blocks[0].text = QStringLiteral("Blue square");
	book.blocks[0].imageWidthPercent = 55;
	const QString sizedPath = temp.filePath(QStringLiteral("sized-image.epub"));
	QVERIFY(EpubExport::writeBook(book, sizedPath).exported());
	UnZip sizedArchive;
	QCOMPARE(sizedArchive.openArchive(sizedPath), UnZip::Ok);
	QVERIFY(archiveEntry(sizedArchive, QStringLiteral("OEBPS/chapter.xhtml"))
		.contains("<figure style=\"width: 55%; max-width: 100%;\""));
	QVERIFY(archiveEntry(sizedArchive, QStringLiteral("OEBPS/chapter.xhtml"))
		.contains("alt=\"Blue square\" style=\"width: 100%;\""));
	sizedArchive.closeArchive();
	book.blocks[0].imageWidthPercent = 101;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("oversize-image.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].imageWidthPercent = -1;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("negative-width.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].imageWidthPercent = 0;
	auto invalidTextWidth = sampleBook();
	invalidTextWidth.blocks[0].imageWidthPercent = 50;
	QCOMPARE(EpubExport::writeBook(invalidTextWidth, temp.filePath(QStringLiteral("text-width.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].imageIndex = 2;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("bad-index.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].imageIndex = 0;
	book.images[0].data = QByteArrayLiteral("not a PNG");
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("corrupt-image.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.images[0].data = png;
	book.images[0].mediaType = QStringLiteral("image/jpeg");
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("wrong-media-type.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.images[0].mediaType = QStringLiteral("image/png");
	book.images.append({ png, QStringLiteral("image/png") });
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("unreferenced-image.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.images.removeLast();
	book.blocks[0].runs = { { EpubExport::InlineKind::Plain, QStringLiteral("Blue square") } };
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("image-runs.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].runs.clear();
	book.blocks[0].caption = QStringLiteral(" \t ");
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("blank-caption.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].caption = QStringLiteral("Invalid") + QChar::Null;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("invalid-caption.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].caption = QStringLiteral("A visible caption");
	book.blocks[0].captionAlignment = static_cast<EpubExport::TextAlignment>(7);
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("invalid-caption-alignment.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].captionAlignment = EpubExport::TextAlignment::Right;
	book.blocks[0].caption.clear();
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("orphan-caption-alignment.epub"))).status,
		EpubExport::Status::InvalidInput);
	book.blocks[0].caption = QStringLiteral("A visible caption");
	book.blocks[0].kind = EpubExport::BlockKind::Paragraph;
	QCOMPARE(EpubExport::writeBook(book, temp.filePath(QStringLiteral("wrong-block-kind.epub"))).status,
		EpubExport::Status::InvalidInput);
}

void EpubExportTests::writesHierarchicalNavigation()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::Heading, QStringLiteral("Opening"), 2 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Body"), 0 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Deep"), 4 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Sibling"), 3 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Deeper"), 5 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("New chapter"), 1 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Section"), 2 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Second section"), 2 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Next chapter"), 1 }
	};
	QVERIFY(EpubExport::writeBook(book, temp.filePath(QStringLiteral("outline.epub"))).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(temp.filePath(QStringLiteral("outline.epub"))), UnZip::Ok);
	const QByteArray nav = archiveEntry(archive, QStringLiteral("OEBPS/nav.xhtml"));
	QVERIFY(isWellFormed(nav));
	QVERIFY(archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml")).contains("<title>Opening</title>"));
	const NavigationOutline outline = navigationOutline(nav);
	QVERIFY(outline.listsInsideEntries);
	QCOMPARE(outline.entries, (QVector<QPair<QString, int>> {
		{ QStringLiteral("chapter.xhtml#h1"), 1 },
		{ QStringLiteral("chapter.xhtml#h3"), 2 },
		{ QStringLiteral("chapter.xhtml#h4"), 2 },
		{ QStringLiteral("chapter.xhtml#h5"), 3 },
		{ QStringLiteral("chapter.xhtml#h6"), 1 },
		{ QStringLiteral("chapter.xhtml#h7"), 2 },
		{ QStringLiteral("chapter.xhtml#h8"), 2 },
		{ QStringLiteral("chapter-2.xhtml#h9"), 1 }
	}));
	archive.closeArchive();
	book.blocks = { { EpubExport::BlockKind::Paragraph, QStringLiteral("No headings"), 0 } };
	QVERIFY(EpubExport::writeBook(book, temp.filePath(QStringLiteral("fallback.epub"))).exported());
	QCOMPARE(archive.openArchive(temp.filePath(QStringLiteral("fallback.epub"))), UnZip::Ok);
	QVERIFY(archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml")).contains("<title>A &amp; B</title>"));
	const NavigationOutline fallback = navigationOutline(archiveEntry(archive, QStringLiteral("OEBPS/nav.xhtml")));
	QVERIFY(fallback.listsInsideEntries);
	QCOMPARE(fallback.entries, (QVector<QPair<QString, int>> {
		{ QStringLiteral("chapter.xhtml#book-title"), 1 }
	}));
	archive.closeArchive();
}

void EpubExportTests::splitsTopLevelChaptersIntoSpineFiles()
{
	QTemporaryDir temp;
	QVERIFY(temp.isValid());
	auto book = sampleBook();
	book.blocks = {
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Preface"), 0 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("One"), 1 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("One section"), 2 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("First body"), 0 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Two"), 1 },
		{ EpubExport::BlockKind::Paragraph, QStringLiteral("Second body"), 0 },
		{ EpubExport::BlockKind::Heading, QStringLiteral("Three"), 1 }
	};
	const QString output = temp.filePath(QStringLiteral("chapters.epub"));
	QVERIFY(EpubExport::writeBook(book, output).exported());
	UnZip archive;
	QCOMPARE(archive.openArchive(output), UnZip::Ok);
	const QStringList names = archive.fileList();
	QCOMPARE(names.size(), 8);
	for (const QString& name : { QStringLiteral("OEBPS/chapter.xhtml"),
		QStringLiteral("OEBPS/chapter-2.xhtml"), QStringLiteral("OEBPS/chapter-3.xhtml") })
	{
		QVERIFY(names.contains(name));
		QVERIFY(isWellFormed(archiveEntry(archive, name)));
	}
	const QByteArray first = archiveEntry(archive, QStringLiteral("OEBPS/chapter.xhtml"));
	const QByteArray second = archiveEntry(archive, QStringLiteral("OEBPS/chapter-2.xhtml"));
	const QByteArray third = archiveEntry(archive, QStringLiteral("OEBPS/chapter-3.xhtml"));
	QVERIFY(first.contains("<title>One</title>"));
	QVERIFY(second.contains("<title>Two</title>"));
	QVERIFY(third.contains("<title>Three</title>"));
	QVERIFY(first.contains("id=\"book-title\""));
	QVERIFY(first.contains("Preface"));
	QVERIFY(first.contains("id=\"h2\""));
	QVERIFY(!first.contains("Second body"));
	QVERIFY(second.contains("id=\"h5\""));
	QVERIFY(second.contains("Second body"));
	QVERIFY(!second.contains("book-title"));
	QVERIFY(!second.contains("First body"));
	QVERIFY(third.contains("id=\"h7\""));
	QVERIFY(!third.contains("Second body"));
	const QByteArray package = archiveEntry(archive, QStringLiteral("OEBPS/content.opf"));
	QVERIFY(isWellFormed(package));
	const auto firstRef = package.indexOf("idref=\"chapter\"");
	const auto secondRef = package.indexOf("idref=\"chapter-2\"");
	const auto thirdRef = package.indexOf("idref=\"chapter-3\"");
	QVERIFY(firstRef >= 0 && firstRef < secondRef && secondRef < thirdRef);
	const QByteArray nav = archiveEntry(archive, QStringLiteral("OEBPS/nav.xhtml"));
	QVERIFY(isWellFormed(nav));
	QVERIFY(nav.contains("<title>A &amp; B</title>"));
	QVERIFY(nav.contains("chapter.xhtml#h2"));
	QVERIFY(nav.contains("chapter.xhtml#h3"));
	QVERIFY(nav.contains("chapter-2.xhtml#h5"));
	QVERIFY(nav.contains("chapter-3.xhtml#h7"));
	archive.closeArchive();
}

QTEST_APPLESS_MAIN(EpubExportTests)
#include "epubexporttests.moc"
