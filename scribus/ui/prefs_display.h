/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#ifndef PREFS_DISPLAY_H
#define PREFS_DISPLAY_H

#include "ui_prefs_displaybase.h"
#include "prefs_pane.h"
#include "scribusapi.h"

class ScribusDoc;

class SCRIBUS_API Prefs_Display : public Prefs_Pane, Ui::Prefs_Display
{
	Q_OBJECT

	public:
		Prefs_Display(QWidget* parent, ScribusDoc* doc=NULL);
		~Prefs_Display();
		virtual void restoreDefaults(struct ApplicationPrefs *prefsData);
		virtual void saveGuiToPrefs(struct ApplicationPrefs *prefsData) const;

	public slots:
		void languageChange();


	protected slots:
		/*!
		\author Franz Schmid
		\brief Preferences (Display, Display Size). Draws ruler depending on scaling factor
		*/
		void drawRuler();
		/*!
		\author Franz Schmid
		\brief Sets Scale for ruler scale from the display value
		*/
		void restoreDisScale();
		/*!
		\author Franz Schmid
		\brief Preferences (Display, Display Size). Sets Scale for ruler scale
		*/
		void setDisScale();
		/*!
		\author Franz Schmid
		\brief Slots for changing colors
		*/
		virtual void changePaperColor();
		virtual void changeScratchColor();
		virtual void changeFrameColor();
		virtual void changeNormFrameColor();
		virtual void changeGroupFrameColor();
		virtual void changeChainFrameColor();
		virtual void changeLockFrameColor();
		virtual void changeAnnotFrameColor();
		virtual void changePageBorderColor();
		virtual void changeControlCharsColor();



	protected:
		int docUnitIndex;
		QColor colorPaper;
		QColor colorScratch;
		QColor colorFrame;
		QColor colorFrameNorm;
		QColor colorFrameGroup;
		QColor colorFrameLocked;
		QColor colorFrameLinked;
		QColor colorFrameAnnotation;
		QColor colorPageBorder;
		QColor colorControlChars;
		double displayScale;

};

#endif // PREFS_DISPLAY_H
