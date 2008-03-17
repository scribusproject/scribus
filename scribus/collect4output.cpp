/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "collect4output.h"

#include "scribusdoc.h"
#include "scribuscore.h"
#include "util.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "commonstrings.h"
#include "undomanager.h"
#include "filewatcher.h"
#include "pageitem.h"
#include "scraction.h"
#include "scpattern.h"
#include "util_file.h"

#include <QMessageBox>
#include <QString>
#include <QMap>
#include <QDir>

CollectForOutput::CollectForOutput(ScribusDoc* doc, bool withFontsA, bool withProfilesA, bool compressDocA)
	: QObject(ScCore),
	m_Doc(0)
{
	m_Doc=doc;
	outputDirectory = QString();
	compressDoc = compressDocA;
	withFonts = withFontsA;
	withProfiles = withProfilesA;
	dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	collectedFiles.clear();
}

bool CollectForOutput::newDirDialog()
{
	QString curDir = QDir::currentPath();
	if (ScCore->usingGUI())
	{
		QString wdir = ".";
		QString prefsDocDir = PrefsManager::instance()->documentDir();
		if (!prefsDocDir.isEmpty())
			wdir = dirs->get("collect", prefsDocDir);
		else
			wdir = dirs->get("collect", ".");
		outputDirectory = ScCore->primaryMainWindow()->CFileDialog(wdir, tr("Choose a Directory"), "", "", fdDirectoriesOnly, &compressDoc, &withFonts, &withProfiles);
	}
	if (outputDirectory.isEmpty())
		return false;
	if (!outputDirectory.endsWith("/"))
		outputDirectory += "/";
	return true;
}

QString CollectForOutput::collect()
{
	if (!newDirDialog())
		return "";
	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();
	dirs->set("collect", outputDirectory.left(outputDirectory.lastIndexOf("/",-2)));
	ScCore->primaryMainWindow()->setStatusBarInfoText( tr("Collecting..."));

	if (!collectItems())
	{
		QMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning,
							 "<qt>" + tr("Cannot collect all files for output for file:\n%1").arg(newName) + "</qt>",
							 CommonStrings::tr_OK);
		return "";
	}

	if (withFonts)
		collectFonts();
	if (withProfiles)
		collectProfiles();

	/* collect document must go last because of image paths changes
	in collectItems() */
	if (!collectDocument())
	{
		QMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, "<qt>" + tr("Cannot collect the file: \n%1").arg(newName) + "</qt>", CommonStrings::tr_OK);
		return "";
	}

	QDir::setCurrent(outputDirectory);
	ScCore->primaryMainWindow()->updateActiveWindowCaption(newName);
	UndoManager::instance()->renameStack(newName);
	ScCore->primaryMainWindow()->scrActions["fileSave"]->setEnabled(false);
	ScCore->primaryMainWindow()->scrActions["fileRevert"]->setEnabled(false);
	ScCore->primaryMainWindow()->updateRecent(newName);
	ScCore->primaryMainWindow()->setStatusBarInfoText("");
	ScCore->primaryMainWindow()->mainWindowProgressBar->reset();
	ScCore->fileWatcher->start();
	collectedFiles.clear();
	return newName;
}

bool CollectForOutput::collectDocument()
{
	QFileInfo fi = QFileInfo(outputDirectory);
	newName = outputDirectory;
	if (!fi.exists())
		return false;
	if (!fi.isDir() || !fi.isWritable())
		return false;

	if (m_Doc->hasName)
	{
		QFileInfo fis(m_Doc->DocName);
		newName += fis.fileName();
	}
	else
		newName += m_Doc->DocName+".sla";

	m_Doc->hasName = true;
	if (compressDoc)
	{
		if (!newName.endsWith(".gz"))
			newName += ".gz";
		else
		{
			if (newName.endsWith(".gz"))
				newName = newName.remove(".gz");
		}
	}

	if (!overwrite(ScCore->primaryMainWindow(), newName))
		return false;
	if (!ScCore->primaryMainWindow()->DoFileSave(newName))
		return false;
	return true;
}

bool CollectForOutput::collectItems()
{
	uint counter = 0;
	for (uint lc = 0; lc < 3; ++lc)
	{
		PageItem* ite = NULL;
		switch (lc)
		{
			case 0:
				counter = m_Doc->MasterItems.count();
				break;
			case 1:
				counter = m_Doc->DocItems.count();
				break;
			case 2:
				counter = m_Doc->FrameItems.count();
				break;
		}
		for (uint b = 0; b < counter; ++b)
		{
			switch (lc)
			{
				case 0:
					ite = m_Doc->MasterItems.at(b);
					break;
				case 1:
					ite = m_Doc->DocItems.at(b);
					break;
				case 2:
					ite = m_Doc->FrameItems.at(b);
					break;
			}
			processItem(ite);
		}
	}
	QStringList patterns = m_Doc->getUsedPatterns();
	for (int c = 0; c < patterns.count(); ++c)
	{
		ScPattern pa = m_Doc->docPatterns[patterns[c]];
		for (int o = 0; o < pa.items.count(); o++)
		{
			processItem(pa.items.at(o));
		}
	}
	return true;
}

void CollectForOutput::processItem(PageItem *ite)
{
	if (ite->asImageFrame())
	{
		/* hack for subsequent c4o "./" -> "/doc/full/path" */
		QString ofName(ite->Pfile);
		QFileInfo itf = QFileInfo(ofName);
		if (!itf.exists())
		{
			ofName = QDir::convertSeparators(PrefsManager::instance()->documentDir() + "/" + ofName);
			itf.setFile(ofName);
		}
		// end of hack
		if (itf.exists())
		{
			QString oldFile = ofName;
			ite->Pfile = collectFile(oldFile, itf.fileName());
			ScCore->fileWatcher->removeFile(oldFile);
			ScCore->fileWatcher->addFile(ite->Pfile);
		}
	}
	if (ite->asTextFrame())
	{
		if (ite->isAnnotation())
		{
			QFileInfo itf;
			if (!ite->Pfile.isEmpty())
			{
				itf = QFileInfo(ite->Pfile);
				if (itf.exists())
				{
					QString oldFile = ite->Pfile;
					ite->Pfile = collectFile(oldFile, itf.fileName());
					ScCore->fileWatcher->removeFile(oldFile);
					ScCore->fileWatcher->addFile(ite->Pfile);
				}
			}
			if (!ite->Pfile2.isEmpty())
			{
				itf = QFileInfo(ite->Pfile2);
				if (itf.exists())
					ite->Pfile2 = collectFile(ite->Pfile2, itf.fileName());
			}
			if (!ite->Pfile3.isEmpty())
			{
				itf = QFileInfo(ite->Pfile3);
				if (itf.exists())
					ite->Pfile3 = collectFile(ite->Pfile3, itf.fileName());
			}
		}
	}
}

bool CollectForOutput::collectFonts()
{
	PrefsManager *prefsManager = PrefsManager::instance();
	QMap<QString,int>::Iterator it3;
	QMap<QString,int>::Iterator it3end = m_Doc->UsedFonts.end();
	for (it3 = m_Doc->UsedFonts.begin(); it3 != it3end; ++it3)
	{
		QFileInfo itf = QFileInfo(prefsManager->appPrefs.AvailFonts[it3.key()].fontFilePath());
		copyFile(prefsManager->appPrefs.AvailFonts[it3.key()].fontFilePath(), outputDirectory + itf.fileName());
		if (prefsManager->appPrefs.AvailFonts[it3.key()].type() == ScFace::TYPE1)
		{
			QStringList metrics;
			QString fontDir  = itf.absolutePath();
			QString fontFile = itf.fileName();
			metrics += findFontMetrics(fontDir, fontFile);
			if ( metrics.size() <= 0 )
			{
				QDir dir;
				if (dir.exists(fontDir + "/AFMs"))
					metrics += findFontMetrics(fontDir + "/AFMs", fontFile);
				if (dir.exists(fontDir + "/afm") && metrics.size() <= 0)
					metrics += findFontMetrics(fontDir + "/afm", fontFile);
				if (dir.exists(fontDir + "/Pfm") && metrics.size() <= 0)
					metrics += findFontMetrics(fontDir + "/Pfm", fontFile);
				if (dir.exists(fontDir + "/pfm") && metrics.size() <= 0)
					metrics += findFontMetrics(fontDir + "/pfm", fontFile);
			}
			for (int a = 0; a < metrics.size(); a++)
			{
				QString origAFM = metrics[a];
				QFileInfo fi(origAFM);
				copyFile(origAFM, outputDirectory + fi.fileName());
			}
		}
	}
	return true;
}

QStringList CollectForOutput::findFontMetrics(const QString& baseDir, const QString& baseName) const
{
	QStringList metricsFiles;
	QString     basePath = baseDir + "/" + baseName;
	QString     afnm = basePath.left(basePath.length()-3);
	// Look for afm files
	QString afmName(afnm+"afm");
	if(QFile::exists(afmName))
		metricsFiles.append(afmName);
	else
	{
		afmName = afnm+"Afm";
		if(QFile::exists(afmName))
			metricsFiles.append(afmName);
		else
		{
			afmName = afnm+"AFM";
			if(QFile::exists(afmName))
				metricsFiles.append(afmName);
		}
	}
	// Look for pfm files
	QString pfmName(afnm+"pfm");
	if(QFile::exists(pfmName))
		metricsFiles.append(pfmName);
	else
	{
		pfmName = afnm+"Pfm";
		if(QFile::exists(pfmName))
			metricsFiles.append(pfmName);
		else
		{
			afmName = afnm+"PFM";
			if(QFile::exists(pfmName))
				metricsFiles.append(pfmName);
		}
	}
	return metricsFiles;
}

bool CollectForOutput::collectProfiles()
{
	ProfilesL docProfiles;
	m_Doc->getUsedProfiles(docProfiles);
	ProfilesL::Iterator itend = docProfiles.end();
	for (ProfilesL::Iterator it = docProfiles.begin(); it != itend; ++it)
	{
		QString profileName(it.key());
		QString profilePath(it.value());
		copyFile(profilePath, outputDirectory + QFileInfo(profilePath).fileName());
	}
	return true;
}

QString CollectForOutput::collectFile(QString oldFile, QString newFile)
{
	uint cnt = 1;
	bool copy = true;

	while (collectedFiles.contains(newFile))
	{
		// overwrite only different sources
		if (collectedFiles[newFile] == oldFile)
		{
			copy = false;
			break;
		}
		QFileInfo fi(newFile);
		QString basename = fi.baseName().left(fi.baseName().lastIndexOf("_"));
		newFile = QString("%1_%2.%3").arg(basename).arg(cnt).arg(fi.completeSuffix());
		++cnt;
	}
	if (copy)
		copyFile(oldFile, outputDirectory + newFile);
	collectedFiles[newFile] = oldFile;
	return outputDirectory + newFile;
}
