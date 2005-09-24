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
