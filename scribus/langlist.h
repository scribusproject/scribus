#ifndef LANGLIST_H
#define LANGLIST_H

#include <qmap.h>
#include <qstring.h>

class LangList : public QMap<QString, QString>
{
public:
	LangList();

};

#endif

