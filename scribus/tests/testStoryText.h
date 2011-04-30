/*
 *  testStoryText.h
 *  ScribusProject
 *
 *  Created by Andreas Vox on 22.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


#include <QtTest/QtTest>

#include "text/storytext.h"

class TestStoryText: public QObject
{
		Q_OBJECT
		
private slots:
		
	void initST();
	void addText();
	void removeText();
	void copy();
	void insertPar();
	void removePar();
	void removePars();
	void applyCharStyle();
	void removeCharStyle();
};
