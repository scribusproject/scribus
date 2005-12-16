#include "cmdvar.h"

#if defined(HAVE_BOOST_PYTHON)

#include "scribus.h"

void export_ScribusMainWindow()
{
	using namespace boost::python;

	class_<ScribusMainWindow, bases<QWidget>, boost::noncopyable>(
			"ScribusMainWindow",
			"The app's main window, which also provides much of its core functionality",
			no_init);
}

#endif
