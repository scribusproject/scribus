/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef QUERY2_H
#define QUERY2_H

#include <QDialog>
class QHBoxLayout;
class QVBoxLayout;
class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;

#include "scribusapi.h"
class ScribusDoc;

class SCRIBUS_API NewTm : public QDialog
{
	Q_OBJECT

public:
	NewTm( QWidget* parent, QString text, QString titel, ScribusDoc *doc, const QString& answerText=QString::null );
	~NewTm() {};

	QPushButton* PushButton1;
	QPushButton* PushButton2;
	QLineEdit* Answer;
	QLabel* Frage;
	QComboBox* Links;

protected:
	QVBoxLayout* QueryLayout;
	QHBoxLayout* Layout3;
	QHBoxLayout* Layout2;
	QHBoxLayout* Layout1;
};

#endif // QUERY_H
