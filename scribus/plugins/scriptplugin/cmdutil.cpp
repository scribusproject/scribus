/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "cmdutil.h"
#include "units.h"
#include "page.h"
#include "selection.h"

ScribusMainWindow* Carrier;
ScribusDoc* doc;

/// Convert a value in points to a value in the current document units
double PointToValue(double Val)
{
	return pts2value(Val, ScMW->doc->unitIndex());
}

/// Convert a value in the current document units to a value in points
double ValueToPoint(double Val)
{
	return value2pts(Val, ScMW->doc->unitIndex());
}

/// Convert an X co-ordinate part in page units to a document co-ordinate
/// in system units.
double pageUnitXToDocX(double pageUnitX)
{
	return ValueToPoint(pageUnitX) + ScMW->doc->currentPage->xOffset();
}

// Convert doc units to page units
double docUnitXToPageX(double pageUnitX)
{
	return PointToValue(pageUnitX - ScMW->doc->currentPage->xOffset());
}

/// Convert a Y co-ordinate part in page units to a document co-ordinate
/// in system units. The document co-ordinates have their origin somewere
/// up and left of the first page, where page co-ordinates have their
/// origin on the top left of the current page.
double pageUnitYToDocY(double pageUnitY)
{
	return ValueToPoint(pageUnitY) + ScMW->doc->currentPage->yOffset();
}

double docUnitYToPageY(double pageUnitY)
{
	return PointToValue(pageUnitY - ScMW->doc->currentPage->yOffset());
}

int GetItem(QString Name)
{
	if (!Name.isEmpty())
	{
		for (uint a = 0; a < ScMW->doc->Items->count(); a++)
		{
			if (ScMW->doc->Items->at(a)->itemName() == Name)
				return static_cast<int>(a);
		}
	}
	else
	{
		if (ScMW->doc->m_Selection->count() != 0)
			return ScMW->doc->m_Selection->itemAt(0)->ItemNr;
	}
	return -1;
}

void ReplaceColor(QString col, QString rep)
{
	QColor tmpc;
	for (uint c = 0; c < ScMW->doc->Items->count(); c++)
	{
		PageItem *ite = ScMW->doc->Items->at(c);
		if (ite->itemType() == PageItem::TextFrame)
		{
			for (uint d = 0; d < ite->itemText.count(); d++)
			{
				if (col == ite->itemText.at(d)->ccolor)
					ite->itemText.at(d)->ccolor = rep;
				if (col == ite->itemText.at(d)->cstroke)
					ite->itemText.at(d)->cstroke = rep;
			}
		}
		if (col == ite->fillColor())
			ite->setFillColor(rep);
		if (col == ite->lineColor())
			ite->setLineColor(rep);
		QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			if (col == cstops.at(cst)->name)
			{
				ite->SetFarbe(&tmpc, rep, cstops.at(cst)->shade);
				cstops.at(cst)->color = tmpc;
				cstops.at(cst)->name = rep;
			}
		}
	}
	for (uint c = 0; c < ScMW->doc->MasterItems.count(); c++)
	{
		PageItem *ite = ScMW->doc->MasterItems.at(c);
		if (ite->itemType() == PageItem::TextFrame)
		{
			for (uint d = 0; d < ite->itemText.count(); d++)
			{
				if (col == ite->itemText.at(d)->ccolor)
					ite->itemText.at(d)->ccolor = rep;
				if (col == ite->itemText.at(d)->cstroke)
					ite->itemText.at(d)->cstroke = rep;
			}
		}
		if (col == ite->fillColor())
			ite->setFillColor(rep);
		if (col == ite->lineColor())
			ite->setLineColor(rep);
		QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
		for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
		{
			if (col == cstops.at(cst)->name)
			{
				ite->SetFarbe(&tmpc, rep, cstops.at(cst)->shade);
				cstops.at(cst)->color = tmpc;
				cstops.at(cst)->name = rep;
			}
		}
	}
}

/* 04/07/10 returns selection if is not name specified  pv  */
PageItem* GetUniqueItem(QString name)
{
	if (name.length()==0)
		if (ScMW->doc->m_Selection->count() != 0)
			return ScMW->doc->m_Selection->itemAt(0);
		else
		{
			PyErr_SetString(NoValidObjectError, QString("Cannot use empty string for object name when there is no selection"));
			return NULL;
		}
	else
		return getPageItemByName(name);
}

PageItem* getPageItemByName(QString name)
{
	if (name.length() == 0)
	{
		PyErr_SetString(PyExc_ValueError, QString("Cannot accept empty name for pageitem"));
		return NULL;
	}
	for (uint j = 0; j<ScMW->doc->Items->count(); j++)
	{
		if (name==ScMW->doc->Items->at(j)->itemName())
			return ScMW->doc->Items->at(j);
	} // for items
	PyErr_SetString(NoValidObjectError, QString("Object not found"));
	return NULL;
}


/*!
 * Checks to see if a pageItem named 'name' exists and return true
 * if it does exist. Returns false if there is no such object, or
 * if the empty string ("") is passed.
 */
bool ItemExists(QString name)
{
	if (name.length() == 0)
		return false;
	for (uint j = 0; j<ScMW->doc->Items->count(); j++)
	{
		if (name==ScMW->doc->Items->at(j)->itemName())
			return true;
	} // for items
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
    if (ScMW->HaveDoc)
        return true;
    // Caller is required to check for false return from this function
    // and return NULL.
    PyErr_SetString(NoDocOpenError, QString("Command does not make sense without an open document"));
    return false;
}

QStringList getSelectedItemsByName()
{
	/*
	QStringList names;
	QPtrListIterator<PageItem> it(ScMW->view->SelItem);
	for ( ; it.current() != 0 ; ++it)
		names.append(it.current()->itemName());
	return names;
	*/
	return ScMW->doc->m_Selection->getSelectedItemsByName();
}

bool setSelectedItemsByName(QStringList& itemNames)
{
	ScMW->view->Deselect();
	// For each named item
	for (QStringList::Iterator it = itemNames.begin() ; it != itemNames.end() ; it++)
	{
		// Search for the named item
		PageItem* item = 0;
		for (uint j = 0; j < ScMW->doc->Items->count(); j++)
			if (*it == ScMW->doc->Items->at(j)->itemName())
				item = ScMW->doc->Items->at(j);
		if (!item)
			return false;
		// and select it
		ScMW->view->SelectItemNr(item->ItemNr);
	}
	return true;
}
