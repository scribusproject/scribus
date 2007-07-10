/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include "scribusdoc.h"

void export_ScribusView()
{
	using namespace boost::python;

	scope w = class_<ScribusView, bases<QScrollView>, boost::noncopyable>("ScribusView",
					"The document display canvas",
					no_init);
}


