#ifndef APPLYT_H
#define APPLYT_H

#include <qdialog.h>

class QLabel;
class QComboBox;
class QButtonGroup;
class QRadioButton;
class QSpinBox;
class QCheckBox;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
	
class ScribusDoc;

class ApplyT : public QDialog
{
	Q_OBJECT

public:
	ApplyT( QWidget* parent, ScribusDoc *view, QString Nam);
	~ApplyT() {};

	QLabel* TextLabel1;
	QLabel* TextLabel2;
	QComboBox* Templ;
	QButtonGroup* buttonGroup1;
	QRadioButton* singlePage;
	QRadioButton* pageRange;
	QRadioButton* oddRange;
	QRadioButton* evenRange;
	QSpinBox* fromPage;
	QSpinBox* toPage;
	QCheckBox* useRange;
	QPushButton* cancelButton;
	QPushButton* okButton;
	

public slots:
	void RangeSel();
	void SingleSel();
	void CheckRangeF();
	void CheckRangeT();

protected:
	QVBoxLayout* ApplyTLayout;
	QHBoxLayout* Layout4;
	QHBoxLayout* layout3;
	QVBoxLayout* buttonGroup1Layout;
	QHBoxLayout* layout2;
};

#endif // APPLYT_H
