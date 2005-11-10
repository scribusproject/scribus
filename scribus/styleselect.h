//Added by qt3to4:
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <Q3PopupMenu>
#ifndef STYLESELECT_H
#define STYLESELECT_H

class QToolButton;
class QLayout;
class MSpinBox;
class QLabel;
class Q3PopupMenu;
class WidgetPopupMenu;

#include "scribusapi.h"

class SCRIBUS_API StrikeValues : public QWidget
{
    Q_OBJECT

public:
	StrikeValues( QWidget* parent );
	~StrikeValues() {};
	MSpinBox* LPos;
	MSpinBox* LWidth;
	QLabel* LPosTxt;
	QLabel* LWidthTxt;

public slots:
	void languageChange();
	
protected:
	QGridLayout* group1Layout;
};

class SCRIBUS_API UnderlineValues : public QWidget
{
    Q_OBJECT

public:
	UnderlineValues( QWidget* parent );
	~UnderlineValues() {};
	MSpinBox* LPos;
	MSpinBox* LWidth;
	QLabel* LPosTxt;
	QLabel* LWidthTxt;

public slots:
	void languageChange();
	
protected:
	QGridLayout* group1Layout;
};

class SCRIBUS_API OutlineValues : public QWidget
{
    Q_OBJECT

public:
	OutlineValues( QWidget* parent );
	~OutlineValues() {};
	MSpinBox* LWidth;
	QLabel* LWidthTxt;

public slots:
	void languageChange();
	
protected:
	QGridLayout* group1Layout;
};

class SCRIBUS_API ShadowValues : public QWidget
{
    Q_OBJECT

public:
	ShadowValues( QWidget* parent );
	~ShadowValues() {};
	MSpinBox* Xoffset;
	MSpinBox* Yoffset;
	QLabel* XoffsetTxt;
	QLabel* YoffsetTxt;

public slots:
	void languageChange();
	
protected:
	QGridLayout* group1Layout;
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
	QHBoxLayout* ssLayout;
	QHBoxLayout* buttonGroupLayout;
	QHBoxLayout* buttonGroup2Layout;
	QHBoxLayout* buttonGroup3Layout;
	QHBoxLayout* buttonGroup4Layout;
	QWidget* buttonGroup;
                QWidget* buttonGroup2;
	QWidget* buttonGroup3;
	QWidget* buttonGroup4;
	QToolButton* underlineButton;
	QToolButton* underlineWordButton;
	QToolButton* subscriptButton;
	QToolButton* superscriptButton;
	QToolButton* smallcapsButton;
	QToolButton* allcapsButton;
	QToolButton* strikeoutButton;
	QToolButton* outlineButton;
	QToolButton* shadowButton;
	WidgetPopupMenu* ShadowPop;
	WidgetPopupMenu* OutlinePop;
	WidgetPopupMenu* UnderlinePop;
	WidgetPopupMenu* StrikePop;
};

#endif

