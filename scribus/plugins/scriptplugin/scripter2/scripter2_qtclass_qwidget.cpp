#include "cmdvar.h"

#include <qwidget.h>



void export_QWidget()
{
	using namespace boost::python;

	class_<QWidget,bases<QObject>,boost::noncopyable>(
			"QWidget",
			"A generic Qt widget")
		.def("show", &QWidget::show)
		.def("hide", &QWidget::hide);
}


