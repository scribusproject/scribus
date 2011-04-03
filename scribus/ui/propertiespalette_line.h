/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_LINE_H
#define PROPERTIESPALETTE_LINE_H

#include "ui_propertiespalette_linebase.h"

#include "scribusapi.h"
#include "linecombo.h"
#include "scguardedptr.h"

class ArrowChooser;
class DashEditor;
class PageItem;
class ScComboBox;
class ScribusDoc;
class ScribusMainWindow;

class SCRIBUS_API PropertiesPalette_Line : public QWidget, Ui::PropertiesPalette_LineBase
{
	Q_OBJECT

public:
	PropertiesPalette_Line(QWidget* parent);
	~PropertiesPalette_Line() {};

	virtual void changeEvent(QEvent *e);

	void updateLineStyles();
	void updateArrowStyles();

protected:

	bool      m_haveDoc;
	bool      m_haveItem;
	bool      m_lineMode;
	double    m_unitRatio;
	int       m_unitIndex;

	PageItem *m_item;
	ScribusMainWindow*       m_ScMW;
	ScGuardedPtr<ScribusDoc> m_doc;

private:

	PageItem* currentItemFromSelection();
	void      updateArrowStyles(ScribusDoc *dd);
	void      updateLineStyles(ScribusDoc *dd);

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *i);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void displayLineWidth(double s);
	void displayLineValues(Qt::PenStyle p, Qt::PenCapStyle pc, Qt::PenJoinStyle pj);

	void languageChange();
	void unitChange();

private slots:

	void handleLineWidth();
	void handleLineStyle();
	void handleLineJoin();
	void handleLineEnd();
	void handleLineMode();
	void handleDashChange();
	void handleTLines();
	void handleStartArrow(int id);
	void handleEndArrow(int id);
	void handleStartArrowScale(int sc);
	void handleEndArrowScale(int sc);
	void handleLineStyle(QListWidgetItem *c);

signals:
	void DocChanged();
	void lineModeChanged(int);
};

#endif
