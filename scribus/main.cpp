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
#include "scribus.h"
#include "config.h"
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
		for (uint dc = 0; dc < d.count(); dc++)
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
  		std::cout << "Scribus Version " << VERSION << endl;
//			a.unlock();
  		return 0;
  		}
  	if (Arg1 == "--help")
  		{
  		std::cout << endl;
  		std::cout << "Scribus, a DTP-Program for Linux." << endl;
  		std::cout << endl;
  		std::cout << "Usage:" << endl;
  		std::cout << "scribus --version  -> prints Version-Number and exits." << endl;
  		std::cout << "scribus --help     -> prints this Info and exits." << endl;
  		std::cout << "scribus --lang xx  -> uses xx as Shortcut for a Language." << endl;
  		std::cout << "scribus \"String\"   -> Interprets \"String\" as Filename" << endl;
  		std::cout << "                      for a Document and tries to open it." << endl;
  		std::cout << endl;
//			a.unlock();
  		return 0;
  		}
  	if ((Arg1 != "--lang") && (Arg1 != "--help") && (Arg1 != "--version"))
			{
  		QFileInfo fi = QFileInfo(Arg1);
  		if (!fi.exists())
  			{
  			std::cout << "File does not exist, aborting." << endl;
  			std::cout << endl;
  			std::cout << "Usage:" << endl;
  			std::cout << "scribus --version  -> prints Version-Number and exits." << endl;
  			std::cout << "scribus --help     -> prints this Info and exits." << endl;
  			std::cout << "scribus --lang xx  -> uses xx as Shortcut for a Language." << endl;
  			std::cout << "scribus \"String\"   -> Interprets \"String\" as Filename" << endl;
  			std::cout << "                      for a Document and tries to open it." << endl;
  			std::cout << endl;
//				a.unlock();
  			return 0;
				}
  		}
  	}  
  ScribusApp *scribus=new ScribusApp();
  if (scribus->NoFonts)
  	return 0;
  a.setMainWidget(scribus);
  a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
  scribus->show();
  scribus->ShowSubs();
  if (argc > 1)
  	{
  	if ((Arg1 != "--lang") && (Arg1 != "--help") && (Arg1 != "--version"))
  		scribus->LadeDoc(argv[1]);
  	}
  return a.exec();
}
