#ifndef PROPERTYWIDGET_ALIGNMENT_H
#define PROPERTYWIDGET_ALIGNMENT_H

#include <QFrame>
#include "sctextstruct.h"
#include "ui_propertywidget_alignment.h"

#include "propertywidgetbase.h"
#include "units.h"

class ScribusDoc;
class ScribusMainWindow;

class PropertyWidget_Alignment : public QFrame, public Ui::PropertyWidget_Alignment,
		public PropertyWidgetBase
{
	Q_OBJECT

public:
	PropertyWidget_Alignment(QWidget* parent);
	~PropertyWidget_Alignment() {};

	void updateStyle(const ParagraphStyle& newCurrent);

protected:
	void connectSignals();
	void disconnectSignals();

	double m_unitRatio {1.0};
	bool m_haveItem {false};
	bool m_haveDoc {false};
	int m_unitIndex {SC_PT};
	PageItem *m_item {nullptr};
	ScribusMainWindow* m_ScMW {nullptr};

	void configureWidgets();
	void setCurrentItem(PageItem *item);

	void changeEvent(QEvent *e) override;

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *d);

	void handleAppModeChanged(int oldMode, int mode);
	void handleSelectionChanged();
	void handleUpdateRequest(int) {};

	void iconSetChange() {};
	void languageChange();
	void unitChange();
	void localeChange() {};
	void toggleLabelVisibility(bool v);

	void showFirstLinePolicy(FirstLineOffsetPolicy f);
	void showOpticalMargins(int e);
	void showAlignment(int e);
	void showDirection(int e);

private slots:

	void handleTabs();
	void handleFirstLinePolicy(int);
	void handleOpticalMargins();
//	void resetOpticalMargins();

	void handleAlignment(int a);
	void handleVAlignment(int a);
	void handleDirection(int d);

};

#endif // PROPERTYWIDGET_ALIGNMENT_H
