/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMCSTYLEWIDGET_H
#define SMCSTYLEWIDGET_H

class QEvent;

#include "ui_smcstylewidget.h"
#include "styles/charstyle.h"
// #include "styles/styleset.h"


class SMCStyleWidget : public QWidget, public Ui::SMCStyleWidget
{
	Q_OBJECT
public:
	SMCStyleWidget(QWidget *parent = 0);
	~SMCStyleWidget();
	
	virtual void changeEvent(QEvent *e);

	void show(CharStyle *cstyle, QList<CharStyle> &cstyles, const QString &defLang, int unitIndex);
	void show(QList<CharStyle*> &cstyles, QList<CharStyle> &cstylesAll, const QString &defLang, int unitIndex);
	void fillLangCombo(QMap<QString,QString> langMap);
	void fillColorCombo(ColorList &colors);
	void languageChange();
	void clearAll();

private:
	QMap<QString, QString> langMap_;

	void showSizeAndPosition(const QList<CharStyle*> &cstyles);
	void showEffects(const QList<CharStyle*> &cstyles);
	void showColors(const QList<CharStyle*> &cstyles);
	void showLanguage(const QList<CharStyle*> &cstyles, const QString &defLang);
	void showParent(const QList<CharStyle*> &cstyles);

	friend class SMParagraphStyle;
	friend class SMCharacterStyle;

private slots:
	void slotColorChange();
};

#endif
