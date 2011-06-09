/*
 *  testGlyphStore.cpp
 *  ScribusProject
 *
 *  Created by Andreas Vox on 22.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <QtTest/QtTest>

/* Avox possibly introduced a bug here. Commenting out for now.
 * #include "text/index.h" */

class TestIndex: public QObject
{
	Q_OBJECT

private slots:

	void empty();
	void add();
	void remove();
	void access();
	void insert();
};
