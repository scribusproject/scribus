/* copyright 2003 Jim Bublitz <jbublitz@nwinternet.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __pythonize_h__
#define __pythonize_h__

// Pythonize is a general purpose library that wraps the Python
// interpreter with an interface to a set of common operations
// used when embedding the interpreter.

#include <Python.h>

struct ObjectRef
{
    ObjectRef (ObjectRef *oi, PyObject *o);
    ~ObjectRef () {
        Py_XDECREF (object);
    }

    PyObject        *object;        // pointer to an object we created
    ObjectRef       *prevObject;    // pointer to next object on the stack
};

class Pythonize
{
public:
    Pythonize ();
    ~Pythonize ();

    // adds a path to sys.path
    bool appendToSysPath (const char* newPath);

    // imports a module into the interpreter
    // or gets a PyObject for an already loaded module
    PyObject *importModule (char *moduleName);

    // returns an object from a loaded module
    // you must decref the object returned when done with it (new reference returned)
    PyObject *getNewObjectRef (PyObject *module, char *object) {
        return PyObject_GetAttrString (module, object);
    }

    int getPythonInit () {
        return pythonInit;
    }

    // decrements the ref count of an object
    void decref (PyObject *object) {
        Py_XDECREF (object);
    }

    // runs a script on the current sys.path
    bool runScript (char *scriptPath);

    // executes a string of Python in the interpreter
    bool runString (char *str);

    // runs a callable Python object
    PyObject *runFunction(PyObject *object, PyObject *args);
    void *runFunctionVoid(PyObject *object, PyObject *args);

private:
    int pythonInit;         // status of Py_Initialize
    ObjectRef *objects;     // a stack of PyObjects (used in destructor)
};

extern "C" {
    Pythonize *initialize();
    void finalize();

    // adds a path to sys.path
    bool appendToSysPath (const char* newPath);

    // imports a module into the interpreter
    // or gets a PyObject for an already loaded module
    PyObject *importModule (char *moduleName);

    // returns an object from a loaded module
    // you must decref the object returned when done with it (new reference returned)
    PyObject *getNewObjectRef (PyObject *module, char *object);

    bool getPythonInit();

    // decrements the ref count of an object
    void decref (PyObject *object);

    // runs a script on the current sys.path
    bool runScript (char *scriptPath);

    // executes a string of Python in the interpreter
    bool runString (char *str);

    // runs a callable Python object
    PyObject *runFunction (PyObject *object, PyObject *args);
}

#endif
