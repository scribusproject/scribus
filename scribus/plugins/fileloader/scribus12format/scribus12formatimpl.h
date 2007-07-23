/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUS12FORMATIMPL_H
#define SCRIBUS12FORMATIMPL_H

#include <QObject>

class QString;

class Scribus12FormatImpl : public QObject
{
	Q_OBJECT
	public:
		Scribus12FormatImpl();
		~Scribus12FormatImpl() {};
		bool run(const QString & target);
};

#endif
