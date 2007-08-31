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

#include <QDebug>
#include <QTemporaryFile>

#include "prefsmanager.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "undostate.h"
#include "filewatcher.h"

PageItem_LatexFrame::PageItem_LatexFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
		: PageItem_ImageFrame(pa, x, y, w, h, w2, fill, outline)
{
	setUPixmap(Um::IImageFrame);
	AnName = tr("Latex") + QString::number(m_Doc->TotalItems);
	
	imgValid = false;
	err = 0;
	
	latex = new QProcess();
	connect(latex, SIGNAL(finished(int, QProcess::ExitStatus)), 
		this, SLOT(updateImage(int, QProcess::ExitStatus)));
	connect(latex, SIGNAL(error(QProcess::ProcessError)), 
		this, SLOT(latexError(QProcess::ProcessError)));
	QTemporaryFile *tempfile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_latex_XXXXXX");
	tempfile->open();
	tempFileBase = tempfile->fileName();
	//TODO: Avoid races with file creation!
	tempfile->close();
	delete tempfile;
	qDebug() << "Base" << tempFileBase;
	editor = new QProcess();
	connect(editor, SIGNAL(finished(int, QProcess::ExitStatus)), 
		this, SLOT(editorFinished(int, QProcess::ExitStatus)));
	connect(editor, SIGNAL(error(QProcess::ProcessError)), 
		this, SLOT(editorError(QProcess::ProcessError)));	

	fileWatcher = new FileWatcher(this);
	fileWatcher->stop();
	fileWatcher->setTimeOut(1500);
	
	dpi = PrefsManager::instance()->latexResolution();
	pixm.imgInfo.lowResType = 0;
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
	delete latex;
}

void PageItem_LatexFrame::clearContents()
{
	PageItem_ImageFrame::clearContents();
	formulaText = "";
	err = 0;
	imgValid = false;
}

void PageItem_LatexFrame::deleteImageFile()
{
	if (ImageFile.isEmpty()) {
		return;
	}
	
	QFileInfo fi(tempFileBase);
	QDir dir = fi.absoluteDir();
	QStringList filter;
	
	filter << fi.fileName() + "*";
	Q_ASSERT(!fi.fileName().isEmpty());
	Q_ASSERT(!fi.fileName().contains("/"));
	Q_ASSERT(!fi.fileName().contains("\\"));
	qDebug() << "Filter: " << filter << dir.absolutePath();
	QStringList files;
	files = dir.entryList(filter);
	foreach (QString file, files) {
		Q_ASSERT(file.startsWith("scribus_temp"));
		qDebug() << "LATEX: Deleting " << file << 
			" (please check that this file is correct!)";
		//TODO dir.remove(file);
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
	err = exitCode;
	static bool firstWarning = true;
	if (exitCode) {
		imgValid = false;
		if (firstWarning)
		{
			QMessageBox::critical(0, tr("Error"), "<qt>" + 
								  tr("Running the external application failed!") + "</qt>", 1, 0, 0);
			firstWarning = false;
		}
		else
		{
			qDebug() << "LATEX:" << tr("Running the external application failed!");
		}
		qDebug() << "LATEX: Stdout was: " << qPrintable(appStdout);
		qDebug() << "LATEX: Stderr was: " << qPrintable(appStderr);
		doc()->view()->RefreshItem(this); //Show error marker
		return;
	}
	else
	{
		firstWarning = true;
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
	PageItem_ImageFrame::loadImage(ImageFile, true, dpi);
	if (PrefsManager::instance()->latexForceDpi()) 
	{
		pixm.imgInfo.xres = pixm.imgInfo.yres = dpi;
	}
	
	if (update) 
	{
		//Restoring parameters
		LocalScX = scaleX / pixm.imgInfo.xres; //Account for dpi changes!
		LocalScY = scaleY / pixm.imgInfo.yres;
		LocalX   = offX   * pixm.imgInfo.xres;
		LocalY   = offY   * pixm.imgInfo.yres;
	} 
	else 
	{
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
	
	static bool firstWarningTmpfile = true;
	static bool firstWarningLatexMissing = true;
	
	QFile tempfile(tempFileBase);
	if (!tempfile.open(QIODevice::Truncate|QIODevice::WriteOnly)) {
		err = 0xffff;
		if (firstWarningTmpfile)
		{
			QMessageBox::critical(0, tr("Error"), "<qt>" +
								  tr("Could not create a temporary file to run the application!") 
								  + "</qt>", 1, 0, 0);
			firstWarningTmpfile = false;
		}
		else {
			qDebug() << "LATEX:" << tr("Could not create a temporary file to run the application!");
		}
		//Don't know how to continue as it's impossible to create tempfile
		return;
	}
	else
	{
		firstWarningTmpfile = true;
	}
	
	QString full_command = PrefsManager::instance()->latexExecutable().replace("%dpi", QString::number(dpi));
	
	if (full_command.isEmpty()) {
		if (firstWarningLatexMissing)
		{
			QMessageBox::information(0, tr("Information"),
									 "<qt>" + tr("Please specify a latex executable in the preferences!") +
									 "</qt>",1, 0, 0);
			firstWarningLatexMissing = false;
		}
		else
		{
			qDebug() << "LATEX:" << tr("Please specify a latex executable in the preferences!");
		}
		return;
	}
	else
	{
		firstWarningLatexMissing = true;
	}
	
	if (full_command.contains("%file")) {
		full_command.replace("%file", tempFileBase);
	} else {
		full_command += " " + tempFileBase;
	}
	if (full_command.contains("%dir")) {
		full_command.replace("%dir", QDir::tempPath());
	} else {
		latex->setWorkingDirectory(QDir::tempPath());
	}
	qDebug() << "Full command" << full_command;
	
	QStringList args = full_command.split(' ', QString::SkipEmptyParts); //TODO: This does not handle quoted arguments with spaces! Perhaps use the other QProcess::start-function, but that has problems on windows.
	QString command = args.at(0);
	args.removeAt(0);
	
	deleteImageFile();
	ImageFile = tempFileBase + PrefsManager::instance()->latexExtension();

	writeFileContents(&tempfile);
	tempfile.close();
	
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
	
	writeEditorFile(); //This must be at this position, because it sets editorFile
	
	if (full_command.contains("%file")) {
		full_command.replace("%file", editorFile);
	} else {
		full_command += " " + editorFile;
	}
	if (full_command.contains("%dir")) {
		full_command.replace("%dir", QDir::tempPath());
	} else {
		editor->setWorkingDirectory(QDir::tempPath());
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
"\\section*{Manual}\n"
"Your latex-frames setup is working when you can read this text!\\\\\n"
"Placing formulas is very easy:\\\\\nRight click $\\Rightarrow$ Edit Latex Source\\\\\n"
"And replace this text with your own. Here is an example:\n"
"\\begin{verbatim}\\[J = \\int r^2 \\mathrm{d}m\\]\\end{verbatim}\n"
"becomes\n"
"\\[J = \\int r^2 \\mathrm{d}m\\]\n"
"Some scribus values:\\\\\n"
"\\$scribus\\_width\\$ $\\Rightarrow$ $scribus_width$ pt\\\\\n"
"\\$scribus\\_height\\$ $\\Rightarrow$ $scribus_height$ pt\\\\\n"
"\\$scribus\\_dpi\\$ $\\Rightarrow$ $scribus_dpi$ dpi\\\\\n";

void PageItem_LatexFrame::writeFileContents(QFile *tempfile)
{
	if (formulaText.isEmpty()) {
		formulaText = demofile;
	}
	QString tmp(formulaText);
	double scaleX, scaleY, realW, realH, offsetX, offsetY;
	static bool firstrun = true;
	if (firstrun) {
		//TODO: I don't know why, but the values are wrong the first time
		scaleX = scaleY = 1;
		offsetX = offsetY = 0;
		realW = Width;
		realH = Height;
	} else {
		scaleX = LocalScX/72.0*dpi;
		scaleY = LocalScY/72.0*dpi;
		offsetX = LocalX*LocalScX;
		offsetY = LocalY*LocalScY;
		realW = Width/scaleX - LocalX*72.0/dpi;
		realH = Height/scaleY - LocalY*72.0/dpi;
	}
	if (!tmp.contains("$scribus_noprepost$")) {
		tmp = PrefsManager::instance()->latexPre() + tmp + PrefsManager::instance()->latexPost();
	}
	tmp.replace(QString("$scribus_width$"), QString::number(Width));
	tmp.replace(QString("$scribus_height$"), QString::number(Height));
	tmp.replace(QString("$scribus_realwidth$"), QString::number(realW));
	tmp.replace(QString("$scribus_realheight$"), QString::number(realH));
	tmp.replace(QString("$scribus_offsetX$"), QString::number(offsetX));
	tmp.replace(QString("$scribus_offsetY$"), QString::number(offsetY));
	tmp.replace(QString("$scribus_scaleX$"), QString::number(scaleX));
	tmp.replace(QString("$scribus_scaleY$"), QString::number(scaleY));
	tmp.replace(QString("$scribus_dpi$"), QString::number(dpi));
	tempfile->write(tmp.toUtf8());
}

void PageItem_LatexFrame::writeEditorFile()
{
	fileWatcher->stop();
	fileWatcher->disconnect(); //Avoid triggering false updates
	//First create a temp file name
	if (editorFile.isEmpty()) {
		QTemporaryFile *editortempfile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_editor_XXXXXX");
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
	f.write(formulaText.toUtf8());
	f.close();
	fileWatcher->forceScan();
	connect(fileWatcher, SIGNAL(fileChanged(QString)),
		this, SLOT(editorFileChanged(QString)));
	fileWatcher->start();
}
	
void PageItem_LatexFrame::setFormula(QString formula, bool undoable)
{
	qDebug() << "LATEX: setFormula()";
	imgValid = false;
	err = 0;
	if (UndoManager::undoEnabled() && undoable)
	{
		SimpleState *ss = new SimpleState(Um::ChangeFormula, "", Um::IChangeFormula);
		ss->set("CHANGE_FORMULA", "change_formula");
		ss->set("OLD_FORMULA", formulaText);
		ss->set("NEW_FORMULA", formula);
		undoManager->action(this, ss);
	}
	formulaText = formula;
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
	static bool firstWarning = true;
	if (firstWarning)
	{
		QMessageBox::critical(0, tr("Error"), "<qt>" +
							  tr("Running the application \"%1\" failed!").
							  arg(PrefsManager::instance()->latexExecutable()) + "</qt>", 1, 0, 0);
		firstWarning = false;
	}
	else
	{
		qDebug() << "LATEX:" << tr("Running the application \"%1\" failed!").arg(PrefsManager::instance()->latexExecutable());
	}
}


QString PageItem_LatexFrame::getApplication()
{
	return PrefsManager::instance()->latexExecutable();
}


int PageItem_LatexFrame::getDpi()
{
	return PrefsManager::instance()->latexResolution();
}

void PageItem_LatexFrame::restore(UndoState *state, bool isUndo)
{
	SimpleState *ss = dynamic_cast<SimpleState*>(state);
	if (!ss) {
		PageItem_ImageFrame::restore(state, isUndo);
		return;
	}
	if (ss->contains("CHANGE_FORMULA")) {
		if (isUndo) {
			setFormula(ss->get("OLD_FORMULA"), false);
		} else {
			setFormula(ss->get("NEW_FORMULA"), false);
		}
	} else {
		PageItem_ImageFrame::restore(state, isUndo);
	}
}

const QString PageItem_LatexFrame::defaultApp = "pdflatex  --interaction nonstopmode";
const QString PageItem_LatexFrame::defaultPre = 
		"\\documentclass[a4paper,10pt]{article}\n"
		"\\usepackage[utf8]{inputenc}\n"
		"\\usepackage[left=0cm,top=0cm,right=0cm,bottom=0cm,nohead,nofoot]{geometry}\n"
		"\\title{Scribus-Latex-File}\n"
		"\\author{Scribus}\n"
		"\\pagestyle{empty}\n"
		"\\setlength{\\textwidth}{$scribus_realwidth$ pt}\n"
		"\\begin{document}\n";
const QString PageItem_LatexFrame::defaultPost = "\\end{document}";
