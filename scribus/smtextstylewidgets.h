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
class MSpinBox;
class QLabel;
class QTabWidget;
class QString;
class QComboBox;
class AlignSelect;
class QSpinBox;
class QVBoxLayout;
class Tabruler;
class FontComboH;
class StyleSelect;
class QSpacerItem;
class ScComboBox;
class ShadeButton;
class CharStyle;
class ParagraphStyle;
class SMCStylePage;
class ColorList;
class ColorCombo;

class SMPStyleWidget : public PStyleWBase
{
	Q_OBJECT
public:
	SMPStyleWidget();
	~SMPStyleWidget();

	void show(ParagraphStyle &pstyle, QValueList<ParagraphStyle> &pstyles, QValueList<CharStyle> &cstyles, int unitIndex);
	void show(QValueList<ParagraphStyle> &pstyles, QValueList<ParagraphStyle> &pstylesAll, QValueList<CharStyle> &cstyles, int unitIndex);

	void clearAll();

	void languageChange();

private:
	QGridLayout *distancesBoxLayout;
	QGridLayout *dropCapsBoxLayout;
	QVBoxLayout *tabsBoxLayout;

	QComboBox   *lineSpacingMode_;

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
	QSpinBox    *dropCapLines_;
	SMMSpinBox  *dropCapOffset_;

	AlignSelect  *alignement_;
	Tabruler     *tabList_;
	SMCStylePage *cpage;

	void setupDistances();
	void setupDropCaps();
	void setupTabs();
	void setupCharStyle();
	void setupCStyleCombo(QValueList<CharStyle> &cstyles);

	friend class SMParagraphStyle;

private slots:
	void slotLineSpacingModeChanged(int);
};

class SMCStylePage : public CStylePBase
{
	Q_OBJECT
public:
	SMCStylePage(QWidget *parent = 0);
	~SMCStylePage();

	void show(CharStyle &cstyle, QValueList<CharStyle> &cstyles);
	void show(QValueList<CharStyle> &cstyles, QValueList<CharStyle> &cstylesAll);
	void fillLangCombo(QMap<QString,QString> langMap, const QString &defaultLang);
	void fillColorCombo(ColorList &colors);
	void languageChange();
	void clearAll();

private:
	QVBoxLayout *characterBoxLayout;
	QHBoxLayout *layout7;
	QHBoxLayout *layout8;
	QHBoxLayout *layout9a;
	QHBoxLayout *layout5;
	QHBoxLayout *layout6;

	FontComboH  *fontFace_;
	StyleSelect *effects_;
	ColorCombo  *fillColor_;
	ShadeButton *fillShade_;
	ColorCombo  *strokeColor_;
	ShadeButton *strokeShade_;
	ScComboBox  *language_;

	SMMSpinBox  *fontSize_;
	SMMSpinBox  *fontHScale_;
	SMMSpinBox  *fontVScale_;
	SMMSpinBox  *tracking_;
	SMMSpinBox  *baselineOffset_;

	QLabel      *TextF2;
	QLabel      *pixmapLabel3;
	QLabel      *pixmapLabel3_2;
	QLabel      *FillIcon;
	QLabel      *pixmapLabel3_20;
	QLabel      *StrokeIcon;
	QLabel      *pixmapLabel3_19;
	QLabel      *pixmapLabel3_3;
	QLabel      *pixmapLabel2;

	QSpacerItem *spacer1;
	QSpacerItem *spacer2;
	QSpacerItem *spacer4;

	QString                defaultLang_;
	QMap<QString, QString> langMap_;

	friend class SMParagraphStyle;
	friend class SMCharacterStyle;
};

#endif

