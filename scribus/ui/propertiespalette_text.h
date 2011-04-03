/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_TEXT_H
#define PROPERTIESPALETTE_TEXT_H

#include "ui_propertiespalette_textbase.h"

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scrspinbox.h"

#include "linecombo.h"
#include "spalette.h"
#include "alignselect.h"
#include "shadebutton.h"
#include "sclistboxpixmap.h"
#include "scguardedptr.h"
#include "sctextstruct.h"
#include "sctreewidget.h"

class PageItem;
class PropertyWidget_Advanced;
class PropertyWidget_Distance;
class PropertyWidget_Flop;
class PropertyWidget_OptMargins;
class PropertyWidget_PathText;
class PropertyWidget_TextColor;
class ScComboBox;
class ScribusDoc;
class ScribusMainWindow;
class Selection;

class SCRIBUS_API PropertiesPalette_Text : public QWidget, public Ui::PropertiesPalette_TextBase
{
	Q_OBJECT

public:
	PropertiesPalette_Text(QWidget* parent);
	~PropertiesPalette_Text() {};

	virtual void changeEvent(QEvent *e);
	
	void updateColorList();
	/*! \brief fills the langs combobox in language specific order
	\author 10/07/2004 - Petr Vanek - rewritten to fix #1185.
	Uses sortQStringList from utils.cpp - STL!
	\param langMap a structure with languages/hyphs*/
	void fillLangCombo(QMap<QString,QString> langMap);
	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // commmited

protected:

	bool   m_haveDoc;
	bool   m_haveItem;
	double m_unitRatio;
	int    m_unitIndex;

	PageItem *m_item;
	ScribusMainWindow*       m_ScMW;
	ScGuardedPtr<ScribusDoc> m_doc;

private:
	PageItem* currentItemFromSelection();
	
public slots:
	void setMainWindow(ScribusMainWindow *mw);
	
	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *i);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void languageChange();
	void unitChange();

	void displayAlignment(int e);
	void displayCharStyle(const QString& name);
	void displayColumns(int r, double g);
	void displayFontFace(const QString&);
	void displayFontSize(double s);
	void displayFirstLinePolicy(FirstLineOffsetPolicy);
	void displayLineSpacing(double r);
	void displayOpticalMargins(const ParagraphStyle& pStyle);
	void displayParStyle(const QString& name);
	void displayTextDistances(double left, double top, double bottom, double right);
	
	void setupLineSpacingSpinbox(int mode, double value);
	
	/// update TB values:
	void updateCharStyle(const CharStyle& charStyle);
	void updateStyle(const ParagraphStyle& newCurrent);
	
	void updateCharStyles();
	void updateParagraphStyles();
	void updateTextStyles();
	
	void handleLineSpacingMode(int id);
	void handleOpticalMargins();
	void resetOpticalMargins();
	void handleTabs();
	void handleTextFont(QString);

private slots:
	void handleAlignement(int a);
	void handleColumns();
	void handleColumnGap();
	void handleFirstLinePolicy(int);
	void handleFontSize();
	void handleGapSwitch();
	void handleHyphLanguage();
	void handleLineSpacing();
	void handleTextDistances();
	
	void doClearCStyle();
	void doClearPStyle();

	void handlePathType();
	void handlePathFlip();
	void handlePathLine();
	void handlePathDist();
	void handlePathOffs();

protected:

	ScComboBox* pathTextType;
	QComboBox* langCombo;

	PropertyWidget_TextColor* colorWidgets;
	QTreeWidgetItem* colorWidgetsItem;

	PropertyWidget_Distance* distanceWidgets;
	QTreeWidgetItem* distanceItem;

	PropertyWidget_Advanced* advancedWidgets;
	QTreeWidgetItem* advancedWidgetsItem;

	PropertyWidget_OptMargins* optMargins;
	QTreeWidgetItem* optMarginsItem;

	PropertyWidget_Flop* flopBox;
	QTreeWidgetItem* flopItem;
	
	PropertyWidget_PathText* pathTextWidgets;
	QTreeWidgetItem* pathTextItem;

signals:
	void DocChanged();
	void NewAlignment(int);
	void NewFont(const QString&);
};

#endif
