/****************************************************************************
** Form interface generated from reading ui file 'tabruler.ui'
**
** Created: Son Mai 9 12:45:32 2004
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

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
	void UpdateTabList();
	bool Mpressed;
	QValueList<double> TabValues;
	bool haveInd;
	int Einheit;
	int Offset;
	int ActTab;
	double Indent;
	double First;
	double Width;
	int RulerCode;
	int MouseX;

public slots:
	void decreaseOffset();
	void increaseOffset();
	void changeTab(int t);
	void moveTab(double t);
	void moveFirst(double t);
	void moveIndent(double t);

signals:
	void TabMoved(double);
	void TypeChanged(int);
	void IndentMoved(double);
	void FirstMoved(double);
	void NoTabs();
	void NewTab();

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
	double getFirst();
	double getIndent();

	QComboBox* TypeCombo;
	QToolButton* RulerScrollL;
	RulerT* Ruler;
	QToolButton* RulerScrollR;
	QLabel* Label1;
	MSpinBox* TabSpin;
	QLabel* Label2;
	MSpinBox* FirstSpin;
	QLabel* Label3;
	MSpinBox* IndentSpin;
	QPushButton* ClearButton;

public slots:
	void clearAll();
	void tabAdded();
	void lastTabRemoved();
	void setTabType(int t);
	void setType();
	void setTabSpin(double t);
	void setTab();
	void setFirstSpin(double t);
	void setFirst();
	void setIndentSpin(double t);
	void setIndent();

protected:
	QVBoxLayout* tabrulerLayout;
	QHBoxLayout* layout2;
	QHBoxLayout* layout1;

};

#endif // TABRULER_H
