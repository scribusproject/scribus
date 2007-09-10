/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						latexeditor.h  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef LATEXEDITOR_H
#define LATEXEDITOR_H

#include "ui_latexeditor.h"
#include "scribusapi.h"
#include <QWidget>
#include <QProcess>
class PageItem_LatexFrame;
class LatexHighlighter;

class SCRIBUS_API LatexEditor : public QDialog, Ui::LatexEditor
{
	Q_OBJECT
	public:
		LatexEditor(PageItem_LatexFrame *);
		~LatexEditor();
		void startEditor();
		void exitEditor();
		void revert();
		void apply();
	protected:
		PageItem_LatexFrame *frame;
		LatexHighlighter *highlighter;
	public slots:
		void okClicked();
		void cancelClicked();
		void updateClicked(bool);
		void revertClicked(bool);
		void updateAppSettings(bool);
		void formulaChanged(QString, QString);
		void latexFinished();
		void stateChanged(QProcess::ProcessState state);
};

#endif
