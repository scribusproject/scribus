/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_shortwords.h"
#include "prefsstructs.h"
#include "ui/scmessagebox.h"
#include "swsyntaxhighlighter.h"
#include "version.h"
#include "scpaths.h"
#include "commonstrings.h"

Prefs_ShortWords::Prefs_ShortWords(QWidget* parent)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();

	// defaults
	if (QFile::exists(RC_PATH_USR))
	{
		messageLabel->setText( tr("User settings"));
		loadCfgFile(RC_PATH_USR);
	}
	else
	{
		messageLabel->setText( tr("System wide configuration"));
		resetButton->setEnabled(false);
		loadCfgFile(RC_PATH);
	}
	saveButton->setEnabled(false);
	new SWSyntaxHighlighter(cfgEdit);

	// signals
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButton_pressed()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetButton_pressed()));
	connect(cfgEdit, SIGNAL(textChanged()), this, SLOT(cfgEdit_changed()));
}

Prefs_ShortWords::~Prefs_ShortWords()
{
}

void Prefs_ShortWords::languageChange()
{
}

void Prefs_ShortWords::restoreDefaults(struct ApplicationPrefs *prefsData)
{
}

void Prefs_ShortWords::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
}

void Prefs_ShortWords::apply()
{
	if (saveButton->isEnabled())
		saveButton_pressed();
}

void Prefs_ShortWords::saveButton_pressed()
{
	if (cfgEdit->document()->isModified() && QFile::exists(RC_PATH_USR))
	{
		if ((ScMessageBox::warning(this, tr("Short Words"),
				"<qt>" + tr("User configuration exists already. "
						"Do you really want to overwrite it?") + "</qt>",
				CommonStrings::trYes,
				CommonStrings::trNo, 0, 0, 1)
			) == 1)
			return;
	}

	QFile f(RC_PATH_USR);
	if (!f.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, tr("Short Words"),
			 "<qt>" + tr("Cannot write file %1.").arg(RC_PATH_USR) + "</qt>",
			 CommonStrings::tr_OK);
	}
	QTextStream stream(&f);
	stream.setCodec("UTF-8");
	stream << cfgEdit->toPlainText();
	f.close();
	messageLabel->setText( tr("User settings saved"));
	saveButton->setEnabled(false);
}

void Prefs_ShortWords::resetButton_pressed()
{
	loadCfgFile(RC_PATH);
	QDir d;
	d.remove(RC_PATH_USR);
	saveButton->setEnabled(false);
	resetButton->setEnabled(false);
	messageLabel->setText( tr("System wide configuration reloaded"));
}

void Prefs_ShortWords::cfgEdit_changed()
{
	resetButton->setEnabled(true);
	saveButton->setEnabled(true);
}

bool Prefs_ShortWords::loadCfgFile(QString filename)
{
	QFile f(filename);
	if (!f.open(QIODevice::ReadOnly))
	{
		messageLabel->setText( tr("Cannot open file %1").arg(f.fileName()));
		return false;
	}
	cfgEdit->clear();
	QTextStream stream(&f);
	stream.setCodec("UTF-8");
	while (!stream.atEnd())
		cfgEdit->append(stream.readLine());
	f.close();
	// it's a must to prevent "overwrite?" message box on saving prefs
	cfgEdit->document()->setModified(false);
	return true;
}


