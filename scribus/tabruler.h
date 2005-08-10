#ifndef TABRULER_H
#define TABRULER_H

#include <qvariant.h>
#include <qwidget.h>

#include "scribusapi.h"
#include "pageitem.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;
class QLabel;
class QPushButton;
class MSpinBox;
class QToolButton;

class SCRIBUS_API RulerT : public QWidget
{
	Q_OBJECT

public:
	RulerT(QWidget* parent, int ein, QValueList<PageItem::TabRecord> Tabs, bool ind, double wid);
	~RulerT() {};
	void updateTabList();
	bool mousePressed;
	QValueList<PageItem::TabRecord> tabValues;
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
	void changeTabChar(QChar t);
	void moveTab(double t);
	void moveFirstLine(double t);
	void moveLeftIndent(double t);

signals:
	void tabMoved(double);
	void typeChanged(int);
	void fillCharChanged(QChar);
	void leftIndentMoved(double);
	void firstLineMoved(double);
	void noTabs();
	void newTab();
	void mouseReleased();
	void tabSelected();

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void mousePressEvent(QMouseEvent *m);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *m);
	virtual void leaveEvent(QEvent*);
	
private:
	double iter, iter2;
};

class SCRIBUS_API Tabruler : public QWidget
{
	Q_OBJECT

public:
	Tabruler( QWidget* parent, bool haveFirst, int ein, QValueList<PageItem::TabRecord> Tabs, double wid );
	~Tabruler() {};
	QValueList<PageItem::TabRecord> getTabVals();
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
	void setTabFillChar(QChar t);
	void setFillChar();
	void setCustomFillChar(const QString &txt);

signals:
	/*! This signal is emited when is something changed in the tab ruler dialog/widget.
	4/11/2005 pv */
	void tabrulerChanged();

protected:
	QVBoxLayout* tabrulerLayout;
	QHBoxLayout* layout2;
	QHBoxLayout* layout1;
	QVBoxLayout* layout3;
	QComboBox* TypeCombo;
	QComboBox* tabFillCombo;
	QLabel* tabFillComboT;
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
