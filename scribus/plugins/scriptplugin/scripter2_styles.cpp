#include "cmdvar.h"

#include "scribusstructs.h"
#include "scribusdoc.h"
#include "scribus.h"

// This file contains some basic methods for testing purposes,
// providing access to the docParagraphStyles member of the current
// ScribusDoc. It's a dirty hack that won't stay around.
//
// This will probably need to be replaced with a fake mapping class, since the
// styles are actually stored in a rather clumsy QValueList.

#if defined(HAVE_BOOST_PYTHON)

using namespace boost::python;

ParagraphStyle & getStyleRef(const QString & styleName)
{
	QValueList<ParagraphStyle>::iterator it(ScMW->doc->docParagraphStyles.begin());
	QValueList<ParagraphStyle>::iterator itEnd(ScMW->doc->docParagraphStyles.end());
	for ( ; it != itEnd; ++it)
	{
		if ((*it).Vname == styleName)
			return *it;
	}
	throw "Style not found";
}

ParagraphStyle & getStyleRefi(int index)
{
	return ScMW->doc->docParagraphStyles[index];
}

ParagraphStyle getStyleVal(const QString & styleName)
{
	return getStyleRef(styleName);
}

ParagraphStyle getStyleVali(int index)
{
	return getStyleRefi(index);
}

void addStyle(const ParagraphStyle & style)
{
	QValueList<ParagraphStyle>::iterator it(ScMW->doc->docParagraphStyles.begin());
	QValueList<ParagraphStyle>::iterator itEnd(ScMW->doc->docParagraphStyles.end());
	for ( ; it != itEnd; ++it)
	{
		if ((*it).Vname == style.Vname)
			throw "Style of same name already exists";
	}
	ScMW->doc->docParagraphStyles.append(style);
}

// This returns a COPY of the paragraph styles; modifications to this list do
// NOT affect the real paragraph style list. That'll have to happen much later,
// probably with a "fake mapping" class wrapper around the docParagraphStyles
// list, since we don't want Python users seeing the int-indexed list.
dict getStylesVal()
{
	dict d;
	QValueList<ParagraphStyle>::iterator it(ScMW->doc->docParagraphStyles.begin());
	QValueList<ParagraphStyle>::iterator itEnd(ScMW->doc->docParagraphStyles.end());
	for ( ; it != itEnd; ++it)
		d[(*it).Vname] = *it;
	return d;
}

dict getStylesRef()
{
	dict d;
	QValueList<ParagraphStyle>::iterator it(ScMW->doc->docParagraphStyles.begin());
	QValueList<ParagraphStyle>::iterator itEnd(ScMW->doc->docParagraphStyles.end());
	for ( ; it != itEnd; ++it)
		d[(*it).Vname] = boost::ref(*it);
	return d;
}

list getStyleNames()
{
	list l;
	QValueList<ParagraphStyle>::iterator it(ScMW->doc->docParagraphStyles.begin());
	QValueList<ParagraphStyle>::iterator itEnd(ScMW->doc->docParagraphStyles.end());
	for ( ; it != itEnd; ++it)
		l.append((*it).Vname);
	return l;
}

void nothing() { }

#endif

void export_styles()
{
#if defined(HAVE_BOOST_PYTHON)
	def("getStyleRef", getStyleRef, return_internal_reference<>());
	def("getStyleVal", getStyleVal);
	def("getStyleRefi", getStyleRefi, return_internal_reference<>());
	def("getStyleVali", getStyleVali);
	def("addStyle", addStyle);
	def("getStylesVal", getStylesVal);
	def("getStylesRef", getStylesRef);
	def("getStyleNames", getStyleNames);
#endif
}
