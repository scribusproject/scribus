/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#include <cmath>
#include "framelinkrouter.h"

#include "pageitem.h"
#include "scpainter.h"
#include "fpoint.h"
#include "fpointarray.h"


FrameLinkRouter::FrameLinkRouter(PageItem* src, PageItem* dst, double scale)
	: m_srcItem(src)
	, m_dstItem(dst)
	, m_scale(scale)
{
}

/**
 * Computes the full route from source to destination.
 *
 * Steps:
 *   1. Compute bounding geometry for both frames (handles rotation, groups)
 *   2. Pick the best exit edge on each frame (e.g. Right on src, Left on dst)
 *   3. Compute exact connection points on those edges (with chain offsets)
 *   4. Build the orthogonal waypoint route between those points
 */
void FrameLinkRouter::route()
{
	m_sourceGeometry = computeFrameGeom(m_srcItem);
	m_destinationGeometry = computeFrameGeom(m_dstItem);
	m_sourceEdge = bestExitEdge(m_sourceGeometry, m_destinationGeometry);
	m_destinationEdge = bestEntryEdge(m_destinationGeometry, m_sourceGeometry, m_sourceEdge);
	computeConnectionPoints();
	m_route = buildRoute(m_startPoint, m_sourceEdge, m_endPoint, m_destinationEdge, m_sourceGeometry, m_destinationGeometry);
}

/**
 * Renders the computed route onto the canvas.
 *
 * If the frames are touching (gap < 75% of exit padding), draws a small
 * chevron marker instead of a full line, since there's no room for a
 * clean route.
 *
 * Otherwise draws:
 *   - The orthogonal polyline with rounded corners at each bend
 *   - A filled arrowhead at the destination end
 *
 * Line width is 1 screen pixel (1.0/scale so it stays constant on screen).
 * Corner radius also scales to remain visually consistent at all zoom levels.
 */
void FrameLinkRouter::draw(ScPainter* painter) const
{
	if (m_route.size() < 2)
		return;
	if (isTouching())
	{
		drawChevron(painter);
		return;
	}
	painter->setPen(Qt::black, 1.0 / m_scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	painter->setPenOpacity(1.0);
	drawRoundedOrthogonalPath(painter, m_route, m_cornerRadius / m_scale);
	drawArrowHead(painter, m_route.last(), m_route[m_route.size() - 2]);
}

/**
 * Computes the exact start and end points on the chosen edges.
 *
 * For mid-chain frames (where a frame both receives and sends links),
 * the connection point is shifted along the edge tangent so that the
 * incoming and outgoing lines don't overlap. This offset only applies
 * when the same edge is used for both directions (e.g. a frame that
 * receives from the left and sends to the left).
 *
 * Offset is expressed as a fraction of the edge's half-length:
 *   - Outgoing connection shifts in the +tangent direction (+m_chainOffset)
 *   - Incoming connection shifts in the -tangent direction (-m_chainOffset)
 */
void FrameLinkRouter::computeConnectionPoints()
{
	double srcOffset = 0.0;
	double dstOffset = 0.0;

	// Check if source frame's outgoing edge is the same edge where it receives
	// its incoming link. If so, shift the outgoing point along the edge.
	if (m_srcIsAlsoEntry && m_srcItem->prevInChain() != nullptr)
	{
		FrameGeometry prevGeom = computeFrameGeom(m_srcItem->prevInChain());
		Edge prevArrivalEdge = bestExitEdge(m_sourceGeometry, prevGeom);
		if (prevArrivalEdge == m_sourceEdge)
			srcOffset = m_chainOffset;
	}

	// Same check for destination: if its incoming edge is also used for
	// its outgoing link, shift the incoming point the other direction.
	if (m_dstIsAlsoExit && m_dstItem->nextInChain() != nullptr)
	{
		FrameGeometry nextGeom = computeFrameGeom(m_dstItem->nextInChain());
		Edge nextDepartEdge = bestExitEdge(m_destinationGeometry, nextGeom);
		if (nextDepartEdge == m_destinationEdge)
			dstOffset = -m_chainOffset;
	}

	m_startPoint = edgeMidpoint(m_sourceGeometry, m_sourceEdge, srcOffset);
	m_endPoint   = edgeMidpoint(m_destinationGeometry, m_destinationEdge, dstOffset);
}

bool FrameLinkRouter::isHorizontalEdge(Edge e)
{
	return (e == Edge::Left || e == Edge::Right);
}

void FrameLinkRouter::edgeDirection(Edge edge, double& dx, double& dy)
{
	dx = dy = 0.0;
	switch (edge)
	{
		case Edge::Top:
			dy = -1.0;
			break;
		case Edge::Bottom:
			dy = 1.0;
			break;
		case Edge::Left:
			dx = -1.0;
			break;
		case Edge::Right:
			dx = 1.0;
			break;
	}
}

/**
 * Returns the connection point on a given edge of a frame.
 *
 * With offset == 0, returns the edge midpoint (e.g. centre of the right edge).
 * With a non-zero offset, the point is shifted along the edge's tangent vector.
 * Offset is a fraction of the edge's half-length, so offset=0.15 on a 140pt
 * wide edge shifts by 0.15 * 70 = 10.5pt along the edge.
 *
 * @param g       The frame geometry containing pre-computed midpoints and tangents
 * @param edge    Which edge to get the point from
 * @param offset  Fractional shift along the edge (-1 to 1), 0 = dead centre
 */
FPoint FrameLinkRouter::edgeMidpoint(const FrameGeometry& g, Edge edge, double offset) const
{
	FPoint mid, tan;
	double halfLen = 0.0;
	switch (edge)
	{
		case Edge::Top:
			mid = g.topMid;
			tan = g.topTan;
			halfLen = g.topHalfLen;
			break;
		case Edge::Bottom:
			mid = g.bottomMid;
			tan = g.bottomTan;
			halfLen = g.bottomHalfLen;
			break;
		case Edge::Left:
			mid = g.leftMid;
			tan = g.leftTan;
			halfLen = g.leftHalfLen;
			break;
		case Edge::Right:
			mid = g.rightMid;
			tan = g.rightTan;
			halfLen = g.rightHalfLen;
			break;
	}
	if (fabs(offset) > 1e-6 && halfLen > 1e-6)
	{
		double shift = offset * halfLen;
		return FPoint(mid.x() + tan.x() * shift, mid.y() + tan.y() * shift);
	}
	return mid;
}


/**
 * Builds the complete geometry description for a single frame.
 *
 * Handles three cases:
 *   - Normal frames: uses xPos/yPos + width/height directly
 *   - Group children: applies the group transform to get canvas-space coords,
 *     including scale and flip corrections
 *   - Rotated frames: transforms all points through the item's rotation
 *
 * Computes:
 *   - Bounding box corners (x1,y1)-(x2,y2) in post-rotation canvas space
 *   - Centre point (xMid, yMid)
 *   - Midpoint of each edge (topMid, bottomMid, leftMid, rightMid)
 *   - Unit tangent vector along each edge (for shifting connection points)
 *   - Half-length of each edge (for computing the shift amount)
 *
 * Edge midpoints and tangents are computed from pre-rotation coordinates,
 * then transformed by the rotation. This preserves accuracy for rotated frames
 * where the axis-aligned bounding box doesn't match the visual frame edges.
 */
FrameLinkRouter::FrameGeometry FrameLinkRouter::computeFrameGeom(const PageItem* item) const
{
	FrameGeometry geo;

	// Start with the item's position and size in document coordinates
	double x1 = item->xPos();
	double y1 = item->yPos();
	double x2 = x1 + item->width();
	double y2 = y1 + item->height();

	// For items inside groups, apply the group's transform matrix
	// to get the actual canvas-space position and scale
	if (item->isGroupChild())
	{
		QTransform itemTrans = item->getTransform();
		QPointF itPos = itemTrans.map(QPointF(0, 0));
		x1 = itPos.x();
		y1 = itPos.y();
		double grScXi = 1.0;
		double grScYi = 1.0;
		getScaleFromMatrix(itemTrans, grScXi, grScYi);
		// Correct for flipped (mirrored) groups
		if (itemTrans.m11() < 0)
			x1 -= item->width() * grScXi;
		if (itemTrans.m22() < 0)
			y1 -= item->height() * grScYi;
		x2 = x1 + item->width() * grScXi;
		y2 = y1 + item->height() * grScYi;
	}

	// The pivot point is (x1, y1) — Scribus's xPos/yPos is the rotation
	// origin. Width and height define the unrotated frame size.
	// We compute the 4 corners by rotating offsets from the pivot.
	double ox = x1;
	double oy = y1;
	double w = x2 - x1;
	double h = y2 - y1;

	double rot = item->rotation();
	double rad = rot * M_PI / 180.0;
	double cosR = cos(rad);
	double sinR = sin(rad);

	// Compute rotated corners directly.
	// Each corner is an offset (dx, dy) from the pivot, rotated by rot:
	//   rotated_x = ox + dx*cos - dy*sin
	//   rotated_y = oy + dx*sin + dy*cos
	// TL offset is (0, 0), TR is (w, 0), BL is (0, h), BR is (w, h)
	double tlX = ox;
	double tlY = oy;
	double trX = ox + w * cosR;
	double trY = oy + w * sinR;
	double blX = ox - h * sinR;
	double blY = oy + h * cosR;
	double brX = ox + w * cosR - h * sinR;
	double brY = oy + w * sinR + h * cosR;

	// Axis-aligned bounding box from all 4 rotated corners.
	// For unrotated frames, this matches (tlX,tlY)-(brX,brY).
	// For rotated frames, the extremes may come from any corner.
	geo.x1 = qMin(qMin(tlX, trX), qMin(blX, brX));
	geo.y1 = qMin(qMin(tlY, trY), qMin(blY, brY));
	geo.x2 = qMax(qMax(tlX, trX), qMax(blX, brX));
	geo.y2 = qMax(qMax(tlY, trY), qMax(blY, brY));

	// Centre is the average of two opposite corners (rotation-invariant)
	geo.xMid = (tlX + brX) / 2;
	geo.yMid = (tlY + brY) / 2;

	// Store the rotated corner positions for point-in-rect tests
	geo.tlX = tlX; geo.tlY = tlY;
	geo.trX = trX; geo.trY = trY;
	geo.blX = blX; geo.blY = blY;
	geo.brX = brX; geo.brY = brY;

	// Derive edge midpoints by averaging the rotated corner pairs.
	geo.topMid    = FPoint((tlX + trX) / 2, (tlY + trY) / 2);
	geo.bottomMid = FPoint((blX + brX) / 2, (blY + brY) / 2);
	geo.leftMid   = FPoint((tlX + blX) / 2, (tlY + blY) / 2);
	geo.rightMid  = FPoint((trX + brX) / 2, (trY + brY) / 2);

	// Compute unit tangent vectors, half-lengths, and outward normals for each edge.
	// The tangent runs along the edge. The normal is perpendicular, pointing outward.
	auto makeTangent = [](double ax, double ay, double bx, double by,
			FPoint& tan, double& halfLen, FPoint& normal,
			double cx, double cy)
	{
		double tdx = bx - ax;
		double tdy = by - ay;
		double len = sqrt(tdx * tdx + tdy * tdy);
		halfLen = len * 0.5;
		if (len > 1e-6)
		{
			tan = FPoint(tdx / len, tdy / len);
			// Two possible perpendiculars: (-tdy, tdx) and (tdy, -tdx)
			// Choose the one pointing away from the frame centre
			double midX = (ax + bx) / 2;
			double midY = (ay + by) / 2;
			double n1x = -tdy / len;
			double n1y = tdx / len;
			// Dot product of normal candidate with (midpoint - centre) vector
			// Positive means it points outward
			double dot = n1x * (midX - cx) + n1y * (midY - cy);
			if (dot >= 0)
				normal = FPoint(n1x, n1y);
			else
				normal = FPoint(-n1x, -n1y);
		}
		else
		{
			tan = FPoint(1.0, 0.0);
			normal = FPoint(0.0, -1.0);
		}
	};

	double cx = geo.xMid;
	double cy = geo.yMid;
	makeTangent(tlX, tlY, trX, trY, geo.topTan, geo.topHalfLen, geo.topNormal, cx, cy);
	makeTangent(blX, blY, brX, brY, geo.bottomTan, geo.bottomHalfLen, geo.bottomNormal, cx, cy);
	makeTangent(tlX, tlY, blX, blY, geo.leftTan, geo.leftHalfLen, geo.leftNormal, cx, cy);
	makeTangent(trX, trY, brX, brY, geo.rightTan, geo.rightHalfLen, geo.rightNormal, cx, cy);
	return geo;
}


// ---------------------------------------------------------------------------
// Edge selection
// ---------------------------------------------------------------------------

/**
 * Returns the true outward normal for a given edge from the geometry.
 * For unrotated frames these match the nominal directions.
 * For rotated frames they follow the actual edge orientation.
 */
FPoint FrameLinkRouter::edgeNormal(const FrameGeometry& g, Edge edge)
{
	switch (edge)
	{
		case Edge::Top:    return g.topNormal;
		case Edge::Bottom: return g.bottomNormal;
		case Edge::Left:   return g.leftNormal;
		case Edge::Right:  return g.rightNormal;
	}
	return FPoint(1.0, 0.0);
}


/**
 * Tests if a point is inside a rotated rectangle defined by its 4 corners.
 *
 * Uses the cross-product winding method: a point is inside a convex polygon
 * if it is on the same side of every edge. The corners must be in order
 * (TL -> TR -> BR -> BL for clockwise winding).
 *
 * @param px, py  The point to test
 * @param g       The frame geometry with rotated corners
 * @param margin  Expansion margin (positive = larger rectangle)
 */
bool FrameLinkRouter::pointInRotatedRect(double px, double py, const FrameGeometry& g, double margin)
{
	// Expand corners outward by margin along the normals
	double tlx = g.tlX + (-g.topNormal.x() - g.leftNormal.x()) * margin * 0.5;
	double tly = g.tlY + (-g.topNormal.y() - g.leftNormal.y()) * margin * 0.5;
	double trx = g.trX + (-g.topNormal.x() + g.rightNormal.x()) * margin * 0.5;
	double try_ = g.trY + (-g.topNormal.y() + g.rightNormal.y()) * margin * 0.5;
	double brx = g.brX + (g.bottomNormal.x() + g.rightNormal.x()) * margin * 0.5;
	double bry = g.brY + (g.bottomNormal.y() + g.rightNormal.y()) * margin * 0.5;
	double blx = g.blX + (g.bottomNormal.x() - g.leftNormal.x()) * margin * 0.5;
	double bly = g.blY + (g.bottomNormal.y() - g.leftNormal.y()) * margin * 0.5;

	// Cross products for clockwise winding: TL->TR->BR->BL
	// Point is inside if all cross products have the same sign
	auto cross = [](double ax, double ay, double bx, double by, double px, double py) -> double
	{
		return (bx - ax) * (py - ay) - (by - ay) * (px - ax);
	};

	double c1 = cross(tlx, tly, trx, try_, px, py);
	double c2 = cross(trx, try_, brx, bry, px, py);
	double c3 = cross(brx, bry, blx, bly, px, py);
	double c4 = cross(blx, bly, tlx, tly, px, py);

	bool allNeg = (c1 <= 0 && c2 <= 0 && c3 <= 0 && c4 <= 0);
	bool allPos = (c1 >= 0 && c2 >= 0 && c3 >= 0 && c4 >= 0);
	return allNeg || allPos;
}


/**
 * Computes the projected gap between two frames along a direction vector.
 *
 * Projects all 8 corners (4 from each frame) onto the direction axis,
 * then measures the gap between the source's maximum extent and the
 * destination's minimum extent in that direction.
 *
 * Returns positive when there's a gap, negative when frames overlap.
 */
double FrameLinkRouter::projectedGap(const FrameGeometry& src, const FrameGeometry& dst, double dirX, double dirY)
{
	// Project source corners onto direction axis, find max
	double s1 = src.tlX * dirX + src.tlY * dirY;
	double s2 = src.trX * dirX + src.trY * dirY;
	double s3 = src.blX * dirX + src.blY * dirY;
	double s4 = src.brX * dirX + src.brY * dirY;
	double srcMax = qMax(qMax(s1, s2), qMax(s3, s4));

	// Project destination corners onto direction axis, find min
	double d1 = dst.tlX * dirX + dst.tlY * dirY;
	double d2 = dst.trX * dirX + dst.trY * dirY;
	double d3 = dst.blX * dirX + dst.blY * dirY;
	double d4 = dst.brX * dirX + dst.brY * dirY;
	double dstMin = qMin(qMin(d1, d2), qMin(d3, d4));

	return dstMin - srcMax;
}


/**
 * Determines which edge of the source frame should be used to exit toward
 * the destination frame.
 *
 * Uses the dot product of each edge's true outward normal with the direction
 * from source centre to destination centre. The edge whose normal most
 * closely faces the destination gets the highest score and is selected.
 *
 * This works correctly for rotated frames because it uses the actual
 * rotated edge normals rather than axis-aligned directions.
 */
FrameLinkRouter::Edge FrameLinkRouter::bestExitEdge(const FrameGeometry& src, const FrameGeometry& dst) const
{
	double dx = dst.xMid - src.xMid;
	double dy = dst.yMid - src.yMid;
	double len = sqrt(dx * dx + dy * dy);
	if (len < 1e-6)
		return Edge::Right;
	dx /= len;
	dy /= len;

	Edge candidates[] = { Edge::Top, Edge::Right, Edge::Bottom, Edge::Left };
	Edge bestEdge = Edge::Right;
	double bestScore = -999.0;

	for (Edge e : candidates)
	{
		FPoint n = edgeNormal(src, e);
		double score = n.x() * dx + n.y() * dy;

		if (score > bestScore)
		{
			bestScore = score;
			bestEdge = e;
		}
	}
	return bestEdge;
}


/**
 * Chooses the best entry edge for the destination frame, given the
 * source frame's already-chosen exit edge.
 *
 * The default is the symmetric choice from bestExitEdge(). This method
 * overrides that in two situations where a perpendicular entry produces
 * a cleaner L-bend route:
 *
 *   1. Corner overlap: frames overlap on both axes (corners clip each other).
 *   2. Near-corner diagonal: frames are very close on the exit axis
 *      (gap < m_exitPadding) with the destination's near perpendicular edge
 *      between the source's midpoint and far edge.
 */
FrameLinkRouter::Edge FrameLinkRouter::bestEntryEdge(const FrameGeometry& dst, const FrameGeometry& src, Edge srcExitEdge) const
{
	Edge facing = bestExitEdge(dst, src);

	if (isHorizontalEdge(facing) == isHorizontalEdge(srcExitEdge))
	{
		FPoint srcNorm = edgeNormal(src, srcExitEdge);
		double gap = projectedGap(src, dst, srcNorm.x(), srcNorm.y());

		double dstLeft   = qMin(qMin(dst.tlX, dst.trX), qMin(dst.blX, dst.brX));
		double dstRight  = qMax(qMax(dst.tlX, dst.trX), qMax(dst.blX, dst.brX));
		double dstTop    = qMin(qMin(dst.tlY, dst.trY), qMin(dst.blY, dst.brY));
		double dstBottom = qMax(qMax(dst.tlY, dst.trY), qMax(dst.blY, dst.brY));
		double srcLeft   = qMin(qMin(src.tlX, src.trX), qMin(src.blX, src.brX));
		double srcRight  = qMax(qMax(src.tlX, src.trX), qMax(src.blX, src.brX));
		double srcTop    = qMin(qMin(src.tlY, src.trY), qMin(src.blY, src.brY));
		double srcBottom = qMax(qMax(src.tlY, src.trY), qMax(src.blY, src.brY));

		bool xOverlap = srcRight > dstLeft && srcLeft < dstRight;
		bool yOverlap = srcBottom > dstTop && srcTop < dstBottom;

		// Case 1: corner overlap — frames actually clip each other
		if (xOverlap && yOverlap)
		{
			FPoint srcMid = edgeMidpoint(src, srcExitEdge);
			double exitX = srcMid.x() + srcNorm.x() * m_exitPadding;
			double exitY = srcMid.y() + srcNorm.y() * m_exitPadding;

			if (isHorizontalEdge(srcExitEdge))
			{
				if (exitY < dstTop)
					return Edge::Top;
				if (exitY > dstBottom)
					return Edge::Bottom;
			}
			else
			{
				if (exitX < dstLeft)
					return Edge::Left;
				if (exitX > dstRight)
					return Edge::Right;
			}
		}
		// Case 2: near-corner — gap is small enough that Z-route would be cramped
		else if (gap < m_exitPadding)
		{
			double srcMidY = (srcTop + srcBottom) / 2;
			double srcMidX = (srcLeft + srcRight) / 2;

			if (isHorizontalEdge(srcExitEdge))
			{
				if (dstTop > srcMidY && dstTop < srcBottom + m_exitPadding)
					return Edge::Top;
				if (dstBottom < srcMidY && dstBottom > srcTop - m_exitPadding)
					return Edge::Bottom;
			}
			else
			{
				if (dstLeft > srcMidX && dstLeft < srcRight + m_exitPadding)
					return Edge::Left;
				if (dstRight < srcMidX && dstRight > srcLeft - m_exitPadding)
					return Edge::Right;
			}
		}
	}

	return facing;
}



// ---------------------------------------------------------------------------
// Route generation
// ---------------------------------------------------------------------------

/**
 * Builds the sequence of orthogonal waypoints that form the route.
 *
 * The route always starts at startPt (on the source edge) and ends at endPt
 * (on the destination edge). Between them, intermediate waypoints create
 * axis-aligned segments that avoid overlapping frames.
 *
 * Route types (chosen based on edge orientations and frame positions):
 *
 *   SAME-AXIS EDGES (both horizontal or both vertical):
 *     - I-route (straight): When edges face each other and are nearly aligned.
 *       Example: src exits Right, dst enters Left, both at similar Y.
 *       Route: startPt -----> endPt
 *
 *     - Z-route: When edges face each other but are offset.
 *       Route goes: exit stub -> horizontal crossover -> entry stub.
 *       Stubs shrink adaptively when frames are close together.
 *       Example: src exits Right, dst enters Left, dst is lower.
 *       Route: startPt -> exitPt -> midX,exitY -> midX,entryY -> entryPt -> endPt
 *
 *     - U-route: When the destination is "behind" the source (on the wrong
 *       side), or when frames overlap on the exit axis. The line routes
 *       around the outside of both frames.
 *       Example: src exits Right but dst is to the left of src.
 *       Route: startPt -> exitPt -> exitX,routeY -> entryX,routeY -> entryPt -> endPt
 *
 *   PERPENDICULAR EDGES (one horizontal, one vertical):
 *     - L-bend: A single 90-degree turn. Two candidates are tested (bendA
 *       and bendB). Each is validated against rollback (going backwards)
 *       and frame-crossing (bend point inside a frame's bounding box).
 *       Example: src exits Right, dst enters Top.
 *       Route: startPt -> exitPt -> bend -> entryPt -> endPt
 *
 *     - Z-route fallback: If neither L-bend is valid, falls back to a
 *       two-turn route through the midpoint.
 */
QVector<FPoint> FrameLinkRouter::buildRoute(const FPoint& startPt, Edge startEdge, const FPoint& endPt, Edge endEdge, const FrameGeometry& srcGeom, const FrameGeometry& dstGeom) const
{
	QVector<FPoint> pts;
	pts.append(startPt);

	// Get the outward direction vectors for each edge.
	// For unrotated frames, these are axis-aligned (0,1), (1,0), etc.
	// For rotated frames, use the true outward normals so exit stubs
	// leave perpendicular to the actual frame edge.
	FPoint srcNorm = edgeNormal(srcGeom, startEdge);
	FPoint dstNorm = edgeNormal(dstGeom, endEdge);
	double sdx = srcNorm.x(), sdy = srcNorm.y();
	double edx = dstNorm.x(), edy = dstNorm.y();

	// Exit and entry stubs: points just outside each frame edge.
	// These ensure the line starts perpendicular to the frame edge
	// before turning, creating clean visual separation from the frame.
	FPoint exitPt(startPt.x() + sdx * m_exitPadding, startPt.y() + sdy * m_exitPadding);
	FPoint entryPt(endPt.x() + edx * m_exitPadding, endPt.y() + edy * m_exitPadding);

	bool startH = isHorizontalEdge(startEdge);
	bool endH = isHorizontalEdge(endEdge);

	// Bounding box of both frames combined (used for U-route clearance)
	double minX = qMin(qMin(srcGeom.x1, srcGeom.x2), qMin(dstGeom.x1, dstGeom.x2));
	double maxX = qMax(qMax(srcGeom.x1, srcGeom.x2), qMax(dstGeom.x1, dstGeom.x2));
	double minY = qMin(qMin(srcGeom.y1, srcGeom.y2), qMin(dstGeom.y1, dstGeom.y2));
	double maxY = qMax(qMax(srcGeom.y1, srcGeom.y2), qMax(dstGeom.y1, dstGeom.y2));

	// ===== SAME-AXIS EDGES (both horizontal or both vertical) =====
	if (startH == endH)
	{
		// Check if edges face each other (e.g. Right toward Left)
		bool facing = false;
		if (startH)
			facing = (sdx > 0 && edx < 0) || (sdx < 0 && edx > 0);
		else
			facing = (sdy > 0 && edy < 0) || (sdy < 0 && edy > 0);

		// Check if connection points are nearly aligned on the perpendicular axis
		bool nearAligned = false;
		if (startH)
			nearAligned = fabs(startPt.y() - endPt.y()) < m_alignTolerance;
		else
			nearAligned = fabs(startPt.x() - endPt.x()) < m_alignTolerance;

		// --- I-route: direct straight line when facing and aligned ---
		if (facing && nearAligned)
		{
			// No intermediate points needed; startPt and endPt are enough
		}
		// --- Horizontal same-axis: Z-route or U-route ---
		else if (startH)
		{
			double srcRight = qMax(srcGeom.x1, srcGeom.x2);
			double srcLeft = qMin(srcGeom.x1, srcGeom.x2);
			double dstLeft = qMin(dstGeom.x1, dstGeom.x2);
			double dstRight = qMax(dstGeom.x1, dstGeom.x2);

			// The frame edge we're exiting from, and the facing edge of dst
			double srcEdgeX = (sdx > 0) ? srcRight : srcLeft;
			double dstEdgeX = (sdx > 0) ? dstLeft : dstRight;

			// Gap is negative when frames overlap on the exit axis —
			// the Z-route crossover would be behind the exit point
			bool gapNegative = (sdx > 0) ? (dstEdgeX < srcEdgeX) : (dstEdgeX > srcEdgeX);

			// U-route needed when:
			// - Destination is entirely behind source (wrong side), OR
			// - Frames overlap on the exit axis (gap is negative)
			bool needU = (sdx > 0 && dstRight < srcRight) || (sdx < 0 && dstLeft > srcLeft) || gapNegative;

			if (needU)
			{
				// U-route: go out, then route above or below both frames,
				// then come back in. Pick whichever side (above/below) is closer.
				double midY = (exitPt.y() + entryPt.y()) * 0.5;
				double aboveY = minY - m_exitPadding;
				double belowY = maxY + m_exitPadding;
				double routeY = (fabs(midY - aboveY) <= fabs(midY - belowY)) ? aboveY : belowY;
				pts.append(exitPt);
				pts.append(FPoint(exitPt.x(), routeY));
				pts.append(FPoint(entryPt.x(), routeY));
				pts.append(entryPt);
			}
			else
			{
				// Z-route: exit stub, horizontal crossover at midpoint, entry stub.
				// Stubs shrink adaptively when frames are close (30% of gap each)
				// to prevent the stubs from overshooting the crossover line.
				double midX = (srcEdgeX + dstEdgeX) * 0.5;
				double stubLen = qMin(m_exitPadding, fabs(dstEdgeX - srcEdgeX) * 0.3);
				FPoint exitAdj(startPt.x() + sdx * stubLen, startPt.y() + sdy * stubLen);
				FPoint entryAdj(endPt.x() + edx * stubLen, endPt.y() + edy * stubLen);
				pts.append(exitAdj);
				pts.append(FPoint(midX, exitAdj.y()));
				pts.append(FPoint(midX, entryAdj.y()));
				pts.append(entryAdj);
			}
		}
		// --- Vertical same-axis: Z-route or U-route (same logic, Y axis) ---
		else
		{
			// Vertical same-axis: same logic as horizontal but on Y axis
			double srcBot = qMax(srcGeom.y1, srcGeom.y2);
			double srcTop = qMin(srcGeom.y1, srcGeom.y2);
			double dstTop = qMin(dstGeom.y1, dstGeom.y2);
			double dstBot = qMax(dstGeom.y1, dstGeom.y2);
			double srcEdgeY = (sdy > 0) ? srcBot : srcTop;
			double dstEdgeY = (sdy > 0) ? dstTop : dstBot;
			bool gapNegative = (sdy > 0) ? (dstEdgeY < srcEdgeY) : (dstEdgeY > srcEdgeY);
			bool needU = (sdy > 0 && dstBot < srcBot) || (sdy < 0 && dstTop > srcTop) || gapNegative;
			if (needU)
			{
				// U-route: go out to the left or right of both frames
				double midX = (exitPt.x() + entryPt.x()) * 0.5;
				double leftX  = minX - m_exitPadding;
				double rightX = maxX + m_exitPadding;
				double routeX = (fabs(midX - leftX) <= fabs(midX - rightX)) ? leftX : rightX;
				pts.append(exitPt);
				pts.append(FPoint(routeX, exitPt.y()));
				pts.append(FPoint(routeX, entryPt.y()));
				pts.append(entryPt);
			}
			else
			{
				// Z-route: vertical stubs with horizontal crossover between frame edges
				double midY = (srcEdgeY + dstEdgeY) * 0.5;
				double stubLen = qMin(m_exitPadding, fabs(dstEdgeY - srcEdgeY) * 0.3);
				FPoint exitAdj(startPt.x() + sdx * stubLen, startPt.y() + sdy * stubLen);
				FPoint entryAdj(endPt.x() + edx * stubLen, endPt.y() + edy * stubLen);
				pts.append(exitAdj);
				pts.append(FPoint(exitAdj.x(), midY));
				pts.append(FPoint(entryAdj.x(), midY));
				pts.append(entryAdj);
			}
		}
	}
	// ===== PERPENDICULAR EDGES (one horizontal, one vertical) =====
	else
	{
		// Two candidate L-bends exist for perpendicular edges:
		//   bendA: follow exit direction first, then turn toward entry
		//   bendB: follow entry direction first, then turn toward exit
		//
		// For example, if src exits Right and dst enters Top:
		//   bendA = (entryPt.x, exitPt.y)  -- go right first, then down
		//   bendB = (exitPt.x, entryPt.y)  -- go down first, then right
		FPoint bendA, bendB;
		if (startH)
		{
			bendA = FPoint(entryPt.x(), exitPt.y());
			bendB = FPoint(exitPt.x(),  entryPt.y());
		}
		else
		{
			bendA = FPoint(exitPt.x(),  entryPt.y());
			bendB = FPoint(entryPt.x(), exitPt.y());
		}

		// Validate each candidate bend point. A bend is rejected if:
		//   1. It would require the line to reverse direction (rollback)
		//      from either the exit or entry side
		//   2. The bend point falls inside either frame's bounding box
		//      (with a small margin), which would make the line cross a frame
		auto isGoodBend = [&](const FPoint& bend) -> bool
		{
			// Rollback check from exit side: bend must be reachable
			// by continuing in the exit direction (not going backwards)
			if (startH)
			{
				if (sdx > 0 && bend.x() < exitPt.x()) return false;
				if (sdx < 0 && bend.x() > exitPt.x()) return false;
			}
			else
			{
				if (sdy > 0 && bend.y() < exitPt.y()) return false;
				if (sdy < 0 && bend.y() > exitPt.y()) return false;
			}
			// Rollback check from entry side: bend must be reachable
			// by continuing in the entry direction
			if (isHorizontalEdge(endEdge))
			{
				if (edx > 0 && bend.x() < entryPt.x()) return false;
				if (edx < 0 && bend.x() > entryPt.x()) return false;
			}
			else
			{
				if (edy > 0 && bend.y() < entryPt.y()) return false;
				if (edy < 0 && bend.y() > entryPt.y()) return false;
			}
			// Frame-crossing check: bend must not be inside either frame
			double margin = 2.0;
			if (pointInRotatedRect(bend.x(), bend.y(), srcGeom, margin))
				return false;
			if (pointInRotatedRect(bend.x(), bend.y(), dstGeom, margin))
				return false;
			return true;
		};

		// Prefer bendA; use bendB if bendA fails
		bool aGood = isGoodBend(bendA);
		bool bGood = isGoodBend(bendB);

		if (aGood || bGood)
		{
			// L-route: single 90-degree bend
			FPoint bend = aGood ? bendA : bendB;
			pts.append(exitPt);
			// Skip the bend point if either segment would be too short
			// (this collapses near-zero segments for clean rendering)
			double d1 = fabs(bend.x()-exitPt.x()) + fabs(bend.y()-exitPt.y());
			double d2 = fabs(bend.x()-entryPt.x()) + fabs(bend.y()-entryPt.y());
			if (d1 > m_minSegmentLen && d2 > m_minSegmentLen)
				pts.append(bend);
			pts.append(entryPt);
		}
		else
		{
			// Z-route fallback: neither L-bend works, use two turns
			// through the midpoint between exit and entry stubs
			pts.append(exitPt);
			if (startH)
			{
				double midY = (exitPt.y() + entryPt.y()) * 0.5;
				pts.append(FPoint(exitPt.x(), midY));
				pts.append(FPoint(entryPt.x(), midY));
			}
			else
			{
				double midX = (exitPt.x() + entryPt.x()) * 0.5;
				pts.append(FPoint(midX, exitPt.y()));
				pts.append(FPoint(midX, entryPt.y()));
			}
			pts.append(entryPt);
		}
	}

	pts.append(endPt);
	return pts;
}


/**
 * Checks whether the source and destination frames are touching or nearly
 * touching on the exit axis.
 *
 * Uses projectedGap along the source edge's true outward normal to measure
 * the actual gap between frames, which works correctly for rotated frames.
 * Also checks perpendicular distance — if connection points are far apart
 * on the perpendicular axis, there's room for an L-bend even if the gap
 * on the exit axis is small.
 */
bool FrameLinkRouter::isTouching() const
{
	// Measure the true gap along the source edge's outward normal
	FPoint norm = edgeNormal(m_sourceGeometry, m_sourceEdge);
	double gap = projectedGap(m_sourceGeometry, m_destinationGeometry, norm.x(), norm.y());

	if (fabs(gap) >= m_exitPadding * 0.75)
		return false;

	// Check perpendicular distance using edge midpoints (without chain offsets)
	// so that chain offset shifting doesn't prevent the chevron
	FPoint srcMid = edgeMidpoint(m_sourceGeometry, m_sourceEdge, 0.0);
	FPoint dstMid = edgeMidpoint(m_destinationGeometry, m_destinationEdge, 0.0);
	double perpDist = isHorizontalEdge(m_sourceEdge) ? fabs(srcMid.y() - dstMid.y()) : fabs(srcMid.x() - dstMid.x());

	return perpDist <= m_exitPadding;
}

// Drawing: orthogonal path with rounded bends

/**
 * Draws an orthogonal polyline with rounded corners at each bend point.
 *
 * For each interior waypoint (not the first or last), the corner is replaced
 * with a cubic Bezier arc that approximates a quarter-circle of the given
 * radius. The m_kappa constant positions the Bezier control points so the
 * curve is within 0.027% of a true circular arc.
 *
 * The radius is automatically reduced if any segment is too short to
 * accommodate the full corner radius (each corner needs 'r' on both sides).
 * If the radius becomes sub-pixel, corners are drawn as sharp 90-degree turns.
 *
 * Collinear points (where the incoming and outgoing directions are the same)
 * are drawn as straight lines without rounding — this handles I-route
 * intermediate points gracefully.
 *
 * @param painter  The ScPainter to draw on
 * @param pts      Ordered waypoints (startPt, intermediates, endPt)
 * @param radius   Desired corner radius in canvas coordinates
 */
void FrameLinkRouter::drawRoundedOrthogonalPath(ScPainter* painter, const QVector<FPoint>& pts, double radius) const
{
	if (pts.size() < 2) return;

	// Clamp radius so it never exceeds half of any segment length.
	// Without this, the Bezier arcs from adjacent corners would overlap.
	double r = radius;
	for (int i = 0; i < pts.size() - 1; ++i)
	{
		double segLen = fabs(pts[i+1].x() - pts[i].x()) + fabs(pts[i+1].y() - pts[i].y());
		if (segLen * 0.5 < r)
			r = segLen * 0.5;
	}
	// If radius is sub-pixel, skip rounding entirely
	if (r < 0.5 / m_scale)
		r = 0.0;

	painter->newPath();
	painter->moveTo(pts[0].x(), pts[0].y());

	for (int i = 1; i < pts.size(); ++i)
	{
		if (i < pts.size() - 1 && r > 0.0)
		{
			const FPoint& prev = pts[i-1];
			const FPoint& curr = pts[i];
			const FPoint& next = pts[i+1];

			// Incoming and outgoing direction vectors at this bend
			double inDx = curr.x()-prev.x();
			double inDy = curr.y()-prev.y();
			double outDx = next.x()-curr.x();
			double outDy = next.y()-curr.y();
			double inLen = fabs(inDx)+fabs(inDy);
			double outLen = fabs(outDx)+fabs(outDy);

			// Degenerate segment: just draw a straight line through
			if (inLen < 1e-6 || outLen < 1e-6)
			{
				painter->lineTo(curr.x(), curr.y());
				continue;
			}

			// Unit direction vectors
			double inNx = inDx/inLen;
			double inNy = inDy/inLen;
			double outNx = outDx/outLen;
			double outNy = outDy/outLen;

			// Collinearity check: if directions are nearly parallel,
			// this is a straight segment (e.g. I-route intermediates),
			// not a bend — skip rounding
			double dot = inNx * outNx + inNy * outNy;
			if (fabs(dot) > 0.99)
			{
				painter->lineTo(curr.x(), curr.y());
				continue;
			}

			// Arc start: 'r' back from the bend along the incoming direction
			double ax = curr.x() - inNx * r;
			double ay = curr.y() - inNy * r;
			// Arc end: 'r' forward from the bend along the outgoing direction
			double bx = curr.x() + outNx * r;
			double by = curr.y() + outNy * r;

			// Draw the straight segment up to the arc start
			painter->lineTo(ax, ay);

			// Draw the quarter-circle arc using a cubic Bezier.
			// Control points are placed at kappa * r along each direction
			// from the arc endpoints, producing a near-perfect circular arc.
			painter->curveTo(FPoint(ax + inNx * r * m_kappa, ay + inNy * r * m_kappa), FPoint(bx - outNx * r * m_kappa, by - outNy * r * m_kappa), FPoint(bx, by));
		}
		else
			painter->lineTo(pts[i].x(), pts[i].y());
	}
	painter->strokePath();
}


// Arrowhead

/**
 * Draws a filled arrowhead at the end of the route.
 *
 * The arrow is defined as a small polygon in local coordinates, then
 * transformed to the tip position and rotated to point in the direction
 * of the last segment (from 'from' to 'tip').
 *
 * Arrow size is bounded so it remains readable at all zoom levels:
 *   - At normal zoom: proportional to canvas scale
 *   - When zoomed far in: capped at maximum size (2.0)
 *   - When zoomed far out: capped at minimum size (0.2)
 *
 * The arrow shape is:  a concave kite/chevron pointing right in local space,
 * with the tip at origin and the base indented slightly for a clean look.
 */
void FrameLinkRouter::drawArrowHead(ScPainter* painter, const FPoint& tip, const FPoint& from) const
{
	QTransform arrowTrans;
	arrowTrans.translate(tip.x(), tip.y());

	// Rotate to match the direction of the last route segment
	double angle = atan2(tip.y()-from.y(), tip.x()-from.x()) * (180.0/M_PI);
	arrowTrans.rotate(angle);

	// Scale with bounds: grows with zoom but clamped to [0.2, 2.0] range
	double sc = qBound(0.2, 0.5 * m_scale, 2.0) / m_scale;
	arrowTrans.scale(sc, sc);
	FPointArray arrow;
	arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
	arrow.addQuadPoint(-15,-5, -15,-5, -15,-5, -15,-5);
	arrow.addQuadPoint(  0, 0,   0, 0,   0, 0,   0, 0);
	arrow.addQuadPoint(-15, 5, -15, 5, -15, 5, -15, 5);
	arrow.addQuadPoint(-12, 0, -12, 0, -12, 0, -12, 0);
	arrow.map(arrowTrans);
	painter->setBrush(painter->pen());
	painter->setBrushOpacity(1.0);
	painter->setLineWidth(0);
	painter->setFillMode(ScPainter::Solid);
	painter->setupPolygon(&arrow);
	painter->fillPath();
}

/**
 * Draws a small filled triangle (chevron) on the shared edge between
 * touching frames, pointing in the text flow direction.
 *
 * The chevron is positioned at the midpoint between the start and end
 * connection points. Its tip points outward in the source edge's normal
 * direction (the flow direction), and its base is perpendicular to that.
 *
 * Size is bounded between 3pt and 10pt to remain visible but not
 * overwhelming at all zoom levels.
 */
void FrameLinkRouter::drawChevron(ScPainter* painter) const
{
	// Centre the chevron between the two connection points
	double mx = (m_startPoint.x() + m_endPoint.x()) / 2;
	double my = (m_startPoint.y() + m_endPoint.y()) / 2;

	// Flow direction: the true outward normal of the source edge
	// (follows rotation for rotated frames)
	FPoint norm = edgeNormal(m_sourceGeometry, m_sourceEdge);
	double dx = norm.x();
	double dy = norm.y();

	// Chevron size, bounded for all zoom levels
	double sz = qBound(3.0, 6.0 / m_scale, 10.0);

	// Triangle geometry: tip in flow direction, base perpendicular
	// dy/-dx gives the perpendicular direction for the base points
	double tipX = mx + dx * sz;
	double tipY = my + dy * sz;
	double baseX1 = mx - dy * sz * 0.5;
	double baseY1 = my + dx * sz * 0.5;
	double baseX2 = mx + dy * sz * 0.5;
	double baseY2 = my - dx * sz * 0.5;

	FPointArray chevron;
	chevron.addQuadPoint(tipX, tipY, tipX, tipY, tipX, tipY, tipX, tipY);
	chevron.addQuadPoint(baseX1, baseY1, baseX1, baseY1, baseX1, baseY1, baseX1, baseY1);
	chevron.addQuadPoint(baseX2, baseY2, baseX2, baseY2, baseX2, baseY2, baseX2, baseY2);
	chevron.addQuadPoint(tipX, tipY, tipX, tipY, tipX, tipY, tipX, tipY);

	painter->setBrush(QColor(Qt::black));
	painter->setBrushOpacity(1.0);
	painter->setLineWidth(0);
	painter->setFillMode(ScPainter::Solid);
	painter->setupPolygon(&chevron);
	painter->fillPath();
}
