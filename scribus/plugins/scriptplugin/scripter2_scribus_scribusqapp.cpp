#include "cmdvar.h"

#if defined(HAVE_BOOST_PYTHON)

#include "scribusapp.h"

extern ScribusQApp* ScQApp;

void export_ScribusQApp()
{
	using namespace boost::python;

	class_<ScribusQApp, bases<QApplication>, boost::noncopyable>("ScribusQApp",
			"The core application",
			no_init);

	scope().attr("ScQApp") = boost::ref(ScQApp);
}

#endif
