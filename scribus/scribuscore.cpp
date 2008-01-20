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

#include "cmserrorhandling.h"
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
#include "util_cms.h"
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


int ScribusCore::init(bool useGUI, bool swapDialogButtonOrder, const QString fileToUse)
{
	m_UseGUI=useGUI;
	m_File=fileToUse;
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
	scribus->ShowSubs();

	if (!m_File.isEmpty())
		scribus->loadDoc(m_File);
	else
	{
		if (PrefsManager::instance()->appPrefs.showStartupDialog)
			scribus->startUpDialog();
		else
			scribus->setFocus();
	}
	qApp->setOverrideCursor(QCursor(Qt::ArrowCursor));

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
#ifdef Q_OS_MAC
	haveFonts=ScCore->initFonts(true);
#else
	haveFonts=ScCore->initFonts(showFontInfo);
#endif
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
	/*
		initPalettes();

		setSplashStatus( tr("Initializing Hyphenator") );
		QString preLang = prefsManager->appPrefs.Language;
		initHyphenator();
		if (Sprachen.contains(preLang))
			prefsManager->appPrefs.Language = preLang;
		setSplashStatus( tr("Reading Scrapbook") );
		initScrapbook();
		setSplashStatus( tr("Setting up Shortcuts") );
		SetShortCut();
		scrActions["helpTooltips"]->setChecked(prefsManager->appPrefs.showToolTips);
		ToggleTips();

		connect(fileWatcher, SIGNAL(fileDeleted(QString )), this, SLOT(removeRecent(QString)));
		connect(this, SIGNAL(TextIFont(const QString&)), propertiesPalette, SLOT(setFontFace(const QString&)));
		connect(this, SIGNAL(TextISize(int)), propertiesPalette, SLOT(setSize(int)));
		connect(this, SIGNAL(TextUSval(int)), propertiesPalette, SLOT(setExtra(int)));
		connect(this, SIGNAL(TextStil(int)), propertiesPalette, SLOT(setStil(int)));
		connect(this, SIGNAL(TextScale(int)), propertiesPalette, SLOT(setTScale(int)));
		connect(this, SIGNAL(TextScaleV(int)), propertiesPalette, SLOT(setTScaleV(int)));
		connect(this, SIGNAL(TextBase(int)), propertiesPalette, SLOT(setTBase(int)));
		connect(this, SIGNAL(TextShadow(int, int )), propertiesPalette, SLOT(setShadowOffs(int, int )));
		connect(this, SIGNAL(TextOutline(int)), propertiesPalette, SLOT(setOutlineW(int)));
		connect(this, SIGNAL(TextUnderline(int, int)), propertiesPalette, SLOT(setUnderline(int, int)));
		connect(this, SIGNAL(TextStrike(int, int)), propertiesPalette, SLOT(setStrike(int, int)));
		connect(this, SIGNAL(TextFarben(QString, QString, int, int)), propertiesPalette, SLOT(setActFarben(QString, QString, int, int)));
	}*/

	return retVal;
}


void ScribusCore::initSplash(bool showSplash)
{
	if (showSplash)
	{
		m_SplashScreen = new SplashScreen();
		if (m_SplashScreen != NULL && m_SplashScreen->isVisible())
			setSplashStatus(QObject::tr("Initializing..."));
	}
	else
		m_SplashScreen = NULL;
}

void ScribusCore::setSplashStatus(const QString& newText)
{
	if (m_SplashScreen != NULL)
	{
		m_SplashScreen->setStatus( newText );
		qApp->processEvents();
	}
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
	QString pfad = ScPaths::instance().libDir();
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
	QDir d(pfad, "*", QDir::Name, QDir::Files | QDir::Readable | QDir::Dirs | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		QString nam = "";
		const char *Descriptor;
		cmsHPROFILE hIn = NULL;

		for (uint dc = 0; dc < d.count(); ++dc)
		{
			if (d[dc] == "." ||  d[dc] == "..")
				continue;
			QFileInfo fi(pfad + "/" + d[dc]);
			if (fi.isDir() && !recursive)
				continue;
			else if (fi.isDir() && d[dc][0] != '.')
			{
				getCMSProfilesDir(fi.filePath()+"/", showInfo, true);
				continue;
			}

			QFile f(fi.filePath());
			QByteArray bb(40, ' ');
			if (!f.open(QIODevice::ReadOnly)) {
				sDebug(QString("couldn't open %1 as color profile").arg(fi.filePath()));
				continue;
			}
			int len = f.read(bb.data(), 40);
			f.close();
			if (len == 40 && bb[36] == 'a' && bb[37] == 'c' && bb[38] == 's' && bb[39] == 'p')
			{
				const QByteArray profilePath( QString(pfad + d[dc]).toLocal8Bit() );
				if (setjmp(cmsJumpBuffer))
				{
					// Profile is broken, show info if necessary
					if (showInfo)
						sDebug(QString("Color profile %s is broken").arg(fi.filePath()));
					// Reset to the default handler otherwise may enter a loop
					// if an error occur in cmsCloseProfile()
					cmsSetErrorHandler(NULL);
					if (hIn)
					{
						cmsCloseProfile(hIn);
						hIn = NULL;
					}
					continue;
				}
				cmsSetErrorHandler(&cmsErrorHandler);
				hIn = cmsOpenProfileFromFile(profilePath.data(), "r");
				if (hIn == NULL)
					continue;
				Descriptor = cmsTakeProductDesc(hIn);
				nam = QString(Descriptor);
				switch (static_cast<int>(cmsGetDeviceClass(hIn)))
				{
				case icSigInputClass:
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
					{
						if (!InputProfiles.contains(nam))
							InputProfiles.insert(nam, pfad + d[dc]);
					}
					break;
				case icSigColorSpaceClass:
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
					{
						if (!InputProfiles.contains(nam))
							InputProfiles.insert(nam, pfad + d[dc]);
					}
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
					{
						if (!InputProfilesCMYK.contains(nam))
							InputProfilesCMYK.insert(nam, pfad + d[dc]);
					}
					break;
				case icSigDisplayClass:
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigRgbData)
					{
						if (!MonitorProfiles.contains(nam))
							MonitorProfiles.insert(nam, pfad + d[dc]);
						if (!InputProfiles.contains(nam))
							InputProfiles.insert(nam, pfad + d[dc]);
					}
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
					{
						if (!InputProfilesCMYK.contains(nam))
							InputProfilesCMYK.insert(nam, pfad + d[dc]);
					}
					break;
				case icSigOutputClass:
					// Disable rgb printer profile detection until effective support
					// PrinterProfiles.insert(nam, pfad + d[dc], false);
					if (static_cast<int>(cmsGetColorSpace(hIn)) == icSigCmykData)
					{
						if (!PDFXProfiles.contains(nam))
							PDFXProfiles.insert(nam, pfad + d[dc]);
						if (!InputProfilesCMYK.contains(nam))
							InputProfilesCMYK.insert(nam, pfad + d[dc]);
						if (!PrinterProfiles.contains(nam))
							PrinterProfiles.insert(nam, pfad + d[dc]);
					}
					break;
				}
				cmsCloseProfile(hIn);
				hIn = NULL;
				if (showInfo)
					sDebug( QString("Color profile %1 loaded from %2").arg(nam).arg(pfad + d[dc]) );
			}
		}
		cmsSetErrorHandler(NULL);
	}
}

void ScribusCore::InitDefaultColorTransforms(void)
{
	TermDefaultColorTransforms();

	cmsErrorAction(LCMS_ERROR_ABORT);
	if (setjmp(cmsJumpBuffer))
	{
		// Reset to the default handler otherwise may enter a loop
		// if an error occur afterwards
		cmsSetErrorHandler(NULL);
		cmsErrorAction(LCMS_ERROR_IGNORE);
		TermDefaultColorTransforms();
		cmsErrorAction(LCMS_ERROR_ABORT);
		return;
	}
	cmsSetErrorHandler(&cmsErrorHandler);

	// Ouvre le profile RGB par d�fault
	if (InputProfiles.contains("sRGB IEC61966-2.1"))
	{
		const QByteArray rgbProfPath(InputProfiles["sRGB IEC61966-2.1"].toLocal8Bit());
		defaultRGBProfile = cmsOpenProfileFromFile(rgbProfPath.data(), "r");
	}
	else
		defaultRGBProfile = cmsCreate_sRGBProfile();

	// Ouvre le profile CMYK par d�faut
	if (InputProfilesCMYK.contains("Fogra27L CMYK Coated Press"))
	{
		const QByteArray cmykProfPath(InputProfilesCMYK["Fogra27L CMYK Coated Press"].toLocal8Bit());
		defaultCMYKProfile = cmsOpenProfileFromFile(cmykProfPath.data(), "r");
	}

	if (!defaultRGBProfile || !defaultCMYKProfile)
	{
		TermDefaultColorTransforms();
		cmsSetErrorHandler(NULL);
		return;
	}

	int dcmsFlags = cmsFLAGS_LOWRESPRECALC | cmsFLAGS_BLACKPOINTCOMPENSATION;
	int intent    = INTENT_RELATIVE_COLORIMETRIC;

	defaultRGBToScreenSolidTrans  = scCmsCreateTransform(defaultRGBProfile, TYPE_RGB_16,
				defaultRGBProfile, TYPE_RGB_16, intent, cmsFLAGS_NULLTRANSFORM);
	defaultRGBToScreenImageTrans  = scCmsCreateTransform(defaultRGBProfile, TYPE_RGBA_8,
				defaultRGBProfile, TYPE_RGBA_8, intent, cmsFLAGS_NULLTRANSFORM);
	defaultCMYKToScreenImageTrans = scCmsCreateTransform(defaultRGBProfile, TYPE_CMYK_8,
				defaultCMYKProfile, TYPE_RGBA_8, intent, cmsFLAGS_NULLTRANSFORM);
	defaultRGBToCMYKTrans = scCmsCreateTransform(defaultRGBProfile, TYPE_RGB_16,
				defaultCMYKProfile, TYPE_CMYK_16, intent, dcmsFlags);
	defaultCMYKToRGBTrans = scCmsCreateTransform(defaultCMYKProfile, TYPE_CMYK_16,
				defaultRGBProfile, TYPE_RGB_16, intent, dcmsFlags);
	if (!defaultRGBToScreenSolidTrans  || !defaultRGBToScreenImageTrans || 
		!defaultCMYKToScreenImageTrans || !defaultRGBToCMYKTrans || 
		!defaultCMYKToRGBTrans )
	{
		TermDefaultColorTransforms();
	}
	cmsSetErrorHandler(NULL);
}

void ScribusCore::TermDefaultColorTransforms(void)
{
	if (defaultRGBProfile)
		cmsCloseProfile(defaultRGBProfile); 
	if (defaultCMYKProfile)
		cmsCloseProfile(defaultCMYKProfile);
	if (defaultRGBToScreenSolidTrans)
		cmsDeleteTransform(defaultRGBToScreenSolidTrans);
	if (defaultRGBToScreenImageTrans)
		cmsDeleteTransform(defaultRGBToScreenImageTrans);
	if (defaultCMYKToScreenImageTrans)
		cmsDeleteTransform(defaultCMYKToScreenImageTrans);
	if (defaultRGBToCMYKTrans)
		cmsDeleteTransform(defaultRGBToCMYKTrans);
	if (defaultCMYKToRGBTrans)
		cmsDeleteTransform(defaultCMYKToRGBTrans);
	defaultRGBProfile  = NULL;
	defaultCMYKProfile = NULL;
	defaultRGBToScreenSolidTrans  = NULL;
	defaultRGBToScreenImageTrans  = NULL;
	defaultCMYKToScreenImageTrans = NULL;
	defaultRGBToCMYKTrans = NULL;
	defaultCMYKToRGBTrans = NULL;
}

bool ScribusCore::IsDefaultProfile(cmsHPROFILE prof)
{
	if (prof == defaultRGBProfile || prof == defaultCMYKProfile)
		return true;
	return false;
}

bool ScribusCore::IsDefaultTransform(cmsHTRANSFORM trans)
{
	if (trans == defaultRGBToScreenSolidTrans || trans == defaultRGBToScreenImageTrans)
		return true;
	if (trans == defaultCMYKToScreenImageTrans)
		return true;
	if (trans == defaultRGBToCMYKTrans || trans == defaultCMYKToRGBTrans)
		return true;
	return false;
}

void ScribusCore::initCMS()
{
	if (m_HaveCMS)
	{
		ProfilesL::Iterator ip;
		if ((prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile.isEmpty()) || (!InputProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile)))
		{
			ip = InputProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultImageRGBProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile.isEmpty()) || (!InputProfilesCMYK.contains(prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile)))
		{
			ip = InputProfilesCMYK.begin();
			prefsManager->appPrefs.DCMSset.DefaultImageCMYKProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile.isEmpty()) || (!InputProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile)))
		{
			ip = InputProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultSolidColorRGBProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultSolidColorCMYKProfile.isEmpty()) || (!InputProfilesCMYK.contains(prefsManager->appPrefs.DCMSset.DefaultSolidColorCMYKProfile)))
		{
			ip = InputProfilesCMYK.begin();
			prefsManager->appPrefs.DCMSset.DefaultSolidColorCMYKProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultMonitorProfile.isEmpty()) || (!MonitorProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultMonitorProfile)))
		{
			ip = MonitorProfiles.begin();
			prefsManager->appPrefs.DCMSset.DefaultMonitorProfile = ip.key();
		}
		if ((prefsManager->appPrefs.DCMSset.DefaultPrinterProfile.isEmpty()) || (!PrinterProfiles.contains(prefsManager->appPrefs.DCMSset.DefaultPrinterProfile)))
		{
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
