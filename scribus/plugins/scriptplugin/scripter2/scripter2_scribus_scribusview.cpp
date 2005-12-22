#include "cmdvar.h"

#include "scribusdoc.h"

void export_ScribusView()
{
	using namespace boost::python;

	scope w = class_<ScribusView, bases<QScrollView>, boost::noncopyable>("ScribusView",
					"The document display canvas",
					no_init);
}


