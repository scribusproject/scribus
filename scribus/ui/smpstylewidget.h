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

#include "ui/charselectenhanced.h"
// #include "styles/styleset.h"


class SMPStyleWidget : public QWidget, Ui::SMPStyleWidget
{
	Q_OBJECT
public:
	SMPStyleWidget(ScribusDoc *doc);
	~SMPStyleWidget();
	
	virtual void changeEvent(QEvent *e);

	void show(ParagraphStyle *pstyle, QList<ParagraphStyle> &pstyles, QList<CharStyle> &cstyles, int unitIndex, const QString &defLang);
	void show(QList<ParagraphStyle*> &pstyles, QList<ParagraphStyle> &pstylesAll, QList<CharStyle> &cstyles, int unitIndex, const QString &defLang);

	void clearAll();
	void languageChange();
	void unitChange(double oldRatio, double newRatio, int unitIndex);
	void setDoc(ScribusDoc* doc);

private:
	bool hasParent_;
	//bool parentParEffects_;
	bool parentDC_, parentBul_, parentNum_;
	CharSelectEnhanced * m_enhanced;
	ScribusDoc * m_Doc;
	QString currFontName;
	ParagraphStyle* currPStyle;

	void fillBulletStrEditCombo();
	void fillNumFormatCombo();
	void fillNumerationsCombo();
	void fillNumRestartCombo();
	void checkParEffectState();
	void showLineSpacing(QList<ParagraphStyle*> &pstyles);
	void showSpaceAB(QList<ParagraphStyle*> &pstyles, int unitIndex);
	void showDropCap(QList<ParagraphStyle*> &pstyles, QList<CharStyle> &cstyles, int unitIndex);
	void showBullet(QList<ParagraphStyle*> &pstyles, QList<CharStyle> &cstyles, int unitIndex);
	void showNumeration(QList<ParagraphStyle*> &pstyles, QList<CharStyle> &cstyles, int unitIndex);
	void showAlignment(QList<ParagraphStyle*> &pstyles);
	void showOpticalMargin(QList<ParagraphStyle*> &pstyles);
	void showMinSpace(QList<ParagraphStyle*> &pstyles);
	void showMinGlyphExt(QList<ParagraphStyle*> &pstyles);
	void showMaxGlyphExt(QList<ParagraphStyle*> &pstyles);
	void showTabs(QList<ParagraphStyle*> &pstyles, int unitIndex);
	void showCStyle(QList<ParagraphStyle*> &pstyles, QList<CharStyle> &cstyles, const QString &defLang, int unitIndex);
	void showParent(QList<ParagraphStyle*> &pstyles);
	void setOpticalMargins(int o, bool inhO=false, const ParagraphStyle *parent=NULL);

	void openEnhanced();
	void closeEnhanced(bool show = false);
	void connectPESignals();
	void disconnectPESignals();

	friend class SMParagraphStyle;

private slots:
	void slotLineSpacingModeChanged(int);
	void slotDropCap(bool isOn);
	void slotBullets(bool isOn);
	void insertSpecialChars(const QString &);
	void slotNumbering(bool isOn);
	void slotParentParEffects();
	void slotDefaultOpticalMargins();
	void slotParentOpticalMargins();
//	void slotUpdateOpticalMarginsFromCheckBoxes(int);
	void on_bulletCharTableButton_toggled(bool checked);
	void handleUpdateRequest(int);

signals:
	void useParentParaEffects();
	void useParentOptMargins();
};

#endif
