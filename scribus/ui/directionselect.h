/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef DIRECTIONSELECT_H
#define DIRECTIONSELECT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QButtonGroup>

class QEvent;

#include "scribusapi.h"

class SCRIBUS_API DirectionSelect : public QWidget
{
	Q_OBJECT

	public:
		DirectionSelect(QWidget* parent);
		~DirectionSelect() {};

		void setStyle(int s);
		int getStyle();
		int selectedId();

		QButtonGroup* buttonGroup { nullptr };
		QToolButton* RTL { nullptr };
		QToolButton* LTR { nullptr };

	public slots:
		void iconSetChange();
		void languageChange();

	protected:
		QHBoxLayout* groupSelectLayout { nullptr };

		int m_selected { 0 };

		void changeEvent(QEvent *e) override;

	private slots:
		void setTypeStyle(int a);

	signals:
		void State(int);
};

#endif // DIRECTIONSELECT_H
