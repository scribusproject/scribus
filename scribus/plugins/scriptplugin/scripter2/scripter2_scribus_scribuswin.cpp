#include "cmdvar.h"

#include "scribuswin.h"

void export_ScribusWin()
{
	using namespace boost::python;

	class_<ScribusWin, bases<QMainWindow>, boost::noncopyable>("ScribusWin",
			"A Scribus document window",
			no_init);
}
