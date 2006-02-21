/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef STENCILREADER_H
#define STENCILREADER_H

#include "scribusapi.h"
#include "scribusstructs.h"


class SCRIBUS_API StencilReader
{
public:
	StencilReader();
	~StencilReader() {};
	QPixmap createPreview(QString data);
};
#endif
