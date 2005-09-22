#ifndef MYPLUGINIMPL_H
#define MYPLUGINIMPL_H

#include <qobject.h>

class QString;

class MyPluginImpl : public QObject
{
	public:
		MyPluginImpl();
		~MyPluginImpl() {};
		bool run(const QString & target);
};

#endif
