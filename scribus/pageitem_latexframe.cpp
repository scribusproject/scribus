/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						pageitem_latexframe.cpp  -  description
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
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPoint>
#include <QXmlStreamReader>

#include "prefsmanager.h"
#include "scpainter.h"
#include "scraction.h"
#include "scribus.h"
#include "scribusdoc.h"
#include "undomanager.h"
#include "undostate.h"
#include "filewatcher.h"
#include "latexeditor.h"



/* Local helper class */
class LatexConfigParser
{
	public:
		LatexConfigParser() {};
		bool parseConfigFile(QString fn);
		QString executable() const { return m_executable; }
		QString imageExtension() const { return m_imageExtension; }
		/** Returns the path to the highlighter definition.
		 * The different options are handled in this function
		 * so the caller doesn't have to care about.
		 */
		QString highlighter() const; //TODO
		QString emptyFrameText() const { return m_emptyFrameText; }
		QString preamble() const { return m_preamble; }
		QString postamble() const { return m_postamble; }
		QString description() const { return m_description; }
		QString error() const { return m_error; }
		QMap<QString,QString> properties;
	protected:
		QString m_error;
		QString m_description, m_executable, m_imageExtension, m_emptyFrameText,
   			m_preamble, m_postamble, m_highlight_kate, m_highlight_file;
		QString filename;
		QXmlStreamReader xml;
		void formatError(QString message);
		void parseElements();
		void parseTab();
		void ignoreList();
};

PageItem_LatexFrame::PageItem_LatexFrame(ScribusDoc *pa, double x, double y, double w, double h, double w2, QString fill, QString outline)
		: PageItem_ImageFrame(pa, x, y, w, h, w2, fill, outline)
{
	setUPixmap(Um::IImageFrame);
	AnName = tr("Latex") + QString::number(m_Doc->TotalItems);
	
	imgValid = false;
	usePreamble = true;
	err = 0;
	internalEditor = 0;
	killed = false;
	
	config = new LatexConfigParser();
	if (!config->parseConfigFile(PrefsManager::instance()->latexEditorConfig())) {
		QMessageBox::critical(0, tr("Error"), "<qt>" + 
				tr("Parsing the configfile %1 failed! Depending on the type of the error "
						"latexframes might not work correctly!\n%2").arg(
						PrefsManager::instance()->latexEditorConfig(),
						config->error())
						+ "</qt>", 1, 0, 0);
	}
	editorProperties = config->properties; //Initialize with default values
	
	if (PrefsManager::instance()->latexStartWithEmptyFrames()) {
		formulaText = "";
	} else {
		formulaText = config->emptyFrameText();
	}
	
	
	
	latex = new QProcess();
	connect(latex, SIGNAL(finished(int, QProcess::ExitStatus)),
		this, SLOT(updateImage(int, QProcess::ExitStatus)));
	connect(latex, SIGNAL(error(QProcess::ProcessError)),
		this, SLOT(latexError(QProcess::ProcessError)));
	latex->setProcessChannelMode(QProcess::MergedChannels);
	
	
	
	QTemporaryFile *tempfile = new QTemporaryFile(QDir::tempPath() + "/scribus_temp_latex_XXXXXX");
	tempfile->open();
	tempFileBase = tempfile->fileName();
	tempfile->close();
	delete tempfile;
	Q_ASSERT(!tempFileBase.isEmpty());
	qDebug() << "LATEX: Base" << tempFileBase;
	editor = new QProcess();
	connect(editor, SIGNAL(finished(int, QProcess::ExitStatus)), 
		this, SLOT(editorFinished(int, QProcess::ExitStatus)));
	connect(editor, SIGNAL(error(QProcess::ProcessError)), 
		this, SLOT(editorError(QProcess::ProcessError)));	
	editor->setProcessChannelMode(QProcess::MergedChannels);
	
	fileWatcher = new FileWatcher(this);
	fileWatcher->stop();
	fileWatcher->setTimeOut(1500);
	
	dpi = 0;
	pixm.imgInfo.lowResType = 0;
}

PageItem_LatexFrame::~PageItem_LatexFrame()
{
	if (internalEditor) delete internalEditor;
	
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
		killed = true;
		latex->terminate();
		latex->waitForFinished(500);
		if (latex->state() != QProcess::NotRunning) {
			latex->kill();
			latex->waitForFinished(500);
		}
	}
	deleteImageFile();
	delete latex;
	delete config;
}

void PageItem_LatexFrame::clearContents()
{
	PageItem_ImageFrame::clearContents();
	formulaText = "";
	appStdout = "";
	err = 0;
	imgValid = false;
}

void PageItem_LatexFrame::deleteImageFile()
{
	QFileInfo fi(tempFileBase);
	QDir dir = fi.absoluteDir();
	QStringList filter;
	
	filter << fi.fileName() + "*";
	Q_ASSERT(!fi.fileName().isEmpty());
	Q_ASSERT(!fi.fileName().contains("/"));
	Q_ASSERT(!fi.fileName().contains("\\"));
	QStringList files;
	files = dir.entryList(filter);
	foreach (QString file, files) {
		Q_ASSERT(file.startsWith("scribus_temp"));
		qDebug() << "LATEX: Deleting " << file;
		dir.remove(file);
	}
	imgValid = false;
}

void PageItem_LatexFrame::DrawObj_Item(ScPainter *p, QRectF e, double sc)
{
	layout();
	if (!imgValid && !err) {
		//Draw indicator that latex is running
		p->setPen(Qt::green, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->drawLine(FPoint(0, 0), FPoint(Width, Height));
	} else if (err) {
		//Draw error indicator
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
	err = exitCode;
	
	emit latexFinished();
	emit stateChanged(latex->state());
	
	static bool firstWarning = true;
	if (exitCode) {
		imgValid = false;
		if (firstWarning && !killed)
		{
			QMessageBox::critical(0, tr("Error"), "<qt>" + 
				tr("Running the external application failed!")
				+ "</qt>", 1, 0, 0);
			firstWarning = false;
		}
		qDebug() << "LATEX: updateImage():" << tr("Running the external application failed!");
		killed = false;
		update(); //Show error marker
		return;
	}
	else
	{
		firstWarning = true;
	}
	imgValid = true;

	//Save state and restore afterwards
	bool do_update = PicAvail;
	double scaleX = 72.0, scaleY = 72.0, offX = 0.0, offY = 0.0;
	if (do_update) {
		scaleX = LocalScX * pixm.imgInfo.xres;
		scaleY = LocalScY * pixm.imgInfo.yres;
		offX   = LocalX   / pixm.imgInfo.xres;
		offY   = LocalY   / pixm.imgInfo.yres;
	}
	PageItem_ImageFrame::loadImage(imageFile, true, getRealDpi());
	if (PrefsManager::instance()->latexForceDpi()) 
	{
		pixm.imgInfo.xres = pixm.imgInfo.yres = getRealDpi();
	}
	
	if (do_update) 
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
		LocalScX = 72.0/pixm.imgInfo.xres;
		LocalScY = 72.0/pixm.imgInfo.yres;
	}
	emit imageOffsetScale(LocalScX, LocalScY, LocalX, LocalY );
	
	update();
}


void PageItem_LatexFrame::runApplication()
{
	imgValid = false;
	err = 0;
	killed = false;
	
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
		qDebug() << "LATEX:" << tr("Could not create a temporary file to run the application!");
		//Don't know how to continue as it's impossible to create tempfile
		return;
	}
	else
	{
		firstWarningTmpfile = true;
	}
	
	QString full_command = config->executable().replace(
			"%dpi", QString::number(getRealDpi()));
	
	if (full_command.isEmpty()) {
		if (firstWarningLatexMissing)
		{
			QMessageBox::information(0, tr("Information"),
									 "<qt>" + tr("Please specify a latex executable in the preferences!") +
									 "</qt>",1, 0, 0);
			firstWarningLatexMissing = false;
		}
		qDebug() << "LATEX:" << tr("Please specify a latex executable in the preferences!");
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
	}
	latex->setWorkingDirectory(QDir::tempPath());
	
	qDebug() << "Full command" << full_command;
	
	QStringList args = full_command.split(' ', QString::SkipEmptyParts); //TODO: This does not handle quoted arguments with spaces! Perhaps use the other QProcess::start-function, but that has problems on windows.
	QString command = args.at(0);
	args.removeAt(0);
	
	//deleteImageFile();
	imageFile = tempFileBase + config->imageExtension();

	writeFileContents(&tempfile);
	tempfile.close();
	
	latex->start(command, args);
	emit stateChanged(QProcess::Starting);
}

void PageItem_LatexFrame::runEditor()
{
	if (PrefsManager::instance()->latexUseEmbeddedEditor()) {
		if (!internalEditor) {
			internalEditor = new LatexEditor(this);
		}
		internalEditor->startEditor();
		return;
	}
	
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
	}
	editor->setWorkingDirectory(QDir::tempPath());
	
	QStringList args = full_command.split(' ', QString::SkipEmptyParts); //TODO: This does not handle quoted arguments with spaces!
	QString command = args.at(0);
	args.removeAt(0);
	
	qDebug() << "LATEX: Starting editor" << qPrintable(command);
	editor->start(command, args);
}

void PageItem_LatexFrame::rerunApplication(bool updateDisplay)
{
	qDebug() << "LATEX: rerunApplication()";
	if (latex->state() != QProcess::NotRunning) {
		//TODO: Do not flag this as an error!
		qDebug() << "LATEX: rerunApplication(): Killing running process from latexFrame";
		killed = true;
		latex->terminate();
		latex->waitForFinished(500);
		if (latex->state() != QProcess::NotRunning) {
			//Still not terminated?
			latex->kill();
			latex->waitForFinished(500);
		}
	}
	runApplication();
	if (updateDisplay) this->update();
}


void PageItem_LatexFrame::writeFileContents(QFile *tempfile)
{
	QString tmp(formulaText);
	double scaleX, scaleY, realW, realH, offsetX, offsetY;
	double lDpi = getRealDpi()/72.0;
	scaleX = LocalScX*lDpi;
	scaleY = LocalScY*lDpi;
	offsetX = LocalX*LocalScX;
	offsetY = LocalY*LocalScY;
	realW = Width/scaleX - LocalX/lDpi;
	realH = Height/scaleY - LocalY/lDpi;
	if (!tmp.contains("$scribus_noprepost$") && usePreamble) {
		tmp = config->preamble() + tmp + config->postamble();
	}
	tmp.replace(QString("$scribus_width$"), QString::number(Width));
	tmp.replace(QString("$scribus_width_px$"), QString::number(round(Width*lDpi)));
	tmp.replace(QString("$scribus_height$"), QString::number(Height));
	tmp.replace(QString("$scribus_height_px$"), QString::number(round(Height*lDpi)));
	tmp.replace(QString("$scribus_realwidth$"), QString::number(realW));
	tmp.replace(QString("$scribus_realwidth_px$"), QString::number(round(realW*lDpi)));
	tmp.replace(QString("$scribus_realheight$"), QString::number(realH));
	tmp.replace(QString("$scribus_realheight_px$"), QString::number(round(realH*lDpi)));
	tmp.replace(QString("$scribus_offsetX$"), QString::number(offsetX));
	tmp.replace(QString("$scribus_offsetX_px$"), QString::number(round(offsetX*lDpi)));
	tmp.replace(QString("$scribus_offsetY$"), QString::number(offsetY));
	tmp.replace(QString("$scribus_offsetY$"), QString::number(round(offsetY*lDpi)));
	tmp.replace(QString("$scribus_scaleX$"), QString::number(scaleX));
	tmp.replace(QString("$scribus_scaleY$"), QString::number(scaleY));
	tmp.replace(QString("$scribus_dpi$"), QString::number(getRealDpi()));
	tmp.replace(QString("$scribus_file$"), tempFileBase);
	tmp.replace(QString("$scribus_dir$"), QDir::tempPath());
	QMapIterator<QString, QString> i(editorProperties);
	while (i.hasNext()) {
		i.next();
		tmp.replace("$scribus_"+i.key()+"$", i.value());
	}
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
	
bool PageItem_LatexFrame::setFormula(QString formula, bool undoable)
{
	if (formula == formulaText) {
		//Nothing changed
		return false;
	}
	imgValid = false;
	err = 0;
	if (UndoManager::undoEnabled() && undoable)
	{
		SimpleState *ss = new SimpleState(Um::ChangeFormula, "", Um::IChangeFormula);
		ss->set("CHANGE_FORMULA", "change_formula");
		ss->set("OLD_FORMULA", formulaText);
		ss->set("NEW_FORMULA", formula);
		undoManager->action(this, ss);
	} else {
		emit formulaAutoUpdate(formulaText, formula);
	}
	formulaText = formula;
	return true;
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
	this->update();
}


void PageItem_LatexFrame::editorFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_ASSERT(editor);
	qDebug() << "LATEX: editor exited with exit code: " << exitCode;
	
	if (exitCode) {
		qDebug() << "LATEX: Editor's output was: " << 
			qPrintable(QString(editor->readAllStandardOutput()));
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
		tr("Running the editor \"%1\" failed!").
			arg(PrefsManager::instance()->latexEditorExecutable())+ "</qt>", 1, 0, 0);
}

void PageItem_LatexFrame::latexError(QProcess::ProcessError error)
{
	static bool firstWarning = true;
	if (killed) {
		killed = false;
		update();
		//Don't show errors caused by killing processes
		return; 
	}
	if (firstWarning)
	{
		if (latex->error() == QProcess::FailedToStart) {
			QMessageBox::critical(0, tr("Error"), "<qt>" +
								  tr("The application \"%1\" failed to start!").
								  arg(config->executable())
								  + "</qt>", 1, 0, 0);
		} else {
			QMessageBox::critical(0, tr("Error"), "<qt>" +
					tr("The application \"%1\" crashed!").arg(config->executable())
					+ "</qt>", 1, 0, 0);
		}
		firstWarning = false;
	}
	qDebug() << "LATEX: latexError():" << 
			tr("Running the application \"%1\" failed!").arg(config->executable()) << latex->error();
}


QString PageItem_LatexFrame::getApplication() const
{
	return config->executable();
}

QString PageItem_LatexFrame::getRealApplication() const
{
	return config->executable();
}

int PageItem_LatexFrame::getDpi() const
{
	return dpi;
}

int PageItem_LatexFrame::getRealDpi() const
{
	if (dpi) {
		return dpi;
	} else {
		return PrefsManager::instance()->latexResolution();
	}
}

void PageItem_LatexFrame::setDpi(int newDpi)
{
	dpi = newDpi;
}

void PageItem_LatexFrame::setApplication(QString newApp)
{
	//TODO
}

void PageItem_LatexFrame::killProcess()
{
	killed = true;
	latex->kill();
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


void PageItem_LatexFrame::setUsePreamble(bool useP) 
{
	usePreamble = useP;
}

void PageItem_LatexFrame::layout()
{
	
	qDebug() << "PageItem_LatexFrame::layout() called! Invalid: " << (invalid?"true":"false");
	if (!invalid) return;
	invalid = false;
	
	if (Width == lastWidth && Height == lastHeight) return;
	lastWidth = Width;
	lastHeight = Height;
	
	rerunApplication(false);
}

void PageItem_LatexFrame::applicableActions(QStringList & actionList)
{
	actionList << "itemPreviewFull";
	actionList << "itemPreviewLow";
	actionList << "itemPreviewNormal";
	actionList << "itemUpdateImage";
	actionList << "editEditWithLatexEditor";
	if (PicAvail)
	{
		if (!isTableItem)
			actionList << "itemAdjustFrameToImage";
		actionList << "editClearContents";
		actionList << "editCopyContents";
	}
	//TODO: Type correct?
	if (doc()->scMW()->contentsBuffer.sourceType==PageItem::LatexFrame)
	{
		actionList << "editPasteContents";
		actionList << "editPasteContentsAbs";
	}
}

QString PageItem_LatexFrame::infoDescription()
{
	QString htmlText;
	htmlText.append("<h2>"+tr("Latex") + "</h2><table>");
	htmlText.append("<tr><th align=\"right\">" + tr("Application") + ": </th><td>" + getRealApplication());
	htmlText.append("</td></tr><tr><th align=\"right\">" + tr("DPI") + ": </th><td>" +
			 QString::number(getRealDpi()));
	htmlText.append("</td></tr><tr><th align=\"right\">" + tr("State") + ": </th><td>");
	if (latex->state() == QProcess::NotRunning) {
		if (getError()) {
			htmlText.append(tr("Error"));
		} else {
			htmlText.append(tr("Finished"));
		}
	} else {
		htmlText.append(tr("Running"));
	}
	htmlText.append("</td></tr></table><hr>");
	htmlText.append(PageItem::infoDescription());
	return htmlText;
}

//TODO: Pass this information to LatexEditor, so the second parser can be removed
bool LatexConfigParser::parseConfigFile(QString fn)
{
	m_error = "";
	filename = fn;
	QFile f(fn);
	f.open(QIODevice::ReadOnly);
	xml.setDevice(&f);
	
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment() || xml.isStartDocument() || xml.isEndDocument()) continue;
		if (xml.isStartElement() && xml.name() == "editorsettings") {
			parseElements();
		} else {
			formatError("Unexpected element at root level"+xml.name().toString()+", Token String: "+
				xml.tokenString());
		}
	}
	if (xml.hasError()) {
		formatError(xml.errorString());
	}
	if (!m_error.isEmpty()) {
		return false;
	}
	return true;
}

void LatexConfigParser::parseElements()
{
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isEndElement() && xml.name() == "editorsettings") break;
		if (xml.isWhitespace() || xml.isComment() || xml.isEndElement()) continue;
		if (!xml.isStartElement()) {
			formatError("Unexpected element in <editorsettings>"+xml.name().toString()+", Token String: "+
								 xml.tokenString());
			continue;
		}
	
		if (xml.name() == "executable") {
			m_executable = xml.attributes().value("command").toString();
		} else if (xml.name() == "imagefile") {
			m_imageExtension = xml.attributes().value("extension").toString();
		} else if (xml.name() == "highlighter") {
			m_highlight_kate = xml.attributes().value("kate").toString();
			m_highlight_file = xml.attributes().value("file").toString();
		} else if (xml.name() == "empty-frame-text") {
			m_emptyFrameText = XmlWidget::readI18nText(&xml);
		} else if (xml.name() == "preamble") {
			m_preamble = xml.readElementText();
		} else if (xml.name() == "postamble") {
			m_postamble = xml.readElementText();
		} else if (xml.name() == "tab") {
			parseTab();
		} else {
			formatError("Unknown tag in <editorsettings>: "+xml.name().toString());
		}
	}
}

void LatexConfigParser::formatError(QString message)
{
	QString new_error = QString::number(xml.lineNumber()) + ":" + 
			QString::number(xml.columnNumber()) + ":" + message;
	qDebug() << filename << new_error;
	m_error += new_error + "\n";
}

void LatexConfigParser::parseTab()
{
	QString type = xml.attributes().value("type").toString();
	bool itemstab = (type == "items");
	QString title = "";
	QString name, text, default_value;
	
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isWhitespace() || xml.isComment()) continue;
		if (xml.isEndElement() && xml.name() == "tab") break;
		if (!xml.isStartElement()) {
			formatError("Unexpected element in <tab>: "+xml.name().toString()+", Token String: "+
								 xml.tokenString());
			continue;
		}
		if (xml.name() == "title") {
			if (!title.isEmpty()) {
				formatError("Second <title> tag in <tab>");
			}
			title = XmlWidget::readI18nText(&xml);
		} else if (xml.name() == "item") {
			if (!itemstab) {
				formatError("Found <item> in a 'settings'-tab!");
			}
			QString value = xml.attributes().value("value").toString();
			QString img = xml.attributes().value("image").toString();
			text = XmlWidget::readI18nText(&xml);
			//qDebug() << "For future use: <item>" << value << img << text;
		} else if (xml.name() == "comment" || xml.name() == "font" 
			|| xml.name() == "spinbox" || xml.name() == "color" 
			|| xml.name() == "text" || xml.name() == "list") {
			//TODO: Store this + attributes in a list
			QString tagname = xml.name().toString();
			name = xml.attributes().value("name").toString();
			default_value = xml.attributes().value("default").toString();
			if (xml.name() != "list") {
				text = XmlWidget::readI18nText(&xml);
			} else {
				ignoreList();
			}
			if (!name.isEmpty()) {
				if (properties.contains(name)) {
					formatError("Redeclared setting with name: " + name);
				} else {
					properties.insert(name, default_value);
				}
			}
			qDebug() << "For future use:" << tagname << name << text << default_value;
		} else {
			formatError("Unexpected element in <tab>: " + xml.name().toString());
		}
	}
	
	if (title.isEmpty()) {
		formatError("Tab ended here, but no title was found!");
	}
}

void LatexConfigParser::ignoreList()
{
	//TODO: Quick hack to avoid real parsing
	while (!xml.atEnd()) {
		xml.readNext();
		if (xml.isEndElement() && xml.name() == "list") break;
	}
}
