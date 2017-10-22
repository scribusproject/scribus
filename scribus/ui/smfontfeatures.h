/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef SMFONTFEATURES_H
#define SMFONTFEATURES_H

#include "ui_smfontfeaturesbase.h"
#include <QMenu>

class StylisticSetsMenu : public QMenu
{
	Q_OBJECT
public:
	StylisticSetsMenu(QWidget *parent): QMenu(parent){}
	~StylisticSetsMenu(){}

	void mouseReleaseEvent(QMouseEvent *e)
	{
		QAction *action = activeAction();
		if (action && action->isEnabled()) {
			action->setEnabled(false);
			QMenu::mouseReleaseEvent(e);
			action->setEnabled(true);
			action->trigger();
		}
		else
			QMenu::mouseReleaseEvent(e);
	}
};

class SMFontFeatures : public QWidget, public Ui::smFontFeaturesBase
{
	Q_OBJECT
public:
	SMFontFeatures(QWidget *parent);
	~SMFontFeatures(){}

	virtual void changeEvent(QEvent *e);
	void languageChange();

	void setFontFeatures(QString s, QStringList fontFeaturesList);
	void setFontFeatures(QString val, QStringList fontFeatures, bool isParentVal);
	QString fontFeatures();
	void enableFontFeatures(QStringList fontFeatures);
	void resetFontFeatures();

	void setParentValue(QString val);
	bool useParentValue();

	StylisticSetsMenu *styleSetsMenu;

protected:
	void connectSignals();
	void disconnectSignals();

private:
	void hideAllFontFeatures();
	void setupCapitalCombo(quint64 featureFlags);
	void setupPositionCombo(quint64 featureFlags);
	void setupNumeralStyleCombo(quint64 featureFlags);
	void setupNumeralWidthCombo(quint64 featureFlags);
	void setupNumeralFractionCombo(quint64 featureFlags);

	QStringList comboboxFeatures(QComboBox* combo);

	bool m_hasParent;
	bool m_useParentValue;
	QString m_pValue;

private slots:
	void slotChange();
	void slotContextualCheck();
	void slotCommonCheck();
	void slotDiscretionaryCheck();
	void slotHistoricalCheck();
	void slotCapitalsComboBox();
	void slotPositionComboBox();
	void slotNumeralComboBox();
	void slotWidthComboBox();
	void slotFractionComboBox();
	void slotSlashedZeroCheck();
	void slotSetMenu();


signals:
	void changed();
};

#endif // SMFONTFEATURES_H
