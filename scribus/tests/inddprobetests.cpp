/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QRandomGenerator>
#include <QTemporaryDir>
#include <QtTest>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

#include "plugins/import/indd/inddmetadata.h"
#include "plugins/import/indd/inddblockdiff.h"
#include "plugins/import/indd/inddinspection.h"
#include "plugins/import/indd/inddpreview.h"
#include "plugins/import/indd/inddprobe.h"
#include "plugins/import/indd/inddrecordprobe.h"

namespace
{
void put16(QByteArray& bytes, qsizetype offset, quint16 value, bool littleEndian)
{
	bytes[offset] = char((value >> (littleEndian ? 0 : 8)) & 0xff);
	bytes[offset + 1] = char((value >> (littleEndian ? 8 : 0)) & 0xff);
}

void put32(QByteArray& bytes, qsizetype offset, quint32 value, bool littleEndian)
{
	for (int i = 0; i < 4; ++i)
		bytes[offset + i] = char((value >> (8 * (littleEndian ? i : 3 - i))) & 0xff);
}

void put64LittleEndian(QByteArray& bytes, qsizetype offset, quint64 value)
{
	for (int i = 0; i < 8; ++i)
		bytes[offset + i] = char((value >> (8 * i)) & 0xff);
}

void putDoubleLittleEndian(QByteArray& bytes, qsizetype offset, double value)
{
	quint64 bits = 0;
	static_assert(sizeof(bits) == sizeof(value), "INDD double must be 64 bits");
	std::memcpy(&bits, &value, sizeof(bits));
	put64LittleEndian(bytes, offset, bits);
}

QByteArray makeHeader(quint32 version, quint64 sequence, quint32 databasePages, bool littleEndian = true)
{
	QByteArray header(4096, '\0');
	const QByteArray signature = QByteArray::fromHex("0606edf5d81d46e5bd31efe7fe74b71d");
	header.replace(0, signature.size(), signature);
	header.replace(16, 8, "DOCUMENT");
	header[24] = littleEndian ? '\1' : '\2';
	put32(header, 29, version, littleEndian);
	put64LittleEndian(header, 264, sequence);
	put32(header, 280, databasePages, littleEndian);
	return header;
}

void setSyntheticCandidateRoot(QByteArray& document)
{
	put32(document, 936, 7, true);
	put32(document, 4096 + 936, 7, true);
	put32(document, 7 * 4096 + 128, 9, true);
	put32(document, 7 * 4096 + 4084, 4, true);
	put32(document, 7 * 4096 + 4088, 8, true);
}

Indd::ProbeResult probeBytes(const QByteArray& bytes)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::probeFile(path);
}

Indd::BlockDiffResult diffBytes(const QByteArray& firstBytes, const QByteArray& secondBytes)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString firstPath = tempDir.filePath("first.indd");
	const QString secondPath = tempDir.filePath("second.indd");
	for (const auto& entry : { qMakePair(firstPath, firstBytes), qMakePair(secondPath, secondBytes) })
	{
		QFile file(entry.first);
		if (!file.open(QIODevice::WriteOnly) || file.write(entry.second) != entry.second.size())
			return {};
	}
	return Indd::compareDatabaseBlocks(firstPath, secondPath);
}

Indd::RecordPageResult recordPageBytes(const QByteArray& bytes, quint32 pageIndex)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::inspectCandidateRecordPage(path, pageIndex);
}

Indd::RecordCatalogResult recordCatalogBytes(const QByteArray& bytes,
	quint32 pageLimit = Indd::maxCatalogDatabasePages, int recordLimit = Indd::maxCatalogRecords)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::catalogCandidateRecordPages(path, pageLimit, recordLimit);
}

Indd::PageDirectoryResult pageDirectoryBytes(const QByteArray& bytes)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::inspectCandidatePageDirectory(path);
}

Indd::CandidateDirectoryRootResult candidateDirectoryRootBytes(const QByteArray& bytes)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::inspectCandidateDirectoryRoot(path);
}

Indd::PageDirectoryAuditResult pageDirectoryAuditBytes(const QByteArray& bytes)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::auditCandidatePageDirectory(path);
}

Indd::DirectoryMappedRecordCatalogResult mappedRecordCatalogBytes(const QByteArray& bytes,
	int recordLimit = Indd::maxCatalogRecords)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::catalogDirectoryMappedRecordPages(path, recordLimit);
}

Indd::CandidateFrameStoryCatalogResult candidateFrameStoryBytes(const QByteArray& bytes,
	int recordLimit = Indd::maxCatalogRecords)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::catalogCandidateFrameStoryReferences(path, recordLimit);
}

Indd::CandidateType6RowTargetResult candidateType6RowsBytes(const QByteArray& bytes,
	quint32 rawKeyWord, int matchLimit = 4096, int recordLimit = Indd::maxCatalogRecords)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::findCandidateType6RowTargets(path, rawKeyWord, matchLimit, recordLimit);
}

Indd::RecordSlotDiffResult recordSlotDiffBytes(const QByteArray& firstBytes,
	const QByteArray& secondBytes, int recordLimit = Indd::maxCatalogRecords)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString firstPath = tempDir.filePath("first.indd");
	const QString secondPath = tempDir.filePath("second.indd");
	for (const auto& entry : { qMakePair(firstPath, firstBytes), qMakePair(secondPath, secondBytes) })
	{
		QFile file(entry.first);
		if (!file.open(QIODevice::WriteOnly) || file.write(entry.second) != entry.second.size())
			return {};
	}
	return Indd::compareDirectoryMappedRecordSlots(firstPath, secondPath, recordLimit);
}

Indd::ExactRecordRelocationResult exactRecordRelocationBytes(const QByteArray& firstBytes,
	const QByteArray& secondBytes, int recordLimit = 10000)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString firstPath = tempDir.filePath("first.indd");
	const QString secondPath = tempDir.filePath("second.indd");
	for (const auto& entry : { qMakePair(firstPath, firstBytes), qMakePair(secondPath, secondBytes) })
	{
		QFile file(entry.first);
		if (!file.open(QIODevice::WriteOnly) || file.write(entry.second) != entry.second.size())
			return {};
	}
	return Indd::compareExactCandidateRecordRelocations(firstPath, secondPath, recordLimit);
}

Indd::CandidateStoryResult candidateStoryBytes(const QByteArray& bytes,
	quint32 token = 1, quint16 slot = 2, int recordLimit = Indd::maxCatalogRecords)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::inspectCandidateStoryText(path, token, slot, recordLimit);
}

Indd::RecordWordSearchResult recordWordSearchBytes(const QByteArray& bytes, quint32 word,
	int matchLimit = 4096, int recordLimit = Indd::maxCatalogRecords)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::findCandidateRecordWords(path, word, matchLimit, recordLimit);
}

Indd::CandidateAffineResult candidateAffineBytes(const QByteArray& bytes,
	quint32 token = 1, quint16 slot = 2, int recordLimit = Indd::maxCatalogRecords)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::inspectCandidateAffineTransform(path, token, slot, recordLimit);
}

QByteArray makeMappedSlotDocument(quint32 physicalPage = 10, quint16 slot = 2,
	char payload = 'A', quint16 firstLength = 56)
{
	QByteArray document = makeHeader(8, 1, 13) + makeHeader(8, 2, 13) + QByteArray(11 * 4096, '\0');
	setSyntheticCandidateRoot(document);
	put32(document, 9 * 4096, 987, true);
	put32(document, 9 * 4096 + 128 + 4, physicalPage, true);
	put32(document, 9 * 4096 + 4084, 5, true);
	put32(document, 9 * 4096 + 4088, 10, true);
	const int page = int(physicalPage * 4096);
	put16(document, page, 20, true);
	put16(document, page + 20, firstLength, true);
	put16(document, page + 22, slot, true);
	document[page + 28] = payload;
	put16(document, page + 20 + firstLength, quint16(3996 - 20 - firstLength), true);
	put32(document, page + 3996 + 4 * (16 - slot), 20, true);
	put32(document, page + 4060, 16, true);
	put32(document, page + 4084, 9, true);
	put32(document, page + 4088, 1, true);
	return document;
}

QByteArray makeCandidateStoryDocument(const QByteArray& runs, quint32 unitCount)
{
	QByteArray document = makeMappedSlotDocument(10, 2, 'A', 64);
	const int record = 10 * 4096 + 20;
	const quint32 sectionLength = quint32(runs.size()) + 4;
	put32(document, record + 4, 0x262, true);
	put32(document, record + 8, sectionLength + 26, true);
	put32(document, record + 12, 0x202, true);
	put16(document, record + 20, 1, true);
	put32(document, record + 22, sectionLength, true);
	put32(document, record + 26, unitCount, true);
	document.replace(record + 30, runs.size(), runs);
	return document;
}

QByteArray makeCandidateAffineDocument(double x = 4.0, double y = -100.8)
{
	QByteArray document = makeMappedSlotDocument(10, 2, 'A', 952);
	const int record = 10 * 4096 + 20;
	put32(document, record + 896, 0x151, true);
	put32(document, record + 900, 48, true);
	const double matrix[] = { 1.0, 0.0, 0.0, 1.0, x, y };
	for (int i = 0; i < 6; ++i)
		putDoubleLittleEndian(document, record + 904 + 8 * i, matrix[i]);
	return document;
}

QByteArray makeCandidateFrameStoryDocument(bool outerStoryFirst)
{
	QByteArray document = makeMappedSlotDocument(10, 2, 'A', 124);
	const int record = 10 * 4096 + 20;
	constexpr quint32 frameWord = 0xfd;
	constexpr quint32 storyWord = 0xe8;
	if (outerStoryFirst)
	{
		put32(document, record + 4, 0x2ab, true);
		put32(document, record + 8, 8, true);
		put32(document, record + 12, frameWord - 4, true);
		put32(document, record + 16, storyWord, true);
		put32(document, record + 20, 0x205, true);
		put32(document, record + 24, 12, true);
		put32(document, record + 28, storyWord, true);
		put32(document, record + 36, frameWord - 1, true);
		put32(document, record + 40, 0x2ad, true);
		put32(document, record + 44, 4, true);
		put32(document, record + 52, 0x2dd, true);
		put32(document, record + 56, 44, true);
		put32(document, record + 60, 2, true);
		put32(document, record + 76, frameWord - 1, true);
		put32(document, record + 100, frameWord, true);
		put32(document, record + 104, 0x261, true);
		put32(document, record + 108, 10, true);
	}
	else
	{
		put32(document, record + 4, 0x2ad, true);
		put32(document, record + 8, 4, true);
		put32(document, record + 16, 0x2dd, true);
		put32(document, record + 20, 44, true);
		put32(document, record + 24, 2, true);
		put32(document, record + 40, frameWord - 1, true);
		put32(document, record + 44, frameWord - 1, true);
		put32(document, record + 64, frameWord, true);
		put32(document, record + 68, 0x261, true);
		put32(document, record + 72, 10, true);
		put32(document, record + 82, frameWord - 4, true);
		put32(document, record + 86, 0x2ab, true);
		put32(document, record + 90, 8, true);
		put32(document, record + 94, frameWord - 4, true);
		put32(document, record + 98, storyWord, true);
		put32(document, record + 102, 0x205, true);
		put32(document, record + 106, 12, true);
		put32(document, record + 110, storyWord, true);
	}
	return document;
}

QByteArray makeObject(const QByteArray& body)
{
	QByteArray header(32, '\0');
	header.replace(0, 16, QByteArray::fromHex("de39397951884b6c8e63eef8aee0dd38"));
	put32(header, 24, body.size(), true);
	QByteArray trailer(32, '\0');
	trailer.replace(0, 16, QByteArray::fromHex("fdcedb70f7864b4fa4d3c728b3417106"));
	return header + body + trailer;
}

QByteArray makeXmpObject(const QByteArray& packet, bool littleEndian = true)
{
	QByteArray length(4, '\0');
	put32(length, 0, packet.size(), littleEndian);
	return makeObject(length + packet);
}

Indd::MetadataResult metadataBytes(const QByteArray& bytes)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::readMetadata(path);
}

Indd::PreviewResult previewBytes(const QByteArray& bytes, int pageNumber = 1)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::readPreview(path, pageNumber);
}

Indd::InspectionResult inspectionBytes(const QByteArray& bytes)
{
	QTemporaryDir tempDir;
	if (!tempDir.isValid())
		return {};
	const QString path = tempDir.filePath("sample.indd");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly) || file.write(bytes) != bytes.size())
		return {};
	file.close();
	return Indd::inspectDocument(path);
}

QByteArray sampleInspectionXmp()
{
	return "<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>"
		"<x:xmpmeta xmlns:x=\"adobe:ns:meta/\"><rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">"
		"<rdf:Description xmlns:xmpTPg=\"http://ns.adobe.com/xap/1.0/t/pg/\" "
		"xmlns:stFnt=\"http://ns.adobe.com/xap/1.0/sType/Font#\" "
		"xmlns:xmpMM=\"http://ns.adobe.com/xap/1.0/mm/\" "
		"xmlns:stRef=\"http://ns.adobe.com/xap/1.0/sType/ResourceRef#\" xmpTPg:NPages=\"3\">"
		"<xmpTPg:Fonts><rdf:Bag><rdf:li><stFnt:fontName>Example-Regular</stFnt:fontName></rdf:li>"
		"<rdf:li><stFnt:fontName>Example-Regular</stFnt:fontName></rdf:li></rdf:Bag></xmpTPg:Fonts>"
		"<xmpMM:Ingredients><rdf:Bag><rdf:li><stRef:filePath>file:///images/photo.jpg</stRef:filePath></rdf:li>"
		"<rdf:li><stRef:filePath>file:///images/photo.jpg</stRef:filePath></rdf:li></rdf:Bag></xmpMM:Ingredients>"
		"<stRef:filePath>file:///not-an-ingredient.jpg</stRef:filePath>"
		"<xmpMM:Pantry><rdf:Bag><rdf:li><rdf:Description>"
		"<xmpTPg:Fonts><rdf:Bag><rdf:li><stFnt:fontName>Nested-Font</stFnt:fontName></rdf:li>"
		"</rdf:Bag></xmpTPg:Fonts></rdf:Description></rdf:li></rdf:Bag></xmpMM:Pantry>"
		"</rdf:Description></rdf:RDF></x:xmpmeta><?xpacket end=\"w\"?>";
}

QByteArray sampleJpeg()
{
	QImage image(2, 3, QImage::Format_RGB32);
	image.fill(Qt::red);
	QByteArray jpeg;
	QBuffer buffer(&jpeg);
	if (!buffer.open(QIODevice::WriteOnly) || !image.save(&buffer, "JPEG"))
		return {};
	return jpeg;
}

QByteArray samplePreviewXmp(const QByteArray& encodedImage, int width = 2)
{
	return QByteArray("<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>") +
		"<x:xmpmeta xmlns:x=\"adobe:ns:meta/\"><rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">"
		"<rdf:Description xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\" "
		"xmlns:xmpTPg=\"http://ns.adobe.com/xap/1.0/t/pg/\" "
		"xmlns:xmpGImg=\"http://ns.adobe.com/xap/1.0/g/img/\">"
		"<xmp:PageInfo><rdf:Seq><rdf:li rdf:parseType=\"Resource\">"
		"<xmpTPg:PageNumber>1</xmpTPg:PageNumber><xmpGImg:format>JPEG</xmpGImg:format>"
		"<xmpGImg:width>" + QByteArray::number(width) + "</xmpGImg:width>"
		"<xmpGImg:height>3</xmpGImg:height><xmpGImg:image>" + encodedImage + "</xmpGImg:image>"
		"</rdf:li></rdf:Seq></xmp:PageInfo></rdf:Description></rdf:RDF></x:xmpmeta><?xpacket end=\"w\"?>";
}

QByteArray sampleXmp()
{
	return "<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?>"
		"<x:xmpmeta xmlns:x=\"adobe:ns:meta/\"><rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">"
		"<rdf:Description xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:xmp=\"http://ns.adobe.com/xap/1.0/\">"
		"<dc:title><rdf:Alt><rdf:li xml:lang=\"x-default\">Portfolio</rdf:li></rdf:Alt></dc:title>"
		"<dc:creator><rdf:Seq><rdf:li>Example Author</rdf:li></rdf:Seq></dc:creator>"
		"<xmp:CreateDate>2024-01-01T00:00:00Z</xmp:CreateDate>"
		"<xmp:ModifyDate>2024-01-02T00:00:00Z</xmp:ModifyDate>"
		"</rdf:Description></rdf:RDF></x:xmpmeta><?xpacket end=\"w\"?>";
}
} // namespace

class InddProbeTests : public QObject
{
	Q_OBJECT

private slots:
	void acceptsLittleEndianAndNewestHeader();
	void acceptsBigEndian();
	void rejectsMalformedHeaders_data();
	void rejectsMalformedHeaders();
	void rejectsMissingFile();
	void probesOptionalLocalSamples();
	void comparesDatabaseBlocksWithoutInterpretingRecords();
	void comparesDifferentDatabaseSizes();
	void rejectsIncompatibleDatabaseComparisons();
	void comparesOptionalLocalVariants();
	void readsOpaqueRecordEnvelopes();
	void validatesOnlyMatchingPageLocalFooterSlots();
	void parsesOnlyValidatedShortHeaderVariant();
	void parsesDenseIndexedPagesWithFlaggedSlots();
	void parsesGuardedZeroStartIndexedPages();
	void rejectsMalformedRecordPages();
	void readsOptionalLocalRecordPage();
	void catalogsCandidatePagesWithoutClaimingFullCoverage();
	void mapsOpaqueFooterTokensWithoutClaimingLiveness();
	void tracesOnlyGuardedCandidateDirectoryRoots();
	void readsOptionalLocalCandidateDirectoryRoot();
	void validatesCandidatePageDirectory();
	void selectsOnlyTheRootReferencedDirectory();
	void readsOptionalLocalPageDirectory();
	void auditsMappedAndUnmappedRecordPages();
	void readsOptionalLocalPageDirectoryAudit();
	void catalogsOnlyDirectoryMappedRecordPages();
	void exposesOnlyValidatedDirectoryMappedSlots();
	void readsOptionalLocalMappedRecordCatalog();
	void catalogsOnlyGuardedCandidateFrameStoryShapes();
	void readsOptionalLocalCandidateFrameStoryPairs();
	void matchesOnlyValidatedCandidateType6RowTargets();
	void readsOptionalLocalCandidateType6Rows();
	void comparesOnlyValidatedSlotAddressesAcrossSaves();
	void readsOptionalLocalSlotDiff();
	void matchesOnlyUniqueExactCandidateRecordsAcrossSlots();
	void readsOptionalLocalExactRecordRelocations();
	void decodesOnlyBoundedCandidateStoryRuns();
	void readsOptionalLocalCandidateStory();
	void decodesOnlyGuardedCandidateAffineTails();
	void readsOptionalLocalCandidateAffine();
	void findsOnlyRawWordsInValidatedEnvelopes();
	void readsOptionalLocalRecordWordMatches();
	void survivesCandidateStoryMutations();
	void survivesRecordPageMutations();
	void extractsStructuredXmpMetadata();
	void skipsOtherContiguousObjects();
	void rejectsMalformedXmpObject();
	void readsOptionalLocalMetadata();
	void extractsSavedPagePreview();
	void rejectsInvalidPreview();
	void readsOptionalLocalPreview();
	void inventoriesValidatedXmpFields();
	void rejectsInvalidPageCount();
	void readsOptionalLocalInventory();
	void rejectsOversizedInventoryFields();
	void survivesDeterministicMutations();
};

void InddProbeTests::acceptsLittleEndianAndNewestHeader()
{
	const auto result = probeBytes(makeHeader(8, 7, 2) + makeHeader(9, 8, 2));
	QVERIFY(result.valid());
	QCOMPARE(result.formatVersion, quint32(9));
	QCOMPARE(result.streamByteOrder, quint8(1));
	QCOMPARE(result.activeHeaderSequence, quint64(8));
	QCOMPARE(result.databasePageCount, quint32(2));
	QCOMPARE(result.fileSize, qint64(8192));

	const auto firstActive = probeBytes(makeHeader(10, 9, 2) + makeHeader(11, 8, 2));
	QVERIFY(firstActive.valid());
	QCOMPARE(firstActive.formatVersion, quint32(10));
}

void InddProbeTests::acceptsBigEndian()
{
	const auto result = probeBytes(makeHeader(0x10203, 1, 2, false) + makeHeader(0x10204, 2, 2, false));
	QVERIFY(result.valid());
	QCOMPARE(result.formatVersion, quint32(0x10204));
	QCOMPARE(result.streamByteOrder, quint8(2));
}

void InddProbeTests::rejectsMalformedHeaders_data()
{
	QTest::addColumn<QByteArray>("bytes");
	QTest::addColumn<int>("expectedStatus");
	const QByteArray valid = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);

	QTest::newRow("truncated") << valid.left(8191) << int(Indd::ProbeStatus::TooShort);
	QByteArray bad = valid;
	bad[0] = '\0';
	QTest::newRow("first-signature") << bad << int(Indd::ProbeStatus::InvalidSignature);
	bad = valid;
	bad[4096] = '\0';
	QTest::newRow("second-signature") << bad << int(Indd::ProbeStatus::InvalidSignature);
	bad = valid;
	bad[16] = 'X';
	QTest::newRow("document-type") << bad << int(Indd::ProbeStatus::InvalidDocumentType);
	bad = valid;
	bad[24] = '\0';
	QTest::newRow("byte-order") << bad << int(Indd::ProbeStatus::InvalidByteOrder);
	bad = valid;
	put32(bad, 29, 0, true);
	QTest::newRow("version") << bad << int(Indd::ProbeStatus::InvalidVersion);
	bad = valid;
	put32(bad, 4096 + 280, 3, true);
	QTest::newRow("page-count-beyond-file") << bad << int(Indd::ProbeStatus::InvalidDatabasePageCount);
	bad = valid;
	put32(bad, 4096 + 280, 1, true);
	QTest::newRow("page-count-too-small") << bad << int(Indd::ProbeStatus::InvalidDatabasePageCount);
}

void InddProbeTests::rejectsMalformedHeaders()
{
	QFETCH(QByteArray, bytes);
	QFETCH(int, expectedStatus);
	const auto result = probeBytes(bytes);
	QCOMPARE(int(result.status), expectedStatus);
	QVERIFY(!result.valid());
}

void InddProbeTests::rejectsMissingFile()
{
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	const auto result = Indd::probeFile(tempDir.filePath("missing.indd"));
	QCOMPARE(result.status, Indd::ProbeStatus::IoError);
}

void InddProbeTests::probesOptionalLocalSamples()
{
	// The proprietary fixtures stay outside the source tree and CI.
	for (const char* variable : { "SCRIBUS_INDD_TEST_FILE", "SCRIBUS_INDD_TEST_FILE2" })
	{
		const QString path = qEnvironmentVariable(variable);
		if (path.isEmpty())
			continue;
		const auto result = Indd::probeFile(path);
		QVERIFY2(result.valid(), variable);
		QVERIFY(result.formatVersion > 0);
		QVERIFY(result.databasePageCount >= 2);
	}
}

void InddProbeTests::comparesDatabaseBlocksWithoutInterpretingRecords()
{
	const QByteArray headers = makeHeader(8, 1, 3) + makeHeader(8, 2, 3);
	const QByteArray first = headers + QByteArray(4096, '\0');
	QByteArray second = first;
	second[8192 + 17] = 'X';
	const auto changed = diffBytes(first, second);
	QVERIFY(changed.compared());
	QCOMPARE(changed.firstPageCount, quint32(3));
	QCOMPARE(changed.secondPageCount, quint32(3));
	QCOMPARE(changed.comparedPageCount, quint32(3));
	QCOMPARE(changed.changedPageIndices, QVector<quint32> { 2 });

	const auto unchanged = diffBytes(first, first);
	QVERIFY(unchanged.compared());
	QVERIFY(unchanged.changedPageIndices.isEmpty());
}

void InddProbeTests::comparesDifferentDatabaseSizes()
{
	const QByteArray first = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	const QByteArray second = makeHeader(8, 1, 3) + makeHeader(8, 2, 3) + QByteArray(4096, '\0');
	const auto result = diffBytes(first, second);
	QVERIFY(result.compared());
	QCOMPARE(result.firstPageCount, quint32(2));
	QCOMPARE(result.secondPageCount, quint32(3));
	QCOMPARE(result.comparedPageCount, quint32(2));
	QCOMPARE(result.changedPageIndices, (QVector<quint32> { 0, 1 }));
}

void InddProbeTests::rejectsIncompatibleDatabaseComparisons()
{
	const QByteArray valid = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	const QByteArray otherVersion = makeHeader(9, 1, 2) + makeHeader(9, 2, 2);
	QCOMPARE(diffBytes(valid, otherVersion).status, Indd::BlockDiffStatus::IncompatibleDocuments);
	const QByteArray otherEndianness = makeHeader(8, 1, 2, false) + makeHeader(8, 2, 2, false);
	QCOMPARE(diffBytes(valid, otherEndianness).status, Indd::BlockDiffStatus::IncompatibleDocuments);
	QCOMPARE(diffBytes(valid, valid.left(8191)).status, Indd::BlockDiffStatus::InvalidDocument);
	QByteArray invalid = valid;
	invalid[0] = '\0';
	QCOMPARE(diffBytes(valid, invalid).status, Indd::BlockDiffStatus::InvalidDocument);
}

void InddProbeTests::comparesOptionalLocalVariants()
{
	const QString firstPath = qEnvironmentVariable("SCRIBUS_INDD_TEST_FILE");
	const QString secondPath = qEnvironmentVariable("SCRIBUS_INDD_TEST_FILE2");
	if (firstPath.isEmpty() || secondPath.isEmpty())
		return;
	const auto result = Indd::compareDatabaseBlocks(firstPath, secondPath);
	// Physical prefix comparison is available across different database sizes,
	// but never across format versions or byte orders.
	const auto firstProbe = Indd::probeFile(firstPath);
	const auto secondProbe = Indd::probeFile(secondPath);
	if (firstProbe.formatVersion == secondProbe.formatVersion &&
		firstProbe.streamByteOrder == secondProbe.streamByteOrder)
	{
		const quint32 commonPages = qMin(firstProbe.databasePageCount, secondProbe.databasePageCount);
		if (commonPages > Indd::maxComparedDatabaseBlocks)
			QCOMPARE(result.status, Indd::BlockDiffStatus::LimitExceeded);
		else
		{
			QVERIFY(result.compared());
			QCOMPARE(result.firstPageCount, firstProbe.databasePageCount);
			QCOMPARE(result.secondPageCount, secondProbe.databasePageCount);
			QCOMPARE(result.comparedPageCount, commonPages);
			const QByteArray expectedCount = qgetenv("SCRIBUS_INDD_EXPECTED_CHANGED_BLOCKS");
			if (!expectedCount.isEmpty())
			{
				bool ok = false;
				const int count = expectedCount.toInt(&ok);
				QVERIFY(ok);
				QCOMPARE(result.changedPageIndices.size(), count);
			}
		}
	}
	else
		QCOMPARE(result.status, Indd::BlockDiffStatus::IncompatibleDocuments);
}

void InddProbeTests::readsOpaqueRecordEnvelopes()
{
	for (const bool littleEndian : { true, false })
	{
		QByteArray page(4096, '\0');
		put16(page, 0, 64, littleEndian);
		put16(page, 64, 32, littleEndian);
		put16(page, 66, 18, littleEndian);
		put32(page, 68, 0x262, littleEndian);
		put16(page, 96, 16, littleEndian);
		put32(page, 100, 0x2a0, littleEndian);
		const QByteArray headers = makeHeader(8, 1, 3, littleEndian) + makeHeader(8, 2, 3, littleEndian);
		const auto result = recordPageBytes(headers + page, 2);
		QVERIFY(result.parsed());
		QCOMPARE(result.records.size(), 2);
		QCOMPARE(result.records[0].offset, quint16(64));
		QCOMPARE(result.records[0].length, quint16(32));
		QCOMPARE(result.records[0].wordAt2, quint16(18));
		QCOMPARE(result.records[0].wordAt4, quint32(0x262));
		QCOMPARE(result.records[1].offset, quint16(96));
		QCOMPARE(result.records[1].wordAt2, quint16(0));
		QCOMPARE(result.records[1].wordAt4, quint32(0x2a0));
	}
}

void InddProbeTests::validatesOnlyMatchingPageLocalFooterSlots()
{
	const QByteArray headers = makeHeader(8, 1, 3) + makeHeader(8, 2, 3);
	QByteArray page(4096, '\0');
	put16(page, 0, 64, true);
	put16(page, 64, 32, true);
	put16(page, 66, 18, true);
	put16(page, 96, 3836, true); // Retired/unknown slot zero; chain ends at 0xf5c.
	put32(page, 0xf94, 64, true); // Footer entry for slot 18.
	const auto indexed = recordPageBytes(headers + page, 2);
	QVERIFY(indexed.parsed());
	QCOMPARE(indexed.records.size(), 2);
	QVERIFY(indexed.footerSlotIndexValidated);
	QVERIFY(indexed.records[0].slotIndexEntryMatches);
	QVERIFY(!indexed.records[1].slotIndexEntryMatches);
	const auto catalog = recordCatalogBytes(headers + page);
	QVERIFY(catalog.scanned());
	QCOMPARE(catalog.footerValidatedPageCount, quint32(1));
	QVERIFY(catalog.records[0].envelope.slotIndexEntryMatches);

	put32(page, 0xf94, 96, true);
	const auto mismatch = recordPageBytes(headers + page, 2);
	QVERIFY(mismatch.parsed());
	QVERIFY(!mismatch.footerSlotIndexValidated);
	QVERIFY(!mismatch.records[0].slotIndexEntryMatches);
	QCOMPARE(recordCatalogBytes(headers + page).footerValidatedPageCount, quint32(0));
	put32(page, 0xf94, 64, true);
	put16(page, 98, 18, true); // Duplicate slot cannot identify two envelopes.
	const auto duplicate = recordPageBytes(headers + page, 2);
	QVERIFY(duplicate.parsed());
	QVERIFY(!duplicate.footerSlotIndexValidated);

	QByteArray bigEndianPage(4096, '\0');
	put16(bigEndianPage, 0, 64, false);
	put16(bigEndianPage, 64, 32, false);
	put16(bigEndianPage, 66, 18, false);
	put16(bigEndianPage, 96, 3836, false);
	put32(bigEndianPage, 0xf94, 64, false);
	const QByteArray bigEndianHeaders = makeHeader(8, 1, 3, false) + makeHeader(8, 2, 3, false);
	const auto unsupported = recordPageBytes(bigEndianHeaders + bigEndianPage, 2);
	QVERIFY(unsupported.parsed());
	QVERIFY(!unsupported.footerSlotIndexValidated);
}

void InddProbeTests::parsesOnlyValidatedShortHeaderVariant()
{
	const QByteArray headers = makeHeader(8, 1, 3) + makeHeader(8, 2, 3);
	QByteArray page(4096, '\0');
	put16(page, 0, 20, true);
	put16(page, 20, 64, true);
	put16(page, 84, 3912, true); // The next byte is the slot table at 0xf9c.
	put16(page, 86, 1, true);
	put32(page, 4056, 84, true); // Slot 1 of 16.
	put32(page, 4060, 16, true);
	put32(page, 4084, 9, true);
	put32(page, 4088, 4, true);
	const auto result = recordPageBytes(headers + page, 2);
	QVERIFY(result.parsed());
	QCOMPARE(result.records.size(), 2);
	QVERIFY(result.footerSlotIndexValidated);
	QVERIFY(!result.records[0].slotIndexEntryMatches);
	QVERIFY(result.records[1].slotIndexEntryMatches);
	const auto catalog = recordCatalogBytes(headers + page);
	QVERIFY(catalog.scanned());
	QCOMPARE(catalog.footerValidatedPageCount, quint32(1));
	QCOMPARE(catalog.observedType9Footers.size(), 1);
	QVERIFY(catalog.observedType9Footers[0].envelopeChainParsed);

	put32(page, 4056, 20, true);
	QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
	put32(page, 4056, 84, true);
	put16(page, 84, 32, true);
	QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
	put16(page, 84, 3912, true);
	put32(page, 4084, 8, true);
	QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
}

void InddProbeTests::parsesDenseIndexedPagesWithFlaggedSlots()
{
	const QByteArray headers = makeHeader(8, 1, 3) + makeHeader(8, 2, 3);
	for (const quint16 slotCount : { quint16(16), quint16(32), quint16(48), quint16(64) })
	{
		QByteArray page(4096, '\0');
		const quint32 indexStart = 4060 - 4 * slotCount;
		const quint16 slot = slotCount == 16 ? quint16(2) : quint16(0x8000 | slotCount);
		put16(page, 0, 20, true);
		put16(page, 20, 56, true);
		put16(page, 22, slot, true);
		put16(page, 76, quint16(indexStart - 76), true);
		const quint32 entryOffset = indexStart + 4 * (slotCount - (slot & 0x7fff));
		put32(page, entryOffset, 20, true);
		put32(page, 4060, slotCount, true);
		put32(page, 4084, 9, true);
		put32(page, 4088, 4, true);
		const auto parsed = recordPageBytes(headers + page, 2);
		QVERIFY(parsed.parsed());
		QCOMPARE(parsed.records.size(), 2);
		QVERIFY(parsed.footerSlotIndexValidated);
		QVERIFY(parsed.records[0].slotIndexEntryMatches);
		QVERIFY(!parsed.records[1].slotIndexEntryMatches);
		put32(page, entryOffset, 76, true);
		QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
	}
	QByteArray noOccupiedSlot(4096, '\0');
	put16(noOccupiedSlot, 0, 1004, true);
	put16(noOccupiedSlot, 1004, 2992, true);
	put32(noOccupiedSlot, 4060, 16, true);
	put32(noOccupiedSlot, 4084, 9, true);
	put32(noOccupiedSlot, 4088, 20, true);
	QCOMPARE(recordPageBytes(headers + noOccupiedSlot, 2).status, Indd::RecordPageStatus::NotRecordPage);
}

void InddProbeTests::parsesGuardedZeroStartIndexedPages()
{
	const QByteArray headers = makeHeader(8, 1, 3) + makeHeader(8, 2, 3);
	for (const quint16 slotCount : { quint16(16), quint16(32), quint16(48), quint16(64) })
	{
		QByteArray page(4096, '\0');
		const quint32 indexStart = 4060 - 4 * slotCount;
		put16(page, 0, 120, true);
		put16(page, 2, 0x8001, true);
		put32(page, 4, 0x3709, true);
		put16(page, 120, quint16(indexStart - 120), true);
		put16(page, 122, 2, true);
		put32(page, indexStart + 4 * (slotCount - 2), 120, true);
		put32(page, 4060, slotCount, true);
		put32(page, 4084, 9, true);
		put32(page, 4088, 12, true);
		const auto parsed = recordPageBytes(headers + page, 2);
		QVERIFY(parsed.parsed());
		QVERIFY(parsed.footerSlotIndexValidated);
		QCOMPARE(parsed.records.size(), 2);
		QCOMPARE(parsed.records[0].offset, quint16(0));
		QCOMPARE(parsed.records[0].length, quint16(120));
		QVERIFY(parsed.records[0].slotIndexEntryMatches);
		QVERIFY(parsed.records[1].slotIndexEntryMatches);

		put32(page, indexStart + 4 * (slotCount - 1), 4, true);
		QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
		put32(page, indexStart + 4 * (slotCount - 1), 0, true);
		put16(page, 122, 1, true);
		QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
		put16(page, 122, 2, true);
		put16(page, 120, quint16(indexStart - 124), true);
		QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
	}
	QByteArray mapped = makeMappedSlotDocument();
	const int mappedPage = 10 * 4096;
	QByteArray zeroStartPage(4096, '\0');
	put16(zeroStartPage, 0, 120, true);
	put16(zeroStartPage, 2, 1, true);
	put16(zeroStartPage, 120, 3996 - 120, true);
	put32(zeroStartPage, 4060, 16, true);
	put32(zeroStartPage, 4084, 9, true);
	put32(zeroStartPage, 4088, 1, true);
	mapped.replace(mappedPage, 4096, zeroStartPage);
	const auto catalog = mappedRecordCatalogBytes(mapped);
	QVERIFY(catalog.cataloged());
	QCOMPARE(catalog.validatedSlots.size(), 1);
	QCOMPARE(catalog.validatedSlots[0].opaquePageToken, quint32(1));
	QCOMPARE(catalog.validatedSlots[0].pageLocalSlot, quint16(1));
	QCOMPARE(catalog.validatedSlots[0].envelopeOffset, quint16(0));
}

void InddProbeTests::rejectsMalformedRecordPages()
{
	const QByteArray headers = makeHeader(8, 1, 3) + makeHeader(8, 2, 3);
	QByteArray page(4096, '\0');
	put16(page, 0, 64, true);
	put16(page, 64, 32, true);
	QCOMPARE(recordPageBytes(headers + page, 0).status, Indd::RecordPageStatus::InvalidPageIndex);
	QCOMPARE(recordPageBytes(headers + page, 3).status, Indd::RecordPageStatus::InvalidPageIndex);
	QCOMPARE(recordPageBytes(headers.left(8191), 2).status, Indd::RecordPageStatus::InvalidDocument);
	QByteArray invalid = headers + page;
	invalid[0] = '\0';
	QCOMPARE(recordPageBytes(invalid, 2).status, Indd::RecordPageStatus::InvalidDocument);
	put16(page, 0, 63, true);
	QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
	put16(page, 0, 64, true);
	put16(page, 64, 4092, true);
	QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
	put16(page, 64, 6, true);
	QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
	put16(page, 64, 32, true);
	put16(page, 96, 13, true);
	QCOMPARE(recordPageBytes(headers + page, 2).status, Indd::RecordPageStatus::NotRecordPage);
}

void InddProbeTests::readsOptionalLocalRecordPage()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_RECORD_TEST_FILE");
	if (path.isEmpty())
		return;
	bool pageOk = false;
	const quint32 pageIndex = qEnvironmentVariable("SCRIBUS_INDD_RECORD_TEST_PAGE").toUInt(&pageOk);
	QVERIFY(pageOk);
	const auto result = Indd::inspectCandidateRecordPage(path, pageIndex);
	QVERIFY(result.parsed());
	const QByteArray expectedCount = qgetenv("SCRIBUS_INDD_RECORD_EXPECTED_COUNT");
	if (!expectedCount.isEmpty())
	{
		bool countOk = false;
		const int count = expectedCount.toInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(result.records.size(), count);
	}
	const QByteArray expectedFooterIndex = qgetenv("SCRIBUS_INDD_RECORD_EXPECTED_FOOTER_INDEX");
	if (!expectedFooterIndex.isEmpty())
	{
		QCOMPARE(expectedFooterIndex, QByteArray("1"));
		QVERIFY(result.footerSlotIndexValidated);
	}
	const auto catalog = Indd::catalogCandidateRecordPages(path);
	QVERIFY(catalog.scanned());
	QCOMPARE(catalog.candidatePageCount + catalog.skippedPageCount, catalog.databasePageCount - 2);
	const QByteArray expectedCandidatePages = qgetenv("SCRIBUS_INDD_CATALOG_EXPECTED_PAGES");
	if (!expectedCandidatePages.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedCandidatePages.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.candidatePageCount, count);
	}
	const QByteArray expectedCatalogRecords = qgetenv("SCRIBUS_INDD_CATALOG_EXPECTED_RECORDS");
	if (!expectedCatalogRecords.isEmpty())
	{
		bool countOk = false;
		const int count = expectedCatalogRecords.toInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.records.size(), count);
	}
	const QByteArray expectedFooterPages = qgetenv("SCRIBUS_INDD_CATALOG_EXPECTED_FOOTER_PAGES");
	if (!expectedFooterPages.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedFooterPages.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.footerValidatedPageCount, count);
	}
	const QByteArray expectedType9Pages = qgetenv("SCRIBUS_INDD_EXPECTED_TYPE9_PAGES");
	if (!expectedType9Pages.isEmpty())
	{
		bool countOk = false;
		const int count = expectedType9Pages.toInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.observedType9Footers.size(), count);
		QCOMPARE(catalog.duplicateFooterTokenCount, quint32(0));
	}
	const QByteArray expectedToken = qgetenv("SCRIBUS_INDD_EXPECTED_FOOTER_TOKEN");
	if (!expectedToken.isEmpty())
	{
		bool tokenOk = false;
		bool pageOk = false;
		const quint32 token = expectedToken.toUInt(&tokenOk);
		const quint32 page = qEnvironmentVariable("SCRIBUS_INDD_EXPECTED_FOOTER_PAGE").toUInt(&pageOk);
		QVERIFY(tokenOk);
		QVERIFY(pageOk);
		int matches = 0;
		for (const auto& footer : catalog.observedType9Footers)
		{
			if (footer.opaqueToken != token)
				continue;
			++matches;
			QCOMPARE(footer.databasePageIndex, page);
			QVERIFY(footer.envelopeChainParsed);
		}
		QCOMPARE(matches, 1);
	}
}

void InddProbeTests::catalogsCandidatePagesWithoutClaimingFullCoverage()
{
	QByteArray candidate(4096, '\0');
	put16(candidate, 0, 64, true);
	put16(candidate, 64, 32, true);
	put16(candidate, 66, 18, true);
	put16(candidate, 96, 16, true);
	QByteArray unsupported(4096, '\0');
	const QByteArray headers = makeHeader(8, 1, 4) + makeHeader(8, 2, 4);
	const QByteArray document = headers + candidate + unsupported;
	const auto result = recordCatalogBytes(document);
	QVERIFY(result.scanned());
	QCOMPARE(result.databasePageCount, quint32(4));
	QCOMPARE(result.candidatePageCount, quint32(1));
	QCOMPARE(result.skippedPageCount, quint32(1));
	QCOMPARE(result.records.size(), 2);
	QCOMPARE(result.records[0].databasePageIndex, quint32(2));
	QCOMPARE(result.records[0].envelope.offset, quint16(64));
	QCOMPARE(result.records[1].databasePageIndex, quint32(2));
	QCOMPARE(recordCatalogBytes(document, 3).status, Indd::RecordCatalogStatus::LimitExceeded);
	QCOMPARE(recordCatalogBytes(document, 4, 1).status, Indd::RecordCatalogStatus::LimitExceeded);
	QCOMPARE(recordCatalogBytes(document, 0).status, Indd::RecordCatalogStatus::LimitExceeded);
	QCOMPARE(recordCatalogBytes(document, 4, 0).status, Indd::RecordCatalogStatus::LimitExceeded);
	QCOMPARE(recordCatalogBytes(headers.left(8191)).status, Indd::RecordCatalogStatus::InvalidDocument);
	QByteArray bigEndianCandidate(4096, '\0');
	put16(bigEndianCandidate, 0, 64, false);
	put16(bigEndianCandidate, 64, 32, false);
	put16(bigEndianCandidate, 66, 18, false);
	const QByteArray bigEndianHeaders = makeHeader(8, 1, 3, false) + makeHeader(8, 2, 3, false);
	const auto bigEndian = recordCatalogBytes(bigEndianHeaders + bigEndianCandidate);
	QVERIFY(bigEndian.scanned());
	QCOMPARE(bigEndian.candidatePageCount, quint32(1));
	QCOMPARE(bigEndian.records.size(), 1);
	QCOMPARE(bigEndian.records[0].envelope.wordAt2, quint16(18));
}

void InddProbeTests::mapsOpaqueFooterTokensWithoutClaimingLiveness()
{
	QByteArray parsedPage(4096, '\0');
	put16(parsedPage, 0, 64, true);
	put16(parsedPage, 64, 32, true);
	put32(parsedPage, 4084, 9, true);
	put32(parsedPage, 4088, 19, true);
	QByteArray unsupportedPage(4096, '\0');
	put32(unsupportedPage, 4084, 9, true);
	put32(unsupportedPage, 4088, 4, true);
	const QByteArray headers = makeHeader(8, 1, 4) + makeHeader(8, 2, 4);
	const auto catalog = recordCatalogBytes(headers + parsedPage + unsupportedPage);
	QVERIFY(catalog.scanned());
	QCOMPARE(catalog.observedType9Footers.size(), 2);
	QCOMPARE(catalog.duplicateFooterTokenCount, quint32(0));
	QCOMPARE(catalog.observedType9Footers[0].databasePageIndex, quint32(2));
	QCOMPARE(catalog.observedType9Footers[0].opaqueToken, quint32(19));
	QVERIFY(catalog.observedType9Footers[0].envelopeChainParsed);
	QCOMPARE(catalog.observedType9Footers[1].databasePageIndex, quint32(3));
	QCOMPARE(catalog.observedType9Footers[1].opaqueToken, quint32(4));
	QVERIFY(!catalog.observedType9Footers[1].envelopeChainParsed);

	put32(unsupportedPage, 4088, 19, true);
	const auto collision = recordCatalogBytes(headers + parsedPage + unsupportedPage);
	QVERIFY(collision.scanned());
	QCOMPARE(collision.duplicateFooterTokenCount, quint32(1));

	const QByteArray bigEndianHeaders = makeHeader(8, 1, 3, false) + makeHeader(8, 2, 3, false);
	QByteArray bigEndianPage(4096, '\0');
	put16(bigEndianPage, 0, 64, false);
	put16(bigEndianPage, 64, 32, false);
	put32(bigEndianPage, 4084, 9, false);
	put32(bigEndianPage, 4088, 19, false);
	const auto unsupportedEndian = recordCatalogBytes(bigEndianHeaders + bigEndianPage);
	QVERIFY(unsupportedEndian.scanned());
	QVERIFY(unsupportedEndian.observedType9Footers.isEmpty());
}

void InddProbeTests::tracesOnlyGuardedCandidateDirectoryRoots()
{
	QByteArray document = makeHeader(8, 1, 12) + makeHeader(8, 2, 12) + QByteArray(10 * 4096, '\0');
	put32(document, 936, 7, true);
	put32(document, 4096 + 936, 7, true);
	put32(document, 7 * 4096 + 128, 9, true);
	put32(document, 7 * 4096 + 4084, 4, true);
	put32(document, 7 * 4096 + 4088, 8, true);
	put32(document, 9 * 4096 + 4084, 5, true);
	put32(document, 9 * 4096 + 4088, 10, true);
	const auto first = candidateDirectoryRootBytes(document);
	QVERIFY(first.candidate());
	QCOMPARE(first.headerRootPageIndex, quint32(7));
	QCOMPARE(first.directoryPageIndex, quint32(9));

	put32(document, 8 * 4096 + 128, 10, true);
	put32(document, 8 * 4096 + 4084, 4, true);
	put32(document, 8 * 4096 + 4088, 7, true);
	put32(document, 10 * 4096 + 4084, 5, true);
	put32(document, 10 * 4096 + 4088, 9, true);
	put32(document, 4096 + 936, 8, true); // Newer header chooses the other root.
	const auto second = candidateDirectoryRootBytes(document);
	QVERIFY(second.candidate());
	QCOMPARE(second.headerRootPageIndex, quint32(8));
	QCOMPARE(second.directoryPageIndex, quint32(10));

	QByteArray invalid = document;
	put32(invalid, 4096 + 936, 12, true);
	QCOMPARE(candidateDirectoryRootBytes(invalid).status,
		Indd::CandidateDirectoryRootStatus::UnsupportedLayout);
	invalid = document;
	put32(invalid, 8 * 4096 + 4088, 8, true);
	QCOMPARE(candidateDirectoryRootBytes(invalid).status,
		Indd::CandidateDirectoryRootStatus::UnsupportedLayout);
	invalid = document;
	put32(invalid, 8 * 4096 + 128, 11, true);
	QCOMPARE(candidateDirectoryRootBytes(invalid).status,
		Indd::CandidateDirectoryRootStatus::UnsupportedLayout);
	invalid = document;
	put32(invalid, 10 * 4096 + 4084, 9, true);
	QCOMPARE(candidateDirectoryRootBytes(invalid).status,
		Indd::CandidateDirectoryRootStatus::UnsupportedLayout);
	QCOMPARE(candidateDirectoryRootBytes(document.left(8191)).status,
		Indd::CandidateDirectoryRootStatus::InvalidDocument);
	const QByteArray bigEndian = makeHeader(8, 1, 12, false) + makeHeader(8, 2, 12, false) + QByteArray(10 * 4096, '\0');
	QCOMPARE(candidateDirectoryRootBytes(bigEndian).status,
		Indd::CandidateDirectoryRootStatus::UnsupportedLayout);
}

void InddProbeTests::readsOptionalLocalCandidateDirectoryRoot()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_ROOT_TEST_FILE");
	if (path.isEmpty())
		return;
	const auto root = Indd::inspectCandidateDirectoryRoot(path);
	QVERIFY(root.candidate());
	const QByteArray expectedDirectory = qgetenv("SCRIBUS_INDD_ROOT_EXPECTED_DIRECTORY_PAGE");
	if (!expectedDirectory.isEmpty())
	{
		bool ok = false;
		const quint32 page = expectedDirectory.toUInt(&ok);
		QVERIFY(ok);
		QCOMPARE(root.directoryPageIndex, page);
	}
	qInfo().nospace() << "Candidate root page " << root.headerRootPageIndex
		<< " -> directory page " << root.directoryPageIndex;
}

void InddProbeTests::validatesCandidatePageDirectory()
{
	QByteArray document = makeHeader(8, 1, 12) + makeHeader(8, 2, 12) + QByteArray(10 * 4096, '\0');
	setSyntheticCandidateRoot(document);
	put32(document, 9 * 4096, 986, true); // 988 table slots, two populated.
	put32(document, 9 * 4096 + 128 + 4, 10, true);
	put32(document, 9 * 4096 + 128 + 8, 11, true);
	put32(document, 9 * 4096 + 4084, 5, true);
	put32(document, 9 * 4096 + 4088, 10, true);
	put32(document, 10 * 4096 + 4084, 9, true);
	put32(document, 10 * 4096 + 4088, 1, true);
	put32(document, 11 * 4096 + 4084, 6, true);
	put32(document, 11 * 4096 + 4088, 2, true);
	const auto valid = pageDirectoryBytes(document);
	QVERIFY(valid.validated());
	QCOMPARE(valid.declaredEmptySlots, quint32(986));
	QCOMPARE(valid.headerRootPageIndex, quint32(7));
	QCOMPARE(valid.directoryPageIndex, quint32(9));
	QCOMPARE(valid.entries.size(), 2);
	QCOMPARE(valid.entries[0].opaqueToken, quint32(1));
	QCOMPARE(valid.entries[0].databasePageIndex, quint32(10));
	QCOMPARE(valid.entries[0].footerType, quint32(9));
	QCOMPARE(valid.entries[1].opaqueToken, quint32(2));
	QCOMPARE(valid.entries[1].databasePageIndex, quint32(11));
	QCOMPARE(valid.entries[1].footerType, quint32(6));

	QByteArray invalid = document;
	put32(invalid, 9 * 4096, 987, true);
	QCOMPARE(pageDirectoryBytes(invalid).status, Indd::PageDirectoryStatus::InvalidMapping);
	invalid = document;
	put32(invalid, 9 * 4096, 988, true);
	put32(invalid, 9 * 4096 + 128 + 4, 0, true);
	put32(invalid, 9 * 4096 + 128 + 8, 0, true);
	QCOMPARE(pageDirectoryBytes(invalid).status, Indd::PageDirectoryStatus::InvalidMapping);
	invalid = document;
	put32(invalid, 11 * 4096 + 4088, 3, true);
	QCOMPARE(pageDirectoryBytes(invalid).status, Indd::PageDirectoryStatus::InvalidMapping);
	invalid = document;
	put32(invalid, 9 * 4096 + 128 + 8, 10, true);
	QCOMPARE(pageDirectoryBytes(invalid).status, Indd::PageDirectoryStatus::InvalidMapping);
	invalid = document;
	put32(invalid, 9 * 4096 + 128 + 8, 12, true);
	QCOMPARE(pageDirectoryBytes(invalid).status, Indd::PageDirectoryStatus::InvalidMapping);
	invalid = document;
	put32(invalid, 9 * 4096 + 4084, 9, true);
	QCOMPARE(pageDirectoryBytes(invalid).status, Indd::PageDirectoryStatus::UnsupportedLayout);
	QCOMPARE(pageDirectoryBytes(document.left(8191)).status, Indd::PageDirectoryStatus::InvalidDocument);
	const QByteArray bigEndian = makeHeader(8, 1, 12, false) + makeHeader(8, 2, 12, false) + QByteArray(10 * 4096, '\0');
	QCOMPARE(pageDirectoryBytes(bigEndian).status, Indd::PageDirectoryStatus::UnsupportedLayout);
}

void InddProbeTests::selectsOnlyTheRootReferencedDirectory()
{
	QByteArray document = makeHeader(8, 1, 13) + makeHeader(8, 2, 13) + QByteArray(11 * 4096, '\0');
	setSyntheticCandidateRoot(document);
	put32(document, 9 * 4096, 987, true);
	put32(document, 9 * 4096 + 128 + 4, 12, true);
	put32(document, 9 * 4096 + 4084, 5, true);
	put32(document, 9 * 4096 + 4088, 10, true);
	put32(document, 8 * 4096 + 128, 10, true);
	put32(document, 8 * 4096 + 4084, 4, true);
	put32(document, 8 * 4096 + 4088, 7, true);
	put32(document, 10 * 4096, 987, true);
	put32(document, 10 * 4096 + 128 + 4, 11, true);
	put32(document, 10 * 4096 + 4084, 5, true);
	put32(document, 10 * 4096 + 4088, 9, true);
	for (int page = 11; page <= 12; ++page)
	{
		put16(document, page * 4096, 64, true);
		put16(document, page * 4096 + 64, 32, true);
		put32(document, page * 4096 + 4084, 9, true);
		put32(document, page * 4096 + 4088, 1, true);
	}
	put32(document, 4096 + 936, 8, true);
	const auto selected = pageDirectoryBytes(document);
	QVERIFY(selected.validated());
	QCOMPARE(selected.headerRootPageIndex, quint32(8));
	QCOMPARE(selected.directoryPageIndex, quint32(10));
	QCOMPARE(selected.entries.size(), 1);
	QCOMPARE(selected.entries[0].databasePageIndex, quint32(11));
	const auto catalog = mappedRecordCatalogBytes(document);
	QVERIFY(catalog.cataloged());
	QCOMPARE(catalog.records.size(), 1);
	QCOMPARE(catalog.records[0].databasePageIndex, quint32(11));

	QByteArray invalid = document;
	put32(invalid, 10 * 4096 + 128 + 4, 13, true);
	QCOMPARE(pageDirectoryBytes(invalid).status, Indd::PageDirectoryStatus::InvalidMapping);
	QCOMPARE(mappedRecordCatalogBytes(invalid).status, Indd::PageDirectoryStatus::InvalidMapping);
	invalid = document;
	put32(invalid, 8 * 4096 + 128, 11, true);
	QCOMPARE(pageDirectoryBytes(invalid).status, Indd::PageDirectoryStatus::UnsupportedLayout);
}

void InddProbeTests::readsOptionalLocalPageDirectory()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_DIRECTORY_TEST_FILE");
	if (path.isEmpty())
		return;
	const auto result = Indd::inspectCandidatePageDirectory(path);
	QVERIFY(result.validated());
	const QByteArray expectedDirectory = qgetenv("SCRIBUS_INDD_DIRECTORY_EXPECTED_PAGE");
	if (!expectedDirectory.isEmpty())
	{
		bool pageOk = false;
		const quint32 page = expectedDirectory.toUInt(&pageOk);
		QVERIFY(pageOk);
		QCOMPARE(result.directoryPageIndex, page);
	}
	const QByteArray expectedCount = qgetenv("SCRIBUS_INDD_DIRECTORY_EXPECTED_ENTRIES");
	if (!expectedCount.isEmpty())
	{
		bool countOk = false;
		const int count = expectedCount.toInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(result.entries.size(), count);
	}
	const QByteArray expectedPhysicalPage = qgetenv("SCRIBUS_INDD_DIRECTORY_EXPECTED_TOKEN19_PAGE");
	if (!expectedPhysicalPage.isEmpty())
	{
		bool pageOk = false;
		const quint32 page = expectedPhysicalPage.toUInt(&pageOk);
		QVERIFY(pageOk);
		int matches = 0;
		for (const auto& entry : result.entries)
		{
			if (entry.opaqueToken != 19)
				continue;
			++matches;
			QCOMPARE(entry.databasePageIndex, page);
			QCOMPARE(entry.footerType, quint32(9));
		}
		QCOMPARE(matches, 1);
	}
}

void InddProbeTests::auditsMappedAndUnmappedRecordPages()
{
	QByteArray document = makeHeader(8, 1, 12) + makeHeader(8, 2, 12) + QByteArray(10 * 4096, '\0');
	setSyntheticCandidateRoot(document);
	put32(document, 9 * 4096, 987, true);
	put32(document, 9 * 4096 + 128 + 4, 10, true);
	put32(document, 9 * 4096 + 4084, 5, true);
	put32(document, 9 * 4096 + 4088, 10, true);
	for (int physicalPage = 10; physicalPage <= 11; ++physicalPage)
	{
		put16(document, physicalPage * 4096, 64, true);
		put16(document, physicalPage * 4096 + 64, 32, true);
		put32(document, physicalPage * 4096 + 4084, 9, true);
		put32(document, physicalPage * 4096 + 4088, physicalPage - 9, true);
	}
	const auto audit = pageDirectoryAuditBytes(document);
	QVERIFY(audit.audited());
	QCOMPARE(audit.mappedType9PageCount, quint32(1));
	QCOMPARE(audit.unmappedType9PageCount, quint32(1));
	QCOMPARE(audit.mappedParsedPageCount, quint32(1));
	QCOMPARE(audit.unmappedParsedPageCount, quint32(1));

	put32(document, 9 * 4096 + 128 + 4, 11, true);
	QCOMPARE(pageDirectoryAuditBytes(document).status, Indd::PageDirectoryStatus::InvalidMapping);
}

void InddProbeTests::readsOptionalLocalPageDirectoryAudit()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_AUDIT_TEST_FILE");
	if (path.isEmpty())
		return;
	const auto audit = Indd::auditCandidatePageDirectory(path);
	QVERIFY(audit.audited());
	const QByteArray expectedMapped = qgetenv("SCRIBUS_INDD_AUDIT_EXPECTED_MAPPED_TYPE9");
	if (!expectedMapped.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedMapped.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(audit.mappedType9PageCount, count);
	}
	const QByteArray expectedUnmapped = qgetenv("SCRIBUS_INDD_AUDIT_EXPECTED_UNMAPPED_TYPE9");
	if (!expectedUnmapped.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedUnmapped.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(audit.unmappedType9PageCount, count);
	}
}

void InddProbeTests::catalogsOnlyDirectoryMappedRecordPages()
{
	QByteArray document = makeHeader(8, 1, 13) + makeHeader(8, 2, 13) + QByteArray(11 * 4096, '\0');
	setSyntheticCandidateRoot(document);
	put32(document, 9 * 4096, 986, true);
	put32(document, 9 * 4096 + 128 + 4, 10, true);
	put32(document, 9 * 4096 + 128 + 8, 11, true);
	put32(document, 9 * 4096 + 4084, 5, true);
	put32(document, 9 * 4096 + 4088, 10, true);
	for (int physicalPage = 10; physicalPage <= 12; ++physicalPage)
	{
		put32(document, physicalPage * 4096 + 4084, 9, true);
		put32(document, physicalPage * 4096 + 4088, physicalPage - 9, true);
	}
	put16(document, 10 * 4096, 64, true);
	put16(document, 10 * 4096 + 64, 32, true);
	put16(document, 10 * 4096 + 96, 16, true);
	put16(document, 12 * 4096, 64, true);
	put16(document, 12 * 4096 + 64, 32, true); // Valid, but not mapped.
	const auto catalog = mappedRecordCatalogBytes(document);
	QVERIFY(catalog.cataloged());
	QCOMPARE(catalog.mappedType9PageCount, quint32(2));
	QCOMPARE(catalog.parsedPageCount, quint32(1));
	QCOMPARE(catalog.unsupportedPageCount, quint32(1));
	QCOMPARE(catalog.records.size(), 2);
	QCOMPARE(catalog.records[0].databasePageIndex, quint32(10));
	QCOMPARE(catalog.records[1].databasePageIndex, quint32(10));
	QCOMPARE(mappedRecordCatalogBytes(document, 1).status, Indd::PageDirectoryStatus::LimitExceeded);
	QCOMPARE(mappedRecordCatalogBytes(document, 0).status, Indd::PageDirectoryStatus::LimitExceeded);
	QByteArray ambiguous = document;
	put32(ambiguous, 12 * 4096 + 4084, 5, true);
	put32(ambiguous, 12 * 4096 + 4088, 9, true);
	QVERIFY(mappedRecordCatalogBytes(ambiguous).cataloged()); // Unselected type-5 page is not the root.

	put32(document, 9 * 4096, 985, true);
	QCOMPARE(mappedRecordCatalogBytes(document).status, Indd::PageDirectoryStatus::InvalidMapping);
}

void InddProbeTests::exposesOnlyValidatedDirectoryMappedSlots()
{
	QByteArray document = makeHeader(8, 1, 13) + makeHeader(8, 2, 13) + QByteArray(11 * 4096, '\0');
	setSyntheticCandidateRoot(document);
	put32(document, 9 * 4096, 986, true);
	put32(document, 9 * 4096 + 128 + 4, 10, true);
	put32(document, 9 * 4096 + 128 + 8, 11, true);
	put32(document, 9 * 4096 + 4084, 5, true);
	put32(document, 9 * 4096 + 4088, 10, true);
	for (int physicalPage = 10; physicalPage <= 12; ++physicalPage)
	{
		put32(document, physicalPage * 4096 + 4084, 9, true);
		put32(document, physicalPage * 4096 + 4088, physicalPage - 9, true);
	}
	const int indexedPage = 10 * 4096;
	put16(document, indexedPage, 20, true);
	put16(document, indexedPage + 20, 56, true);
	put16(document, indexedPage + 22, 0x8002, true);
	put16(document, indexedPage + 76, 3920, true); // Slot zero is not a validated handle.
	put32(document, indexedPage + 4052, 20, true); // Slot 2 of 16.
	put32(document, indexedPage + 4048, 4, true); // Opaque non-pointer table value.
	put32(document, indexedPage + 4060, 16, true);
	put16(document, 11 * 4096, 64, true);
	put16(document, 11 * 4096 + 64, 32, true); // Parsed, but no validated slot index.
	put16(document, 12 * 4096, 64, true);
	put16(document, 12 * 4096 + 64, 32, true); // Unmapped page is excluded.

	const auto catalog = mappedRecordCatalogBytes(document);
	QVERIFY(catalog.cataloged());
	QCOMPARE(catalog.mappedType9PageCount, quint32(2));
	QCOMPARE(catalog.parsedPageCount, quint32(2));
	QCOMPARE(catalog.records.size(), 3);
	QCOMPARE(catalog.unindexedEnvelopeCount, quint32(2));
	QCOMPARE(catalog.validatedSlots.size(), 1);
	const auto& slot = catalog.validatedSlots[0];
	QCOMPARE(slot.opaquePageToken, quint32(1));
	QCOMPARE(slot.pageLocalSlot, quint16(2));
	QCOMPARE(slot.databasePageIndex, quint32(10));
	QCOMPARE(slot.envelopeOffset, quint16(20));
	QCOMPARE(slot.envelopeLength, quint16(56));

	put32(document, indexedPage + 4052, 76, true);
	const auto invalidPointer = mappedRecordCatalogBytes(document);
	QVERIFY(invalidPointer.cataloged());
	QCOMPARE(invalidPointer.parsedPageCount, quint32(1));
	QCOMPARE(invalidPointer.unsupportedPageCount, quint32(1));
	QVERIFY(invalidPointer.validatedSlots.isEmpty());
}

void InddProbeTests::readsOptionalLocalMappedRecordCatalog()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_MAPPED_CATALOG_TEST_FILE");
	if (path.isEmpty())
		return;
	const auto catalog = Indd::catalogDirectoryMappedRecordPages(path);
	if (qEnvironmentVariable("SCRIBUS_INDD_MAPPED_CATALOG_EXPECT_UNSUPPORTED_LAYOUT") == QLatin1String("1"))
	{
		QCOMPARE(catalog.status, Indd::PageDirectoryStatus::UnsupportedLayout);
		QVERIFY(catalog.records.isEmpty());
		QVERIFY(catalog.validatedSlots.isEmpty());
		return;
	}
	QVERIFY(catalog.cataloged());
	QCOMPARE(catalog.parsedPageCount + catalog.unsupportedPageCount, catalog.mappedType9PageCount);
	QCOMPARE(catalog.validatedSlots.size() + qsizetype(catalog.unindexedEnvelopeCount), catalog.records.size());
	const QByteArray expectedMapped = qgetenv("SCRIBUS_INDD_MAPPED_CATALOG_EXPECTED_PAGES");
	if (!expectedMapped.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedMapped.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.mappedType9PageCount, count);
	}
	const QByteArray expectedParsed = qgetenv("SCRIBUS_INDD_MAPPED_CATALOG_EXPECTED_PARSED");
	if (!expectedParsed.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedParsed.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.parsedPageCount, count);
	}
	const QByteArray expectedUnsupported = qgetenv("SCRIBUS_INDD_MAPPED_CATALOG_EXPECTED_UNSUPPORTED");
	if (!expectedUnsupported.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedUnsupported.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.unsupportedPageCount, count);
	}
	const QByteArray expectedRecords = qgetenv("SCRIBUS_INDD_MAPPED_CATALOG_EXPECTED_RECORDS");
	if (!expectedRecords.isEmpty())
	{
		bool countOk = false;
		const int count = expectedRecords.toInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.records.size(), count);
	}
	const QByteArray expectedSlots = qgetenv("SCRIBUS_INDD_MAPPED_CATALOG_EXPECTED_SLOTS");
	if (!expectedSlots.isEmpty())
	{
		bool countOk = false;
		const int count = expectedSlots.toInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(catalog.validatedSlots.size(), count);
	}
	const QByteArray zeroSlotToken = qgetenv("SCRIBUS_INDD_MAPPED_CATALOG_EXPECTED_ZERO_SLOT_TOKEN");
	if (!zeroSlotToken.isEmpty())
	{
		bool tokenOk = false;
		const quint32 token = zeroSlotToken.toUInt(&tokenOk);
		QVERIFY(tokenOk);
		const auto found = std::find_if(catalog.validatedSlots.cbegin(), catalog.validatedSlots.cend(),
			[token](const Indd::DirectoryMappedRecordSlotReference& slot) {
				return slot.opaquePageToken == token && slot.pageLocalSlot == 1 && slot.envelopeOffset == 0;
			});
		QVERIFY(found != catalog.validatedSlots.cend());
	}
}

void InddProbeTests::catalogsOnlyGuardedCandidateFrameStoryShapes()
{
	const QByteArray first = makeCandidateFrameStoryDocument(true);
	const auto firstPairs = candidateFrameStoryBytes(first);
	QVERIFY(firstPairs.cataloged());
	QCOMPARE(firstPairs.scannedValidatedSlotCount, quint32(1));
	QCOMPARE(firstPairs.unindexedEnvelopeCount, quint32(1));
	QCOMPARE(firstPairs.rejectedShapeCount, quint32(0));
	QCOMPARE(firstPairs.references.size(), 1);
	QCOMPARE(firstPairs.references[0].opaquePageToken, quint32(1));
	QCOMPARE(firstPairs.references[0].pageLocalSlot, quint16(2));
	QCOMPARE(firstPairs.references[0].frameCandidateWord, quint32(0xfd));
	QCOMPARE(firstPairs.references[0].storyCandidateWord, quint32(0xe8));
	const auto secondPairs = candidateFrameStoryBytes(makeCandidateFrameStoryDocument(false));
	QVERIFY(secondPairs.cataloged());
	QCOMPARE(secondPairs.references.size(), 1);
	QCOMPARE(secondPairs.references[0].frameCandidateWord, quint32(0xfd));
	QCOMPARE(secondPairs.references[0].storyCandidateWord, quint32(0xe8));

	QByteArray invalid = first;
	put32(invalid, 10 * 4096 + 20 + 28, 0xe9, true);
	const auto duplicateMismatch = candidateFrameStoryBytes(invalid);
	QVERIFY(duplicateMismatch.cataloged());
	QCOMPARE(duplicateMismatch.rejectedShapeCount, quint32(1));
	QVERIFY(duplicateMismatch.references.isEmpty());
	invalid = makeCandidateFrameStoryDocument(false);
	put32(invalid, 10 * 4096 + 20 + 86, 0x2ac, true);
	const auto nestedMismatch = candidateFrameStoryBytes(invalid);
	QVERIFY(nestedMismatch.cataloged());
	QCOMPARE(nestedMismatch.rejectedShapeCount, quint32(1));
	QVERIFY(nestedMismatch.references.isEmpty());
	invalid = first;
	put32(invalid, 10 * 4096 + 20 + 4, 0x262, true);
	const auto otherKind = candidateFrameStoryBytes(invalid);
	QVERIFY(otherKind.cataloged());
	QCOMPARE(otherKind.rejectedShapeCount, quint32(0));
	QVERIFY(otherKind.references.isEmpty());
	invalid = first;
	put32(invalid, 9 * 4096 + 128 + 4, 11, true);
	QCOMPARE(candidateFrameStoryBytes(invalid).status, Indd::PageDirectoryStatus::InvalidMapping);
	QByteArray unselectedOldPair = first;
	unselectedOldPair.replace(11 * 4096, 4096, first.mid(10 * 4096, 4096));
	put32(unselectedOldPair, 11 * 4096 + 20 + 4, 0x262, true);
	put32(unselectedOldPair, 9 * 4096 + 128 + 4, 11, true);
	const auto selectedOnly = candidateFrameStoryBytes(unselectedOldPair);
	QVERIFY(selectedOnly.cataloged());
	QVERIFY(selectedOnly.references.isEmpty()); // The old, unselected page still has the pair.
	QCOMPARE(candidateFrameStoryBytes(first, 0).status, Indd::PageDirectoryStatus::LimitExceeded);
}

void InddProbeTests::readsOptionalLocalCandidateFrameStoryPairs()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_PAIR_TEST_FILE");
	if (path.isEmpty())
		return;
	const auto pairs = Indd::catalogCandidateFrameStoryReferences(path);
	QVERIFY(pairs.cataloged());
	const QByteArray expectedCount = qgetenv("SCRIBUS_INDD_PAIR_EXPECTED_COUNT");
	if (!expectedCount.isEmpty())
	{
		bool ok = false;
		const int count = expectedCount.toInt(&ok);
		QVERIFY(ok);
		QCOMPARE(pairs.references.size(), count);
	}
	qInfo().nospace() << "Candidate pairs=" << pairs.references.size()
		<< " scanned-slots=" << pairs.scannedValidatedSlotCount
		<< " rejected-shapes=" << pairs.rejectedShapeCount
		<< " unsupported-pages=" << pairs.unsupportedPageCount
		<< " unindexed-envelopes=" << pairs.unindexedEnvelopeCount;
	for (const auto& pair : pairs.references)
		qInfo().nospace() << "  " << pair.opaquePageToken << "/" << pair.pageLocalSlot
			<< " frame-word=" << Qt::hex << pair.frameCandidateWord
			<< " story-word=" << pair.storyCandidateWord << Qt::dec;
}

void InddProbeTests::matchesOnlyValidatedCandidateType6RowTargets()
{
	constexpr quint32 key = 0xfd;
	QByteArray document = makeMappedSlotDocument();
	const int directory = 9 * 4096;
	const int source = 11 * 4096;
	put32(document, directory, 986, true);
	put32(document, directory + 128 + 8, 11, true); // Token 2.
	put32(document, source, 3, true);
	put32(document, source + 8, key, true);
	put32(document, source + 12, (2u << 16) | 52u, true); // 56-byte target minus four.
	put32(document, source + 16, 1, true);
	put32(document, source + 20, 1, true);
	put32(document, source + 24, key, true);
	put32(document, source + 28, (2u << 16) | 52u, true);
	put32(document, source + 32, 1, true);
	put32(document, source + 36, 0, true); // Similar row, wrong marker.
	put32(document, source + 40, 0x115, true);
	put32(document, source + 44, (2u << 16) | 52u, true);
	put32(document, source + 48, 1, true);
	put32(document, source + 52, 1, true);
	put32(document, source + 4084, 6, true);
	put32(document, source + 4088, 2, true);
	put32(document, 10 * 4096 + 24, 0x3709, true);
	put32(document, 10 * 4096 + 32, 0xfa, true);

	const auto found = candidateType6RowsBytes(document, key);
	QVERIFY(found.searched());
	QCOMPARE(found.scannedType6PageCount, quint32(1));
	QCOMPARE(found.rejectedMatchingRowCount, quint32(1));
	QCOMPARE(found.targets.size(), 1);
	QCOMPARE(found.targets[0].sourcePageToken, quint32(2));
	QCOMPARE(found.targets[0].sourceRowOffset, quint16(8));
	QCOMPARE(found.targets[0].targetPageToken, quint32(1));
	QCOMPARE(found.targets[0].targetPageLocalSlot, quint16(2));
	QCOMPARE(found.targets[0].targetEnvelopeLength, quint16(56));
	QCOMPARE(found.targets[0].targetRecordWordAt4, quint32(0x3709));
	QVERIFY(found.targets[0].targetHasWordAt12);
	QCOMPARE(found.targets[0].targetRecordWordAt12, quint32(0xfa));
	QCOMPARE(candidateType6RowsBytes(document, 0x115).targets.size(), 1);
	QCOMPARE(candidateType6RowsBytes(document, 0x116).targets.size(), 0);
	QCOMPARE(candidateType6RowsBytes(document, key, 0).status, Indd::PageDirectoryStatus::LimitExceeded);
	QCOMPARE(candidateType6RowsBytes(document, key, 4097).status, Indd::PageDirectoryStatus::LimitExceeded);
	QCOMPARE(candidateType6RowsBytes(document, key, 4096, 0).status, Indd::PageDirectoryStatus::LimitExceeded);
	QByteArray padded = document;
	put16(padded, 10 * 4096 + 20, 180, true);
	put16(padded, 10 * 4096 + 200, 3796, true);
	put32(padded, source + 12, (2u << 16) | 174u, true); // 178 bytes, aligned to 180.
	const auto aligned = candidateType6RowsBytes(padded, key);
	QVERIFY(aligned.searched());
	QCOMPARE(aligned.targets.size(), 1);
	QCOMPARE(aligned.targets[0].targetEnvelopeLength, quint16(180));
	put32(padded, source + 12, (2u << 16) | 172u, true); // 176 does not align to 180.
	QVERIFY(candidateType6RowsBytes(padded, key).targets.isEmpty());

	QByteArray invalid = document;
	put32(invalid, source + 12, (2u << 16) | 48u, true);
	QCOMPARE(candidateType6RowsBytes(invalid, key).rejectedMatchingRowCount, quint32(2));
	QVERIFY(candidateType6RowsBytes(invalid, key).targets.isEmpty());
	invalid = document;
	put32(invalid, source + 16, 3, true); // Target token is not mapped.
	QCOMPARE(candidateType6RowsBytes(invalid, key).rejectedMatchingRowCount, quint32(2));
	invalid = document;
	put32(invalid, source, 255, true); // Count cannot fit before the footer.
	const auto unsupported = candidateType6RowsBytes(invalid, key);
	QVERIFY(unsupported.searched());
	QCOMPARE(unsupported.unsupportedType6PageCount, quint32(1));
	QVERIFY(unsupported.targets.isEmpty());
	invalid = document;
	put32(invalid, 10 * 4096 + 4052, 76, true); // Target slot back-pointer is wrong.
	const auto unindexed = candidateType6RowsBytes(invalid, key);
	QVERIFY(unindexed.searched());
	QCOMPARE(unindexed.unsupportedType9PageCount, quint32(1));
	QVERIFY(unindexed.targets.isEmpty());
	invalid = document;
	put32(invalid, source + 4088, 3, true);
	QCOMPARE(candidateType6RowsBytes(invalid, key).status, Indd::PageDirectoryStatus::InvalidMapping);
	invalid = document;
	put32(invalid, source + 36, 1, true); // Two matching validated rows exceed the cap.
	QCOMPARE(candidateType6RowsBytes(invalid, key, 1).status, Indd::PageDirectoryStatus::LimitExceeded);
}

void InddProbeTests::readsOptionalLocalCandidateType6Rows()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_TYPE6_TEST_FILE");
	if (path.isEmpty())
		return;
	bool keyOk = false;
	const quint32 key = qEnvironmentVariable("SCRIBUS_INDD_TYPE6_TEST_KEY").toUInt(&keyOk, 0);
	QVERIFY(keyOk);
	const auto found = Indd::findCandidateType6RowTargets(path, key);
	QVERIFY(found.searched());
	const QByteArray expectedCount = qgetenv("SCRIBUS_INDD_TYPE6_EXPECTED_MATCH_COUNT");
	if (!expectedCount.isEmpty())
	{
		bool countOk = false;
		const int count = expectedCount.toInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(found.targets.size(), count);
	}
	const QByteArray expectedToken = qgetenv("SCRIBUS_INDD_TYPE6_EXPECTED_TARGET_TOKEN");
	const QByteArray expectedSlot = qgetenv("SCRIBUS_INDD_TYPE6_EXPECTED_TARGET_SLOT");
	const Indd::CandidateType6RowTarget* selectedTarget = nullptr;
	if (!expectedToken.isEmpty() && !expectedSlot.isEmpty())
	{
		bool tokenOk = false;
		bool slotOk = false;
		const quint32 token = expectedToken.toUInt(&tokenOk);
		const quint16 slot = expectedSlot.toUShort(&slotOk);
		QVERIFY(tokenOk && slotOk);
		const auto target = std::find_if(found.targets.cbegin(), found.targets.cend(),
			[token, slot](const Indd::CandidateType6RowTarget& row) {
				return row.targetPageToken == token && row.targetPageLocalSlot == slot;
			});
		QVERIFY(target != found.targets.cend());
		selectedTarget = &*target;
	}
	const QByteArray expectedKind = qgetenv("SCRIBUS_INDD_TYPE6_EXPECTED_TARGET_KIND");
	const QByteArray expectedWord12 = qgetenv("SCRIBUS_INDD_TYPE6_EXPECTED_TARGET_WORD12");
	if (!expectedKind.isEmpty() || !expectedWord12.isEmpty())
	{
		QVERIFY(selectedTarget);
		bool kindOk = false;
		bool wordOk = false;
		const quint32 kind = expectedKind.toUInt(&kindOk, 0);
		const quint32 word12 = expectedWord12.toUInt(&wordOk, 0);
		QVERIFY(kindOk && wordOk);
		QCOMPARE(selectedTarget->targetRecordWordAt4, kind);
		QVERIFY(selectedTarget->targetHasWordAt12);
		QCOMPARE(selectedTarget->targetRecordWordAt12, word12);
	}
	qInfo().nospace() << "Candidate type-6 key=" << Qt::hex << key << Qt::dec
		<< " matched=" << found.targets.size() << " rejected=" << found.rejectedMatchingRowCount
		<< " scanned-pages=" << found.scannedType6PageCount
		<< " unsupported-type6=" << found.unsupportedType6PageCount
		<< " unsupported-type9=" << found.unsupportedType9PageCount;
	for (const auto& row : found.targets)
		qInfo().nospace() << "  source=" << row.sourcePageToken << "+" << row.sourceRowOffset
			<< " target=" << row.targetPageToken << "/" << row.targetPageLocalSlot
			<< " envelope-length=" << row.targetEnvelopeLength
			<< " kind=" << Qt::hex << row.targetRecordWordAt4
			<< " word-at-12=" << row.targetRecordWordAt12 << Qt::dec;
}

void InddProbeTests::comparesOnlyValidatedSlotAddressesAcrossSaves()
{
	const QByteArray original = makeMappedSlotDocument();
	const auto identical = recordSlotDiffBytes(original, original);
	QVERIFY(identical.compared());
	QCOMPARE(identical.firstValidatedSlotCount, quint32(1));
	QCOMPARE(identical.secondValidatedSlotCount, quint32(1));
	QCOMPARE(identical.firstUnindexedEnvelopeCount, quint32(1));
	QCOMPARE(identical.secondUnindexedEnvelopeCount, quint32(1));
	QCOMPARE(identical.commonCount, quint32(1));
	QCOMPARE(identical.identicalEnvelopeCount, quint32(1));
	QCOMPARE(identical.changedEnvelopeCount, quint32(0));
	QCOMPARE(identical.relocatedPhysicalPageCount, quint32(0));
	QVERIFY(identical.differences.isEmpty());

	const auto edited = recordSlotDiffBytes(original, makeMappedSlotDocument(10, 2, 'B'));
	QVERIFY(edited.compared());
	QCOMPARE(edited.commonCount, quint32(1));
	QCOMPARE(edited.changedEnvelopeCount, quint32(1));
	QCOMPARE(edited.changedEnvelopeLengthCount, quint32(0));
	QCOMPARE(edited.relocatedPhysicalPageCount, quint32(0));
	QCOMPARE(edited.differences.size(), 1);
	QCOMPARE(edited.differences[0].opaquePageToken, quint32(1));
	QCOMPARE(edited.differences[0].pageLocalSlot, quint16(2));
	QVERIFY(edited.differences[0].presentInFirst);
	QVERIFY(edited.differences[0].presentInSecond);
	QVERIFY(edited.differences[0].envelopeBytesChanged);
	QVERIFY(!edited.differences[0].physicalPageChanged);
	QCOMPARE(edited.differences[0].firstEnvelopeLength, quint16(56));
	QCOMPARE(edited.differences[0].secondEnvelopeLength, quint16(56));
	QCOMPARE(edited.differences[0].sharedAfterHeaderPrefixBytes, quint16(0));
	QCOMPARE(edited.differences[0].sharedAfterHeaderSuffixBytes, quint16(47));

	const auto grown = recordSlotDiffBytes(original, makeMappedSlotDocument(10, 2, 'A', 60));
	QVERIFY(grown.compared());
	QCOMPARE(grown.changedEnvelopeCount, quint32(1));
	QCOMPARE(grown.changedEnvelopeLengthCount, quint32(1));
	QCOMPARE(grown.differences[0].firstEnvelopeLength, quint16(56));
	QCOMPARE(grown.differences[0].secondEnvelopeLength, quint16(60));
	QCOMPARE(grown.differences[0].sharedAfterHeaderPrefixBytes, quint16(48));
	QCOMPARE(grown.differences[0].sharedAfterHeaderSuffixBytes, quint16(0));

	const auto relocated = recordSlotDiffBytes(original, makeMappedSlotDocument(11));
	QVERIFY(relocated.compared());
	QCOMPARE(relocated.commonCount, quint32(1));
	QCOMPARE(relocated.identicalEnvelopeCount, quint32(1));
	QCOMPARE(relocated.changedEnvelopeCount, quint32(0));
	QCOMPARE(relocated.relocatedPhysicalPageCount, quint32(1));
	QCOMPARE(relocated.differences.size(), 1);
	QVERIFY(!relocated.differences[0].envelopeBytesChanged);
	QVERIFY(relocated.differences[0].physicalPageChanged);
	QCOMPARE(relocated.differences[0].sharedAfterHeaderPrefixBytes, quint16(48));
	QCOMPARE(relocated.differences[0].sharedAfterHeaderSuffixBytes, quint16(0));

	const auto replacedSlot = recordSlotDiffBytes(original, makeMappedSlotDocument(10, 3));
	QVERIFY(replacedSlot.compared());
	QCOMPARE(replacedSlot.commonCount, quint32(0));
	QCOMPARE(replacedSlot.firstOnlyCount, quint32(1));
	QCOMPARE(replacedSlot.secondOnlyCount, quint32(1));
	QCOMPARE(replacedSlot.differences.size(), 2);
	QVERIFY(replacedSlot.differences[0].presentInFirst);
	QVERIFY(!replacedSlot.differences[0].presentInSecond);
	QCOMPARE(replacedSlot.differences[0].firstEnvelopeLength, quint16(56));
	QCOMPARE(replacedSlot.differences[0].secondEnvelopeLength, quint16(0));
	QVERIFY(!replacedSlot.differences[1].presentInFirst);
	QVERIFY(replacedSlot.differences[1].presentInSecond);
	QCOMPARE(replacedSlot.differences[1].firstEnvelopeLength, quint16(0));
	QCOMPARE(replacedSlot.differences[1].secondEnvelopeLength, quint16(56));
	QCOMPARE(recordSlotDiffBytes(original, original, 1).status, Indd::RecordSlotDiffStatus::LimitExceeded);
	QCOMPARE(recordSlotDiffBytes(original, original.left(8191)).status, Indd::RecordSlotDiffStatus::InvalidDocument);
	QCOMPARE(recordSlotDiffBytes(original, makeHeader(9, 1, 13) + makeHeader(9, 2, 13) + original.mid(8192)).status,
		Indd::RecordSlotDiffStatus::IncompatibleDocuments);
	QByteArray invalidMap = original;
	put32(invalidMap, 9 * 4096 + 128 + 4, 11, true);
	QCOMPARE(recordSlotDiffBytes(original, invalidMap).status, Indd::RecordSlotDiffStatus::InvalidMapping);
	const QByteArray bigEndian = makeHeader(8, 1, 13, false) + makeHeader(8, 2, 13, false)
		+ QByteArray(11 * 4096, '\0');
	QCOMPARE(recordSlotDiffBytes(bigEndian, bigEndian).status, Indd::RecordSlotDiffStatus::UnsupportedLayout);
}

void InddProbeTests::readsOptionalLocalSlotDiff()
{
	const QString firstPath = qEnvironmentVariable("SCRIBUS_INDD_SLOT_DIFF_FIRST_FILE");
	const QString secondPath = qEnvironmentVariable("SCRIBUS_INDD_SLOT_DIFF_SECOND_FILE");
	if (firstPath.isEmpty() || secondPath.isEmpty())
		return;
	const auto diff = Indd::compareDirectoryMappedRecordSlots(firstPath, secondPath);
	QVERIFY(diff.compared());
	QCOMPARE(diff.firstOnlyCount + diff.commonCount, diff.firstValidatedSlotCount);
	QCOMPARE(diff.secondOnlyCount + diff.commonCount, diff.secondValidatedSlotCount);
	QCOMPARE(diff.identicalEnvelopeCount + diff.changedEnvelopeCount, diff.commonCount);
	const QByteArray expectedChanged = qgetenv("SCRIBUS_INDD_SLOT_DIFF_EXPECTED_CHANGED");
	if (!expectedChanged.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedChanged.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(diff.changedEnvelopeCount, count);
	}
	const QByteArray expectedRelocated = qgetenv("SCRIBUS_INDD_SLOT_DIFF_EXPECTED_RELOCATED");
	if (!expectedRelocated.isEmpty())
	{
		bool countOk = false;
		const quint32 count = expectedRelocated.toUInt(&countOk);
		QVERIFY(countOk);
		QCOMPARE(diff.relocatedPhysicalPageCount, count);
	}
	qInfo().nospace() << "Validated slot diff: first=" << diff.firstValidatedSlotCount
		<< " second=" << diff.secondValidatedSlotCount << " first-only=" << diff.firstOnlyCount
		<< " second-only=" << diff.secondOnlyCount << " common=" << diff.commonCount
		<< " changed-bytes=" << diff.changedEnvelopeCount
		<< " changed-length=" << diff.changedEnvelopeLengthCount
		<< " relocated-pages=" << diff.relocatedPhysicalPageCount
		<< " unsupported-pages=" << diff.firstUnsupportedPageCount << "/" << diff.secondUnsupportedPageCount;
	const QByteArray watchToken = qgetenv("SCRIBUS_INDD_SLOT_DIFF_WATCH_TOKEN");
	const QByteArray watchSlot = qgetenv("SCRIBUS_INDD_SLOT_DIFF_WATCH_SLOT");
	if (!watchToken.isEmpty() || !watchSlot.isEmpty())
	{
		QVERIFY(!watchToken.isEmpty() && !watchSlot.isEmpty());
		bool tokenOk = false;
		bool slotOk = false;
		const quint32 token = watchToken.toUInt(&tokenOk);
		const quint16 slot = quint16(watchSlot.toUShort(&slotOk));
		QVERIFY(tokenOk && slotOk);
		int matches = 0;
		for (const auto& entry : diff.differences)
		{
			if (entry.opaquePageToken != token || entry.pageLocalSlot != slot)
				continue;
			++matches;
			qInfo().nospace() << "Watched slot " << token << "/" << slot
				<< ": envelope-length=" << entry.firstEnvelopeLength << "/" << entry.secondEnvelopeLength
				<< " shared-after-header-prefix=" << entry.sharedAfterHeaderPrefixBytes
				<< " suffix=" << entry.sharedAfterHeaderSuffixBytes;
		}
		QCOMPARE(matches, 1);
	}
}

void InddProbeTests::matchesOnlyUniqueExactCandidateRecordsAcrossSlots()
{
	const QByteArray original = makeMappedSlotDocument();
	const auto same = exactRecordRelocationBytes(original, original);
	QVERIFY(same.compared());
	QCOMPARE(same.firstValidatedSlotCount, quint32(1));
	QCOMPARE(same.uniqueExactMatchCount, quint32(1));
	QCOMPARE(same.sameAddressCount, quint32(1));
	QCOMPARE(same.movedAddressCount, quint32(0));
	QVERIFY(same.moves.isEmpty());

	const auto moved = exactRecordRelocationBytes(original, makeMappedSlotDocument(10, 3));
	QVERIFY(moved.compared());
	QCOMPARE(moved.uniqueExactMatchCount, quint32(1));
	QCOMPARE(moved.sameAddressCount, quint32(0));
	QCOMPARE(moved.movedAddressCount, quint32(1));
	QCOMPARE(moved.moves[0].firstOpaquePageToken, quint32(1));
	QCOMPARE(moved.moves[0].firstPageLocalSlot, quint16(2));
	QCOMPARE(moved.moves[0].secondOpaquePageToken, quint32(1));
	QCOMPARE(moved.moves[0].secondPageLocalSlot, quint16(3));
	const auto physicallyRelocated = exactRecordRelocationBytes(original, makeMappedSlotDocument(11));
	QVERIFY(physicallyRelocated.compared());
	QCOMPARE(physicallyRelocated.sameAddressCount, quint32(1));

	const auto edited = exactRecordRelocationBytes(original, makeMappedSlotDocument(10, 2, 'B'));
	QVERIFY(edited.compared());
	QCOMPARE(edited.uniqueExactMatchCount, quint32(0));
	QCOMPARE(edited.firstUnmatchedCount, quint32(1));
	QCOMPARE(edited.secondUnmatchedCount, quint32(1));

	QByteArray duplicated = original;
	const int page = 10 * 4096;
	put16(duplicated, page + 76, 56, true);
	put16(duplicated, page + 78, 3, true);
	duplicated[page + 84] = 'A';
	put16(duplicated, page + 132, 3864, true);
	put32(duplicated, page + 3996 + 4 * (16 - 3), 76, true);
	const auto ambiguous = exactRecordRelocationBytes(original, duplicated);
	QVERIFY(ambiguous.compared());
	QCOMPARE(ambiguous.firstValidatedSlotCount, quint32(1));
	QCOMPARE(ambiguous.secondValidatedSlotCount, quint32(2));
	QCOMPARE(ambiguous.uniqueExactMatchCount, quint32(0));
	QCOMPARE(ambiguous.firstAmbiguousCount, quint32(1));
	QCOMPARE(ambiguous.secondAmbiguousCount, quint32(2));
	QVERIFY(ambiguous.moves.isEmpty());

	QCOMPARE(exactRecordRelocationBytes(original, original, 1).status,
		Indd::RecordSlotDiffStatus::LimitExceeded);
	QCOMPARE(exactRecordRelocationBytes(original, original, 10001).status,
		Indd::RecordSlotDiffStatus::LimitExceeded);
	QCOMPARE(exactRecordRelocationBytes(original, original.left(8191)).status,
		Indd::RecordSlotDiffStatus::InvalidDocument);
	QCOMPARE(exactRecordRelocationBytes(original,
		makeHeader(9, 1, 13) + makeHeader(9, 2, 13) + original.mid(8192)).status,
		Indd::RecordSlotDiffStatus::IncompatibleDocuments);
	QByteArray invalidMap = original;
	put32(invalidMap, 9 * 4096 + 128 + 4, 11, true);
	QCOMPARE(exactRecordRelocationBytes(original, invalidMap).status,
		Indd::RecordSlotDiffStatus::InvalidMapping);
	const QByteArray bigEndian = makeHeader(8, 1, 13, false) + makeHeader(8, 2, 13, false)
		+ QByteArray(11 * 4096, '\0');
	QCOMPARE(exactRecordRelocationBytes(bigEndian, bigEndian).status,
		Indd::RecordSlotDiffStatus::UnsupportedLayout);
}

void InddProbeTests::readsOptionalLocalExactRecordRelocations()
{
	const QString firstPath = qEnvironmentVariable("SCRIBUS_INDD_EXACT_FIRST_FILE");
	const QString secondPath = qEnvironmentVariable("SCRIBUS_INDD_EXACT_SECOND_FILE");
	if (firstPath.isEmpty() || secondPath.isEmpty())
		return;
	const auto result = Indd::compareExactCandidateRecordRelocations(firstPath, secondPath);
	QVERIFY(result.compared());
	QCOMPARE(result.firstValidatedSlotCount,
		result.uniqueExactMatchCount + result.firstUnmatchedCount + result.firstAmbiguousCount);
	QCOMPARE(result.secondValidatedSlotCount,
		result.uniqueExactMatchCount + result.secondUnmatchedCount + result.secondAmbiguousCount);
	qInfo().nospace() << "Exact candidate payloads: first=" << result.firstValidatedSlotCount
		<< " second=" << result.secondValidatedSlotCount
		<< " unique=" << result.uniqueExactMatchCount
		<< " moved-address=" << result.movedAddressCount
		<< " unmatched=" << result.firstUnmatchedCount << "/" << result.secondUnmatchedCount
		<< " ambiguous=" << result.firstAmbiguousCount << "/" << result.secondAmbiguousCount
		<< " unsupported-pages=" << result.firstUnsupportedPageCount << "/" << result.secondUnsupportedPageCount;
	for (const auto& move : result.moves)
		qInfo().nospace() << "  " << move.firstOpaquePageToken << "/" << move.firstPageLocalSlot
			<< " -> " << move.secondOpaquePageToken << "/" << move.secondPageLocalSlot;
}

void InddProbeTests::decodesOnlyBoundedCandidateStoryRuns()
{
	QByteArray asciiRun(2, '\0');
	put16(asciiRun, 0, 0x4006, true);
	asciiRun.append("Hello\r", 6);
	const QByteArray asciiDocument = makeCandidateStoryDocument(asciiRun, 6);
	const auto ascii = candidateStoryBytes(asciiDocument);
	QVERIFY(ascii.decoded());
	QCOMPARE(ascii.text, QStringLiteral("Hello\r"));

	QByteArray mixedRuns(2, '\0');
	put16(mixedRuns, 0, 0x4003, true);
	mixedRuns.append("Hi ", 3);
	const QString telugu = QStringLiteral("తెలుగు");
	const qsizetype unicodeOffset = mixedRuns.size();
	mixedRuns.append(QByteArray(2 + 2 * telugu.size(), '\0'));
	put16(mixedRuns, unicodeOffset, quint16(0x8000 | telugu.size()), true);
	for (qsizetype i = 0; i < telugu.size(); ++i)
		put16(mixedRuns, unicodeOffset + 2 + 2 * i, telugu[i].unicode(), true);
	const qsizetype endOffset = mixedRuns.size();
	mixedRuns.append(QByteArray(2, '\0'));
	put16(mixedRuns, endOffset, 0x4001, true);
	mixedRuns.append('\r');
	QByteArray mixedDocument = makeCandidateStoryDocument(mixedRuns, 10);
	const auto mixed = candidateStoryBytes(mixedDocument);
	QVERIFY(mixed.decoded());
	QCOMPARE(mixed.text, QStringLiteral("Hi తెలుగు\r"));

	const int record = 10 * 4096 + 20;
	QByteArray invalid = mixedDocument;
	put32(invalid, record + 4, 0x263, true);
	QCOMPARE(candidateStoryBytes(invalid).status, Indd::CandidateStoryStatus::UnsupportedRecord);
	invalid = mixedDocument;
	put16(invalid, record + 30, 0xc003, true);
	QCOMPARE(candidateStoryBytes(invalid).status, Indd::CandidateStoryStatus::InvalidEncoding);
	invalid = mixedDocument;
	put32(invalid, record + 22, 9999, true);
	QCOMPARE(candidateStoryBytes(invalid).status, Indd::CandidateStoryStatus::InvalidEncoding);
	invalid = mixedDocument;
	put32(invalid, record + 26, 11, true);
	QCOMPARE(candidateStoryBytes(invalid).status, Indd::CandidateStoryStatus::InvalidEncoding);
	invalid = mixedDocument;
	put16(invalid, record + 37, 0xd800, true); // Unpaired UTF-16 surrogate.
	QCOMPARE(candidateStoryBytes(invalid).status, Indd::CandidateStoryStatus::InvalidEncoding);
	invalid = asciiDocument;
	invalid[record + 32] = char(0xff);
	QCOMPARE(candidateStoryBytes(invalid).status, Indd::CandidateStoryStatus::InvalidEncoding);
	QCOMPARE(candidateStoryBytes(mixedDocument, 1, 0).status, Indd::CandidateStoryStatus::NotIndexed);
	QCOMPARE(candidateStoryBytes(mixedDocument, 1, 3).status, Indd::CandidateStoryStatus::NotIndexed);
	QCOMPARE(candidateStoryBytes(mixedDocument, 1, 2, 1).status, Indd::CandidateStoryStatus::LimitExceeded);
	invalid = mixedDocument;
	put32(invalid, 9 * 4096 + 128 + 4, 11, true);
	QCOMPARE(candidateStoryBytes(invalid).status, Indd::CandidateStoryStatus::InvalidMapping);
}

void InddProbeTests::readsOptionalLocalCandidateStory()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_STORY_TEST_FILE");
	if (path.isEmpty())
		return;
	bool tokenOk = false;
	bool slotOk = false;
	const quint32 token = qEnvironmentVariable("SCRIBUS_INDD_STORY_TEST_TOKEN").toUInt(&tokenOk);
	const quint16 slot = qEnvironmentVariable("SCRIBUS_INDD_STORY_TEST_SLOT").toUShort(&slotOk);
	QVERIFY(tokenOk && slotOk);
	const auto story = Indd::inspectCandidateStoryText(path, token, slot);
	QVERIFY(story.decoded());
	const QString expected = qEnvironmentVariable("SCRIBUS_INDD_STORY_EXPECTED_TEXT");
	if (!expected.isEmpty())
		QCOMPARE(story.text, expected + QLatin1Char('\r'));
}

void InddProbeTests::decodesOnlyGuardedCandidateAffineTails()
{
	const QByteArray original = makeCandidateAffineDocument();
	const auto baseline = candidateAffineBytes(original);
	QVERIFY(baseline.decoded());
	QCOMPARE(baseline.matrix[0], 1.0);
	QCOMPARE(baseline.matrix[1], 0.0);
	QCOMPARE(baseline.matrix[2], 0.0);
	QCOMPARE(baseline.matrix[3], 1.0);
	QCOMPARE(baseline.matrix[4], 4.0);
	QCOMPARE(baseline.matrix[5], -100.8);
	const auto horizontal = candidateAffineBytes(makeCandidateAffineDocument(14.0, -100.8));
	const auto vertical = candidateAffineBytes(makeCandidateAffineDocument(4.0, -90.8));
	QVERIFY(horizontal.decoded() && vertical.decoded());
	QCOMPARE(horizontal.matrix[4] - baseline.matrix[4], 10.0);
	QCOMPARE(vertical.matrix[5] - baseline.matrix[5], 10.0);

	const int record = 10 * 4096 + 20;
	QByteArray invalid = original;
	put32(invalid, record + 896, 0x152, true);
	QCOMPARE(candidateAffineBytes(invalid).status, Indd::CandidateAffineStatus::UnsupportedRecord);
	invalid = original;
	put32(invalid, record + 900, 47, true);
	QCOMPARE(candidateAffineBytes(invalid).status, Indd::CandidateAffineStatus::UnsupportedRecord);
	QCOMPARE(candidateAffineBytes(makeMappedSlotDocument()).status,
		Indd::CandidateAffineStatus::UnsupportedRecord);
	invalid = original;
	putDoubleLittleEndian(invalid, record + 936, std::numeric_limits<double>::quiet_NaN());
	QCOMPARE(candidateAffineBytes(invalid).status, Indd::CandidateAffineStatus::InvalidEncoding);
	invalid = original;
	putDoubleLittleEndian(invalid, record + 944, 1e12);
	QCOMPARE(candidateAffineBytes(invalid).status, Indd::CandidateAffineStatus::InvalidEncoding);
	QCOMPARE(candidateAffineBytes(original, 1, 0).status, Indd::CandidateAffineStatus::NotIndexed);
	QCOMPARE(candidateAffineBytes(original, 1, 3).status, Indd::CandidateAffineStatus::NotIndexed);
	QCOMPARE(candidateAffineBytes(original, 1, 2, 1).status, Indd::CandidateAffineStatus::LimitExceeded);
	invalid = original;
	put32(invalid, 9 * 4096 + 128 + 4, 11, true);
	QCOMPARE(candidateAffineBytes(invalid).status, Indd::CandidateAffineStatus::InvalidMapping);

	QRandomGenerator random(0x20261002);
	for (int iteration = 0; iteration < 128; ++iteration)
	{
		QByteArray mutated = original;
		for (int edit = 0, count = 1 + random.bounded(4); edit < count; ++edit)
		{
			const int offset = record + random.bounded(952);
			mutated[offset] = char(random.bounded(256));
		}
		const auto result = candidateAffineBytes(mutated);
		if (!result.decoded())
			continue;
		for (double value : result.matrix)
			QVERIFY(std::isfinite(value) && std::abs(value) <= 1e9);
	}
}

void InddProbeTests::readsOptionalLocalCandidateAffine()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_AFFINE_TEST_FILE");
	if (path.isEmpty())
		return;
	bool tokenOk = false;
	bool slotOk = false;
	const quint32 token = qEnvironmentVariable("SCRIBUS_INDD_AFFINE_TEST_TOKEN").toUInt(&tokenOk);
	const quint16 slot = qEnvironmentVariable("SCRIBUS_INDD_AFFINE_TEST_SLOT").toUShort(&slotOk);
	QVERIFY(tokenOk && slotOk);
	const auto result = Indd::inspectCandidateAffineTransform(path, token, slot);
	if (qEnvironmentVariable("SCRIBUS_INDD_AFFINE_EXPECT_UNSUPPORTED") == QLatin1String("1"))
	{
		QCOMPARE(result.status, Indd::CandidateAffineStatus::UnsupportedRecord);
		return;
	}
	QVERIFY(result.decoded());
	const QByteArray expectedX = qgetenv("SCRIBUS_INDD_AFFINE_EXPECTED_X");
	const QByteArray expectedY = qgetenv("SCRIBUS_INDD_AFFINE_EXPECTED_Y");
	if (!expectedX.isEmpty() || !expectedY.isEmpty())
	{
		bool xOk = false;
		bool yOk = false;
		const double x = expectedX.toDouble(&xOk);
		const double y = expectedY.toDouble(&yOk);
		QVERIFY(xOk && yOk);
		QVERIFY(std::abs(result.matrix[4] - x) < 1e-9);
		QVERIFY(std::abs(result.matrix[5] - y) < 1e-9);
	}
	qInfo().nospace() << "Candidate affine at " << token << "/" << slot
		<< ": [" << result.matrix[0] << ", " << result.matrix[1] << ", "
		<< result.matrix[2] << ", " << result.matrix[3] << ", "
		<< result.matrix[4] << ", " << result.matrix[5] << "]";
}

void InddProbeTests::findsOnlyRawWordsInValidatedEnvelopes()
{
	constexpr quint32 needle = 0x12345678;
	QByteArray document = makeMappedSlotDocument();
	const int record = 10 * 4096 + 20;
	put32(document, record + 16, needle, true);
	put32(document, record + 34, needle, true); // An unaligned byte match is still reported.
	put32(document, record + 4, needle, true); // The opaque header is excluded.
	const auto found = recordWordSearchBytes(document, needle);
	QVERIFY(found.searched());
	QCOMPARE(found.scannedValidatedSlotCount, quint32(1));
	QCOMPARE(found.occurrences.size(), 2);
	QCOMPARE(found.occurrences[0].opaquePageToken, quint32(1));
	QCOMPARE(found.occurrences[0].pageLocalSlot, quint16(2));
	QCOMPARE(found.occurrences[0].envelopeByteOffset, quint16(16));
	QCOMPARE(found.occurrences[1].envelopeByteOffset, quint16(34));
	QCOMPARE(recordWordSearchBytes(document, needle, 1).status,
		Indd::RecordWordSearchStatus::LimitExceeded);
	QCOMPARE(recordWordSearchBytes(document, needle, 0).status,
		Indd::RecordWordSearchStatus::LimitExceeded);
	QCOMPARE(recordWordSearchBytes(document, needle, 4097).status,
		Indd::RecordWordSearchStatus::LimitExceeded);
	QCOMPARE(recordWordSearchBytes(document, needle, 4096, 1).status,
		Indd::RecordWordSearchStatus::LimitExceeded);
	QByteArray unindexed = document;
	put16(unindexed, record + 2, 0, true);
	const auto skipped = recordWordSearchBytes(unindexed, needle);
	QVERIFY(skipped.searched());
	QCOMPARE(skipped.scannedValidatedSlotCount, quint32(0));
	QVERIFY(skipped.unsupportedPageCount > 0);
	QCOMPARE(skipped.occurrences.size(), 0);
	QByteArray invalidMap = document;
	put32(invalidMap, 9 * 4096 + 128 + 4, 11, true);
	QCOMPARE(recordWordSearchBytes(invalidMap, needle).status,
		Indd::RecordWordSearchStatus::InvalidMapping);
}

void InddProbeTests::readsOptionalLocalRecordWordMatches()
{
	const QString path = qEnvironmentVariable("SCRIBUS_INDD_WORD_TEST_FILE");
	if (path.isEmpty())
		return;
	bool ok = false;
	const quint32 word = qEnvironmentVariable("SCRIBUS_INDD_WORD_TEST_VALUE").toUInt(&ok, 0);
	QVERIFY(ok);
	const auto result = Indd::findCandidateRecordWords(path, word);
	QVERIFY(result.searched());
	qInfo().nospace() << "Raw word " << Qt::hex << word << Qt::dec
		<< ": matches=" << result.occurrences.size()
		<< " scanned-slots=" << result.scannedValidatedSlotCount
		<< " unsupported-pages=" << result.unsupportedPageCount
		<< " unindexed-envelopes=" << result.unindexedEnvelopeCount;
	for (const auto& occurrence : result.occurrences)
		qInfo().nospace() << "  " << occurrence.opaquePageToken << "/"
			<< occurrence.pageLocalSlot << " +" << occurrence.envelopeByteOffset;
}

void InddProbeTests::survivesCandidateStoryMutations()
{
	QByteArray run(2, '\0');
	put16(run, 0, 0x4006, true);
	run.append("Hello\r", 6);
	const QByteArray original = makeCandidateStoryDocument(run, 6);
	QRandomGenerator random(0x20261001);
	for (int iteration = 0; iteration < 128; ++iteration)
	{
		QByteArray mutated = original;
		for (int edit = 0, count = 1 + random.bounded(4); edit < count; ++edit)
		{
			const int offset = 10 * 4096 + 20 + random.bounded(64);
			mutated[offset] = char(random.bounded(256));
		}
		const auto result = candidateStoryBytes(mutated);
		if (result.decoded())
			QVERIFY(result.text.size() <= 4096);
	}
}

void InddProbeTests::survivesRecordPageMutations()
{
	const QByteArray headers = makeHeader(8, 1, 3) + makeHeader(8, 2, 3);
	QByteArray page(4096, '\0');
	put16(page, 0, 64, true);
	put16(page, 64, 32, true);
	put16(page, 66, 18, true);
	put16(page, 96, 16, true);
	QRandomGenerator random(0x1ddd2026);
	for (int iteration = 0; iteration < 128; ++iteration)
	{
		QByteArray mutated = page;
		for (int mutation = 0, count = 1 + random.bounded(8); mutation < count; ++mutation)
		{
			const int offset = random.bounded(mutated.size());
			mutated[offset] = char(random.bounded(256));
		}
		const auto result = recordPageBytes(headers + mutated, 2);
		if (!result.parsed())
			continue;
		quint32 end = 0;
		for (const auto& record : result.records)
		{
			QVERIFY(record.offset >= end);
			QVERIFY(record.length >= 8);
			QVERIFY(quint32(record.offset) + record.length <= 4096);
			end = quint32(record.offset) + record.length;
		}
	}
}

void InddProbeTests::extractsStructuredXmpMetadata()
{
	const QByteArray packet = sampleXmp();
	const auto result = metadataBytes(makeHeader(8, 1, 2) + makeHeader(8, 2, 2) + makeXmpObject(packet));
	QVERIFY(result.found());
	QCOMPARE(result.title, QStringLiteral("Portfolio"));
	QCOMPARE(result.creator, QStringLiteral("Example Author"));
	QCOMPARE(result.created, QStringLiteral("2024-01-01T00:00:00Z"));
	QCOMPARE(result.modified, QStringLiteral("2024-01-02T00:00:00Z"));
	QCOMPARE(result.xmpByteCount, packet.size());

	const auto bigEndian = metadataBytes(makeHeader(8, 1, 2, false) + makeHeader(8, 2, 2, false) + makeXmpObject(packet, false));
	QVERIFY(bigEndian.found());
	QCOMPARE(bigEndian.title, QStringLiteral("Portfolio"));
}

void InddProbeTests::skipsOtherContiguousObjects()
{
	const QByteArray headers = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	const auto result = metadataBytes(headers + makeObject(QByteArray(60, 'A')) + makeXmpObject(sampleXmp()));
	QVERIFY(result.found());
	QCOMPARE(result.title, QStringLiteral("Portfolio"));

	const auto absent = metadataBytes(headers + makeObject(QByteArray(60, 'A')));
	QCOMPARE(absent.status, Indd::MetadataStatus::NoXmp);
}

void InddProbeTests::rejectsMalformedXmpObject()
{
	const QByteArray headers = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	QByteArray badTrailer = makeXmpObject(sampleXmp());
	badTrailer[badTrailer.size() - 32] = '\0';
	QCOMPARE(metadataBytes(headers + badTrailer).status, Indd::MetadataStatus::InvalidObject);

	QByteArray badLength = makeXmpObject(sampleXmp());
	badLength[32] = '\0';
	QCOMPARE(metadataBytes(headers + badLength).status, Indd::MetadataStatus::InvalidObject);
	QByteArray truncated = makeXmpObject(sampleXmp());
	truncated.chop(40);
	QCOMPARE(metadataBytes(headers + truncated).status, Indd::MetadataStatus::InvalidObject);

	const QByteArray invalidXml = "<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?><x:xmpmeta>";
	QCOMPARE(metadataBytes(headers + makeXmpObject(invalidXml)).status, Indd::MetadataStatus::MalformedXmp);

	const QByteArray noRdf = "<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?><x:xmpmeta xmlns:x=\"adobe:ns:meta/\"/>";
	QCOMPARE(metadataBytes(headers + makeXmpObject(noRdf)).status, Indd::MetadataStatus::MalformedXmp);

	const QByteArray withDtd = "<?xpacket begin=\"\xef\xbb\xbf\" id=\"W5M0MpCehiHzreSzNTczkc9d\"?><!DOCTYPE foo [<!ENTITY bar \"x\">]><rdf:RDF xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"/>";
	QCOMPARE(metadataBytes(headers + makeXmpObject(withDtd)).status, Indd::MetadataStatus::MalformedXmp);
}

void InddProbeTests::readsOptionalLocalMetadata()
{
	for (const char* variable : { "SCRIBUS_INDD_TEST_FILE", "SCRIBUS_INDD_TEST_FILE2" })
	{
		const QString path = qEnvironmentVariable(variable);
		if (path.isEmpty())
			continue;
		const auto result = Indd::readMetadata(path);
		QVERIFY2(result.found() || result.status == Indd::MetadataStatus::NoXmp, variable);
		if (result.found())
			QVERIFY(result.xmpByteCount > 0);
	}
}

void InddProbeTests::extractsSavedPagePreview()
{
	const QByteArray jpeg = sampleJpeg();
	QVERIFY(!jpeg.isEmpty());
	QByteArray encoded = jpeg.toBase64();
	encoded.insert(20, "&#xA;");
	const QByteArray headers = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	const QByteArray document = headers + makeXmpObject(samplePreviewXmp(encoded));
	const auto result = previewBytes(document);
	QVERIFY(result.found());
	QCOMPARE(result.pageNumber, 1);
	QCOMPARE(result.width, 2);
	QCOMPARE(result.height, 3);
	QCOMPARE(result.jpegBytes, jpeg);
	QCOMPARE(previewBytes(document, 2).status, Indd::PreviewStatus::NoPreview);
	QCOMPARE(previewBytes(document, 0).status, Indd::PreviewStatus::InvalidRequest);
}

void InddProbeTests::rejectsInvalidPreview()
{
	const QByteArray headers = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	QCOMPARE(previewBytes(headers).status, Indd::PreviewStatus::NoPreview);
	QCOMPARE(previewBytes(headers + makeXmpObject(samplePreviewXmp("not-base64!"))).status,
		Indd::PreviewStatus::InvalidPreview);
	QCOMPARE(previewBytes(headers + makeXmpObject(samplePreviewXmp(sampleJpeg().toBase64(), 9999))).status,
		Indd::PreviewStatus::InvalidPreview);
	QCOMPARE(previewBytes(headers + makeXmpObject(samplePreviewXmp(sampleJpeg().toBase64(), 1))).status,
		Indd::PreviewStatus::InvalidPreview);
	const auto fitted = previewBytes(headers + makeXmpObject(samplePreviewXmp(sampleJpeg().toBase64(), 3)));
	QVERIFY(fitted.found());
	QCOMPARE(fitted.width, 2);
	QCOMPARE(fitted.height, 3);
	QCOMPARE(previewBytes(headers + makeXmpObject(sampleXmp())).status,
		Indd::PreviewStatus::NoPreview);
}

void InddProbeTests::readsOptionalLocalPreview()
{
	for (const char* variable : { "SCRIBUS_INDD_TEST_FILE", "SCRIBUS_INDD_TEST_FILE2" })
	{
		const QString path = qEnvironmentVariable(variable);
		if (path.isEmpty())
			continue;
		for (int page = 1; page <= 2; ++page)
		{
			const auto result = Indd::readPreview(path, page);
			QVERIFY2(result.found() || result.status == Indd::PreviewStatus::NoPreview,
				qPrintable(QString::fromLatin1("%1 page %2: status %3").arg(QString::fromLatin1(variable)).arg(page).arg(int(result.status))));
			if (result.found())
			{
				QCOMPARE(result.pageNumber, page);
				QVERIFY(result.width > 0 && result.height > 0);
				QVERIFY(!result.jpegBytes.isEmpty());
			}
		}
	}
}

void InddProbeTests::inventoriesValidatedXmpFields()
{
	const QByteArray headers = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	const auto result = inspectionBytes(headers + makeXmpObject(sampleInspectionXmp()));
	QVERIFY(result.found());
	QCOMPARE(result.pageCount, 3);
	QCOMPARE(result.fontPostScriptNames, QStringList { QStringLiteral("Example-Regular") });
	QCOMPARE(result.ingredientUris, QStringList { QStringLiteral("file:///images/photo.jpg") });

	const auto noCount = inspectionBytes(headers + makeXmpObject(sampleXmp()));
	QVERIFY(noCount.found());
	QCOMPARE(noCount.pageCount, 0);
	QVERIFY(noCount.fontPostScriptNames.isEmpty());
	QVERIFY(noCount.ingredientUris.isEmpty());
}

void InddProbeTests::rejectsInvalidPageCount()
{
	const QByteArray headers = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	QByteArray negative = sampleInspectionXmp();
	negative.replace("xmpTPg:NPages=\"3\"", "xmpTPg:NPages=\"-1\"");
	QCOMPARE(inspectionBytes(headers + makeXmpObject(negative)).status, Indd::InspectionStatus::InvalidValue);

	QByteArray conflicting = sampleInspectionXmp();
	conflicting.replace("</xmpTPg:Fonts>", "</xmpTPg:Fonts><xmpTPg:NPages>4</xmpTPg:NPages>");
	const auto rejected = inspectionBytes(headers + makeXmpObject(conflicting));
	QCOMPARE(rejected.status, Indd::InspectionStatus::InvalidValue);
	QVERIFY(rejected.fontPostScriptNames.isEmpty());
}

void InddProbeTests::readsOptionalLocalInventory()
{
	for (const char* variable : { "SCRIBUS_INDD_TEST_FILE", "SCRIBUS_INDD_TEST_FILE2" })
	{
		const QString path = qEnvironmentVariable(variable);
		if (path.isEmpty())
			continue;
		const auto result = Indd::inspectDocument(path);
		QVERIFY2(result.found() || result.status == Indd::InspectionStatus::NoXmp, variable);
		if (result.found())
			QVERIFY(result.pageCount >= 0); // XMP fields are optional, including NPages.
	}
}

void InddProbeTests::rejectsOversizedInventoryFields()
{
	const QByteArray headers = makeHeader(8, 1, 2) + makeHeader(8, 2, 2);
	QByteArray oversized = sampleInspectionXmp();
	oversized.replace("Example-Regular", QByteArray(4097, 'A'));
	QCOMPARE(inspectionBytes(headers + makeXmpObject(oversized)).status, Indd::InspectionStatus::LimitExceeded);

	QByteArray nested = sampleXmp();
	QByteArray deep;
	for (int i = 0; i < 70; ++i)
		deep += "<layer>";
	for (int i = 0; i < 70; ++i)
		deep += "</layer>";
	nested.replace("</rdf:RDF>", deep + "</rdf:RDF>");
	QCOMPARE(inspectionBytes(headers + makeXmpObject(nested)).status, Indd::InspectionStatus::LimitExceeded);
}

void InddProbeTests::survivesDeterministicMutations()
{
	QTemporaryDir tempDir;
	QVERIFY(tempDir.isValid());
	const QString path = tempDir.filePath("mutated.indd");
	const QByteArray original = makeHeader(8, 1, 2) + makeHeader(8, 2, 2) + makeXmpObject(sampleInspectionXmp());
	QRandomGenerator random(0x1ddd);
	for (int iteration = 0; iteration < 96; ++iteration)
	{
		QByteArray bytes = original;
		for (int mutation = 0, count = 1 + random.bounded(8); mutation < count; ++mutation)
		{
			const int offset = random.bounded(bytes.size());
			bytes[offset] = char(random.bounded(256));
		}
		QFile file(path);
		QVERIFY(file.open(QIODevice::WriteOnly | QIODevice::Truncate));
		QCOMPARE(file.write(bytes), qint64(bytes.size()));
		file.close();

		const auto probe = Indd::probeFile(path);
		if (probe.valid())
			QVERIFY(probe.databasePageCount <= quint64(bytes.size()) / 4096);
		const auto metadata = Indd::readMetadata(path);
		if (metadata.found())
			QVERIFY(metadata.xmpByteCount <= 16 * 1024 * 1024);
		const auto preview = Indd::readPreview(path);
		if (preview.found())
		{
			QVERIFY(preview.width <= 4096 && preview.height <= 4096);
			QVERIFY(preview.jpegBytes.size() <= 8 * 1024 * 1024);
		}
		const auto inventory = Indd::inspectDocument(path);
		if (inventory.found())
		{
			QVERIFY(inventory.pageCount <= 1000000);
			QVERIFY(inventory.fontPostScriptNames.size() <= 512);
			QVERIFY(inventory.ingredientUris.size() <= 2048);
		}
	}
}

QTEST_GUILESS_MAIN(InddProbeTests)
#include "inddprobetests.moc"
