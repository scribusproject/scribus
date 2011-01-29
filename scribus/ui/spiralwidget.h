/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SPIRALWIDGET_H
#define SPIRALWIDGET_H

#include <QWidget>
#include "scribusapi.h"
#include "ui_spiralwidgetbase.h"
struct toolPrefs;

class SCRIBUS_API SpiralWidget : public QWidget, Ui::SpiralWidgetBase
{
	Q_OBJECT

	public:
		SpiralWidget(QWidget* parent);
		~SpiralWidget() {};
		void restoreDefaults(struct ItemToolPrefs *prefsData);
		void saveGuiToPrefs(struct ItemToolPrefs *prefsData);

	protected slots:
		void updatePreview();

	protected:
		void connectSignals(bool);
};

#endif
