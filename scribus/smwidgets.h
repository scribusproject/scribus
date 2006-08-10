/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMWIDGETS_H
#define SMWIDGETS_H

#include "mspinbox.h"
#include "sccombobox.h"
#include "alignselect.h"
#include "styleselect.h"
#include "shadebutton.h"

class SMSpinBox : public QSpinBox
{
	Q_OBJECT
public:
	SMSpinBox(QWidget *parent, const char *name);
	~SMSpinBox() {};

	void setValue(int val);
	void setValue(int val, bool isParentVal);

	void setParentValue(int val);

	bool useParentValue();

protected:
	void interpretText();

private:
	bool   hasParent_;
	bool   useParentValue_;
	int    pValue_;
	void setFont(bool wantBold);

private slots:
	void slotValueChanged();
};

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

class SMScComboBox  : public ScComboBox
{
	Q_OBJECT
public:
	SMScComboBox(QWidget *parent, const char *name);
	SMScComboBox(bool rw, QWidget* parent, const char* name);
	~SMScComboBox() {};

	void setCurrentItem(int i);
	void setCurrentItem(int i, bool isParentValue);

	void setParentItem(int i);

	bool useParentValue();

private:
	bool   hasParent_;
	bool   useParentValue_;
	int    pItem_;
	void setFont(bool wantBold);

private slots:
	void currentChanged();
};

class SMAlignSelect : public AlignSelect
{
	Q_OBJECT
public:
	SMAlignSelect(QWidget *parent);
	~SMAlignSelect() {}

	void setStyle(int i);
	void setStyle(int i, bool isParentValue);

	void setParentItem(int i);

	bool useParentValue();

	QToolButton *parentButton;

private:
	bool   hasParent_;
	bool   useParentStyle_;
	int    pStyle_;
	void setFont(bool wantBold);

private slots:
	void styleChanged();
	void pbPressed();
};

class SMStyleSelect  : public StyleSelect
{
	Q_OBJECT
public:
	SMStyleSelect(QWidget *parent);
	~SMStyleSelect() {}

	void setStyle(int i);
	void setStyle(int i, bool isParentValue);

	void setParentItem(int i);

	bool useParentValue();

	QToolButton *parentButton;

private:
	bool   hasParent_;
	bool   useParentStyle_;
	int    pStyle_;
	void setFont(bool wantBold);

private slots:
	void styleChanged();
	void pbPressed();
};

class SMShadeButton  : public ShadeButton
{
	Q_OBJECT
public:
	SMShadeButton(QWidget *parent);
	~SMShadeButton() {};

	void setValue(int i);
	void setValue(int i, bool isParentValue);

	void setParentValue(int i);

	bool useParentValue();

private:
	bool   hasParent_;
	bool   useParentValue_;
	int    pValue_;
	void setFont(bool wantBold);

private slots:
	void currentChanged();
	void slotUseParent();
};


// class SMTabruler : public Tabruler
// {
// 	Q_OBJECT
// };
// 
// class SMFontComboH  : public FontComboH
// {
// 	Q_OBJECT
// };
// class SMColorCombo  : public ColorCombo
// {
// 	Q_OBJECT
// };
// 



#endif

