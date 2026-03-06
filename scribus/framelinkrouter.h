/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef FRAMELINKROUTER_H
#define FRAMELINKROUTER_H

#include <math.h>
#include <QVector>

#include "fpoint.h"
class PageItem;
class ScPainter;


/**
 * @brief Routes and draws orthogonal connector lines between linked text frames.
 *
 * Inspired by the elbow-arrow routing approach used in Excalidraw. Given a source
 * and destination frame, the router:
 *   1. Computes each frame's geometry (bounding box, edge midpoints, tangent vectors),
 *      accounting for rotation and group transforms.
 *   2. Picks the best exit/entry edges based on relative frame positions and overlap.
 *   3. Builds an orthogonal route (sequence of axis-aligned waypoints) using one of
 *      several strategies: straight line, Z-route, U-route, or L-bend.
 *   4. Draws the route with rounded corners and a directional arrowhead.
 *
 * For frames that are touching or very close, a small chevron marker is drawn
 * instead of a full routed line.
 *
 * Usage:
 *   FrameLinkRouter router(srcItem, dstItem, viewScale);
 *   router.setSourceIsAlsoEntry(hasPrev);   // mid-chain offset hint
 *   router.setDestinationIsAlsoExit(hasNext);
 *   router.route();
 *   router.draw(painter);
 */
class FrameLinkRouter
{
public:
	/// Which edge of a frame the connector exits from or enters into.
	enum class Edge
	{
		Top,
		Right,
		Bottom,
		Left
	};

	/**
	 * @brief Pre-computed geometry for one frame.
	 *
	 * All coordinates are in canvas (document) space, with rotation applied.
	 * The AABB (x1,y1)-(x2,y2) is the axis-aligned bounding box of the
	 * rotated frame (used for broad-phase checks).
	 * The 4 rotated corners (tl,tr,bl,br) define the actual frame shape.
	 * Edge normals point outward from the frame surface.
	 */
	struct FrameGeometry
	{
		// Axis-aligned bounding box (min/max of all 4 rotated corners)
		double x1 {0.0};          ///< AABB min x
		double y1 {0.0};          ///< AABB min y
		double x2 {0.0};          ///< AABB max x
		double y2 {0.0};          ///< AABB max y
		double xMid {0.0};        ///< Centre x
		double yMid {0.0};        ///< Centre y

		// Rotated corner positions
		double tlX {0.0}, tlY {0.0};  ///< Top-left corner (post-rotation)
		double trX {0.0}, trY {0.0};  ///< Top-right corner (post-rotation)
		double blX {0.0}, blY {0.0};  ///< Bottom-left corner (post-rotation)
		double brX {0.0}, brY {0.0};  ///< Bottom-right corner (post-rotation)

		// Edge half-lengths (for chain offset computation)
		double topHalfLen {0.0};
		double bottomHalfLen {0.0};
		double leftHalfLen {0.0};
		double rightHalfLen {0.0};

		// Edge midpoints (post-rotation)
		FPoint topMid, bottomMid, leftMid, rightMid;

		// Unit tangent vectors along each edge
		FPoint topTan, bottomTan, leftTan, rightTan;

		// True outward normal vectors for each edge (post-rotation).
		// For unrotated frames: top=(0,-1), bottom=(0,1), left=(-1,0), right=(1,0)
		// For rotated frames: perpendicular to the actual edge direction.
		FPoint topNormal, bottomNormal, leftNormal, rightNormal;
	};

	/**
	 * @brief Construct a router for one source->destination link.
	 * @param src    The source PageItem (link origin)
	 * @param dst    The destination PageItem (link target)
	 * @param scale  The current canvas zoom scale (m_viewMode.scale)
	 */
	FrameLinkRouter(PageItem* src, PageItem* dst, double scale);

	/// Indicate that the source frame also receives an incoming link (is mid-chain).
	/// When true, the connection point may be offset along the edge to avoid overlap.
	void setSourceIsAlsoEntry(bool b) { m_srcIsAlsoEntry = b; }

	/// Indicate that the destination frame also sends an outgoing link (is mid-chain).
	void setDestinationIsAlsoExit(bool b) { m_dstIsAlsoExit = b; }

	/// Compute frame geometries, pick edges, and build the route waypoints.
	void route();

	/// Draw the routed line (or chevron if touching) onto the painter.
	void draw(ScPainter* painter) const;

private:
	// Bezier constant for approximating a quarter-circle arc with a cubic curve.
	// Derived from (4/3) * tan(PI/8). Using this as the control point distance
	// from the arc endpoints produces a curve within 0.027% of a true circle.
	static constexpr double m_kappa = 0.5522847498;

	// --- Input state ---
	PageItem* m_srcItem { nullptr };  ///< Source frame
	PageItem* m_dstItem { nullptr };  ///< Destination frame
	double m_scale {0.0};             ///< Canvas zoom factor

	/// True if source frame also receives a link (from prevInChain)
	bool m_srcIsAlsoEntry = false;
	/// True if destination frame also sends a link (to nextInChain)
	bool m_dstIsAlsoExit  = false;

	// --- Tuning constants ---
	double m_cornerRadius { 6.0 };     ///< Radius for rounded bends (document points)
	double m_exitPadding { 12.0 };     ///< How far the line extends from the frame edge before turning
	double m_minSegmentLen { 4.0 };    ///< Minimum segment length; shorter segments are collapsed
	double m_alignTolerance { 5.0 };   ///< Max offset (pts) to still treat frames as aligned
	double m_chainOffset { 0.15 };     ///< Fraction of edge length to shift mid-chain connection points

	// --- Computed state (populated by route()) ---
	FrameGeometry m_sourceGeometry;       ///< Source frame geometry
	FrameGeometry m_destinationGeometry;  ///< Destination frame geometry
	Edge m_sourceEdge;                    ///< Which edge the line exits the source from
	Edge m_destinationEdge;               ///< Which edge the line enters the destination at
	FPoint m_startPoint;                  ///< Connection point on source edge
	FPoint m_endPoint;                    ///< Connection point on destination edge
	QVector<FPoint> m_route;              ///< Ordered waypoints from start to end

	// --- Geometry computation ---

	/// Build FrameGeometry for a PageItem, handling group transforms and rotation.
	FrameGeometry computeFrameGeom(const PageItem* item) const;

	/// Choose the best edge of 'src' to exit from when connecting toward 'dst'.
	/// Uses dot-product of true rotated edge normals with direction to destination.
	Edge bestExitEdge(const FrameGeometry& src, const FrameGeometry& dst) const;

	/// Choose the best entry edge for the destination, given the source's exit edge.
	/// May select a perpendicular edge for L-bend routing when frames overlap
	/// or are very close on the exit axis.
	Edge bestEntryEdge(const FrameGeometry& dst, const FrameGeometry& src, Edge srcExitEdge) const;

	/// True if the edge is Left or Right (horizontal exit direction for unrotated frames).
	static bool isHorizontalEdge(Edge e);

	/// Get the nominal outward direction for an edge as (dx, dy).
	/// Returns axis-aligned values: Top=(0,-1), Right=(1,0), etc.
	/// For rotation-aware normals, use FrameGeometry's normal vectors.
	static void edgeDirection(Edge edge, double& dx, double& dy);

	/// Get the true outward normal for a given edge from the geometry.
	static FPoint edgeNormal(const FrameGeometry& g, Edge edge);

	/// Get the connection point on an edge, optionally shifted along the edge
	/// by 'offset' (fraction of half-edge-length) for chain separation.
	FPoint edgeMidpoint(const FrameGeometry& g, Edge edge, double offset = 0.0) const;

	/// Compute m_startPoint and m_endPoint, applying chain offsets when
	/// a mid-chain frame uses the same edge for both incoming and outgoing links.
	void computeConnectionPoints();

	// --- Rotated rectangle helpers ---

	/// Test if a point is inside a rotated rectangle defined by its 4 corners,
	/// with an optional margin expansion.
	static bool pointInRotatedRect(double px, double py, const FrameGeometry& g, double margin = 0.0);

	/// Compute the gap between two frames along a given direction vector.
	/// Returns the signed distance; negative means frames overlap in that direction.
	static double projectedGap(const FrameGeometry& src, const FrameGeometry& dst, double dirX, double dirY);

	// --- Route building ---

	/// Build the sequence of orthogonal waypoints connecting startPt to endPt.
	/// Chooses between: straight line, Z-route, U-route, or L-bend based on
	/// edge orientations and frame positions.
	QVector<FPoint> buildRoute(const FPoint& startPt, Edge startEdge, const FPoint& endPt, Edge endEdge, const FrameGeometry& srcGeom, const FrameGeometry& dstGeom) const;


	/// Check if the source and destination frames are touching or overlapping
	/// on the exit axis (gap less than 75% of exit padding).
	bool isTouching() const;

	// --- Drawing ---

	/// Draw the route as a polyline with rounded corners at each bend.
	/// Uses cubic Bezier arcs (via m_kappa) to approximate quarter-circle bends.
	void drawRoundedOrthogonalPath(ScPainter* painter, const QVector<FPoint>& pts, double radius) const;

	/// Draw a filled arrowhead at 'tip', pointing in the direction from 'from' to 'tip'.
	/// Arrow size is bounded to stay readable at all zoom levels.
	void drawArrowHead(ScPainter* painter, const FPoint& tip, const FPoint& from) const;

	/// Draw a small filled chevron (triangle) on the shared edge between touching
	/// frames, pointing in the text flow direction. Used instead of a full routed line.
	void drawChevron(ScPainter* painter) const;
};

#endif // FRAMELINKROUTER_H
