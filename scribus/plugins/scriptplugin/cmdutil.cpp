/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "cmdutil.h"
#include "prefsmanager.h"
#include "resourcecollection.h"
#include "scpage.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "selection.h"
#include "tableborder.h"
#include "units.h"

#include <QMap>

/// Convert a value in points to a value in the current document units
double PointToValue(double Val)
{
	return pts2value(Val, ScCore->primaryMainWindow()->doc->unitIndex());
}

/// Convert a value in the current document units to a value in points
double ValueToPoint(double Val)
{
	return value2pts(Val, ScCore->primaryMainWindow()->doc->unitIndex());
}

/// Convert an X coordinate part in page units to a document coordinate
/// in system units.
double pageUnitXToDocX(double pageUnitX)
{
	return ValueToPoint(pageUnitX) + ScCore->primaryMainWindow()->doc->currentPage()->xOffset();
}

// Convert doc units to page units
double docUnitXToPageX(double pageUnitX)
{
	return PointToValue(pageUnitX - ScCore->primaryMainWindow()->doc->currentPage()->xOffset());
}

/// Convert a Y coordinate part in page units to a document coordinate
/// in system units. The document coordinates have their origin somewhere
/// up and left of the first page, where page coordinates have their
/// origin on the top left of the current page.
double pageUnitYToDocY(double pageUnitY)
{
	return ValueToPoint(pageUnitY) + ScCore->primaryMainWindow()->doc->currentPage()->yOffset();
}

double docUnitYToPageY(double pageUnitY)
{
	return PointToValue(pageUnitY - ScCore->primaryMainWindow()->doc->currentPage()->yOffset());
}

PageItem *GetItem(const QString& name)
{
	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	if (!name.isEmpty())
	{
		for (int i = 0; i < currentDoc->Items->count(); ++i)
		{
			if (currentDoc->Items->at(i)->itemName() == name)
				return currentDoc->Items->at(i);
		}
	}
	else
	{
		if (currentDoc->m_Selection->count() != 0)
			return currentDoc->m_Selection->itemAt(0);
	}
	return nullptr;
}

void ReplaceColor(const QString& col, const QString& rep)
{
	QMap<QString, QString> colorMap;
	colorMap.insert(col, rep);

	ResourceCollection colorRsc;
	colorRsc.mapColor(col, rep);

	if (!ScCore->primaryMainWindow()->HaveDoc)
		return;
	ScribusDoc* doc = ScCore->primaryMainWindow()->doc;

	// Update tools colors
	PrefsManager::replaceToolColors(doc->itemToolPrefs(), colorRsc.colors());
	// Update objects and styles colors
	doc->replaceNamedResources(colorRsc);
	// Temporary code until LineStyle is effectively used
	doc->replaceLineStyleColors(colorMap);
}

/* 04/07/10 returns selection if is not name specified  pv  */
PageItem* GetUniqueItem(const QString& name)
{
	if (name.length() == 0)
	{
		if (ScCore->primaryMainWindow()->doc->m_Selection->count() != 0)
			return ScCore->primaryMainWindow()->doc->m_Selection->itemAt(0);
		PyErr_SetString(NoValidObjectError, QString("Cannot use empty string for object name when there is no selection").toLocal8Bit().constData());
		return nullptr;
	}
	return getPageItemByName(name);
}

PageItem* getPageItemByName(const QString& name)
{
	if (name.length() == 0)
	{
		PyErr_SetString(PyExc_ValueError, QString("Cannot accept empty name for pageitem").toLocal8Bit().constData());
		return nullptr;
	}

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	for (int i = 0; i < currentDoc->Items->count(); ++i)
	{
		if (name == currentDoc->Items->at(i)->itemName())
			return currentDoc->Items->at(i);
	}

	PyErr_SetString(NoValidObjectError, QString("Object not found").toLocal8Bit().constData());
	return nullptr;
}


/*!
 * Checks to see if a pageItem named 'name' exists and return true
 * if it does exist. Returns false if there is no such object, or
 * if the empty string ("") is passed.
 */
bool ItemExists(const QString& name)
{
	if (name.length() == 0)
		return false;

	ScribusDoc* currentDoc = ScCore->primaryMainWindow()->doc;
	for (int i = 0; i < currentDoc->Items->count(); ++i)
	{
		if (name == currentDoc->Items->at(i)->itemName())
			return true;
	}
	return false;
}

/*!
 * Checks to see if there is a document open.
 * If there is an open document, returns true.
 * If there is no open document, sets a Python
 * exception and returns false.
 * 2004-10-27 Craig Ringer
 */
bool checkHaveDocument()
{
	if (ScCore->primaryMainWindow()->HaveDoc)
		return true;
	// Caller is required to check for false return from this function
	// and return nullptr.
	PyErr_SetString(NoDocOpenError, QString("Command does not make sense without an open document").toLocal8Bit().constData());
	return false;
}

bool checkValidPageNumber(int page)
{
	const int numPages = ScCore->primaryMainWindow()->doc->Pages->count();
	if (page < 0 || page >= numPages)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("%1 is not a valid page number.", "python error").arg(page).toLocal8Bit().constData());
		return false;
	}
	return true;
}

QStringList getSelectedItemsByName()
{
	/*
	QStringList names;
	QPtrListIterator<PageItem> it(ScCore->primaryMainWindow()->view->SelItem);
	for ( ; it.current() != 0 ; ++it)
		names.append(it.current()->itemName());
	return names;
	*/
	return ScCore->primaryMainWindow()->doc->m_Selection->getSelectedItemsByName();
}

bool setSelectedItemsByName(const QStringList& itemNames)
{
	ScribusDoc* currentDoc =  ScCore->primaryMainWindow()->doc;
	ScribusView* currentView = ScCore->primaryMainWindow()->view;

	currentView->deselectItems();

	// For each named item
	for (auto it = itemNames.begin() ; it != itemNames.end() ; it++)
	{
		// Search for the named item
		PageItem* item = nullptr;
		for (int j = 0; j < currentDoc->Items->count(); j++)
		{
			if (*it == currentDoc->Items->at(j)->itemName())
				item = currentDoc->Items->at(j);
		}
		if (!item)
			return false;
		// And select it
		currentView->selectItem(item);
	}
	return true;
}

TableBorder parseBorder(PyObject* borderLines, bool* ok)
{
	TableBorder border;

	if (!PyList_Check(borderLines))
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Expected a list of border lines", "python error").toLocal8Bit().constData());
		*ok = false;
		return border;
	}

	// Get the sequence of border lines.
	PyObject* borderLinesList = PySequence_List(borderLines);
	if (borderLinesList == nullptr)
	{
		PyErr_SetString(PyExc_ValueError, QObject::tr("Expected a list of border lines", "python error").toLocal8Bit().constData());
		*ok = false;
		return border;
	}

	// Parse each tuple describing a border line and append it to the border.
	int nBorderLines = PyList_Size(borderLinesList);
	for (int i = 0; i < nBorderLines; ++i)
	{
		double width = 0.0;
		double shade = 100.0;
		int style;
		char* color;
		PyObject* props = PyList_GET_ITEM(borderLinesList, i);
		if (!PyArg_ParseTuple(props, "dies|d", &width, &style, "utf-8", &color, &shade))
		{
			PyErr_SetString(PyExc_ValueError, QObject::tr("Border lines are specified as (width,style,color,shade) tuples", "python error").toLocal8Bit().constData());
			*ok = false;
			return border;
		}
		if (width <= 0.0)
		{
			PyErr_SetString(PyExc_ValueError, QObject::tr("Border line width must be > 0.0", "python error").toLocal8Bit().constData());
			*ok = false;
			return border;
		}
		border.addBorderLine(TableBorderLine(width, static_cast<Qt::PenStyle>(style), QString::fromUtf8(color), shade));
	}
	Py_DECREF(borderLinesList);

	*ok = true;
	return border;
}

QString PyUnicode_asQString(PyObject* arg)
{
	const char* utf8Str = PyUnicode_AsUTF8(arg);
	if (!utf8Str)
		return QString();
	return QString::fromUtf8(utf8Str);
}

