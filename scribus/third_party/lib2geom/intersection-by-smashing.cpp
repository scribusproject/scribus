#include <d2.h>
#include <sbasis.h>
#include <sbasis-geometric.h>
#include <intersection-by-smashing.h>

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>


namespace Geom {
using namespace Geom;

/*
 * Computes the top and bottom boundaries of the L_\infty neighborhood
 * of a curve. The curve is supposed to be a graph over the x-axis.
 */
static
void computeLinfinityNeighborhood( D2<SBasis > const &f, double tol, D2<Piecewise<SBasis> > &topside, D2<Piecewise<SBasis> > &botside ){
	double signx = ( f[X].at0() > f[X].at1() )? -1 : 1;
	double signy = ( f[Y].at0() > f[Y].at1() )? -1 : 1;

	Piecewise<D2<SBasis> > top, bot;
	top = Piecewise<D2<SBasis> > (f);
	top.cuts.insert( top.cuts.end(), 2);
	top.segs.insert( top.segs.end(), D2<SBasis>(Linear( f[X].at1(), f[X].at1()+2*tol*signx),
			                                    Linear( f[Y].at1() )) );
	bot = Piecewise<D2<SBasis> >(f);
	bot.cuts.insert( bot.cuts.begin(), - 1 );
	bot.segs.insert( bot.segs.begin(), D2<SBasis>(Linear( f[X].at0()-2*tol*signx, f[X].at0()),
												  Linear( f[Y].at0() )) );
	top += Point(-tol*signx,  tol);
	bot += Point( tol*signx, -tol);

	if ( signy < 0 ){
		std::swap( top, bot );
		top += Point( 0,  2*tol);
		bot += Point( 0, -2*tol);
	}
	topside = make_cuts_independent(top);
	botside = make_cuts_independent(bot);
}


/*
 * Compute top and bottom boundaries of the L^infty nbhd of the graph of a *monotonic* function f.
 * if f is increasing, it is given by [f(t-tol)-tol, f(t+tol)+tol].
 * if not, it is [f(t+tol)-tol, f(t-tol)+tol].
 */
static
void computeLinfinityNeighborhood( Piecewise<SBasis> const &f, double tol, Piecewise<SBasis> &top, Piecewise<SBasis> &bot){
	top = f + tol;
	top.offsetDomain( - tol );
	top.cuts.insert( top.cuts.end(), f.domain().max() + tol);
	top.segs.insert( top.segs.end(), SBasis(Linear( f.lastValue() + tol )) );

	bot = f - tol;
	bot.offsetDomain( tol );
	bot.cuts.insert( bot.cuts.begin(), f.domain().min() - tol);
	bot.segs.insert( bot.segs.begin(), SBasis(Linear( f.firstValue() - tol )) );

	if ( f.firstValue() > f.lastValue() ){
	std::swap( top, bot );
	top += 2*tol;
	bot -= 2*tol;
	}
}

/*
 * Returns the intervals over which the curve keeps its slope
 * in one of the 8 sectors delimited by x=0, y=0, y=x, y=-x.
 */
static
std::vector<Interval> monotonicSplit(D2<SBasis> const &p){
	std::vector<Interval> result;

	D2<SBasis> v = derivative(p);

	std::vector<double> someroots;
	std::vector<double> cuts (2,0.);
	cuts[1] = 1.;

	someroots = roots(v[X]);
	cuts.insert( cuts.end(), someroots.begin(), someroots.end() );

	someroots = roots(v[Y]);
	cuts.insert( cuts.end(), someroots.begin(), someroots.end() );

	//we could split in the middle to avoid computing roots again...
	someroots = roots(v[X]-v[Y]);
	cuts.insert( cuts.end(), someroots.begin(), someroots.end() );

	someroots = roots(v[X]+v[Y]);
	cuts.insert( cuts.end(), someroots.begin(), someroots.end() );

	sort(cuts.begin(),cuts.end());
	unique(cuts.begin(), cuts.end() );

	for (unsigned i=1; i<cuts.size(); i++){
		result.push_back( Interval( cuts[i-1], cuts[i] ) );
	}
	return result;
}

//std::vector<Interval> level_set( D2<SBasis> const &f, Rect region){
//	std::vector<Interval> x_in_reg = level_set( f[X], region[X] );
//	std::vector<Interval> y_in_reg = level_set( f[Y], region[Y] );
//	std::vector<Interval> result = intersect ( x_in_reg, y_in_reg );
//	return result;
//}

/*TODO: remove this!!!
 * the minimum would be to move it to piecewise.h but this would be stupid.
 * The best would be to let 'compose' be aware of extension modes (constant, linear, polynomial..)
 * (I think the extension modes (at start and end) should be properties of the pwsb).
 */
static
void prolongateByConstants( Piecewise<SBasis> &f, double paddle_width ){
	if ( f.size() == 0 ) return; //do we have a covention about the domain of empty pwsb?
	f.cuts.insert( f.cuts.begin(), f.cuts.front() - paddle_width );
	f.segs.insert( f.segs.begin(), SBasis( f.segs.front().at0() ) );
	f.cuts.insert( f.cuts.end(), f.cuts.back() + paddle_width );
	f.segs.insert( f.segs.end(), SBasis( f.segs.back().at1() ) );
}

static
bool compareIntersectionsTimesX( Intersection const &inter1, Intersection const &inter2 ){
	return inter1.times[X].min() < inter2.times[Y].min();
}
/*Fuse contiguous intersection domains
 *
 */
static
void cleanup_and_fuse( std::vector<Intersection> &inters ){
	std::sort( inters.begin(), inters.end(), compareIntersectionsTimesX);
	for (unsigned i=0; i < inters.size(); i++ ){
		for (unsigned j=i+1; j < inters.size() && inters[i].times[X].intersects( inters[j].times[X]) ; j++ ){
			if (inters[i].times[Y].intersects( inters[j].times[Y] ) ){
				inters[i].times.unionWith(inters[j].times);
				inters[i].bbox.unionWith(inters[j].bbox);
				inters.erase( inters.begin() + j );
			}
		}
	}
}

/* Computes the intersection of two sets given as (ordered) union intervals.
 */
static
std::vector<Interval> intersect( std::vector<Interval> const &a, std::vector<Interval> const &b){
	std::vector<Interval> result;
	//TODO: use order to optimize this!
	for (unsigned i=0; i < a.size(); i++){
		for (unsigned j=0; j < b.size(); j++){
			OptInterval c( a[i] );
			c &= b[j];
			if ( c ) {
				result.push_back( *c );
			}
		}
	}
	return result;
}

/* Returns the intervals over which the curves are in the
 * tol-neighborhood one of the other for the L_\infty metric.
 * WARNING: each curve is supposed to be a graph over x or y axis
 * (but not necessarily the same axis for both) and the smaller
 * the slope the better (typically <=45°).
 */
std::vector<Intersection> monotonic_smash_intersect( D2<SBasis> const &a, D2<SBasis> const &b, double tol){

	// a and b or X and Y may have to be exchanged, so make local copies.
	D2<SBasis> aa = a;
	D2<SBasis> bb = b;
	bool swapresult = false;
	bool swapcoord = false;//debug only!

	//if the (enlarged) bounding boxes don't intersect, stop.
	OptRect abounds = bounds_fast( a );
	OptRect bbounds = bounds_fast( b );
	if ( !abounds || !bbounds ) return std::vector<Intersection>();
	abounds->expandBy(tol);
	if ( !(abounds->intersects(*bbounds))){
		return std::vector<Intersection>();
	}

	//Choose the best curve to be re-parametrized by x or y values.
	OptRect dabounds = bounds_exact(derivative(a));
	OptRect dbbounds = bounds_exact(derivative(b));
	if	( dbbounds->min().length() > dabounds->min().length() ){
		aa=b;
		bb=a;
		std::swap( dabounds, dbbounds );
		swapresult = true;
	}

	//Choose the best coordinate to use as new parameter
	double dxmin = std::min( std::abs((*dabounds)[X].max()), std::abs((*dabounds)[X].min()) );
	double dymin = std::min( std::abs((*dabounds)[Y].max()), std::abs((*dabounds)[Y].min()) );
	if ( (*dabounds)[X].max()*(*dabounds)[X].min() < 0 ) dxmin=0;
	if ( (*dabounds)[Y].max()*(*dabounds)[Y].min() < 0 ) dymin=0;
	assert (dxmin>=0 && dymin>=0);

	if (dxmin < dymin) {
		aa = D2<SBasis>( aa[Y], aa[X] );
		bb = D2<SBasis>( bb[Y], bb[X] );
		swapcoord = true;
	}

	//re-parametrize aa by the value of x.
	Interval x_range_strict( aa[X].at0(), aa[X].at1() );
	Piecewise<SBasis> y_of_x = pw_compose_inverse(aa[Y],aa[X], 2, 1e-5);

	//Compute top and bottom boundaries of the L^infty nbhd of aa.
	Piecewise<SBasis> top_ay, bot_ay;
	computeLinfinityNeighborhood( y_of_x, tol, top_ay, bot_ay);

	Interval ax_range = top_ay.domain();//i.e. aa[X] domain ewpanded by tol.
	std::vector<Interval> bx_in_ax_range = level_set(bb[X], ax_range );

	// find times when bb is in the neighborhood of aa.
	std::vector<Interval> tbs;
	for (unsigned i=0; i<bx_in_ax_range.size(); i++){
		D2<Piecewise<SBasis> > bb_in;
		bb_in[X] = Piecewise<SBasis> ( portion( bb[X], bx_in_ax_range[i] ) );
		bb_in[Y] = Piecewise<SBasis> ( portion( bb[Y], bx_in_ax_range[i]) );
		bb_in[X].setDomain( bx_in_ax_range[i] );
		bb_in[Y].setDomain( bx_in_ax_range[i] );

		Piecewise<SBasis> h;
		Interval level;
		h = bb_in[Y] - compose( top_ay, bb_in[X] );
		level = Interval( -infinity(), 0 );
		std::vector<Interval> rts_lo = level_set( h, level);
		h = bb_in[Y] - compose( bot_ay, bb_in[X] );
		level = Interval( 0, infinity());
		std::vector<Interval> rts_hi = level_set( h, level);

		std::vector<Interval> rts = intersect( rts_lo, rts_hi );
		tbs.insert(tbs.end(), rts.begin(),  rts.end()  );
	}

	std::vector<Intersection > result(tbs.size(), Intersection() );

	/* for each solution I, find times when aa is in the neighborhood of bb(I).
	 * (Note: the preimage of bb[X](I) by aa[X], enlarged by tol, is a good approximation of this:
	 * it would give points in the 2*tol neighborhood of bb (if the slope of aa is never more than 1).
	 *  + faster computation.
	 *  - implies little jumps depending on the subdivision of the input curve into monotonic pieces
	 *  and on the choice of prefered axis. If noticable, these jumps would feel random to the user :-(
	 */
	for (unsigned j=0; j<tbs.size(); j++){
		result[j].times[Y] = tbs[j];
		std::vector<Interval> tas;
		//TODO: replace this by some option in the "compose(pw,pw)" method!
		Piecewise<SBasis> fat_y_of_x = y_of_x;
		prolongateByConstants( fat_y_of_x, 100*(1+tol) );

		D2<Piecewise<SBasis> > top_b, bot_b;
		D2<SBasis> bbj = portion( bb, tbs[j] );
		computeLinfinityNeighborhood( bbj, tol, top_b, bot_b );

		Piecewise<SBasis> h;
		Interval level;
		h = top_b[Y] - compose( fat_y_of_x, top_b[X] );
		level = Interval( +infinity(), 0 );
		std::vector<Interval> rts_top = level_set( h, level);
		for (unsigned idx=0; idx < rts_top.size(); idx++){
			rts_top[idx] = Interval( top_b[X].valueAt( rts_top[idx].min() ),
									 top_b[X].valueAt( rts_top[idx].max() ) );
		}
		assert( rts_top.size() == 1 );

		h = bot_b[Y] - compose( fat_y_of_x, bot_b[X] );
		level = Interval( 0, -infinity());
		std::vector<Interval> rts_bot = level_set( h, level);
		for (unsigned idx=0; idx < rts_bot.size(); idx++){
			rts_bot[idx] = Interval( bot_b[X].valueAt( rts_bot[idx].min() ),
									 bot_b[X].valueAt( rts_bot[idx].max() ) );
		}
		assert( rts_bot.size() == 1 );
		rts_top = intersect( rts_top, rts_bot );
		assert (rts_top.size() == 1);
		Interval x_dom = rts_top[0];

		if ( x_dom.max() <= x_range_strict.min() ){
			tas.push_back( Interval ( ( aa[X].at0() < aa[X].at1() ) ? 0 : 1 ) );
		}else if ( x_dom.min() >= x_range_strict.max() ){
			tas.push_back( Interval ( ( aa[X].at0() < aa[X].at1() ) ? 1 : 0 ) );
		}else{
			tas = level_set(aa[X], x_dom );
		}
		assert( tas.size()==1 );
		result[j].times[X] = tas.front();

		result[j].bbox = Rect( bbj.at0(), bbj.at1() );
		Interval y_dom( aa[Y](result[j].times[X].min()), aa[Y](result[j].times[X].max()) );
		result[j].bbox.unionWith( Rect( x_dom, y_dom ) );
	}

	if (swapresult) {
		for ( unsigned i=0; i<result.size(); i++){
			std::swap( result[i].times[X], result[i].times[Y]);
		}
	}
	if (swapcoord) {
		for ( unsigned i=0; i<result.size(); i++){
			std::swap( result[i].bbox[X], result[i].bbox[Y] );
		}
	}

	//TODO: cleanup result? fuse contiguous intersections?
	return result;
}

std::vector<Intersection> smash_intersect( D2<SBasis> const &a, D2<SBasis> const &b, double tol){
	std::vector<Intersection> result;

	std::vector<Interval> acuts = monotonicSplit(a);
	std::vector<Interval> bcuts = monotonicSplit(b);
	for (unsigned i=0; i<acuts.size(); i++){
		D2<SBasis> ai = portion( a, acuts[i]);
		for (unsigned j=0; j<bcuts.size(); j++){
			D2<SBasis> bj = portion( b, bcuts[j]);
			std::vector<Intersection> ai_cap_bj = monotonic_smash_intersect( ai, bj, tol );
			for (unsigned k=0; k < ai_cap_bj.size(); k++){
				ai_cap_bj[k].times[X] = ai_cap_bj[k].times[X] * acuts[i].extent() + acuts[i].min();
				ai_cap_bj[k].times[Y] = ai_cap_bj[k].times[Y] * bcuts[j].extent() + bcuts[j].min();
			}
			result.insert( result.end(), ai_cap_bj.begin(), ai_cap_bj.end() );
		}
	}
	cleanup_and_fuse( result );
	return result;
}

}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=99 :
