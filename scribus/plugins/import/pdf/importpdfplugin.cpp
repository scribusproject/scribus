/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"
#include "ui/customfdialog.h"
#include "importpdf.h"
#include "importpdfplugin.h"
#include "ui/scmwmenumanager.h"
#include "scpage.h"
#include "scpaths.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "scraction.h"
#include "scribuscore.h"
#include "undomanager.h"
#include "util_formats.h"
#include "util.h"
#include <QMessageBox>


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

ImportPdfPlugin::ImportPdfPlugin() : LoadSavePlugin(),
	importAction(new ScrAction(ScrAction::DLL, QPixmap(), QPixmap(), "", QKeySequence(), this))
{
	// Set action info in languageChange, so we only have to do it in one
	// place. This includes registering file format support.
	languageChange();
}
/*
void ImportXfigPlugin::addToMainWindowMenu(ScribusMainWindow *mw)
{
	importAction->setEnabled(true);
	connect( importAction, SIGNAL(triggered()), SLOT(import()) );
	mw->scrMenuMgr->addMenuItem(importAction, "FileImport");
}
*/
void ImportPdfPlugin::languageChange()
{
	importAction->setText( tr("Import PDF..."));
	// (Re)register file format support
	unregisterAll();
	registerFormats();
}

ImportPdfPlugin::~ImportPdfPlugin()
{
	unregisterAll();
};

const QString ImportPdfPlugin::fullTrName() const
{
	return QObject::tr("PDF Importer");
}


const ScActionPlugin::AboutData* ImportPdfPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Franz Schmid <franz@scribus.info>";
	about->shortDescription = tr("Imports PDF Files");
	about->description = tr("Imports most PDF files into the current document,\nconverting their vector data into Scribus objects.");
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
	fmt.formatId = FORMATID_PDFIMPORT;
	fmt.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::PDF); // QFileDialog filter
	fmt.nameMatch = QRegExp("\\."+FormatsManager::instance()->extensionListForFormat(FormatsManager::PDF, 1)+"$", Qt::CaseInsensitive);
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
		fmt2.formatId = FORMATID_EPSIMPORT;
		fmt2.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::EPS);// QFileDialog filter
		fmt2.nameMatch = QRegExp("\\.("+FormatsManager::instance()->extensionListForFormat(FormatsManager::EPS, 1)+")$", Qt::CaseInsensitive);
		fmt2.fileExtensions = QStringList() << "eps" << "epsf" << "epsi" << "eps2" << "eps3" << "epi" << "ept";
		fmt2.load = true;
		fmt2.save = false;
		fmt2.mimeTypes = FormatsManager::instance()->mimetypeOfFormat(FormatsManager::EPS); // MIME types
		fmt2.priority = 64; // Priority
		registerFormat(fmt2);

		FileFormat fmt3(this);
		fmt3.trName = FormatsManager::instance()->nameOfFormat(FormatsManager::PS); // Human readable name
		fmt3.formatId = FORMATID_PSIMPORT;
		fmt3.filter = FormatsManager::instance()->extensionsForFormat(FormatsManager::PS);// QFileDialog filter
		fmt3.nameMatch = QRegExp("\\.("+FormatsManager::instance()->extensionListForFormat(FormatsManager::PS, 1)+")$", Qt::CaseInsensitive);
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
	if( fileName.isEmpty() )
	{
		flags |= lfInteractive;
		PrefsContext* prefs = PrefsManager::instance()->prefsFile->getPluginContext("importpdf");
		QString wdir = prefs->get("wdir", ".");
		CustomFDialog diaf(ScCore->primaryMainWindow(), wdir, QObject::tr("Open"), tr("All Supported Formats")+" (*.pdf *.PDF);;All Files (*)");
		if (diaf.exec())
		{
			fileName = diaf.selectedFile();
			prefs->set("wdir", fileName.left(fileName.lastIndexOf("/")));
		}
		else
			return true;
	}
	m_Doc=ScCore->primaryMainWindow()->doc;
	UndoTransaction* activeTransaction = NULL;
	bool emptyDoc = (m_Doc == NULL);
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
		activeTransaction = new UndoTransaction(UndoManager::instance()->beginTransaction(trSettings));
	bool isCleanedFile = false;
	QString cleanFile = "";
	QFileInfo fi(fileName);
	QStringList exts = QStringList() << "eps" << "epsf" << "epsi" << "eps2" << "eps3" << "epi" << "ept" << "ps";
	if (exts.contains(fi.suffix().toLower()))
	{
		if (ScCore->haveGS())
		{
			// Destill the eps/ps with ghostscript to get a clean pdf file
			bool cancel = false;
			QString errFile = getShortPathName(ScPaths::getTempFileDir())+ "/ps.err";
			cleanFile = getShortPathName(ScPaths::getTempFileDir())+ "/clean.pdf";
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
			System(getShortPathName(PrefsManager::instance()->ghostscriptExecutable()), args, errFile, errFile, &cancel);
			args.clear();
			isCleanedFile = true;
		}
		else
		{
			QMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, tr("The Import plugin cannot handle Postscript files"), 1, 0, 0);
			return false;
		}
	}
	PdfPlug *dia = new PdfPlug(m_Doc, flags);
	Q_CHECK_PTR(dia);
	if (isCleanedFile)
		dia->import(cleanFile, trSettings, flags);
	else
		dia->import(fileName, trSettings, flags);
	if (activeTransaction)
	{
		activeTransaction->commit();
		delete activeTransaction;
		activeTransaction = NULL;
	}
	if (emptyDoc || !(flags & lfInteractive) || !(flags & lfScripted))
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	if (isCleanedFile)
		QFile::remove(cleanFile);
	return true;
}

QImage ImportPdfPlugin::readThumbnail(const QString& fileName)
{
	bool wasUndo = false;
	if( fileName.isEmpty() )
		return QImage();
	if (UndoManager::undoEnabled())
	{
		UndoManager::instance()->setUndoEnabled(false);
		wasUndo = true;
	}
	m_Doc = NULL;
	PdfPlug *dia = new PdfPlug(m_Doc, lfCreateThumbnail);
	Q_CHECK_PTR(dia);
	QImage ret = dia->readThumbnail(fileName);
	if (wasUndo)
		UndoManager::instance()->setUndoEnabled(true);
	delete dia;
	return ret;
}
