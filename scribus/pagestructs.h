#ifndef PAGESTRUCTS_H
#define PAGESTRUCTS_H

#include <qvaluevector.h>
#include <qstring.h>

struct ObjectAttribute {
	QString name;
	QString type;
	QString value;
	QString parameter;
	QString relationship;
	QString relationshipto;
	QString autoaddto;
};

typedef QValueVector<ObjectAttribute> ObjAttrVector;
#endif
