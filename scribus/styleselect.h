#ifndef STYLESELECT_H
#define STYLESELECT_H

class QGroupBox;
class QToolButton;
class QLayout;
class MSpinBox;
class QLabel;
class QPopupMenu;

class StrikeValues : public QGroupBox
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

class UnderlineValues : public QGroupBox
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

class OutlineValues : public QGroupBox
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

class ShadowValues : public QGroupBox
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

class StyleSelect : public QWidget
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
	QGroupBox* buttonGroup;
	QGroupBox* buttonGroup2;
	QGroupBox* buttonGroup3;
	QGroupBox* buttonGroup4;
	QToolButton* underlineButton;
	QToolButton* underlineWordButton;
	QToolButton* subscriptButton;
	QToolButton* superscriptButton;
	QToolButton* smallcapsButton;
	QToolButton* allcapsButton;
	QToolButton* strikeoutButton;
	QToolButton* outlineButton;
	QToolButton* shadowButton;
	QPopupMenu* ShadowPop;
	QPopupMenu* OutlinePop;
	QPopupMenu* UnderlinePop;
	QPopupMenu* StrikePop;
};

#endif

