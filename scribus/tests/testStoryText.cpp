/*
 *  testStoryText.cpp
 *  ScribusProject
 *
 *  Created by Andreas Vox on 22.08.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <QDebug>
#include "testStoryText.h"

void TestStoryText::initST()
{
	StoryText story;
	QCOMPARE(story.length(), 0);
	story.insertChars(0, "Hallo Welt");
	QCOMPARE(story.nrOfParagraphs(), 1u);
	QCOMPARE(story.nrOfRuns(), 1u);
}

void TestStoryText::addText()
{
	StoryText story;
	story.insertChars(0, "Hallo Welt");
	QCOMPARE(story.length(), 10);
	QCOMPARE(story.text(0, story.length()), QString("Hallo Welt"));
	story.insertChars(5, " schöne neue");
	QCOMPARE(story.text(0, story.length()), QString("Hallo schöne neue Welt"));
}

void TestStoryText::removeText()
{
	StoryText story;
	story.insertChars(0, "Hallo Welt");
	story.removeChars(5, 5);
	QCOMPARE(story.text(0, story.length()), QString("Hallo"));	
}

void TestStoryText::copy()
{
	StoryText story1;
	story1.insertChars(0, "Hallo Welt");
	StoryText story2;
	story2.insert(0, story1);
	QCOMPARE(story2.text(0, story2.length()), QString("Hallo Welt"));
}

void TestStoryText::insertPar()
{
	StoryText story;
	story.insertChars(0, QString("Hallo") + SpecialChars::PARSEP + QString("Welt"));
	QCOMPARE(story.length(), 10);
	QCOMPARE(story.nrOfParagraphs(), 2u);
	QCOMPARE(story.nrOfRuns(), 2u);
	for (int i =0; i <= story.nrOfParagraphs(); ++i)
		qDebug() << "par" << i << "offset" << story.startOfParagraph(i);
	QCOMPARE(story.text(0,10).length(), 10);
	QCOMPARE(story.text(0, story.length()), QString("Hallo") + SpecialChars::PARSEP + QString("Welt"));
}

void TestStoryText::removePar()
{
	StoryText story;
	story.insertChars(0, QString("Hallo") + SpecialChars::PARSEP + QString("Welt"));
	story.removeChars(3, 6);
	QCOMPARE(story.length(), 10 - 6);
	QCOMPARE(story.text(0, story.length()), QString("Halt"));
}

void TestStoryText::removePars()
{		
	StoryText story;
	story.insertChars(0,
					  QString("0123456789") + SpecialChars::PARSEP + 
					  QString("abcdefghijklmnopqrstuvwxyz") + SpecialChars::PARSEP + 
					  QString("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	story.removeChars(5 + 26 + 1,  26 + 1);
	
	QCOMPARE(story.nrOfParagraphs(), 2u);
	QCOMPARE(story.text(0, story.length()), 
			 QString("0123456789") + SpecialChars::PARSEP + QString("abcdefghijklmnopqrstuVWXYZ"));
}

void TestStoryText::applyCharStyle()
{
	StoryText story;
	story.insertChars(0,
					  QString("0123456789") + SpecialChars::PARSEP + 
					  QString("abcdefghijklmnopqrstuvwxyz") + SpecialChars::PARSEP + 
					  QString("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	QCOMPARE(story.nrOfRuns(), 3u);
	CharStyle cs;
	cs.setFontSize(10);
	story.applyCharStyle(5 + 26 + 1,  26 + 1, cs);
	QCOMPARE(story.nrOfRuns(), 5u);
	QCOMPARE(story.startOfRun(0), 0);
	QCOMPARE(story.endOfRun(0), 11);
	QCOMPARE(story.startOfRun(1), 11);
	QCOMPARE(story.endOfRun(1), 5 + 26 + 1);
	QCOMPARE(story.startOfRun(2), 5 + 26 + 1);
	QCOMPARE(story.endOfRun(2), 11 + 26 + 1);
	QCOMPARE(story.startOfRun(3), 11 + 26 + 1);
	QCOMPARE(story.endOfRun(3), 5 + 26 + 1 + 26 + 1);
	QCOMPARE(story.startOfRun(4), 5 + 26 + 1 + 26 + 1);
	QCOMPARE(story.endOfRun(4), 11 + 26 + 1 + 26);
	QCOMPARE(story.charStyle(5 + 26 + 1).fontSize(), 10.0);
}

void TestStoryText::removeCharStyle()
{
	StoryText story;
	story.insertChars(0,
					  QString("0123456789") + SpecialChars::PARSEP + 
					  QString("abcdefghijklmnopqrstuvwxyz") + SpecialChars::PARSEP + 
					  QString("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	QCOMPARE(story.nrOfRuns(), 3u);
	CharStyle cs;
	cs.setFontSize(10);
	story.applyCharStyle(5 + 26 + 1,  26 + 1, cs);
	story.removeChars(11, 26+1);
	QCOMPARE(story.nrOfRuns(), 3u);
	QCOMPARE(story.startOfRun(0), 0);
	QCOMPARE(story.endOfRun(0), 11);
	QCOMPARE(story.startOfRun(1), 11);
	QCOMPARE(story.endOfRun(1), 5 + 26 + 1);
	QCOMPARE(story.startOfRun(2), 5  + 26 + 1);
	QCOMPARE(story.endOfRun(2), 11 + 26);
}
