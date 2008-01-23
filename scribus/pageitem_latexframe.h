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
#include <QMap>

#include "scribusapi.h"
#include "pageitem.h"
#include "pageitem_imageframe.h"

class FileWatcher;
class QTemporaryFile;
class LatexEditor;
class LatexConfigParser;
class QTimer;

class SCRIBUS_API PageItem_LatexFrame : public PageItem_ImageFrame
{
	Q_OBJECT

	public:
		PageItem_LatexFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline);
		~PageItem_LatexFrame();

		virtual PageItem_LatexFrame * asLatexFrame() { return this; }
		virtual void clearContents();
		virtual ItemType realItemType() const { return PageItem::LatexFrame; }
		virtual void applicableActions(QStringList& actionList);
		virtual QString infoDescription();
		void layout();
		void restore(UndoState *state, bool isUndo);
		
		
		/* Called from UI callback 
		UI-Handlers in other parts of the code should be as short as 
		possible so this function will take most of the UI related code. */
		void runEditor();
		/*TODO void convertToVector(); */
		
		/* Sets the formula text and forces rerunning latex at the next update 
			Returns true if the frame has to be updated*/
		bool setFormula(QString formula, bool undoable=true);
		QString formula() const { return formulaText; }
		
		/* Runs the external application and sets internal vars and loads
		the image.*/
		void runApplication();
		void rerunApplication(bool updateDisplay=false);
		
		QString configFile() const;
		void setConfigFile(QString newConfig);
		QString application() const;
		bool usePreamble() const { return m_usePreamble; }
		int dpi() const { return m_dpi; }
		int realDpi() const;
		
		const QString output() const { return appStdout; }
		QProcess::ProcessState state() const { return latex->state(); }
		int error() const { return err; }

		QMap<QString,QString> editorProperties;
	protected:
		virtual void DrawObj_Item(ScPainter *p, QRectF e, double sc);
		double lastWidth, lastHeight;
		
		QString formulaText;
		
		void writeEditorFile();
		void writeFileContents(QFile *tempfile);
		void deleteImageFile();
		void loadEditorFile();
		/* Last error code */
		int err;
		int m_dpi;
		
		QString imageFile, editorFile, tempFileBase;
		QString appStdout;
		QString configFilename;
		
		QProcess *latex, *editor;
		LatexEditor *internalEditor;
		FileWatcher *fileWatcher;
		LatexConfigParser *config;
		bool imgValid;
		bool m_usePreamble;
		bool killed;
	signals:
		void formulaAutoUpdate(QString oldText, QString newText);
		void latexFinished();
		void stateChanged(QProcess::ProcessState state);
		void applicationChanged();
	protected slots:
		void updateImage(int exitCode, QProcess::ExitStatus exitStatus);
		void editorFinished(int exitCode, QProcess::ExitStatus exitStatus);
		void editorFileChanged(QString filename);
		void editorError(QProcess::ProcessError error);
		void latexError(QProcess::ProcessError error);
	public slots:
		void killProcess();
		void setDpi(int dpi);
		void setUsePreamble(bool);
};

#endif
