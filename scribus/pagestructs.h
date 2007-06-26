/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PAGESTRUCTS_H
#define PAGESTRUCTS_H

#include <QMap>
#include <QList>
#include <QString>

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

typedef QList<ObjectAttribute> ObjAttrVector;

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

typedef QList<ToCSetup> ToCSetupVector;


typedef enum 
{
	Type_1_2_3,
	Type_i_ii_iii,
	Type_I_II_III,
	Type_a_b_c,
	Type_A_B_C,
	Type_None=99
} DocumentSectionType;

struct DocumentSection
{
	uint number; //Just an index in the section list
	QString name; //User defined name for the section
	uint fromindex; //First page _index_ of the section in the document (old page number)
	uint toindex; //Last page _index_ of the section in the document (old page number)
	DocumentSectionType type; //Type of section numbering, ie i,ii,iii or a,b,c or 1,2,3, etc
	uint sectionstartindex; // Start of section, an index in the range of type, eg for type i,ii,iii, this would be 2 for "ii".
	bool reversed; // Counting 10-1 ?
	bool active; // Is the section active, ie, if the fromindex is 10, and theres 5 pages, this should be inactive.
};

typedef QMap<uint, DocumentSection> DocumentSectionMap;

typedef enum
{
	singlePage,
	doublePage,
	triplePage,
	quadroPage
} PageLayout;

typedef enum
{
	LeftPage,
	MiddlePage,
	RightPage
} PageLocation;


#endif
