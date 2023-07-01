/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABRULER_H
#define TABRULER_H

#include <QEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QVariant>
#include <QVBoxLayout>
#include <QWidget>

#include "scribusapi.h"
#include "sctextstruct.h"

class QComboBox;
class QPushButton;
class QToolButton;
class ScrSpinBox;

class SCRIBUS_API RulerT : public QWidget
{
	Q_OBJECT

public:
	RulerT(QWidget* parent, int unit, const QList<ParagraphStyle::TabRecord>& tabs, bool ind, double wid);
	~RulerT() {};

	void setTabs(const QList<ParagraphStyle::TabRecord>& tabs, int unit);
	void updateTabList();

	bool mousePressed { false };
	QList<ParagraphStyle::TabRecord> tabValues;
	bool haveInd { false };
	int unitIndex { 0 };
	int offset { 0 };
	int actTab { -1 };
	double leftIndent { 0.0 };
	double firstLine { 0.0 };
	int rulerCode { 0 };
	int mouseX { 0 };
	int offsetIncrement { 5 };

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
	void paintEvent(QPaintEvent *) override;
	void mousePressEvent(QMouseEvent *m) override;
	void mouseReleaseEvent(QMouseEvent *) override;
	void mouseMoveEvent(QMouseEvent *m) override;
	void leaveEvent(QEvent*) override;
	
private:
	double m_iter { 10.0 }; // Result of unitRulerGetIter1FromIndex() for point unit
	double m_iter2 { 100.0 }; // Result of unitRulerGetIter2FromIndex() for point unit
	double m_rulerWidth { 0.0 };
};

class SCRIBUS_API Tabruler : public QWidget
{
	Q_OBJECT

public:
	Tabruler(QWidget* parent,
			 bool haveFirst = true,
			 int unit = 1,
			 const QList<ParagraphStyle::TabRecord>& tabs = QList<ParagraphStyle::TabRecord>(),
			 double wid = -1);
	~Tabruler() {};

	virtual void setTabs(const QList<ParagraphStyle::TabRecord>& tabs, int unit);

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
	QToolButton* clearButton { nullptr };
	QToolButton* clearOneButton { nullptr };

	bool   m_haveFirst { true };
	double m_docUnitRatio { 1.0 };

	void changeEvent(QEvent *e) override;

protected slots:
	void iconSetChange();
	void languageChange();
	void slotMouseReleased();
};

#endif // TABRULER_H
