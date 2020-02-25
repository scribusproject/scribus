/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABRULER_H
#define TABRULER_H

#include <QVariant>
#include <QWidget>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QVBoxLayout>

#include "scribusapi.h"
#include "sctextstruct.h"

class QComboBox;
class QPushButton;
class ScrSpinBox;
class QToolButton;

class SCRIBUS_API RulerT : public QWidget
{
	Q_OBJECT

public:
	RulerT(QWidget* parent, int ein, const QList<ParagraphStyle::TabRecord>& Tabs, bool ind, double wid);
	~RulerT() {};

	void setTabs(const QList<ParagraphStyle::TabRecord>& Tabs, int dEin);
	void updateTabList();

	bool mousePressed;
	QList<ParagraphStyle::TabRecord> tabValues;
	bool haveInd;
	int unitIndex;
	int offset;
	int actTab;
	double leftIndent;
	double firstLine;
	double Width;
	int rulerCode;
	int mouseX;
	int offsetIncrement;

public slots:
	void resetOffsetInc();
	void decreaseOffset();
	void increaseOffset();
	void changeTab(int t);
	void changeTabChar(QChar t);
	void moveTab(double t);
	void removeActTab();
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
	Tabruler(QWidget* parent,
			 bool haveFirst = true,
			 int dEin = 1,
			 const QList<ParagraphStyle::TabRecord>& Tabs = QList<ParagraphStyle::TabRecord>(),
			 double wid = -1);
	~Tabruler() {};

	virtual void changeEvent(QEvent *e);

	virtual void setTabs(const QList<ParagraphStyle::TabRecord>& Tabs, int dEin);

	QList<ParagraphStyle::TabRecord> getTabVals();
	double getFirstLine();
	double getLeftIndent();
	double getRightIndent();

public slots:
	void resetOFfL();
	void resetOFfR();
	void clearAll();
	void clearOne();
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
	void setRightIndentData(double t);
	void setRightIndent();
	void setTabFillChar(QChar t);
	void setFillChar();
	void setCustomFillChar(const QString &txt);

signals:
	/*! This signal is emitted when something is changed in the tab ruler dialog/widget.
	4/11/2005 pv */
	void tabrulerChanged();

	/** emitted when tabs are changed */
	void tabsChanged();
	/** emitted when left indent is changed */
	void leftIndentChanged(double);
	/** emitted when right indent is changed */
	void rightIndentChanged(double);
	/** emitted when first line is changed */
	void firstLineChanged(double);
	void mouseReleased();

protected:
	QVBoxLayout* tabrulerLayout { nullptr };
	QHBoxLayout* layout2 { nullptr };
	QHBoxLayout* layout1 { nullptr };
	QHBoxLayout* indentLayout { nullptr };
//	QVBoxLayout* layout3 { nullptr };
	QHBoxLayout* layout4 { nullptr };
	QComboBox* typeCombo { nullptr };
	QComboBox* tabFillCombo { nullptr };
	QLabel* tabFillComboT { nullptr };
	RulerT* ruler { nullptr };
	QToolButton* rulerScrollL { nullptr };
	QToolButton* rulerScrollR { nullptr };
	QLabel* positionLabel { nullptr };
	QLabel* firstLineLabel { nullptr };
	QLabel* leftIndentLabel { nullptr };
	QLabel* rightIndentLabel { nullptr };
	ScrSpinBox* tabData { nullptr };
	ScrSpinBox* firstLineData { nullptr };
	ScrSpinBox* leftIndentData { nullptr };
	ScrSpinBox* rightIndentData { nullptr };
	QPushButton* clearButton { nullptr };
	QPushButton* clearOneButton { nullptr };

	bool   m_haveFirst;
	double m_docUnitRatio;

protected slots:
	void iconSetChange();
	void languageChange();
	void slotMouseReleased();
};

#endif // TABRULER_H
