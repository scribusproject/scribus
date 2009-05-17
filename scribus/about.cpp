/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include <iostream> // only for debugging

#include <QFile>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QShowEvent>
#include <QString>
#include <QTabWidget>
#include <QTextStream>
#include <QToolTip>
#include <QWidget>

#include "about.h"
#include "commonstrings.h"
#include "scconfig.h"
#include "scpaths.h"
#include "sctextbrowser.h"

#ifdef HAVE_CAIRO
#include <cairo.h>
#endif

#include "util_ghostscript.h"
#include "util_icon.h"
#include "upgradechecker.h"
#include "langmgr.h"


/*
* The content for the About dialog is mostly built from the files in
* // SCRIBUS_INSTALL_DIRECTORY/share/doc/scribus-1.3.5svn/
* 
* The format of the files should follow this pattern:
* - AUTHOR
*   Tr(Title)
*   first_name name email_address
*   first_name name email_address
*   
*   Tr(Title)
*   ...
*
* - TRANSLATION
*   Tr(Title)
*   
*   Language(language_code)
*   first_name name (email_address)
*
*   Language(language_code)
*   first_name name (email_address)
*
*
*   Tr(Title)
*   ...
* - LINKS
*   Tr(Title)
*   url
*
*
*   Tr(Title)
*   ...
*
*
*  Text which has to be translated has to be added to the local tr...Title()
*  methods, otherwise in won't be added to the list of translatable strings
*
*  a.l.e
*/

/*!
\fn About::About( QWidget* parent, AboutMode diaMode )
\author Franz Schmid
\date
\brief Constructor for About dialog box
\param parent QWidget pointer to parent window
\param diaMode a dialog mode. See AboutMode.
\retval About dialog
*/
About::About( QWidget* parent, AboutMode diaMode ) : QDialog( parent )
{
	m_mode = diaMode;
	m_firstShow = true;
	setWindowTitle( tr("About Scribus %1").arg(VERSION) );
	setWindowIcon(loadIcon("AppIcon.png", true));
	setModal(true);
	aboutLayout = new QVBoxLayout( this );
	aboutLayout->setSpacing( 6 );
	aboutLayout->setMargin( 10 );
	tabWidget2 = new QTabWidget( this );
//	tabWidget2->setMinimumSize( QSize( 438, 258 ) );
	tab = new QWidget( tabWidget2 );
	tabLayout1 = new QVBoxLayout( tab );
	tabLayout1->setSpacing( 6 );
	tabLayout1->setMargin( 15 );
	pixmapLabel1 = new QLabel( tab );
	pixmapLabel1->setPixmap(loadIcon("scribus_logo.png", true));
	pixmapLabel1->setFixedSize(QSize(pixmapLabel1->pixmap()->width(), pixmapLabel1->pixmap()->height()));
	pixmapLabel1->setAlignment(Qt::AlignCenter);
	tabLayout1->addWidget( pixmapLabel1 );
	buildID = new QLabel( tab );
	buildID->setAlignment(Qt::AlignCenter);
	buildID->setTextInteractionFlags(Qt::TextSelectableByMouse);
	QString BUILD_DAY = "17";
	QString BUILD_MONTH = CommonStrings::may;
	QString BUILD_YEAR = "2009";
	QString BUILD_TIME = "";
	QString BUILD_TZ = "";
	QString BUILD_NAME = "";

// This is the old way:
	QString built = tr("%1 %2 %3").arg(BUILD_DAY).arg(BUILD_MONTH).arg(BUILD_YEAR);

// This is my way, only activated when envvar BUILD_NAME is set :-)  AV
//#include "about_builddate.inc"
	QString version = VERSION;
	if (BUILD_NAME != "")
		version += " \"" + BUILD_NAME + "\"";
	if (BUILD_NAME == "BleedingEdge")
		built = tr("%3-%2-%1 %4 %5").arg(BUILD_DAY).arg(BUILD_MONTH).arg(BUILD_YEAR).arg(BUILD_TIME).arg(BUILD_TZ);

	QString bu;
	bu += "C";
	bu += "-";
#ifdef HAVE_CUPS
	bu += "C";
#else
	bu += "*";
#endif
	bu += "-";
	bu += "T";
	bu += "-";
#ifdef HAVE_FONTCONFIG
	bu += "F";
#else
	bu += "*";
#endif
	bu += "-";
#ifdef HAVE_CAIRO
	bu += "C";
	bu += cairo_version_string();
#else
	bu += "Q";
#endif

// Some more information if we are not on a 32bit little endian Unix machine
#if defined(Q_OS_WIN)
	bu += "-Windows";
#elif defined(Q_OS_MAC)
	bu += "-Mac/";
#  if defined(Q_WS_MACX)
	bu += "Aqua";
#  elif define(Q_WS_X11)
	bu += "X11";
#  else
	bu += "?";
#  endif
#elif defined(Q_OS_DARWIN)
	// dunno if anyone uses this...
	bu += "-Darwin";
#endif
	if (QSysInfo::WordSize != 32)
		bu += QString("-%1bit").arg(QSysInfo::WordSize);
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
	if (QSysInfo::ByteOrder==QSysInfo::BigEndian)
		bu += "-Big";
#endif
	QString gsver(getGSVersion());
	if (!gsver.isEmpty())
		gsver = tr("Using Ghostscript version %1").arg(gsver);
	else
		gsver = tr("No Ghostscript version available");
	buildID->setText( tr("<p align=\"center\"><b>%1 %2</b></p><p align=\"center\">%3<br>%4 %5<br>%6</p>").arg( tr("Scribus Version")).arg(version).arg(built).arg( tr("Build ID:")).arg(bu).arg(gsver));
	tabLayout1->addWidget( buildID, 0, Qt::AlignHCenter );
	tabWidget2->addTab( tab, tr( "&About" ) );
	tab_2 = new QWidget( tabWidget2 );
	tabLayout = new QHBoxLayout( tab_2 );
	tabLayout->setSpacing( 6 );
	tabLayout->setMargin( 10 );


	/*! AUTHORS tab */
	// /usr/local/scribus14/share/doc/scribus-1.3.5svn/AUTHORS
	textView1 = new ScTextBrowser( tab_2 );
	textView1->setText(About::parseAuthorFile(ScPaths::instance().docDir() + "AUTHORS"));

	tabLayout->addWidget( textView1 );
	tabWidget2->addTab( tab_2, tr( "A&uthors" ) );
	tab_3 = new QWidget( tabWidget2 );
	tabLayout_2 = new QHBoxLayout( tab_3 );
	tabLayout_2->setSpacing( 6 );
	tabLayout_2->setMargin( 10 );
	textView2 = new ScTextBrowser( tab_3);
// 	LanguageManager langmgr;
// 	langmgr.init(false);

	/*! TRANSLATION tab */
	// /usr/local/scribus14/share/doc/scribus-1.3.5svn/TRANSLATION
	textView2->setText(About::parseTranslationFile(ScPaths::instance().docDir() + "TRANSLATION"));

	tabLayout_2->addWidget( textView2 );
	tabWidget2->addTab( tab_3, tr( "&Translations" ) );

	/*! ONLINE tab (03/04/2004 petr vanek) */
	// /usr/local/scribus14/share/doc/scribus-1.3.5svn/LINKS
	tab_4 = new QWidget( tabWidget2 );
	textView4 = new ScTextBrowser( tab_4 );
	textView4->setText(About::parseLinksFile(ScPaths::instance().docDir() + "LINKS"));
	tabLayout_4 = new QHBoxLayout( tab_4 );
	tabLayout_4->setSpacing( 6 );
	tabLayout_4->setMargin( 10 );
	tabLayout_4->addWidget( textView4 );
	tabWidget2->addTab( tab_4, tr( "&Online" ) );
	/*! UPDATE tab */
	tab_5 = new QWidget( tabWidget2 );
	tabWidget2->addTab( tab_5, tr( "&Updates" ) );
	updateLayout = new QVBoxLayout( tab_5 );
	updateLayout->setSpacing( 6 );
	updateLayout->setMargin( 10 );
	checkForUpdateButton = new QPushButton( tr( "Check for Updates" ), tab_5 );
	textView5 = new ScTextBrowser( tab_5);
	updateLayout->addWidget( checkForUpdateButton );
	updateLayout->addWidget( textView5 );
	
	/*! LICENCE tab */
	tab_Licence = new QWidget( tabWidget2 );
	tabWidget2->addTab( tab_Licence, tr( "&Licence" ) );
	licenceLayout = new QVBoxLayout( tab_Licence );
	licenceLayout->setSpacing( 6 );
	licenceLayout->setMargin( 10 );
	textViewLicence = new ScTextBrowser( tab_Licence);
	licenceLayout->addWidget( textViewLicence );
	
	QFile licenceFile(ScPaths::instance().docDir() + "/COPYING");
	if (!licenceFile.open(QIODevice::ReadOnly | QIODevice::Text))
		textViewLicence->setText( tr("Unable to open licence file. Please check your install directory or the Scribus website for licencing information.") );
	else
	{
		QTextStream inTS(&licenceFile);
		QString licenceText = inTS.readAll();
		textViewLicence->setText(licenceText);
	} 
	
	//Add tab widget to about window
	aboutLayout->addWidget( tabWidget2 );

	layout2 = new QHBoxLayout;
	layout2->setSpacing( 6 );
	layout2->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer );
	okButton = new QPushButton( tr( "&Close" ), this );
	okButton->setDefault( true );
	layout2->addWidget( okButton );
	aboutLayout->addLayout( layout2 );
	setMaximumSize(sizeHint());


	//tooltips
	buildID->setToolTip( "<qt>" + tr( "This panel shows the version, build date and compiled in library support in Scribus.")+"<br>"
	+ tr("The C-C-T-F equates to C=littlecms C=CUPS T=TIFF support F=Fontconfig support.Last Letter is the renderer C=cairo or Q=Qt")+"<br>"
	+ tr("Missing library support is indicated by a *. This also indicates the version of Ghostscript which Scribus has detected.")+"<br>"
	+ tr("The Windows version does not use fontconfig or CUPS libraries." ) + "</qt>" );
	checkForUpdateButton->setToolTip( "<qt>" + tr( "Check for updates to Scribus. No data from your machine will be transferred off it." ) + "</qt>" );
	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( checkForUpdateButton, SIGNAL( clicked() ), this, SLOT( runUpdateCheck() ) );
}


void About::showEvent (QShowEvent * event)
{
	if (m_mode == About::CheckUpdates && m_firstShow)
		tabWidget2->setCurrentIndex(4);
	QDialog::showEvent(event);
}



QString About::trAuthorTitle(QString title)
{
	QString result;
	if ( title == "Development Team:" )
		result = tr("Development Team:");
	else if ( title == "Mac OS&#174; X Aqua Port:" )
		result = tr("Mac OS&#174; X Aqua Port:");
	else if ( title == "OS/2&#174;/eComStation&#8482; Port:" )
		result = tr("OS/2&#174;/eComStation&#8482; Port:");
	else if ( title == "Windows&#174; Port:" )
		result = tr("Windows&#174; Port:");
	else if ( title == "Contributions from:" )
		result = tr("Contributions from:");
	else if ( title == "Official Documentation:" )
		result = tr("Official Documentation:");
	else if ( title == "Doc Translators:" )
		result = tr("Doc Translators:");
	else if ( title == "Other Documentation:" )
		result = tr("Other Documentation:");
	else if ( title == "Webmasters:" )
		result = tr("Webmasters:");
	else if ( title == "Splash Screen:" )
		result = tr("Splash Screen:");
	else if ( title == "Tango Project Icons:" )
		result = tr("Tango Project Icons:");
	else
	{
		std::cout << "please add the untranslated title \"" << qPrintable(title) << "\" to About::trAuthorTitle()" << std::endl;
		result = title;
	}
	return result;
}
QString About::trTranslationTitle(QString title)
{
	QString result;
	if ( title == "Official Translations and Translators:" )
		result = tr("Official Translations and Translators:");
	else if ( title == "Previous Translation Contributors:" )
		result = tr("Previous Translation Contributors:");
	else
	{
		std::cout << "please add the untranslated title \"" << qPrintable(title) << "\" to About::trAuthorTitle()" << std::endl;
		result = title;
	}
	return result;
}


QString About::trLinksTitle(QString title)
{
	QString result;
	if ( title == "Homepage" )
		result = tr("Homepage");
	else if ( title == "Online Reference" )
		result = tr("Online Reference");
	else if ( title == "Wiki" )
		result = tr("Wiki");
	else if ( title == "Bugs and Feature Requests" )
		result = tr("Bugs and Feature Requests");
	else if ( title == "Developer Blog" )
		result = tr("Developer Blog");
	else if ( title == "Mailing List" )
		result = tr("Mailing List");
	else
	{
		std::cout << "please add the untranslated title \"" << qPrintable(title) << "\" to About::trLinksTitle()" << std::endl;
		result = title;
	}
	return result;
}


/*!
* parse a text file and return an author list in an html table:
* left the names and right the contact address
* the html table can be "split" in sections
* @param QString fileName the file to be parsed
* @return QString the html table
*/
QString About::parseAuthorFile(QString fileName)
{
	QString result;
	QString file;
	QFile authorsFile(fileName);
	if (authorsFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream inTS(&authorsFile);
//		inTS.setCodec("UTF-8");
		QString lineTS;
		QStringList fieldTS;
		QString name;
		QString contact;
		bool isTitle = true;
		bool startText = false;
		bool startTitle = false;
		result = "<table>";
		while (!inTS.atEnd())
		{
			lineTS = inTS.readLine();
			// convert (r) "�" to &#174, "�" to &#8482
			// lineTS = QString::fromUtf8(lineTS);
			lineTS.replace("<", "&lt;");
			lineTS.replace(">", "&gt;");
			lineTS.replace("�", "&#174;");
			lineTS.replace("�", "&#8482;");
			name = "";
			contact = "";
			if (!lineTS.isEmpty())
			{
				if (isTitle)
				{
					startTitle = false;
					result += "<tr><td><b>"+About::trAuthorTitle(lineTS)+"</b></td><td></td></tr>";

				} // if is title
				else
				{
					startText = false;
					fieldTS = lineTS.split(" ");
					contact = (fieldTS.isEmpty() ? "" : fieldTS.takeLast());
					name = (fieldTS.isEmpty() ? "" : fieldTS.join(" "));
					result += "<tr><td>"+name+"</td><td>"+(contact == "@" ? "" : contact)+"</td></tr>";
				} // else is title
			} // if is empty line
			else
			{
				// empty lines switch between title and text
				if (!startText && !startTitle)
				{
					isTitle = !isTitle;
					if (isTitle)
					{
						result += "<tr><td></td><td></td></tr>";
						startTitle = true;
					}
					else
					{
						startText = true;
					}
				}
			} // else is empty line
		} // while ! atEnd
		result += "<table>";
	} // if file found
	else
	{
		if (!fileName.isEmpty())
		{
			QStringList field = fileName.split("/");
			if (!field.isEmpty())
			{
				file = field.takeLast();
			}
		}
		result = tr("Unable to open %1 file. Please check your install directory or the Scribus website for %1 information.").arg(file);
		result = "";
	} // else file found
	return result;
} // parseTextFile()

/*!
* parse a text file and return an author list in an html table:
* left the names and right the contact address
* the html table can be "split" in sections
* @param QString fileName the file to be parsed
* @return QString the html table
*/
QString About::parseTranslationFile(QString fileName)
{
	QString result;
	QString file;
	QFile translationFile(fileName);
	if (translationFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream inTS(&translationFile);
		QString lineTS;
		QStringList fieldTS;
		QString code;
		QString name;
		QString contact;
// 		LanguageManager langmgr;
// 		langmgr.init(false);
		bool isSectionTitle = true;
		bool isTitle = false;
		bool startText = false;
		bool startTitle = false;
		result = "<table>";
		while (!inTS.atEnd())
		{
			lineTS = inTS.readLine();
			lineTS.replace("<", "&lt;");
			lineTS.replace(">", "&gt;");
			name = "";
			contact = "";
			if (!lineTS.isEmpty())
			{
				if  (isSectionTitle)
				{
					result += "<tr><td><b><i>"+About::trTranslationTitle(lineTS)+"</i></b></td><td></td></tr>";
					isSectionTitle = false;
					isTitle = false;
					startTitle = false;
				}
				else if (isTitle)
				{
					startTitle = false;
					fieldTS = lineTS.split(" ");
					code = (fieldTS.isEmpty() ? "" : fieldTS.takeLast());
					if (!code.isEmpty())
					{
						code.replace("(", "");
						code.replace(")", "");
						code = LanguageManager::instance()->getLangFromAbbrev(code);
					}
					result += "<tr><td><b>"+code+"</b></td><td></td></tr>";
					isTitle = false;

				} // if is title
				else
				{
					startText = false;
					fieldTS = lineTS.split(" ");
					contact = (fieldTS.isEmpty() ? "" : fieldTS.takeLast());
					contact.replace("(", "");
					contact.replace(")", "");
					name = (fieldTS.isEmpty() ? "" : fieldTS.join(" "));
					result += "<tr><td>"+name+"</td><td>"+(contact == "@" ? "" : contact)+"</td></tr>";
				} // else is title
			} // if is empty line
			else
			{
				// empty lines switch between title and text
				if (!startText && !startTitle)
				{
					isTitle = !isTitle;
					if (isTitle)
					{
						result += "<tr><td></td><td></td></tr>";
						startTitle = true;
					}
					else
					{
						startText = true;
					}
				//
				// multiple empty lines start a section instead of marking a title
				}
				else if (startTitle)
				{
					isTitle = false;
					isSectionTitle = true;
				}
			} // else is empty line
		} // while ! atEnd
		result += "<table>";
	} // if file found
	else
	{
		if (!fileName.isEmpty())
		{
			QStringList field = fileName.split("/");
			if (!field.isEmpty())
			{
				file = field.takeLast();
			}
		}
		result = tr("Unable to open %1 file. Please check your install directory or the Scribus website for %1 information.").arg(file);
		result = "";
	} // else file found
	return result;
} // parseTranslationFile()

/*!
* parse a text file and return a links list in html format:
* left the names and right the contact address
* the html table can be "split" in sections
* @param QString fileName the file to be parsed
* @return QString the html table
*/
QString About::parseLinksFile(QString fileName)
{
	QString result;
	QString file;
	QFile linksFile(fileName);
	if (linksFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream inTS(&linksFile);
		inTS.setCodec("UTF-8");
		QString lineTS;
		bool isTitle = true;
		result = "<table>";
		while (!inTS.atEnd())
		{
			lineTS = inTS.readLine();
			// convert (r) "�" to &#174, "�" to &#8482
			// lineTS = QString::fromUtf8(lineTS);
			lineTS.replace("<", "&lt;");
			lineTS.replace(">", "&gt;");
			lineTS.replace("�", "&#174;");
			lineTS.replace("�", "&#8482;");
			if (!lineTS.isEmpty())
			{
				if (isTitle)
				{
					isTitle = false;
					result += "<tr><td><b>"+About::trLinksTitle(lineTS)+"</b></td></tr>";

				} // if is title
				else
				{
					result += "<tr><td><a href=\""+lineTS+"\">"+lineTS+"</a></td></tr>";
				} // else is title
			} // if is empty line
			else
			{
				// empty lines switch to title (one line)
			  result += "<tr><td></td></tr>";
			  isTitle = true;
			} // else is empty line
		} // while ! atEnd
		result += "<table>";
	} // if file found
	else
	{
		if (!fileName.isEmpty())
		{
			QStringList field = fileName.split("/");
			if (!field.isEmpty())
			{
				file = field.takeLast();
			}
		}
		result = tr("Unable to open %1 file. Please check your install directory or the Scribus website for %1 information.").arg(file);
		result = "";
	} // else file found
	return result;
} // parseLinksFile()

void About::setVisible (bool visible)
{
	QDialog::setVisible(visible);
	if (m_firstShow  && (m_mode == About::CheckUpdates) && visible)
	{
		m_firstShow = false;
		runUpdateCheck();
	}
}

void About::runUpdateCheck()
{
	textView5->clear();
	UpgradeCheckerGUI uc(textView5);
	disconnect( checkForUpdateButton, SIGNAL( clicked() ), this, SLOT( runUpdateCheck() ) );
	connect(checkForUpdateButton, SIGNAL( clicked() ), &uc, SLOT( abort() ));
	checkForUpdateButton->setText( tr("Abort Update Check") );
	uc.fetch();
	checkForUpdateButton->setText( tr("Check for Updates") );
	connect( checkForUpdateButton, SIGNAL( clicked() ), this, SLOT( runUpdateCheck() ) );
}
