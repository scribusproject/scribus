#ifndef STYLESELECT_H
#define STYLESELECT_H

class QGroupBox;
class QToolButton;
class QLayout;
class MSpinBox;
class QLabel;
class QPopupMenu;

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
	QGroupBox* buttonGroup;
	QGroupBox* buttonGroup2;
	QGroupBox* buttonGroup3;
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
};

#endif

