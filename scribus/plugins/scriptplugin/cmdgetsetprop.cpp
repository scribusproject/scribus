/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdgetsetprop.h"
#include "cmdutil.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QList>
#include <QObject>
#include <QObjectList>

QObject* getQObjectFromPyArg(PyObject* arg)
{
	if (PyString_Check(arg))
		// It's a string. Look for a pageItem by that name. Do NOT accept a
		// selection.
		return getPageItemByName(QString::fromUtf8(PyString_AsString(arg)));
	else if (PyCObject_Check(arg))
	{
		// It's a PyCObject, ie a wrapped pointer. Check it's not NULL
		// and return it.
		// FIXME: Try to check that its a pointer to a QObject instance
		QObject* tempObject = (QObject*)PyCObject_AsVoidPtr(arg);
		if (!tempObject)
		{
			PyErr_SetString(PyExc_TypeError, "INTERNAL: Passed NULL PyCObject");
			return NULL;
		}
		else
			return tempObject;
	}
	else
	{
		// It's not a type we know what to do with
		PyErr_SetString(PyExc_TypeError, QObject::tr("Argument must be page item name, or PyCObject instance").toLocal8Bit().constData());
		return NULL;
	}
}


PyObject* wrapQObject(QObject* obj)
{
	return PyCObject_FromVoidPtr((void*)obj, NULL);
}


const char* getpropertytype(QObject* obj, const char* propname, bool includesuper)
{
	const QMetaObject* objmeta = obj->metaObject();
	int i = objmeta->indexOfProperty(propname);
	if (i == -1)
		return NULL;
	QMetaProperty propmeta = objmeta->property(i);
	if (!propmeta.isValid())
		return NULL;
	const char* type = "";
//type=propmeta->type();
	assert(type);
	return type;
}


PyObject* scribus_propertyctype(PyObject* /*self*/, PyObject* args, PyObject* kw)
{
	PyObject* objArg = NULL;
	char* propertyname = NULL;
	int includesuper = 1;
	char* kwargs[] = {const_cast<char*>("object"),
					  const_cast<char*>("property"),
					  const_cast<char*>("includesuper"),
					  NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "Oes|i", kwargs,
				&objArg, "ascii", &propertyname, &includesuper))
		return NULL;

	// Get the QObject* the object argument refers to
	QObject* obj = getQObjectFromPyArg(objArg);
	if (!obj)
		return NULL;
	objArg = NULL; // no need to decref, it's borrowed

	// Look up the property and retrive its type information
	const char* type = getpropertytype( (QObject*)obj, propertyname, includesuper);
	if (type == NULL)
	{
		PyErr_SetString(PyExc_KeyError, QObject::tr("Property not found").toLocal8Bit().constData());
		return NULL;
	}
	return PyString_FromString(type);
}

PyObject* convert_QStringList_to_PyListObject(QStringList& origlist)
{
	PyObject* resultList = PyList_New(0);
	if (!resultList)
		return NULL;

	for ( QStringList::Iterator it = origlist.begin(); it != origlist.end(); ++it )
		if (PyList_Append(resultList, PyString_FromString((*it).toUtf8().data())) == -1)
			return NULL;

	return resultList;
}


PyObject* convert_QObjectList_to_PyListObject(QObjectList* origlist)
{
	PyObject* resultList = PyList_New(0);
	if (!resultList)
		return NULL;

	PyObject* objPtr = NULL;
	// Loop over the objects in the list and add them to the python
	// list wrapped in PyCObjects .
	for (int i = 0; i < origlist->count(); ++i)
	{
		// Wrap up the object pointer
		objPtr = wrapQObject(origlist->at(i));
		if (!objPtr)
		{
			// Failed to wrap the object. An exception is already set.
			Py_DECREF(resultList);
			return NULL;
		}
		// and add it to the list
		if (PyList_Append(resultList, (PyObject*)objPtr) == -1)
			return NULL;
	}
	return resultList;
}

/*Qt4 we either need to copy QObject::qChildHelper or rewrite this

PyObject* scribus_getchildren(PyObject* , PyObject* args, PyObject* kw)
{
	PyObject* objArg = NULL;
	char* ofclass = NULL;
	char* ofname = NULL;
	int recursive = 0;
	int regexpmatch = 0;
	char* kwnames[] = {const_cast<char*>("object"),
					   const_cast<char*>("ofclass"),
					   const_cast<char*>("ofname"),
					   const_cast<char*>("regexpmatch"),
					   const_cast<char*>("recursive"),
					   NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "O|esesii", kwnames,
				&objArg, "ascii", &ofclass, "ascii", &ofname, &regexpmatch, &recursive))
		return NULL;

	// Get the QObject* the object argument refers to
	QObject* obj = getQObjectFromPyArg(objArg);
	if (!obj)
		return NULL;
	objArg = NULL; // no need to decref, it's borrowed

	// Our job is to return a Python list containing the children of this
	// widget (as PyCObjects).
//qt4 FIXME	QObjectList* children;
//qt4 FIXME	children = obj->queryList(ofclass, ofname, regexpmatch, recursive);
	PyObject* itemlist = 0;
//qt4 FIXME 	itemlist = convert_QObjectList_to_PyListObject(children);
//qt4 FIXME	delete children;
	return itemlist;
}


// Perform a recursive (by default) search for the named child, possibly of a
// select class.
PyObject* scribus_getchild(PyObject* , PyObject* args, PyObject* kw)
{
	PyObject* objArg = NULL;
	char* childname = NULL;
	char* ofclass = NULL;
	bool recursive = true;
	char* kwnames[] = {const_cast<char*>("object"),
					   const_cast<char*>("childname"),
					   const_cast<char*>("ofclass"),
					   const_cast<char*>("recursive"),
					   NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "Oes|esi", kwnames,
				&objArg, "ascii", &childname, "ascii", &ofclass, &recursive))
		return NULL;

	// Get the QObject* the object argument refers to
	QObject* obj = getQObjectFromPyArg(objArg);
	if (!obj)
		return NULL;
	objArg = NULL; // no need to decref, it's borrowed

	// Search for the child, possibly restricting the search to children
	// of a particular type, and possibly recursively searching through
	// grandchildren etc.
	QObject* child = obj->child(childname, ofclass, recursive);
	if (child == NULL)
	{
		PyErr_SetString(PyExc_KeyError, QObject::tr("Child not found").toLocal8Bit().constData());
		return NULL;
	}

	return wrapQObject(child);
}
*/

PyObject* scribus_getpropertynames(PyObject* /*self*/, PyObject* args, PyObject* kw)
{
	PyObject* objArg = NULL;
	int includesuper = 1;
	char* kwargs[] = {const_cast<char*>("object"),
					  const_cast<char*>("includesuper"),
					  NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "O|i", kwargs,
				&objArg, &includesuper))
		return NULL;

	// Get the QObject* the object argument refers to
	QObject* obj = getQObjectFromPyArg(objArg);
	if (!obj)
		return NULL;
	objArg = NULL; // no need to decref, it's borrowed

	// Retrive the object's meta object so we can query it
	const QMetaObject* objmeta = obj->metaObject();
	assert(objmeta);

	// Return the list of properties
	QStringList propertyNames;
//qt4 FIXME	propertNames = objmeta->propertyNames(includesuper);
	return convert_QStringList_to_PyListObject(propertyNames);
}


PyObject* scribus_getproperty(PyObject* /*self*/, PyObject* args, PyObject* kw)
{
	PyObject* objArg = NULL;
	char* propertyName = NULL;
	char* kwargs[] = {const_cast<char*>("object"),
					  const_cast<char*>("property"),
					  NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "Oes", kwargs,
				&objArg, "ascii", &propertyName))
		return NULL;

	// Get the QObject* the object argument refers to
	QObject* obj = getQObjectFromPyArg(objArg);
	if (!obj)
		return NULL;
	objArg = NULL; // no need to decref, it's borrowed

	// Get the QMetaProperty for the property, so we can check
	// if it's a set/enum and do name/value translation.
/*qt4 FIXME	const QMetaObject* objmeta = obj->metaObject();
	int i = objmeta->findProperty(propertyName, true);
	if (i == -1)
	{
		PyErr_SetString(PyExc_ValueError,
				QObject::tr("Property not found"));
		return NULL;
	}

	const QMetaProperty* propmeta = objmeta->property(i, true);
	assert(propmeta);
*/
	// Get the property value as a variant type
	QVariant prop = obj->property(propertyName);

	// Convert the property to an instance of the closest matching Python type.
	PyObject* resultobj = NULL;
	// NUMERIC TYPES
	if (prop.type() == QVariant::Int)
		resultobj = PyLong_FromLong(prop.toInt());
	else if (prop.type() == QVariant::Double)
		resultobj = PyFloat_FromDouble(prop.toDouble());
	// BOOLEAN
	else if (prop.type() == QVariant::Bool)
		resultobj = PyBool_FromLong(prop.toBool());
	// STRING TYPES
	else if (prop.type() == QVariant::ByteArray)
		resultobj = PyString_FromString(prop.toByteArray().data());
	else if (prop.type() == QVariant::String)
		resultobj = PyString_FromString(prop.toString().toUtf8().data());
	// HIGHER ORDER TYPES
	else if (prop.type() == QVariant::Point)
	{
		// Return a QPoint as an (x,y) tuple.
		QPoint pt = prop.toPoint();
		return Py_BuildValue("(ii)", pt.x(), pt.y());
	}
	else if (prop.type() == QVariant::Rect)
	{
		// Return a QRect as an (x,y,width,height) tuple.
		// FIXME: We should really construct and return an object that
		// matches the API of QRect and has properties to keep
		// left/top/right/bottom and x/y/width/height in sync.
		QRect r = prop.toRect();
		return Py_BuildValue("(iiii)", r.x(), r.y(), r.width(), r.height());
	}
	else if (prop.type() == QVariant::StringList)
	{
		QStringList tmp = prop.toStringList();
		return convert_QStringList_to_PyListObject(tmp);
	}
	// UNHANDLED TYPE
	else
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("Couldn't convert result type '%1'.").arg(prop.typeName()).toLocal8Bit().constData() );
		return NULL;
	}

	// Return the resulting Python object
	if (resultobj == NULL)
	{
		// An exception was set while assigning to resultobj
		assert(PyErr_Occurred());
		return NULL;
	}
	else
		return resultobj;
}



PyObject* scribus_setproperty(PyObject* /*self*/, PyObject* args, PyObject* kw)
{
	PyObject* objArg = NULL;
	char* propertyName = NULL;
	PyObject* objValue = NULL;
	char* kwargs[] = {const_cast<char*>("object"),
					  const_cast<char*>("property"),
					  const_cast<char*>("value"),
					  NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kw, "OesO", kwargs,
				&objArg, "ascii", &propertyName, &objValue))
		return NULL;

	// We're going to hang on to the value object for a while, so
	// claim a reference to it.
	Py_INCREF(objValue);

	// Get the QObject* the object argument refers to
	QObject* obj = getQObjectFromPyArg(objArg);
	if (!obj)
		return NULL;
	objArg = NULL; // no need to decref, it's borrowed

	const QString propertyType = getpropertytype(obj, propertyName, true);

	// Did we know how to convert the value argument to the right type?
	bool matched = false;
	// Did the set call succceed?
	bool success = false;

	// Check the C++ type of the property, and try to convert the passed
	// PyObject to something sensible looking for it.
	// FIXME: handle enums/sets
	// NUMERIC TYPES
	// These are unfortuately a TOTAL PITA because of the multitude of
	// C and Python numeric types. TODO This needs to be broken out into a subroutine.
	if (propertyType == "bool")
	{
		matched = true;
		if (PyObject_IsTrue(objValue) == 0)
			success = obj->setProperty(propertyName, 0);
		else if (PyObject_IsTrue(objValue) == 1)
			success = obj->setProperty(propertyName, 1);
		else if (PyInt_Check(objValue))
			success = obj->setProperty(propertyName, PyInt_AsLong(objValue) == 0);
		else if (PyLong_Check(objValue))
			success = obj->setProperty(propertyName, PyLong_AsLong(objValue) == 0);
		else
			matched = false;
	}
	else if (propertyType == "int")
	{
		matched = true;
		if (PyObject_IsTrue(objValue) == 0)
			success = obj->setProperty(propertyName, 0);
		else if (PyObject_IsTrue(objValue) == 1)
			success = obj->setProperty(propertyName, 1);
		else if (PyInt_Check(objValue))
			success = obj->setProperty(propertyName, (int)PyInt_AsLong(objValue));
		else if (PyLong_Check(objValue))
			success = obj->setProperty(propertyName, (int)PyLong_AsLong(objValue));
		else
			matched = false;
	}
	else if (propertyType == "double")
	{
		matched = true;
		// FIXME: handle int, long  and bool too
		if (PyFloat_Check(objValue))
			success = obj->setProperty(propertyName, PyFloat_AsDouble(objValue));
		else
			matched = false;
	}
	// STRING TYPES
	else if (propertyType == "QString")
	{
		matched = true;
		if (PyString_Check(objValue))
			success = obj->setProperty(propertyName, QString::fromUtf8(PyString_AsString(objValue)));
		else if (PyUnicode_Check(objValue))
		{
			// Get a pointer to the internal buffer of the Py_Unicode object, which is UCS2 formatted
			const unsigned short * ucs2Data = (const unsigned short *)PyUnicode_AS_UNICODE(objValue);
			// and make a new QString from it (the string is copied)
			success = obj->setProperty(propertyName, QString::fromUtf16(ucs2Data));
		}
		else
			matched = false;
	}
	else if (propertyType == "QCString")
	{
		matched = true;
		if (PyString_Check(objValue))
		{
			// FIXME: should raise an exception instead of mangling the string when
			// out of charset chars present.
			QString utfString = QString::fromUtf8(PyString_AsString(objValue));
			success = obj->setProperty(propertyName, utfString.toAscii());
		}
		else if (PyUnicode_Check(objValue))
		{
			// Get a pointer to the internal buffer of the Py_Unicode object, which is UCS2 formatted
			const unsigned short * utf16Data = (const unsigned short *)PyUnicode_AS_UNICODE(objValue);
			// and make a new QString from it (the string is copied)
			success = obj->setProperty(propertyName, QString::fromUtf16(utf16Data).toAscii());
		}
		else
			matched = false;
	}
	// HIGHER ORDER TYPES
	// ... which I can't be stuffed supporting yet. FIXME.
	else
	{
		Py_DECREF(objValue);
		PyErr_SetString(PyExc_TypeError,
				QObject::tr("Property type '%1' not supported").arg(propertyType).toLocal8Bit().constData());
		return NULL;
	}

	// If `matched' is false, we recognised the C type but weren't able to
	// convert the passed Python value to anything suitable.
	if (!matched)
	{
		// Get a string representation of the object
		PyObject* objRepr = PyObject_Repr(objValue);
		Py_DECREF(objValue); // We're done with it now
		if (!objRepr)
			return NULL;
		// Extract the repr() string
		QString reprString = QString::fromUtf8(PyString_AsString(objRepr));
		Py_DECREF(objRepr);

		// And return an error
		PyErr_SetString(PyExc_TypeError,
				QObject::tr("Couldn't convert '%1' to property type '%2'").arg(reprString).arg(propertyType).toLocal8Bit().constData());
		return NULL;
	}

	// `success' is the return value of the setProperty() call
	if (!success)
	{
		Py_DECREF(objValue);
		PyErr_SetString(PyExc_ValueError, QObject::tr("Types matched, but setting property failed.").toLocal8Bit().constData());
		return NULL;
	}

	Py_DECREF(objValue);
//	Py_INCREF(Py_None);
//	return Py_None;
	Py_RETURN_NONE;
}

/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdgetsetpropdocwarnings()
{
    QStringList s;
    s << scribus_propertyctype__doc__ << scribus_getpropertynames__doc__ << scribus_getproperty__doc__ << scribus_setproperty__doc__;
	//Qt4 << scribus_getchildren__doc__ << scribus_getchild__doc__;
}
