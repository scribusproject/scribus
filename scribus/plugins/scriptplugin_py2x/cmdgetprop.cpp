/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <algorithm>

#include "cmdgetprop.h"
#include "cmdutil.h"
#include "scribuscore.h"
#include "scribusdoc.h"

/* getObjectType(name) */
PyObject *scribus_getobjecttype(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	PageItem *item = nullptr;
	QString result = "";

	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;

	if (!checkHaveDocument())
		return nullptr;

	item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;

	if (item->itemType() == PageItem::TextFrame) {
		result = "TextFrame";
	} else if (item->itemType() == PageItem::PathText) {
		result = "PathText";
	} else if (item->itemType() == PageItem::ImageFrame) {
		result = "ImageFrame";
	} else if (item->itemType() == PageItem::Line) {
		result = "Line";
	} else if (item->itemType() == PageItem::Polygon) {
		result = "Polygon";
	} else if (item->itemType() == PageItem::PolyLine) {
		result = "Polyline";
	} else if (item->itemType() == PageItem::LatexFrame) {
		result = "LatexFrame";
	} else if (item->itemType() == PageItem::OSGFrame) {
		result = "OSGFrame";
	} else if (item->itemType() == PageItem::Symbol) {
		result = "Symbol";
	} else if (item->itemType() == PageItem::Group) {
		result = "Group";
	} else if (item->itemType() == PageItem::RegularPolygon) {
		result = "RegularPolygon";
	} else if (item->itemType() == PageItem::Arc) {
		result = "Arc";
	} else if (item->itemType() == PageItem::Spiral) {
		result = "Spiral";
	} else if (item->itemType() == PageItem::Table) {
		result = "Table";
	} else if (item->itemType() == PageItem::NoteFrame) {
		result = "NoteFrame";
	} else if (item->itemType() == PageItem::Multiple) {
		result = "Multiple";
	}

	return PyString_FromString(result.toUtf8());
}

PyObject *scribus_getfillcolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyString_FromString(item->fillColor().toUtf8());
}

PyObject *scribus_getfilltransparency(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyFloat_FromDouble(static_cast<double>(1.0 - item->fillTransparency()));
}

PyObject *scribus_getfillblendmode(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyInt_FromLong(static_cast<long>(item->fillBlendmode()));
}

PyObject *scribus_getcustomlinestyle(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyString_FromString(item->customLineStyle().toUtf8());
}

PyObject *scribus_getlinecolor(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyString_FromString(item->lineColor().toUtf8());
}

PyObject *scribus_getlinetransparency(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyFloat_FromDouble(static_cast<double>(1.0 - item->lineTransparency()));
}

PyObject *scribus_getlineblendmode(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyInt_FromLong(static_cast<long>(item->lineBlendmode()));
}

PyObject *scribus_getlinewidth(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyFloat_FromDouble(static_cast<double>(item->lineWidth()));
}

PyObject *scribus_getlineshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyInt_FromLong(static_cast<long>(item->lineShade()));
}

PyObject *scribus_getlinejoin(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyInt_FromLong(static_cast<long>(item->PLineJoin));
}

PyObject *scribus_getlinecap(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyInt_FromLong(static_cast<long>(item->PLineEnd));
}

PyObject *scribus_getlinestyle(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyInt_FromLong(static_cast<long>(item->PLineArt));
}

PyObject *scribus_getfillshade(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyInt_FromLong(static_cast<long>(item->fillShade()));
}

PyObject *scribus_getcornerradius(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyInt_FromLong(static_cast<long>(item->cornerRadius()));
}

PyObject *scribus_getimageoffset(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return Py_BuildValue("(ff)", item->imageXOffset() * item->imageXScale(), item->imageYOffset() * item->imageYScale());
}

PyObject *scribus_getimagescale(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return Py_BuildValue("(ff)", item->imageXScale() / 72.0 * item->pixm.imgInfo.xres, item->imageYScale() / 72.0 * item->pixm.imgInfo.yres);
}

PyObject *scribus_getimagefile(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyString_FromString(item->Pfile.toUtf8());
}

PyObject *scribus_getposition(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return Py_BuildValue("(ff)", docUnitXToPageX(item->xPos()),
								 docUnitYToPageY(item->yPos()));
}

PyObject *scribus_getsize(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return Py_BuildValue("(ff)", PointToValue(item->width()), PointToValue(item->height()));
}

PyObject *scribus_getrotation(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	return PyFloat_FromDouble(static_cast<double>(item->rotation() * -1));
}

PyObject *scribus_getallobjects(PyObject* /* self */, PyObject* args, PyObject *keywds)
{
	int itemType = -1;
	int layerId = -1;
	uint counter = 0;

	if (!checkHaveDocument())
		return nullptr;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	int pageNr = currentDoc->currentPageNumber();
	char *kwlist[] = { const_cast<char*>("type"), const_cast<char*>("page"), const_cast<char*>("layer"), nullptr};
	char* szLayerName = const_cast<char*>("");

	if (!PyArg_ParseTupleAndKeywords(args, keywds, "|iies", kwlist, &itemType, &pageNr, "utf-8", &szLayerName))
		return nullptr;

	int numPages = currentDoc->Pages->count();
	if (pageNr < 0 || pageNr >= numPages)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("page number is invalid.", "python error").toLocal8Bit().constData());
		return nullptr;
	}

	QString layerName = QString::fromUtf8(szLayerName);
	if (!layerName.isEmpty())
	{
		const ScLayer *layer = currentDoc->Layers.layerByName(layerName);
		if (!layer)
		{
			PyErr_SetString(PyExc_ValueError, QObject::tr("layer name is invalid.", "python error").toLocal8Bit().constData());
			return nullptr;
		}
		layerId = layer->ID;
	}

	auto isReturnedItem = [pageNr, layerId, itemType](PageItem* item)
	{
		if  (pageNr != item->OwnPage)
			return false;
		if ((itemType != -1) && (item->itemType() != itemType))
			return false;
		if ((layerId != -1) && (item->m_layerID) != layerId)
			return false;
		return true;
	};

	int returnedItemCount = std::count_if(currentDoc->DocItems.begin(), currentDoc->DocItems.end(), isReturnedItem);

	PyObject* pyItemList = PyList_New(returnedItemCount);
	for (int i = 0; i < currentDoc->Items->count(); ++i)
	{
		PageItem* item = currentDoc->Items->at(i);
		if (!isReturnedItem(item))
			continue;
		PyList_SetItem(pyItemList, counter, PyString_FromString(item->itemName().toUtf8()));
		counter++;
	}
	return pyItemList;
}

PyObject *scribus_getobjectattributes(PyObject* /* self */, PyObject* args)
{
	if (!checkHaveDocument())
		return nullptr;
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;

	ObjAttrVector *attributes=item->getObjectAttributes();
	PyObject *lst;
	lst = PyList_New(attributes->count());
	if (!lst)
		return nullptr;
	int n = 0;
	for (ObjAttrVector::Iterator objAttrIt = attributes->begin() ; objAttrIt != attributes->end(); ++objAttrIt)
	{
		PyObject *tmp;
		tmp = Py_BuildValue("{ssssssssssssss}",
				    "Name", objAttrIt->name.toUtf8().data(),
				    "Type", objAttrIt->type.toUtf8().data(),
				    "Value", objAttrIt->value.toUtf8().data(),
				    "Parameter", objAttrIt->parameter.toUtf8().data(),
				    "Relationship", objAttrIt->relationship.toUtf8().data(),
				    "RelationshipTo", objAttrIt->relationshipto.toUtf8().data(),
				    "AutoAddTo", objAttrIt->autoaddto.toUtf8().data());
		if (tmp)
			PyList_SetItem(lst, n, tmp);
		else {
			// Error string is already set by Py_BuildValue()
			Py_DECREF(lst);
			return nullptr;
		}
		n++;
	}
	return lst;
}

PyObject *scribus_getimagecolorspace(PyObject* /* self */, PyObject* args)
{
	char *Name = const_cast<char*>("");
	if (!PyArg_ParseTuple(args, "|es", "utf-8", &Name))
		return nullptr;
	if (!checkHaveDocument())
		return nullptr;
	PageItem *item = GetUniqueItem(QString::fromUtf8(Name));
	if (item == nullptr)
		return nullptr;
	if (item->itemType() != PageItem::ImageFrame)
	{
		PyErr_SetString(WrongFrameTypeError,
			QObject::tr("Page item must be an ImageFrame", "python error").toLocal8Bit().constData());
		return nullptr;
	}

	const ScImage& pixm = item->pixm;
	if (pixm.width() == 0 || pixm.height() == 0)
		return PyInt_FromLong(static_cast<long>(-1));

	const ImageInfoRecord& iir = pixm.imgInfo;
	int cspace = iir.colorspace;
	/*
	RGB  = 0,
	CMYK = 1,
	Gray = 2,
	Duotone = 3,
	Monochrome = 4
	*/
	return PyInt_FromLong(static_cast<long>(cspace));
}


/*! HACK: this removes "warning: 'blah' defined but not used" compiler warnings
with header files structure untouched (docstrings are kept near declarations)
PV */
void cmdgetpropdocwarnings()
{
	QStringList s;
	s << scribus_getallobjects__doc__
	  << scribus_getcornerradius__doc__ 
	  << scribus_getcustomlinestyle__doc__
	  << scribus_getfillcolor__doc__
	  << scribus_getfillblendmode__doc__
	  << scribus_getfillshade__doc__ 
	  << scribus_getfilltransparency__doc__
	  << scribus_getimagecolorspace__doc__
	  << scribus_getimagefile__doc__
	  << scribus_getimageoffset__doc__
	  << scribus_getimagescale__doc__
	  << scribus_getlinecolor__doc__ 
	  << scribus_getlineblendmode__doc__ 
	  << scribus_getlinecap__doc__
	  << scribus_getlinejoin__doc__ 
	  << scribus_getlineshade__doc__
	  << scribus_getlinetransparency__doc__
	  << scribus_getlinewidth__doc__
	  << scribus_getlinestyle__doc__ 
	  << scribus_getobjectattributes__doc__ 
	  << scribus_getobjecttype__doc__
	  << scribus_getposition__doc__
	  << scribus_getrotation__doc__
	  << scribus_getsize__doc__;
}
