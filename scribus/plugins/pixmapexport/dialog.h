/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EXPORTFORM_H
#define EXPORTFORM_H

#include "ui_exportform.h"

class ScribusDoc;
class PrefsContext;

class ExportForm : public QDialog, public Ui::ExportForm
{
	Q_OBJECT

public:
	ExportForm( QWidget* parent, ScribusDoc* doc, int size, int quality, QString type);
	~ExportForm();

public slots:
	virtual void OutputDirectoryButton_pressed();
	virtual void IntervalPagesRadio_stateChanged();
	virtual void AllPagesRadio_stateChanged();
	virtual void OnePageRadio_stateChanged();
	virtual void computeSize();

protected:
	PrefsContext* prefs;
	ScribusDoc*  m_doc;
	int m_PageCount;
	
protected slots:
	virtual void createPageNumberRange();
	virtual void languageChange();
	virtual void readConfig();
	virtual void writeConfig();
};

#endif // EXPORTFORM_H
