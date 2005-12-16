#include "cmdvar.h"

#include <qwidget.h>

#if defined(HAVE_BOOST_PYTHON)

void export_QObject()
{
	using namespace boost::python;

	class_<QObject, boost::noncopyable>(
			"QObject",
			"A generic Qt object");
}

#endif
