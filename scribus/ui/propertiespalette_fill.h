#ifndef PROPERTIESPALETTE_FILL_H
#define PROPERTIESPALETTE_FILL_H

#include "ui_propertiespalette_fill.h"
#include <QWidget>
#include "scribusapi.h"
#include "scguardedptr.h"

class ScribusDoc;
class ScribusMainWindow;
class PageItem;
class ColorPicker;
class UndoManager;

class SCRIBUS_API PropertiesPalette_Fill : public QWidget, Ui::PropertiesPalette_Fill
{
	Q_OBJECT

public:
	explicit PropertiesPalette_Fill(QWidget *parent = nullptr);
	~PropertiesPalette_Fill() {};

	void updateColorList();

public slots:
	void setMainWindow(ScribusMainWindow *mw);

	void setDoc(ScribusDoc *d);
	void setCurrentItem(PageItem *item);
	void unsetDoc();
	void unsetItem();

	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void iconSetChange();
	void languageChange();
	void unitChange();
	void toggleLabelVisibility(bool v);

private:
	PageItem* currentItemFromSelection();
	void setCurrentItem_Fill(PageItem *item);
	void setCurrentItem_FillMask(PageItem *item);

	int m_blockUpdates {0};
	void blockUpdates(bool block) { if (block) ++m_blockUpdates; else --m_blockUpdates; }
	bool updatesBlocked() { return (m_blockUpdates > 0); }

protected:
	bool m_haveDoc {false};
	bool m_haveItem {false};
	double m_unitRatio {1.0};
	PageItem *m_item {nullptr};
	ScribusMainWindow* m_ScMW {nullptr};
	ScGuardedPtr<ScribusDoc> m_doc;
	UndoManager* undoManager {nullptr};
	QButtonGroup *buttonsCaps;
	QButtonGroup *buttonsJoins;

private slots:
	void handleFillRule();
	void handleFillColor();
	void handleFillColorVector();
	void handleFillColorMesh();
	void handleFillMask();
	void handleFillMaskVector();
	void handleFillBlendmode(int mode);


};

#endif // PROPERTIESPALETTE_FILL_H
