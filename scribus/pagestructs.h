#ifndef PAGESTRUCTS_H
#define PAGESTRUCTS_H

#include <qmap.h>
#include <qstring.h>

struct ObjectAttribute {
	QString name;
	QString type;
	QString value;
	QString parameter;
};

typedef QMap<int, ObjectAttribute> ObjAttrMap;
#endif
