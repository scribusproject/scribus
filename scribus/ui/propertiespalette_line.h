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
#include "units.h"

class ArrowChooser;
class DashEditor;
class PageItem;
class ScribusDoc;
class ScribusMainWindow;

class SCRIBUS_API PropertiesPalette_Line : public QWidget, Ui::PropertiesPalette_LineBase
{
	Q_OBJECT

public:
	PropertiesPalette_Line(QWidget* parent);
	~PropertiesPalette_Line() {delete lineStyles->itemDelegate();};

	virtual void changeEvent(QEvent *e);

	void updateLineStyles();
	void updateArrowStyles();

protected:
	bool m_lineMode {false};
	bool m_haveDoc {false};
	bool m_haveItem {false};
	double m_unitRatio {1.0};
	int m_unitIndex {SC_PT};
	PageItem *m_item {nullptr};
	ScribusMainWindow* m_ScMW {nullptr};
	ScGuardedPtr<ScribusDoc> m_doc;

private:

	PageItem* currentItemFromSelection();
	void updateArrowStyles(ScribusDoc *doc);
	void updateLineStyles(ScribusDoc *doc);

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void showLineWidth(double s);
	void showLineValues(Qt::PenStyle p, Qt::PenCapStyle pc, Qt::PenJoinStyle pj);

	void iconSetChange();
	void languageChange();
	void unitChange();
	void localeChange();

private slots:

	void handleLineWidth();
	void handleLineStyle();
	void handleLineJoin();
	void handleLineEnd();
	void handleLineMode();
	void handleDashChange();
	void handleStartArrow(int id);
	void handleEndArrow(int id);
	void handleStartArrowScale(double sc);
	void handleEndArrowScale(double sc);
	void handleLineStyle(QListWidgetItem *c);

signals:
	void lineModeChanged(int);
};

#endif
