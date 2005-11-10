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

#include "scribusapi.h"
#include <q3ptrlist.h>
#include <qstringlist.h>
//Added by qt3to4:
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDialog>
#include <QComboBox> // needed for ~QComboBox() from Q3PtrList<QComboBox>

class QLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class Q3GroupBox;
class QCheckBox;
class MSpinBox;
class ScribusDoc;

class SCRIBUS_API InsPage : public QDialog
{
	Q_OBJECT

public:
	InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages );
	~InsPage() {};
	Q3GroupBox* dsGroupBox7;
	Q3GroupBox* masterPageGroup;
	MSpinBox* widthMSpinBox;
	MSpinBox* heightMSpinBox;
	QLabel*	widthQLabel;
	QLabel*	heightQLabel;
	QLabel* TextLabel1;
	QLabel* TextLabel2;
	QComboBox* sizeQComboBox;
	QComboBox* orientationQComboBox;
	QCheckBox* moveObjects;
	double unitRatio;
	QString prefsPageSizeName;
	Q3PtrList<QComboBox> masterPageCombos;

	const QStringList InsPage::getMasterPages();
	const QString getMasterPageN(uint n);
	const int getWhere();
	const int getWherePage();
	const int getCount();

private:
	QLabel* insCountLabel;
	QLabel* masterPageLabel;
	QLabel* pagesLabel;
	QPushButton* cancelButton;
	QPushButton* okButton;
	QComboBox* insWhereData;
	QSpinBox* insWherePageData;
	QSpinBox* insCountData;
	QVBoxLayout* dialogLayout;
	QGridLayout* whereLayout;
	QGridLayout* masterPageLayout;
	QHBoxLayout* okCancelLayout;
	QGridLayout* dsGroupBox7Layout;

private slots:
	void insWherePageDataDisable (int index);
	void setSize(const QString &);
	void setOrien(int ori);
};

#endif // INSPAGE_H
