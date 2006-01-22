/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MYPLUGINIMPL_H
#define MYPLUGINIMPL_H

#include <qobject.h>

class QString;

class MyPluginImpl : public QObject
{
	Q_OBJECT
	public:
		MyPluginImpl();
		~MyPluginImpl() {};
		bool run(const QString & target);
};

#endif
