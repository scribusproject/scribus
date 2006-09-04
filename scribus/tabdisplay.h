/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABDISPLAY_H
#define TABDISPLAY_H

#include "tabdisplaybase.h"
#include "scribusapi.h"

/*! \brief Display panel for preferences dialog.
This class is inherited from UI base class.
\author Petr Vanek <petr@scribus.info>
*/
class SCRIBUS_API TabDisplay : public TabDisplayBase
{
	Q_OBJECT

	public:
		TabDisplay(QWidget* parent = 0, const char* name = 0);
		~TabDisplay(){};
		void restoreDefaults(struct ApplicationPrefs *prefsData, struct guidesPrefs *guidesSettings);
		void unitChange(QString unit, int docUnitIx, int decimals, double invUnitConversion);
		//! \brief Hide non-needed widgets for ReformDoc widget
		void hideReform();

		QColor colorPaper;
		double DisScale;

	private:
		int docUnitIndex;

	private slots:
		/*!
		\author Franz Schmid
		\brief Preferences (Display, Display Size). Draws ruler depending on scaling factor
		*/
		void drawRuler();
		/*!
		\author Franz Schmid
		\brief Preferences (Display, Display Size). Sets Scale for ruler scale
		*/
		void setDisScale();
		/*!
		\author Franz Schmid
		\brief Sets implicit paper color
		*/
		virtual void changePaperColor();

};

#endif
