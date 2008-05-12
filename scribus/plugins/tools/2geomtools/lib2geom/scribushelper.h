/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SCRIBUSHELPER_H
#define SCRIBUSHELPER_H

#include "fpointarray.h"
#include <vector>
#include "sbasis.h"
#include "sbasis-geometric.h"
#include "bezier-to-sbasis.h"
#include "sbasis-to-bezier.h"
#include "d2.h"
#include "piecewise.h"
#include "utils.h"
#include "path.h"
#include "sbasis-2d.h"
#include <QPainterPath>

static FPoint currentPoint;
// Functions to convert from lib2geom to FPointArray and vice versa
void scribus_curve(FPointArray *cr, Geom::Curve const& c);
void geomPath2FPointArray(FPointArray *p, Geom::Path &pp);
void Piecewise2FPointArray(FPointArray *p, Geom::Piecewise<Geom::D2<Geom::SBasis> > &pp);
void D2sb2d2FPointArray(FPointArray* cr, Geom::D2<Geom::SBasis2d> const &sb2, Geom::Point dir, double width);
std::vector<Geom::Path> FPointArray2geomPath(FPointArray &p, bool closed);
Geom::Piecewise<Geom::D2<Geom::SBasis> > FPointArray2Piecewise(FPointArray &p, bool closed);

// Functions to convert from lib2geom to QPainterPath and vice versa
void arthur_curve(QPainterPath *cr, Geom::Curve const& c);
void geomPath2QPainterPath(QPainterPath *p, Geom::Path &pp);
void Piecewise2QPainterPath(QPainterPath *p, Geom::Piecewise<Geom::D2<Geom::SBasis> > &pp);
void D2sb2d2QPainterPath(QPainterPath* cr, Geom::D2<Geom::SBasis2d> const &sb2, Geom::Point dir, double width);
std::vector<Geom::Path> QPainterPath2geomPath(QPainterPath &p, bool closed);
Geom::Piecewise<Geom::D2<Geom::SBasis> > QPainterPath2Piecewise(QPainterPath &p, bool closed);

#endif
