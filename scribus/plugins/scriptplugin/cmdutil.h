#ifndef CMDUTIL_H
#define CMDUTIL_H

#include "scribus.h"

double PointToValue(double Val);
double ValToPts(double Val, int ein);
double ValueToPoint(double Val);
int GetItem(QString Name);
void ReplaceColor(QString col, QString rep);
#endif

