/*
 *  testGlyphStore.cpp
 *  ScribusProject
 *
 *  Created by Andreas Vox on 22.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <QtTest/QtTest>

#include "text/storytext.h"
#include "text/glyphstore.h"

class TestGlyphStore: public QObject
{
	Q_OBJECT

private slots:

	void cluster();
	void initGS();

};
