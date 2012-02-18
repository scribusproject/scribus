/*
Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QPointF>

#include "tableborder.h"

#include "tableutils.h"
#include "pageitem_table.h"

namespace TableUtils
{

void resolveBordersHorizontal(const TableCell& topLeftCell, const TableCell& topCell,
	const TableCell& topRightCell, const TableCell& bottomLeftCell, const TableCell& bottomCell,
	const TableCell& bottomRightCell, TableBorder* topLeft, TableBorder* left, TableBorder* bottomLeft,
	TableBorder* center, TableBorder* topRight, TableBorder* right, TableBorder* bottomRight, PageItem_Table* table)
{
	// Resolve top left.
	if (!topCell.isValid() && !bottomCell.isValid())
		return;
	if (topLeftCell.column() == topCell.column())
		*topLeft = TableBorder();
	else if (topLeftCell.isValid() && topCell.isValid())
		*topLeft = collapseBorders(topCell.leftBorder(), topLeftCell.rightBorder());
	else if (topLeftCell.isValid())
		*topLeft = collapseBorders(table->rightBorder(), topLeftCell.rightBorder());
	else if (topCell.isValid())
		*topLeft = collapseBorders(topCell.leftBorder(), table->leftBorder());
	else
		*topLeft = TableBorder();
	// Resolve left.
	if (topLeftCell.row() == bottomLeftCell.row())
		*left = TableBorder();
	else if (topLeftCell.isValid() && bottomLeftCell.isValid())
		*left = collapseBorders(bottomLeftCell.topBorder(), topLeftCell.bottomBorder());
	else if (topLeftCell.isValid())
		*left = collapseBorders(table->bottomBorder(), topLeftCell.bottomBorder());
	else if (bottomLeftCell.isValid())
		*left = collapseBorders(bottomLeftCell.topBorder(), table->topBorder());
	else
		*left = TableBorder();
	// Resolve bottom left.
	if (bottomLeftCell.column() == bottomCell.column())
		*bottomLeft = TableBorder();
	else if (bottomLeftCell.isValid() && bottomCell.isValid())
		*bottomLeft = collapseBorders(bottomCell.leftBorder(), bottomLeftCell.rightBorder());
	else if (bottomLeftCell.isValid())
		*bottomLeft = collapseBorders(table->rightBorder(), bottomLeftCell.rightBorder());
	else if (bottomCell.isValid())
		*bottomLeft = collapseBorders(bottomCell.leftBorder(), table->leftBorder());
	else
		*bottomLeft = TableBorder();
	// Resolve center.
	if (topCell.row() == bottomCell.row())
		*center = TableBorder();
	else if (topCell.isValid() && bottomCell.isValid())
		*center = collapseBorders(topCell.bottomBorder(), bottomCell.topBorder());
	else if (topCell.isValid())
		*center = collapseBorders(table->bottomBorder(), topCell.bottomBorder());
	else if (bottomCell.isValid())
		*center = collapseBorders(bottomCell.topBorder(), table->topBorder());
	else
		*center = TableBorder();
	// Resolve top right.
	if (topRightCell.column() == topCell.column())
		*topRight = TableBorder();
	else if (topRightCell.isValid() && topCell.isValid())
		*topRight = collapseBorders(topRightCell.leftBorder(), topCell.rightBorder());
	else if (topRightCell.isValid())
		*topRight = collapseBorders(topRightCell.leftBorder(), table->leftBorder());
	else if (topCell.isValid())
		*topRight = collapseBorders(table->rightBorder(), topCell.rightBorder());
	else
		*topRight = TableBorder();
	// Resolve right.
	if (topRightCell.row() == bottomRightCell.row())
		*right = TableBorder();
	else if (topRightCell.isValid() && bottomRightCell.isValid())
		*right = collapseBorders(bottomRightCell.topBorder(), topRightCell.bottomBorder());
	else if (topRightCell.isValid())
		*right = collapseBorders(table->bottomBorder(), topRightCell.bottomBorder());
	else if (bottomRightCell.isValid())
		*right = collapseBorders(bottomRightCell.topBorder(), table->topBorder());
	else
		*right = TableBorder();
	// Resolve bottom right.
	if (bottomRightCell.column() == bottomCell.column())
		*bottomRight = TableBorder();
	else if (bottomRightCell.isValid() && bottomCell.isValid())
		*bottomRight = collapseBorders(bottomRightCell.leftBorder(), bottomCell.rightBorder());
	else if (bottomRightCell.isValid())
		*bottomRight = collapseBorders(bottomRightCell.leftBorder(), table->leftBorder());
	else if (bottomCell.isValid())
		*bottomRight = collapseBorders(table->rightBorder(), bottomCell.rightBorder());
	else
		*bottomRight = TableBorder();
}

void resolveBordersVertical(const TableCell& topLeftCell, const TableCell& topRightCell, const TableCell& leftCell, const TableCell& rightCell, const TableCell& bottomLeftCell,
	const TableCell& bottomRightCell, TableBorder* topLeft, TableBorder* top, TableBorder* topRight, TableBorder* center, TableBorder* bottomLeft, TableBorder* bottom, TableBorder* bottomRight, PageItem_Table* table)
{
	if (!leftCell.isValid() && !rightCell.isValid())
		return;
	// Resolve top left.
	if (topLeftCell.row() == leftCell.row())
		*topLeft = TableBorder();
	else if (topLeftCell.isValid() && leftCell.isValid())
		*topLeft = collapseBorders(leftCell.topBorder(), topLeftCell.bottomBorder());
	else if (topLeftCell.isValid())
		*topLeft = collapseBorders(table->bottomBorder(), topLeftCell.bottomBorder());
	else if (leftCell.isValid())
		*topLeft = collapseBorders(leftCell.topBorder(), table->topBorder());
	else
		*topLeft = TableBorder();
	// Resolve top.
	if (topLeftCell.column() == topRightCell.column())
		*top = TableBorder();
	else if (topLeftCell.isValid() && topRightCell.isValid())
		*top = collapseBorders(topRightCell.leftBorder(), topLeftCell.rightBorder());
	else if (topLeftCell.isValid())
		*top = collapseBorders(table->rightBorder(), topLeftCell.rightBorder());
	else if (topRightCell.isValid())
		*top = collapseBorders(topRightCell.leftBorder(), table->leftBorder());
	else
		*top = TableBorder();
	// Resolve top right.
	if (topRightCell.row() == rightCell.row())
		*topRight = TableBorder();
	else if (topRightCell.isValid() && rightCell.isValid())
		*topRight = collapseBorders(rightCell.topBorder(), topRightCell.bottomBorder());
	else if (topRightCell.isValid())
		*topRight = collapseBorders(table->bottomBorder(), topRightCell.bottomBorder());
	else if (rightCell.isValid())
		*topRight = collapseBorders(rightCell.topBorder(), table->topBorder());
	else
		*topRight = TableBorder();
	// Resolve center.
	if (leftCell.column() == rightCell.column())
		*center = TableBorder();
	else if (leftCell.isValid() && rightCell.isValid())
		*center = collapseBorders(rightCell.leftBorder(), leftCell.rightBorder());
	else if (leftCell.isValid())
		*center = collapseBorders(table->rightBorder(), leftCell.rightBorder());
	else if (rightCell.isValid())
		*center = collapseBorders(rightCell.leftBorder(), table->leftBorder());
	else
		*center = TableBorder();
	// Resolve bottom left.
	if (bottomLeftCell.row() == leftCell.row())
		*bottomLeft = TableBorder();
	else if (bottomLeftCell.isValid() && leftCell.isValid())
		*bottomLeft = collapseBorders(bottomLeftCell.topBorder(), leftCell.bottomBorder());
	else if (bottomLeftCell.isValid())
		*bottomLeft = collapseBorders(bottomLeftCell.topBorder(), table->topBorder());
	else if (leftCell.isValid())
		*bottomLeft = collapseBorders(table->bottomBorder(), leftCell.bottomBorder());
	else
		*bottomLeft = TableBorder();
	// Resolve bottom.
	if (bottomLeftCell.column() == bottomRightCell.column())
		*bottom = TableBorder();
	else if (bottomLeftCell.isValid() && bottomRightCell.isValid())
		*bottom = collapseBorders(bottomRightCell.leftBorder(), bottomLeftCell.rightBorder());
	else if (bottomLeftCell.isValid())
		*bottom = collapseBorders(table->rightBorder(), bottomLeftCell.rightBorder());
	else if (bottomRightCell.isValid())
		*bottom = collapseBorders(bottomRightCell.leftBorder(), table->leftBorder());
	else
		*bottom = TableBorder();
	// Resolve bottom right.
	if (bottomRightCell.row() == rightCell.row())
		*bottomRight = TableBorder();
	else if (bottomRightCell.isValid() && rightCell.isValid())
		*bottomRight = collapseBorders(bottomRightCell.topBorder(), rightCell.bottomBorder());
	else if (bottomRightCell.isValid())
		*bottomRight = collapseBorders(bottomRightCell.topBorder(), table->topBorder());
	else if (rightCell.isValid())
		*bottomRight = collapseBorders(table->bottomBorder(), rightCell.bottomBorder());
	else
		*bottomRight = TableBorder();
}

TableBorder collapseBorders(const TableBorder& firstBorder, const TableBorder& secondBorder)
{
	TableBorder collapsedBorder;

	if (firstBorder.isNull() && secondBorder.isNull())
	{
		// Both borders are null, so return a null border.
		return collapsedBorder;
	}
	else if (firstBorder.isNull())
	{
		// First border is null, so return second border.
		collapsedBorder = secondBorder;
	}
	else if (secondBorder.isNull())
	{
		// Second border is null, so return first border.
		collapsedBorder = firstBorder;
	}
	else
	{
		if (firstBorder.width() > secondBorder.width())
		{
			// First border is wider than second border, so return first border.
			collapsedBorder = firstBorder; // (4)
		}
		else if (firstBorder.width() < secondBorder.width())
		{
			// Second border is wider than first border, so return second border.
			collapsedBorder = secondBorder; // (5)
		}
		else
		{
			if (firstBorder.borderLines().size() > secondBorder.borderLines().size())
			{
				// First border has more border lines than second border, so return first border.
				collapsedBorder = firstBorder;
			}
			else
			{
				// Second border has more or equal border lines than first border, so return second border.
				collapsedBorder = secondBorder;
			}
		}
	}

	return collapsedBorder;
}

void joinVertical(const TableBorder& border, const TableBorder& topLeft, const TableBorder& top,
				  const TableBorder& topRight, const TableBorder& bottomLeft, const TableBorder& bottom,
				  const TableBorder& bottomRight, QPointF* start, QPointF* end, QPointF* startOffsetFactors,
				  QPointF* endOffsetFactors)
{
	Q_ASSERT(start);
	Q_ASSERT(end);
	Q_ASSERT(startOffsetFactors);
	Q_ASSERT(endOffsetFactors);

	// Reset offset coefficients.
	startOffsetFactors->setX(0.0);
	startOffsetFactors->setY(0.0);
	endOffsetFactors->setX(0.0);
	endOffsetFactors->setY(0.0);

	/*
	 * The numbered cases in the code below refers to the 45 possible join cases illustrated
	 * in the picture at http://wiki.scribus.net/canvas/File:Table_border_join_cases.png
	 */

	/*
	 * Adjust start point(s). Possible cases are 1-20, 26-39.
	 */
	if (border.joinsWith(topLeft))
	{
		if (border.joinsWith(topRight))
		{
			if (!border.joinsWith(top))
			{
				// Cases: 8, 19.
				startOffsetFactors->setY(-0.5);
			}
		}
		else if (!border.joinsWith(top))
		{
			if (top.joinsWith(topRight))
			{
				if (border.width() < top.width())
				{
					// Cases: 15A.
					start->setY(start->y() + 0.5 * top.width());
				}
				else
				{
					// Cases: 15B.
					startOffsetFactors->setY(-0.5);
				}
			}
			else
			{
				// Cases: 5, 17, 27, 38.
				startOffsetFactors->setY(-0.5);
			}
		}
	}
	else if (border.joinsWith(topRight))
	{
		if (!border.joinsWith(top))
		{
			if (top.joinsWith(topLeft))
			{
				if (border.width() < top.width())
				{
					// Cases: 14A.
					start->setY(start->y() + 0.5 * top.width());
				}
				else
				{
					// Cases: 14B.
					startOffsetFactors->setY(-0.5);
				}
			}
			else
			{
				// Cases: 4, 18, 32, 36.
				startOffsetFactors->setY(-0.5);
			}
		}
	}
	else if (border.joinsWith(top))
	{
		if (topLeft.joinsWith(topRight))
		{
			// Cases: 11.
			start->setY(start->y() + 0.5 * topLeft.width());
		}
	}
	else
	{
		// Cases: 1, 2, 3, 6, 12, 16, 20, 26, 28, 31, 33, 37, 39.
		start->setY(start->y() + 0.5 * qMax(topLeft.width(), topRight.width()));
	}
	// Cases: 7, 9, 10, 13, 29, 30, 34, 35 - No adjustment to start point(s) needed.

	/*
	 * Adjust end point(s). Possible cases are 1-15, 21-35, 40-43.
	 */
	if (border.joinsWith(bottomLeft))
	{
		if (border.joinsWith(bottomRight))
		{
			if (!border.joinsWith(bottom))
			{
				// Cases: 6, 24.
				endOffsetFactors->setY(0.5);
			}
		}
		else if (!border.joinsWith(bottom))
		{
			if (bottom.joinsWith(bottomRight))
			{
				if (bottom.width() < border.width())
				{
					// Cases: 14A.
					endOffsetFactors->setY(0.5);
				}
				else
				{
					// Cases: 14B.
					end->setY(end->y() - 0.5 * bottom.width());
				}
			}
			else
			{
				// Cases: 2, 22, 28, 42.
				endOffsetFactors->setY(0.5);
			}
		}
	}
	else if (border.joinsWith(bottomRight))
	{
		if (!border.joinsWith(bottom))
		{
			if (bottom.joinsWith(bottomLeft))
			{
				if (bottom.width() < border.width())
				{
					// Cases: 15A.
					endOffsetFactors->setY(0.5);
				}
				else
				{
					// Cases: 15B.
					end->setY(end->y() - 0.5 * bottom.width());
				}
			}
			else
			{
				// Cases: 3, 23, 33, 40.
				endOffsetFactors->setY(0.5);
			}
		}
	}
	else if (border.joinsWith(bottom))
	{
		if (bottomLeft.joinsWith(bottomRight))
		{
			// Cases: 11.
			end->setY(end->y() - 0.5 * bottomLeft.width());
		}
	}
	else
	{
		// Cases: 1, 4, 5, 8, 12, 21, 25, 26, 27, 31, 32, 41, 43.
		end->setY(end->y() - 0.5 * qMax(bottomLeft.width(), bottomRight.width()));
	}
	// Cases: 7, 9, 10, 13, 29, 30, 34, 35 - No adjustment to end point(s) needed.
}

void joinHorizontal(const TableBorder& border, const TableBorder& topLeft, const TableBorder& left,
				  const TableBorder& bottomLeft, const TableBorder& topRight, const TableBorder& right,
				  const TableBorder& bottomRight, QPointF* start, QPointF* end, QPointF* startOffsetFactors,
				  QPointF* endOffsetFactors)
{
	Q_ASSERT(start);
	Q_ASSERT(end);
	Q_ASSERT(startOffsetFactors);
	Q_ASSERT(endOffsetFactors);

	// Reset offset coefficients.
	startOffsetFactors->setX(0.0);
	startOffsetFactors->setY(0.0);
	endOffsetFactors->setX(0.0);
	endOffsetFactors->setY(0.0);

	/*
	 * The numbered cases in the code below refers to the 45 possible join cases illustrated
	 * in the picture at http://wiki.scribus.net/canvas/File:Table_border_join_cases.png
	 */

	/*
	 * Adjust start point(s). Possible cases are 1-25, 31-37, 40-41.
	 */
	if (border.joinsWith(bottomLeft))
	{
		if (border.joinsWith(topLeft))
		{
			if (border.joinsWith(left))
			{
				// Cases: 10.
				startOffsetFactors->setX(0.5);
			}
			else
			{
				// Cases: 7, 34.
				startOffsetFactors->setX(0.5);
			}
		}
		else
		{
			if (border.joinsWith(left))
			{
				// Cases: 8, 19.
				startOffsetFactors->setX(0.5);
			}
			else if (left.joinsWith(topLeft))
			{
				if (border.width() < left.width())
				{
					// Cases: 14A.
					start->setX(start->x() + 0.5 * left.width());
				}
				else
				{
					// Cases: 14B.
					startOffsetFactors->setX(0.5);
				}
			}
			else
			{
				// Cases: 4, 18, 32, 36.
				startOffsetFactors->setX(0.5);
			}
		}
	}
	else if (border.joinsWith(topLeft))
	{
		if (border.joinsWith(left))
		{
			// Cases: 6, 24.
			startOffsetFactors->setX(0.5);
		}
		else
		{
			if (left.joinsWith(bottomLeft))
			{
				if (left.width() < border.width())
				{
					// Cases: 15A.
					startOffsetFactors->setX(0.5);
				}
				else
				{
					// Cases: 15B.
					start->setX(start->x() + 0.5 * left.width());
				}
			}
			else
			{
				// Cases: 3, 23, 33, 40.
				startOffsetFactors->setX(0.5);
			}
		}
	}
	else if (!border.joinsWith(left) &&
			 (topLeft.joinsWith(bottomLeft) || topLeft.joinsWith(left) || bottomLeft.joinsWith(left)))
	{
		// Cases: 2, 5, 9, 13, 17, 22, 35.
		start->setX(start->x() + 0.5 * qMax(topLeft.width(), bottomLeft.width()));
	}
	else if (left.isNull())
	{
		// Cases: 31, 37, 41.
		start->setX(start->x() - 0.5 * qMax(topLeft.width(), bottomLeft.width()));
	}
	// Cases: 1, 11, 12, 16, 20, 21, 25 - No adjustment to start point(s) needed.

	/*
	 * Adjust end point(s). Possible cases are 1-30, 38-39, 42-43.
	 */
	if (border.joinsWith(bottomRight))
	{
		if (border.joinsWith(topRight))
		{
			if (border.joinsWith(right))
			{
				// Cases: 10.
				endOffsetFactors->setX(-0.5);
			}
			else
			{
				// Cases: 9, 29.
				endOffsetFactors->setX(-0.5);
			}
		}
		else if (border.joinsWith(right))
		{
			// Cases: 6, 24.
			endOffsetFactors->setX(-0.5);
		}
		else
		{
			if (right.joinsWith(topRight))
			{
				if (border.width() < right.width())
				{
					// Cases: 15A.
					end->setX(end->x() - 0.5 * right.width());
				}
				else
				{
					// Cases: 15B.
					endOffsetFactors->setX(-0.5);
				}
			}
			else
			{
				// Cases: 5, 17, 27, 38.
				endOffsetFactors->setX(-0.5);
			}
		}
	}
	else if (border.joinsWith(topRight))
	{
		if (border.joinsWith(right))
		{
			// Cases: 6, 24.
			endOffsetFactors->setX(-0.5);
		}
		else
		{
			if (right.joinsWith(bottomRight))
			{
				if (right.width() < border.width())
				{
					// Cases: 14A.
					endOffsetFactors->setX(-0.5);
				}
				else
				{
					// Cases: 14B.
					end->setX(end->x() - 0.5 * right.width());
				}
			}
			else
			{
				// Cases: 2, 22, 28, 42.
				endOffsetFactors->setX(-0.5);
			}
		}
	}
	else if (!border.joinsWith(right) &&
			 (topRight.joinsWith(bottomRight) || topRight.joinsWith(right) || bottomRight.joinsWith(right)))
	{
		// Cases: 3, 4, 7, 13, 18, 23, 30.
		end->setX(end->x() - 0.5 * qMax(topRight.width(), bottomRight.width()));
	}
	else if (right.isNull())
	{
		// Cases: 26, 39, 43.
		end->setX(end->x() + 0.5 * qMax(topRight.width(), bottomRight.width()));
	}
	// Cases: 1, 11, 12, 16, 20, 21, 25 - No adjustment to end point(s) needed.
}

} // namespace TableUtils
