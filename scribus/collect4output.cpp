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
		outputDirectory = ScApp->CFileDialog(wdir, tr("Choose a Directory"), "", "", false, false, false, false, true, &compressDoc, &withFonts);
	}
	if (outputDirectory.isEmpty())
		return false;
	outputDirectory += "/";
	return true;
}

QString CollectForOutput::collect()
{
	if (!newDirDialog())
		return "";
	ScApp->fileWatcher->forceScan();
	ScApp->fileWatcher->stop();
	if(outputDirectory.right(1) != "/")
		outputDirectory += "/";
	dirs->set("collect", outputDirectory.left(outputDirectory.findRev("/",-2)));
	ScApp->mainWindowStatusLabel->setText(tr("Collecting..."));
	if (!collectItems())
	{
		QMessageBox::warning(ScApp, tr("Warning"), "<qt>" + tr("Cannot collect all files for output for file:\n%1").arg(newName) + "</qt>", CommonStrings::tr_OK);
		return "";
	}
	if (!collectDocument())
		return "";
	collectFonts();
	if (!collectDocument())
	{
		QMessageBox::warning(ScApp, CommonStrings::trWarning, "<qt>" + tr("Cannot collect  the file: \n%1").arg(newName) + "</qt>", CommonStrings::tr_OK);
		return "";
	}
	QDir::setCurrent(outputDirectory);
	ScApp->updateActiveWindowCaption(newName);
	UndoManager::instance()->renameStack(newName);
	ScApp->scrActions["fileSave"]->setEnabled(false);
	ScApp->scrActions["fileRevert"]->setEnabled(false);
	ScApp->updateRecent(newName);
	ScApp->mainWindowStatusLabel->setText("");
	ScApp->mainWindowProgressBar->reset();
	ScApp->fileWatcher->start();
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

	if (ScApp->doc->hasName)
	{
		QFileInfo fis(ScApp->doc->DocName);
		newName += fis.fileName();
	}
	else
		newName += ScApp->doc->DocName+".sla";

	ScApp->doc->hasName = true;
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

	if (!overwrite(ScApp, newName))
		return false;
	if (!ScApp->DoFileSave(newName))
		return false;
	return true;
}

bool CollectForOutput::collectItems()
{
	if (ScApp->doc->masterPageMode)
		ScApp->doc->MasterPages = ScApp->doc->Pages;
	else
		ScApp->doc->DocPages = ScApp->doc->Pages;
	uint counter = 0;
	for (uint lc = 0; lc < 3; ++lc)
	{
		PageItem* ite;
		switch (lc)
		{
			case 0:
				counter = ScApp->doc->MasterItems.count();
				break;
			case 1:
				counter = ScApp->doc->DocItems.count();
				break;
			case 2:
				counter = ScApp->doc->FrameItems.count();
				break;
		}
		for (uint b = 0; b < counter; ++b)
		{
			switch (lc)
			{
				case 0:
					ite = ScApp->doc->MasterItems.at(b);
					break;
				case 1:
					ite = ScApp->doc->DocItems.at(b);
					break;
				case 2:
					ite = ScApp->doc->FrameItems.at(b);
					break;
			}
			if (ite->asImageFrame())
			{
				QFileInfo itf = QFileInfo(ite->Pfile);
				if (itf.exists())
				{
					QString oldFile = ite->Pfile;
					ite->Pfile = collectFile(oldFile, itf.fileName());
					if (ScApp->fileWatcherActive())
					{
						ScApp->fileWatcher->removeFile(oldFile);
						ScApp->fileWatcher->addFile(ite->Pfile);
					}
				}
			}
			if (ite->asTextFrame())
			{
				if (ite->isAnnotation)
				{
					QFileInfo itf;
					if (!ite->Pfile.isEmpty())
					{
						itf = QFileInfo(ite->Pfile);
						if (itf.exists())
						{
							QString oldFile = ite->Pfile;
							ite->Pfile = collectFile(oldFile, itf.fileName());
							if (ScApp->fileWatcherActive())
							{
								ScApp->fileWatcher->removeFile(oldFile);
								ScApp->fileWatcher->addFile(ite->Pfile);
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
	if (!withFonts)
		return false;

	PrefsManager *prefsManager = PrefsManager::instance();
	QMap<QString,int>::Iterator it3;
	QMap<QString,int>::Iterator it3end = ScApp->doc->UsedFonts.end();
	for (it3 = ScApp->doc->UsedFonts.begin(); it3 != it3end; ++it3)
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
		copyFile(oldFile, newFile);
	collectedFiles[newFile] = oldFile;
	return newFile;
}
