/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "inddprobe.h"

#include <QFile>

#include <cstring>

namespace Indd
{
namespace
{
constexpr qsizetype headerSize = 4096;
constexpr qsizetype signatureSize = 16;
constexpr qsizetype documentTypeOffset = 16;
constexpr qsizetype byteOrderOffset = 24;
constexpr qsizetype versionOffset = 29;
constexpr qsizetype sequenceOffset = 264;
constexpr qsizetype databasePageCountOffset = 280;

constexpr unsigned char signature[signatureSize] = {
	0x06, 0x06, 0xed, 0xf5, 0xd8, 0x1d, 0x46, 0xe5,
	0xbd, 0x31, 0xef, 0xe7, 0xfe, 0x74, 0xb7, 0x1d
};

quint32 read32(const char* bytes, bool littleEndian)
{
	const auto* p = reinterpret_cast<const unsigned char*>(bytes);
	if (littleEndian)
		return quint32(p[0]) | (quint32(p[1]) << 8) | (quint32(p[2]) << 16) | (quint32(p[3]) << 24);
	return quint32(p[3]) | (quint32(p[2]) << 8) | (quint32(p[1]) << 16) | (quint32(p[0]) << 24);
}

quint64 read64LittleEndian(const char* bytes)
{
	const auto* p = reinterpret_cast<const unsigned char*>(bytes);
	quint64 value = 0;
	for (int i = 7; i >= 0; --i)
		value = (value << 8) | p[i];
	return value;
}

ProbeStatus validateHeader(const char* header)
{
	if (std::memcmp(header, signature, signatureSize) != 0)
		return ProbeStatus::InvalidSignature;
	if (std::memcmp(header + documentTypeOffset, "DOCUMENT", 8) != 0)
		return ProbeStatus::InvalidDocumentType;
	const auto byteOrder = static_cast<unsigned char>(header[byteOrderOffset]);
	if (byteOrder != 1 && byteOrder != 2)
		return ProbeStatus::InvalidByteOrder;
	if (read32(header + versionOffset, byteOrder == 1) == 0)
		return ProbeStatus::InvalidVersion;
	return ProbeStatus::Valid;
}
} // namespace

ProbeResult probeFile(const QString& filePath)
{
	ProbeResult result;
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
		return result;

	result.fileSize = file.size();
	if (result.fileSize < 2 * headerSize)
	{
		result.status = ProbeStatus::TooShort;
		return result;
	}

	const QByteArray headers = file.read(2 * headerSize);
	if (headers.size() != 2 * headerSize)
		return result;

	const char* first = headers.constData();
	const char* second = first + headerSize;
	result.status = validateHeader(first);
	if (result.status != ProbeStatus::Valid)
		return result;
	result.status = validateHeader(second);
	if (result.status != ProbeStatus::Valid)
		return result;

	const quint64 firstSequence = read64LittleEndian(first + sequenceOffset);
	const quint64 secondSequence = read64LittleEndian(second + sequenceOffset);
	const char* active = secondSequence > firstSequence ? second : first;
	result.activeHeaderSequence = qMax(firstSequence, secondSequence);
	result.streamByteOrder = static_cast<unsigned char>(active[byteOrderOffset]);
	result.formatVersion = read32(active + versionOffset, result.streamByteOrder == 1);
	result.databasePageCount = read32(active + databasePageCountOffset, result.streamByteOrder == 1);
	if (result.databasePageCount < 2 || result.databasePageCount > quint64(result.fileSize) / headerSize)
		result.status = ProbeStatus::InvalidDatabasePageCount;
	return result;
}

} // namespace Indd
