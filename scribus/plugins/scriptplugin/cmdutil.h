#ifndef CMDUTIL_H
#define CMDUTIL_H

#include <Python.h>
#include "scribus.h"

double PointToValue(double Val);
double ValToPts(double Val, int ein);
double ValueToPoint(double Val);
int GetItem(QString Name);
#endif

