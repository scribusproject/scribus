/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ANNOT_H
#define ANNOT_H

#include <QDialog>

#include "scribusapi.h"

#include "annotation.h"
#include "sccolor.h"

#include "ui_annot.h"

class PageItem;
class Navigator;
class ColorCombo;
class ScribusView;
class PrefsContext;

class SCRIBUS_API ScAnnot : public QDialog, Ui::ScAnnot
{
	Q_OBJECT

public:
	ScAnnot ( QWidget* parent, PageItem *it, int Seite, int b, int h, ColorList Farben, ScribusView* vie );
	~ScAnnot();

	void DecodeNum();
	void DecodeVali();
	void DecodeCalc();

	Navigator* Pg1;

	PageItem* item;
	ScribusView* view;
	int Width;
	int Height;
	int OriWidth;
	int OriHeight;
	int ScrEdited;
	int FormNum;
	QString JavaScr;
	int MaxSeite;
	QString OldName;

public slots:
	void NewName();
	void IPlace();
	void RemoveNIcon();
	void RemovePIcon();
	void RemoveRIcon();
	void IconsEin();
	void GetNIcon();
	void GetPIcon();
	void GetRIcon();
	void SelectFelder();
	void editKeySc();
	void editFormatSc();
	void editValidSc();
	void editCalcSc();
	void editJavaSc();
	void SetFormNum();
	void SetCurr();
	void HandleVali();
	void SetVali();
	void HandleCalc();
	void SetCalc();
	void SetFoScript ( int it );
	void SetCoords ( double x, double y );
	void SetPage ( int v );
	void SetCross();
	void SetValues();
	void SetAnnotationType ( int i );
	void GetFile();
	void SetActionType ( int it );
	void SetLimit();
	void SetActionScript ( int it );
	void setDateSample ( const QString& ds );
	void SetExternLink();

protected:
	PrefsContext* dirs;
	Annotation m_annotation;
};

#endif // ANNOT_H
