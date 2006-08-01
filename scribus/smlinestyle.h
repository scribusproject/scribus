/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMLINESTYLE_H
#define SMLINESTYLE_H

#include "scribusstructs.h"
#include "styleitem.h"
#include "smlinestylewbase.h"

#include <qobject.h>
#include <qcolor.h>

class ScribusDoc;
class LineCombo;
class MSpinBox;

class LineStyleWidget : public LineStyleWBase // .ui implementation
{
	Q_OBJECT
public:
	LineStyleWidget();
	~LineStyleWidget();
	void showStyle(const multiLine &lineStyle, ColorList &colorList);

private:
	LineCombo *dashCombo;
	MSpinBox  *lineWidth;
	multiLine  currentStyle;
	ColorList  colors;

	void updateLineList();
	QColor getColor(const QString &name, int shade);

protected slots:
	void slotEditNewLine(int i);
};

class SMLineStyle : public StyleItem
{
	Q_OBJECT
public:
	SMLineStyle();
	~SMLineStyle();
	QTabWidget* widget();
	QString typeName();
	void currentDoc(ScribusDoc *doc);
	QValueList<StyleName> styles();
	void selected(const QStringList &styleNames);
	QString fromSelection() const;
	void apply();
	void deleteStyles(const QValueList<RemoveItem> &removeList);
	void nameChanged(const QString &newName);
	void languageChange();

signals:
	void deleteDone();
	void selectionDirty();

private:
	ScribusDoc              *doc_;
	LineStyleWidget         *widget_;
	QTabWidget              *twidget_;
	QMap<QString, multiLine> tmpLines;

	void reset();
	void setSelection(const QString& styleName);
	void setMultiSelection(const QStringList& styles);
};

#endif
