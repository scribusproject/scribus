/***************************************************************************
                 	scribusXml.h the document xml library for scribus
                             -------------------
    begin                : Sam Jul 14 10:00:00 CEST 2001
    copyright            : (C) 2001 by Christian T�p
    email                : christian.toepp@mr-ct@gmx.de
 ***************************************************************************/

#ifndef _SCRIBUS_CONFIG_
#define _SCRIBUS_CONFIG_

#include "scribusview.h"
#include <qvaluelist.h>
#include <qdom.h>
#include <qprogressbar.h>

class ScriXmlDoc : public QObject
{
Q_OBJECT
public:
	ScriXmlDoc() {};
	~ScriXmlDoc() {};
	bool IsScribus(QString fileName);
	QString ReadDatei(QString fileName);
	QString AskForFont(SCFonts &avail, QString fStr, preV *Prefs, ScribusDoc *doc);
	QString GetItemText(QDomElement *it, ScribusDoc *doc, preV *Prefs, bool VorLFound, bool impo);
	void SetItemProps(QDomElement *ob, PageItem* item, bool newFormat);
	bool ReadLStyles(QString fileName, QMap<QString,multiLine> *Sty);
	void GetStyle(QDomElement *pg, struct StVorL *vg, QValueList<StVorL> &Vorlagen, ScribusDoc* doc, preV *Prefs, bool fl);
	bool ReadStyles(QString fileName, ScribusDoc* doc, preV *Prefs);
	bool ReadColors(QString fileName);
	bool ReadPageCount(QString fileName, int *num1, int *num2);
	bool ReadPage(QString fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, int PageToLoad, bool Mpage);
	QString WriteElem(QPtrList<PageItem> *Selitems, ScribusDoc *doc, ScribusView *view);
	bool ReadElemHeader(QString file, bool isFile, double *x, double *y, double *w, double *h);
	bool ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, int Xp, int Yp, bool Fi, bool loc,
					 QMap<QString,QString> &FontSub, preV *Prefs, ScribusView *view);
	bool ReadDoc(QString fileName, SCFonts &avail, ScribusDoc *docu, ScribusView *viewx, QProgressBar *dia2);
	void WritePages(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, bool master);
	void WriteObjects(ScribusDoc *doc, QDomDocument *docu, QDomElement *dc, QProgressBar *dia2, uint maxC, bool master);
	bool WriteDoc(QString fileName, ScribusDoc *docu, QProgressBar *dia2);
	void WritePref(preV *Vor, QString ho);
	bool ReadPref(struct preV *Vorein, QString ho);
	CListe Farben;
	QValueList<StVorL> Vorlagen;
	struct Linked { 
					int Start;
					int StPag;
              	};
	QValueList<Linked> LFrames;
	QStringList MNames;
	QMap<QString,QString> DoFonts;
	QMap<uint,QString> DoVorl;
	uint VorlC;
	
signals:
	void NewPage(int);
};

#endif // _SCRIBUS_CONFIG_
