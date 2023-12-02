#ifndef PROPERTYWIDGET_TEXT_H
#define PROPERTYWIDGET_TEXT_H

#include "ui_propertywidget_textbase.h"
#include "propertywidgetbase.h"
#include <QFrame>

//namespace Ui {
//class PropertyWidget_Text;
//}

class CharStyle;
class ParagraphStyle;
class ScribusDoc;
class ScribusMainWindow;


class PropertyWidget_Text : public QFrame, Ui::PropertyWidget_TextBase,
		public PropertyWidgetBase
{
	Q_OBJECT

public:
	PropertyWidget_Text(QWidget *parent = nullptr);
	~PropertyWidget_Text() {};

private:
	PageItem* currentItemFromSelection();

protected:
	void connectSignals();
	void disconnectSignals();

	void configureWidgets();
	void setCurrentItem(PageItem *item);
	void changeEvent(QEvent *e) override;

	bool m_haveDoc {false};
	bool m_haveItem {false};
	PageItem* m_item { nullptr };
	ScribusMainWindow* m_ScMW { nullptr };

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);

	void handleSelectionChanged();
	void handleUpdateRequest(int);
	void iconSetChange();
	void languageChange();
	void localeChange();
	void changeLang(int id);
	void unitChange() {};

	void showAlignment(int e);
	void showDirection(int e);
	void showCharStyle(const QString& name);
	void showFontFace(const QString&);
	void showFontSize(double s);
	void showLanguage(const QString& w);
	void showLineSpacing(double r);
	void showParStyle(const QString& name);

	void setupLineSpacingSpinbox(int mode, double value);

	void updateCharStyle(const CharStyle& charStyle);
	void updateStyle(const ParagraphStyle& newCurrent);

	void updateCharStyles();
	void updateParagraphStyles();
	void updateTextStyles();

	void handleLineSpacingMode(int id);
	void handleTextFont(const QString&);

private slots:
	void handleAlignment(int a);
	void handleDirection(int d);
	void handleFontSize();
	void handleLineSpacing();

	void doClearCStyle();
	void doClearPStyle();

signals:
	void alignmentHandled();
};

#endif // PROPERTYWIDGET_TEXT_H
