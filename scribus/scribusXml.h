/***************************************************************************
                 	scribusXml.h the document xml library for scribus
                             -------------------
    begin                : Sam Jul 14 10:00:00 CEST 2001
    copyright            : (C) 2001 by Christian Töpp
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
	QString ReadDatei(QString fileName);
	bool ReadColors(QString fileName);
	int ReadPageCount(QString fileName);
	bool ReadPage(QString fileName, SCFonts &avail, ScribusDoc *doc, ScribusView *view, int PageToLoad);
	QString WriteElem(QPtrList<PageItem> *Selitems, ScribusDoc *doc);
	bool ReadElemHeader(QString file, bool isFile, double *x, double *y, double *w, double *h);
	bool ReadElem(QString fileName, SCFonts &avail, ScribusDoc *doc, int Xp, int Yp, bool Fi, bool loc, QMap<QString,QString> &FontSub, preV *Prefs);
	bool ReadDoc(QString fileName, SCFonts &avail, ScribusDoc *docu, ScribusView *viewx, QProgressBar *dia2);
	void WritePages(ScribusView *view, QDomDocument docu, QDomElement dc, QProgressBar *dia2, uint maxC);
	bool WriteDoc(QString fileName, ScribusDoc *docu, ScribusView *viewx, QProgressBar *dia2);
	void WritePref(preV *Vor, QString ho);
	bool ReadPref(struct preV *Vorein, QString ho);
	CListe Farben;
	struct Linked { int Start;
									int StPag;
              	};
	QValueList<Linked> LFrames;
	
signals:
	void NewPage(int);
};

#endif // _SCRIBUS_CONFIG_
