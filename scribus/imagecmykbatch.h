/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef IMAGECMYKBATCH_H
#define IMAGECMYKBATCH_H

#include <QString>
#include <QVector>

#include "imagecmykconversion.h"
#include "scribusapi.h"

class ScribusDoc;

struct ImageCMYKBatchOptions
{
	ImageCMYKConversionOptions color;
	bool dryRun {false};
	bool relink {false};
	bool copyOriginals {false};
};

struct ImageCMYKBatchEntry
{
	QString frame;
	QString source;
	QString destination;
	QString backup;
	QString status;
	QString error;
};

struct ImageCMYKBatchResult
{
	QVector<ImageCMYKBatchEntry> entries;
	QString reportPath;
	QString error;
	int ready {0};
	int exported {0};
	int relinked {0};
	int failed {0};
};

SCRIBUS_API ImageCMYKBatchResult runImageCMYKBatch(ScribusDoc* doc,
	const QString& outputDirectory, const ImageCMYKBatchOptions& options);

#endif
