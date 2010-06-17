/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "collectforoutput_ui.h"

#include "commonstrings.h"
#include "undomanager.h"
#include "filewatcher.h"
#include "prefscontext.h"
#include "scribuscore.h"
#include "scraction.h"
#include "scribusapp.h"

CollectForOutput_UI::CollectForOutput_UI(QWidget* parent, ScribusDoc* doc, QString outputDirectory, bool withFonts, bool withProfiles, bool compressDoc) :
	CollectForOutput(doc, outputDirectory, withFonts, withProfiles, compressDoc)
{
	uiCollect=true;
	connect(this, SIGNAL(fontsCollected(int)), this, SLOT(collectedFonts(int)));
	connect(this, SIGNAL(itemsCollected(int)), this, SLOT(collectedItems(int)));
	connect(this, SIGNAL(patternsCollected(int)), this, SLOT(collectedPatterns(int)));
	connect(this, SIGNAL(profilesCollected(int)), this, SLOT(collectedProfiles(int)));
}

QString CollectForOutput_UI::collect(QString &newFileName)
{
	if (!newDirDialog())
		return "Collect cancelled or unable to create collect destination directory";

	progressDialog=new MultiProgressDialog("Collect for Output", "Cancel");
	QStringList barNames, barTexts;
	QList<bool> barsNumeric;
	barNames << "items";
	barTexts << tr("Collecting Items:");
	barsNumeric << true;
	barNames << "patterns";
	barTexts << tr("Collecting Patterns:");
	barsNumeric << true;

	if (m_withFonts)
	{
		barNames << "fonts";
		barTexts << tr("Collecting Fonts:");
		barsNumeric << true;
	}
	if (m_withProfiles)
	{
		barNames << "profiles";
		barTexts << tr("Collecting Profiles:");
		barsNumeric << true;
	}

	progressDialog->addExtraProgressBars(barNames, barTexts, barsNumeric);
	progressDialog->setOverallTotalSteps(profileCount+itemCount+fontCount+patternCount);
	progressDialog->setTotalSteps("items", itemCount);
	progressDialog->setTotalSteps("patterns", patternCount);
	progressDialog->setProgress("items", 0);
	progressDialog->setProgress("patterns", 0);
	if (m_withFonts)
	{
		progressDialog->setTotalSteps("fonts", fontCount);
		progressDialog->setProgress("fonts", 0);
	}
	if (m_withProfiles)
	{
		progressDialog->setTotalSteps("profiles", profileCount);
		progressDialog->setProgress("profiles", 0);
	}
	progressDialog->setOverallProgress(0);



	ScCore->fileWatcher->forceScan();
	ScCore->fileWatcher->stop();


	progressDialog->show();
//	connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelRequested()));
	ScQApp->processEvents();

	dirs->set("collect", m_outputDirectory.left(m_outputDirectory.lastIndexOf("/",-2)));
	ScCore->primaryMainWindow()->setStatusBarInfoText( tr("Collecting..."));

	if (!collectItems())
	{
		QString errorMsg( tr("Cannot collect all files for output for file:\n%1").arg(newName) );
		QMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning,
							 "<qt>" + errorMsg + "</qt>",
							 CommonStrings::tr_OK);
		return errorMsg;
	}
	progressDialog->setOverallProgress(itemCount+patternCount);

	if (m_withFonts)
	{
		collectFonts();
		progressDialog->setProgress("fonts", fontCount);
		progressDialog->setOverallProgress(itemCount+patternCount+fontCount);
	}
	if (m_withProfiles)
	{
		collectProfiles();
		progressDialog->setProgress("profiles", profileCount);
		progressDialog->setOverallProgress(itemCount+patternCount+fontCount+profileCount);
	}
	/* collect document must go last because of image paths changes in collectItems() */
	if (!collectDocument())
	{
		QString errorMsg( tr("Cannot collect the file: \n%1").arg(newName) );
		QMessageBox::warning(ScCore->primaryMainWindow(), CommonStrings::trWarning, "<qt>" + errorMsg + "</qt>", CommonStrings::tr_OK);
		return errorMsg;
	}

	QDir::setCurrent(m_outputDirectory);
	ScCore->primaryMainWindow()->updateActiveWindowCaption(newName);
	UndoManager::instance()->renameStack(newName);
	ScCore->primaryMainWindow()->scrActions["fileRevert"]->setEnabled(false);
	ScCore->primaryMainWindow()->updateRecent(newName);
	ScCore->primaryMainWindow()->setStatusBarInfoText("");
	ScCore->primaryMainWindow()->mainWindowProgressBar->reset();
	ScCore->fileWatcher->start();
	collectedFiles.clear();
	newFileName=newName;
	progressDialog->close();
	delete progressDialog;
	return QString::null;
}

void CollectForOutput_UI::collectedFonts(int c)
{
	progressDialog->setProgress("fonts", c);
	ScQApp->processEvents();
}

void CollectForOutput_UI::collectedItems(int c)
{
	progressDialog->setProgress("items", c);
	ScQApp->processEvents();
}

void CollectForOutput_UI::collectedPatterns(int c)
{
	progressDialog->setProgress("patterns", c);
	ScQApp->processEvents();
}

void CollectForOutput_UI::collectedProfiles(int c)
{
	progressDialog->setProgress("profiles", c);
	ScQApp->processEvents();
}
