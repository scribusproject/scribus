#ifndef OLDSCRIBUSFORMATIMPL_H
#define OLDSCRIBUSFORMATIMPL_H

#include <qobject.h>

class QString;

class OldScribusFormatImpl : public QObject
{
	Q_OBJECT
	public:
		OldScribusFormatImpl();
		~OldScribusFormatImpl() {};
		bool run(const QString & target);
};

#endif
