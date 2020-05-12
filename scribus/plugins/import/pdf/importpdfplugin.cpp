/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include <QApplication>
#include <QMessageBox>

#include "commonstrings.h"

#include "importpdf.h"
#include "importpdfplugin.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scpage.h"
#include "scpaths.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"
#include "util.h"

#include "ui/customfdialog.h"
#include "ui/scmessagebox.h"
#include "ui/scmwmenumanager.h"

int importpdf_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* importpdf_getPlugin()
{
	ImportPdfPlugin* plug = new ImportPdfPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void importpdf_freePlugin(ScPlugin* plugin)
{
	ImportPdfPlugin* plug = dynamic_cast<ImportPdfPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

ImportPdfPlugin::ImportPdfPlugin() :
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	registerFormats();
	languageChange();
}

void ImportPdfPlugin::languageChange()
{
	importAction->setText( tr("Import PDF..."));
	FileFormat* fmt = getFormatByExt("pdf");
	fmt->trName = FormatsManager::instance()->nameOfFormat(FormatsManager::PDF); // Human readable name
	fmt->filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::PDF); // QFileDialog filter
	if (ScCore->haveGS())
	{
		FileFormat* fmt2 = getFormatByExt("eps");
		fmt2->trName = FormatsManager::instance()->nameOfFormat(FormatsManager::EPS);
		fmt2->filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::EPS);
		FileFormat* fmt3 = getFormatByExt("ps");
		fmt3->trName = FormatsManager::instance()->nameOfFormat(FormatsManager::PS);
		fmt3->filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::PS);
	}
}

ImportPdfPlugin::~ImportPdfPlugin()
{
	unregisterAll();
}

QString ImportPdfPlugin::fullTrName() const
{
	return QObject::tr("PDF Importer");
}


const ScActionPlugin::AboutData* ImportPdfPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports PDF Files");
	about->description = tr("Imports most PDF files into the current document, converting their vector data into Scribus objects.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void ImportPdfPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void ImportPdfPlugin::registerFormats()
{
	FileFormat fmt(this);
	fmt.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::PDF); // Human readable name
	fmt.formatId = 0;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::PDF); // QFileDialog filter
	fmt.fileExtensions = QStringList() << "pdf";
	fmt.load = true;
	fmt.save = false;
	fmt.thumb = true;
	fmt.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::PDF); // MIME types
	fmt.priority = 64; // Priority
	registerFormat(fmt);

	if (ScCore->haveGS())
	{
		FileFormat fmt2(this);
		fmt2.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::EPS); // Human readable name
		fmt2.formatId = 0;
		fmt2.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::EPS);// QFileDialog filter
		fmt2.fileExtensions = QStringList() << "eps" << "epsf" << "epsi" << "eps2" << "eps3" << "epi" << "ept";
		fmt2.load = true;
		fmt2.save = false;
		fmt2.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::EPS); // MIME types
		fmt2.priority = 64; // Priority
		registerFormat(fmt2);

		FileFormat fmt3(this);
		fmt3.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::PS); // Human readable name
		fmt3.formatId = 0;
		fmt3.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::PS);// QFileDialog filter
		fmt3.fileExtensions = QStringList() << "ps";
		fmt3.load = true;
		fmt3.save = false;
		fmt3.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::PS); // MIME types
		fmt3.priority = 64; // Priority
		registerFormat(fmt3);
	}
}

bool ImportPdfPlugin::fileSupported(QIODevice* /* file */, const QString & fileName) const
{
	return true;
}

bool ImportPdfPlugin::loadFile(const QString & fileName, const FileFormat &, int flags, int /*index*/)
{
	// There's only one format to handle, so we just call import(...)
	return import(fileName, flags);
}

bool ImportPdfPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;
	if (fileName.isEmpty())
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("importpdf");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.pdf *.PDF);;All Files (*)");
		if (diaf.exec() != QDialog::Accepted)
			return false;
		fileName = diaf.selectedFile();
		prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
	}
	m_Doc =ScCore->primaryMainWindow()->doc;
	UndoTransaction activeTransaction;
	bool emptyDoc = (m_Doc == nullptr);
	bool hasCurrentPage = (m_Doc && m_Doc->currentPage());
	TransactionSettings trSettings;
	trSettings.targetName   = hasCurrentPage ? m_Doc->currentPage()->getUName() : "";
	trSettings.targetPixmap = Um::IImageFrame;
	trSettings.actionName   = Um::ImportXfig;
	trSettings.description  = fileName;
	trSettings.actionPixmap = Um::IXFIG;
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(false);
	if (UndoManager::undoEnabled())
		activeTransaction = UndoManager::instance()->beginTransaction(trSettings);
	bool isCleanedFile = false;
	QString cleanFile = "";
	QFileInfo fi(fileName);
	QStringList exts = QStringList() << "eps" << "epsf" << "epsi" << "eps2" << "eps3" << "epi" << "ept" << "ps" << "ai";
	if (exts.contains(fi.suffix().toLower()))
	{
		if (!ScCore->haveGS())
		{
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
			ScMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, tr("The Import plugin cannot handle Postscript files"));
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
			return false;
		}

		// Destill the eps/ps with ghostscript to get a clean pdf file
		bool cancel = false;
		QString errFile = getShortPathName(ScPaths::tempFileDir())+ "/ps.err";
		cleanFile = getShortPathName(ScPaths::tempFileDir()) + "/" + fi.baseName() + ".pdf";
		QStringList args;
		args.append( "-q" );
		args.append( "-dNOPAUSE" );
		args.append( "-sDEVICE=pdfwrite" );
		args.append( "-dBATCH" );
		args.append( "-dSAFER" );
		if (extensionIndicatesEPS(fi.suffix().toLower()))
			args.append("-dEPSCrop");
		args.append("-dCompatibilityLevel=1.4");
		args.append( QString("-sOutputFile=%1").arg(QDir::toNativeSeparators(cleanFile)) );
		args.append( QDir::toNativeSeparators(fileName) );
		System(getShortPathName(PrefsManager::instance().ghostscriptExecutable()), args, errFile, errFile, &cancel);
		args.clear();
		isCleanedFile = true;
	}
	bool ret = false;
	PdfPlug *dia = new PdfPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	if (isCleanedFile)
		ret = dia->import(cleanFile, trSettings, flags, !(flags & lfScripted));
	else
		ret = dia->import(fileName, trSettings, flags, !(flags & lfScripted));
	if (activeTransaction)
		activeTransaction.commit();
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	if (isCleanedFile)
		QFile::remove(cleanFile);
	return ret;
}

QImage ImportPdfPlugin::readThumbnail(const QString& fileName)
{
	if (fileName.isEmpty())
		return QImage();
	UndoManager::instance()->setUndoEnabled(false);
	m_Doc = nullptr;
	PdfPlug *dia = new PdfPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
