/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include <q3mainwindow.h>

void export_QMainWindow()
{
	using namespace boost::python;

	class_<Q3MainWindow,bases<QWidget>,boost::noncopyable>(
			"QMainWindow",
			"A Qt main window widget");
}


