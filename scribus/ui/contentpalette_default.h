/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CONTENTPALETTE_DEFAULTBASE_H
#define CONTENTPALETTE_DEFAULTBASE_H

#include "ui_contentpalette_defaultbase.h"

#include "scribusapi.h"
#include "pageitem.h"
#include "scguardedptr.h"
#include "scribusdoc.h"

class ScribusDoc;
class ScribusMainWindow;
class Selection;

class SCRIBUS_API ContentPalette_Default : public QWidget, Ui::ContentPalette_DefaultBase
{
	Q_OBJECT

public:
	ContentPalette_Default(QWidget* parent);
	~ContentPalette_Default() {};

	// void showTextFlowMode(PageItem::TextFlowMode mode);

	// void updateColorList();
	// void updateColorSpecialGradient();

protected:

	bool      m_haveDoc = false;
	bool      m_haveItem = false;
	double    m_unitRatio = 1.0;
	int       m_unitIndex = 0;

	ScribusMainWindow*       m_ScMW = nullptr;
	ScGuardedPtr<ScribusDoc> m_doc;

	void changeEvent(QEvent *e) override;
	void setLabelText();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void languageChange();
	void unitChange();
};

#endif
