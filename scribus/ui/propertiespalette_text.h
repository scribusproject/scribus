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
#include "directionselect.h"
#include "shadebutton.h"
#include "sclistboxpixmap.h"
#include "scguardedptr.h"
#include "sctextstruct.h"
#include "sctreewidget.h"

class PageItem;
class PropertyWidget_Advanced;
class PropertyWidget_Distance;
class PropertyWidget_ParEffect;
class PropertyWidget_Flop;
class PropertyWidget_FontFeatures;
class PropertyWidget_Hyphenation;
class PropertyWidget_OptMargins;
class PropertyWidget_Orphans;
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
	void changeLang(int id);

	void showAlignment(int e);
	void showDirection(int e);
	void showCharStyle(const QString& name);
	void showFontFace(const QString&);
	void showFontSize(double s);
	void showLanguage(QString w);
	void showFirstLinePolicy(FirstLineOffsetPolicy);
	void showLineSpacing(double r);
	void showParStyle(const QString& name);
	
	void setupLineSpacingSpinbox(int mode, double value);
	
	/// update TB values:
	void updateCharStyle(const CharStyle& charStyle);
	void updateStyle(const ParagraphStyle& newCurrent);
	
	void updateCharStyles();
	void updateParagraphStyles();
	void updateTextStyles();
	
	void handleLineSpacingMode(int id);
	void handleTextFont(QString);

private slots:
	void handleAlignment(int a);
	void handleDirection(int d);
	void handleFirstLinePolicy(int);
	void handleFontSize();
	void handleLineSpacing();
	
	void doClearCStyle();
	void doClearPStyle();

protected:
	PropertyWidget_Advanced* advancedWidgets;
	PropertyWidget_Distance* distanceWidgets;
	PropertyWidget_Flop* flopBox;
	PropertyWidget_FontFeatures* fontfeaturesWidget;
	PropertyWidget_Hyphenation* hyphenationWidget;
	PropertyWidget_OptMargins* optMargins;
	PropertyWidget_Orphans* orphanBox;
	PropertyWidget_ParEffect* parEffectWidgets;
	PropertyWidget_PathText* pathTextWidgets;
	PropertyWidget_TextColor* colorWidgets;
//	QTreeWidgetItem* advancedWidgetsItem;
//	QTreeWidgetItem* colorWidgetsItem;
//	QTreeWidgetItem* distanceItem;
//	QTreeWidgetItem* flopItem;
//	QTreeWidgetItem* fontfeaturesWidgetItem;
//	QTreeWidgetItem* hyphenationWidgetItem;
//	QTreeWidgetItem* optMarginsItem;
//	QTreeWidgetItem* orphanItem;
//	QTreeWidgetItem* parEffectItem;
//	QTreeWidgetItem* pathTextItem;
	int advancedWidgetsItem;
	int colorWidgetsItem;
	int distanceItem;
	int flopItem;
	int fontfeaturesWidgetItem;
	int hyphenationWidgetItem;
	int optMarginsItem;
	int orphanItem;
	int parEffectItem;
	int pathTextItem;

};

#endif
