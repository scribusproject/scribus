/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          pageitem.h  -  description
                             -------------------
    copyright            : Scribus Team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#include <cassert>
#include <iostream>
#include <QByteArray>
#include <QDebug>
#include <QGlobalStatic>

#include "colormgmt/sccolormgmtengine.h"
#include "commonstrings.h"
#include "filewatcher.h"
#include "pluginmanager.h"
#include "prefsmanager.h"
#include "scpaths.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "splash.h"
#include "undomanager.h"
#include "util.h"
#include "util_icon.h"
#include "util_ghostscript.h"


extern ScribusQApp* ScQApp;

#include "cmserrorhandling.h"

ScribusCore::ScribusCore() : QObject()
{
	m_ScribusInitialized=false;
	m_SplashScreen=0;
	m_UseGUI=false;
	m_HaveCMS=false;
// 	m_PaletteParent=0;
	m_currScMW=0;

	defaultRGBProfile  = NULL;
	defaultCMYKProfile = NULL;
	defaultRGBToScreenSolidTrans  = NULL;
	defaultRGBToScreenImageTrans  = NULL;
	defaultCMYKToScreenImageTrans = NULL;
	defaultRGBToCMYKTrans = NULL;
	defaultCMYKToRGBTrans = NULL;
}



ScribusCore::~ScribusCore()
{
	TermDefaultColorTransforms();
// 	delete m_PaletteParent;
}

#ifndef NDEBUG
static void abort_on_error(QtMsgType t, const char * m)
{
	std::cerr << m << "\n";
	if(t==QtFatalMsg) assert(false);
//	if(t!=QtDebugMsg) assert(false);
}
#endif


int ScribusCore::init(bool useGUI, bool swapDialogButtonOrder, const QList<QString>& filesToUse)
{
	m_UseGUI=useGUI;
	m_Files=filesToUse;
	m_SwapDialogButtonOrder=swapDialogButtonOrder;
#if !defined(NDEBUG) && !defined(_WIN32)
	qInstallMsgHandler( & abort_on_error );
#endif

	return 0;
}

const QString& ScribusCore::getGuiLanguage() const
{
	return ScQApp->currGUILanguage();
}

int ScribusCore::startGUI(bool showSplash, bool showFontInfo, bool showProfileInfo, const QString newGuiLanguage, const QString prefsUserFile)
{
// 	m_PaletteParent=new QWidget(0);
// 	Q_CHECK_PTR(m_PaletteParent);
	ScribusMainWindow* scribus = new ScribusMainWindow();
	Q_CHECK_PTR(scribus);
	if (!scribus)
		return(EXIT_FAILURE);
	ScMWList.append(scribus);
	m_currScMW=0;
//	ScMW=scribus;
	int retVal=initScribusCore(showSplash, showFontInfo, showProfileInfo,newGuiLanguage, prefsUserFile);
	if (retVal == 1)
		return(EXIT_FAILURE);
	
	retVal = scribus->initScMW(true);
	if (retVal == 1)
		return(EXIT_FAILURE);
	
	closeSplash();
	m_ScribusInitialized=true;
//	ScQApp->setMainWidget(scribus);
	connect(ScQApp, SIGNAL(lastWindowClosed()), ScQApp, SLOT(quit()));

	scribus->show();
	int subsRet=scribus->ShowSubs();
	if (subsRet==0)
	{
		if (!m_Files.isEmpty())
		{
			for (int i = 0; i < m_Files.size(); ++i)
				scribus->loadDoc(m_Files.at(i));
		}
		else
		{
			if (PrefsManager::instance()->appPrefs.showStartupDialog)
				scribus->startUpDialog();
			else
				scribus->setFocus();
		}
	}	
	else if (subsRet==QMessageBox::Help)
	{
		scribus->slotRaiseOnlineHelp();
	}

	// A hook for plugins and scripts to trigger on. Some plugins and scripts
	// require the app to be fully set up (in particular, the main window to be
	// built and shown) before running their setup.
	emit appStarted();
	
	return EXIT_SUCCESS;
}

int ScribusCore::initScribusCore(bool showSplash, bool showFontInfo, bool showProfileInfo, 
								 const QString newGuiLanguage, const QString prefsUserFile)
{
	CommonStrings::languageChange();
	int retVal=0;
//FIXME	ExternalApp = 0;
	initSplash(showSplash);
	prefsManager = PrefsManager::instance();
	prefsManager->setup();
	//CB #4428 Get fonts before prefs are set to default
	bool haveFonts=false;
//#ifdef Q_OS_MAC
//	haveFonts=ScCore->initFonts(true);
//#else
	haveFonts=ScCore->initFonts(showFontInfo);
//#endif
	if (!haveFonts)
		return 1;
	prefsManager->initDefaults();
	prefsManager->initDefaultGUIFont(qApp->font());
	prefsManager->initArrowStyles();
	undoManager = UndoManager::instance();
	fileWatcher = new FileWatcher(this);
	pluginManager = new PluginManager();
	setSplashStatus( tr("Initializing Plugins") );
	pluginManager->initPlugs();
/* #4428, remove block later if ok
	bool haveFonts=false;
#ifdef Q_OS_MAC
	haveFonts=ScCore->initFonts(true);
#else
	haveFonts=ScCore->initFonts(showFontInfo);
#endif
	if (!haveFonts)
		return 1;
*/
	setSplashStatus( tr("Initializing Keyboard Shortcuts") );
	prefsManager->initDefaultActionKeys();
	setSplashStatus( tr("Reading Preferences") );
	if (prefsUserFile.isNull())
		prefsManager->ReadPrefs();
	else
		prefsManager->ReadPrefs(prefsUserFile);
//	setSplashStatus( tr("Applying User Shortcuts") );
//	prefsManager->applyLoadedShortCuts();
		
	m_HaveGS = testGSAvailability();
	m_HavePngAlpha = testGSDeviceAvailability("pngalpha");
	m_HaveTiffSep = testGSDeviceAvailability("tiffsep");
	
	ScCore->setSplashStatus( tr("Reading Color Profiles") );
	m_HaveCMS = false;
	getCMSProfiles(showProfileInfo);
	initCMS();

	return retVal;
}


void ScribusCore::initSplash(bool showSplash)
{
	if (showSplash)
	{
        QPixmap pix = loadIcon("Splash.png", true);
        m_SplashScreen = new SplashScreen(pix, Qt::WindowStaysOnTopHint);
        if (m_SplashScreen != NULL)
            m_SplashScreen->show();
		if (m_SplashScreen != NULL && m_SplashScreen->isVisible())
			setSplashStatus(QObject::tr("Initializing..."));
	}
	else
		m_SplashScreen = NULL;
}

void ScribusCore::setSplashStatus(const QString& newText)
{
	if (m_SplashScreen != NULL)
        m_SplashScreen->setStatus( newText );
}

void ScribusCore::showSplash(bool shown)
{
	if (m_SplashScreen!=NULL && shown!=m_SplashScreen->isVisible())
		m_SplashScreen->setVisible(shown);
}

bool ScribusCore::splashShowing() const
{
	if (m_SplashScreen != NULL)
		return m_SplashScreen->isVisible();
	return false;
}

void ScribusCore::closeSplash()
{
	if (m_SplashScreen!=NULL)
	{
		m_SplashScreen->close();
		delete m_SplashScreen;
		m_SplashScreen = NULL;
	}
}

bool ScribusCore::usingGUI() const
{
	return m_UseGUI;
}

bool ScribusCore::isMacGUI() const
{
	// Do it statically for now
#if defined(Q_WS_MAC)
	return true;
#else
	return false;
#endif
}

bool ScribusCore::isWinGUI() const
{
	// Do it statically for now
#if defined(_WIN32)
	return true;
#else
	return false;
#endif
}

bool ScribusCore::reverseDialogButtons() const
{
	if (m_SwapDialogButtonOrder)
		return true;
	//Win32 - dont switch
	#if defined(_WIN32)
		return false;
	//Mac Aqua - switch
	#elif defined(Q_WS_MAC)
		return true;
	#else
	//Gnome - switch
	QString gnomesession= ::getenv("GNOME_DESKTOP_SESSION_ID");
	if (!gnomesession.isEmpty())
		return true;

	//KDE/KDE Aqua - dont switch
	//Best guess for now if we are running under KDE
	QString kdesession= ::getenv("KDE_FULL_SESSION");
	if (!kdesession.isEmpty())
		return false;
	#endif
	return false;
}

//Returns false when there are no fonts
bool ScribusCore::initFonts(bool showFontInfo)
{
	setSplashStatus( tr("Searching for Fonts") );
	bool haveFonts=prefsManager->GetAllFonts(showFontInfo);
	if (!haveFonts)
	{
		closeSplash();
		QString mess = tr("There are no fonts found on your system.");
		mess += "\n" + tr("Exiting now.");
		QMessageBox::critical(0, tr("Fatal Error"), mess, 1, 0, 0);
	}
	else
		setSplashStatus( tr("Font System Initialized") );
	return haveFonts;
}


void ScribusCore::getCMSProfiles(bool showInfo)
{
	QString profDir;
	QStringList profDirs;
	MonitorProfiles.clear();
	PrinterProfiles.clear();
	InputProfiles.clear();
	InputProfilesCMYK.clear();
	QString pfad = ScPaths::instance().shareDir();
	pfad += "profiles/";
	profDirs = ScPaths::getSystemProfilesDirs();
	profDirs.prepend( prefsManager->appPrefs.ProfileDir );
	profDirs.prepend( pfad );
	for(int i = 0; i < profDirs.count(); i++)
	{
		profDir = profDirs[i];
		if(!profDir.isEmpty())
		{
			if(profDir.right(1) != "/")
				profDir += "/";
			getCMSProfilesDir(profDir, showInfo, true);
		}
	}
	if ((!PrinterProfiles.isEmpty()) && (!InputProfiles.isEmpty()) && (!MonitorProfiles.isEmpty()))
		m_HaveCMS = true;
	else
		m_HaveCMS = false;
}

void ScribusCore::getCMSProfilesDir(QString pfad, bool showInfo, bool recursive)
{
	QString profileName;
	QList<ScColorProfileInfo> profileInfos = ScColorMgmtEngine::getAvailableProfileInfo(pfad, recursive);
	for (int i = 0; i < profileInfos.count(); ++i)
	{
		const ScColorProfileInfo& profInfo = profileInfos.at(i);
		profileName = profInfo.description;
		if (profileName.isEmpty())
		{
			if (showInfo)
				sDebug(QString("Color profile %1 is broken : no valid description").arg(profInfo.file));
			continue;
		}
		if (!profInfo.debug.isEmpty())
		{
			if (showInfo)
				sDebug(profInfo.debug);
			continue;
		}
		switch (static_cast<int>(profInfo.deviceClass))
		{
		case Class_Input:
			if (profInfo.colorSpace == ColorSpace_Rgb)
			{
				if (!InputProfiles.contains(profileName))
					InputProfiles.insert(profileName, profInfo.file);
			}
			break;
		case Class_ColorSpace:
			if (profInfo.colorSpace == ColorSpace_Rgb)
			{
				if (!InputProfiles.contains(profileName))
					InputProfiles.insert(profileName, profInfo.file);
			}
			if (profInfo.colorSpace == ColorSpace_Cmyk)
			{
				if (!InputProfilesCMYK.contains(profileName))
					InputProfilesCMYK.insert(profileName, profInfo.file);
			}
			break;
		case Class_Display:
			if (profInfo.colorSpace == ColorSpace_Rgb)
			{
				if (!MonitorProfiles.contains(profileName))
					MonitorProfiles.insert(profileName, profInfo.file);
				if (!InputProfiles.contains(profileName))
					InputProfiles.insert(profileName, profInfo.file);
			}
			if (profInfo.colorSpace == ColorSpace_Cmyk)
			{
				if (!InputProfilesCMYK.contains(profileName))
					InputProfilesCMYK.insert(profileName, profInfo.file);
			}
			break;
		case Class_Output:
			// Disable rgb printer profile detection until effective support
			// PrinterProfiles.insert(nam, pfad + d[dc], false);
			if (profInfo.colorSpace == ColorSpace_Cmyk)
			{
				if (!PDFXProfiles.contains(profileName))
					PDFXProfiles.insert(profileName, profInfo.file);
				if (!InputProfilesCMYK.contains(profileName))
					InputProfilesCMYK.insert(profileName, profInfo.file);
				if (!PrinterProfiles.contains(profileName))
					PrinterProfiles.insert(profileName, profInfo.file);
			}
			break;
		}
		if (showInfo)
			sDebug( QString("Color profile %1 loaded from %2").arg(profileName).arg(profInfo.file) );
	}
}

void ScribusCore::InitDefaultColorTransforms(void)
{
	TermDefaultColorTransforms();

	// Ouvre le profile RGB par d�fault
	if (InputProfiles.contains("sRGB IEC61966-2.1"))
		defaultRGBProfile = ScColorMgmtEngine::openProfileFromFile(InputProfiles["sRGB IEC61966-2.1"]);
	else
		defaultRGBProfile = ScColorMgmtEngine::createProfile_sRGB();

	// Ouvre le profile CMYK par d�faut
	if (InputProfilesCMYK.contains("Fogra27L CMYK Coated Press"))
	{
		defaultCMYKProfile = ScColorMgmtEngine::openProfileFromFile(InputProfilesCMYK["Fogra27L CMYK Coated Press"]);
	}

	if (!defaultRGBProfile || !defaultCMYKProfile)
	{
		TermDefaultColorTransforms();
		return;
	}

	int dcmsFlags = Ctf_BlackPointCompensation | Ctf_LowResPrecalc;
	eRenderIntent intent = Intent_Relative_Colorimetric;

	defaultRGBToScreenSolidTrans  = ScColorMgmtEngine::createTransform(defaultRGBProfile, Format_RGB_16,
				defaultRGBProfile, Format_RGB_16, intent, Ctf_NullTransform);
	defaultRGBToScreenImageTrans  = ScColorMgmtEngine::createTransform(defaultRGBProfile, Format_RGBA_8,
				defaultRGBProfile, Format_RGBA_8, intent, Ctf_NullTransform);
	defaultCMYKToScreenImageTrans = ScColorMgmtEngine::createTransform(defaultCMYKProfile, Format_CMYK_8,
				defaultRGBProfile, Format_RGBA_8, intent, dcmsFlags);
	defaultRGBToCMYKTrans = ScColorMgmtEngine::createTransform(defaultRGBProfile, Format_RGB_16,
				defaultCMYKProfile, Format_CMYK_16, intent, dcmsFlags);
	defaultCMYKToRGBTrans = ScColorMgmtEngine::createTransform(defaultCMYKProfile, Format_CMYK_16,
				defaultRGBProfile, Format_RGB_16, intent, dcmsFlags);
	if (!defaultRGBToScreenSolidTrans  || !defaultRGBToScreenImageTrans || 
		!defaultCMYKToScreenImageTrans || !defaultRGBToCMYKTrans || 
		!defaultCMYKToRGBTrans )
	{
		TermDefaultColorTransforms();
	}
}

void ScribusCore::TermDefaultColorTransforms(void)
{
	defaultRGBProfile  = ScColorProfile();
	defaultCMYKProfile = ScColorProfile();
	defaultRGBToScreenSolidTrans  = ScColorTransform();
	defaultRGBToScreenImageTrans  = ScColorTransform();
	defaultCMYKToScreenImageTrans = ScColorTransform();
	defaultRGBToCMYKTrans = ScColorTransform();
	defaultCMYKToRGBTrans = ScColorTransform();
}

void ScribusCore::initCMS()
{
	if (m_HaveCMS)
	{
		ProfilesL::Iterator ip;
		if ((prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile.isEmpty()) || (!InputProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile)))
		{
			ip = InputProfiles.find("sRGB IEC61966-2.1");
			if (ip == InputProfiles.end())
				ip = InputProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile.isEmpty()) || (!InputProfilesCMYK.contains(prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile)))
		{
			ip = InputProfilesCMYK.find("Fogra27L CMYK Coated Press");
			if (ip == InputProfilesCMYK.end())
				ip = InputProfilesCMYK.begin();
			prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile.isEmpty()) || (!InputProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile)))
		{
			ip = InputProfiles.find("sRGB IEC61966-2.1");
			if (ip == InputProfiles.end())
				ip = InputProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultSolidColorCMYKProfile.isEmpty()) || (!InputProfilesCMYK.contains(prefsManager->appPrefs.DCMSset.DefaultSolidColorCMYKProfile)))
		{
			ip = InputProfilesCMYK.find("Fogra27L CMYK Coated Press");
			if (ip == InputProfilesCMYK.end())
				ip = InputProfilesCMYK.begin();
			prefsManager->appPrefs.DCMSset.DefaultSolidColorCMYKProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultMonitorProfile.isEmpty()) || (!MonitorProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultMonitorProfile)))
		{
			ip = MonitorProfiles.find("sRGB IEC61966-2.1");
			if (ip == MonitorProfiles.end())
				ip = MonitorProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultMonitorProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultPrinterProfile.isEmpty()) || (!PrinterProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultPrinterProfile)))
		{
			ip = PrinterProfiles.find("Fogra27L CMYK Coated Press");
			if (ip == PrinterProfiles.end())
				ip = PrinterProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultPrinterProfile = ip.key();
		}
		InitDefaultColorTransforms();
	}
}

ScribusMainWindow * ScribusCore::primaryMainWindow( )
{
	if (ScMWList.count() == 0 || m_currScMW > ScMWList.count())
		return 0;
	ScribusMainWindow* mw=ScMWList.at(m_currScMW);
	if (!mw)
		return 0;
	return mw;
}

void ScribusCore::recheckGS()
{
	m_HaveGS = testGSAvailability();
	m_HavePngAlpha = testGSDeviceAvailability("pngalpha");
	m_HaveTiffSep = testGSDeviceAvailability("tiffsep");
}

bool ScribusCore::fileWatcherActive() const
{
	if (fileWatcher!=NULL)
		return fileWatcher->isActive();
	return false;
}
