/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"



#include "scribusapp.h"

extern ScribusQApp* ScQApp;

void export_ScribusQApp()
{
	using namespace boost::python;

	class_<ScribusQApp, bases<QApplication>, boost::noncopyable>("ScribusQApp",
			"The core application",
			no_init)
		.add_property("usingGUI", &ScribusQApp::usingGUI)
		.add_property("isMacGUI", &ScribusQApp::isMacGUI)
		.add_property("reverseDialogButtons", &ScribusQApp::reverseDialogButtons);

	scope().attr("ScQApp") = boost::ref(ScQApp);
}


