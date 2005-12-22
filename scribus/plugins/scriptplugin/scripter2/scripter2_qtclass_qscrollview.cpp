#include "cmdvar.h"

#include <qscrollview.h>

void export_QScrollView()
{
	using namespace boost::python;

	class_<QScrollView,bases<QFrame>,boost::noncopyable>(
			"QScrollView",
			"A generic Qt scroll view widget");
}


