/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PROPERTYWIDGET_PATHTEXT_H
#define PROPERTYWIDGET_PATHTEXT_H

#include "ui_propertywidget_pathtextbase.h"

#include "scguardedptr.h"

class ScribusDoc;

class PropertyWidget_PathText : public QFrame, public Ui::PropertyWidget_PathTextBase
{
	Q_OBJECT

public:
	PropertyWidget_PathText(QWidget* parent);
	~PropertyWidget_PathText() {};

	virtual void changeEvent(QEvent *e);

protected:

	double m_unitRatio;
	int    m_unitIndex;

	ScGuardedPtr<ScribusDoc> m_doc;

public slots:

	void setDoc(ScribusDoc *d);

	void languageChange();
	void unitChange();
};

#endif
