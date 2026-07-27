/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fsw_finish.h"
#include <QEvent>
#include <QPixmap>
#include <QWizard>

#include "firststartwizard.h"
#include "fsw_language.h"
#include "fsw_appearance.h"
#include "fsw_newdocument.h"
#include "fsw_fontsscripts.h"

#include "iconmanager.h"

FSW_Finish::FSW_Finish(QWidget* parent)
	: QWizardPage(parent)
{
	setupUi(this);
	finishSplashLabel->setAlignment(Qt::AlignCenter);
	loadSplash();
}

void FSW_Finish::initializePage()
{
	QWizardPage::initializePage();

	QWizard* wiz = wizard();
	if (!wiz)
		return;

	// Reach the other pages by id and read their getters. Pages are all constructed up
	// front and their combos default-populated, so unvisited pages still report sane
	// values rather than blanks.
	auto* lang = qobject_cast<FSW_Language*>(wiz->page(FirstStartWizard::Page_Language));
	auto* appr = qobject_cast<FSW_Appearance*>(wiz->page(FirstStartWizard::Page_Appearance));
	auto* nd   = qobject_cast<FSW_NewDocument*>(wiz->page(FirstStartWizard::Page_NewDocument));
	auto* fs   = qobject_cast<FSW_FontsScripts*>(wiz->page(FirstStartWizard::Page_FontsScripts));

	QString html = QStringLiteral("<table cellspacing='0' cellpadding='0'>");
	auto addRow = [&html](const QString& k, const QString& v) {
		html += QStringLiteral("<tr><td style='padding:3px 18px 3px 0'>%1</td>"
							   "<td style='padding:3px 0'><b>%2</b></td></tr>")
				.arg(k.toHtmlEscaped(), v.toHtmlEscaped());
	};

	if (lang)
	{
		addRow(tr("Interface language"), lang->uiLanguageName());
		addRow(tr("Document language"), lang->documentLanguageName());
		addRow(tr("Units"), lang->unitName());
	}
	if (appr)
	{
		QString theme;
		switch (appr->themeMode())
		{
			case 0:
				theme = tr("Light");
				break;
			case 1:
				theme = tr("Dark");
				break;
			default:
				theme = tr("Automatic");
				break;
		}
		addRow(tr("Theme"), theme);
		addRow(tr("Workspace"), appr->minimalWorkspace() ? tr("Minimal") : tr("Everything visible"));
	}
	if (nd)
	{
		addRow(tr("Page size"), nd->pageSizeDisplayName());
		addRow(tr("Text direction"), nd->isRTL() ? tr("Right to left") : tr("Left to right"));
	}
	if (fs)
	{
		const int fontCount = fs->fontPaths().size();
		if (fontCount > 0)
			addRow(tr("Font folders added"), QString::number(fontCount));
		const QString scriptDir = fs->scriptPath();
		if (!scriptDir.isEmpty())
			addRow(tr("Script folder"), scriptDir);
	}
	html += QStringLiteral("</table>");

	summaryLabel->setTextFormat(Qt::RichText);
	summaryLabel->setText(html);
}

void FSW_Finish::setThemeMode(int mode)
{
	if (m_mode == mode)
		return;
	m_mode = mode;
	loadSplash();
}

void FSW_Finish::loadSplash()
{
	IconManager& im = IconManager::instance();
	QPixmap pix;
	switch (m_mode)
	{
		case 0:
			pix = im.splashScreen(false);
			break;  // light
		case 1:
			pix = im.splashScreen(true);
			break;  // dark
		default:
			pix = im.splashScreen();
			break;  // automatic
	}
	finishSplashLabel->setPixmap(pix);
}

void FSW_Finish::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange)
		retranslateUi(this);
	QWizardPage::changeEvent(e);
}
