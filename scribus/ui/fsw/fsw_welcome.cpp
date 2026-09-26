/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "fsw_welcome.h"
#include <QEvent>
#include <QPixmap>

#include "iconmanager.h"
#include "ui/splash.h"

FSW_Welcome::FSW_Welcome(QWidget* parent)
	: QWizardPage(parent)
{
	setupUi(this);
	splashLabel->setScaledContents(false);
	splashLabel->setAlignment(Qt::AlignCenter);
	loadSplash();
}

void FSW_Welcome::setThemeMode(int mode)
{
	if (m_mode == mode)
		return;
	m_mode = mode;
	loadSplash();
}

void FSW_Welcome::loadSplash()
{
	// The splash is an icon-set asset (the same one the startup screen uses); IconManager
	// resolves light/dark from its declared paths. Automatic follows the app appearance.
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
	const bool darkMode = m_mode == 1 || (m_mode != 0 && im.iconsForDarkMode());
	splashLabel->setPixmap(ScSplashScreen::previewPixmap(pix, darkMode));
}

void FSW_Welcome::changeEvent(QEvent* e)
{
	if (e->type() == QEvent::LanguageChange)
		retranslateUi(this);
	QWizardPage::changeEvent(e);
}
