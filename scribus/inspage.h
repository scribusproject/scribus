/****************************************************************************
** Form interface generated from reading ui file 'InsPage.ui'
**
** Created: Tue Jun 12 21:21:09 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef INSPAGE_H
#define INSPAGE_H

class QDialog;
class QComboBox;
class QLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class ScribusDoc;

class InsPage : public QDialog
{ 
    Q_OBJECT

public:
    InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages, bool facingPages );
    ~InsPage() {};

	const QString getMasterPage();
	const QString getMasterPage2();
	const int getWhere();
	const int getWherePage();
	const int getCount();

private:
    QLabel* insCountLabel;
    QLabel* masterPageLabel;
    QLabel* masterPage2Label;
    QLabel* pagesLabel;
    QPushButton* cancelButton;
    QPushButton* okButton;
    QComboBox* insWhereData;
    QComboBox* masterPageData;
    QComboBox* masterPage2Data;
    QSpinBox* insWherePageData;
    QSpinBox* insCountData;
    QVBoxLayout* dialogLayout;
    QGridLayout* whereLayout;
    QHBoxLayout* masterPageLayout;
    QHBoxLayout* masterPage2Layout;
    QHBoxLayout* okCancelLayout;

private slots:
	void insWherePageDataDisable (int index);
};

#endif // INSPAGE_H
