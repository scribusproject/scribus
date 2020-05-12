/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMLINESTYLE_H
#define SMLINESTYLE_H

class QEvent;

#include "scribusstructs.h"
#include "styleitem.h"

class ScribusDoc;
class SMLineStyleWidget;


class SMLineStyle : public StyleItem
{
	Q_OBJECT
public:
	SMLineStyle();
	~SMLineStyle();
	
	virtual void changeEvent(QEvent *e);
	
	QTabWidget* widget();
	QString typeNamePlural();
	QString typeNameSingular();
	void setCurrentDoc(ScribusDoc *doc);
	QList<StyleName> styles(bool reloadFromDoc = true);
	void reload();
	void selected(const QStringList &styleNames);
	QString fromSelection() const;
	void toSelection(const QString &styleName) const;
	QString newStyle();
	QString newStyle(const QString &fromStyle);
	void apply();
	bool isDefaultStyle(const QString &stylename) const;
	void setDefaultStyle(bool ids);
	QString shortcut(const QString &stylename) const;
	void setShortcut(const QString &shortcut);
	void deleteStyles(const QList<RemoveItem> &removeList);
	void nameChanged(const QString &newName);
	QString getUniqueName(const QString &name);
	void languageChange();
	void unitChange();

signals:
	void deleteDone();
	void selectionDirty();

private:
	ScribusDoc               *m_doc = nullptr;
	SMLineStyleWidget        *m_widget = nullptr;
	QTabWidget               *m_twidget = nullptr;
	QHash<QString, multiLine> m_tmpLines;
	bool                      m_selectionIsDirty = false;
	double                    m_unitRatio = 1.0;
	QHash<QString, multiLine*> m_selection;
	QList<RemoveItem>         m_deleted;
	int                       m_currentLine = -1;

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
