#include "cmdvar.h"

#include <qmainwindow.h>

void export_QMainWindow()
{
	using namespace boost::python;

	class_<QMainWindow,bases<QWidget>,boost::noncopyable>(
			"QMainWindow",
			"A Qt main window widget");
}


