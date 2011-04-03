/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_GROUP_H
#define PROPERTIESPALETTE_GROUP_H

#include "ui_propertiespalette_groupbase.h"

#include "scribusapi.h"
#include "pageitem.h"
#include "scguardedptr.h"

class ScribusDoc;
class ScribusMainWindow;
class Selection;

class SCRIBUS_API PropertiesPalette_Group : public QWidget, Ui::PropertiesPalette_GroupBase
{
	Q_OBJECT

public:
	PropertiesPalette_Group(QWidget* parent);
	~PropertiesPalette_Group() {};

	virtual void changeEvent(QEvent *e);

	const VGradient getMaskGradientGroup();

	void displayTextFlowMode(PageItem::TextFlowMode mode);

	void updateColorList();
	void updateColorSpecialGradient();

protected:

	bool      m_haveDoc;
	bool      m_haveItem;
	bool      m_userActionOn;
	double    m_unitRatio;
	int       m_unitIndex;

	PageItem *m_item;
	ScribusMainWindow*       m_ScMW;
	ScGuardedPtr<ScribusDoc> m_doc;
	Selection*               m_tmpSelection;

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

private slots:

	void handleGroupTransparency(double trans);
	void handleGroupBlending(int blend);
	void handleGroupGradMask(int typ);
	void handleGroupPatternMask(QString pattern);
	void handleGroupPatternMaskProps(double, double, double, double, double, double, double, bool, bool);

	void handleTextFlow();
	void handleNewShape(int f, int c, qreal *vals);

	void handleShapeEdit();
	void handleShapeEditEnded();

	void handleSpecialGradient(double, double, double, double, double, double, double, double );
	void handleGradientEdit();

signals:
	void DocChanged();
	void shapeChanged(int);
	void shapeEditStarted();
};

#endif
