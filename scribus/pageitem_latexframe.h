/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						pageitem_latexframe.h  -  description
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

#ifndef PAGEITEM_LATEXFRAME_H
#define PAGEITEM_LATEXFRAME_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QProcess>

#include "scribusapi.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"

class FileWatcher;
class QTemporaryFile;

class SCRIBUS_API PageItem_LatexFrame : public PageItem_ImageFrame
{
	Q_OBJECT

	public:
		PageItem_LatexFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
		/*PageItem_LatexFrame(const PageItem & p) : PageItem_ImageFrame(p) { }*/
		~PageItem_LatexFrame();

		virtual PageItem_LatexFrame * asLatexFrame() { return this; }
		virtual void clearContents();
		
		/* Called from UI callback 
		UI-Handlers in other parts of the code should be as short as 
		possible so this function will take most of the UI related code. */
		void runEditor();
		/*TODO*/
		void convertToVector();
		/* Sets the formula text and forces rerunning latex at the next update */
		void setFormula(QString formula, bool undoable=true);
		QString getFormula() { return formulaText; }
		/* Runs the external application and sets internal vars and loads
		the image.*/
		void runApplication();
		QString getApplication();
		void setApplication(QString app) { /*TODO */ }
		int getDpi();
		void setDpi(int dpi) { /*TODO*/ }
		
		int getError() { return err; }
		
		void rerunApplication();
		
		void restore(UndoState *state, bool isUndo);
		
		static const QString defaultApp;
		static const QString defaultPre;
		static const QString defaultPost;
	protected:
		/* DrawObj_Item is defined in ImageFrame and should be usable for
		displaying images created by latex too in most cases */
		virtual void DrawObj_Item(ScPainter *p, QRect e, double sc);
		
		QString formulaText;
		
		void writeEditorFile();
		void writeFileContents(QFile *tempfile);
		void deleteImageFile();
		void loadEditorFile();
		/* Last error code */
		int err;
		int dpi;
		
		QString ImageFile, editorFile, tempFileBase;
		QString appStdout;
		QString appStderr;
		
		QProcess *latex, *editor;
		FileWatcher *fileWatcher;
		bool imgValid;
	protected slots:
		void updateImage(int exitCode, QProcess::ExitStatus exitStatus);
		void editorFinished(int exitCode, QProcess::ExitStatus exitStatus);
		void editorFileChanged(QString filename);
		void editorError(QProcess::ProcessError error);
		void latexError(QProcess::ProcessError error);
};

#endif
