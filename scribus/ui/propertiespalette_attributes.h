#ifndef PROPERTIESPALETTE_ATTRIBUTES_H
#define PROPERTIESPALETTE_ATTRIBUTES_H

#include <QWidget>
#include "ui_propertiespalette_attributes.h"
#include "scribusapi.h"
#include "scguardedptr.h"

class BasePointWidget;
class NameWidget;
class PageItem;
class ScribusDoc;
class Selection;
class ScribusMainWindow;

class SCRIBUS_API PropertiesPalette_Attributes : public QWidget, Ui::PropertiesPalette_Attributes
{
	Q_OBJECT

public:
	explicit PropertiesPalette_Attributes(QWidget *parent = nullptr);
	~PropertiesPalette_Attributes() {};

private:
	PageItem* currentItemFromSelection();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void iconSetChange();
	void languageChange();
	void toggleLabelVisibility(bool visibility);

	void handleSelectionChanged();

private slots:
	void handlePrint();
	void handleNewName();
	void handlePDFBookmark();
	void handlePDFAnnotation();
	void handlePDFAnnotationSettings();

protected:
	bool m_haveDoc {false};
	bool m_haveItem {false};
	PageItem *m_item {nullptr};
	ScribusMainWindow* m_ScMW {nullptr};
	ScGuardedPtr<ScribusDoc> m_doc;

	void changeEvent(QEvent *e) override;
};

#endif // PROPERTIESPALETTE_ATTRIBUTES_H
