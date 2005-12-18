#include "cmdvar.h"

#include <qwidget.h>



void export_QObject()
{
	using namespace boost::python;

	class_<QObject, boost::noncopyable>(
			"QObject",
			"A generic Qt object");
}


