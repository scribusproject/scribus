/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef INSPAGE_H
#define INSPAGE_H

#include "scribusapi.h"
#include <q3ptrlist.h>
#include <qstringlist.h>
//Added by qt3to4:
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>
#include <QDialog>

class QComboBox;
class QLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class Q3GroupBox;
class QCheckBox;
class ScrSpinBox;
class ScribusDoc;

class SCRIBUS_API InsPage : public QDialog
{
	Q_OBJECT

public:
	InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages );
	~InsPage() {};
	Q3GroupBox* dsGroupBox7;
	Q3GroupBox* masterPageGroup;
	ScrSpinBox* widthSpinBox;
	ScrSpinBox* heightSpinBox;
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

	const QStringList getMasterPages();
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
	Q3VBoxLayout* dialogLayout;
	Q3GridLayout* whereLayout;
	Q3GridLayout* masterPageLayout;
	Q3HBoxLayout* okCancelLayout;
	Q3GridLayout* dsGroupBox7Layout;

private slots:
	void insWherePageDataDisable (int index);
	void setSize(const QString &);
	void setOrien(int ori);
};

#endif // INSPAGE_H
