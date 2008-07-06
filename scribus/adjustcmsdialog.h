/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ADJUSTCMSDIALOG_H
#define ADJUSTCMSDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
class QPushButton;
class ScribusDoc;

#include "scribusapi.h"
#include "cmsprefs.h"

class SCRIBUS_API AdjustCmsDialog : public QDialog
{
	Q_OBJECT

public:
	AdjustCmsDialog(QWidget* parent, ScribusDoc* doc);
	~AdjustCmsDialog() {};
	CMSPrefs* tabColorManagement;

private:
	QVBoxLayout* queryLayout;
	QHBoxLayout* okCancelLayout;
	QPushButton* okButton;
	QPushButton* cancelButton;
};

#endif
