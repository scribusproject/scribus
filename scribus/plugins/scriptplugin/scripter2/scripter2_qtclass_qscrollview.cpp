/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include <q3scrollview.h>
//Added by qt3to4:
#include <Q3Frame>

void export_QScrollView()
{
	using namespace boost::python;

	class_<Q3ScrollView,bases<Q3Frame>,boost::noncopyable>(
			"QScrollView",
			"A generic Qt scroll view widget");
}


