/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MARGINDIALOG_H
#define MARGINDIALOG_H

#include "scribusapi.h"

class QDialog;
class QLayout;
class QGroupBox;
class QLabel;
class QPushbutton;
class QComboBox;
class QCheckBox;
class MSpinBox;
class MarginWidget;
class ScribusDoc;

class SCRIBUS_API MarginDialog : public QDialog
{
	Q_OBJECT

public:
	MarginDialog( QWidget* parent,  ScribusDoc* doc);
	~MarginDialog() {};
	QGroupBox* dsGroupBox7;
	MSpinBox* widthMSpinBox;
	MSpinBox* heightMSpinBox;
	QLabel*	widthQLabel;
	QLabel*	heightQLabel;
	QLabel* TextLabel1;
	QLabel* TextLabel2;
	QComboBox* sizeQComboBox;
	QComboBox* orientationQComboBox;
	QComboBox* Links;
	QLabel* TextLabel3;
	QCheckBox* moveObjects;
	MarginWidget* GroupRand;
	QPushButton* cancelButton;
	QPushButton* okButton;
	double unitRatio;
	double pageWidth;
	double pageHeight;
	int oldOri;
	QString prefsPageSizeName;

public slots:
	virtual void setPageSize();
	virtual void setSize(const QString &);
	virtual void setOrien(int ori);
	virtual void setPageWidth(int v);
	virtual void setPageHeight(int v);

protected:
	QGridLayout* dsGroupBox7Layout;
	QVBoxLayout* dialogLayout;
	QHBoxLayout* okCancelLayout;
};

#endif
