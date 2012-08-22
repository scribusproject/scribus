/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGET_FLOP_H
#define PROPERTYWIDGET_FLOP_H

#include "ui_propertywidget_flopbase.h"

#include "scguardedptr.h"

class ScribusDoc;

class PropertyWidget_Flop : public QFrame, public Ui::PropertyWidget_FlopBase
{
	Q_OBJECT

public:

	enum FlopButtonID
    {
        RealHeightID   = 0,
        FontAscentID   = 1,
        LineSpacingID  = 2,
		BaselineGridID = 3
    };

	PropertyWidget_Flop(QWidget* parent);
	~PropertyWidget_Flop() {};

protected:

	ScGuardedPtr<ScribusDoc> m_doc;

	virtual void changeEvent(QEvent *e);

public slots:

	void setDoc(ScribusDoc *d);

	void languageChange();
	void unitChange() {};
};

#endif
