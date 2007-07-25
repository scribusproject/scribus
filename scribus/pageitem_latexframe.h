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

#include <qobject.h>
#include <qwidget.h>


#include <qstring.h>
#include <qrect.h>
#include <QTemporaryFile>
#include <QProcess>

#include "scribusapi.h"
#include "undoobject.h"
#include "scimage.h"
#include "pagestructs.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"
#include "filewatcher.h"
// class ScPainter;
// class ScribusDoc;
// class UndoManager;
// class UndoState;
// struct CopyPasteBuffer;



class SCRIBUS_API PageItem_LatexFrame : public PageItem_ImageFrame
{
	Q_OBJECT

	public:
		PageItem_LatexFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
		PageItem_LatexFrame(const PageItem & p) : PageItem_ImageFrame(p) {}
		~PageItem_LatexFrame();

		/* Not sure about this casts */
		//virtual PageItem_ImageFrame * asImageFrame() { return this; }
		virtual PageItem_LatexFrame * asLatexFrame() { return this; }
		/* Keep handleModeEditKey from ImageFrame? */
		// virtual void handleModeEditKey(QKeyEvent *k, bool& keyRepeat);
		virtual void clearContents();
		
		/* Called from UI callback 
		UI-Handlers in other parts of the code should be as short as 
		possible so this function will take most of the UI related code. */
		void runEditor();
		void convertToVector();
		/* Sets the formula text and forces rerunning latex at the next update */
		void setFormula(QString &formula);
		QString getFormula() { return formula_text; }
		/* Runs the external application and sets internal vars and loads
		the image.*/
		void runApplication();
		
		
		//void changeApp(ExtAppConfig *app_config);
		
		
		int getError() { return err; }
		
		void rerunApplication();
		
	protected:
		/* DrawObj_Item is defined in ImageFrame and should be usable for
		displaying images created by latex too in most cases */
		virtual void DrawObj_Item(ScPainter *p, QRect e, double sc);
		
		QString formula_text;
		
		/* Which application to run */
		//ExtAppConfig application;
		
		void writeEditorFile();
		void writeFileContents(QFile *tempfile);
		void deleteImageFile();
		void loadEditorFile();
		/* Last error code */
		int err;
		int dpi;
		
		QString ImageFile, editorFile;
		QString appStdout;
		QString appStderr;
		
		QProcess *latex, *editor;
		QTemporaryFile *tempfile;
		FileWatcher *fileWatcher; //TODO: Use ScCore->fileWatcher?
		bool imgValid;
	protected slots:
		void updateImage(int exitCode, QProcess::ExitStatus exitStatus);
		void editorFinished(int exitCode, QProcess::ExitStatus exitStatus);
		void editorFileChanged(QString filename);
		void editorError(QProcess::ProcessError error);
		void latexError(QProcess::ProcessError error);
};

#endif
