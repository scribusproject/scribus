#ifndef CMDUTIL_H
#define CMDUTIL_H

#include "scribus.h"

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

// 2004-10-27 Craig Ringer see cmdutil.cpp for description
bool checkHaveDocument();

#endif
