/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          fontcombo.cpp  -  description
                             -------------------
    begin                : Die Jun 17 2003
    copyright            : (C) 2003 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QGridLayout>
#include <QPixmap>
#include <QLabel>
#include <QAbstractItemView>
#include <QFont>
#include <QStringList>

#include "sccombobox.h"

#include "scribusdoc.h"
#include "fontcombo.h"
#include "page.h"
#include "prefsmanager.h"
#include "util.h"

FontCombo::FontCombo(QWidget* pa) : QComboBox(pa)
{
	prefsManager = PrefsManager::instance();
	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	substFont = loadIcon("font_subst16.png");
	setEditable(false);
	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);
	RebuildList(0);
}

void FontCombo::RebuildList(ScribusDoc *currentDoc, bool forAnnotation, bool forSubstitute)
{
	clear();
	QMap<QString, QString> rlist;
	rlist.clear();
	SCFontsIterator it(prefsManager->appPrefs.AvailFonts);
	for ( ; it.hasNext(); it.next())
	{
		if (it.current().usable())
		{
			if (currentDoc != NULL)
			{
				if (currentDoc->DocName == it.current().localForDocument() || it.current().localForDocument().isEmpty())
					rlist.insert(it.currentKey().lower(), it.currentKey());
			}
			else
				rlist.insert(it.currentKey().lower(), it.currentKey());
		}
	}
	for (QMap<QString,QString>::Iterator it2 = rlist.begin(); it2 != rlist.end(); ++it2)
	{
		ScFace fon = prefsManager->appPrefs.AvailFonts[it2.data()];
		if (! fon.usable() )
			continue;
		ScFace::FontType type = fon.type();
		if ((forAnnotation) && ((type == ScFace::TYPE1) || (type == ScFace::OTF) || fon.subset()))
			continue;
		if (forSubstitute && fon.isReplacement())
			continue;
		if (fon.isReplacement())
			addItem(substFont, it2.data());
		else if (type == ScFace::OTF)
			addItem(otfFont, it2.data());
		else if (type == ScFace::TYPE1)
			addItem(psFont, it2.data());
		else if (type == ScFace::TTF)
			addItem(ttfFont, it2.data());
	}
	QAbstractItemView *tmpView = view();
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
}

FontComboH::FontComboH(QWidget* parent, bool labels) :
		QWidget(parent, "FontComboH"),
		fontFaceLabel(0),
		fontStyleLabel(0),
		showLabels(labels)
{
	prefsManager = PrefsManager::instance();
	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	substFont = loadIcon("font_subst16.png");
	currDoc = 0;
	fontComboLayout = new QGridLayout(this);
	fontComboLayout->setMargin(0);
	fontComboLayout->setSpacing(0);
	int col=0;
	if (showLabels)
	{
		fontFaceLabel=new QLabel("", this);
		fontStyleLabel=new QLabel("", this);
		fontComboLayout->addWidget(fontFaceLabel,0,0);
		fontComboLayout->addWidget(fontStyleLabel,1,0);
		fontComboLayout->setColStretch(1,10);
		col=1;
	}
	fontFamily = new ScComboBox( false, this, "fontFamily" );
	fontComboLayout->addWidget(fontFamily,0,col);
	fontStyle = new ScComboBox( false, this, "fontStyle" );
	fontComboLayout->addWidget(fontStyle,1,col);
	RebuildList(0);
	connect(fontFamily, SIGNAL(activated(int)), this, SLOT(familySelected(int)));
	connect(fontStyle, SIGNAL(activated(int)), this, SLOT(styleSelected(int)));
	languageChange();
}

void FontComboH::languageChange()
{
	if(showLabels)
	{
		fontFaceLabel->setText( tr("Face:"));
		fontStyleLabel->setText( tr("Style:"));
	}
}

void FontComboH::familySelected(int id)
{
	disconnect(fontStyle, SIGNAL(activated(int)), this, SLOT(styleSelected(int)));
	QString curr = fontStyle->currentText();
	fontStyle->clear();
	QStringList slist = prefsManager->appPrefs.AvailFonts.fontMap[fontFamily->text(id)];
	slist.sort();
	fontStyle->addItems(slist);
	if (slist.contains(curr))
		fontStyle->setCurrentText(curr);
	else if (slist.contains("Regular"))
		fontStyle->setCurrentText("Regular");
	else if (slist.contains("Roman"))
		fontStyle->setCurrentText("Roman");
	emit fontSelected(fontFamily->text(id) + " " + fontStyle->currentText());
	connect(fontStyle, SIGNAL(activated(int)), this, SLOT(styleSelected(int)));
}

void FontComboH::styleSelected(int id)
{
	emit fontSelected(fontFamily->currentText() + " " + fontStyle->text(id));
}

QString FontComboH::currentFont()
{
	return fontFamily->currentText() + " " + fontStyle->currentText();
}

void FontComboH::setCurrentFont(QString f)
{
	disconnect(fontFamily, SIGNAL(activated(int)), this, SLOT(familySelected(int)));
	disconnect(fontStyle, SIGNAL(activated(int)), this, SLOT(styleSelected(int)));
	QString family = prefsManager->appPrefs.AvailFonts[f].family();
	QString style = prefsManager->appPrefs.AvailFonts[f].style();
	fontFamily->setCurrentText(family);
	fontStyle->clear();
	QStringList slist = prefsManager->appPrefs.AvailFonts.fontMap[family];
	slist.sort();
	QStringList ilist;
	ilist.clear();
	if (currDoc != NULL)
	{
		for (QStringList::ConstIterator it3 = slist.begin(); it3 != slist.end(); ++it3)
		{
			if ((currDoc->DocName == prefsManager->appPrefs.AvailFonts[family + " " + *it3].localForDocument()) || (prefsManager->appPrefs.AvailFonts[family + " " + *it3].localForDocument().isEmpty()))
				ilist.append(*it3);
		}
		fontStyle->addItems(ilist);
	}
	else
		fontStyle->addItems(slist);
	fontStyle->setCurrentText(style);
	connect(fontFamily, SIGNAL(activated(int)), this, SLOT(familySelected(int)));
	connect(fontStyle, SIGNAL(activated(int)), this, SLOT(styleSelected(int)));
}

void FontComboH::RebuildList(ScribusDoc *currentDoc, bool forAnnotation, bool forSubstitute)
{
	currDoc = currentDoc;
	disconnect(fontFamily, SIGNAL(activated(int)), this, SLOT(familySelected(int)));
	disconnect(fontStyle, SIGNAL(activated(int)), this, SLOT(styleSelected(int)));
	fontFamily->clear();
	fontStyle->clear();
	QStringList rlist = prefsManager->appPrefs.AvailFonts.fontMap.keys();
	QMap<QString, QString> flist;
	flist.clear();
	for (QStringList::ConstIterator it2 = rlist.begin(); it2 != rlist.end(); ++it2)
	{
		if (currentDoc != NULL)
		{
			QStringList slist = prefsManager->appPrefs.AvailFonts.fontMap[*it2];
			slist.sort();
			QStringList ilist;
			ilist.clear();
			for (QStringList::ConstIterator it3 = slist.begin(); it3 != slist.end(); ++it3)
			{
				if ( prefsManager->appPrefs.AvailFonts.contains(*it2 + " " + *it3))
				{
					const ScFace& fon(prefsManager->appPrefs.AvailFonts[*it2 + " " + *it3]);
					if (fon.usable() && !fon.isReplacement() && (currentDoc->DocName == fon.localForDocument() || fon.localForDocument().isEmpty()))
						ilist.append(*it3);
				}
			}
			if (!ilist.isEmpty())
				flist.insert((*it2).lower(), *it2);
		}
		else
			flist.insert((*it2).lower(), *it2);
	}
	for (QMap<QString,QString>::Iterator it2a = flist.begin(); it2a != flist.end(); ++it2a)
	{
		ScFace fon = prefsManager->appPrefs.AvailFonts[it2a.data()+" "+prefsManager->appPrefs.AvailFonts.fontMap[it2a.data()][0]];
		if ( !fon.usable() || fon.isReplacement() )
			continue;
		ScFace::FontType type = fon.type();
		if ((forAnnotation) && ((type == ScFace::TYPE1) || (type == ScFace::OTF) || (fon.subset())))
			continue;
		if ((forSubstitute) && fon.isReplacement())
			continue;
		if (fon.isReplacement())
			fontFamily->addItem(substFont, it2a.data());
		else if (type == ScFace::OTF)
			fontFamily->addItem(otfFont, it2a.data());
		else if (type == ScFace::TYPE1)
			fontFamily->addItem(psFont, it2a.data());
		else if (type == ScFace::TTF)
			fontFamily->addItem(ttfFont, it2a.data());
	}
	QString family = fontFamily->currentText();
	QStringList slist = prefsManager->appPrefs.AvailFonts.fontMap[family];
	slist.sort();
	QStringList ilist;
	ilist.clear();
	if (currentDoc != NULL)
	{
		for (QStringList::ConstIterator it3 = slist.begin(); it3 != slist.end(); ++it3)
		{
			if (prefsManager->appPrefs.AvailFonts.contains(family + " " + *it3) && (currentDoc->DocName == prefsManager->appPrefs.AvailFonts[family + " " + *it3].localForDocument() || prefsManager->appPrefs.AvailFonts[family + " " + *it3].localForDocument().isEmpty()))
				ilist.append(*it3);
		}
		fontStyle->addItems(ilist);
	}
	else
		fontStyle->insertStringList(slist);
	connect(fontFamily, SIGNAL(activated(int)), this, SLOT(familySelected(int)));
	connect(fontStyle, SIGNAL(activated(int)), this, SLOT(styleSelected(int)));
}
