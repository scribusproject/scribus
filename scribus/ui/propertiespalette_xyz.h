/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTIESPALETTE_XYZ_H
#define PROPERTIESPALETTE_XYZ_H

#include "ui_propertiespalette_xyzbase.h"

#include "scribusapi.h"
#include "scrspinbox.h"
#include "linkbutton.h"
#include "sclistboxpixmap.h"
#include "scguardedptr.h"

class BasePointWidget;
class NameWidget;
class PageItem;
class ScribusDoc;
class Selection;
class ScribusMainWindow;
class UserActionSniffer;

class SCRIBUS_API PropertiesPalette_XYZ : public QWidget, public Ui::PropertiesPalette_XYZBase
{
	Q_OBJECT

public:
	PropertiesPalette_XYZ(QWidget* parent);
	~PropertiesPalette_XYZ() {};

	virtual void changeEvent(QEvent *e);
	
	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // commmited

	void setLineMode(int lineMode);

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

	void displayLevel(uint l);
	void displayXY(double x, double y);
	void displayWH(double x, double y);
	void displayRotation(double r);
	void displayLocked(bool);
	void displaySizeLocked(bool);
	void displayPrintingEnabled(bool);
	void displayFlippedH(bool);
	void displayFlippedV(bool);

	void handleSelectionChanged();

private slots:
	void handleNewX();
	void handleNewY();
	void handleNewW();
	void handleNewH();
	void handleRotation();
	void handleFlipH();
	void handleFlipV();
	void handleLower();
	void handleRaise();
	void handleFront();
	void handleBack();
	void handleBasePoint(int m);
	void handleLock();
	void handleLockSize();
	void handlePrint();
	void handleNewName();
	void handleGrouping();
	void handleUngrouping();

protected slots:
	//virtual void reject();
	void spinboxStartUserAction();
	void spinboxFinishUserAction();
	void updateSpinBoxConstants();

signals:
	void DocChanged();

protected:
	ScribusMainWindow *m_ScMW;

	bool      m_haveDoc;
	bool      m_haveItem;
	bool      m_lineMode;
	double    m_unitRatio;
	int       m_unitIndex;
	PageItem *m_item;
	ScGuardedPtr<ScribusDoc> m_doc;
	
	double    m_oldRotation;

	bool _userActionOn;
	UserActionSniffer *userActionSniffer;
	void installSniffer(ScrSpinBox *spinBox);
};

#endif
