/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						pageitem.cpp  -  description
						-------------------
	begin                : Mon May 28 2007
	copyright            : (C) 2007 by Hermann Kraus
	email                : hermann@physiklaborant.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include "pageitem_latexframe.h"
#include <qpainter.h>
#include <qpen.h>
#include <qfont.h>
#include <qregion.h>
#include <qpoint.h>
#include <qfileinfo.h>
#include <qdrawutil.h>
#include <qbitmap.h>
#include <qregexp.h>
#include <qmessagebox.h>
#include <qprocess.h>
//Added by qt3to4:
#include <QKeyEvent>
#include <cmath>
#include <cassert>

#include "mpalette.h"
#include "page.h"
#include "pageitem.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribusstructs.h"
#include "scribusdoc.h"
#include "commonstrings.h"
#include "undomanager.h"
#include "undostate.h"
#include "scconfig.h"

#include "util.h"

PageItem_LatexFrame::PageItem_LatexFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
		: PageItem_ImageFrame(pa, x, y, w, h, w2, fill, outline)
{
	imgValid = false;
	err = 0;
	
	latex = new QProcess();
	connect(latex, SIGNAL(finished(int, QProcess::ExitStatus)), 
		this, SLOT(updateImage(int, QProcess::ExitStatus)));
	connect(latex, SIGNAL(error(QProcess::ProcessError)), 
		this, SLOT(latexError(QProcess::ProcessError)));
	tempfile = new QTemporaryFile("scribus_temp_file_XXXXXX");

	editor = new QProcess();
	connect(editor, SIGNAL(finished(int, QProcess::ExitStatus)), 
		this, SLOT(editorFinished(int, QProcess::ExitStatus)));
	connect(editor, SIGNAL(error(QProcess::ProcessError)), 
		this, SLOT(editorError(QProcess::ProcessError)));	

	fileWatcher = new FileWatcher(this);
	fileWatcher->stop();
	fileWatcher->setTimeOut(1500);
	
	dpi = PrefsManager::instance()->latexResolution();
}

PageItem_LatexFrame::~PageItem_LatexFrame()
{
	//IMPORTANT: Make sure no signals are emitted which
	// would cause crashes because the handlers access undefined memory.
	fileWatcher->disconnect();
	delete fileWatcher;
	editor->disconnect();
	//No need to kill the editor TODO: Make sure editor continues running
	delete editor;
	QDir dir;
	if (!editorFile.isEmpty() && !dir.remove(editorFile)) {
		qDebug() << "LATEX: Failed to remove editorfile" << qPrintable(editorFile);
	}
	latex->disconnect();
	if (latex->state() != QProcess::NotRunning) {
		qDebug() << "LATEX: Destructor: Killing running process from latexFrame";
		latex->terminate();
		latex->waitForFinished(500);
		if (latex->state() != QProcess::NotRunning) {
			latex->kill();
			latex->waitForFinished(500);
		}
	}
	deleteImageFile();
	delete tempfile;
	delete latex;
}

void PageItem_LatexFrame::clearContents()
{
	PageItem_ImageFrame::clearContents();
	formula_text = "";
	err = 0;
	imgValid = false;
}

void PageItem_LatexFrame::deleteImageFile()
{
	QDir dir;
	if (ImageFile.isEmpty()) {
		return;
	}
	
	if (!dir.remove(ImageFile)) {
		qDebug() << "LATEX: Failed to remove imagefile" << qPrintable(ImageFile);
	}
	imgValid = false;
}

void PageItem_LatexFrame::DrawObj_Item(ScPainter *p, QRect e, double sc)
{
		if (!imgValid && !err) {
			if (latex->state() == QProcess::NotRunning) {
				//We need to create the image
				runApplication();
			}
			//Draw indicator that latex is running
			p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->drawLine(FPoint(0, 0), FPoint(Width, Height));
		} else if (err) {
			//Draw error indicator
			//TODO: blue is only used to make it distinct from missing images
			p->setPen(Qt::blue, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
			p->drawLine(FPoint(0, 0), FPoint(Width, Height));
			p->drawLine(FPoint(0, Height), FPoint(Width, 0));
		} else {
			//Just pass it to ImageFrame
			PageItem_ImageFrame::DrawObj_Item(p, e, sc);
		}
}

void PageItem_LatexFrame::updateImage(int exitCode, QProcess::ExitStatus exitStatus)
{
	appStdout = latex->readAllStandardOutput();
	appStderr = latex->readAllStandardError();
	qDebug() << "LATEX: latex finished with exit code: " << exitCode;
	tempfile->close(); //Close AND delete tempfile
	err = exitCode;
	if (exitCode) {
		imgValid = false;
		QMessageBox::critical(0, tr("Error"), "<qt>" + 
			tr("Running the external application failed!") + "</qt>", 1, 0, 0);
			
		qDebug() << "LATEX: Stdout was: " << qPrintable(appStdout);
		qDebug() << "LATEX: Stderr was: " << qPrintable(appStderr);
		doc()->view()->RefreshItem(this); //Show error marker
		return;
	}
	imgValid = true;

	qDebug() << "LATEX: loading image file:" << qPrintable(ImageFile);
	
	//Save state and restore afterwards
	bool update = PicAvail;
	double scaleX = 72.0, scaleY = 72.0, offX = 0.0, offY = 0.0;
	if (update) {
		scaleX = LocalScX * pixm.imgInfo.xres;
		scaleY = LocalScY * pixm.imgInfo.yres;
		offX   = LocalX   / pixm.imgInfo.xres;
		offY   = LocalY   / pixm.imgInfo.yres;
	}
	PageItem_ImageFrame::loadImage(ImageFile, true);
	//NOTE: QT seems to assume 96x96 dpi if no information is available
	// so I replace the probably wrong value with our setting
	//TODO:
	if (PrefsManager::instance()->latexForceDpi()) {
	pixm.imgInfo.xres = pixm.imgInfo.yres = dpi;
	}
	
	if (update) {
		//Restoring parameters
		LocalScX = scaleX / pixm.imgInfo.xres; //Account for dpi changes!
		LocalScY = scaleY / pixm.imgInfo.yres;
		LocalX   = offX   * pixm.imgInfo.xres;
		LocalY   = offY   * pixm.imgInfo.yres;
	} else {
		//Setting sane defaults
		LocalX = LocalY = 0;
		//NOTE: Originally this was 72.0/dpi however postscript and pdf use
		// the ghostscript dpi setting. So this is (in combination with the
		// fix above) a better solution.
		LocalScX = 72.0/pixm.imgInfo.xres;
		LocalScY = 72.0/pixm.imgInfo.yres;
	}
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY );
	
	doc()->view()->RefreshItem(this);
}


void PageItem_LatexFrame::runApplication()
{
	imgValid = false;
	err = 0;
	
	dpi = PrefsManager::instance()->latexResolution();
	
	//TODO: Make sure we start with an empty file!
	//tempfile->remove();
	if (!tempfile->open()) {
		err = 0xffff;
		QMessageBox::critical(0, tr("Error"), "<qt>" +
			tr("Could not create a temporary file to run the application!") 
			+ "</qt>", 1, 0, 0);
		//Don't know how to continue as it's impossible to create tempfile
		return;
	}
	
	QString full_command = PrefsManager::instance()->latexExecutable().replace("%dpi", QString::number(dpi));
	
	if (full_command.isEmpty()) {
		QMessageBox::information(0, tr("Information"),
		"<qt>" + tr("Please specify a latex executable in the preferences!") +
		"</qt>",1, 0, 0);
		return;
	}
	
	if (full_command.indexOf("%file")>=0) {
		full_command.replace("%file", tempfile->fileName());
	} else {
		full_command += " " + tempfile->fileName();
	}
	qDebug() << "Full command" << full_command;
	
	QStringList args = full_command.split(' ', QString::SkipEmptyParts); //TODO: This does not handle quoted arguments with spaces! Perhaps use the other QProcess::start-function, but that has problems on windows.
	QString command = args.at(0);
	args.removeAt(0);
	
	deleteImageFile();
	ImageFile = tempfile->fileName() + PrefsManager::instance()->latexExtension();

	writeFileContents(tempfile);
	
	qDebug() << "LATEX: Running app " << command << 
		" with file" << tempfile->fileName().latin1();
	latex->start(command, args);
}

void PageItem_LatexFrame::runEditor()
{
	if (editor->state() != QProcess::NotRunning) {
		QMessageBox::information(0, tr("Information"),
		"<qt>" + tr("An editor for this frame is already running!") +
		"</qt>", 1, 0, 0);
		return;
	}
	
	QString full_command = PrefsManager::instance()->latexEditorExecutable();
	if (full_command.isEmpty()) {
		QMessageBox::information(0, tr("Information"),
		"<qt>" + tr("Please specify an editor in the preferences!") +
		"</qt>",1, 0, 0);
		return;
	}
	
	writeEditorFile(); //This must be here, because it sets editorFile
	
	if (full_command.indexOf("%file")>=0) {
		full_command.replace("%file", editorFile);
	} else {
		full_command += " " + editorFile;
	}
	
	QStringList args = full_command.split(' ', QString::SkipEmptyParts); //TODO: This does not handle quoted arguments with spaces!
	QString command = args.at(0);
	args.removeAt(0);
	//args << editorFile;
	
	qDebug() << "LATEX: Starting editor" << qPrintable(command);
	editor->start(command, args);
}

void PageItem_LatexFrame::rerunApplication()
{
	qDebug() << "LATEX: rerunApplication()";
	if (latex->state() != QProcess::NotRunning) {
		qDebug() << "LATEX: rerunApplication(): Killing running process from latexFrame";
		latex->terminate();
		latex->waitForFinished(500);
		if (latex->state() != QProcess::NotRunning) {
			//Still not terminated?
			latex->kill();
			latex->waitForFinished(500);
		}
	}
	runApplication();
}
	
	

char demofile[] = 
"\\documentclass[a4paper,10pt]{article}\n"
"\\usepackage[utf8]{inputenc}\n"
"\\usepackage[left=0cm,top=0cm,right=0cm,bottom=0cm,nohead,nofoot]{geometry}\n"
"\\title{Scribus-Test-File}\n"
"\\author{Hermann Kraus}\n"
"\\pagestyle{empty}\n"
"\\begin{document}\n"
"\\section*{Scribus-Test}\n"
"Das ist ein Test $E=mc^2$\n"
"\\end{document}\n";

void PageItem_LatexFrame::writeFileContents(QFile *tempfile)
{
	if (formula_text.isEmpty()) {
		formula_text = demofile;
	}
	qDebug() << "LATEX: Writing temporary file. size: " << formula_text.size();
	tempfile->write(formula_text.toUtf8());
	//TODO Does this work on windows or do I have to close the file first
	if (!tempfile->flush()) {
		qDebug() << "LATEX: Can't flush. Program will likely fail!";
	}
}

void PageItem_LatexFrame::writeEditorFile()
{
	fileWatcher->stop();
	fileWatcher->disconnect(); //Avoid 
	//First create a temp file name
	if (editorFile.isEmpty()) {
		QTemporaryFile *editortempfile = new QTemporaryFile("scribus_editor_XXXXXX");
		if (!editortempfile->open()) {
			QMessageBox::critical(0, tr("Error"), "<qt>" + 
				tr("Could not create a temporary file to run the external editor!") 
				+ "</qt>", 1, 0, 0);
		}
		editorFile = editortempfile->fileName();
		editortempfile->setAutoRemove(false);
		editortempfile->close();
		delete editortempfile;
		fileWatcher->addFile(editorFile);
	}
	QFile f(editorFile);
	f.open(QIODevice::WriteOnly);
	writeFileContents(&f);
	f.close();
	fileWatcher->forceScan();
	connect(fileWatcher, SIGNAL(fileChanged(QString)),
		this, SLOT(editorFileChanged(QString)));
	fileWatcher->start();
}
	
void PageItem_LatexFrame::setFormula(QString &formula)
{
	qDebug() << "LATEX: setFormula()";
	imgValid = false;
	err = 0;
	formula_text = formula;
	//Stop any running process to avoid overwriting the image again
	//TODO: move this to a common place
	if (latex->state() != QProcess::NotRunning) {
		qDebug() << "LATEX: setFormula(): Killing running process";
		latex->terminate();
		latex->waitForFinished(500);
		if (latex->state() != QProcess::NotRunning) {
			latex->kill();
			latex->waitForFinished(500);
		}
	}
}


void PageItem_LatexFrame::loadEditorFile()
{
	QString new_formula;
	QFile f(editorFile);
	f.open(QIODevice::ReadOnly);
	new_formula = QString::fromUtf8(f.readAll());
	f.close();
	if (!new_formula.isEmpty()) {
		setFormula(new_formula);
	}
}


void PageItem_LatexFrame::editorFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_ASSERT(editor);
	qDebug() << "LATEX: editor exited with exit code: " << exitCode;
	
	if (exitCode) {
		qDebug() << "-------\nLATEX: Editor's stdout was: " << 
			qPrintable(QString(editor->readAllStandardOutput()));
		qDebug() << "-------\nLATEX: Editor's stderr was: " << 
			qPrintable(QString(editor->readAllStandardError())) << "\n-------";
		QMessageBox::critical(0, tr("Error"), "<qt>" +
			tr("Running the editor failed with exitcode %d!").arg(exitCode) +
			"</qt>", 1, 0, 0);
		return;
	}
}

void PageItem_LatexFrame::editorFileChanged(QString filename)
{
	qDebug() << "LATEX: editorFileChanged()";
	loadEditorFile();
	rerunApplication();
}

void PageItem_LatexFrame::editorError(QProcess::ProcessError error)
{
	QMessageBox::critical(0, tr("Error"), "<qt>" +
		tr("Running the editor \"%1\" failed!").arg(
			PrefsManager::instance()->latexEditorExecutable()) + "</qt>", 1, 0, 0);
}

void PageItem_LatexFrame::latexError(QProcess::ProcessError error)
{
	QMessageBox::critical(0, tr("Error"), "<qt>" +
		tr("Running the application \"%1\" failed!").arg(
			PrefsManager::instance()->latexExecutable()) + "</qt>", 1, 0, 0);
}
