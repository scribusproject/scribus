#include "cmdutil.h"
#include "units.h"

ScribusApp* Carrier;
ScribusDoc* doc;

double PointToValue(double Val)
{
	return pts2value(Val, Carrier->doc->docUnitIndex);
}

double ValToPts(double Val, int ein)
{
	//TODO Remove references to the old function, use the units.cpp one only in the future.
	return value2pts(Val, ein);
}

double ValueToPoint(double Val)
{
	return value2pts(Val, Carrier->doc->docUnitIndex);
}

/// Convert an X co-ordinate part in page units to a document co-ordinate
/// in system units.
double pageUnitXToDocX(double pageUnitX)
{
	return ValueToPoint(pageUnitX) + Carrier->doc->currentPage->Xoffset;
}

// Convert doc units to page units
double docUnitXToPageX(double pageUnitX)
{
	return PointToValue(pageUnitX - Carrier->doc->currentPage->Xoffset);
}

/// Convert a Y co-ordinate part in page units to a document co-ordinate
/// in system units. The document co-ordinates have their origin somewere
/// up and left of the first page, where page co-ordinates have their
/// origin on the top left of the current page.
double pageUnitYToDocY(double pageUnitY)
{
	return ValueToPoint(pageUnitY) + Carrier->doc->currentPage->Yoffset;
}

double docUnitYToPageY(double pageUnitY)
{
	return PointToValue(pageUnitY - Carrier->doc->currentPage->Yoffset);
}

int GetItem(QString Name)
{
	if (Name != "")
	{
		for (uint a = 0; a < Carrier->doc->Items.count(); a++)
		{
			if (Carrier->doc->Items.at(a)->itemName() == Name)
				return static_cast<int>(a);
		}
	}
	else
	{
		if (Carrier->view->SelItem.count() != 0)
			return Carrier->view->SelItem.at(0)->ItemNr;
	}
	return -1;
}

void ReplaceColor(QString col, QString rep)
{
	QColor tmpc;
	for (uint c = 0; c < Carrier->doc->Items.count(); c++)
	{
		PageItem *ite = Carrier->doc->Items.at(c);
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
	for (uint c = 0; c < Carrier->doc->MasterItems.count(); c++)
	{
		PageItem *ite = Carrier->doc->MasterItems.at(c);
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
		if (Carrier->view->SelItem.count() != 0)
			return Carrier->view->SelItem.at(0);
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
	for (uint j = 0; j<Carrier->doc->Items.count(); j++)
	{
		if (name==Carrier->doc->Items.at(j)->itemName())
			return Carrier->doc->Items.at(j);
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
	for (uint j = 0; j<Carrier->doc->Items.count(); j++)
	{
		if (name==Carrier->doc->Items.at(j)->itemName())
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
    if (Carrier->HaveDoc)
        return true;
    // Caller is required to check for false return from this function
    // and return NULL.
    PyErr_SetString(NoDocOpenError, QString("Command does not make sense without an open document"));
    return false;
}
