#ifndef CMDUTIL_H
#define CMDUTIL_H

#include "scribus.h"

/*! Macro to check for Carrier->HaveDoc and set a sensible
exception if not found. */
#define HAVEDOC_OR_ERR if (!Carrier->HaveDoc) { PyErr_SetString(NoDocOpenError, "Command does not make sense without an open document"); return NULL; }


double PointToValue(double Val);
double ValToPts(double Val, int ein);
double ValueToPoint(double Val);
int GetItem(QString Name);
void ReplaceColor(QString col, QString rep);
/*!
 * Returns PageItem itself by its unique name - page
 * independent.
 * 03/10/2004 petr vanek
 */
PageItem* GetUniqueItem(QString name);

#endif
