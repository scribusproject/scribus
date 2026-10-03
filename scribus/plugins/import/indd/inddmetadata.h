/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef INDDMETADATA_H
#define INDDMETADATA_H

#include <QByteArray>
#include <QString>
#include <QtGlobal>

namespace Indd
{

enum class MetadataStatus
{
	Found,
	NoXmp,
	InvalidDocument,
	InvalidObject,
	XmpTooLarge,
	MalformedXmp,
	IoError,
	ScanLimit
};

struct MetadataResult
{
	MetadataStatus status { MetadataStatus::IoError };
	QString title;
	QString creator;
	QString created;
	QString modified;
	qsizetype xmpByteCount { 0 };

	bool found() const { return status == MetadataStatus::Found; }
};

struct XmpPacketResult
{
	MetadataStatus status { MetadataStatus::IoError };
	QByteArray packet;

	bool found() const { return status == MetadataStatus::Found; }
};

// Shared bounded reader for metadata and preview fields in the active XMP stream.
XmpPacketResult readXmpPacket(const QString& filePath);

// Read-only extraction from a bounded contiguous XMP object; no page import.
MetadataResult readMetadata(const QString& filePath);

} // namespace Indd

#endif
