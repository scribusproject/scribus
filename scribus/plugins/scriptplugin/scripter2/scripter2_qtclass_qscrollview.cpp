/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include <QScrollView>
#include <QFrame>

void export_QScrollView()
{
	using namespace boost::python;

	class_<QScrollView,bases<QFrame>,boost::noncopyable>(
			"QScrollView",
			"A generic Qt scroll view widget");
}


