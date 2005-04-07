#ifndef TABRULER_H
#define TABRULER_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;
class QLabel;
class QPushButton;
class MSpinBox;
class QToolButton;

class RulerT : public QWidget
{
	Q_OBJECT

public:
	RulerT(QWidget* parent, int ein, QValueList<double> Tabs, bool ind, double wid);
	~RulerT() {};
	void updateTabList();
	bool mousePressed;
	QValueList<double> tabValues;
	bool haveInd;
	int unitIndex;
	int offset;
	int actTab;
	double leftIndent;
	double firstLine;
	double Width;
	int rulerCode;
	int mouseX;

public slots:
	void decreaseOffset();
	void increaseOffset();
	void changeTab(int t);
	void moveTab(double t);
	void moveFirstLine(double t);
	void moveLeftIndent(double t);

signals:
	void tabMoved(double);
	void typeChanged(int);
	void leftIndentMoved(double);
	void firstLineMoved(double);
	void noTabs();
	void newTab();

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void leaveEvent(QEvent*);
};

class Tabruler : public QWidget
{
	Q_OBJECT

public:
	Tabruler( QWidget* parent, bool haveFirst, int ein, QValueList<double> Tabs, double wid );
	~Tabruler() {};
	QValueList<double> getTabVals();
	bool haveF;
	double getFirstLine();
	double getLeftIndent();

public slots:
	void clearAll();
	void tabAdded();
	void lastTabRemoved();
	void setTabType(int t);
	void setType();
	void setTabData(double t);
	void setTab();
	void setFirstLineData(double t);
	void setFirstLine();
	void setLeftIndentData(double t);
	void setLeftIndent();

protected:
	QVBoxLayout* tabrulerLayout;
	QHBoxLayout* layout2;
	QHBoxLayout* layout1;
	QComboBox* TypeCombo;
	RulerT* ruler;
	QToolButton* rulerScrollL;
	QToolButton* rulerScrollR;
	QLabel* positionLabel;
	QLabel* firstLineLabel;
	QLabel* leftIndentLabel;
	MSpinBox* tabData;
	MSpinBox* firstLineData;
	MSpinBox* leftIndentData;
	QPushButton* clearButton;
	
	double docUnitRatio;
};

#endif // TABRULER_H
