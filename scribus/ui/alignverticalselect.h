/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef ALIGNVERTICALSELECT_H
#define ALIGNVERTICALSELECT_H

#include <QHBoxLayout>
#include <QToolButton>
#include <QButtonGroup>

class QEvent;

#include "scribusapi.h"
#include "ui/widgets/form_widget.h"

class SCRIBUS_API AlignVerticalSelect : public FormWidget
{

Q_OBJECT

public:
	AlignVerticalSelect(QWidget* parent);
	~AlignVerticalSelect() {};

	void setStyle(int s);
	int getStyle() const;

	QButtonGroup* buttonGroup { nullptr };
	QToolButton* TextT { nullptr };
	QToolButton* TextC { nullptr };
	QToolButton* TextB { nullptr };

public slots:
	void iconSetChange();
	void languageChange();
	void setTypeStyle(int a);

protected:

	void changeEvent(QEvent *e) override;

signals:
	void State(int);
};

#endif // ALIGNVERTICALSELECT_H


