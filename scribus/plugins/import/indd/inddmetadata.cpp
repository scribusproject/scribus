/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "inddmetadata.h"

#include "inddprobe.h"

#include <QFile>
#include <QXmlStreamReader>

#include <cstring>

namespace Indd
{
namespace
{
constexpr qint64 databaseBlockSize = 4096;
constexpr qint64 objectHeaderSize = 32;
constexpr qint64 objectTrailerSize = 32;
constexpr qint64 maxXmpBytes = 16 * 1024 * 1024;
constexpr int maxObjects = 4096;

constexpr unsigned char objectHeaderSignature[16] = {
	0xde, 0x39, 0x39, 0x79, 0x51, 0x88, 0x4b, 0x6c,
	0x8e, 0x63, 0xee, 0xf8, 0xae, 0xe0, 0xdd, 0x38
};
constexpr unsigned char objectTrailerSignature[16] = {
	0xfd, 0xce, 0xdb, 0x70, 0xf7, 0x86, 0x4b, 0x4f,
	0xa4, 0xd3, 0xc7, 0x28, 0xb3, 0x41, 0x71, 0x06
};

quint32 read32(const char* bytes, bool littleEndian)
{
	const auto* p = reinterpret_cast<const unsigned char*>(bytes);
	if (littleEndian)
		return quint32(p[0]) | (quint32(p[1]) << 8) | (quint32(p[2]) << 16) | (quint32(p[3]) << 24);
	return quint32(p[3]) | (quint32(p[2]) << 8) | (quint32(p[1]) << 16) | (quint32(p[0]) << 24);
}

QString readDcValue(QXmlStreamReader& xml, const QString& name)
{
	QString firstListItem;
	QString plainText;
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isEndElement() && xml.namespaceUri() == QLatin1String("http://purl.org/dc/elements/1.1/") && xml.name() == name)
			break;
		if (xml.isStartElement() && xml.namespaceUri() == QLatin1String("http://www.w3.org/1999/02/22-rdf-syntax-ns#") && xml.name() == QLatin1String("li"))
		{
			const QString item = xml.readElementText(QXmlStreamReader::SkipChildElements).trimmed();
			if (firstListItem.isEmpty())
				firstListItem = item;
		}
		else if (xml.isCharacters() && !xml.isWhitespace())
			plainText += xml.text();
	}
	return firstListItem.isEmpty() ? plainText.trimmed() : firstListItem;
}

bool parseXmp(const QByteArray& packet, MetadataResult& result)
{
	QXmlStreamReader xml(packet);
	bool hasRdf = false;
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isDTD() || xml.isEntityReference())
			return false;
		if (!xml.isStartElement())
			continue;

		const auto ns = xml.namespaceUri();
		const auto name = xml.name();
		if (ns == QLatin1String("http://www.w3.org/1999/02/22-rdf-syntax-ns#") && name == QLatin1String("RDF"))
			hasRdf = true;
		if (ns == QLatin1String("http://purl.org/dc/elements/1.1/"))
		{
			if (name == QLatin1String("title") && result.title.isEmpty())
				result.title = readDcValue(xml, QStringLiteral("title"));
			else if (name == QLatin1String("creator") && result.creator.isEmpty())
				result.creator = readDcValue(xml, QStringLiteral("creator"));
		}
		else if (ns == QLatin1String("http://ns.adobe.com/xap/1.0/"))
		{
			if (name == QLatin1String("CreateDate") && result.created.isEmpty())
				result.created = xml.readElementText().trimmed();
			else if (name == QLatin1String("ModifyDate") && result.modified.isEmpty())
				result.modified = xml.readElementText().trimmed();
		}
		else if (ns == QLatin1String("http://www.w3.org/1999/02/22-rdf-syntax-ns#") && name == QLatin1String("Description"))
		{
			for (const auto& attribute : xml.attributes())
			{
				if (attribute.namespaceUri() != QLatin1String("http://ns.adobe.com/xap/1.0/"))
					continue;
				if (attribute.name() == QLatin1String("CreateDate") && result.created.isEmpty())
					result.created = attribute.value().toString();
				else if (attribute.name() == QLatin1String("ModifyDate") && result.modified.isEmpty())
					result.modified = attribute.value().toString();
			}
		}
	}
	return !xml.hasError() && hasRdf;
}

bool isZeroPadding(const QByteArray& bytes)
{
	for (char byte : bytes)
	{
		if (byte != '\0')
			return false;
	}
	return true;
}
} // namespace

XmpPacketResult readXmpPacket(const QString& filePath)
{
	XmpPacketResult result;
	const ProbeResult probe = probeFile(filePath);
	if (!probe.valid())
	{
		result.status = probe.status == ProbeStatus::IoError ? MetadataStatus::IoError : MetadataStatus::InvalidDocument;
		return result;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
		return result;
	const qint64 fileSize = file.size();
	qint64 position = qint64(probe.databasePageCount) * databaseBlockSize;
	for (int object = 0; object < maxObjects; ++object)
	{
		if (position > fileSize - objectHeaderSize)
		{
			result.status = MetadataStatus::NoXmp;
			return result;
		}
		if (!file.seek(position))
			return result;
		const QByteArray header = file.read(objectHeaderSize);
		if (header.size() != objectHeaderSize)
			return result;
		if (isZeroPadding(header))
		{
			result.status = MetadataStatus::NoXmp;
			return result;
		}
		if (std::memcmp(header.constData(), objectHeaderSignature, 16) != 0)
		{
			result.status = MetadataStatus::InvalidObject;
			return result;
		}

		const quint32 length = read32(header.constData() + 24, true);
		if (qint64(length) > fileSize - position - objectHeaderSize - objectTrailerSize)
		{
			result.status = MetadataStatus::InvalidObject;
			return result;
		}
		const qint64 trailerPosition = position + objectHeaderSize + length;
		if (!file.seek(trailerPosition))
			return result;
		const QByteArray trailer = file.read(objectTrailerSize);
		if (trailer.size() != objectTrailerSize)
			return result;
		if (std::memcmp(trailer.constData(), objectTrailerSignature, 16) != 0 ||
			std::memcmp(header.constData() + 16, trailer.constData() + 16, 8) != 0)
		{
			result.status = MetadataStatus::InvalidObject;
			return result;
		}

		if (length >= 56)
		{
			if (!file.seek(position + objectHeaderSize))
				return result;
			const QByteArray prefix = file.read(56);
			if (prefix.size() != 56)
				return result;
			if (prefix.mid(4).startsWith("<?xpacket begin=") && prefix.contains("W5M0MpCehiHzreSzNTczkc9d"))
			{
				if (length - 4 > maxXmpBytes)
				{
					result.status = MetadataStatus::XmpTooLarge;
					return result;
				}
				const quint32 declaredLength = read32(prefix.constData(), probe.streamByteOrder == 1);
				if (declaredLength != length - 4)
				{
					result.status = MetadataStatus::InvalidObject;
					return result;
				}
				if (!file.seek(position + objectHeaderSize + 4))
					return result;
				const QByteArray packet = file.read(length - 4);
				if (packet.size() != length - 4)
					return result;
				result.packet = packet;
				result.status = MetadataStatus::Found;
				return result;
			}
		}
		position = trailerPosition + objectTrailerSize;
	}
	result.status = MetadataStatus::ScanLimit;
	return result;
}

MetadataResult readMetadata(const QString& filePath)
{
	MetadataResult result;
	const XmpPacketResult xmp = readXmpPacket(filePath);
	result.status = xmp.status;
	if (!xmp.found())
		return result;
	result.xmpByteCount = xmp.packet.size();
	result.status = parseXmp(xmp.packet, result) ? MetadataStatus::Found : MetadataStatus::MalformedXmp;
	return result;
}

} // namespace Indd
