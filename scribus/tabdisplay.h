/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABDISPLAY_H
#define TABDISPLAY_H

#include <q3valuelist.h>
#include "tabdisplaybase.h"
#include "scribusapi.h"
#include "scribusstructs.h"

class ScribusDoc;

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
		void restoreDefaults(struct ApplicationPrefs *prefsData, struct guidesPrefs &guidesSettings, Q3ValueList<PageSet> &pageSets, int pageLayout, MarginStruct &scratch);
		void unitChange(QString unit, int docUnitIx, int decimals, double invUnitConversion);
		//! \brief Hide non-needed widgets for ReformDoc widget
		void setDocSetupMode();
		void setPaperColor(QColor neu);

		double DisScale;
		QColor colorPaper;
		QColor colorFrame;
		QColor colorFrameNorm;
		QColor colorFrameGroup;
		QColor colorFrameLocked;
		QColor colorFrameLinked;
		QColor colorFrameAnnotation;
		QColor colorPageBorder;
		QColor colorControlChars;

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
		virtual void changeFrameColor();
		virtual void changeNormFrameColor();
		virtual void changeGroupFrameColor();
		virtual void changeChainFrameColor();
		virtual void changeLockFrameColor();
		virtual void changeAnnotFrameColor();
		virtual void changePageBorderColor();
		virtual void changeControlCharsColor();
};

#endif
