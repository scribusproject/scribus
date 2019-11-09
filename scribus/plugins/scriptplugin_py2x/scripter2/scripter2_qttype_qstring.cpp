/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdvar.h"

#include <QString>
#include <wchar.h>



// Shamelessly cribbed from code in python-qt4
// Originally by Eric Jardim <ericjardim@gmail.com>
// Rewritten by Craig Ringer <craig@postnewspapers.com.au>
// Ported to Scribus and Qt3 by Craig Ringer <craig@postnewspapers.com.au>

using namespace boost::python;
/**
 * @brief Boost::Python type converter for QString to Python `unicode' object
 */
struct QString_to_python_unicode
{
    static PyObject*
    convert(QString const& s)
    {
        //FIXME: Done with Python/C API, needs Boost::Python-ifying
        //FIXME: UCS-4 case can probably be done a lot more effciently.
#if defined(Py_UNICODE_WIDE)
        //qDebug("Py_UNICODE_WIDE");
        int unichars = s.length();
        Py_UNICODE* buf = new Py_UNICODE[unichars];
        for (int i = 0; i < unichars; i++)
            buf[i] = s.at(i).unicode();
        PyObject* tempObj = PyUnicode_FromUnicode(buf, unichars);
        delete[] buf;
        return tempObj;
#else
        return PyUnicode_FromUnicode(s.utf16(), s.length());
#endif
    }
};


/**
 * @brief Boost::Python type converter for Python `unicode' or `str' to QString
 */
struct QString_from_python_str_or_unicode
{
    QString_from_python_str_or_unicode()
    {
        boost::python::converter::registry::push_back(  &convertible,
                                                        &construct,
                                                        boost::python::type_id<QString>() );
    }

    static void*
    convertible(PyObject* obj_ptr)
    {
        if (! (PyUnicode_Check(obj_ptr) || PyString_Check(obj_ptr)) )
                return 0;
        return obj_ptr;
    }

    static void
    construct(  PyObject* obj_ptr,
                boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        // First, convert the input to Python `unicode'.
        PyObject* temp_obj_ptr;
        if (PyString_Check(obj_ptr))
        {
            // Coerce the `str' to `unicode' using sysdefaultencoding. UnicodeDecodeError
            // is thrown if the string doesn't make sense in that encoding.
            temp_obj_ptr = PyUnicode_FromObject(obj_ptr); // new reference
            if (temp_obj_ptr == 0)
            {
                boost::python::throw_error_already_set();
            }
        }
        else
        {
            temp_obj_ptr = obj_ptr;
            Py_INCREF(temp_obj_ptr); // to balance DECREF at end
        }

        // FIXME: This implementation is probably rather inefficient
        Py_UNICODE* value = PyUnicode_AsUnicode(temp_obj_ptr);
        if (value == 0)
        {
            boost::python::throw_error_already_set();
        }
        int unichars = PyUnicode_GET_SIZE(temp_obj_ptr);
#if defined(Py_UNICODE_WIDE)

        // Python is using a 4-byte unsigned buffer of UCS-4
        // FIXME: Qt doesn't give us any direct way to load UCS-4, so we're doing
        //        it a rather clunky way that can probably be improved.
        // FIXME: Qt can't represent UCS-4 characters; we need to check for this
        //        and throw an exception.
        QString tempString("");
        int i;
        for (i = 0; i < unichars; i++)
            tempString.append(QChar(value[i]));
#else
        // Python is using a 2-byte unsigned buffer of UCS-2 with
        // limited support for UTF-16
        QString tempString(QString::fromUtf16(value, unichars));
#endif
        Py_DECREF(temp_obj_ptr);
        void* storage = ((boost::python::converter::rvalue_from_python_storage<QString>*) data)->storage.bytes;
        new (storage) QString(tempString);
        data->convertible = storage;
    }
};

void export_QString()
{
	boost::python::to_python_converter<QString, QString_to_python_unicode>();
	QString_from_python_str_or_unicode();
}


