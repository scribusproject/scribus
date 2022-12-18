/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "objimageexport.h"

#include <QImageWriter>
#include <structmember.h>
#include <QFileInfo>
#include <vector>

#include "cmdutil.h"
#include "scpage.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"

typedef struct
{
	PyObject_HEAD
	PyObject *name; // string - filename
	PyObject *type; // string - image type (PNG, JPEG etc.)
	PyObject *allTypes; // list - available types
	int dpi; // DPI of the bitmap
	int scale; // how is bitmap scaled 100 = 100%
	int quality; // quality/compression <1; 100>
	int transparentBkgnd; // background transparency
} ImageExport;

static void ImageExport_dealloc(ImageExport* self)
{
	Py_XDECREF(self->name);
	Py_XDECREF(self->type);
	Py_XDECREF(self->allTypes);
	Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject * ImageExport_new(PyTypeObject *type, PyObject * /*args*/, PyObject * /*kwds*/)
{
	if (!checkHaveDocument())
		return nullptr;

	ImageExport *self;
	self = (ImageExport *)type->tp_alloc(type, 0);
	if (self != nullptr) {
		self->name = PyUnicode_FromString("ImageExport.png");
		self->type = PyUnicode_FromString("PNG");
		self->allTypes = PyList_New(0);
		self->dpi = 72;
		self->scale = 100;
		self->quality = 100;
		self->transparentBkgnd = 0;
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
	{const_cast<char*>("transparentBkgnd"), T_INT, offsetof(ImageExport, transparentBkgnd), 0, imgexp_transparentBkgnd__doc__},
	{nullptr, 0, 0, 0, nullptr} // sentinel
};

static PyObject *ImageExport_getName(ImageExport *self, void * /*closure*/)
{
	Py_INCREF(self->name);
	return self->name;
}

static int ImageExport_setName(ImageExport *self, PyObject *value, void * /*closure*/)
{
	if (!PyUnicode_Check(value)) {
		PyErr_SetString(PyExc_TypeError, QObject::tr("The filename must be a string.", "python error").toLocal8Bit().constData());
		return -1;
	}
	if (PyUnicode_GET_LENGTH(value) < 1)
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
	if (value == nullptr) {
		PyErr_SetString(PyExc_TypeError, QObject::tr("Cannot delete image type settings.", "python error").toLocal8Bit().constData());
		return -1;
	}
	if (!PyUnicode_Check(value)) {
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
		PyList_SetItem(l, pos, PyUnicode_FromString(QString((*it)).toLatin1().constData()));
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
	{const_cast<char*>("name"), (getter)ImageExport_getName, (setter)ImageExport_setName, imgexp_filename__doc__, nullptr},
	{const_cast<char*>("type"), (getter)ImageExport_getType, (setter)ImageExport_setType, imgexp_type__doc__, nullptr},
	{const_cast<char*>("allTypes"), (getter)ImageExport_getAllTypes, (setter)ImageExport_setAllTypes, imgexp_alltypes__doc__, nullptr},
	{nullptr, nullptr, nullptr, nullptr, nullptr}  // sentinel
};

static PyObject *ImageExport_save(ImageExport *self)
{
	if (!checkHaveDocument())
		return nullptr;
	ScribusDoc*  doc = ScCore->primaryMainWindow()->doc;
	ScribusView*view = ScCore->primaryMainWindow()->view;

	/* a little magic here - I need to compute the "maxGr" value...
	* We need to know the right size of the page for landscape,
	* portrait and user defined sizes.
	*/
	double pixmapSize = (doc->pageHeight() > doc->pageWidth()) ? doc->pageHeight() : doc->pageWidth();
	PageToPixmapFlags flags = Pixmap_DrawBackground;
	if (self->transparentBkgnd)
		flags &= ~Pixmap_DrawBackground;
	QImage im = view->PageToPixmap(doc->currentPage()->pageNr(), qRound(pixmapSize * self->scale * (self->dpi / 72.0) / 100.0), flags);
	int dpi = qRound(100.0 / 2.54 * self->dpi);
	im.setDotsPerMeterY(dpi);
	im.setDotsPerMeterX(dpi);

	QString imgFileName = PyUnicode_asQString(self->name);
	if (!im.save(imgFileName, PyUnicode_AsUTF8(self->type)))
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to export image", "python error").toLocal8Bit().constData());
		return nullptr;
	}
// 	Py_INCREF(Py_True); // return True not None for backward compat
 //	return Py_True;
//	Py_RETURN_TRUE;
	return PyBool_FromLong(static_cast<long>(true));
}

static PyObject *ImageExport_saveAs(ImageExport *self, PyObject *args)
{
	char* value;
	if (!checkHaveDocument())
		return nullptr;
	if (!PyArg_ParseTuple(args, const_cast<char*>("es"), "utf-8", &value))
		return nullptr;

	ScribusDoc*  doc = ScCore->primaryMainWindow()->doc;
	ScribusView*view = ScCore->primaryMainWindow()->view;

	/* a little magic here - I need to compute the "maxGr" value...
	* We need to know the right size of the page for landscape,
	* portrait and user defined sizes.
	*/
	double pixmapSize = (doc->pageHeight() > doc->pageWidth()) ? doc->pageHeight() : doc->pageWidth();
	PageToPixmapFlags flags = Pixmap_DrawBackground;
	if (self->transparentBkgnd)
		flags &= ~Pixmap_DrawBackground;
	QImage im = view->PageToPixmap(doc->currentPage()->pageNr(), qRound(pixmapSize * self->scale * (self->dpi / 72.0) / 100.0), flags);
	int dpi = qRound(100.0 / 2.54 * self->dpi);
	im.setDotsPerMeterY(dpi);
	im.setDotsPerMeterX(dpi);

	QString outputFileName = QString::fromUtf8(value);
	if (!im.save(outputFileName, PyUnicode_AsUTF8(self->type)))
	{
		PyErr_SetString(ScribusException, QObject::tr("Failed to export image", "python error").toLocal8Bit().constData());
		return nullptr;
	}
// 	Py_INCREF(Py_True); // return True not None for backward compat
 //	return Py_True;
//	Py_RETURN_TRUE;
	return PyBool_FromLong(static_cast<long>(true));
}

static PyMethodDef ImageExport_methods[] = {
	{const_cast<char*>("save"), (PyCFunction)ImageExport_save, METH_NOARGS, imgexp_save__doc__},
	{const_cast<char*>("saveAs"), (PyCFunction)ImageExport_saveAs, METH_VARARGS, imgexp_saveas__doc__},
	{nullptr, (PyCFunction)(nullptr), 0, nullptr} // sentinel
};

PyTypeObject ImageExport_Type = {
	PyVarObject_HEAD_INIT(nullptr, 0)   // PyObject_VAR_HEAD
	const_cast<char*>("scribus.ImageExport"), // char *tp_name; /* For printing, in format "<module>.<name>" */
	sizeof(ImageExport),   // int tp_basicsize, /* For allocation */
	0,  // int tp_itemsize; /* For allocation */

	/* Methods to implement standard operations */

	(destructor) ImageExport_dealloc, //	 destructor tp_dealloc;
#if PY_VERSION_HEX >= 0x03080000
	0,       //     Py_ssize_t tp_vectorcall_offset
#else
	nullptr, //     printfunc tp_print;
#endif
	nullptr, //	 getattrfunc tp_getattr;
	nullptr, //	 setattrfunc tp_setattr;
	nullptr, //	 cmpfunc tp_as_async;
	nullptr, //	 reprfunc tp_repr;

	/* Method suites for standard classes */

	nullptr, //	 PyNumberMethods *tp_as_number;
	nullptr, //	 PySequenceMethods *tp_as_sequence;
	nullptr, //	 PyMappingMethods *tp_as_mapping;

	/* More standard operations (here for binary compatibility) */

	nullptr, //	 hashfunc tp_hash;
	nullptr, //	 ternaryfunc tp_call;
	nullptr, //	 reprfunc tp_str;
	nullptr, //	 getattrofunc tp_getattro;
	nullptr, //	 setattrofunc tp_setattro;

	/* Functions to access object as input/output buffer */
	nullptr, //	 PyBufferProcs *tp_as_buffer;

	/* Flags to define presence of optional/expanded features */
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,	// long tp_flags;

	imgexp__doc__, // char *tp_doc; /* Documentation string */

	/* Assigned meaning in release 2.0 */
	/* call function for all accessible objects */
	nullptr, //	 traverseproc tp_traverse;

	/* delete references to contained objects */
	nullptr, //	 inquiry tp_clear;

	/* Assigned meaning in release 2.1 */
	/* rich comparisons */
	nullptr, //	 richcmpfunc tp_richcompare;

	/* weak reference enabler */
	0, //	 long tp_weaklistoffset;

	/* Added in release 2.2 */
	/* Iterators */
	nullptr, //	 getiterfunc tp_iter;
	nullptr, //	 iternextfunc tp_iternext;

	/* Attribute descriptor and subclassing stuff */
	ImageExport_methods, //	 struct PyMethodDef *tp_methods;
	ImageExport_members, //	 struct PyMemberDef *tp_members;
	ImageExport_getseters, //	 struct PyGetSetDef *tp_getset;
	nullptr, //	 struct _typeobject *tp_base;
	nullptr, //	 PyObject *tp_dict;
	nullptr, //	 descrgetfunc tp_descr_get;
	nullptr, //	 descrsetfunc tp_descr_set;
	0, //	 long tp_dictoffset;
	(initproc)ImageExport_init, //	 initproc tp_init;
	nullptr, //	 allocfunc tp_alloc;
	ImageExport_new, //	 newfunc tp_new;
	nullptr, //	 freefunc tp_free; /* Low-level free-memory routine */
	nullptr, //	 inquiry tp_is_gc; /* For PyObject_IS_GC */
	nullptr, //	 PyObject *tp_bases;
	nullptr, //	 PyObject *tp_mro; /* method resolution order */
	nullptr, //	 PyObject *tp_cache;
	nullptr, //	 PyObject *tp_subclasses;
	nullptr, //	 PyObject *tp_weaklist;
	nullptr, //	 destructor tp_del;
	0, //	 unsigned int tp_version_tag;
	nullptr, //	 destructor tp_finalize;
#if PY_VERSION_HEX >= 0x03080000
	nullptr, // tp_vectorcall
#endif
#if PY_VERSION_HEX >= 0x03080000 && PY_VERSION_HEX < 0x03090000
	nullptr, // deprecated tp_print
#endif
#if PY_VERSION_HEX >= 0x03120000
	0, // char tp_watched
#endif

#if defined(COUNT_ALLOCS) && PY_VERSION_HEX < 0x03090000
	/* these must be last and never explicitly initialized */
	//    int tp_allocs;
	//    int tp_frees;
	//    int tp_maxalloc;
	//    struct _typeobject *tp_prev;
	//    struct _typeobject *tp_next;
#endif
};
