/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          util.cpp  -  description
                             -------------------
    begin                : Fri Sep 14 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QFile>
#include <QPixmap>
#include <QRgb>

#include "util_icon.h"

#include "prefsmanager.h"
#include "scpaths.h"
#include "scpixmapcache.h"


QPixmap loadIcon(const QString nam, bool forceUseColor)
{
	static ScPixmapCache<QString> pxCache;
	if (pxCache.contains(nam))
		return *pxCache[nam];

	QString iconFilePath(QString("%1%2").arg(ScPaths::instance().iconDir()).arg(nam));
	QPixmap *pm = new QPixmap();
	
	if (!QFile::exists(iconFilePath))
		qWarning("Unable to load icon %s: File not found", iconFilePath.toAscii().constData());
	else
	{
		pm->load(iconFilePath);
		if (pm->isNull())
			qWarning("Unable to load icon %s: Got null pixmap", iconFilePath.toAscii().constData());
		if (PrefsManager::instance()->appPrefs.grayscaleIcons && !forceUseColor)
		{
			QImage qi(pm->toImage());
			int h=qi.height();
			int w=qi.width();
			QRgb c_rgb;
			for (int i=0;i<w;++i)
			{
				for (int j=0;j<h;++j)
				{
					c_rgb=qi.pixel(i,j);
					int k = qMin(qRound(0.3 * qRed(c_rgb) + 0.59 * qGreen(c_rgb) + 0.11 * qBlue(c_rgb)), 255);
					qi.setPixel(i, j, qRgba(k, k, k, qAlpha(c_rgb)));
				}
			}
			*pm=QPixmap::fromImage(qi);
		}
	}
	pxCache.insert(nam, pm);
	return *pm;
}
