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

typedef enum {Beginning, End, NotShown} TOCPageLocation;

struct ToCSetup
{
	QString name; //Name of ToC
	QString itemAttrName; //Attribute to Scan for
	QString frameName; //Destination frame
	TOCPageLocation pageLocation; //Place the page number for the TOC at the beginning, end or not at all
	bool listNonPrintingFrames; //List non printing frames in the TOC
	QString textStyle; //Paragraph style for text
	//QString leaderParaStyle; //Paragraph style for leaders
	//QString pageNumberParaStyle; //Paragraph style for page numbers
};

typedef QValueVector<ToCSetup> ToCSetupVector;
#endif
