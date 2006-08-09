/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMWIDGETS_H
#define SMWIDGETS_H

#include "mspinbox.h"

// class SMComboBox : public QComboBox
// {
// 	Q_OBJECT
// };
// 
// class SMSpinBox : public QSpinBox
// {
// 	Q_OBJECT
// };

class SMMSpinBox : public MSpinBox
{
	Q_OBJECT
public:
	SMMSpinBox(QWidget *pa, int s);
	SMMSpinBox(double minValue, double maxValue, QWidget *pa, int s);
	SMMSpinBox(QWidget *parent, const char * name = 0);
	~SMMSpinBox() {};

	void setValue(double val);
	void setValue(double val, bool isParentVal);

	void setParentValue(double val);

	bool useParentValue();

protected:
	void interpretText();

private:
	bool   hasParent_;
	bool   useParentValue_;
	double pValue_;
	void setFont(bool wantBold);

private slots:
	void slotValueChanged();

};

// class SMAlignSelect : public AlignSelect
// {
// 	Q_OBJECT
// };
// 
// class SMTabruler : public Tabruler
// {
// 	Q_OBJECT
// };
// 
// class SMFontComboH  : public FontComboH
// {
// 	Q_OBJECT
// };
// 
// class SMStyleSelect  : public StyleSelect
// {
// 	Q_OBJECT
// };
// 
// class SMColorCombo  : public ColorCombo
// {
// 	Q_OBJECT
// };
// 
// class SMShadeButton  : public ShadeButton
// {
// 	Q_OBJECT
// };
// 
// class SMColorCombo  : public ColorCombo
// {
// 	Q_OBJECT
// };
// 
// class SMShadeButton  : public ShadeButton
// {
// 	Q_OBJECT
// };
// 
// class SMScComboBox  : public ScComboBox
// {
// 	Q_OBJECT
// };

#endif

