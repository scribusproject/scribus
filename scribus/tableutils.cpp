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

namespace TableUtils
{

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
