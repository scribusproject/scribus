/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

using boost::python::list;
using boost::python::throw_error_already_set;

/*
PageItemList getDocItems(ScribusDoc & doc)
{
	return PageItemList(doc.DocItems);
}

PageItemList getMasterItems(ScribusDoc & doc)
{
	return PageItemList(doc.MasterItems);
}
*/

void export_ScribusDoc()
{
	using namespace boost::python;

	scope d = class_<ScribusDoc, bases<QObject>, boost::noncopyable>("ScribusDoc",
					"A Scribus document",
					no_init)
		.add_property("isModified", &ScribusDoc::isModified, &ScribusDoc::setModified);
	/*
		.add_property("items", &getDocItems)
		.add_property("masterPageItems", &getMasterItems);
		*/
}
