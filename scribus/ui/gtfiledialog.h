/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef GTFILEDIALOG_H
#define GTFILEDIALOG_H

#include <QDialog>
#include "ui_gtfiledialog.h"
#include "scribusapi.h"


/*! \brief Enhanced file dialog for get-text plugins
This class is separated from gtdialogs.h file due the huge
(almost) circle dependencies with fonts subdir/library
\author Petr Vanek <petr@scribus.info>
\author Riku Leino
*/
class SCRIBUS_API gtFileDialog : public QDialog, public Ui::gtFileDialog
{
	Q_OBJECT
public:
	gtFileDialog(const QString& filters, const QStringList& importers, const QString& wdir);
	~gtFileDialog();

	QString selectedFile();

public slots:
	virtual void accept();

protected:
	void loadSettings(void);
	void saveSettings(void);
};

#endif
