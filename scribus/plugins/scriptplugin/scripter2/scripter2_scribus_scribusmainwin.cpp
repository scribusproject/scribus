/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include "scribus.h"
#include "scribuswin.h"
#include "scribusdoc.h"
#include "scribusview.h"

void export_ScribusMainWindow()
{
	using namespace boost::python;

	class_<ScribusMainWindow, bases<QWidget>, boost::noncopyable>(
			"ScribusMainWindow",
			"The app's main window, which also provides much of its core functionality",
			no_init)
		.add_property("doc", make_getter(&ScribusMainWindow::doc, return_internal_reference<>()))
		.add_property("view", make_getter(&ScribusMainWindow::view, return_internal_reference<>()))
		.add_property("ActWin", make_getter(&ScribusMainWindow::ActWin, return_internal_reference<>()));

	scope().attr("ScMW") = boost::ref(ScMW);
}


