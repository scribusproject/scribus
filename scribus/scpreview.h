/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCPREVIEW_H
#define SCPREVIEW_H

#include "scribusapi.h"
#include "scribusstructs.h"

#include <QImage>
#include <QString>

class SCRIBUS_API ScPreview
{
public:
	ScPreview();
	~ScPreview() {};
	QImage createPreview(QString data);
};
#endif

