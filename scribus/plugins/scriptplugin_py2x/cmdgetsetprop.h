/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMD_GETSETPROP_H
#define CMD_GETSETPROP_H

// Pulls in Python.h first
#include "cmdvar.h"
#include <QStringList>


/**
 * @brief Return a pointer to the QObject that the PyObject* args refers to
 *
 * This function looks up the PyObject* argument and tries to return a
 * corresponding QObject.
 *
 * If the argument is a PyCObject, it will return the contained pointer.
 * If the pointer is NULL an exception is set before returning.
 *
 * If the argument is a string, a PageItem by that name will be looked for
 * and, if found, returned. If no such object could be found, an exception
 * is set and NULL is returned.
 *
 * If any other type is specified, an exception is set and NULL is returned.
 *
 * @attention may return NULL with an exception set
 *
 * @sa wrapQObject()
 */
QObject* getQObjectFromPyArg(PyObject* arg);


/**
 * @brief Return a PyCObject containing the passed QObject pointer
 * @sa getQObjectFromPyArg()
 * @attention may return NULL with an exception set
 */
PyObject* wrapQObject(QObject* obj);


/**
 * @brief Convert a QStringList to a Python list of str objects
 * @attention may return NULL with an exception set
 */
PyObject* convert_QStringList_to_PyListObject(QStringList& origlist);

/**
 * @brief Convert a QObjectList to a Python list of PyCObjects
 * @attention may return NULL with an exception set
 */
PyObject* convert_QObjectList_to_PyListObject(QObjectList* origlist);

/**
 * @brief return the name of the C type of the specified property
 *
 * Look up `propname' on `obj', searching through parent classes if
 * `includsuper' is true. Get and return the name of the C type
 * of the property.
 *
 * If the property cannot be found, a null string is returned. NO
 * EXCEPTION IS SET.
 *
 * @sa scribus_propertyctype()
 */
const char* getpropertytype(QObject* obj, const char* propname, bool includesuper = true);

/**
 * @brief Get name of C type of property of object
 * @returns Python string object containing name of C type of property.
 * @sa getpropertytype(), scribus_getproperty(), scribus_setproperty()
 */
PyDoc_STRVAR(scribus_propertyctype__doc__,
QT_TR_NOOP("getPropertyCType(object, property, includesuper=True)\n\
\n\
Returns the name of the C type of `property' of `object'. See getProperty()\n\
for details of arguments.\n\
\n\
If `includesuper' is true, search inherited properties too.\n\
"));
PyObject* scribus_propertyctype(PyObject* /*self*/, PyObject* args, PyObject* kw);


/**
 * @brief Return a list of the property names of the passed object
 *
 * See the Python docstring for details on this function.
 *
 * @sa QMetaObject::propertyNames(), scribus_getproperty(),
 *     scribus_setproperty()
 */
PyDoc_STRVAR(scribus_getpropertynames__doc__,
QT_TR_NOOP("getPropertyNames(object, includesuper=True)\n\
\n\
Return a list of property names supported by `object'.\n\
If `includesuper' is true, return properties supported\n\
by parent classes as well.\n\
"));
PyObject* scribus_getpropertynames(PyObject* self, PyObject* args, PyObject* kw);


/**
 * @brief Generic Python getter for Qt object properties
 *
 * This getter uses the Qt property interface (see the Qt object
 * model documentation) to provide a generic getter for any property
 * of a supported type. It finds the closest Python type match to the
 * property's C++ type, converts the property value to the Python type,
 * and returns it.
 *
 * If an unsupported type is encountered, TypeError will be raised.
 *
 * See the Python docstring for details of this function's Python
 * interface.
 *
 * @sa scribus_setproperty(), QObject::property(), QMetaObject.
 *     scribus_propertyctype()
 */
PyDoc_STRVAR(scribus_getproperty__doc__,
QT_TR_NOOP("getProperty(object, property)\n\
\n\
Return the value of the property `property' of the passed `object'.\n\
\n\
The `object' argument may be a string, in which case the named PageItem\n\
is searched for. It may also be a PyCObject, which may point to any\n\
C++ QObject instance.\n\
\n\
The `property' argument must be a string, and is the name of the property\n\
to look up on `object'.\n\
\n\
The return value varies depending on the type of the property.\n\
"));
PyObject* scribus_getproperty(PyObject* /*self*/, PyObject* args, PyObject* kw);


/**
 * @brief Generic setter for Qt object properties
 *
 * This setter uses the Qt property intercace to provide a generic setter
 * for any property of a supported type. It checks the Property's C type
 * and attempts to convert the passed Python object to a compatible type,
 * then call the object's property setter method with the converted value.
 *
 * If an unsupported type is encountered, TypeError will be raised.
 * If the passed object can't be converted to a compatible type,
 * TypeError will be raised.
 *
 * See the Python docstring for details of this function's Python
 * interface.
 *
 * @sa scribus_getproperty(), QObject::property(), QMetaObject,
 *     scribus_propertyctype()
 */
PyDoc_STRVAR(scribus_setproperty__doc__,
QT_TR_NOOP("setProperty(object, property, value)\n\
\n\
Set `property' of `object' to `value'. If `value' cannot be converted to a type\n\
compatible with the type of `property', an exception is raised. An exception may\n\
also be raised if the underlying setter fails.\n\
\n\
See getProperty() for more information.\n\
"));
PyObject* scribus_setproperty(PyObject* /*self*/, PyObject* args, PyObject* kw);


/**
 * @brief Return a list of children of the passed object
 *
 * See the Python docstring for details on this function.
 *
 * @sa QObject::children(), scribus_getchild()
 */
/*Qt4
PyDoc_STRVAR(scribus_getchildren__doc__,
QT_TR_NOOP("getChildren(object, ofclass=None, ofname=None, regexpmatch=False, recursive=True)\n\
\n\
Return a list of children of `object', possibly restricted to children\n\
of class named `ofclass' or children named `ofname'. If `recursive' is true,\n\
search recursively through children, grandchildren, etc.\n\
\n\
See QObject::children() in the Qt docs for more information.\n\
"));
PyObject* scribus_getchildren(PyObject* self, PyObject* args, PyObject* kw);
*/

/**
 * @brief Return a wrapped PyObject* for the named child, if found.
 *
 * See the Python docstring for details on this function.
 *
 * @sa QObject::child(), scribus_children()
 */
/*Qt4
PyDoc_STRVAR(scribus_getchild__doc__,
QT_TR_NOOP("getChild(object, childname, ofclass=None, recursive=True)\n\
\n\
Return the first child of `object' named `childname', possibly restricting\n\
the search to children of type name `ofclass'. If `recursive' is true,\n\
search recursively through children, grandchildren, etc.\n\
"));
PyObject* scribus_getchild(PyObject* self, PyObject* args, PyObject* kw);
*/
#endif
