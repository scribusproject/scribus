/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef CMDUTIL_H
#define CMDUTIL_H

// Pulls in <Python.h> first
#include "cmdvar.h"

double PointToValue(double Val);
double ValueToPoint(double Val);

/*! \brief Convert an X co-ordinate part in page units to a document co-ordinate in system units.
*/
double pageUnitXToDocX(double pageUnitX);
/// \brief Doc units -> page-relative units
double docUnitXToPageX(double pageUnitX);

/*! \brief Convert a Y co-ordinate part in page units to a document co-ordinate
in system units. The document co-ordinates have their origin somewere
up and left of the first page, where page co-ordinates have their
origin on the top left of the current page. */
double pageUnitYToDocY(double pageUnitY);
/// \brief Doc units -> page-relative units
double docUnitYToPageY(double pageUnitY);

int GetItem(QString Name);
void ReplaceColor(QString col, QString rep);
/*!
 * @brief Returns named PageItem, or selection if name '', or exception and NULL if no item.
 *
 * Returns a pointer to a PageItem by looking it up by name - page independent.
 * If `name' is empty, returns selected item.  If the named item is not found,
 * or `name' is empty and no item is selected, sets an exception and returns
 * NULL.
 *
 * @author 03/10/2004 petr vanek
 * @author 05/02/02 Craig Ringer
 */
PageItem* GetUniqueItem(QString name);

/*!
 * @brief Returns named PageItem, or exception and NULL if not found.
 *
 * @author 03/10/2004 Petr Vanek
 * @author 05/02/02 Craig Ringer
 */
PageItem* getPageItemByName(QString name);

// 2004-10-27 Craig Ringer see cmdutil.cpp for description
bool checkHaveDocument();

// 2004-11-12 Craig Ringer see cmdutil.cpp for description
bool ItemExists(QString name);

/*!
 * @brief Returns a list of the names of all selected PageItems
 */
QStringList getSelectedItemsByName();
/*!
 * @brief Replaces the current selection by selecting all the items named in the passed QStringList
 *
 * Returns false if one or more items can't be selected, true if all were selected.
 * Selection state is undefined on failure.
 */
bool setSelectedItemsByName(QStringList& itemNames);

#endif
