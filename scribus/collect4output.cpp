/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "collect4output.h"
#include "collect4output.moc"

#include "scribus.h"
#include "scribusapp.h"
#include "util.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "commonstrings.h"
#include "undomanager.h"
#include "filewatcher.h"
#include "pageitem.h"
#include "scraction.h"
#include "scraction.h"

#include <qmessagebox.h>
#include <qstring.h>
#include <qmap.h>
#include <qdir.h>

extern ScribusQApp* ScQApp;


CollectForOutput::CollectForOutput(bool withFonts, bool compressDoc)
	: QObject(ScQApp, 0)
{
	outputDirectory = QString();
	compressDoc = compressDoc;
	withFonts = withFonts;
	dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	collectedFiles.clear();
}

bool CollectForOutput::newDirDialog()
{
	QString curDir = QDir::currentDirPath();
	if (ScQApp->usingGUI())
	{
		QString wdir = ".";
		QString prefsDocDir = PrefsManager::instance()->documentDir();
		if (!prefsDocDir.isEmpty())
			wdir = dirs->get("collect", prefsDocDir);
		else
			wdir = dirs->get("collect", ".");
		outputDirectory = ScMW->CFileDialog(wdir, tr("Choose a Directory"), "", "", false, false, false, false, true, &compressDoc, &withFonts);
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
	ScMW->fileWatcher->forceScan();
	ScMW->fileWatcher->stop();
	if(outputDirectory.right(1) != "/")
		outputDirectory += "/";
	dirs->set("collect", outputDirectory.left(outputDirectory.findRev("/",-2)));
	ScMW->mainWindowStatusLabel->setText(tr("Collecting..."));

	if (!collectDocument())
	{
		QMessageBox::warning(ScMW, CommonStrings::trWarning, "<qt>" + tr("Cannot collect the file: \n%1").arg(newName) + "</qt>", CommonStrings::tr_OK);
		return "";
	}

	if (!collectItems())
	{
		QMessageBox::warning(ScMW, tr("Warning"), "<qt>" + tr("Cannot collect all files for output for file:\n%1").arg(newName) + "</qt>", CommonStrings::tr_OK);
		return "";
	}

	if (withFonts)
		collectFonts();

	PrefsManager::instance()->setDocumentDir(outputDirectory);
	QDir::setCurrent(outputDirectory);
	ScMW->updateActiveWindowCaption(newName);
	UndoManager::instance()->renameStack(newName);
	ScMW->scrActions["fileSave"]->setEnabled(false);
	ScMW->scrActions["fileRevert"]->setEnabled(false);
	ScMW->updateRecent(newName);
	ScMW->mainWindowStatusLabel->setText("");
	ScMW->mainWindowProgressBar->reset();
	ScMW->fileWatcher->start();
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

	if (ScMW->doc->hasName)
	{
		QFileInfo fis(ScMW->doc->DocName);
		newName += fis.fileName();
	}
	else
		newName += ScMW->doc->DocName+".sla";

	ScMW->doc->hasName = true;
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

	if (!overwrite(ScMW, newName))
		return false;
	if (!ScMW->DoFileSave(newName))
		return false;
	return true;
}

bool CollectForOutput::collectItems()
{
	uint counter = 0;
	for (uint lc = 0; lc < 3; ++lc)
	{
		PageItem* ite;
		switch (lc)
		{
			case 0:
				counter = ScMW->doc->MasterItems.count();
				break;
			case 1:
				counter = ScMW->doc->DocItems.count();
				break;
			case 2:
				counter = ScMW->doc->FrameItems.count();
				break;
		}
		for (uint b = 0; b < counter; ++b)
		{
			switch (lc)
			{
				case 0:
					ite = ScMW->doc->MasterItems.at(b);
					break;
				case 1:
					ite = ScMW->doc->DocItems.at(b);
					break;
				case 2:
					ite = ScMW->doc->FrameItems.at(b);
					break;
			}
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
					if (ScMW->fileWatcherActive())
					{
						ScMW->fileWatcher->removeFile(oldFile);
						ScMW->fileWatcher->addFile(ite->Pfile);
					}
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
							if (ScMW->fileWatcherActive())
							{
								ScMW->fileWatcher->removeFile(oldFile);
								ScMW->fileWatcher->addFile(ite->Pfile);
							}
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
	}
	return true;
}

bool CollectForOutput::collectFonts()
{
	PrefsManager *prefsManager = PrefsManager::instance();
	QMap<QString,int>::Iterator it3;
	QMap<QString,int>::Iterator it3end = ScMW->doc->UsedFonts.end();
	for (it3 = ScMW->doc->UsedFonts.begin(); it3 != it3end; ++it3)
	{
		QFileInfo itf = QFileInfo(prefsManager->appPrefs.AvailFonts[it3.key()]->fontFilePath());
		copyFile(prefsManager->appPrefs.AvailFonts[it3.key()]->fontFilePath(), outputDirectory + itf.fileName());
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
		QString basename = fi.baseName().left(fi.baseName().findRev("_"));
		newFile = QString("%1_%2.%3").arg(basename).arg(cnt).arg(fi.extension());
		++cnt;
	}
	if (copy)
		copyFile(oldFile, outputDirectory + newFile);
	collectedFiles[newFile] = oldFile;
	return newFile;
}
