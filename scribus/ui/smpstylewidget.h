/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMPSTYLEWIDGET_H
#define SMPSTYLEWIDGET_H

class QEvent;
class CharStyle;

#include <QVector>

#include "ui_smpstylewidget.h"

#include "styles/styleset.h"
#include "ui/charselectenhanced.h"

class SMPStyleWidget : public QWidget, Ui::SMPStyleWidget
{
	Q_OBJECT

public:
	SMPStyleWidget(ScribusDoc *doc, StyleSet<CharStyle> *cstyles);
	~SMPStyleWidget() = default;

	void show(ParagraphStyle *pstyle, QList<ParagraphStyle> &pstyles, QList<CharStyle> &cstyles, int unitIndex, const QString &defLang);
	void show(QList<ParagraphStyle*> &pstyles, QList<ParagraphStyle> &pstylesAll, QList<CharStyle> &cstyles, int unitIndex, const QString &defLang);

	void clearAll();
	void languageChange();
	void unitChange(double oldRatio, double newRatio, int unitIndex);
	void setDoc(ScribusDoc* doc);

protected:
	void changeEvent(QEvent *e) override;

private:
	bool m_hasParent = false;
	bool m_parentDC = false;
	bool m_parentBul = false;
	bool m_parentNum = false;
	CharSelectEnhanced * m_enhanced = nullptr;
	ScribusDoc * m_Doc = nullptr;
	QString m_currFontName;
	ParagraphStyle* m_currPStyle = nullptr;
	StyleSet<CharStyle> *m_cstyles = nullptr;

	void fillColorCombo(ColorList &colors);
	void fillBulletStrEditCombo();
	void fillNumerationsCombo();
	void fillNumRestartCombo();
	void checkParEffectState();
	void showColors(const QList<ParagraphStyle*> &cstyles);
	void showLineSpacing(const QList<ParagraphStyle*> &pstyles);
	void showSpaceAB(const QList<ParagraphStyle*> &pstyles, int unitIndex);
	void showDropCap(const QList<ParagraphStyle*> &pstyles, const QList<CharStyle> &cstyles, int unitIndex);
	void showBullet(const QList<ParagraphStyle*> &pstyles, const QList<CharStyle> &cstyles, int unitIndex);
	void showNumeration(const QList<ParagraphStyle*> &pstyles, const QList<CharStyle> &cstyles, int unitIndex);
	void showAlignment(const QList<ParagraphStyle*> &pstyles);
	void showDirection(const QList<ParagraphStyle*> &pstyles);
	void showOpticalMargin(const QList<ParagraphStyle*> &pstyles);
	void showMinSpace(const QList<ParagraphStyle*> &pstyles);
	void showMinGlyphExt(const QList<ParagraphStyle*> &pstyles);
	void showMaxGlyphExt(const QList<ParagraphStyle*> &pstyles);
	void showConsecutiveLines(const QList<ParagraphStyle*> &pstyles);
	void showTabs(const QList<ParagraphStyle*> &pstyles, int unitIndex);
	void showCStyle(const QList<ParagraphStyle*> &pstyles, const QList<CharStyle> &cstyles, const QString &defLang, int unitIndex);
	void showParent(const QList<ParagraphStyle*> &pstyles);
	void setOpticalMargins(int o, bool inhO = false, const ParagraphStyle* parent = nullptr);

	void openEnhanced();
	void closeEnhanced(bool show = false);
	void connectPESignals();
	void disconnectPESignals();

	friend class SMParagraphStyle;

private slots:
	void iconSetChange();
	void slotLineSpacingModeChanged(int);
	void slotDropCap(bool isOn);
	void slotBullets(bool isOn);
	void insertSpecialChars(const QVector<uint> &charCodes);
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
