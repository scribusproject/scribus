/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCPIXMAPCACHE_H
#define SCPIXMAPCACHE_H

#include <QMap>
#include <QPixmap>

template <typename T>
class ScPixmapCache : public QMap<T, QPixmap*>
{
public:
	ScPixmapCache() {};
	~ScPixmapCache();
};

template <typename T>
ScPixmapCache<T>::~ScPixmapCache()
{
	typename QMap<T, QPixmap*>::iterator it;
	typename QMap<T, QPixmap*>::iterator itEnd;

	itEnd = this->end();
	for (it = this->begin(); it != itEnd; it++)
		delete (it.value());
};

#endif
