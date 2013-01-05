#ifndef PROPERTYWIDGET_PAREFFECT_H
#define PROPERTYWIDGET_PAREFFECT_H

#include "ui_propertywidget_pareffectbase.h"
#include "numeration.h"
#include "propertywidgetbase.h"
#include "ui/charselectenhanced.h"

class PageItem;
class ParagraphStyle;
class ScribusMainWindow;

class PropertyWidget_ParEffect : public QFrame, private Ui::PropertyWidget_ParEffectBase, public PropertyWidgetBase
{
    Q_OBJECT

public:
	PropertyWidget_ParEffect(QWidget *parent = 0);
	~PropertyWidget_ParEffect() {}

	void updateStyle(const ParagraphStyle& newPStyle);
	void updateCharStyles();

	void displayCharStyle(const QString& name);
	void connectSignals();
	void disconnectSignals();
	CharSelectEnhanced * m_enhanced;
	
	void fillNumerationsCombo();
protected:
	double m_unitRatio;
	int    m_unitIndex;

	PageItem *         m_item;
	ScribusMainWindow* m_ScMW;

	void configureWidgets();
	void setCurrentItem(PageItem *item);

	virtual void changeEvent(QEvent *e);
	void handleChanges(PageItem* item, ParagraphStyle& newStyle);

public slots:
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *doc);

	void handleAppModeChanged(int oldMode, int mode);
	void handleSelectionChanged();
	void handleUpdateRequest(int);

	void languageChange();
	void unitChange();

	void handleParEffectUse();
	void handleBulletStr(QString);
	void handleDropCapLines(int);
	void handleNumName(QString);
	void handleNumFormat(int);
	void handleNumLevel(int);
	void handleNumPrefix(QString);
	void handleNumSuffix(QString);
	void handleNumStart(int);
	void handlePEOffset(double);
	void handlePEIndent(bool);
	void handlePECharStyle(QString);

private slots:
	void on_bulletCharTableButton__toggled(bool checked);
	void insertSpecialChars(const QString &chars);

private:
	void openEnhanced();
	void closeEnhanced(bool show = false);
	void enableDropCap(bool);
	void enableBullet(bool);
	void enableNum(bool);
	void enableParEffect(bool);
	void fillBulletStrEditCombo()
	{
		bulletStrEdit_->clear();
		bulletStrEdit_->addItem(QChar(0x2022));
		bulletStrEdit_->addItem("*");
		bulletStrEdit_->addItem(QChar(0x2013));
		bulletStrEdit_->setMinimumWidth(50);
		if (bulletStrEdit_->currentText().isEmpty())
			bulletStrEdit_->setEditText(QChar(0x2022));
	}

	void fillNumFormatCombo()
	{
		numFormatCombo->clear();
		numFormatCombo->addItems(getFormatList());
	}

	void fillPECombo()
	{
		peCombo->clear();
		peCombo->addItem(tr("No Paragraph Effects"));
		peCombo->addItem(tr("Drop Caps"));
		peCombo->addItem(tr("Bulleted List"));
		peCombo->addItem(tr("Numerated List"));
	}

//	void SMPStyleWidget::fillNumRestartCombo()
//	{
//		numRestartCombo->clear();
//		numRestartCombo->addItem(tr("Document"));
//		numRestartCombo->addItem(tr("Section"));
//		numRestartCombo->addItem(tr("Story"));
//		numRestartCombo->addItem(tr("Page"));
//		numRestartCombo->addItem(tr("Frame"));
//	}
};

#endif // PROPERTYWIDGET_PAREFFECT_H
