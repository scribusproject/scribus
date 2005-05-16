#ifndef STYLESELECT_H
#define STYLESELECT_H

class QGroupBox;
class QToolButton;
class QLayout;

class StyleSelect : public QWidget
{

	Q_OBJECT

public:
	StyleSelect(QWidget* parent);
	~StyleSelect() {};
	void setStyle(int s);
	int getStyle();

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
};

#endif

