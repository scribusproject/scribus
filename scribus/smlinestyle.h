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
#include "ui_smlinestylew.h"

#include <qobject.h>
#include <qcolor.h>
//Added by qt3to4:
#include <Q3ValueList>

class ScribusDoc;
class LineCombo;
class ScrSpinBox;

class LineStyleWidget : public QWidget, Ui::LineStyleW
{
	Q_OBJECT
public:
	LineStyleWidget();
	~LineStyleWidget();
	void showStyle(const multiLine &lineStyle, ColorList &colorList, int subLine = 0);

	void languageChange();

private:
	LineCombo *dashCombo;
	ScrSpinBox  *lineWidth;
	multiLine  currentStyle;
	ColorList  colors;

	void updateLineList();
	QColor getColor(const QString &name, int shade);

	friend class SMLineStyle;

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
	QString typeNamePlural();
	QString typeNameSingular();
	void currentDoc(ScribusDoc *doc);
	Q3ValueList<StyleName> styles(bool reloadFromDoc = true);
	void reload();
	void selected(const QStringList &styleNames);
	QString fromSelection() const;
	void toSelection(const QString &styleName) const;
	QString newStyle();
	QString newStyle(const QString &fromStyle);
	void apply();
	QString shortcut(const QString &stylename) const;
	void setShortcut(const QString &shortcut);
	void deleteStyles(const Q3ValueList<RemoveItem> &removeList);
	void nameChanged(const QString &newName);
	QString getUniqueName(const QString &name);
	void languageChange();
	void unitChange();

signals:
	void deleteDone();
	void selectionDirty();

private:
	ScribusDoc               *doc_;
	LineStyleWidget          *widget_;
	QTabWidget               *twidget_;
	QMap<QString, multiLine>  tmpLines;
	bool                      selectionIsDirty_;
	QMap<QString, multiLine*> selection_;
	Q3ValueList<RemoveItem>    deleted_;
	int                       currentLine_;

	void setSelection(const QString& styleName);
	void setMultiSelection(const QStringList& styles);
	void setupConnections();
	void removeConnections();
	void updateSList();
	void updatePreview();
	void resort();
	void rebuildList();
	QColor calcFarbe(const QString &name, int shade);

private slots:
	void slotCurrentLineChanged(int i);
	void slotLineStyle(int i);
	void slotSetEnd(int i);
	void slotSetJoin(int i);
	void slotColor(const QString &s);
	void slotShade(int i);
	void slotLineWidth();
	void slotAddLine();
	void slotDeleteLine();

	// FIXME make line styles subclass of Style and remove this hack
	// ugly hack which will be removed once all styles are subclassed from Style
	friend class StyleManager;
};

#endif
