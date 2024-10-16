#ifndef PROPERTYWIDGET_TEXT_H
#define PROPERTYWIDGET_TEXT_H

#include "ui_propertywidget_textbase.h"
#include "propertywidgetbase.h"
#include <QFrame>

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
	bool m_blockUpdate {false};
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
	void unitChange() {};
	void toggleLabelVisibility(bool v);

	void showFontFace(const QString&);
	void showFontSize(double s);
	void showLineSpacing(const ParagraphStyle &newCurrent);
	void showTextColors(const QString& fillCol, const QString& backCol, const QString& strokeCol, double fillShd, double backShd, double strokeShd);

	void setupLineSpacingSpinbox(int mode, double value);

	void updateCharStyle(const CharStyle& charStyle);
	void updateStyle(const ParagraphStyle& newCurrent);

	void handleLineSpacingMode(int id);
	void handleTextFont(const QString&);


private slots:

	void handleFontSize();
	void handleLineSpacing();
	void handleTextFill();
	void handleTextBackground();
	void handleTextStroke();

};

#endif // PROPERTYWIDGET_TEXT_H
