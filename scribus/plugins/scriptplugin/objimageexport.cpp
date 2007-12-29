/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "objimageexport.h"
#include "cmdutil.h"
#include "page.h"

#include <QImageWriter>
#include <structmember.h>
#include <QFileInfo>
#include <vector>
#include "scribuscore.h"

typedef struct
{
	PyObject_HEAD
	PyObject *name; // string - filename
	PyObject *type; // string - image type (PNG, JPEG etc.)
	PyObject *allTypes; // list - available types
	int dpi; // DPI of the bitmap
	int scale; // how is bitmap scaled 100 = 100%
	int quality; // quality/compression <1; 100>
} ImageExport;

static void ImageExport_dealloc(ImageExport* self)
{
	Py_XDECREF(self->name);
	Py_XDECREF(self->type);
	Py_XDECREF(self->allTypes);
	self->ob_type->tp_free((PyObject *)self);
}

static PyObject * ImageExport_new(PyTypeObject *type, PyObject * /*args*/, PyObject * /*kwds*/)
{
	if(!checkHaveDocument())
		return NULL;

	ImageExport *self;
	self = (ImageExport *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->name = PyString_FromString("ImageExport.png");
		self->type = PyString_FromString("PNG");
		self->allTypes = PyList_New(0);
		self->dpi = 72;
		self->scale = 100;
		self->quality = 100;
	}
	return (PyObject *) self;
}

static int ImageExport_init(ImageExport * /*self*/, PyObject * /*args*/, PyObject * /*kwds*/)
{
	return 0;
}

static PyMemberDef ImageExport_members[] = {
	{const_cast<char*>("dpi"), T_INT, offsetof(ImageExport, dpi), 0, imgexp_dpi__doc__},
	{const_cast<char*>("scale"), T_INT, offsetof(ImageExport, scale), 0, imgexp_scale__doc__},
	{const_cast<char*>("quality"), T_INT, offsetof(ImageExport, quality), 0, imgexp_quality__doc__},
	{NULL, 0, 0, 0, NULL} // sentinel
};

static PyObject *ImageExport_getName(ImageExport *self, void * /*closure*/)
{
	Py_INCREF(self->name);
	return self->name;
}

static int ImageExport_setName(ImageExport *self, PyObject *value, void * /*closure*/)
{
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, QObject::tr("The filename must be a string.", "python error").toLocal8Bit().constData());
		return -1;
	}
	if (PyString_Size(value) < 1)
	{
		PyErr_SetString(PyExc_TypeError, QObject::tr("The filename should not be empty string.", "python error").toLocal8Bit().constData());
		return -1;
	}
	Py_DECREF(self->name);
	Py_INCREF(value);
	self->name = value;
	return 0;
}

static PyObject *ImageExport_getType(ImageExport *self, void * /*closure*/)
{
	Py_INCREF(self->type);
	return self->type;
}

static int ImageExport_setType(ImageExport *self, PyObject *value, void * /*closure*/)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, QObject::tr("Cannot delete image type settings.", "python error").toLocal8Bit().constData());
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, QObject::tr("The image type must be a string.", "python error").toLocal8Bit().constData());
		return -1;
	}
	Py_DECREF(self->type);
	Py_INCREF(value);
	self->type = value;
	return 0;
}

static PyObject *ImageExport_getAllTypes(ImageExport * /*self*/, void * /*closure*/)
{
	PyObject *l;
	int pos = 0;
	QList<QByteArray> list = QImageWriter::supportedImageFormats();
	l = PyList_New(list.count());
	for (QList<QByteArray>::Iterator it = list.begin(); it != list.end(); ++it)
	{
		PyList_SetItem(l, pos, PyString_FromString(QString((*it)).toLatin1().constData()));
		++pos;
	}
	return l;
}

static int ImageExport_setAllTypes(ImageExport * /*self*/, PyObject * /*value*/, void * /*closure*/)
{
	PyErr_SetString(PyExc_ValueError, QObject::tr("'allTypes' attribute is READ-ONLY", "python error").toLocal8Bit().constData());
	return -1;
}

static PyGetSetDef ImageExport_getseters [] = {
	{const_cast<char*>("name"), (getter)ImageExport_getName, (setter)ImageExport_setName, imgexp_filename__doc__, NULL},
	{const_cast<char*>("type"), (getter)ImageExport_getType, (setter)ImageExport_setType, imgexp_type__doc__, NULL},
	{const_cast<char*>("allTypes"), (getter)ImageExport_getAllTypes, (setter)ImageExport_setAllTypes, imgexp_alltypes__doc__, NULL},
	{NULL, NULL, NULL, NULL, NULL}  // sentinel
};

static PyObject *ImageExport_save(ImageExport *self)
{
	if(!checkHaveDocument())
		return NULL;

	/* a little magic here - I need to compute the "maxGr" value...
	* We need to know the right size of the page for landscape,
	* portrait and user defined sizes.
	*/
	double pixmapSize;
	(ScCore->primaryMainWindow()->doc->pageHeight > ScCore->primaryMainWindow()->doc->pageWidth)
			? pixmapSize = ScCore->primaryMainWindow()->doc->pageHeight
			: pixmapSize = ScCore->primaryMainWindow()->doc->pageWidth;
	QImage im = ScCore->primaryMainWindow()->view->PageToPixmap(ScCore->primaryMainWindow()->doc->currentPage()->pageNr(), qRound(pixmapSize * self->scale * (self->dpi / 72.0) / 100.0), false);
	int dpi = qRound(100.0 / 2.54 * self->dpi);
	im.setDotsPerMeterY(dpi);
	im.setDotsPerMeterX(dpi);
	if (!im.save(PyString_AsString(self->name), PyString_AsString(self->type)))
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to export image", "python error").toLocal8Bit().constData());
		return NULL;
	}
// 	Py_INCREF(Py_True); // return True not None for backward compat
 //	return Py_True;
//	Py_RETURN_TRUE;
	return PyBool_FromLong(static_cast<long>(true));
}

static PyObject *ImageExport_saveAs(ImageExport *self, PyObject *args)
{
	char* value;
	if(!checkHaveDocument())
		return NULL;
	if (!PyArg_ParseTuple(args, const_cast<char*>("es"), "utf-8", &value))
		return NULL;

	/* a little magic here - I need to compute the "maxGr" value...
	* We need to know the right size of the page for landscape,
	* portrait and user defined sizes.
	*/
	double pixmapSize;
	(ScCore->primaryMainWindow()->doc->pageHeight > ScCore->primaryMainWindow()->doc->pageWidth)
			? pixmapSize = ScCore->primaryMainWindow()->doc->pageHeight
			: pixmapSize = ScCore->primaryMainWindow()->doc->pageWidth;
	QImage im = ScCore->primaryMainWindow()->view->PageToPixmap(ScCore->primaryMainWindow()->doc->currentPage()->pageNr(), qRound(pixmapSize * self->scale * (self->dpi / 72.0) / 100.0), false);
	int dpi = qRound(100.0 / 2.54 * self->dpi);
	im.setDotsPerMeterY(dpi);
	im.setDotsPerMeterX(dpi);
	if (!im.save(value, PyString_AsString(self->type)))
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to export image", "python error").toLocal8Bit().constData());
		return NULL;
	}
// 	Py_INCREF(Py_True); // return True not None for backward compat
 //	return Py_True;
//	Py_RETURN_TRUE;
	return PyBool_FromLong(static_cast<long>(true));
}

static PyMethodDef ImageExport_methods[] = {
	{const_cast<char*>("save"), (PyCFunction)ImageExport_save, METH_NOARGS, imgexp_save__doc__},
	{const_cast<char*>("saveAs"), (PyCFunction)ImageExport_saveAs, METH_VARARGS, imgexp_saveas__doc__},
	{NULL, (PyCFunction)(0), 0, NULL} // sentinel
};

PyTypeObject ImageExport_Type = {
	PyObject_HEAD_INIT(NULL)   // PyObject_VAR_HEAD
	0,
	const_cast<char*>("ImageExport"), // char *tp_name; /* For printing, in format "<module>.<name>" */
	sizeof(ImageExport),   // int tp_basicsize, /* For allocation */
	0,  // int tp_itemsize; /* For allocation */
	(destructor) ImageExport_dealloc, //	 destructor tp_dealloc;
	0, //	 printfunc tp_print;
	0, //	 getattrfunc tp_getattr;
	0, //	 setattrfunc tp_setattr;
	0, //	 cmpfunc tp_compare;
	0, //	 reprfunc tp_repr;
	0, //	 PyNumberMethods *tp_as_number;
	0, //	 PySequenceMethods *tp_as_sequence;
	0, //	 PyMappingMethods *tp_as_mapping;
	0, //	 hashfunc tp_hash;
	0, //	 ternaryfunc tp_call;
	0, //	 reprfunc tp_str;
	0, //	 getattrofunc tp_getattro;
	0, //	 setattrofunc tp_setattro;
	0, //	 PyBufferProcs *tp_as_buffer;
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,	// long tp_flags;
	imgexp__doc__, // char *tp_doc; /* Documentation string */
	0, //	 traverseproc tp_traverse;
	0, //	 inquiry tp_clear;
	0, //	 richcmpfunc tp_richcompare;
	0, //	 long tp_weaklistoffset;
	0, //	 getiterfunc tp_iter;
	0, //	 iternextfunc tp_iternext;
	ImageExport_methods, //	 struct PyMethodDef *tp_methods;
	ImageExport_members, //	 struct PyMemberDef *tp_members;
	ImageExport_getseters, //	 struct PyGetSetDef *tp_getset;
	0, //	 struct _typeobject *tp_base;
	0, //	 PyObject *tp_dict;
	0, //	 descrgetfunc tp_descr_get;
	0, //	 descrsetfunc tp_descr_set;
	0, //	 long tp_dictoffset;
	(initproc)ImageExport_init, //	 initproc tp_init;
	0, //	 allocfunc tp_alloc;
	ImageExport_new, //	 newfunc tp_new;
	0, //	 freefunc tp_free; /* Low-level free-memory routine */
	0, //	 inquiry tp_is_gc; /* For PyObject_IS_GC */
	0, //	 PyObject *tp_bases;
	0, //	 PyObject *tp_mro; /* method resolution order */
	0, //	 PyObject *tp_cache;
	0, //	 PyObject *tp_subclasses;
	0, //	 PyObject *tp_weaklist;
	0, //	 destructor tp_del;

#ifdef COUNT_ALLOCS
	/* these must be last and never explicitly initialized */
	//	int tp_allocs;
	//	int tp_frees;
	//	int tp_maxalloc;
	//	struct _typeobject *tp_next;
#endif
};
