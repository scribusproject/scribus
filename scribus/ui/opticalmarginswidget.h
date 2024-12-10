/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef OPTICALMARGINSWIDGET_H
#define OPTICALMARGINSWIDGET_H

#include <QComboBox>
#include <QToolButton>
#include "opticalmarginlookup.h"
#include "ui/widgets/form_widget.h"

class ParagraphStyle;
class ScribusDoc;
class ScribusMainWindow;

class OpticalMarginsWidget : public FormWidget
{
	Q_OBJECT

public:
	OpticalMarginsWidget(QWidget* parent);

	QToolButton *leftButton { nullptr };
	QToolButton *rightButton { nullptr };
	QToolButton *parentButton { nullptr };
	QComboBox *setsCombo { nullptr };

	int opticalMargin() const;
	void setOpticalMargin(int omt);

	void setOpticalMarginSets(const OpticalMarginSets& sets);
	const OpticalMarginSets& opticalMarginSets() const { return m_sets; };

	void setOpticalMarginSetId(const QString& id);
	QString opticalMarginSetId() const;

protected:
	void changeEvent(QEvent *e) override;
	void updateSetSelector();

	OpticalMarginSets m_sets;
	QString m_setId;

public slots:
	void iconSetChange();
	void languageChange();

signals:
	void opticalMarginChanged();

};

#endif
