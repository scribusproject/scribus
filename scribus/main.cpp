/***************************************************************************
						main.cpp  -  description
							-------------------
	begin                : Fre Apr  6 21:47:55 CEST 2001
	copyright            : (C) 2001 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
	copyright            : (C) 2004 by Alessandro Rimoldi
	email                : http://ideale.ch/contact
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <iostream>
#include <signal.h>

#include <qapplication.h>

#define BASE_QM "scribus"

#include "scribusapp.h"
#include "scribus.h"

#ifdef _MSC_VER
 #if (_MSC_VER >= 1200)
  #include "win-config.h"
 #endif
#else
 #include "config.h"
#endif

int mainApp(int argc, char **argv);
void initCrashHandler();
static void defaultCrashHandler(int sig);

ScribusApp* ScApp;
ScribusQApp* ScQApp;
bool emergencyActivated;

int main(int argc, char *argv[])
{
	return mainApp(argc, argv);
}

/*!
\fn int mainGui(int argc, char **argv)
\author Franz Schmid
\author Alessandro Rimoldi
\author Craig Bradney
\date Mon Feb  9 14:07:46 CET 2004
\brief Launches the Gui
\param int Number of arguments passed to Scribus
\param char *argv list of the arguments passed to Scribus
\retval int Error code from the execution of Scribus
*/
int mainApp(int argc, char **argv)
{
	emergencyActivated=false;
	ScribusQApp app(argc, argv);
	initCrashHandler();
	app.parseCommandLine();
	if (app.usingGUI())
	{
		int appRetVal=app.init();
		if (appRetVal==EXIT_FAILURE)
			return(EXIT_FAILURE);
		return app.exec();
	}
	return EXIT_SUCCESS;	
}

void initCrashHandler()
{
	typedef void (*HandlerType)(int);
	HandlerType handler	= 0;
	handler = defaultCrashHandler;
	if (!handler)
		handler = SIG_DFL;
	sigset_t mask;
	sigemptyset(&mask);
#ifdef SIGSEGV
	signal (SIGSEGV, handler);
	sigaddset(&mask, SIGSEGV);
#endif
#ifdef SIGFPE
	signal (SIGFPE, handler);
	sigaddset(&mask, SIGFPE);
#endif
#ifdef SIGILL
	signal (SIGILL, handler);
	sigaddset(&mask, SIGILL);
#endif
#ifdef SIGABRT
	signal (SIGABRT, handler);
	sigaddset(&mask, SIGABRT);
#endif
	sigprocmask(SIG_UNBLOCK, &mask, 0);
}

void defaultCrashHandler(int sig)
{
	static int crashRecursionCounter = 0;
	crashRecursionCounter++;
	signal(SIGALRM, SIG_DFL);
	if (crashRecursionCounter < 2)
	{
		emergencyActivated=true;
		crashRecursionCounter++;
		QString sigHdr=QObject::tr("Scribus Crash");
		QString sigLine="-------------";
		QString sigMsg=QObject::tr("Scribus crashes due to Signal #%1").arg(sig);
		std::cout << sigHdr << std::endl;
		std::cout << sigLine << std::endl;
		std::cout << sigMsg << std::endl;
		if (ScribusQApp::useGUI)
		{
			ScApp->closeSplash();
			QMessageBox::critical(ScApp, sigHdr, sigMsg, QObject::tr("&OK"));
			ScApp->emergencySave();
			ScApp->close();
		}
		alarm(300);
	}
	exit(255);
}
