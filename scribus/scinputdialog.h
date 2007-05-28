/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SCINPUTDIALOG_H
#define SCINPUTDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class Q3HBoxLayout;
class Q3VBoxLayout;
class Q3GridLayout;
class ScrSpinBox;
class QLabel;
class QPushButton;


/*! \brief A QInputDialog clone with ScrSpinBox widget.
\author Petr Vanek <petr@scribus.info>
*/
class ScInputDialog : public QDialog
{
	Q_OBJECT

public:
	ScInputDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	~ScInputDialog(){};

	QLabel* entryLabel;
	ScrSpinBox* entrySpin;
	QPushButton* buttonOk;
	QPushButton* buttonCancel;

	static double getDouble( const QString &caption, const QString &label, double num = 0,
							 double from = -2147483647, double to = 2147483647,
							 int decimals = 1, QString suffix = "", bool *ok = 0,
							 QWidget *parent = 0, const char *name = 0 );

protected:
	Q3GridLayout* ScInputDialogLayout;
	Q3VBoxLayout* mainLayout;
	Q3HBoxLayout* labelEntryLayout;
	Q3HBoxLayout* buttonLayout;

protected slots:
	virtual void languageChange();
};

#endif // SCINPUTDIALOG_H
