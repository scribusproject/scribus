/*
 * Copyright (C) 2011 Elvis Stansvik <elvstone@gmail.com>
 *
 * For general Scribus (>=1.3.2) copyright and licensing information please refer
 * to the COPYING file provided with the program. Following this notice may exist
 * a copyright and/or license notice that predates the release of Scribus 1.3.2
 * for which a new license (GPL+exception) is in place.
 */
#ifndef CELLAREATESTS_H
#define CELLAREATESTS_H

#include <QtTest/QtTest>

/**
 * Unit tests for CellArea.
 */
class CellAreaTests : public QObject
{
	Q_OBJECT
public:
	CellAreaTests() {}

private slots:
	void testConstructionAndSetters();
	void testComparison();
	void testContainsPoint();
	void testContainsPoint_data();
	void testContainsArea();
	void testContainsArea_data();
	void testIntersects();
	void testIntersects_data();
	void testTranslated();
	void testTranslated_data();
	void testTranslate();
	void testTranslate_data();
	void testAdjusted();
	void testAdjusted_data();
	void testAdjust();
	void testAdjust_data();
	void testUnited();
	void testUnited_data();
	void testAdjustedForRowInsertion();
	void testAdjustedForRowInsertion_data();
	void testAdjustedForRowRemoval();
	void testAdjustedForRowRemoval_data();
	void testAdjustedForColumnInsertion();
	void testAdjustedForColumnInsertion_data();
	void testAdjustedForColumnRemoval();
	void testAdjustedForColumnRemoval_data();
 };

#endif // CELLAREATESTS_H
