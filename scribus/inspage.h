#ifndef INSPAGE_H
#define INSPAGE_H

#include "scribusapi.h"
#include <qptrlist.h>
#include <qstringlist.h>
class QDialog;
class QComboBox;
class QLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QGroupBox;
class QCheckBox;
class MSpinBox;
class ScribusDoc;

class SCRIBUS_API InsPage : public QDialog
{
	Q_OBJECT

public:
	InsPage( QWidget* parent, ScribusDoc* currentDoc, int currentPage, int maxPages );
	~InsPage() {};
	QGroupBox* dsGroupBox7;
	QGroupBox* masterPageGroup;
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
	QPtrList<QComboBox> masterPageCombos;

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
