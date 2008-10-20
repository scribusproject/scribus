/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "scribushelper.h"

// Functions to convert from lib2geom to FPointArray and vice versa
void scribus_curve(FPointArray *cr, Geom::Curve const& c)
{
	if(Geom::LineSegment const* line_segment = dynamic_cast<Geom::LineSegment const*>(&c))
	{
		cr->addPoint(currentPoint);
		cr->addPoint(currentPoint);
		cr->addPoint((*line_segment)[1][0], (*line_segment)[1][1]);
		cr->addPoint((*line_segment)[1][0], (*line_segment)[1][1]);
		currentPoint = FPoint((*line_segment)[1][0], (*line_segment)[1][1]);
	}
	else if(Geom::QuadraticBezier const *quadratic_bezier = dynamic_cast<Geom::QuadraticBezier const*>(&c))
	{
		std::vector<Geom::Point> points = quadratic_bezier->points();
		Geom::Point b1 = points[0] + (2./3) * (points[1] - points[0]);
		Geom::Point b2 = b1 + (1./3) * (points[2] - points[0]);
		cr->addPoint(currentPoint);
		cr->addPoint(b1[0], b1[1]);
		cr->addPoint(points[2][0], points[2][1]);
		cr->addPoint(b2[0], b2[1]);
		currentPoint = FPoint(points[2][0], points[2][1]);
	}
	else if(Geom::CubicBezier const *cubic_bezier = dynamic_cast<Geom::CubicBezier const*>(&c))
	{
		std::vector<Geom::Point> points = cubic_bezier->points();
		cr->addPoint(currentPoint);
		cr->addPoint(points[1][0], points[1][1]);
		cr->addPoint(points[3][0], points[3][1]);
		cr->addPoint(points[2][0], points[2][1]);
		currentPoint = FPoint(points[3][0], points[3][1]);
	}
	else
	{
		//this case handles sbasis as well as all other curve types
		Geom::Path sbasis_path = Geom::path_from_sbasis(c.toSBasis(), 0.1);
		currentPoint = FPoint(sbasis_path.initialPoint()[0], sbasis_path.initialPoint()[1]);
		//recurse to convert the new path resulting from the sbasis to svgd
		for(Geom::Path::iterator iter = sbasis_path.begin(); iter != sbasis_path.end(); ++iter)
		{
			scribus_curve(cr, *iter);
		}
	}
}

void geomPath2FPointArray(FPointArray *p, Geom::Path &pp)
{
	currentPoint = FPoint(pp.initialPoint()[0], pp.initialPoint()[1]);
	for(Geom::Path::iterator iter(pp.begin()), end(pp.end()); iter != end; ++iter)
	{
		scribus_curve(p, *iter);
	}
	if (pp.closed())
		p->setMarker();
}

void Piecewise2FPointArray(FPointArray *p, Geom::Piecewise<Geom::D2<Geom::SBasis> > &pp)
{
	std::vector<Geom::Path> pa = path_from_piecewise( pp, 0.1);
	std::vector<Geom::Path>::iterator it(pa.begin());
	for(; it != pa.end(); it++)
	{
		geomPath2FPointArray(p, *it);
	}
}

void D2sb2d2FPointArray(FPointArray* cr, Geom::D2<Geom::SBasis2d> const &sb2, int num, double width)
{
	Geom::D2<Geom::SBasis> B;
	for(int ui = 0; ui <= num; ui++)
	{
		double u = ui / static_cast<double>(num);
		B[0] = extract_u(sb2[0], u);// + Linear(u);
		B[1] = extract_u(sb2[1], u);
		for(unsigned i = 0; i < 2; i ++)
		{
			B[i] = B[i]*(width/2) + Geom::Linear(width/4);
		}
		Geom::Path pp = path_from_sbasis(B, 0.1);
		geomPath2FPointArray(cr, pp);
		cr->setMarker();
	}
	for(int vi = 0; vi <= num; vi++)
	{
		double v = vi / static_cast<double>(num);
		B[1] = extract_v(sb2[1], v);// + Linear(v);
		B[0] = extract_v(sb2[0], v);
		for(unsigned i = 0; i < 2; i ++)
		{
			B[i] = B[i]*(width/2) + Geom::Linear(width/4);
		}
		Geom::Path pp = path_from_sbasis(B, 0.1);
		geomPath2FPointArray(cr, pp);
		cr->setMarker();
	}
}

std::vector<Geom::Path> FPointArray2geomPath(FPointArray &p, bool closed)
{
	std::vector<Geom::Path> pa;
	Geom::Path ret = Geom::Path();
	Geom::Point cur;
	FPoint np, np1, np2, np3;
	bool nPath = true;
	if (p.size() > 3)
	{
		for (uint poi=0; poi < p.size()-3; poi += 4)
		{
			if (p.point(poi).x() > 900000)
			{
				if (closed)
					ret.close();
				pa.push_back(ret);
				ret.clear();
				nPath = true;
				continue;
			}
			if (nPath)
			{
				np = p.point(poi);
				cur = Geom::Point(np.x(), np.y());
				nPath = false;
			}
			np = p.point(poi);
			np1 = p.point(poi+1);
			np2 = p.point(poi+3);
			np3 = p.point(poi+2);
			if ((np == np1) && (np2 == np3))
			{
			//	Geom::Point pe = Geom::Point(np3.x(), np3.y());
			//	ret.append(Geom::LineSegment(cur, pe));
				Geom::Point pc1 = Geom::Point(np1.x()+0.001, np1.y()+0.001);
				Geom::Point pc2 = Geom::Point(np2.x()+0.001, np2.y()+0.001);
				Geom::Point pe = Geom::Point(np3.x(), np3.y());
				ret.append(Geom::CubicBezier(cur, pc1, pc2, pe));
				cur = pe;
			}
			else
			{
				Geom::Point pc1 = Geom::Point(np1.x(), np1.y());
				Geom::Point pc2 = Geom::Point(np2.x(), np2.y());
				Geom::Point pe = Geom::Point(np3.x(), np3.y());
				ret.append(Geom::CubicBezier(cur, pc1, pc2, pe));
				cur = pe;
			}
		}
	}
	if (closed)
		ret.close();
	pa.push_back(ret);
	return pa;
}

Geom::Piecewise<Geom::D2<Geom::SBasis> > FPointArray2Piecewise(FPointArray &p, bool closed)
{
	Geom::Piecewise<Geom::D2<Geom::SBasis> > patternpwd2;
	std::vector<Geom::Path> originald = FPointArray2geomPath(p, closed);
	for (unsigned int i=0; i < originald.size(); i++)
	{
		patternpwd2.concat( originald[i].toPwSb() );
	}
	return patternpwd2;
}

// Functions to convert from lib2geom to QPainterPath and vice versa
void arthur_curve(QPainterPath *cr, Geom::Curve const& c)
{
	if(Geom::LineSegment const* line_segment = dynamic_cast<Geom::LineSegment const*>(&c))
	{
		cr->lineTo(QPointF((*line_segment)[1][0], (*line_segment)[1][1]));
	}
	else if(Geom::QuadraticBezier const *quadratic_bezier = dynamic_cast<Geom::QuadraticBezier const*>(&c))
	{
		std::vector<Geom::Point> points = quadratic_bezier->points();
		Geom::Point b1 = points[0] + (2./3) * (points[1] - points[0]);
		Geom::Point b2 = b1 + (1./3) * (points[2] - points[0]);
		cr->cubicTo(b1[0], b1[1], b2[0], b2[1], points[2][0], points[2][1]);
	}
	else if(Geom::CubicBezier const *cubic_bezier = dynamic_cast<Geom::CubicBezier const*>(&c))
	{
		std::vector<Geom::Point> points = cubic_bezier->points();
		cr->cubicTo(points[1][0], points[1][1], points[2][0], points[2][1], points[3][0], points[3][1]);
	}
	else
	{
		//this case handles sbasis as well as all other curve types
		Geom::Path sbasis_path = Geom::path_from_sbasis(c.toSBasis(), 0.1);
		cr->moveTo(sbasis_path.initialPoint()[0], sbasis_path.initialPoint()[1]);
		//recurse to convert the new path resulting from the sbasis to svgd
		for(Geom::Path::iterator iter = sbasis_path.begin(); iter != sbasis_path.end(); ++iter)
		{
			arthur_curve(cr, *iter);
		}
	}
}

void geomPath2QPainterPath(QPainterPath *p, Geom::Path &pp)
{
	p->moveTo(pp.initialPoint()[0], pp.initialPoint()[1]);
	for(Geom::Path::iterator iter(pp.begin()), end(pp.end()); iter != end; ++iter)
	{
		arthur_curve(p, *iter);
	}
	if (pp.closed())
		p->closeSubpath();
}

void Piecewise2QPainterPath(QPainterPath *p, Geom::Piecewise<Geom::D2<Geom::SBasis> > &pp)
{
	std::vector<Geom::Path> pa = path_from_piecewise( pp, 0.1);
	std::vector<Geom::Path>::iterator it(pa.begin());
	for(; it != pa.end(); it++)
	{
		geomPath2QPainterPath(p, *it);
	}
}

void D2sb2d2QPainterPath(QPainterPath* cr, Geom::D2<Geom::SBasis2d> const &sb2, int num, double width)
{
	Geom::D2<Geom::SBasis> B;
	for(int ui = 0; ui <= num; ui++)
	{
		double u = ui / static_cast<double>(num);
		B[0] = extract_u(sb2[0], u);// + Linear(u);
		B[1] = extract_u(sb2[1], u);
		for(unsigned i = 0; i < 2; i ++)
		{
			B[i] = B[i]*(width/2) + Geom::Linear(width/4);
		}
		Geom::Path pp = path_from_sbasis(B, 0.1);
		geomPath2QPainterPath(cr, pp);
//		cr->setMarker();
	}
	for(int vi = 0; vi <= num; vi++)
	{
		double v = vi / static_cast<double>(num);
		B[1] = extract_v(sb2[1], v);// + Linear(v);
		B[0] = extract_v(sb2[0], v);
		for(unsigned i = 0; i < 2; i ++)
		{
			B[i] = B[i]*(width/2) + Geom::Linear(width/4);
		}
		Geom::Path pp = path_from_sbasis(B, 0.1);
		geomPath2QPainterPath(cr, pp);
//		cr->setMarker();
	}
}

std::vector<Geom::Path> QPainterPath2geomPath(QPainterPath &p, bool closed)
{
	std::vector<Geom::Path> pa;
	Geom::Path ret = Geom::Path();
	Geom::Point cur;
	bool WasM = false;
	for (int i = 0; i < p.elementCount(); ++i)
	{
		const QPainterPath::Element &elm = p.elementAt(i);
		switch (elm.type)
		{
			case QPainterPath::MoveToElement:
				if (WasM)
				{
					if (closed)
						ret.close();
					pa.push_back(ret);
					ret.clear();
				}
				WasM = true;
				cur = Geom::Point(elm.x, elm.y);
				break;
			case QPainterPath::LineToElement:
				ret.append(Geom::LineSegment(cur, Geom::Point(elm.x, elm.y)));
				cur = Geom::Point(elm.x, elm.y);
				break;
			case QPainterPath::CurveToElement:
				{
					Geom::Point pc1 = Geom::Point(elm.x, elm.y);
					Geom::Point pc2 = Geom::Point(p.elementAt(i+1).x, p.elementAt(i+1).y);
					Geom::Point pe = Geom::Point(p.elementAt(i+2).x, p.elementAt(i+2).y);
					ret.append(Geom::CubicBezier(cur, pc1, pc2, pe));
					cur = pe;
				}
				break;
			default:
				break;
		}
	}
	if (closed)
		ret.close();
	pa.push_back(ret);
	return pa;
}

Geom::Piecewise<Geom::D2<Geom::SBasis> > QPainterPath2Piecewise(QPainterPath &p, bool closed)
{
	Geom::Piecewise<Geom::D2<Geom::SBasis> > patternpwd2;
	std::vector<Geom::Path> originald = QPainterPath2geomPath(p, closed);
	for (unsigned int i=0; i < originald.size(); i++)
	{
		patternpwd2.concat( originald[i].toPwSb() );
	}
	return patternpwd2;
}
