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
	bool IsScribus(QString fileName);
	QString ReadDatei(QString fileName);
	QString AskForFont(SCFonts &avail, QString fStr, ScribusDoc *doc);
	void GetItemText(QDomElement *it, ScribusDoc *doc, bool VorLFound, bool impo, PageItem* obj = 0);
	void SetItemProps(QDomElement *ob, PageItem* item, bool newFormat);
	bool ReadLStyles(QString fileName, QMap<QString,multiLine> *Sty);
	void GetStyle(QDomElement *pg, struct ParagraphStyle *vg, QValueList<ParagraphStyle> &docParagraphStyles, ScribusDoc* doc, bool fl);
	bool ReadStyles(QString fileName, ScribusDoc* doc);
	bool ReadColors(QString fileName);
	bool ReadPageCount(QString fileName, int *num1, int *num2);
	bool ReadPage(QString fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, int PageToLoad, bool Mpage);
	//QString WriteElem(QPtrList<PageItem> *Selitems, ScribusDoc *doc, ScribusView *view);
	//QString WriteElem(ScribusDoc *doc, ScribusView *view, int selectionListNumber);
	QString WriteElem(ScribusDoc *doc, ScribusView *view, Selection *selection);
	bool ReadElemHeader(QString file, bool isFile, double *x, double *y, double *w, double *h);
	bool ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, double Xp, double Yp, bool Fi, bool loc,
					 QMap<QString,QString> &FontSub, ScribusView *view);
	bool ReadDoc(QString fileName, SCFonts &avail, ScribusDoc *docu, ScribusView *viewx, QProgressBar *dia2);
	void WritePages(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, bool master);
	void WriteObjects(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, int master);
	bool WriteDoc(QString fileName, ScribusDoc *docu, QProgressBar *dia2);
	ColorList Farben;
	QValueList<ParagraphStyle> docParagraphStyles;
	struct Linked 
	{ 
		int Start;
		int StPag;
	};
	QValueList<Linked> LFrames;
	QStringList MNames;
	QMap<QString,QString> DoFonts;
	QMap<QString,QString> ReplacedFonts;
	QMap<uint,QString> DoVorl;
	QPtrList<Foi> dummyFois;
	uint VorlC;
	bool newReplacement;
	
signals:
//	void NewPage(int);
	
protected:
	PrefsManager* prefsManager;
};

#endif // _SCRIBUS_CONFIG_

