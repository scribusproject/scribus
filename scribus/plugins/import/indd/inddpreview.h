/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
*/

#ifndef INDDPREVIEW_H
#define INDDPREVIEW_H

#include <QByteArray>
#include <QString>

namespace Indd
{

enum class PreviewStatus
{
	Found,
	NoPreview,
	InvalidRequest,
	InvalidDocument,
	InvalidXmp,
	InvalidPreview,
	IoError
};

struct PreviewResult
{
	PreviewStatus status { PreviewStatus::IoError };
	int pageNumber { 0 };
	int width { 0 };
	int height { 0 };
	QByteArray jpegBytes;

	bool found() const { return status == PreviewStatus::Found; }
};

// Extracts a saved page thumbnail, not a rendered or editable page.
PreviewResult readPreview(const QString& filePath, int pageNumber = 1);

} // namespace Indd

#endif
