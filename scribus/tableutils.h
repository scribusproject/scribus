/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef TABLEUTILS_H
#define TABLEUTILS_H

#include "tableborder.h"

class QPointF;
class TableCell;
class PageItem_Table;

/**
 * Table utility functions.
 */
namespace TableUtils
{

/**
 * Resolves the collapsed borders between six cells in a 3x2 area.
 *
 * Given the six cells @a topLeftCell, @a topCell, @a topRightCell, @a bottomLeftCell,
 * @a bottomCell and @a bottomRightCell in the following picture
 *
 * <pre>
 *  +--------------------------+--------------------------+--------------------------+
 *  |                          |                          |                          |
 *  |                          |                          |                          |
 *  |        topLeftCell   topLeft        topCell      topRight   topRightCell       |
 *  |                          |                          |                          |
 *  |                          |                          |                          |
 *  +----------left------------+--------- center ---------+----------right-----------+
 *  |                          |                          |                          |
 *  |                          |                          |                          |
 *  |     bottomLeftCell   bottomLeft   bottomCell   bottomRight  bottomRightCell    |
 *  |                          |                          |                          |
 *  |                          |                          |                          |
 *  +--------------------------+--------------------------+--------------------------+
 * </pre>
 *
 * the function will return the collapsed borders @a topLeft, @a left, @a bottomLeft,
 * @a center, @a topRight, @a right and @a bottomRight. If two adjacent cells are invalid,
 * the returned collapsed border between them is null. If only one of two adjacent cells is
 * valid, the returned collapsed border will be the border of the valid cell collapsed with
 * the appropriate table border instead. If two "adjacent" cells are actually the same
 * cell, the returned collapsed border between them is null.
 *
 * If both @a topCell and @a bottomCell are invalid, the function will print a
 * warning message and return immediately, leaving @a topLeft, @a top, @a bottomLeft,
 * @a center, @a topRight, @a right and @a bottomRight unchanged.
 *
 * @a topLeft, @a left , @a bottomLeft, @a center, @a topRight, @a right and
 * @a bottomRight must point to existing borders.
 */
void resolveBordersHorizontal(const TableCell& topLeftCell, const TableCell& topCell,
	const TableCell& topRightCell, const TableCell& bottomLeftCell, const TableCell& bottomCell,
	const TableCell& bottomRightCell, TableBorder* topLeft, TableBorder* left, TableBorder* bottomLeft,
	TableBorder* center, TableBorder* topRight, TableBorder* right, TableBorder* bottomRight, PageItem_Table* table);

/**
 * Resolves the collapsed borders between six cells in a 2x3 area.
 *
 * Given the six cells @a topLeftCell, @a topRightCell, @a leftCell, @a rightCell,
 * @a bottomLeftCell and @a bottomRightCell in the following picture
 *
 * <pre>
 *  +----------------------+----------------------+
 *  |                      |                      |
 *  |                      |                      |
 *  |    topLeftCell      top     topRightCell    |
 *  |                      |                      |
 *  |                      |                      |
 *  +--------topLeft-------+-------topRight-------+
 *  |                      |                      |
 *  |                      |                      |
 *  |      leftCell     center     rightCell      |
 *  |                      |                      |
 *  |                      |                      |
 *  +-------bottomLeft-----+------bottomRight-----+
 *  |                      |                      |
 *  |                      |                      |
 *  |  bottomLeftCell   bottom   bottomRightCell  |
 *  |                      |                      |
 *  |                      |                      |
 *  +----------------------+----------------------+
 * </pre>
 *
 * the function will return the collapsed borders @a topLeft, @a top, @a topRight,
 * @a center, @a bottomLeft, @a bottom and @a bottomRight. If two adjacent cells are
 * invalid, the returned collapsed border between them is null. If only one of two adjacent
 * cells is valid, the returned collapsed border will be the border of the valid cell
 * collapsed with the appropriate table border instead. If two "adjacent" cells are
 * actually the same cell, the returned collapsed border between them is null.
 *
 * If both @a leftCell and @a rightCell are invalid, the function will print a
 * warning message and return immediately, leaving @a topLeft, @a top, @a topRight,
 * @a center, @a bottomLeft, @a bottom and @a bottomRight unchanged.
 *
 * @a topLeft, @a top, @a topRight, @a center, @a bottomLeft, @a bottom and
 * @a bottomRight must point to existing borders.
 */
void resolveBordersVertical(const TableCell& topLeftCell, const TableCell& topRightCell,
	const TableCell& leftCell, const TableCell& rightCell, const TableCell& bottomLeftCell,
	const TableCell& bottomRightCell, TableBorder* topLeft, TableBorder* top, TableBorder* topRight,
	TableBorder* center, TableBorder* bottomLeft, TableBorder* bottom, TableBorder* bottomRight, PageItem_Table* table);

/**
 * Collapses @a firstBorder with @a secondBorder and returns the collapsed border.
 *
 * Rules in order of priority:
 *
 * 1) If both borders are null borders, a null border is returned.
 * 2) If one of the two borders is null, the non-null border is returned.
 * 3) If one of the two borders is wider than the other, the wider border is returned.
 * 4) If @a firstBorder has more border lines than @a secondBorder, @a firstBorder is returned,
 *    else, @a secondBorder is returned.
 */
TableBorder collapseBorders(const TableBorder& firstBorder, const TableBorder& secondBorder);

/**
 * Joins the vertical border @a border with neighboring borders.
 *
 * Adjustments are made to the passed @a start and @a end points and offset factors for the
 * individual border lines are returned in @a startOffsetFactors and @a endOffsetFactors.
 *
 * If any of @a topLeft, @a top, @a topRight, @a bottomLeft, @a bottom or @a bottomRight
 * is null (isNull() returning <code>true</code>), then it is assumed that there is no border
 * coming in from that direction.
 *
 * Which neighboring borders the arguments refer to is illustrated by:
 *
 * <pre>
 *                 top
 *                  |
 *       topLeft--(start)--topRight
 *                  |
 *                border
 *                  |
 *    bottomLeft--(end)--bottomRight
 *                  |
 *                bottom
 * </pre>
 *
 * @param border the border that should be joined.
 * @param topLeft horizontal border meeting the start point of the border from the left.
 * @param top vertical border meeting the start point of the border from above.
 * @param topRight horizontal border meeting the start point of the border from the right.
 * @param bottomLeft horizontal border meeting the end point of the border from the left.
 * @param bottom vertical border meeting the end point of the border from above.
 * @param bottomRight horizontal border meeting the end point of the border from the right.
 * @param start start point of @a border, assumed to be above @a end.
 * @param end end point of @a border, assumed to be below @a start.
 * @param startOffsetFactors start offset factors for the individual border lines. The start
 * 		point of each border line of the border should be adjusted by its
 * 		own width multiplied by these factors.
 * @param endOffsetFactors end offset factors for the individual border lines. The end
 * 		point of each border line of the border should be adjusted by its
 * 		own width multiplied by these factors.
 */
void joinVertical(const TableBorder& border, const TableBorder& topLeft, const TableBorder& top,
				  const TableBorder& topRight, const TableBorder& bottomLeft, const TableBorder& bottom,
				  const TableBorder& bottomRight, QPointF* start, QPointF* end, QPointF* startOffsetFactors,
				  QPointF* endOffsetFactors);

/**
 * Joins the horizontal border @a border with neighboring borders.
 *
 * Adjustments are made to the passed @a start and @a end points and offset factors for the
 * individual border lines are returned in @a startOffsetFactors and @a endOffsetFactors.
 *
 * If any of @a topLeft, @a left, @a bottomLeft, @a topRight, @a right or @a bottomRight
 * is null (isNull() returning <code>true</code>), then it is assumed that there is no border
 * coming in from that direction.
 *
 * Which neighboring borders the arguments refer to is illustrated by:
 *
 * <pre>
 *          topLeft              topRight
 *             |                     |
 *   left--(m_start)---m_border---(m_end)--right
 *             |                     |
 *         bottomLeft           bottomRight
 * </pre>
 *
 * @param topLeft vertical border meeting the start point of the border from the top.
 * @param left horizontal border meeting the start point of @a border from the left.
 * @param bottomLeft vertical border meeting the start point of the border from the bottom.
 * @param topRight vertical border meeting the end point of the border from the top.
 * @param right horizontal border meeting the end point of the border from the right.
 * @param bottomRight vertical border meeting the end point of the border from the bottom.
 * @param start start point of @a border, assumed to be above @a end.
 * @param end end point of @a border, assumed to be below @a start.
 * @param startOffsetFactors start offset factors for the individual border lines. The start
 * 		point of each border line of the border should be adjusted by its
 * 		own width multiplied by these factors.
 * @param endOffsetFactors end offset factors for the individual border lines. The end
 * 		point of each border line of the border should be adjusted by its
 * 		own width multiplied by these factors.
 */
void joinHorizontal(const TableBorder& border, const TableBorder& topLeft, const TableBorder& left,
				  const TableBorder& bottomLeft, const TableBorder& topRight, const TableBorder& right,
				  const TableBorder& bottomRight, QPointF* start, QPointF* end, QPointF* startOffsetFactors,
				  QPointF* endOffsetFactors);

} // namespace TableUtils

#endif // TABLEUTILS_H
