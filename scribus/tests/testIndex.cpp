/*
 *  testGlyphStore.cpp
 *  ScribusProject
 *
 *  Created by Andreas Vox on 22.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <QList>
#include "testIndex.h"

namespace QTest {
	template<>
	char *toString(const QList<int> &lst)
	{
		QByteArray ba = "[";
		for (int i=0; i < lst.count(); ++i)
			ba += QByteArray::number(lst[i]) + ", ";
		ba += "]";
		return qstrdup(ba.data());
	}
}

void TestIndex::empty()
{
	Index idx;
	QCOMPARE(idx.count(), 0u);
	QCOMPARE(idx.offset(0), 0);
	QCOMPARE(idx[0], 0u);
	QCOMPARE(idx[100], 0u);
	QCOMPARE(idx[-50], 0u);
	idx.adjust(-1, -100);
	QCOMPARE(idx.offset(idx.count()), 0);
}

void TestIndex::add()
{
	Index idx;
	idx.insert(0);
	idx.insert(20);
	idx.insert(10);
	QCOMPARE(idx.count(), 3u);
	QList<int> ac;
	ac << idx.end(0) << idx.end(1) << idx.end(2);
	QList<int> ex;
	ex << 0 << 10 << 20;
	QCOMPARE(ac, ex);
}

void TestIndex::insert()
{}
void TestIndex::remove()
{}
void TestIndex::access()
{}


