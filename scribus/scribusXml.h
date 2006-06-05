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

#ifndef _SCRIBUS_CONFIG_
#define _SCRIBUS_CONFIG_

#include "scribusapi.h"
#include <qvaluelist.h>
#include <qdom.h>
#include <qprogressbar.h>

#include "pageitem.h"
#include "multiline.h"
#include "scfonts.h"
#include "scribusstructs.h"
#include "selection.h"

class PrefsManager;
class ScribusView;
class SCFonts;
class ScribusDoc;

class SCRIBUS_API ScriXmlDoc : public QObject
{
Q_OBJECT
public:
	ScriXmlDoc();
	~ScriXmlDoc() {};
	/*!
	\author Frederic Dubuy <effediwhy@gmail.com>, Petr Vanek
	\date august 17th 2004, 10/03/2004
	\brief Preliminary Scribus file validator. totally rewritten when fixing crash bug #1092. It's much simpler now.
	\param fileName filename of file to test
	\retval bool true = Scribus format file, false : not Scribus
	*/
	QString ReadDatei(QString fileName);
	void GetItemText(QDomElement *it, ScribusDoc *doc, bool VorLFound, bool impo, PageItem* obj = 0);
	void SetItemProps(QDomElement *ob, PageItem* item, bool newFormat);
	QString WriteElem(ScribusDoc *doc, ScribusView *view, Selection *selection);
	bool ReadElemHeader(QString file, bool isFile, double *x, double *y, double *w, double *h);
	bool ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, double Xp, double Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub, ScribusView *view);
	ColorList Farben;
	QValueList<ParagraphStyle> docParagraphStyles;
	QValueList<Linked> LFrames;
	QStringList MNames;
	QMap<QString,QString> DoFonts;
	QMap<QString,QString> ReplacedFonts;
	QMap<uint,QString> DoVorl;
	QPtrList<Foi> dummyFois;
	uint VorlC;
	bool newReplacement;
	
protected:
	PrefsManager* prefsManager;
	
	void GetStyle(QDomElement *pg, ParagraphStyle *vg, QValueList<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl);
	QString AskForFont(SCFonts &avail, QString fStr, ScribusDoc *doc);
};

#endif // _SCRIBUS_CONFIG_

