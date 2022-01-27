/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMLINESTYLEWIDGET_H
#define SMLINESTYLEWIDGET_H

class QEvent;

#include "scribusstructs.h"
#include "styleitem.h"
#include "ui_smlinestylewidget.h"

class LineCombo;
class ColorCombo;
class ScrSpinBox;

class SMLineStyleWidget : public QWidget, Ui::SMLineStyleWidget
{
	Q_OBJECT

public:
	SMLineStyleWidget();
	~SMLineStyleWidget() = default;
	
	void showStyle(const multiLine &lineStyle, ColorList &colorList, int subLine = 0);
	void unitChange(int unitIndex);

protected:
	void changeEvent(QEvent *e) override;

private:
	multiLine  m_currentStyle;
	ColorList  m_colors;

	void fillEndCombo();
	void fillJoinCombo();

	void updateLineList();
	QColor getColor(const QString &name, int shade);

	friend class SMLineStyle;

protected slots:
	void iconSetChange();
	void languageChange();
	void slotEditNewLine(int i);

};

#endif
