/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include <QFrame>

void export_QFrame()
{
	using namespace boost::python;

	class_<QFrame,bases<QWidget>,boost::noncopyable>(
			"QFrame",
			"A generic Qt frame widget");
}


