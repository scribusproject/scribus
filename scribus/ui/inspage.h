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
#include <QDialogButtonBox>
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

	QString prefsPageSizeName;

	QStringList getMasterPages() const;
	QString getMasterPageN(uint n) const;
	int getWhere() const;
	int getWherePage() const;
	int getCount() const;

	double pageWidth() const;
	double pageHeight() const;
	int    orientation() const;
	bool   overrideMasterPageSizing() const;
	bool   moveObjects() const;

private:
	ScribusDoc* m_doc { nullptr };

	QLabel* insCountLabel { nullptr };
	QLabel* masterPageLabel { nullptr };
	QLabel* pagesLabel { nullptr };
	QDialogButtonBox* buttonBox { nullptr };
	QComboBox* insWhereData { nullptr };
	QSpinBox* insWherePageData { nullptr };
	QSpinBox* insCountData { nullptr };
	QVBoxLayout* dialogLayout { nullptr };
	QGridLayout* whereLayout { nullptr };
	QGridLayout* masterPageLayout { nullptr };
	QHBoxLayout* okCancelLayout { nullptr };
	QGridLayout* dsGroupBox7Layout { nullptr };

	QGroupBox*  dsGroupBox7 { nullptr };
	QGroupBox*  masterPageGroup { nullptr };
	ScrSpinBox* widthSpinBox { nullptr };
	ScrSpinBox* heightSpinBox { nullptr };
	QLabel*	    widthQLabel { nullptr };
	QLabel*	    heightQLabel { nullptr };
	QLabel*     textLabel1 { nullptr };
	QLabel*     textLabel2 { nullptr };
	QComboBox*  sizeQComboBox { nullptr };
	QComboBox*  orientationQComboBox { nullptr };
	QCheckBox*  moveObjectsCheckBox { nullptr };
	QCheckBox*  overrideMPSizingCheckBox { nullptr };
	
	double m_unitRatio { 1.0 };
	QList<QComboBox*> masterPageCombos;

private slots:
	void insWherePageDataDisable (int index);
	void setSize(const QString &);
	void setOrientation(int ori);
	void enableSizingControls(int);
};

#endif // INSPAGE_H
