/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMTEXTSTYLEWIDGETS_H
#define SMTEXTSTYLEWIDGETS_H

#include "smpstylewbase.h"
#include "smcstylewbase.h"
#include "styles/styleset.h"

class QGridLayout;
class QPopupMenu;
class QToolButton;
class SMMSpinBox;
class QLabel;
class QTabWidget;
class QString;
class QComboBox;
class SMAlignSelect;
class SMSpinBox;
class QVBoxLayout;
class SMTabruler;
class SMFontComboH;
class StyleSelect;
class QSpacerItem;
class SMScComboBox;
class SMShadeButton;
class CharStyle;
class ParagraphStyle;
class SMCStylePage;
class ColorList;
class SMColorCombo;
class SMStyleSelect;

class SMPStyleWidget : public PStyleWBase
{
	Q_OBJECT
public:
	SMPStyleWidget();
	~SMPStyleWidget();

	void show(ParagraphStyle *pstyle, QValueList<ParagraphStyle> &pstyles, QValueList<CharStyle> &cstyles, int unitIndex, const QString &defLang);
	void show(QValueList<ParagraphStyle*> &pstyles, QValueList<ParagraphStyle> &pstylesAll, QValueList<CharStyle> &cstyles, int unitIndex, const QString &defLang);

	void clearAll();

	void languageChange();

private:
	QGridLayout *distancesBoxLayout;
	QGridLayout *dropCapsBoxLayout;
	QVBoxLayout *tabsBoxLayout;

	SMScComboBox *lineSpacingMode_;

	QLabel      *pixmapLabel0;
	QLabel      *pixmapLabel1;
	QLabel      *pixmapLabel2;
	QLabel      *pixmapLabel3;
	QLabel      *pixmapLabel4;
	QLabel      *capLabel1;
	QLabel      *capLabel2;

	SMMSpinBox  *lineSpacing_;
	SMMSpinBox  *spaceAbove_;
	SMMSpinBox  *spaceBelow_;
	SMSpinBox   *dropCapLines_;
	SMMSpinBox  *dropCapOffset_;

	SMAlignSelect *alignement_;
	SMTabruler    *tabList_;
	SMCStylePage  *cpage;

	bool hasParent_;
	bool parentDropCap_;

	void setupDistances();
	void setupDropCaps();
	void setupTabs();
	void setupCharStyle();

	void showLineSpacing(QValueList<ParagraphStyle*> &pstyles);
	void showSpaceAB(QValueList<ParagraphStyle*> &pstyles);
	void showDropCap(QValueList<ParagraphStyle*> &pstyles);
	void showAlignment(QValueList<ParagraphStyle*> &pstyles);
	void showTabs(QValueList<ParagraphStyle*> &pstyles, int unitIndex);
	void showCStyle(QValueList<ParagraphStyle*> &pstyles, QValueList<CharStyle> &cstyles, const QString &defLang);
	void showParent(QValueList<ParagraphStyle*> &pstyles);

	friend class SMParagraphStyle;

private slots:
	void slotLineSpacingModeChanged(int);
	void slotDropCap(bool isOn);
	void slotParentDropCap();

signals:
	void useParentDropCap();
};

class SMCStylePage : public CStylePBase
{
	Q_OBJECT
public:
	SMCStylePage(QWidget *parent = 0);
	~SMCStylePage();

	void show(CharStyle *cstyle, QValueList<CharStyle> &cstyles, const QString &defLang);
	void show(QValueList<CharStyle*> &cstyles, QValueList<CharStyle> &cstylesAll, const QString &defLang);
	void fillLangCombo(QMap<QString,QString> langMap);
	void fillColorCombo(ColorList &colors);
	void languageChange();
	void clearAll();

private:
	QVBoxLayout *characterBoxLayout;
	QHBoxLayout *layout8;
	QHBoxLayout *layout9a;
	QHBoxLayout *layout5;
	QHBoxLayout *layout6;
	QGridLayout *spinBoxLayout_;

	SMFontComboH  *fontFace_;
	SMStyleSelect *effects_;
	SMColorCombo  *fillColor_;
	SMShadeButton *fillShade_;
	SMColorCombo  *strokeColor_;
	SMShadeButton *strokeShade_;
	SMScComboBox  *language_;

	SMMSpinBox  *fontSize_;
	SMMSpinBox  *fontHScale_;
	SMMSpinBox  *fontVScale_;
	SMMSpinBox  *tracking_;
	SMMSpinBox  *baselineOffset_;

	QLabel      *fontSizeLabel_;
	QLabel      *hscaleLabel_;
	QLabel      *vscaleLabel_;
	QLabel      *FillIcon;
	QLabel      *pixmapLabel3_20;
	QLabel      *StrokeIcon;
	QLabel      *pixmapLabel3_19;
	QLabel      *trackingLabel_;
	QLabel      *baselineOffsetLabel_;

	QSpacerItem *spacer1;
	QSpacerItem *spacer2;
	QSpacerItem *spacer4;

	QMap<QString, QString> langMap_;

	void showSizeAndPosition(const QValueList<CharStyle*> &cstyles);
	void showEffects(const QValueList<CharStyle*> &cstyles);
	void showColors(const QValueList<CharStyle*> &cstyles);
	void showLanguage(const QValueList<CharStyle*> &cstyles, const QString &defLang);
	void showParent(const QValueList<CharStyle*> &cstyles);

	friend class SMParagraphStyle;
	friend class SMCharacterStyle;

private slots:
	void slotColorChange();
};

#endif

