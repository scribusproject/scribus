/***************************************************************************
	begin                : May 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScApp program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef SCRIBUSAPP_H
#define SCRIBUSAPP_H
#include <qapplication.h>
#include <qstring.h>

#include "scribusapi.h"
class ScribusApp;

class SCRIBUS_API ScribusQApp : public QApplication
{
	Q_OBJECT

	public:
		ScribusQApp( int & argc, char ** argv );
		~ScribusQApp();
		int init();
		void initLang();
		void parseCommandLine();
		QStringList getLang(QString lang);
		void installTranslators(QStringList langs);
		void changeGUILanguage(QString);
		const bool usingGUI();
		
		static bool useGUI;
		
	private:
		ScribusApp *scribus;
		void showHeader();
		void showVersion();
		void showUsage();
		void showAvailLangs();
		
		QString lang;
		bool showSplash;
		bool showFontInfo;
		QString file;

	signals:
		void appStarted();
};
#endif
