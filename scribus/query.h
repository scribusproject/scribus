/****************************************************************************
** Form interface generated from reading ui file 'Query.ui'
**
** Created: Mon Apr 23 20:48:47 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef QUERY_H
#define QUERY_H

#include <qdialog.h>
class QLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QString;
class QVBoxLayout;
class QHBoxLayout;

class Query : public QDialog
{
    Q_OBJECT

public:
    Query( QWidget* parent=0, const char* name=0, bool modal = FALSE, WFlags fl=0, QString text=0, 
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
