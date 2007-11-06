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
#include "about.h"
#include <QPixmap>
#include <QToolTip>
#include <QLabel>
#include <QTabWidget>
#include <QWidget>
#include <QPushButton>
#include <QShowEvent>

#include "commonstrings.h"
#include "scconfig.h"
#include "sctextbrowser.h"

#ifdef HAVE_CAIRO
#include <cairo.h>
#endif

#include "util_ghostscript.h"
#include "util_icon.h"
#include "upgradechecker.h"
#include "langmgr.h"

/*!
 \fn About::About( QWidget* parent )
 \author Franz Schmid
 \date
 \brief Constructor for About dialog box
 \param parent QWidget pointer to parent window
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
	QString BUILD_DAY = "06";
	QString BUILD_MONTH = CommonStrings::november;
	QString BUILD_YEAR = "2007";
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
	if (!gsver.isNull())
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
	textView1 = new ScTextBrowser( tab_2 );
	textView1->setText(QString::fromUtf8("<table><tr><td><b>" + tr("Development Team:").toUtf8() + "</b></td><td> </td></tr>" +
											"<tr><td>Franz Schmid</td><td>Franz.Schmid@altmuehlnet.de</td></tr>" +
											"<tr><td>Peter Linnell</td><td>mrdocs@scribus.info</td></tr>" +
											"<tr><td>Craig Bradney</td><td>cbradney@zip.com.au</td></tr>" +
											"<tr><td>Petr Vaněk</td><td>petr@yarpen.cz</td></tr>" +
											"<tr><td>Riku Leino</td><td>riku@scribus.info</td></tr>" +
											"<tr><td>Craig Ringer</td><td>craig@postnewspapers.com.au</td></tr>" +
											"<tr><td>Andreas Vox</td><td>avox@arcor.de</td></tr>" +
											"<tr><td>Jean Ghali</td><td>jghali@libertysurf.fr</td></tr>" +
											"<tr><td>Oleksandr Moskalenko</td><td>malex@tagancha.org</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +

											"<tr><td><b>" + tr("Contributions from:").toUtf8()  + "</b></td><td></td></tr>" +
											"<tr><td>Niyam Bhushan</td><td></td></tr>" +
											"<tr><td>Martin Costabel</td><td></td></tr>" +
											"<tr><td>Steve Callcott</td><td>stotte@firstwish.co.uk</td></tr>" +
											"<tr><td>Frank Everdij</td><td>f.everdij@citg.tudelft.nl</td></tr>" +
											"<tr><td>S. Hakim Hamdani</td><td>hakimoto@eroteme.org</td></tr>" +
											"<tr><td>Maciej Hański</td><td>m.hanski@gmx.at</td></tr>" +
											"<tr><td>Paul F. Johnson</td><td>paul@all-the-johnsons.co.uk</td></tr>" +
											"<tr><td>Marti Maria</td><td></td></tr>" +
											"<tr><td>Alexandre Prokoudine</td><td>alexandre.prokoudine@gmail.com</td></tr>"
											"<tr><td>Alessandro Rimoldi</td><td>http://ideale.ch/contact</td></tr>" +
											"<tr><td>Alastair Robinson</td><td>blackfive@fakenhamweb.co.uk</td></tr>" +
											"<tr><td>Johannes Rüschel</td><td>jo.rueschel@gmx.de</td></tr>" +
											"<tr><td>Jean-Jacques Sarton</td><td>jj.sarton@t-online.de</td></tr>" +
											"<tr><td>Christian Töpp</td><td>mr-ct@gmx.de</td></tr>" +
											"<tr><td>Eirik Øverby</td><td>ltning@anduin.net</td></tr>" +
											"<tr><td>Kevin Young</td><td>k.young@youngscomputing.com</td></tr>" +
											"<tr><td></td><td></td></tr>" +

											"<tr><td><b>" + tr("Mac OS&#174; X Aqua Port:").toUtf8()  + "</b></td><td> </td></tr>" +
											"<tr><td>Andreas Vox</td><td>avox@arcor.de</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +

											"<tr><td><b>" + tr("OS/2&#174;/eComStation&#8482; Port:").toUtf8()  + "</b></td><td> </td></tr>" +
											"<tr><td>Paul Smedley</td><td>paul@smedley.info</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +

											"<tr><td><b>" + tr("Windows&#174; Port:").toUtf8()  + "</b></td><td> </td></tr>" +
											"<tr><td>Jean Ghali</td><td>jghali@libertysurf.fr</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +

											"<tr><td><b>" + tr("Official Documentation:").toUtf8()  + "</b></td><td></td></tr>" +
											"<tr><td>Peter Linnell</td><td>mrdocs@scribus.info</td></tr>" +
											"<tr><td>Craig Bradney</td><td>cbradney@zip.com.au</td></tr>" +
											"<tr><td>Frédéric Dubuy</td><td>effediwhy@gmail.com</td></tr>" +
											"<tr><td>Johannes Rüschel</td><td>jo.rueschel@gmx.de</td></tr>"
											+"<tr><td>Volker Ribbert</td><td>volkerius@h3c.de</td></tr>" +
											"<tr><td>Louis Desjardins</td><td>louisdesjardins@videotron.ca</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + tr("Other Documentation:").toUtf8()  + "</b></td><td></td></tr>" +
											"<tr><td>Yves Ceccone</td><td>yves@yeccoe.org</td></tr>" +
											"<tr><td>Holger Reibold</td><td>http://www.bomots.de/scribus/</td></tr>" +
											"<tr><td>Thomas Zastrow</td><td>webmaster@thomas-zastrow.de</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + tr("Splash Screen:").toUtf8()  + "</b></td><td></td></tr>" +
											"<tr><td>Andy Fitzsimon</td><td>andrew@fitzsimon.com.au</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + tr("Tango Project Icons:").toUtf8()  + "</b></td><td></td></tr>" +
											"<tr><td>Andreas Nilsson</td><td>nisses.mail@home.se</td></tr>" +
											"<tr><td>Jakub Steiner</td><td>jimmac@ximian.com</td></tr>"  +
											"<tr><td>Antoine Cailliau</td><td>antoinecailliau@gmail.com</td></tr>"  +
											"<tr><td> </td><td> </td></tr>" + "</table>"));
	tabLayout->addWidget( textView1 );
	tabWidget2->addTab( tab_2, tr( "A&uthors" ) );
	tab_3 = new QWidget( tabWidget2 );
	tabLayout_2 = new QHBoxLayout( tab_3 );
	tabLayout_2->setSpacing( 6 );
	tabLayout_2->setMargin( 10 );
	textView2 = new ScTextBrowser( tab_3);
	LanguageManager langmgr;
	langmgr.init(false);
	textView2->setText(QString::fromUtf8( "<table><tr><td><b><i>" + tr("Official Translations and Translators:").toUtf8() + "</i></b></td><td></td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("af").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Kobus Wolvaardt</td><td>kobuswolf@diewereld.co.za</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" +
											langmgr.getLangFromAbbrev("ar").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>حاتم المسلم</td><td>dr.hatim@hotmail.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" +langmgr.getLangFromAbbrev("eu").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Iñaki Larrañaga Murgoitio</td><td>dooteo@euskalgnu.org</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("br").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Alan Monfort</td><td>alan.monfort@free.fr</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("ca").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Xavier Sala Pujolar</td><td>utrescu@xaviersala.net</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("zh_TW").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Gilbert Su</td><td>gilbert_su@yahoo.com.tw</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("cs").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Petr Vaněk</td><td>petr@yarpen.cz</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("da").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Morten Langlo</td><td>mlanglo@mail.dk</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("nl").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Foppe Benedictus</td><td>foppe.benedictus@gmail.coml</td></tr>" +
											"<tr><td>Erik Collou</td><td>collou@gmx.net</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" +
											langmgr.getLangFromAbbrev("en_AU").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Craig Bradney</td><td>cbradney@zip.com.au</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" +
											langmgr.getLangFromAbbrev("en_GB").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Craig Bradney</td><td>cbradney@zip.com.au</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("eo").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Pier Luigi Cinquantini</td><td>plcinquantini@katamail.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("et").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Marek Laane</td><td>bald@starman.ee</td></tr>" +
											"<tr><td>Hasso Tepper</td><td>hasso@estpak.ee</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("fi").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Riku Leino</td><td>riku@scribus.info</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("fr").toLocal8Bit()  + "</b></td><td> </td>" +
											"<tr><td>Nicolas Boos</td><td>nicolas.boos@wanadoo.fr</td></tr>" +
											"<tr><td>Louis Desjardins</td><td>louis_desjardins@mardigrafe.com</td></tr>" +
											"<tr><td>Frédéric Dubuy</td><td>effediwhy@gmail.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("de").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Johannes Rüschel</td><td>jo.rueschel@gmx.de</td></tr>" +
											"<tr><td>Franz Schmid</td><td>Franz.Schmid@altmuehlnet.de</td></tr>" +
											"<tr><td>Christoph Schäfer</td><td>christoph-schaefer@gmx.de</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("de_CH").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Christoph Schäfer</td><td>christoph-schaefer@gmx.de</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("de_ol").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Johannes Rüschel</td><td>jo.rueschel@gmx.de</td></tr>" +
											"<tr><td>Christoph Schäfer</td><td>christoph-schaefer@gmx.de</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("ga").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Xose Calvo</td><td>xosecalvo@galizaweb.net</td></tr>" +
											"<tr><td>Manuel Anxo Rei</td><td>manxopar@avogaciagalega.org</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("el").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Τούσης Μανώλης </br> Toussis Manolis</td><td>manolis@koppermind.homelinux.org</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("hu").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Csaba Zakarias</td><td>csaba.zakarias@gmail.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("it").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Pier Luigi Cinquantini</td><td>plcinquantini@katamail.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("ja").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Shushi Kurose</td><td>md81@bird.email.ne.jp</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("ko").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Kitae Kim</td><td>neeum@yahoo.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("nb").toUtf8()  + "</b></td><td></td></tr>" +
											"<tr><td>Axel Bojer</td><td>axelb@skolelinux.no</td></tr>" +
											"<tr><td>Klaus Ade Johnstad</td><td>klaus@inout.no</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("pl").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Maciej Hański</td><td>m.hanski@gmx.at</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("pt").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Fausto Guilherme</td><td>faustoguilherme@gmail.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("pt_BR").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Ludi Maciel</td><td>iludi@uol.com.br</td></tr>" +
											"<tr><td>Frederico Gon&#231;alves</td><td></td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("ru").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Александр Прокудин </td><td>alexandre.prokoudine@gmail.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("sr").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Bojan Božovi&#263;</td><td>bole89@infosky.net</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("sk").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Zdenko Podobn&yacute;</td><td>zdposter@gmail.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("sl").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Boštjan Špetič</td><td>igzebedze@kiberpipa.org</td></tr>" +
											"<tr><td>Peter Čuhalev</td><td>skatey@slocartoon.net</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("es_ES").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Josep Febrer</td><td>josep@linuxmail.org</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("sv").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Göran Bondeson</td><td>goran@bondeson.net</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("th_TH").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Sira Nokyoungthong</td><td>Sira Nokyoungthong</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("tr").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Barış Atasoy</td><td>batasoy@pozitifpc.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("uk").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Oleksandr Moskalenko</td><td>malex@tagancha.org</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("cy").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Kevin Donnelly</td><td>kevin@dotmon.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"</table>" +
											"<table><tr><td><b><i>" + tr("Previous Translation Contributors:").toLocal8Bit() + "</i></b></td><td></td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("eu").toLocal8Bit() + "</b></td><td></td></tr>" +
											"<tr><td>Pablo Saratxaga</td><td>pablo@mandrakesoft.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("pt_BR").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Celio Santos</td><td>celio@electronic.srv.br</td></tr>" +
											"<tr><td>Cezar de Souza Marson Nido</td><td>cesar@electronic.srv.br</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("bg").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Vasko Tomanov</td><td>vasko@web.bg</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("nl").toUtf8()  + "</b></td><td></td></tr>" +
											"<tr><td>Wilbert Berendsen</td><td>wbsoft@xs4all.nl</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("en_GB").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Paul F. Johnson</td><td>paul@all-the-johnsons.co.uk</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("fr").toLocal8Bit()  + "</b></td><td> </td>" +
											"<tr><td>Michel Briand</td><td>michelbriand@free.fr</td></tr>" +
											"<tr><td>Yves Ceccone</td><td>yves@yeccoe.org</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +

											"<tr><td><b>" + langmgr.getLangFromAbbrev("hu").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Giovanni Biczó</td><td>gbiczo@freestart.hu</td></tr>" +
											"<tr><td>Bence Nagy</td><td>scribus@tipogral.hu</td></tr>" +
											"<tr><td>Zoltán Böszörményi</td><td>zboszor@freemail.hu</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("lt").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Aivaras Kirejevas</td><td>kiras@mail.lt</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("nb").toLocal8Bit()  + "</b></td><td></td></tr>" +
											"<tr><td>Johannes Wilm</td><td>j@indymedia.no</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("tr").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Erkan Kaplan</td><td>Selamsana@uni.de</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"<tr><td><b>" + langmgr.getLangFromAbbrev("uk").toLocal8Bit()  + "</b></td><td> </td></tr>" +
											"<tr><td>Sergiy Kudryk</td><td>kudryk@yahoo.com</td></tr>" +
											"<tr><td> </td><td> </td></tr>" +
											"</table>"));
	tabLayout_2->addWidget( textView2 );
	tabWidget2->addTab( tab_3, tr( "&Translations" ) );

	// online tab (03/04/2004 petr vanek)
	tab_4 = new QWidget( tabWidget2 );
	textView4 = new ScTextBrowser( tab_4 );
	textView4->setText(QString::fromUtf8(
		"<table><tr><td><b>" + tr("Homepage").toUtf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://www.scribus.net\">http://www.scribus.net</a></p></td></tr>" +
		"<tr><td><b>" + tr("Online Reference").toUtf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://docs.scribus.net\">http://docs.scribus.net</a></p></td></tr>" +
		"<tr><td><b>" + tr("Wiki").toUtf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://wiki.scribus.net\">http://wiki.scribus.net</a></p></td></tr>" +
		"<tr><td><b>" + tr("Bugs and Feature Requests").toUtf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://bugs.scribus.net\">http://bugs.scribus.net</a></p></td></tr>" +
		"<tr><td><b>" + tr("Mailing List").toUtf8() + "</b></td><td></td></tr>" +
		"<tr><td colspan=\"2\"><p><a href=\"http://nashi.altmuehlnet.de/mailman/listinfo/scribus\">http://nashi.altmuehlnet.de/mailman/listinfo/scribus</a></p></td></tr>" +
		"</table>"));
	tabLayout_4 = new QHBoxLayout( tab_4 );
	tabLayout_4->setSpacing( 6 );
	tabLayout_4->setMargin( 10 );
	tabLayout_4->addWidget( textView4 );
	tabWidget2->addTab( tab_4, tr( "&Online" ) );
	// Update tab
	tab_5 = new QWidget( tabWidget2 );
	tabWidget2->addTab( tab_5, tr( "&Updates" ) );
	updateLayout = new QVBoxLayout( tab_5 );
	updateLayout->setSpacing( 6 );
	updateLayout->setMargin( 10 );
	checkForUpdateButton = new QPushButton( tr( "Check for Updates" ), tab_5 );
	textView5 = new ScTextBrowser( tab_5);
	updateLayout->addWidget( checkForUpdateButton );
	updateLayout->addWidget( textView5 );

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
