#include "cmdvar.h"

#include <qapplication.h>

//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(QApplication_processEvents_overloads, QApplication::processEvents, 0, 1)

void QApplication_processEvents(QApplication& app)
{
	app.processEvents();
}

void QApplication_processEventsT(QApplication& app, int maxtime)
{
	app.processEvents(maxtime);
}

void export_QApplication()
{
	using namespace boost::python;

	class_<QApplication,boost::noncopyable>(
			"QApplication",
			"The base Qt application class",
			no_init)
		//.def("exec", &QApplication::exec); // BAD PLAN to expose this
		//.def("processEvents", &QApplication::processEvents);
		.def("processEvents", &QApplication_processEvents)
		.def("processEventsT", &QApplication_processEventsT);

	scope().attr("qApp") = boost::ref(qApp);
}
