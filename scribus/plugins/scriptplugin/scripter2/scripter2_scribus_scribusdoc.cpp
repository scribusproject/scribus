#include "cmdvar.h"

#include "scripter2_wrapper_pageitemlist.h"

using boost::python::list;
using boost::python::throw_error_already_set;

PageItemList getDocItems(ScribusDoc & doc)
{
	return PageItemList(doc.DocItems);
}

PageItemList getMasterItems(ScribusDoc & doc)
{
	return PageItemList(doc.MasterItems);
}

void export_ScribusDoc()
{
	using namespace boost::python;

	scope d = class_<ScribusDoc, bases<QObject>, boost::noncopyable>("ScribusDoc",
					"A Scribus document",
					no_init)
		.add_property("isModified", &ScribusDoc::isModified, &ScribusDoc::setModified)
		.add_property("items", &getDocItems)
		.add_property("masterPageItems", &getMasterItems);
}
