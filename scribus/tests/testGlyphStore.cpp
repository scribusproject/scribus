/*
 *  testGlyphStore.cpp
 *  ScribusProject
 *
 *  Created by Andreas Vox on 22.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "testGlyphStore.h"

void TestGlyphStore::cluster()
{
	Cluster c;
	QVERIFY(!c.isValid());
}

void TestGlyphStore::initGS()
{
	StoryText story;
	GlyphStore gs(&story);
	QCOMPARE(gs.nrOfItems(), 0u);
	QCOMPARE(gs.story(), &story);
	
	gs.addItem(0,1);
	QCOMPARE(gs.nrOfItems(), 1u);
}



