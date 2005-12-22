#include "cmdvar.h"

#include <qframe.h>

void export_QFrame()
{
	using namespace boost::python;

	class_<QFrame,bases<QWidget>,boost::noncopyable>(
			"QFrame",
			"A generic Qt frame widget");
}


