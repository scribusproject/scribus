/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Fre Apr  6 21:47:55 CEST 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtranslator.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <cstdlib>
#include "splash.h"
#include "scribus.h"

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	QString pfad = PREL;
	pfad += "/lib/scribus/";
	QApplication a(argc, argv);
	QString Arg1, lang;
	lang = "";
	if (argc > 1)
		Arg1 = QString(argv[1]);
	QTranslator tor( 0 );
	if (argc > 1)
  	{
  		if (Arg1 == "--lang")
  		{
			if (QString(argv[2]) != "en")
			{
				lang = QString(argv[2]);
  				tor.load( QString(pfad+"scribus.") + QString(argv[2]), "." );
  				a.installTranslator( &tor );
			}
		}
		else
		{
			if (QString(QTextCodec::locale()).left(5) == "en_GB")
  				tor.load( QString(pfad+"scribus.") + "en_GB", "." );
		  	else
  				tor.load( QString(pfad+"scribus.") + QString(QTextCodec::locale()).left(2), "." );
			lang = QString(QTextCodec::locale()).left(2);
  			a.installTranslator( &tor );
		}
	}
	else
	{
		lang = QString(QTextCodec::locale()).left(2);
		if (QString(QTextCodec::locale()).left(5) == "en_GB")
  			tor.load( QString(pfad+"scribus.") + "en_GB", "." );
		else
 			tor.load( QString(pfad+"scribus.") + QString(QTextCodec::locale()).left(2), "." );
 		a.installTranslator( &tor );
	}
	pfad = PREL;
	pfad += "/lib/scribus/plugins/";
	QDir d(pfad, "*.*", QDir::Name, QDir::Files | QDir::NoSymLinks);
	if ((d.exists()) && (d.count() != 0))
	{
		for (uint dc = 0; dc < d.count(); ++dc)
		{
			QFileInfo fi(pfad + d[dc]);
			QString ext = fi.extension(false).lower();
			if (ext == "qm")
			{
				QString ext2 = fi.extension(true).lower();
				ext2 = ext2.left(2);
				if (ext2 == lang)
				{
					QTranslator *tox = new QTranslator(0);
					tox->load(QString(pfad+d[dc]), ".");
					a.installTranslator(tox);
				}
			}
		}
	}
	if (argc > 1)
  	{ 
  		if (Arg1 == "--version")
  		{
  			cout << "Scribus Version " << VERSION << endl;
//			a.unlock();
  			return 0;
  		}
  		if (Arg1 == "--help")
  		{
  			cout << endl;
  			cout << "Scribus, a DTP-Program" << endl;
  			cout << endl;
  			cout << "Usage:" << endl;
  			cout << "scribus --version  -> prints Version-Number and exits." << endl;
  			cout << "scribus --help     -> prints this Info and exits." << endl;
  			cout << "scribus --lang xx  -> uses xx as Shortcut for a Language." << endl;
  			cout << "scribus \"String\"   -> Interprets \"String\" as Filename" << endl;
  			cout << "                      for a Document and tries to open it." << endl;
  			cout << endl;
//			a.unlock();
  			return 0;
  		}
  		if ((Arg1 != "--lang") && (Arg1 != "--help") && (Arg1 != "--version") && (Arg1 != "--nosplash"))
		{
  			QFileInfo fi = QFileInfo(Arg1);
  			if (!fi.exists())
  			{
  				cout << "File does not exist, aborting." << endl;
  				cout << endl;
  				cout << "Usage:" << endl;
  				cout << "scribus --version  -> prints Version-Number and exits." << endl;
  				cout << "scribus --help     -> prints this Info and exits." << endl;
  				cout << "scribus --lang xx  -> uses xx as Shortcut for a Language." << endl;
  				cout << "scribus \"String\"   -> Interprets \"String\" as Filename" << endl;
  				cout << "                      for a Document and tries to open it." << endl;
  				cout << endl;
//				a.unlock();
  				return 0;
			}
  		}
  	}
	
	SplashScreen *splash = new SplashScreen();
 	splash->setStatus( QObject::tr("Initializing...") );
 	a.processEvents();
	ScribusApp *scribus = new ScribusApp(splash);
	if (scribus->NoFonts)
	{
    	delete splash;
    	exit(EXIT_FAILURE);
   	}
	a.setMainWidget(scribus);
   	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
   	splash->close();
   	delete splash;

	scribus->show();
	scribus->ShowSubs();
	if (argc > 1)
  	{
  		if ((Arg1 != "--lang") && (Arg1 != "--help") && (Arg1 != "--version"))
  			scribus->LadeDoc(argv[1]);
  	}
	return a.exec();
}
