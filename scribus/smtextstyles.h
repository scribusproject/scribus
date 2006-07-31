/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMTEXTSTYLES_H
#define SMTEXTSTYLES_H

#include "styleitem.h"

class CharStyle;
class ParagraphStyle;
class ScribusDoc;
class SMCStylePage;
class SMPStyleWidget;

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
	void editMode(bool isOn);
	void deleteStyles(const QValueList<RemoveItem> &removeList);
	void nameChanged(const QString &newName);

private:
	SMPStyleWidget *pwidget_;
	ScribusDoc *doc_;
	QValueList<ParagraphStyle> tmpStyles_;
	QValueList<ParagraphStyle*> selection_;

	void updateStyleList();
	void reloadTmpStyles();
	QValueList<CharStyle> getCharStyles();
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
	void editMode(bool isOn);
	void deleteStyles(const QValueList<RemoveItem> &removeList);
	void nameChanged(const QString &newName);

private:
	QTabWidget   *widget_;
	SMCStylePage *page_;
	ScribusDoc   *doc_;
	QValueList<CharStyle> tmpStyles_;
	QValueList<CharStyle*> selection_;

	void reloadTmpStyles();
};

#endif
