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


