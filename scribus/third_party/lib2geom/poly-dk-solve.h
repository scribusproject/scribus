#ifndef SEEN_POLY_DK_SOLVE_H
#define SEEN_POLY_DK_SOLVE_H

#include "poly.h"
#include <complex>

std::vector<std::complex<double> > 
DK(Poly const & ply, const double tol=1e-10);

#endif
