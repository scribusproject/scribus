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
    QGroupBox* buttonGroup;
    QToolButton* underlineButton;
    QToolButton* subscriptButton;
    QToolButton* superscriptButton;
    QToolButton* smallcapsButton;
    QToolButton* strikeoutButton;
    QToolButton* outlineButton;
};

#endif

