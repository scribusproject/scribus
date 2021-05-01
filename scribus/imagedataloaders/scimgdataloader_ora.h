/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCIMGDATALOADER_ORA_H
#define SCIMGDATALOADER_ORA_H

#include "scimgdataloader.h"
#include "scpainter.h"
#include "third_party/zip/scribus_zip.h"
#include <QDomDocument>
#include <QDomElement>

class ScImgDataLoader_ORA : public ScImgDataLoader
{
public:
	ScImgDataLoader_ORA();

	bool preloadAlphaChannel(const QString& fn, int page, int res, bool& hasAlpha) override;
	void loadEmbeddedProfile(const QString& fn, int page = 0) override;
	bool loadPicture(const QString& fn, int page, int res, bool thumbnail) override;

protected:
	void initSupportedFormatList();
	void parseStackXML(QDomElement &elem, ScPainter *painter, ScZipHandler *uz);
	int blendModeToInt(const QString& compositeOp);
	QString blendModeToString(const QString& compositeOp);
	int m_inSubLayer;
	int m_layerCount;
};

#endif
