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
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3GridLayout>
#include <Q3ValueList>
#include <QLabel>
#include <Q3PopupMenu>
#include <Q3VBoxLayout>

class Q3GridLayout;
class Q3PopupMenu;
class QToolButton;
class SMScrSpinBox;
class QLabel;
class QTabWidget;
class QString;
class QComboBox;
class SMAlignSelect;
class SMSpinBox;
class Q3VBoxLayout;
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

	void show(ParagraphStyle *pstyle, Q3ValueList<ParagraphStyle> &pstyles, Q3ValueList<CharStyle> &cstyles, int unitIndex, const QString &defLang);
	void show(Q3ValueList<ParagraphStyle*> &pstyles, Q3ValueList<ParagraphStyle> &pstylesAll, Q3ValueList<CharStyle> &cstyles, int unitIndex, const QString &defLang);

	void clearAll();

	void languageChange();

	void unitChange(double oldRatio, double newRatio, int unitIndex);

private:
	Q3GridLayout *distancesBoxLayout;
	Q3GridLayout *dropCapsBoxLayout;
	Q3VBoxLayout *tabsBoxLayout;

	SMScComboBox *lineSpacingMode_;

	QLabel      *pixmapLabel0;
	QLabel      *pixmapLabel1;
	QLabel      *pixmapLabel2;
	QLabel      *pixmapLabel3;
	QLabel      *pixmapLabel4;
	QLabel      *capLabel1;
	QLabel      *capLabel2;

	SMScrSpinBox  *lineSpacing_;
	SMScrSpinBox  *spaceAbove_;
	SMScrSpinBox  *spaceBelow_;
	SMSpinBox   *dropCapLines_;
	SMScrSpinBox  *dropCapOffset_;

	SMAlignSelect *alignement_;
	SMTabruler    *tabList_;
	SMCStylePage  *cpage;

	bool hasParent_;
	bool parentDropCap_;

	void setupDistances();
	void setupDropCaps();
	void setupTabs();
	void setupCharStyle();

	void showLineSpacing(Q3ValueList<ParagraphStyle*> &pstyles);
	void showSpaceAB(Q3ValueList<ParagraphStyle*> &pstyles, int unitIndex);
	void showDropCap(Q3ValueList<ParagraphStyle*> &pstyles, int unitIndex);
	void showAlignment(Q3ValueList<ParagraphStyle*> &pstyles);
	void showTabs(Q3ValueList<ParagraphStyle*> &pstyles, int unitIndex);
	void showCStyle(Q3ValueList<ParagraphStyle*> &pstyles, Q3ValueList<CharStyle> &cstyles, const QString &defLang, int unitIndex);
	void showParent(Q3ValueList<ParagraphStyle*> &pstyles);

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

	void show(CharStyle *cstyle, Q3ValueList<CharStyle> &cstyles, const QString &defLang, int unitIndex);
	void show(Q3ValueList<CharStyle*> &cstyles, Q3ValueList<CharStyle> &cstylesAll, const QString &defLang, int unitIndex);
	void fillLangCombo(QMap<QString,QString> langMap);
	void fillColorCombo(ColorList &colors);
	void languageChange();
	void clearAll();

private:
	Q3VBoxLayout *basicBoxLayout;
	Q3VBoxLayout *advBoxLayout;
	Q3VBoxLayout *colorBoxLayout;
	Q3HBoxLayout *layout8;
	Q3HBoxLayout *layout9a;
	Q3HBoxLayout *layout5;
	Q3HBoxLayout *layout6;
	Q3GridLayout *spinBoxLayout_;
	Q3GridLayout *spinBoxLayoutBasic_;

	SMFontComboH  *fontFace_;
	SMStyleSelect *effects_;
	SMColorCombo  *fillColor_;
	SMShadeButton *fillShade_;
	SMColorCombo  *strokeColor_;
	SMShadeButton *strokeShade_;
	SMScComboBox  *language_;

	SMScrSpinBox  *fontSize_;
	SMScrSpinBox  *fontHScale_;
	SMScrSpinBox  *fontVScale_;
	SMScrSpinBox  *tracking_;
	SMScrSpinBox  *baselineOffset_;

	QLabel      *fontSizeLabel_;
	QLabel      *hscaleLabel_;
	QLabel      *vscaleLabel_;
	QLabel      *FillIcon;
	QLabel      *pixmapLabel3_20;
	QLabel      *StrokeIcon;
	QLabel      *pixmapLabel3_19;
	QLabel      *trackingLabel_;
	QLabel      *baselineOffsetLabel_;
	QLabel      *languageLabel_;

	QSpacerItem *spacer1;
	QSpacerItem *spacer2;
	QSpacerItem *spacer3;
	QSpacerItem *spacer4;

	QMap<QString, QString> langMap_;

	void showSizeAndPosition(const Q3ValueList<CharStyle*> &cstyles);
	void showEffects(const Q3ValueList<CharStyle*> &cstyles);
	void showColors(const Q3ValueList<CharStyle*> &cstyles);
	void showLanguage(const Q3ValueList<CharStyle*> &cstyles, const QString &defLang);
	void showParent(const Q3ValueList<CharStyle*> &cstyles);

	friend class SMParagraphStyle;
	friend class SMCharacterStyle;

private slots:
	void slotColorChange();
};

#endif

