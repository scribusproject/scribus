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
	ExportForm( QWidget* parent, ScribusDoc* doc, int size, int quality, const QString& type);
	~ExportForm();

public slots:
	void OutputDirectoryButton_pressed();
	void IntervalPagesRadio_stateChanged();
	void AllPagesRadio_stateChanged();
	void OnePageRadio_stateChanged();
	void computeSize();

protected:
	PrefsContext* prefs {nullptr};
	ScribusDoc* m_doc {nullptr};
	int m_PageCount {1};

protected slots:
	void createPageNumberRange();
	void languageChange();
	void readConfig();
	void writeConfig();
};

#endif // EXPORTFORM_H
