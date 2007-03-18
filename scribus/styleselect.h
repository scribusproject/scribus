/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef STYLESELECT_H
#define STYLESELECT_H

#include <Q3PopupMenu>
#include <Q3GridLayout>
#include <Q3GroupBox>
#include <Q3HBoxLayout>
#include <QLabel>

class QToolButton;
class QLayout;
class ScrSpinBox;
class QLabel;
class Q3PopupMenu;

#include "scribusapi.h"

class SCRIBUS_API StrikeValues : public Q3GroupBox
{
    Q_OBJECT

public:
	StrikeValues( QWidget* parent );
	~StrikeValues() {};
	ScrSpinBox* LPos;
	ScrSpinBox* LWidth;
	QLabel* LPosTxt;
	QLabel* LWidthTxt;

public slots:
	void languageChange();
	
protected:
	Q3GridLayout* group1Layout;
};

class SCRIBUS_API UnderlineValues : public Q3GroupBox
{
    Q_OBJECT

public:
	UnderlineValues( QWidget* parent );
	~UnderlineValues() {};
	ScrSpinBox* LPos;
	ScrSpinBox* LWidth;
	QLabel* LPosTxt;
	QLabel* LWidthTxt;

public slots:
	void languageChange();
	
protected:
	Q3GridLayout* group1Layout;
};

class SCRIBUS_API OutlineValues : public Q3GroupBox
{
    Q_OBJECT

public:
	OutlineValues( QWidget* parent );
	~OutlineValues() {};
	ScrSpinBox* LWidth;
	QLabel* LWidthTxt;

public slots:
	void languageChange();
	
protected:
	Q3GridLayout* group1Layout;
};

class SCRIBUS_API ShadowValues : public Q3GroupBox
{
    Q_OBJECT

public:
	ShadowValues( QWidget* parent );
	~ShadowValues() {};
	ScrSpinBox* Xoffset;
	ScrSpinBox* Yoffset;
	QLabel* XoffsetTxt;
	QLabel* YoffsetTxt;

public slots:
	void languageChange();
	
protected:
	Q3GridLayout* group1Layout;
};

class SCRIBUS_API StyleSelect : public QWidget
{

	Q_OBJECT

public:
	StyleSelect(QWidget* parent);
	~StyleSelect() {};
	void setStyle(int s);
	int getStyle();
	ShadowValues* ShadowVal;
	OutlineValues* OutlineVal;
	UnderlineValues* UnderlineVal;
	StrikeValues* StrikeVal;

public slots:
	void languageChange();

private slots:
	void setTypeStyle();

signals:
	void State(int);

protected:
	Q3HBoxLayout* ssLayout;
	Q3HBoxLayout* buttonGroupLayout;
	Q3HBoxLayout* buttonGroup2Layout;
	Q3HBoxLayout* buttonGroup3Layout;
	Q3HBoxLayout* buttonGroup4Layout;
	Q3GroupBox* buttonGroup;
	Q3GroupBox* buttonGroup2;
	Q3GroupBox* buttonGroup3;
	Q3GroupBox* buttonGroup4;
	QToolButton* underlineButton;
	QToolButton* underlineWordButton;
	QToolButton* subscriptButton;
	QToolButton* superscriptButton;
	QToolButton* smallcapsButton;
	QToolButton* allcapsButton;
	QToolButton* strikeoutButton;
	QToolButton* outlineButton;
	QToolButton* shadowButton;
	Q3PopupMenu* ShadowPop;
	Q3PopupMenu* OutlinePop;
	Q3PopupMenu* UnderlinePop;
	Q3PopupMenu* StrikePop;
};

#endif

