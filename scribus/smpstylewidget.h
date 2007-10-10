/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMPSTYLEWIDGET_H
#define SMPSTYLEWIDGET_H

class QEvent;

#include "ui_smpstylewidget.h"

// #include "styles/styleset.h"


class SMPStyleWidget : public QWidget, Ui::SMPStyleWidget
{
	Q_OBJECT
public:
	SMPStyleWidget();
	~SMPStyleWidget();
	
	virtual void changeEvent(QEvent *e);

	void show(ParagraphStyle *pstyle, QList<ParagraphStyle> &pstyles, QList<CharStyle> &cstyles, int unitIndex, const QString &defLang);
	void show(QList<ParagraphStyle*> &pstyles, QList<ParagraphStyle> &pstylesAll, QList<CharStyle> &cstyles, int unitIndex, const QString &defLang);

	void clearAll();

	void languageChange();

	void unitChange(double oldRatio, double newRatio, int unitIndex);

private:
	bool hasParent_;
	bool parentDropCap_;

	void showLineSpacing(QList<ParagraphStyle*> &pstyles);
	void showSpaceAB(QList<ParagraphStyle*> &pstyles, int unitIndex);
	void showDropCap(QList<ParagraphStyle*> &pstyles, int unitIndex);
	void showAlignment(QList<ParagraphStyle*> &pstyles);
	void showTabs(QList<ParagraphStyle*> &pstyles, int unitIndex);
	void showCStyle(QList<ParagraphStyle*> &pstyles, QList<CharStyle> &cstyles, const QString &defLang, int unitIndex);
	void showParent(QList<ParagraphStyle*> &pstyles);

	friend class SMParagraphStyle;

private slots:
	void slotLineSpacingModeChanged(int);
	void slotDropCap(bool isOn);
	void slotParentDropCap();

signals:
	void useParentDropCap();
};

#endif
