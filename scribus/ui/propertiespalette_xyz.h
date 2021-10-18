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
#include "units.h"

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
	
	/** @brief Returns true if there is a user action going on at the moment of call. */
	bool userActionOn(); // not yet implemented!!! This is needed badly.
                         // When user releases the mouse button or arrow key, changes must be checked
                         // and if in ScribusView a groupTransaction has been started it must be also
                         // committed

	void setLineMode(int lineMode);

private:
	PageItem* currentItemFromSelection();
	
public slots:
	void setMainWindow(ScribusMainWindow *mw);
	
	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void iconSetChange();
	void languageChange();
	void unitChange();
	void localeChange();

	void showXY(double x, double y);
	void showWH(double x, double y);
	void showRotation(double r);
	void showLocked(bool);
	void showSizeLocked(bool);
	void showPrintingEnabled(bool);
	void showFlippedH(bool);
	void showFlippedV(bool);

	void handleAppModeChanged(int oldMode, int mode);
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

protected:
	bool m_lineMode {false};
	bool m_haveDoc {false};
	bool m_haveItem {false};
	double m_unitRatio {1.0};
	int m_unitIndex {SC_PT};
	PageItem *m_item {nullptr};
	ScribusMainWindow* m_ScMW {nullptr};
	ScGuardedPtr<ScribusDoc> m_doc;
	
	double m_oldRotation {0.0};

	bool m_userActionOn { false };
	UserActionSniffer *userActionSniffer;
	void installSniffer(ScrSpinBox *spinBox);

	void changeEvent(QEvent *e) override;
};

#endif
