/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef CMDUTIL_H
#define CMDUTIL_H

// Pulls in <Python.h> first
#include "cmdvar.h"

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

// 2004-11-12 Craig Ringer see cmdutil.cpp for description
bool ItemExists(QString name);

#endif
