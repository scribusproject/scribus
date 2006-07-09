/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMTEXTSTYLES_H
#define SMTEXTSTYLES_H

#include "styleitem.h"
#include "smpstylewbase.h"
#include "smcstylewbase.h"

class QGridLayout;
class QPopupMenu;
class QToolButton;
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
class ScribusDoc;

class SMPStyleWidget : public PStyleWBase
{
	Q_OBJECT
public:
	SMPStyleWidget();
	~SMPStyleWidget();

private:
	QGridLayout *distancesBoxLayout;
	QGridLayout *dropCapsBoxLayout;
	QVBoxLayout *tabsBoxLayout;

	QComboBox   *linespacingCombo;

	QLabel      *pixmapLabel0;
	QLabel      *pixmapLabel1;
	QLabel      *pixmapLabel2;
	QLabel      *pixmapLabel3;
	QLabel      *pixmapLabel4;
	QLabel      *capLabel1;
	QLabel      *capLabel2;

	MSpinBox    *LineSpVal;
	MSpinBox    *fontBase;
	MSpinBox    *fontKern;
	MSpinBox    *spaceAbove;
	MSpinBox    *spaceBelow;
	QSpinBox    *DropLines;
	MSpinBox    *DropDist;

	AlignSelect *alignSelect;
	Tabruler    *tabList;

	void setupDistances();
	void setupDropCaps();
	void setupTabs();
};

class SMParagraphStyle : public StyleItem
{
	Q_OBJECT
public:
	SMParagraphStyle();
	~SMParagraphStyle();

	QTabWidget* widget();
	QString typeName();
	void currentDoc(ScribusDoc *doc);
	QValueList<StyleName> styles();
	void selected(const QStringList &styleNames);
	QString fromSelection() const;
	void apply();
	void deleteStyles(const QValueList<RemoveItem> &removeList);
	void nameChanged(const QString &newName);

private:
	SMPStyleWidget *pwidget_;
	QValueList<ParagraphStyle> tmpPStyles_;
	ScribusDoc *doc_;
};

class SMCStyleWidget : public CStyleWBase
{
	Q_OBJECT
public:
	SMCStyleWidget();
	~SMCStyleWidget();

private:
	QVBoxLayout *characterBoxLayout;
	QHBoxLayout *layout7;
	QHBoxLayout *layout8;
	QHBoxLayout *layout9a;
	QHBoxLayout *layout5;
	QHBoxLayout *layout6;

	FontComboH  *FontC;
	StyleSelect *EffeS;
	ScComboBox  *TxFill;
	ShadeButton *PM2;
	ScComboBox  *TxStroke;
	ShadeButton *PM1;

	MSpinBox    *SizeC;
	MSpinBox    *fontHScale;
	MSpinBox    *fontVScale;
	MSpinBox    *fontKern;
	MSpinBox    *fontBase;

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

	void setupCharacter();
};

class SMCharacterStyle : public StyleItem
{
	Q_OBJECT
public:
	SMCharacterStyle();
	~SMCharacterStyle();

	QTabWidget* widget();
	QString typeName();
	void currentDoc(ScribusDoc *doc);
	QValueList<StyleName> styles();
	void selected(const QStringList &styleNames);
	QString fromSelection() const;
	void apply();
	void deleteStyles(const QValueList<RemoveItem> &removeList);
	void nameChanged(const QString &newName);

private:
	SMCStyleWidget *cwidget_;
};

#endif
