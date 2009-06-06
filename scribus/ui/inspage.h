/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef INSPAGE_H
#define INSPAGE_H

#include "scribusapi.h"
#include <QDialog>
#include <QList>
#include <QStringList>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QLabel;
class QPushButton;
class QSpinBox;
class QGroupBox;
class QCheckBox;
class QComboBox;
class ScrSpinBox;
class ScribusDoc;

class SCRIBUS_API InsPage : public QDialog
{
	Q_OBJECT

public:
	InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages );
	~InsPage() {};
	QGroupBox* dsGroupBox7;
	QGroupBox* masterPageGroup;
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
	QList<QComboBox*> masterPageCombos;

	const QStringList getMasterPages();
	const QString getMasterPageN(uint n);
	int getWhere() const;
	int getWherePage() const;
	int getCount() const;

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
