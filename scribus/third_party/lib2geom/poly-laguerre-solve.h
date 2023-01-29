#ifndef SEEN_POLY_LAGUERRE_SOLVE_H
#define SEEN_POLY_LAGUERRE_SOLVE_H

#include "poly.h"
#include <complex>

std::vector<std::complex<double> > 
laguerre(Poly ply, const double tol=1e-10);

std::vector<double> 
laguerre_real_interval(Poly  ply, 
		       const double lo, const double hi,
		       const double tol=1e-10);

#endif
