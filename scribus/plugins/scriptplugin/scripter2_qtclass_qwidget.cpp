#include "cmdvar.h"

#include <qwidget.h>

#if defined(HAVE_BOOST_PYTHON)

void export_QWidget()
{
	using namespace boost::python;

	class_<QWidget,bases<QObject>,boost::noncopyable>(
			"QWidget",
			"A generic Qt widget");
}

#endif
