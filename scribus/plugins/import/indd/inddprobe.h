/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef INDDPROBE_H
#define INDDPROBE_H

#include <QString>
#include <QtGlobal>

namespace Indd
{

enum class ProbeStatus
{
	Valid,
	IoError,
	TooShort,
	InvalidSignature,
	InvalidDocumentType,
	InvalidByteOrder,
	InvalidVersion,
	InvalidDatabasePageCount
};

struct ProbeResult
{
	ProbeStatus status { ProbeStatus::IoError };
	quint32 formatVersion { 0 };
	quint8 streamByteOrder { 0 };
	quint64 activeHeaderSequence { 0 };
	quint32 databasePageCount { 0 };
	qint64 fileSize { 0 };

	bool valid() const { return status == ProbeStatus::Valid; }
};

// This only validates the native INDD container header. It does not import pages.
ProbeResult probeFile(const QString& filePath);

} // namespace Indd

#endif
