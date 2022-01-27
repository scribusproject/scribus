/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SMTABRULER_H
#define SMTABRULER_H

#include "tabruler.h"

class SMScrSpinBox;


class SMTabruler : public Tabruler
{
	Q_OBJECT
public:
	SMTabruler(QWidget* parent,
			   bool haveFirst = true,
			   int dEin = 1,
			   QList<ParagraphStyle::TabRecord> Tabs = QList<ParagraphStyle::TabRecord>(),
			   double wid = -1);
	~SMTabruler() {};

	void unitChange(int unitIndex);

	void setTabs(const QList<ParagraphStyle::TabRecord>& Tabs, int unitIndex);
	void setTabs(const QList<ParagraphStyle::TabRecord>& Tabs, int unitIndex, bool isParentValue);
	void setParentTabs(const QList<ParagraphStyle::TabRecord>& Tabs);

	void setFirstLineValue(double t);
	void setFirstLineValue(double t, bool isParentValue);
	void setParentFirstLine(double t);

	void setLeftIndentValue(double t);
	void setLeftIndentValue(double t, bool isParentValue);
	void setParentLeftIndent(double t);

	void setRightIndentValue(double t);
	void setRightIndentValue(double t, bool isParentValue);
	void setParentRightIndent(double t);

	bool useParentTabs();
	bool useParentFirstLine();
	bool useParentLeftIndent();
	bool useParentRightIndent();

	SMScrSpinBox *firstLineSpin { nullptr };
	SMScrSpinBox *leftIndentSpin { nullptr };
	SMScrSpinBox *rightIndentSpin { nullptr };

private:
	QList<ParagraphStyle::TabRecord> m_parentTabs;
	int  m_unitIndex = 0;
	QToolButton *m_parentButton = nullptr;
	bool m_hasParent = false;
	bool m_tabsChanged = false;
	bool m_useParentTabs = false;
	bool m_isSetupRight = false;
	bool m_isSetupLeft = false;
	bool m_isSetupFirst = false;

private slots:
	void slotTabsChanged();
	void pbClicked();
	void firstDataChanged();
	void leftDataChanged();
	void rightDataChanged();
	void firstValueChanged();
	void leftValueChanged();
	void rightValueChanged();
};

#endif

