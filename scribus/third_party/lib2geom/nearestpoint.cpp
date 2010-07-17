/*
** vim: ts=4 sw=4 et tw=0 wm=0
**
**  RCS Information:
**      $Author: mjw $
**      $Revision: 1 $
**      $Date: 2006-03-28 15:59:38 +1100 (Tue, 28 Mar 2006) $
**
** Solving the Nearest Point-on-Curve Problem and
** A Bezier Curve-Based Root-Finder
** by Philip J. Schneider
** from "Graphics Gems", Academic Press, 1990
** modified by mwybrow, njh
*/

/*	point_on_curve.c	*/

static double SquaredLength(const Geom::Point a) 
{
    return dot(a, a);
}


/*
 *  Forward declarations
 */
static int FindRoots(Geom::Point *w, int degree, double *t, int depth);
static Geom::Point *ConvertToBezierForm( Geom::Point 	P, Geom::Point 	*V);
static double ComputeXIntercept( Geom::Point 	*V, int		degree);
static int ControlPolygonFlatEnough( Geom::Point	*V, int 	degree);
static int CrossingCount(Geom::Point *V, int degree);
static Geom::Point Bez(Geom::Point *V, int degree, double t, Geom::Point *Left,
                     Geom::Point *Right);

int		MAXDEPTH = 64;	/*  Maximum depth for recursion */

#define	EPSILON	(ldexp(1.0,-MAXDEPTH-1)) /*Flatness control value */
#define	DEGREE	3			/*  Cubic Bezier curve		*/
#define	W_DEGREE 5			/*  Degree of eqn to find roots of */


/*
 *  NearestPointOnCurve :
 *  	Compute the parameter value of the point on a Bezier
 *		curve segment closest to some arbtitrary, user-input point.
 *		Return the point on the curve at that parameter value.
 *
 Geom::Point 	P;			The user-supplied point
 Geom::Point 	*V;			Control points of cubic Bezier
*/
double NearestPointOnCurve(Geom::Point P, Geom::Point *V)
{
    double 	t_candidate[W_DEGREE];	/* Possible roots		*/     

    /*  Convert problem to 5th-degree Bezier form	*/
    Geom::Point	*w = ConvertToBezierForm(P, V);
    
    /* Find all possible roots of 5th-degree equation */
    int n_solutions = FindRoots(w, W_DEGREE, t_candidate, 0);
    std::free((char *)w);
    
    /* Check distance to end of the curve, where t = 1 */
    double dist = SquaredLength(P - V[DEGREE]);
    double t = 1.0;

    /* Find distances for candidate points	*/
    for (int i = 0; i < n_solutions; i++) {
        Geom::Point p = Bez(V, DEGREE, t_candidate[i], NULL, NULL);
        double new_dist = SquaredLength(P - p);
        if (new_dist < dist) {
            dist = new_dist;
            t = t_candidate[i];
        }
    }

    /*  Return the parameter value t */
    return t;
}


/*
 *  ConvertToBezierForm :
 *		Given a point and a Bezier curve, generate a 5th-degree
 *		Bezier-format equation whose solution finds the point on the
 *      curve nearest the user-defined point.
 */
static Geom::Point *ConvertToBezierForm(
    Geom::Point 	P,			/* The point to find t for	*/
    Geom::Point 	*V)			/* The control points		*/
{
    Geom::Point 	c[DEGREE+1];	/* V(i)'s - P			*/
    Geom::Point 	d[DEGREE];	/* V(i+1) - V(i)		*/
    Geom::Point 	*w;		/* Ctl pts of 5th-degree curve  */
    double 	cdTable[3][4];		/* Dot product of c, d		*/
    static double z[3][4] = {	/* Precomputed "z" for cubics	*/
	{1.0, 0.6, 0.3, 0.1},
	{0.4, 0.6, 0.6, 0.4},
	{0.1, 0.3, 0.6, 1.0},
    };


    /*Determine the c's -- these are vectors created by subtracting*/
    /* point P from each of the control points				*/
    for (int i = 0; i <= DEGREE; i++) {
        c[i] = V[i] - P;
    }
    /* Determine the d's -- these are vectors created by subtracting*/
    /* each control point from the next					*/
    for (int i = 0; i <= DEGREE - 1; i++) { 
        d[i] = 3.0*(V[i+1] - V[i]);
    }

    /* Create the c,d table -- this is a table of dot products of the */
    /* c's and d's							*/
    for (int row = 0; row <= DEGREE - 1; row++) {
        for (int column = 0; column <= DEGREE; column++) {
            cdTable[row][column] = dot(d[row], c[column]);
        }
    }

    /* Now, apply the z's to the dot products, on the skew diagonal*/
    /* Also, set up the x-values, making these "points"		*/
    w = (Geom::Point *)malloc((unsigned)(W_DEGREE+1) * sizeof(Geom::Point));
    for (int i = 0; i <= W_DEGREE; i++) {
        w[i][Geom::Y] = 0.0;
        w[i][Geom::X] = (double)(i) / W_DEGREE;
    }

    const int n = DEGREE;
    const int m = DEGREE-1;
    for (int k = 0; k <= n + m; k++) {
        const int lb = std::max(0, k - m);
        const int ub = std::min(k, n);
        for (int i = lb; i <= ub; i++) {
            int j = k - i;
            w[i+j][Geom::Y] += cdTable[j][i] * z[j][i];
        }
    }

    return w;
}


/*
 *  FindRoots :
 *	Given a 5th-degree equation in Bernstein-Bezier form, find
 *	all of the roots in the interval [0, 1].  Return the number
 *	of roots found.
 */
static int FindRoots(
    Geom::Point 	*w,			/* The control points		*/
    int 	degree,		/* The degree of the polynomial	*/
    double 	*t,			/* RETURN candidate t-values	*/
    int 	depth)		/* The depth of the recursion	*/
{  
    int 	i;
    Geom::Point 	Left[W_DEGREE+1],	/* New left and right 		*/
        Right[W_DEGREE+1];	/* control polygons		*/
    int 	left_count,		/* Solution count from		*/
        right_count;		/* children			*/
    double 	left_t[W_DEGREE+1],	/* Solutions from kids		*/
        right_t[W_DEGREE+1];

    switch (CrossingCount(w, degree)) {
    case 0 : {	/* No solutions here	*/
        return 0;	
        break;
    }
    case 1 : {	/* Unique solution	*/
        /* Stop recursion when the tree is deep enough	*/
        /* if deep enough, return 1 solution at midpoint 	*/
        if (depth >= MAXDEPTH) {
            t[0] = (w[0][Geom::X] + w[W_DEGREE][Geom::X]) / 2.0;
            return 1;
        }
        if (ControlPolygonFlatEnough(w, degree)) {
            t[0] = ComputeXIntercept(w, degree);
            return 1;
        }
        break;
    }
    }

    /* Otherwise, solve recursively after	*/
    /* subdividing control polygon		*/
    Bez(w, degree, 0.5, Left, Right);
    left_count  = FindRoots(Left,  degree, left_t, depth+1);
    right_count = FindRoots(Right, degree, right_t, depth+1);


    /* Gather solutions together	*/
    for (i = 0; i < left_count; i++) {
        t[i] = left_t[i];
    }
    for (i = 0; i < right_count; i++) {
        t[i+left_count] = right_t[i];
    }

    /* Send back total number of solutions	*/
    return (left_count+right_count);
}


/*
 * CrossingCount :
 *	Count the number of times a Bezier control polygon 
 *	crosses the 0-axis. This number is >= the number of roots.
 *
 */
static int CrossingCount(
    Geom::Point	*V,			/*  Control pts of Bezier curve	*/
    int		degree)			/*  Degree of Bezier curve 	*/
{
    int 	n_crossings = 0;	/*  Number of zero-crossings	*/
    int		old_sign;		/*  Sign of coefficients	*/
    
    old_sign = Geom::sgn(V[0][Geom::Y]);
    for (int i = 1; i <= degree; i++) {
        int sign = Geom::sgn(V[i][Geom::Y]);
        if (sign != old_sign)
            n_crossings++;
        old_sign = sign;
    }
    return n_crossings;
}



/*
 *  ControlPolygonFlatEnough :
 *	Check if the control polygon of a Bezier curve is flat enough
 *	for recursive subdivision to bottom out.
 *
 */
static int ControlPolygonFlatEnough(
    Geom::Point	*V,		/* Control points	*/
    int 	degree)		/* Degree of polynomial	*/
{
    int 	i;			/* Index variable		*/
    double 	*distance;		/* Distances from pts to line	*/
    double 	max_distance_above;	/* maximum of these		*/
    double 	max_distance_below;
    double 	error;			/* Precision of root		*/
    //Geom::Point 	t;			/* Vector from V[0] to V[degree]*/
    double 	intercept_1,
        intercept_2,
        left_intercept,
        right_intercept;
    double 	a, b, c;		/* Coefficients of implicit	*/
    					/* eqn for line from V[0]-V[deg]*/

    /* Find the  perpendicular distance		*/
    /* from each interior control point to 	*/
    /* line connecting V[0] and V[degree]	*/
    distance = (double *)malloc((unsigned)(degree + 1) * 					sizeof(double));
    {
	double	abSquared;

	/* Derive the implicit equation for line connecting first */
        /*  and last control points */
	a = V[0][Geom::Y] - V[degree][Geom::Y];
	b = V[degree][Geom::X] - V[0][Geom::X];
	c = V[0][Geom::X] * V[degree][Geom::Y] - V[degree][Geom::X] * V[0][Geom::Y];

	abSquared = (a * a) + (b * b);

        for (i = 1; i < degree; i++) {
	    /* Compute distance from each of the points to that line	*/
            distance[i] = a * V[i][Geom::X] + b * V[i][Geom::Y] + c;
            if (distance[i] > 0.0) {
                distance[i] = (distance[i] * distance[i]) / abSquared;
            }
            if (distance[i] < 0.0) {
                distance[i] = -((distance[i] * distance[i]) / 						abSquared);
            }
        }
    }


    /* Find the largest distance	*/
    max_distance_above = 0.0;
    max_distance_below = 0.0;
    for (i = 1; i < degree; i++) {
        if (distance[i] < 0.0) {
            max_distance_below = std::min(max_distance_below, distance[i]);
        };
        if (distance[i] > 0.0) {
            max_distance_above = std::max(max_distance_above, distance[i]);
        }
    }
    free((char *)distance);

    {
	double	det;
	double	a1, b1, c1, a2, b2, c2;

	/*  Implicit equation for zero line */
	a1 = 0.0;
	b1 = 1.0;
	c1 = 0.0;

	/*  Implicit equation for "above" line */
	a2 = a;
	b2 = b;
	c2 = c + max_distance_above;

	det = a1 * b2 - a2 * b1;
	
	intercept_1 = (b1 * c2 - b2 * c1) / det;

	/*  Implicit equation for "below" line */
	a2 = a;
	b2 = b;
	c2 = c + max_distance_below;
	
	det = a1 * b2 - a2 * b1;
	
	intercept_2 = (b1 * c2 - b2 * c1) / det;
    }

    /* Compute intercepts of bounding box	*/
    left_intercept = std::min(intercept_1, intercept_2);
    right_intercept = std::max(intercept_1, intercept_2);

    error = 0.5 * (right_intercept-left_intercept);    
    if (error < EPSILON) {
        return 1;
    }
    else {
        return 0;
    }
}



/*
 *  ComputeXIntercept :
 *	Compute intersection of chord from first control point to last
 *  	with 0-axis.
 * 
 */
static double ComputeXIntercept(
    Geom::Point 	*V,			/*  Control points	*/
    int		degree) 		/*  Degree of curve	*/
{
    const Geom::Point A = V[degree] - V[0];

    return (A[Geom::X]*V[0][Geom::Y] - A[Geom::Y]*V[0][Geom::X]) / -A[Geom::Y];
}


/*
 *  Bez : 
 *	Evaluate a Bezier curve at a particular parameter value
 *      Fill in control points for resulting sub-curves if "Left" and
 *	"Right" are non-null.
 * 
 */
static Geom::Point Bez(
    Geom::Point 	*V,			/* Control pts			*/
    int 	degree,		/* Degree of bezier curve	*/
    double 	t,			/* Parameter value		*/
    Geom::Point 	*Left,		/* RETURN left half ctl pts	*/
    Geom::Point 	*Right)		/* RETURN right half ctl pts	*/
{
    Geom::Point 	Vtemp[W_DEGREE+1][W_DEGREE+1];


    /* Copy control points	*/
    for (int j =0; j <= degree; j++) {
        Vtemp[0][j] = V[j];
    }

    /* Triangle computation	*/
    for (int i = 1; i <= degree; i++) {	
        for (int j =0 ; j <= degree - i; j++) {
            Vtemp[i][j] =
                (1.0 - t) * Vtemp[i-1][j] + t * Vtemp[i-1][j+1];
        }
    }
    
    if (Left != NULL) {
        for (int j = 0; j <= degree; j++) {
            Left[j]  = Vtemp[j][0];
        }
    }
    if (Right != NULL) {
        for (int j = 0; j <= degree; j++) {
            Right[j] = Vtemp[degree-j][j];
        }
    }

    return (Vtemp[degree][0]);
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
