/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_ORA_H
#define SCIMGDATALOADER_ORA_H

#include "scimgdataloader.h"
#include <QDomDocument>
#include <QDomElement>
#include <QStack>

class ScImgDataLoader_ORA : public ScImgDataLoader
{
public:
	ScImgDataLoader_ORA(void);

	virtual bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha);
	virtual void loadEmbeddedProfile(const QString& fn, int page = 0);
	virtual bool loadPicture(const QString& fn, int page, int res, bool thumbnail);
protected:
	void initSupportedFormatList();
	struct oraLayer
	{
		int x;
		int y;
		double opacity;
		bool visible;
		QString compositeOp;
		QString fileName;
	};
	QStack<oraLayer> OraLayerStack;
	void parseStackXML(QDomElement &elem);
};

#endif
