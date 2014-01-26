/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_SHADOW_H
#define PROPERTIESPALETTE_SHADOW_H

#include "ui_propertiespalette_shadowbase.h"

#include "scribusapi.h"
#include "scrspinbox.h"
#include "sclistboxpixmap.h"
#include "scguardedptr.h"

class PageItem;
class ScribusDoc;
class Selection;
class ScribusMainWindow;
class UserActionSniffer;

class SCRIBUS_API PropertiesPalette_Shadow : public QWidget, public Ui::PropertiesPalette_ShadowBase
{
	Q_OBJECT

public:
	PropertiesPalette_Shadow(QWidget* parent);
	~PropertiesPalette_Shadow() {};

	virtual void changeEvent(QEvent *e);

	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
						 // commmited
	void updateColorList();

private:

	PageItem* currentItemFromSelection();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *i);
	void unsetDoc();
	void unsetItem();

	void languageChange();
	void unitChange();
	void handleSelectionChanged();
	void handleUpdateRequest(int);

private slots:
	void handleNewValues();

protected slots:
	void spinboxStartUserAction();
	void spinboxFinishUserAction();
	void updateSpinBoxConstants();

protected:
	ScribusMainWindow *m_ScMW;

	bool      m_haveDoc;
	bool      m_haveItem;
	double    m_unitRatio;
	int       m_unitIndex;
	PageItem *m_item;
	ScGuardedPtr<ScribusDoc> m_doc;

	bool _userActionOn;
	UserActionSniffer *userActionSniffer;
	void installSniffer(ScrSpinBox *spinBox);
};

#endif
