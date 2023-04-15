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

class ColorCombo;
class Navigator;
class PageItem;
class PrefsContext;
class ScribusDoc;
class ScribusView;

class SCRIBUS_API ScAnnot : public QDialog, Ui::ScAnnot
{
	Q_OBJECT

public:
	ScAnnot(QWidget* parent, PageItem *it, ScribusDoc* doc, ScribusView* view);
	~ScAnnot() = default;

	void DecodeNum();
	void DecodeVali();
	void DecodeCalc();

	int Width;
	int Height;
	int OriWidth;
	int OriHeight;
	int ScrEdited;
	int FormNum;
	QString JavaScr;
	int MaxPages;
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
	Navigator* m_navig { nullptr };
	PageItem* m_item { nullptr };
	ScribusDoc* m_doc { nullptr };
	ScribusView* m_view { nullptr };

	PrefsContext* m_prefsCtxt{ nullptr };
	Annotation m_annotation;
};

#endif // ANNOT_H
