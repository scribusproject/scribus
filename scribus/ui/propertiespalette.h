/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_H
#define PROPERTIESPALETTE_H

#include <QListWidgetItem>
#include <QLineEdit>

class QCloseEvent;
class QEvent;
class QFocusEvent;
class QToolBox;
class QVBoxLayout;
class QWidget;

#include "scribusapi.h"
#include "scdockpalette.h"
#include "scguardedptr.h"
#include "scrspinbox.h"
#include "pageitem.h"
#include "page.h"
#include "linkbutton.h"
#include "linecombo.h"
#include "spalette.h"
#include "sclistboxpixmap.h"
#include "scguardedptr.h"
#include "sctreewidget.h"

class ColorCombo;
class Cpalette;
class NameWidget;
class PropertiesPalette_Group;
class PropertiesPalette_Image;
class PropertiesPalette_Line;
class PropertiesPalette_Shape;
class PropertiesPalette_Text;
class PropertiesPalette_XYZ;
class ScComboBox;
class ScribusDoc;
class ScribusMainWindow;
class Selection;
class Tpalette;

class SCRIBUS_API PropertiesPalette : public ScDockPalette
{
	Q_OBJECT

public:
	PropertiesPalette(QWidget* parent);
	~PropertiesPalette() {};

	virtual void changeEvent(QEvent *e);
	virtual void closeEvent(QCloseEvent *closeEvent);
	
	void updateColorSpecialGradient();
	const VGradient getMaskGradient();
	const VGradient getMaskGradientGroup();
	void updateColorList();
	void setGradientEditMode(bool);
	void setTextFlowMode(PageItem::TextFlowMode mode);

	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // commmited


	PropertiesPalette_Group* groupPal;
	PropertiesPalette_Image* imagePal;
	PropertiesPalette_Line*  linePal;
	PropertiesPalette_Shape* shapePal;
	PropertiesPalette_Text*  textPal;
	PropertiesPalette_XYZ*   xyzPal;
	Cpalette *Cpal;
	Tpalette *Tpal;

private:

	PageItem* currentItemFromSelection();
	
public slots:
	void setMainWindow(ScribusMainWindow *mw);
	
	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *i);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	
	void unitChange();
	void languageChange();

	void setLocked(bool);

private slots:
	void SelTab(int t);

	void NewLineMode(int mode);
	void NewAlignement(int a);
	void handleNewShape(int frameType);
	void NewSpGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk, double cx, double cy);
	void toggleGradientEdit(int);
	void NewSpGradientM(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk);
	void toggleGradientEditM();
	void handleShapeEdit();

signals:
	void DocChanged();

protected:
	ScribusMainWindow *m_ScMW;

	QVBoxLayout* MpalLayout;
	QVBoxLayout* pageLayout;
	QVBoxLayout* pageLayout_2;
	QVBoxLayout* pageLayout_3;
	QVBoxLayout* pageLayout_4;
	QVBoxLayout* pageLayout_5;
	QVBoxLayout* pageLayout_6;
	QVBoxLayout* pageLayout_7;

	QVBoxLayout* page_group_layout;
	
	QWidget* page;
	QWidget* page_2;
	QWidget* page_3;
	QWidget* page_4;
	QWidget* page_5;
	QWidget* page_6;
	QWidget* page_7;
	QWidget* page_group;

	QToolBox* TabStack;

	bool      m_haveDoc;
	bool      m_haveItem;
	double    m_unitRatio;
	int       m_unitIndex;
	PageItem* m_item;
	
	ScGuardedPtr<ScribusDoc> m_doc;
	
	
	int idXYZItem;
	int idShapeItem;
	int idTextItem;
	int idImageItem;
	int idLineItem;
	int idColorsItem;
	int idTransparencyItem;
	int idGroupItem;
};

#endif
