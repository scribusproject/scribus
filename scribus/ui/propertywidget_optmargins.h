/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGET_OPTMARGINS_H
#define PROPERTYWIDGET_OPTMARGINS_H

#include "ui_propertywidget_optmarginsbase.h"

#include "scguardedptr.h"

class ScribusDoc;

class PropertyWidget_OptMargins : public QFrame, public Ui::PropertyWidget_OptMarginsBase
{
	Q_OBJECT

public:
	PropertyWidget_OptMargins(QWidget* parent);
	~PropertyWidget_OptMargins() {};

	virtual void changeEvent(QEvent *e);

protected:

	ScGuardedPtr<ScribusDoc> m_doc;

public slots:

	void setDoc(ScribusDoc *d);

	void languageChange();
	void unitChange() {};
};

#endif
