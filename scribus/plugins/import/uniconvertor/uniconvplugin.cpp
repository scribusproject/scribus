/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "uniconvplugin.h"
#include "scraction.h"
#include "util_formats.h"
#include "prefsmanager.h"

#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QProcess>
#include <QTemporaryFile>


#include <QCursor>
#include <QDrag>
#include <QFile>
#include <QList>
#include <QMimeData>
#include <QPainterPath>
#include <QRegExp>
#include <cmath>

#include "commonstrings.h"
#include "scribuscore.h"
#include "undomanager.h"

int uniconvertorplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* uniconvertorplugin_getPlugin()
{
	UniconvImportPlugin* plug = new UniconvImportPlugin();
	Q_CHECK_PTR(plug);
	return plug;
	return 0;
}

void uniconvertorplugin_freePlugin(ScPlugin* plugin)
{
	UniconvImportPlugin* plug = dynamic_cast<UniconvImportPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

UniconvImportPlugin::UniconvImportPlugin() : LoadSavePlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place. This includes registering file format
	// support.
	languageChange();
}

UniconvImportPlugin::~UniconvImportPlugin()
{
	unregisterAll();
};

void UniconvImportPlugin::languageChange()
{
	// (Re)register file format support.
	unregisterAll();
	registerFormats();
}

const QString UniconvImportPlugin::fullTrName() const
{
	return QObject::tr("Uniconvertor Import");
}

const ScActionPlugin::AboutData* UniconvImportPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Hermann Kraus <herm@scribus.info>";
	about->shortDescription = tr("Imports Vector Files with UniConvertor");
	about->description = tr("Converts many vector formats to SVG and then "
		"loads the resulting SVG.");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void UniconvImportPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

void UniconvImportPlugin::registerFormats()
{
	QString name = tr("Uniconvertor Import");
	FileFormat fmt(this);
	fmt.trName = name;
	fmt.formatId = FORMATID_UNICONVIMPORT;
	fmt.filter = name + " (" + 
		FormatsManager::instance()->extensionListForFormat(
		FormatsManager::UNICONV, 0)+")"; // QFileDialog filter
	fmt.nameMatch = QRegExp("\\.("+
		FormatsManager::instance()->extensionListForFormat(
		FormatsManager::UNICONV, 1)+")$", Qt::CaseInsensitive);
	fmt.load = true;
	fmt.save = false;
	//TODO: fmt.mimeTypes = QStringList(""); // MIME types
	fmt.priority = 64; // Lowest priority
	registerFormat(fmt);
}

bool UniconvImportPlugin::fileSupported(QIODevice* /* file */, 
	const QString & fileName) const
{
	// TODO: It's hard to tell if a file is supported without first loading it
	return true;
}

bool UniconvImportPlugin::loadFile(const QString & fileName, 
	const FileFormat & /* fmt */, int flags, int /*index*/)
{
	// For now, "load file" and import are the same thing for this plugin
	return import(fileName, flags);
}

bool UniconvImportPlugin::import(QString fileName, int flags)
{
	if (!checkFlags(flags))
		return false;

	m_Doc = ScCore->primaryMainWindow()->doc;
	ScribusMainWindow *mw =
		(m_Doc==0) ? ScCore->primaryMainWindow() : m_Doc->scMW();

	//Get a temporary filename ending in .svg (sadly
	//uniconvertor has no other way of specifying the output format
	QTemporaryFile *tempFile = 
		new QTemporaryFile(QDir::tempPath() + "/scribus_uniconv_XXXXXX");
	tempFile->open();
	QString tempFileName = tempFile->fileName() + ".svg";
	tempFile->close();
	delete tempFile;

	//prepare arguments for uniconvertor call
	QStringList arguments;
	arguments << fileName <<  tempFileName;
	
	//execute uniconvertor
	QProcess uniconv;
	uniconv.setProcessChannelMode(QProcess::MergedChannels);
	uniconv.start(PrefsManager::instance()->uniconvExecutable(), arguments);
	
	//handle errors
	if (!uniconv.waitForStarted(120000)) {
		qWarning() << "Uniconvertor failed:" << 
			PrefsManager::instance()->uniconvExecutable() << arguments;
		QMessageBox::warning(mw, CommonStrings::trWarning,
			tr("Starting Uniconvertor failed! The executable name in "
			"File->Preferences->External Tools may be incorrect or the "
			"software has been uninstalled since preferences "
			"were set. (%1)").arg(uniconv.errorString()));
		return false;
	}
	if (!uniconv.waitForFinished(120000)) {
		qDebug() << "Uniconv exit code:" << uniconv.exitCode();
		QMessageBox::warning(mw, CommonStrings::trWarning,
			tr("Uniconvertor did not exit correctly: %1").arg(
			uniconv.errorString()).arg(QString(uniconv.readAll())));
		return false;
	}
	if (uniconv.exitCode()) {
		qDebug() << "Uniconv exit code:" << uniconv.exitCode();
		QMessageBox::warning(mw, CommonStrings::trWarning,
			tr("Uniconvertor failed to convert the file: %1").arg(
				QString(uniconv.readAll())));
		return false;
	}
	
	//Import SVG
	const FileFormat *fmt = LoadSavePlugin::getFormatById(FORMATID_SVGIMPORT);
	if (!fmt) {
		QMessageBox::warning(mw, CommonStrings::trWarning, tr("The SVG Import plugin could not be found"), 1, 0, 0);
		return false;
	}
	qDebug() << "Loading file!";
	fmt->loadFile(tempFileName, flags);
	return true;
}
