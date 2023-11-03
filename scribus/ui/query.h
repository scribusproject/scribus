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
	Query(QWidget* parent = nullptr, const char* name = nullptr, bool modal = false, const QString& text = 0, const QString& title = 0);
	~Query() {};

	QString getEditText() const;
	void setEditText(const QString& newText, bool setSelected);
	void setTestList(const QStringList& tList);
	void setForbiddenList(const QStringList& tList);
	void setCheckMode(bool mode);
	void setValidator(const QRegExp& rx);

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
	QStringList checkList;
	QStringList forbiddenList;
	bool checkMode;
};

#endif // QUERY_H
