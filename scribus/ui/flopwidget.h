/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FLOPWIDGET_H
#define FLOPWIDGET_H


#include <QButtonGroup>
#include <QToolButton>
#include "ui/widgets/form_widget.h"

class ScribusDoc;

class FlopWidget : public FormWidget
{
	Q_OBJECT

public:

	enum FlopButtonID
    {
        RealHeightID   = 0,
        FontAscentID   = 1,
        LineSpacingID  = 2,
		BaselineGridID = 3
    };

	FlopWidget(QWidget* parent);
	~FlopWidget() {};

	QButtonGroup *flopGroup;
	QToolButton *flopRealHeight = {nullptr};
	QToolButton *flopFontAscent = {nullptr};
	QToolButton *flopLineSpacing = {nullptr};
	QToolButton *flopBaselineGrid = {nullptr};

protected:

	void changeEvent(QEvent *e) override;

public slots:

	void iconSetChange();
	void languageChange();
	void unitChange() {};
};

#endif
