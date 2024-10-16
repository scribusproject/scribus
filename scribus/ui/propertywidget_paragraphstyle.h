#ifndef PROPERTYWIDGET_PARAGRAPHSTYLE_H
#define PROPERTYWIDGET_PARAGRAPHSTYLE_H

#include "ui_propertywidget_paragraphstyle.h"
#include "propertywidgetbase.h"
#include <QFrame>

class CharStyle;
class ParagraphStyle;
class ScribusDoc;
class ScribusMainWindow;


class PropertyWidget_ParagraphStyle : public QFrame, Ui::PropertyWidget_ParagraphStyle,
		public PropertyWidgetBase
{
	Q_OBJECT

public:
	PropertyWidget_ParagraphStyle(QWidget *parent = nullptr);
	~PropertyWidget_ParagraphStyle() {};

	void hideParagraphStyle(bool hide);
	void hideCharacterStyle(bool hide);

private:
	PageItem* currentItemFromSelection();

protected:
	void connectSignals();
	void disconnectSignals();

	void setCurrentItem(PageItem *item);
	void changeEvent(QEvent *e) override;

	bool m_haveDoc {false};
	bool m_haveItem {false};
	bool m_hasParagraph {true};
	bool m_hasCharacter {true};
	PageItem* m_item { nullptr };
	ScribusMainWindow* m_ScMW { nullptr };

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);

	void handleSelectionChanged();
	void handleUpdateRequest(int);
	void iconSetChange();
	void languageChange();
	void localeChange() {};
	void toggleLabelVisibility(bool v) {};

	void showCharStyle(const QString& name);
	void showParStyle(const QString& name);

	void updateStyle(const ParagraphStyle& newCurrent);
	void updateTextStyles();

private slots:
	void doClearCStyle();
	void doClearPStyle();
	void newPStyle();
	void newCStyle();
	void editPStyle();
	void editCStyle();

};



#endif // PROPERTYWIDGET_PARAGRAPHSTYLE_H
