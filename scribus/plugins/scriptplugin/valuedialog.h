/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef VALUEDIALOG_H
#define VALUEDIALOG_H

#include <qvariant.h>
#include <qdialog.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <QLabel>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QLabel;
class QLineEdit;
class QPushButton;

/** Request for response from user - lightweight modal dialog 
09/24/2004 petr vanek
*/
class ValueDialog : public QDialog
{
	Q_OBJECT

public:
	ValueDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, Qt::WFlags fl = 0 );
	~ValueDialog();

	QLabel* dialogLabel;
	QLineEdit* valueEdit;
	QPushButton* okButton;

public slots:
	virtual void okButton_pressed();

protected:
	Q3GridLayout* ScripterValueLayout;
	Q3VBoxLayout* layout3;
	Q3VBoxLayout* layout2;
	Q3HBoxLayout* layout1;

protected slots:
	virtual void languageChange();

};

#endif // VALUEDIALOG_H
