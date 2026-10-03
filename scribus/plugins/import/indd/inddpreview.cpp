/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#include "inddpreview.h"

#include "inddmetadata.h"

#include <QBuffer>
#include <QImageReader>
#include <QXmlStreamReader>

#include <utility>

namespace Indd
{
namespace
{
constexpr qsizetype maxJpegBytes = 8 * 1024 * 1024;
constexpr int maxDimension = 4096;

struct PreviewCandidate
{
	int pageNumber { 0 };
	int width { 0 };
	int height { 0 };
	QString format;
	QString encodedImage;
};

PreviewCandidate readPageItem(QXmlStreamReader& xml)
{
	PreviewCandidate candidate;
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isDTD() || xml.isEntityReference())
		{
			xml.raiseError(QStringLiteral("XMP entities are not supported"));
			break;
		}
		if (xml.isEndElement() && xml.namespaceUri() == QLatin1String("http://www.w3.org/1999/02/22-rdf-syntax-ns#") && xml.name() == QLatin1String("li"))
			break;
		if (!xml.isStartElement())
			continue;

		const auto ns = xml.namespaceUri();
		const auto name = xml.name();
		if (ns == QLatin1String("http://ns.adobe.com/xap/1.0/t/pg/") && name == QLatin1String("PageNumber"))
			candidate.pageNumber = xml.readElementText().trimmed().toInt();
		else if (ns == QLatin1String("http://ns.adobe.com/xap/1.0/g/img/"))
		{
			if (name == QLatin1String("format"))
				candidate.format = xml.readElementText().trimmed();
			else if (name == QLatin1String("width"))
				candidate.width = xml.readElementText().trimmed().toInt();
			else if (name == QLatin1String("height"))
				candidate.height = xml.readElementText().trimmed().toInt();
			else if (name == QLatin1String("image"))
				candidate.encodedImage = xml.readElementText();
		}
	}
	return candidate;
}

PreviewResult decodeCandidate(const PreviewCandidate& candidate)
{
	PreviewResult result;
	result.status = PreviewStatus::InvalidPreview;
	if (candidate.format != QLatin1String("JPEG") ||
		candidate.width < 1 || candidate.height < 1 ||
		candidate.width > maxDimension || candidate.height > maxDimension ||
		candidate.encodedImage.size() > (maxJpegBytes + 2) * 4 / 3 + 4096)
		return result;

	QByteArray encoded;
	encoded.reserve(candidate.encodedImage.size());
	for (QChar character : candidate.encodedImage)
	{
		const ushort code = character.unicode();
		if (code == ' ' || code == '\n' || code == '\r' || code == '\t')
			continue;
		if (code > 127)
			return result;
		encoded.append(char(code));
	}
	const auto decoded = QByteArray::fromBase64Encoding(encoded, QByteArray::AbortOnBase64DecodingErrors);
	if (!decoded || decoded.decoded.isEmpty() || decoded.decoded.size() > maxJpegBytes)
		return result;
	QByteArray jpeg = decoded.decoded;
	if (!jpeg.startsWith(QByteArray::fromHex("ffd8")) || !jpeg.endsWith(QByteArray::fromHex("ffd9")))
		return result;

	QBuffer buffer(&jpeg);
	if (!buffer.open(QIODevice::ReadOnly))
		return result;
	QImageReader reader(&buffer, "JPEG");
	reader.setAutoDetectImageFormat(false);
	const QSize size = reader.size();
	// InDesign can declare a square preview area for an aspect-preserving JPEG.
	if (size.width() < 1 || size.height() < 1 ||
		size.width() > candidate.width || size.height() > candidate.height ||
		(size.width() != candidate.width && size.height() != candidate.height))
		return result;
	const QImage image = reader.read();
	if (image.isNull() || image.size() != size)
		return result;

	result.status = PreviewStatus::Found;
	result.pageNumber = candidate.pageNumber;
	result.width = size.width();
	result.height = size.height();
	result.jpegBytes = std::move(jpeg);
	return result;
}
} // namespace

PreviewResult readPreview(const QString& filePath, int pageNumber)
{
	PreviewResult result;
	if (pageNumber < 1)
	{
		result.status = PreviewStatus::InvalidRequest;
		return result;
	}

	const XmpPacketResult xmp = readXmpPacket(filePath);
	if (!xmp.found())
	{
		if (xmp.status == MetadataStatus::NoXmp)
			result.status = PreviewStatus::NoPreview;
		else if (xmp.status == MetadataStatus::IoError)
			result.status = PreviewStatus::IoError;
		else
			result.status = PreviewStatus::InvalidDocument;
		return result;
	}

	QXmlStreamReader xml(xmp.packet);
	bool inPageInfo = false;
	bool foundInvalid = false;
	while (!xml.atEnd())
	{
		xml.readNext();
		if (xml.isDTD() || xml.isEntityReference())
		{
			xml.raiseError(QStringLiteral("XMP entities are not supported"));
			break;
		}
		if (xml.isStartElement() && xml.namespaceUri() == QLatin1String("http://ns.adobe.com/xap/1.0/") && xml.name() == QLatin1String("PageInfo"))
			inPageInfo = true;
		else if (xml.isEndElement() && xml.namespaceUri() == QLatin1String("http://ns.adobe.com/xap/1.0/") && xml.name() == QLatin1String("PageInfo"))
			inPageInfo = false;
		else if (inPageInfo && xml.isStartElement() && xml.namespaceUri() == QLatin1String("http://www.w3.org/1999/02/22-rdf-syntax-ns#") && xml.name() == QLatin1String("li"))
		{
			const PreviewCandidate candidate = readPageItem(xml);
			if (candidate.pageNumber != pageNumber)
				continue;
			PreviewResult decoded = decodeCandidate(candidate);
			if (!decoded.found())
				foundInvalid = true;
			else if (!result.found() || decoded.width * decoded.height > result.width * result.height)
				result = std::move(decoded);
		}
	}
	if (xml.hasError())
		result.status = PreviewStatus::InvalidXmp;
	else if (!result.found())
		result.status = foundInvalid ? PreviewStatus::InvalidPreview : PreviewStatus::NoPreview;
	if (!result.found())
		result.jpegBytes.clear();
	return result;
}

} // namespace Indd
