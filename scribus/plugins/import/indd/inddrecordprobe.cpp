/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "inddrecordprobe.h"

#include "inddprobe.h"

#include <QFile>
#include <QHash>
#include <QSet>

#include <algorithm>
#include <cmath>
#include <cstring>

namespace Indd
{
namespace
{
constexpr qsizetype blockSize = 4096;
constexpr quint16 minimumFirstOffset = 32;
constexpr quint16 minimumRecordLength = 8;
constexpr int maximumRecordsPerPage = blockSize / minimumRecordLength;
constexpr quint32 indexedRecordChainEnd = 0xf5c;
constexpr quint32 footerSlotTableOffset = 0xf60;
constexpr quint16 maximumOrdinarySlot = 31;
constexpr quint16 indexedMinimumFirstOffset = 20;
constexpr quint32 indexSlotCountOffset = 4060;
constexpr quint32 footerTypeOffset = 4084;
constexpr quint32 footerTokenOffset = 4088;
constexpr quint32 observedRecordFooterType = 9;
constexpr quint32 observedDirectoryFooterType = 5;
constexpr quint32 directoryTableOffset = 128;
constexpr quint32 directoryTableCapacity = (4080 - directoryTableOffset) / 4;

quint16 read16(const char* bytes, bool littleEndian)
{
	const auto* p = reinterpret_cast<const unsigned char*>(bytes);
	return littleEndian ? quint16(p[0] | (quint16(p[1]) << 8))
	                    : quint16(p[1] | (quint16(p[0]) << 8));
}

quint32 read32(const char* bytes, bool littleEndian)
{
	const auto* p = reinterpret_cast<const unsigned char*>(bytes);
	if (littleEndian)
		return quint32(p[0]) | (quint32(p[1]) << 8) | (quint32(p[2]) << 16) | (quint32(p[3]) << 24);
	return quint32(p[3]) | (quint32(p[2]) << 8) | (quint32(p[1]) << 16) | (quint32(p[0]) << 24);
}

double readLittleEndianDouble(const char* bytes)
{
	quint64 bits = 0;
	for (int i = 0; i < 8; ++i)
		bits |= quint64(quint8(bytes[i])) << (8 * i);
	static_assert(sizeof(bits) == sizeof(double), "INDD double must be 64 bits");
	double value = 0;
	std::memcpy(&value, &bits, sizeof(value));
	return value;
}

quint64 readLittleEndian64(const char* bytes)
{
	return quint64(read32(bytes, true)) | (quint64(read32(bytes + 4, true)) << 32);
}

bool validatePageLocalSlotIndex(const QByteArray& page, RecordPageResult& result,
	quint32 tableOffset, quint16 maximumSlot, bool rejectHigherSlots)
{
	quint64 seenSlots = 0;
	bool hasOrdinarySlot = false;
	for (const RecordEnvelope& envelope : result.records)
	{
		// The high bit varies on otherwise matching slots in observed files.
		const quint16 slot = envelope.wordAt2 & 0x7fff;
		if (slot == 0)
		{
			if (rejectHigherSlots && envelope.wordAt2 != 0)
				return false;
			continue;
		}
		if (slot > maximumSlot)
		{
			if (rejectHigherSlots)
				return false;
			continue;
		}
		hasOrdinarySlot = true;
		const quint64 slotBit = quint64(1) << (slot - 1);
		const quint32 entryOffset = tableOffset + 4 * (maximumSlot - slot);
		if ((seenSlots & slotBit) || read32(page.constData() + entryOffset, true) != envelope.offset)
			return false;
		seenSlots |= slotBit;
	}
	if (!hasOrdinarySlot)
		return false;
	result.footerSlotIndexValidated = true;
	for (RecordEnvelope& envelope : result.records)
	{
		const quint16 slot = envelope.wordAt2 & 0x7fff;
		envelope.slotIndexEntryMatches = slot >= 1 && slot <= maximumSlot;
	}
	return true;
}

RecordPageResult inspectPageBytes(const QByteArray& page, bool littleEndian)
{
	RecordPageResult result;
	quint32 offset = read16(page.constData(), littleEndian);
	const quint32 slotCount = littleEndian && read32(page.constData() + footerTypeOffset, true) == observedRecordFooterType
		? read32(page.constData() + indexSlotCountOffset, true) : 0;
	const bool indexedVariant = slotCount == 16 || slotCount == 32 || slotCount == 48 || slotCount == 64;
	const quint32 indexStart = indexedVariant ? indexSlotCountOffset - 4 * slotCount : 0;
	// Some indexed pages begin with slot 1's envelope at byte zero instead of
	// a two-byte first-envelope offset. Treat this as a separate, guarded
	// layout and require the complete chain and slot table to validate below.
	const bool zeroStartIndexedVariant = indexedVariant &&
		(read16(page.constData() + 2, true) & 0x7fff) == 1 &&
		offset >= minimumFirstOffset && offset % 4 == 0 && offset < indexStart;
	if (zeroStartIndexedVariant)
		offset = 0;
	if ((!zeroStartIndexedVariant && offset < (indexedVariant ? indexedMinimumFirstOffset : minimumFirstOffset)) ||
		offset % 4 != 0 || offset > blockSize - minimumRecordLength ||
		(indexedVariant && offset >= indexStart))
	{
		result.status = RecordPageStatus::NotRecordPage;
		return result;
	}

	while (offset <= blockSize - minimumRecordLength && result.records.size() < maximumRecordsPerPage)
	{
		// Indexed pages may have no zero-length terminator; their table starts
		// immediately after the last envelope and must match every used slot.
		if (indexedVariant && offset == indexStart)
		{
			if (validatePageLocalSlotIndex(page, result, indexStart, quint16(slotCount), true))
			{
				result.status = RecordPageStatus::Parsed;
				return result;
			}
			break;
		}
		const char* record = page.constData() + offset;
		const quint16 length = read16(record, littleEndian);
		if (length == 0)
		{
			if (indexedVariant)
				break;
			result.status = result.records.isEmpty() ? RecordPageStatus::NotRecordPage : RecordPageStatus::Parsed;
			// This footer layout was observed in controlled little-endian saves.
			// A slot maps only to an envelope on this physical page; it says
			// nothing about whether the page belongs to the active database.
			if (result.parsed() && littleEndian && offset == indexedRecordChainEnd)
				validatePageLocalSlotIndex(page, result, footerSlotTableOffset, maximumOrdinarySlot, false);
			return result;
		}
		if (length < minimumRecordLength || length % 4 != 0 ||
			offset + length > (indexedVariant ? indexStart : blockSize))
			break;
		result.records.append({ quint16(offset), length, read16(record + 2, littleEndian), read32(record + 4, littleEndian) });
		offset += length;
	}
	result.records.clear();
	result.status = RecordPageStatus::NotRecordPage;
	return result;
}

quint64 slotKey(const DirectoryMappedRecordSlotReference& slot)
{
	return (quint64(slot.opaquePageToken) << 16) | slot.pageLocalSlot;
}

RecordSlotDiffStatus diffStatusForCatalog(PageDirectoryStatus status)
{
	switch (status)
	{
	case PageDirectoryStatus::Validated:
		return RecordSlotDiffStatus::Compared;
	case PageDirectoryStatus::IoError:
		return RecordSlotDiffStatus::IoError;
	case PageDirectoryStatus::InvalidDocument:
		return RecordSlotDiffStatus::InvalidDocument;
	case PageDirectoryStatus::UnsupportedLayout:
		return RecordSlotDiffStatus::UnsupportedLayout;
	case PageDirectoryStatus::InvalidMapping:
		return RecordSlotDiffStatus::InvalidMapping;
	case PageDirectoryStatus::LimitExceeded:
		return RecordSlotDiffStatus::LimitExceeded;
	}
	return RecordSlotDiffStatus::InvalidMapping;
}

struct CachedRecordPage
{
	quint32 index { 0 }; // Physical database pages start at 2.
	QByteArray bytes;
	RecordPageResult parsed;
};

RecordSlotDiffStatus readValidatedEnvelope(QFile& file,
	const DirectoryMappedRecordSlotReference& slot, CachedRecordPage& cached, QByteArray& bytes)
{
	if (cached.index != slot.databasePageIndex)
	{
		if (!file.seek(qint64(slot.databasePageIndex) * blockSize))
			return RecordSlotDiffStatus::IoError;
		cached.bytes = file.read(blockSize);
		if (cached.bytes.size() != blockSize)
			return RecordSlotDiffStatus::IoError;
		if (read32(cached.bytes.constData() + footerTypeOffset, true) != observedRecordFooterType ||
			read32(cached.bytes.constData() + footerTokenOffset, true) != slot.opaquePageToken)
			return RecordSlotDiffStatus::InputChanged;
		cached.parsed = inspectPageBytes(cached.bytes, true);
		if (!cached.parsed.parsed() || !cached.parsed.footerSlotIndexValidated)
			return RecordSlotDiffStatus::InputChanged;
		cached.index = slot.databasePageIndex;
	}
	for (const RecordEnvelope& record : cached.parsed.records)
	{
		if (!record.slotIndexEntryMatches || (record.wordAt2 & 0x7fff) != slot.pageLocalSlot)
			continue;
		if (record.offset != slot.envelopeOffset || record.length != slot.envelopeLength)
			return RecordSlotDiffStatus::InputChanged;
		bytes = cached.bytes.mid(record.offset, record.length);
		return RecordSlotDiffStatus::Compared;
	}
	return RecordSlotDiffStatus::InputChanged;
}

void countSharedAfterHeaderRuns(const QByteArray& first, const QByteArray& second,
	quint16& prefixBytes, quint16& suffixBytes)
{
	const qsizetype firstSize = first.size() - minimumRecordLength;
	const qsizetype secondSize = second.size() - minimumRecordLength;
	const qsizetype commonSize = qMin(firstSize, secondSize);
	qsizetype prefix = 0;
	while (prefix < commonSize && first[minimumRecordLength + prefix] == second[minimumRecordLength + prefix])
		++prefix;
	qsizetype suffix = 0;
	while (suffix < commonSize - prefix && first[first.size() - 1 - suffix] == second[second.size() - 1 - suffix])
		++suffix;
	prefixBytes = quint16(prefix);
	suffixBytes = quint16(suffix);
}

bool appendCandidateStoryUnit(QString& text, quint16 unit)
{
	if ((unit < 0x20 && unit != '\t' && unit != '\n' && unit != '\r') ||
		(unit >= 0xd800 && unit <= 0xdfff) || unit >= 0xfffe)
		return false;
	text.append(QChar(unit));
	return true;
}

CandidateStoryStatus decodeCandidateStoryBytes(const QByteArray& record, QString& text)
{
	constexpr qsizetype streamOffset = 30;
	if (record.size() < streamOffset || read32(record.constData() + 4, true) != 0x262 ||
		read32(record.constData() + 12, true) != 0x202 || read16(record.constData() + 20, true) != 1)
		return CandidateStoryStatus::UnsupportedRecord;
	const quint32 sectionLength = read32(record.constData() + 22, true);
	const quint32 unitCount = read32(record.constData() + 26, true);
	if (sectionLength < 4 || sectionLength > quint32(record.size() - 26) ||
		read32(record.constData() + 8, true) != sectionLength + 26 ||
		unitCount > blockSize)
		return CandidateStoryStatus::InvalidEncoding;
	const qsizetype streamEnd = 26 + sectionLength;
	qsizetype offset = streamOffset;
	quint32 decodedUnits = 0;
	QString decoded;
	decoded.reserve(int(unitCount));
	while (offset < streamEnd)
	{
		if (streamEnd - offset < 2)
			return CandidateStoryStatus::InvalidEncoding;
		const quint16 run = read16(record.constData() + offset, true);
		offset += 2;
		const quint16 kind = run & 0xc000;
		const quint16 length = run & 0x3fff;
		if (length == 0 || length > unitCount - decodedUnits)
			return CandidateStoryStatus::InvalidEncoding;
		if (kind == 0x4000)
		{
			if (length > streamEnd - offset)
				return CandidateStoryStatus::InvalidEncoding;
			for (int i = 0; i < length; ++i)
			{
				const quint16 unit = quint8(record[offset + i]);
				if (unit > 0x7f || !appendCandidateStoryUnit(decoded, unit))
					return CandidateStoryStatus::InvalidEncoding;
			}
			offset += length;
		}
		else if (kind == 0x8000)
		{
			if (2 * qsizetype(length) > streamEnd - offset)
				return CandidateStoryStatus::InvalidEncoding;
			for (int i = 0; i < length; ++i)
			{
				if (!appendCandidateStoryUnit(decoded, read16(record.constData() + offset + 2 * i, true)))
					return CandidateStoryStatus::InvalidEncoding;
			}
			offset += 2 * length;
		}
		else
			return CandidateStoryStatus::InvalidEncoding;
		decodedUnits += length;
	}
	if (decodedUnits != unitCount)
		return CandidateStoryStatus::InvalidEncoding;
	text = decoded;
	return CandidateStoryStatus::Decoded;
}

bool decodeCandidateFrameStoryWords(const QByteArray& record, quint32& frameWord, quint32& storyWord)
{
	if (record.size() != 124)
		return false;
	const char* bytes = record.constData();
	const quint32 kind = read32(bytes + 4, true);
	if (kind == 0x2ab)
	{
		frameWord = read32(bytes + 100, true);
		storyWord = read32(bytes + 16, true);
		return frameWord > 4 && storyWord != 0 &&
			read32(bytes + 8, true) == 8 && read32(bytes + 20, true) == 0x205 &&
			read32(bytes + 24, true) == 12 && read32(bytes + 28, true) == storyWord &&
			read32(bytes + 40, true) == 0x2ad && read32(bytes + 44, true) == 4 &&
			read32(bytes + 52, true) == 0x2dd && read32(bytes + 56, true) == 44 &&
			read32(bytes + 60, true) == 2 && read32(bytes + 104, true) == 0x261 &&
			read32(bytes + 108, true) == 10 &&
			read32(bytes + 12, true) == frameWord - 4 &&
			read32(bytes + 36, true) == frameWord - 1 &&
			read32(bytes + 76, true) == frameWord - 1;
	}
	if (kind == 0x2ad)
	{
		frameWord = read32(bytes + 64, true);
		storyWord = read32(bytes + 98, true);
		return frameWord > 4 && storyWord != 0 &&
			read32(bytes + 8, true) == 4 && read32(bytes + 16, true) == 0x2dd &&
			read32(bytes + 20, true) == 44 && read32(bytes + 24, true) == 2 &&
			read32(bytes + 68, true) == 0x261 && read32(bytes + 72, true) == 10 &&
			read32(bytes + 86, true) == 0x2ab && read32(bytes + 90, true) == 8 &&
			read32(bytes + 102, true) == 0x205 && read32(bytes + 106, true) == 12 &&
			read32(bytes + 110, true) == storyWord &&
			read32(bytes + 40, true) == frameWord - 1 &&
			read32(bytes + 44, true) == frameWord - 1 &&
			read32(bytes + 82, true) == frameWord - 4 &&
			read32(bytes + 94, true) == frameWord - 4;
	}
	return false;
}
} // namespace

RecordPageResult inspectCandidateRecordPage(const QString& filePath, quint32 pageIndex)
{
	RecordPageResult result;
	const ProbeResult probe = probeFile(filePath);
	if (probe.status == ProbeStatus::IoError)
		return result;
	if (!probe.valid())
	{
		result.status = RecordPageStatus::InvalidDocument;
		return result;
	}
	if (pageIndex < 2 || pageIndex >= probe.databasePageCount)
	{
		result.status = RecordPageStatus::InvalidPageIndex;
		return result;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly) || !file.seek(qint64(pageIndex) * blockSize))
		return result;
	const QByteArray page = file.read(blockSize);
	if (page.size() != blockSize)
		return result;
	return inspectPageBytes(page, probe.streamByteOrder == 1);
}

RecordCatalogResult catalogCandidateRecordPages(const QString& filePath, quint32 pageLimit, int recordLimit)
{
	RecordCatalogResult result;
	const ProbeResult probe = probeFile(filePath);
	if (probe.status == ProbeStatus::IoError)
		return result;
	if (!probe.valid())
	{
		result.status = RecordCatalogStatus::InvalidDocument;
		return result;
	}
	result.databasePageCount = probe.databasePageCount;
	if (pageLimit == 0 || pageLimit > maxCatalogDatabasePages ||
		recordLimit <= 0 || recordLimit > maxCatalogRecords ||
		probe.databasePageCount > pageLimit)
	{
		result.status = RecordCatalogStatus::LimitExceeded;
		return result;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly) || !file.seek(2 * blockSize))
		return result;
	const bool littleEndian = probe.streamByteOrder == 1;
	QSet<quint32> seenFooterTokens;
	for (quint32 pageIndex = 2; pageIndex < probe.databasePageCount; ++pageIndex)
	{
		const QByteArray page = file.read(blockSize);
		if (page.size() != blockSize)
		{
			result.records.clear();
			return result;
		}
		const RecordPageResult pageResult = inspectPageBytes(page, littleEndian);
		// This footer correlation is only established for little-endian samples.
		// Include pages with unsupported envelope layouts in the diagnostic map.
		if (littleEndian && read32(page.constData() + footerTypeOffset, true) == observedRecordFooterType)
		{
			const quint32 token = read32(page.constData() + footerTokenOffset, true);
			if (token != 0 && seenFooterTokens.contains(token))
				++result.duplicateFooterTokenCount;
			seenFooterTokens.insert(token);
			result.observedType9Footers.append({ pageIndex, token, pageResult.parsed() });
		}
		if (!pageResult.parsed())
		{
			++result.skippedPageCount;
			continue;
		}
		if (pageResult.records.size() > recordLimit - result.records.size())
		{
			result.records.clear();
			result.status = RecordCatalogStatus::LimitExceeded;
			return result;
		}
		++result.candidatePageCount;
		if (pageResult.footerSlotIndexValidated)
			++result.footerValidatedPageCount;
		for (const RecordEnvelope& record : pageResult.records)
			result.records.append({ pageIndex, record });
	}
	result.status = RecordCatalogStatus::Scanned;
	return result;
}

CandidateDirectoryRootResult inspectCandidateDirectoryRoot(const QString& filePath)
{
	CandidateDirectoryRootResult result;
	const ProbeResult probe = probeFile(filePath);
	if (probe.status == ProbeStatus::IoError)
		return result;
	if (!probe.valid())
	{
		result.status = CandidateDirectoryRootStatus::InvalidDocument;
		return result;
	}
	if (probe.streamByteOrder != 1 || probe.databasePageCount > maxCatalogDatabasePages)
	{
		result.status = CandidateDirectoryRootStatus::UnsupportedLayout;
		return result;
	}
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
		return result;
	const QByteArray headers = file.read(2 * blockSize);
	if (headers.size() != 2 * blockSize)
		return result;
	const char* first = headers.constData();
	const char* second = first + blockSize;
	const char* active = readLittleEndian64(second + 264) > readLittleEndian64(first + 264) ? second : first;
	if (readLittleEndian64(active + 264) != probe.activeHeaderSequence ||
		quint8(active[24]) != probe.streamByteOrder ||
		read32(active + 29, true) != probe.formatVersion ||
		read32(active + 280, true) != probe.databasePageCount)
	{
		result.status = CandidateDirectoryRootStatus::InputChanged;
		return result;
	}
	// Only these two physical-page choices and footer types were observed in
	// controlled InDesign 2026 saves; do not infer a broader format rule.
	const quint32 rootPageIndex = read32(active + 936, true);
	if ((rootPageIndex != 7 && rootPageIndex != 8) || rootPageIndex >= probe.databasePageCount)
	{
		result.status = CandidateDirectoryRootStatus::UnsupportedLayout;
		return result;
	}
	if (!file.seek(qint64(rootPageIndex) * blockSize))
		return result;
	const QByteArray rootPage = file.read(blockSize);
	if (rootPage.size() != blockSize)
		return result;
	if (read32(rootPage.constData() + footerTypeOffset, true) != 4 ||
		read32(rootPage.constData() + footerTokenOffset, true) != 15 - rootPageIndex)
	{
		result.status = CandidateDirectoryRootStatus::UnsupportedLayout;
		return result;
	}
	const quint32 directoryPageIndex = read32(rootPage.constData() + directoryTableOffset, true);
	if ((directoryPageIndex != 9 && directoryPageIndex != 10) ||
		directoryPageIndex >= probe.databasePageCount)
	{
		result.status = CandidateDirectoryRootStatus::UnsupportedLayout;
		return result;
	}
	if (!file.seek(qint64(directoryPageIndex) * blockSize))
		return result;
	const QByteArray directory = file.read(blockSize);
	if (directory.size() != blockSize)
		return result;
	if (read32(directory.constData() + footerTypeOffset, true) != observedDirectoryFooterType ||
		read32(directory.constData() + footerTokenOffset, true) != 19 - directoryPageIndex)
	{
		result.status = CandidateDirectoryRootStatus::UnsupportedLayout;
		return result;
	}
	result.headerRootPageIndex = rootPageIndex;
	result.directoryPageIndex = directoryPageIndex;
	result.status = CandidateDirectoryRootStatus::Candidate;
	return result;
}

PageDirectoryResult inspectCandidatePageDirectory(const QString& filePath)
{
	PageDirectoryResult result;
	const ProbeResult probe = probeFile(filePath);
	if (probe.status == ProbeStatus::IoError)
		return result;
	if (!probe.valid())
	{
		result.status = PageDirectoryStatus::InvalidDocument;
		return result;
	}
	if (probe.databasePageCount > maxCatalogDatabasePages)
	{
		result.status = PageDirectoryStatus::LimitExceeded;
		return result;
	}
	const CandidateDirectoryRootResult root = inspectCandidateDirectoryRoot(filePath);
	if (!root.candidate())
	{
		result.status = root.status == CandidateDirectoryRootStatus::IoError
			? PageDirectoryStatus::IoError : PageDirectoryStatus::UnsupportedLayout;
		return result;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly) || !file.seek(qint64(root.directoryPageIndex) * blockSize))
		return result;
	const QByteArray directory = file.read(blockSize);
	if (directory.size() != blockSize)
		return result;
	if (read32(directory.constData() + footerTypeOffset, true) != observedDirectoryFooterType ||
		read32(directory.constData() + footerTokenOffset, true) != 19 - root.directoryPageIndex)
	{
		result.status = PageDirectoryStatus::UnsupportedLayout;
		return result;
	}
	result.declaredEmptySlots = read32(directory.constData(), true);
	if (result.declaredEmptySlots > directoryTableCapacity)
	{
		result.status = PageDirectoryStatus::InvalidMapping;
		return result;
	}

	QSet<quint32> seenPhysicalPages;
	for (quint32 token = 0; token < directoryTableCapacity; ++token)
	{
		const quint32 physicalPage = read32(directory.constData() + directoryTableOffset + 4 * token, true);
		if (physicalPage == 0)
			continue;
		if (physicalPage < 2 || physicalPage >= probe.databasePageCount || seenPhysicalPages.contains(physicalPage))
		{
			result.entries.clear();
			result.status = PageDirectoryStatus::InvalidMapping;
			return result;
		}
		seenPhysicalPages.insert(physicalPage);
		if (!file.seek(qint64(physicalPage) * blockSize + footerTypeOffset))
		{
			result.entries.clear();
			return result;
		}
		const QByteArray footer = file.read(8);
		if (footer.size() != 8)
		{
			result.entries.clear();
			return result;
		}
		const quint32 footerType = read32(footer.constData(), true);
		if (read32(footer.constData() + 4, true) != token)
		{
			result.entries.clear();
			result.status = PageDirectoryStatus::InvalidMapping;
			return result;
		}
		result.entries.append({ token, physicalPage, footerType });
	}
	if (result.entries.isEmpty() ||
		result.declaredEmptySlots != directoryTableCapacity - quint32(result.entries.size()))
	{
		result.entries.clear();
		result.status = PageDirectoryStatus::InvalidMapping;
		return result;
	}
	// Recheck the selected chain after the table walk. A second, unselected
	// type-5 page can be a previous save and must not replace this mapping.
	const CandidateDirectoryRootResult recheckedRoot = inspectCandidateDirectoryRoot(filePath);
	if (!recheckedRoot.candidate() ||
		recheckedRoot.headerRootPageIndex != root.headerRootPageIndex ||
		recheckedRoot.directoryPageIndex != root.directoryPageIndex)
	{
		result.entries.clear();
		result.status = PageDirectoryStatus::UnsupportedLayout;
		return result;
	}
	result.headerRootPageIndex = root.headerRootPageIndex;
	result.directoryPageIndex = root.directoryPageIndex;
	result.status = PageDirectoryStatus::Validated;
	return result;
}

PageDirectoryAuditResult auditCandidatePageDirectory(const QString& filePath)
{
	PageDirectoryAuditResult result;
	const PageDirectoryResult directory = inspectCandidatePageDirectory(filePath);
	result.status = directory.status;
	if (!directory.validated())
		return result;
	const RecordCatalogResult catalog = catalogCandidateRecordPages(filePath);
	if (!catalog.scanned())
	{
		result.status = catalog.status == RecordCatalogStatus::LimitExceeded
			? PageDirectoryStatus::LimitExceeded : PageDirectoryStatus::IoError;
		return result;
	}

	QSet<quint32> mappedType9Pages;
	for (const PageDirectoryEntry& entry : directory.entries)
	{
		if (entry.footerType == observedRecordFooterType)
			mappedType9Pages.insert(entry.databasePageIndex);
	}
	for (const RecordPageFooterReference& footer : catalog.observedType9Footers)
	{
		if (mappedType9Pages.contains(footer.databasePageIndex))
		{
			++result.mappedType9PageCount;
			if (footer.envelopeChainParsed)
				++result.mappedParsedPageCount;
		}
		else
		{
			++result.unmappedType9PageCount;
			if (footer.envelopeChainParsed)
				++result.unmappedParsedPageCount;
		}
	}
	if (result.mappedType9PageCount != quint32(mappedType9Pages.size()))
	{
		result = {};
		result.status = PageDirectoryStatus::InvalidMapping;
	}
	return result;
}

DirectoryMappedRecordCatalogResult catalogDirectoryMappedRecordPages(const QString& filePath, int recordLimit)
{
	DirectoryMappedRecordCatalogResult result;
	const PageDirectoryResult directory = inspectCandidatePageDirectory(filePath);
	result.status = directory.status;
	if (!directory.validated())
		return result;
	if (recordLimit <= 0 || recordLimit > maxCatalogRecords)
	{
		result.status = PageDirectoryStatus::LimitExceeded;
		return result;
	}
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		result.status = PageDirectoryStatus::IoError;
		return result;
	}
	for (const PageDirectoryEntry& entry : directory.entries)
	{
		if (entry.footerType != observedRecordFooterType)
			continue;
		++result.mappedType9PageCount;
		if (!file.seek(qint64(entry.databasePageIndex) * blockSize))
		{
			result.status = PageDirectoryStatus::IoError;
			result.records.clear();
			result.validatedSlots.clear();
			result.unindexedEnvelopeCount = 0;
			return result;
		}
		const QByteArray page = file.read(blockSize);
		if (page.size() != blockSize)
		{
			result.status = PageDirectoryStatus::IoError;
			result.records.clear();
			result.validatedSlots.clear();
			result.unindexedEnvelopeCount = 0;
			return result;
		}
		// Recheck after reopening the file in case it changed during the scan.
		if (read32(page.constData() + footerTypeOffset, true) != observedRecordFooterType ||
			read32(page.constData() + footerTokenOffset, true) != entry.opaqueToken)
		{
			result.status = PageDirectoryStatus::InvalidMapping;
			result.records.clear();
			result.validatedSlots.clear();
			result.unindexedEnvelopeCount = 0;
			return result;
		}
		const RecordPageResult pageResult = inspectPageBytes(page, true);
		if (!pageResult.parsed())
		{
			++result.unsupportedPageCount;
			continue;
		}
		if (pageResult.records.size() > recordLimit - result.records.size())
		{
			result.status = PageDirectoryStatus::LimitExceeded;
			result.records.clear();
			result.validatedSlots.clear();
			result.unindexedEnvelopeCount = 0;
			return result;
		}
		++result.parsedPageCount;
		if (pageResult.footerSlotIndexValidated)
			++result.footerValidatedPageCount;
		for (const RecordEnvelope& record : pageResult.records)
		{
			result.records.append({ entry.databasePageIndex, record });
			if (record.slotIndexEntryMatches)
			{
				result.validatedSlots.append({ entry.opaqueToken, quint16(record.wordAt2 & 0x7fff),
					entry.databasePageIndex, record.offset, record.length });
			}
			else
				++result.unindexedEnvelopeCount;
		}
	}
	return result;
}

CandidateFrameStoryCatalogResult catalogCandidateFrameStoryReferences(const QString& filePath,
	int recordLimit)
{
	CandidateFrameStoryCatalogResult result;
	const DirectoryMappedRecordCatalogResult catalog = catalogDirectoryMappedRecordPages(filePath, recordLimit);
	result.status = catalog.status;
	if (!catalog.cataloged())
		return result;
	result.unsupportedPageCount = catalog.unsupportedPageCount;
	result.unindexedEnvelopeCount = catalog.unindexedEnvelopeCount;
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		result.status = PageDirectoryStatus::IoError;
		return result;
	}
	CachedRecordPage cached;
	for (const auto& slot : catalog.validatedSlots)
	{
		QByteArray bytes;
		const RecordSlotDiffStatus readStatus = readValidatedEnvelope(file, slot, cached, bytes);
		if (readStatus != RecordSlotDiffStatus::Compared)
		{
			result.references.clear();
			result.status = readStatus == RecordSlotDiffStatus::IoError
				? PageDirectoryStatus::IoError : PageDirectoryStatus::InvalidMapping;
			return result;
		}
		++result.scannedValidatedSlotCount;
		if (bytes.size() < 8)
			continue;
		const quint32 kind = read32(bytes.constData() + 4, true);
		if (kind != 0x2ab && kind != 0x2ad)
			continue;
		quint32 frameWord = 0;
		quint32 storyWord = 0;
		if (!decodeCandidateFrameStoryWords(bytes, frameWord, storyWord))
		{
			++result.rejectedShapeCount;
			continue;
		}
		result.references.append({ slot.opaquePageToken, slot.pageLocalSlot, frameWord, storyWord });
	}
	return result;
}

CandidateType6RowTargetResult findCandidateType6RowTargets(const QString& filePath,
	quint32 rawKeyWord, int matchLimit, int recordLimit)
{
	CandidateType6RowTargetResult result;
	if (matchLimit <= 0 || matchLimit > 4096)
	{
		result.status = PageDirectoryStatus::LimitExceeded;
		return result;
	}
	const PageDirectoryResult directory = inspectCandidatePageDirectory(filePath);
	result.status = directory.status;
	if (!directory.validated())
		return result;
	const DirectoryMappedRecordCatalogResult catalog = catalogDirectoryMappedRecordPages(filePath, recordLimit);
	result.status = catalog.status;
	if (!catalog.cataloged())
		return result;
	result.unsupportedType9PageCount = catalog.unsupportedPageCount;
	QHash<quint64, DirectoryMappedRecordSlotReference> indexedSlots;
	for (const auto& slot : catalog.validatedSlots)
	{
		const quint64 key = slotKey(slot);
		if (indexedSlots.contains(key))
		{
			result.status = PageDirectoryStatus::InvalidMapping;
			return result;
		}
		indexedSlots.insert(key, slot);
	}
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		result.status = PageDirectoryStatus::IoError;
		return result;
	}
	constexpr quint32 maximumType6Rows = (footerTypeOffset - 8) / 16;
	CachedRecordPage targetCache;
	for (const PageDirectoryEntry& entry : directory.entries)
	{
		if (entry.footerType != 6)
			continue;
		if (!file.seek(qint64(entry.databasePageIndex) * blockSize))
		{
			result.status = PageDirectoryStatus::IoError;
			result.targets.clear();
			return result;
		}
		const QByteArray page = file.read(blockSize);
		if (page.size() != blockSize)
		{
			result.status = PageDirectoryStatus::IoError;
			result.targets.clear();
			return result;
		}
		if (read32(page.constData() + footerTypeOffset, true) != 6 ||
			read32(page.constData() + footerTokenOffset, true) != entry.opaqueToken)
		{
			result.status = PageDirectoryStatus::InvalidMapping;
			result.targets.clear();
			return result;
		}
		const quint32 rowCount = read32(page.constData(), true);
		if (rowCount > maximumType6Rows)
		{
			++result.unsupportedType6PageCount;
			continue;
		}
		++result.scannedType6PageCount;
		for (quint32 row = 0; row < rowCount; ++row)
		{
			const quint16 offset = quint16(8 + 16 * row);
			const char* fields = page.constData() + offset;
			if (read32(fields, true) != rawKeyWord)
				continue;
			const quint32 packed = read32(fields + 4, true);
			const quint16 slot = quint16(packed >> 16);
			const quint32 targetToken = read32(fields + 8, true);
			const quint32 marker = read32(fields + 12, true);
			const quint64 key = (quint64(targetToken) << 16) | slot;
			const auto target = indexedSlots.constFind(key);
			const quint32 rawLength = (packed & 0xffff) + 4;
			const quint32 alignedLength = (rawLength + 3) & ~quint32(3);
			if (marker != 1 || slot == 0 || target == indexedSlots.cend() ||
				alignedLength != target->envelopeLength)
			{
				++result.rejectedMatchingRowCount;
				continue;
			}
			if (result.targets.size() == matchLimit)
			{
				result.status = PageDirectoryStatus::LimitExceeded;
				result.targets.clear();
				return result;
			}
			QByteArray targetBytes;
			const RecordSlotDiffStatus readStatus = readValidatedEnvelope(file, *target, targetCache, targetBytes);
			if (readStatus != RecordSlotDiffStatus::Compared)
			{
				result.status = readStatus == RecordSlotDiffStatus::IoError
					? PageDirectoryStatus::IoError : PageDirectoryStatus::InvalidMapping;
				result.targets.clear();
				return result;
			}
			const bool hasWordAt12 = targetBytes.size() >= 16;
			result.targets.append({ entry.opaqueToken, offset, targetToken, slot, target->envelopeLength,
				read32(targetBytes.constData() + 4, true),
				hasWordAt12 ? read32(targetBytes.constData() + 12, true) : 0, hasWordAt12 });
		}
	}
	// The directory and catalog were read in separate passes. Refuse a
	// changed root or table instead of combining two save generations.
	const PageDirectoryResult rechecked = inspectCandidatePageDirectory(filePath);
	if (!rechecked.validated() || rechecked.headerRootPageIndex != directory.headerRootPageIndex ||
		rechecked.directoryPageIndex != directory.directoryPageIndex ||
		rechecked.entries.size() != directory.entries.size())
	{
		result.status = PageDirectoryStatus::InvalidMapping;
		result.targets.clear();
		return result;
	}
	for (qsizetype i = 0; i < directory.entries.size(); ++i)
	{
		const auto& before = directory.entries[i];
		const auto& after = rechecked.entries[i];
		if (before.opaqueToken != after.opaqueToken ||
			before.databasePageIndex != after.databasePageIndex || before.footerType != after.footerType)
		{
			result.status = PageDirectoryStatus::InvalidMapping;
			result.targets.clear();
			return result;
		}
	}
	return result;
}

RecordSlotDiffResult compareDirectoryMappedRecordSlots(const QString& firstPath,
	const QString& secondPath, int recordLimit)
{
	RecordSlotDiffResult result;
	const ProbeResult firstProbe = probeFile(firstPath);
	const ProbeResult secondProbe = probeFile(secondPath);
	if (firstProbe.status == ProbeStatus::IoError || secondProbe.status == ProbeStatus::IoError)
		return result;
	if (!firstProbe.valid() || !secondProbe.valid())
	{
		result.status = RecordSlotDiffStatus::InvalidDocument;
		return result;
	}
	if (firstProbe.formatVersion != secondProbe.formatVersion ||
		firstProbe.streamByteOrder != secondProbe.streamByteOrder)
	{
		result.status = RecordSlotDiffStatus::IncompatibleDocuments;
		return result;
	}
	const DirectoryMappedRecordCatalogResult first = catalogDirectoryMappedRecordPages(firstPath, recordLimit);
	if (!first.cataloged())
	{
		result.status = diffStatusForCatalog(first.status);
		return result;
	}
	const DirectoryMappedRecordCatalogResult second = catalogDirectoryMappedRecordPages(secondPath, recordLimit);
	if (!second.cataloged())
	{
		result.status = diffStatusForCatalog(second.status);
		return result;
	}
	result.firstValidatedSlotCount = quint32(first.validatedSlots.size());
	result.secondValidatedSlotCount = quint32(second.validatedSlots.size());
	result.firstUnsupportedPageCount = first.unsupportedPageCount;
	result.secondUnsupportedPageCount = second.unsupportedPageCount;
	result.firstUnindexedEnvelopeCount = first.unindexedEnvelopeCount;
	result.secondUnindexedEnvelopeCount = second.unindexedEnvelopeCount;
	QHash<quint64, DirectoryMappedRecordSlotReference> secondByKey;
	for (const auto& slot : second.validatedSlots)
	{
		const quint64 key = slotKey(slot);
		if (secondByKey.contains(key))
		{
			result.status = RecordSlotDiffStatus::InvalidMapping;
			return result;
		}
		secondByKey.insert(key, slot);
	}
	QSet<quint64> firstKeys;
	QFile firstFile(firstPath);
	QFile secondFile(secondPath);
	if (!firstFile.open(QIODevice::ReadOnly) || !secondFile.open(QIODevice::ReadOnly))
		return result;
	CachedRecordPage firstPage;
	CachedRecordPage secondPage;
	for (const auto& firstSlot : first.validatedSlots)
	{
		const quint64 key = slotKey(firstSlot);
		if (firstKeys.contains(key))
		{
			result.status = RecordSlotDiffStatus::InvalidMapping;
			return result;
		}
		firstKeys.insert(key);
		if (!secondByKey.contains(key))
		{
			++result.firstOnlyCount;
			result.differences.append({ firstSlot.opaquePageToken, firstSlot.pageLocalSlot, true, false,
				false, false, firstSlot.envelopeLength });
			continue;
		}
		const auto& secondSlot = secondByKey[key];
		QByteArray firstBytes;
		QByteArray secondBytes;
		result.status = readValidatedEnvelope(firstFile, firstSlot, firstPage, firstBytes);
		if (!result.compared())
			return result;
		result.status = readValidatedEnvelope(secondFile, secondSlot, secondPage, secondBytes);
		if (!result.compared())
			return result;
		++result.commonCount;
		const bool bytesChanged = firstBytes != secondBytes;
		const bool relocated = firstSlot.databasePageIndex != secondSlot.databasePageIndex;
		if (bytesChanged)
			++result.changedEnvelopeCount;
		else
			++result.identicalEnvelopeCount;
		if (firstSlot.envelopeLength != secondSlot.envelopeLength)
			++result.changedEnvelopeLengthCount;
		if (relocated)
			++result.relocatedPhysicalPageCount;
		if (bytesChanged || relocated)
		{
			RecordSlotDiffEntry difference { firstSlot.opaquePageToken, firstSlot.pageLocalSlot, true, true,
				bytesChanged, relocated, firstSlot.envelopeLength, secondSlot.envelopeLength };
			countSharedAfterHeaderRuns(firstBytes, secondBytes, difference.sharedAfterHeaderPrefixBytes,
				difference.sharedAfterHeaderSuffixBytes);
			result.differences.append(difference);
		}
	}
	for (const auto& secondSlot : second.validatedSlots)
	{
		if (firstKeys.contains(slotKey(secondSlot)))
			continue;
		++result.secondOnlyCount;
		result.differences.append({ secondSlot.opaquePageToken, secondSlot.pageLocalSlot, false, true,
			false, false, 0, secondSlot.envelopeLength });
	}
	result.status = RecordSlotDiffStatus::Compared;
	return result;
}

ExactRecordRelocationResult compareExactCandidateRecordRelocations(const QString& firstPath,
	const QString& secondPath, int recordLimit)
{
	ExactRecordRelocationResult result;
	// Each index owns a copy of each bounded envelope, so keep a stricter cap
	// than the lightweight catalog's 100,000-record diagnostic limit.
	if (recordLimit <= 0 || recordLimit > 10000)
	{
		result.status = RecordSlotDiffStatus::LimitExceeded;
		return result;
	}
	const ProbeResult firstProbe = probeFile(firstPath);
	const ProbeResult secondProbe = probeFile(secondPath);
	if (firstProbe.status == ProbeStatus::IoError || secondProbe.status == ProbeStatus::IoError)
		return result;
	if (!firstProbe.valid() || !secondProbe.valid())
	{
		result.status = RecordSlotDiffStatus::InvalidDocument;
		return result;
	}
	if (firstProbe.formatVersion != secondProbe.formatVersion ||
		firstProbe.streamByteOrder != secondProbe.streamByteOrder)
	{
		result.status = RecordSlotDiffStatus::IncompatibleDocuments;
		return result;
	}
	const DirectoryMappedRecordCatalogResult first = catalogDirectoryMappedRecordPages(firstPath, recordLimit);
	if (!first.cataloged())
	{
		result.status = diffStatusForCatalog(first.status);
		return result;
	}
	const DirectoryMappedRecordCatalogResult second = catalogDirectoryMappedRecordPages(secondPath, recordLimit);
	if (!second.cataloged())
	{
		result.status = diffStatusForCatalog(second.status);
		return result;
	}
	result.firstValidatedSlotCount = quint32(first.validatedSlots.size());
	result.secondValidatedSlotCount = quint32(second.validatedSlots.size());
	result.firstUnsupportedPageCount = first.unsupportedPageCount;
	result.secondUnsupportedPageCount = second.unsupportedPageCount;
	result.firstUnindexedEnvelopeCount = first.unindexedEnvelopeCount;
	result.secondUnindexedEnvelopeCount = second.unindexedEnvelopeCount;
	using PayloadIndex = QHash<QByteArray, QVector<DirectoryMappedRecordSlotReference>>;
	PayloadIndex firstIndex;
	PayloadIndex secondIndex;
	auto indexFile = [](const QString& path, const DirectoryMappedRecordCatalogResult& catalog,
		PayloadIndex& index) -> RecordSlotDiffStatus
	{
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly))
			return RecordSlotDiffStatus::IoError;
		CachedRecordPage cached;
		QSet<quint64> seenAddresses;
		for (const auto& slot : catalog.validatedSlots)
		{
			if (seenAddresses.contains(slotKey(slot)))
				return RecordSlotDiffStatus::InvalidMapping;
			seenAddresses.insert(slotKey(slot));
			QByteArray bytes;
			const RecordSlotDiffStatus status = readValidatedEnvelope(file, slot, cached, bytes);
			if (status != RecordSlotDiffStatus::Compared)
				return status;
			bytes.remove(2, 2); // Only the page-local slot word is ignored.
			index[bytes].append(slot);
		}
		return RecordSlotDiffStatus::Compared;
	};
	result.status = indexFile(firstPath, first, firstIndex);
	if (!result.compared())
		return result;
	result.status = indexFile(secondPath, second, secondIndex);
	if (!result.compared())
		return result;
	for (auto it = firstIndex.cbegin(); it != firstIndex.cend(); ++it)
	{
		const auto secondIt = secondIndex.constFind(it.key());
		if (secondIt == secondIndex.cend())
		{
			result.firstUnmatchedCount += quint32(it.value().size());
			continue;
		}
		if (it.value().size() != 1 || secondIt.value().size() != 1)
		{
			result.firstAmbiguousCount += quint32(it.value().size());
			result.secondAmbiguousCount += quint32(secondIt.value().size());
			continue;
		}
		++result.uniqueExactMatchCount;
		const auto& before = it.value().front();
		const auto& after = secondIt.value().front();
		if (slotKey(before) == slotKey(after))
			++result.sameAddressCount;
		else
		{
			++result.movedAddressCount;
			result.moves.append({ before.opaquePageToken, before.pageLocalSlot,
				after.opaquePageToken, after.pageLocalSlot });
		}
	}
	for (auto it = secondIndex.cbegin(); it != secondIndex.cend(); ++it)
	{
		if (!firstIndex.contains(it.key()))
			result.secondUnmatchedCount += quint32(it.value().size());
	}
	std::sort(result.moves.begin(), result.moves.end(), [](const ExactRecordMove& left, const ExactRecordMove& right)
	{
		if (left.firstOpaquePageToken != right.firstOpaquePageToken)
			return left.firstOpaquePageToken < right.firstOpaquePageToken;
		return left.firstPageLocalSlot < right.firstPageLocalSlot;
	});
	return result;
}

CandidateStoryResult inspectCandidateStoryText(const QString& filePath,
	quint32 opaquePageToken, quint16 pageLocalSlot, int recordLimit)
{
	CandidateStoryResult result;
	result.opaquePageToken = opaquePageToken;
	result.pageLocalSlot = pageLocalSlot;
	if (pageLocalSlot == 0)
	{
		result.status = CandidateStoryStatus::NotIndexed;
		return result;
	}
	const DirectoryMappedRecordCatalogResult catalog = catalogDirectoryMappedRecordPages(filePath, recordLimit);
	if (!catalog.cataloged())
	{
		switch (catalog.status)
		{
		case PageDirectoryStatus::IoError: result.status = CandidateStoryStatus::IoError; break;
		case PageDirectoryStatus::InvalidDocument: result.status = CandidateStoryStatus::InvalidDocument; break;
		case PageDirectoryStatus::UnsupportedLayout: result.status = CandidateStoryStatus::UnsupportedLayout; break;
		case PageDirectoryStatus::InvalidMapping: result.status = CandidateStoryStatus::InvalidMapping; break;
		case PageDirectoryStatus::LimitExceeded: result.status = CandidateStoryStatus::LimitExceeded; break;
		case PageDirectoryStatus::Validated: result.status = CandidateStoryStatus::InvalidMapping; break;
		}
		return result;
	}
	const DirectoryMappedRecordSlotReference* target = nullptr;
	for (const auto& slot : catalog.validatedSlots)
	{
		if (slot.opaquePageToken != opaquePageToken || slot.pageLocalSlot != pageLocalSlot)
			continue;
		if (target)
		{
			result.status = CandidateStoryStatus::InvalidMapping;
			return result;
		}
		target = &slot;
	}
	if (!target)
	{
		result.status = CandidateStoryStatus::NotIndexed;
		return result;
	}
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
		return result;
	CachedRecordPage cached;
	QByteArray bytes;
	const RecordSlotDiffStatus readStatus = readValidatedEnvelope(file, *target, cached, bytes);
	if (readStatus == RecordSlotDiffStatus::IoError)
		return result;
	if (readStatus != RecordSlotDiffStatus::Compared)
	{
		result.status = CandidateStoryStatus::InputChanged;
		return result;
	}
	result.status = decodeCandidateStoryBytes(bytes, result.text);
	return result;
}

CandidateAffineResult inspectCandidateAffineTransform(const QString& filePath,
	quint32 opaquePageToken, quint16 pageLocalSlot, int recordLimit)
{
	CandidateAffineResult result;
	result.opaquePageToken = opaquePageToken;
	result.pageLocalSlot = pageLocalSlot;
	if (pageLocalSlot == 0)
	{
		result.status = CandidateAffineStatus::NotIndexed;
		return result;
	}
	const DirectoryMappedRecordCatalogResult catalog = catalogDirectoryMappedRecordPages(filePath, recordLimit);
	if (!catalog.cataloged())
	{
		switch (catalog.status)
		{
		case PageDirectoryStatus::IoError: break;
		case PageDirectoryStatus::InvalidDocument: result.status = CandidateAffineStatus::InvalidDocument; break;
		case PageDirectoryStatus::UnsupportedLayout: result.status = CandidateAffineStatus::UnsupportedLayout; break;
		case PageDirectoryStatus::InvalidMapping: result.status = CandidateAffineStatus::InvalidMapping; break;
		case PageDirectoryStatus::LimitExceeded: result.status = CandidateAffineStatus::LimitExceeded; break;
		case PageDirectoryStatus::Validated: result.status = CandidateAffineStatus::InvalidMapping; break;
		}
		return result;
	}
	const DirectoryMappedRecordSlotReference* target = nullptr;
	for (const auto& slot : catalog.validatedSlots)
	{
		if (slot.opaquePageToken != opaquePageToken || slot.pageLocalSlot != pageLocalSlot)
			continue;
		if (target)
		{
			result.status = CandidateAffineStatus::InvalidMapping;
			return result;
		}
		target = &slot;
	}
	if (!target)
	{
		result.status = CandidateAffineStatus::NotIndexed;
		return result;
	}
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
		return result;
	CachedRecordPage cached;
	QByteArray bytes;
	const RecordSlotDiffStatus readStatus = readValidatedEnvelope(file, *target, cached, bytes);
	if (readStatus == RecordSlotDiffStatus::IoError)
		return result;
	if (readStatus != RecordSlotDiffStatus::Compared)
	{
		result.status = CandidateAffineStatus::InputChanged;
		return result;
	}
	constexpr qsizetype affineEnvelopeLength = 952;
	constexpr qsizetype affineMarkerOffset = 896;
	constexpr qsizetype affineMatrixOffset = 904;
	if (bytes.size() != affineEnvelopeLength ||
		read32(bytes.constData() + affineMarkerOffset, true) != 0x151 ||
		read32(bytes.constData() + affineMarkerOffset + 4, true) != 48)
	{
		result.status = CandidateAffineStatus::UnsupportedRecord;
		return result;
	}
	for (int i = 0; i < 6; ++i)
	{
		const double value = readLittleEndianDouble(bytes.constData() + affineMatrixOffset + 8 * i);
		if (!std::isfinite(value) || std::abs(value) > 1e9)
		{
			result.matrix = {};
			result.status = CandidateAffineStatus::InvalidEncoding;
			return result;
		}
		result.matrix[i] = value;
	}
	result.status = CandidateAffineStatus::Decoded;
	return result;
}

RecordWordSearchResult findCandidateRecordWords(const QString& filePath, quint32 word,
	int matchLimit, int recordLimit)
{
	RecordWordSearchResult result;
	if (matchLimit <= 0 || matchLimit > 4096)
	{
		result.status = RecordWordSearchStatus::LimitExceeded;
		return result;
	}
	const DirectoryMappedRecordCatalogResult catalog = catalogDirectoryMappedRecordPages(filePath, recordLimit);
	switch (catalog.status)
	{
	case PageDirectoryStatus::IoError: return result;
	case PageDirectoryStatus::InvalidDocument: result.status = RecordWordSearchStatus::InvalidDocument; return result;
	case PageDirectoryStatus::UnsupportedLayout: result.status = RecordWordSearchStatus::UnsupportedLayout; return result;
	case PageDirectoryStatus::InvalidMapping: result.status = RecordWordSearchStatus::InvalidMapping; return result;
	case PageDirectoryStatus::LimitExceeded: result.status = RecordWordSearchStatus::LimitExceeded; return result;
	case PageDirectoryStatus::Validated: break;
	}
	result.unsupportedPageCount = catalog.unsupportedPageCount;
	result.unindexedEnvelopeCount = catalog.unindexedEnvelopeCount;
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
		return result;
	CachedRecordPage cached;
	for (const auto& slot : catalog.validatedSlots)
	{
		QByteArray bytes;
		const RecordSlotDiffStatus readStatus = readValidatedEnvelope(file, slot, cached, bytes);
		if (readStatus == RecordSlotDiffStatus::IoError)
			return result;
		if (readStatus != RecordSlotDiffStatus::Compared)
		{
			result.status = RecordWordSearchStatus::InputChanged;
			result.occurrences.clear();
			return result;
		}
		++result.scannedValidatedSlotCount;
		for (qsizetype offset = minimumRecordLength; offset + 4 <= bytes.size(); ++offset)
		{
			if (read32(bytes.constData() + offset, true) != word)
				continue;
			if (result.occurrences.size() == matchLimit)
			{
				result.status = RecordWordSearchStatus::LimitExceeded;
				result.occurrences.clear();
				return result;
			}
			result.occurrences.append({ slot.opaquePageToken, slot.pageLocalSlot, quint16(offset) });
		}
	}
	result.status = RecordWordSearchStatus::Searched;
	return result;
}

} // namespace Indd
