/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef QUERY_H
#define QUERY_H

#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
class QLabel;
class QLineEdit;
class QPushButton;
class QString;

#include "scribusapi.h"

class SCRIBUS_API Query : public QDialog
{
	Q_OBJECT

public:
	Query( QWidget* parent=0, const char* name=0, bool modal = FALSE, Qt::WFlags fl=0, QString text=0,
	       QString titel=0 );
	~Query() {};

	const QString getEditText();
	void setEditText(QString newText, bool setSelected);

public slots:
	void Leave();

private:
	QVBoxLayout* queryLayout;
	QHBoxLayout* editLayout;
	QHBoxLayout* okCancelLayout;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QLineEdit* answerEdit;
	QLabel* questionLabel;
};

#endif // QUERY_H
