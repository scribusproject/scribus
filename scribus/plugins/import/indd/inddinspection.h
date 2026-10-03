/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef INDDINSPECTION_H
#define INDDINSPECTION_H

#include <QStringList>

namespace Indd
{

enum class InspectionStatus
{
	Found,
	NoXmp,
	InvalidDocument,
	MalformedXmp,
	InvalidValue,
	LimitExceeded,
	IoError
};

struct InspectionResult
{
	InspectionStatus status { InspectionStatus::IoError };
	int pageCount { 0 }; // Zero means the XMP did not supply a reliable count.
	QStringList fontPostScriptNames;
	QStringList ingredientUris; // XMP references, not verified current links.

	bool found() const { return status == InspectionStatus::Found; }
};

// Read-only XMP inventory. This does not parse editable INDD page content.
InspectionResult inspectDocument(const QString& filePath);

} // namespace Indd

#endif
