#ifndef PAGESTRUCTS_H
#define PAGESTRUCTS_H

#include <qvaluevector.h>
#include <qstring.h>

struct ObjectAttribute 
{
	QString name;
	QString type;
	QString value;
	QString parameter;
	QString relationship;
	QString relationshipto;
	QString autoaddto;
};

typedef QValueVector<ObjectAttribute> ObjAttrVector;

struct ToCSetup
{
	QString name; //Name of ToC
	QString itemAttrName; //Attribute to Scan for
	QString frameName; //Destination frame
	QString style; //Selected ToC style
};

typedef QValueVector<ToCSetup> ToCSetupVector;
#endif
