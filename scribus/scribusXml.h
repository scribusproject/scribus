/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                 	scribusXml.h the document xml library for scribus
                             -------------------
    begin                : Sam Jul 14 10:00:00 CEST 2001
    copyright            : (C) 2001 by Christian T�p
    email                : christian.toepp@mr-ct@gmx.de
 ***************************************************************************/

#ifndef _SCRIBUSXML_H
#define _SCRIBUSXML_H

#include <QDomDocument>
#include <QDomElement>
#include <QList>
#include <QProgressBar>

#include "scfonts.h"
#include "scribusapi.h"
#include "scribusstructs.h"
#include "selection.h"
#include "styles/styleset.h"

#include <QXmlStreamAttributes>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
class PageItem;
class PrefsManager;
class ScribusView;
class SCFonts;
class ScribusDoc;
class ScPattern;
class ScXmlStreamWriter;

class SCRIBUS_API ScriXmlDoc
{

public:
	ScriXmlDoc();
	~ScriXmlDoc() {};
	/*!
	\author Frederic Dubuy <effediwhy@gmail.com>, Petr Vanek
	\date august 17th 2004, 10/03/2004
	\brief Preliminary Scribus file validator. totally rewritten when fixing crash bug #1092. It's much simpler now.
	\param file filename of file to test
	\retval bool true = Scribus format file, false : not Scribus
	*/
	bool    ReadElemHeader(QString file, bool isFile, double *x, double *y, double *w, double *h);
	bool    ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, double Xp, double Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub, ScribusView *view);
	bool    ReadElemToLayer(QString fileName, SCFonts &avail, ScribusDoc *doc, double Xp, double Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub, ScribusView *view, int toLayer);
	QString WriteElem(ScribusDoc *doc, ScribusView *view, Selection *selection);

	ColorList Farben;
	StyleSet<ParagraphStyle> docParagraphStyles;
	QList<Linked> LFrames;
	QStringList MNames;
	QMap<QString,QString> DoFonts;
	QMap<QString,QString> ReplacedFonts;
	QMap<uint,QString> DoVorl;
	QList<ScFace> dummyScFaces;
	uint VorlC;
	bool newReplacement;
	
protected:
	PrefsManager* prefsManager;

	bool	attrHasValue(const QXmlStreamAttributes& attrs, const char* attName);
	bool	attrAsBool(const QXmlStreamAttributes& attrs, const char* attName, bool defVal = false);
	int     attrAsInt (const QXmlStreamAttributes& attrs, const char* attName, int  defVal = 0);
	double  attrAsDbl (const QXmlStreamAttributes& attrs, const char* attName, double defVal = 0.0);
	QString attrAsString (const QXmlStreamAttributes& attrs, const char* attName, const QString& defVal);

	void GetItemProps(const QXmlStreamAttributes& attrs, struct CopyPasteBuffer *OB, const QString& baseDir, bool newVersion);
	void GetItemText (const QXmlStreamAttributes& attrs, StoryText& story, ScribusDoc *doc, LastStyles* last, bool VorLFound, bool impo);
	bool compareStyles(ParagraphStyle &vg, const ParagraphStyle &vg2);
	void GetStyle(QXmlStreamReader& reader, ParagraphStyle &vg, StyleSet<ParagraphStyle>* tempStyles, ScribusDoc* doc, bool fl);

	void ReadPattern(QXmlStreamReader& reader, ScribusDoc* doc, ScribusView *view, const QString& fileName, int& GrMax, bool styleFound, bool newVersion);
	void ReadLegacyCStyle (const QXmlStreamAttributes& attrs, CharStyle& style, ScribusDoc* doc);
	void ReadCStyle (const QXmlStreamAttributes& attrs, CharStyle& style, ScribusDoc* doc);
	void ReadPStyle (QXmlStreamReader& reader, ParagraphStyle &style, ScribusDoc* doc);

	void SetItemProps(ScXmlStreamWriter& writer, ScribusDoc *doc, PageItem* item, const QString& baseDir, bool newFormat);
	void WriteObject (ScXmlStreamWriter& writer, ScribusDoc *doc, PageItem *item, const QString& baseDir, QMap<int, int> &UsedMapped2Saved);
	void WriteITEXTs (ScXmlStreamWriter& writer, ScribusDoc *doc, PageItem *item);
	void WriteLegacyCStyle (ScXmlStreamWriter& writer, const CharStyle& style);
	void WriteCStyle (ScXmlStreamWriter& writer, const CharStyle& style);
	void WritePStyle (ScXmlStreamWriter& writer, const ParagraphStyle& style, const QString& nodeName);
	
	QString AskForFont(SCFonts &avail, QString fStr, ScribusDoc *doc);
};

#endif // _SCRIBUS_CONFIG_




