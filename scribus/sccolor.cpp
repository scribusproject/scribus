/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          sccolor.cpp  -  description
                             -------------------
    begin                : Sun Sep 9 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include <assert.h>

#include "commonstrings.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scribuscore.h"
#include "scribusdoc.h"

ScColor::ScColor(void)
{
//	Model = colorModelCMYK;
//	CR = MG = YB = K = 0;
	Model = colorModelRGB;
	CR = 150;
	MG = 100;
	YB = 50;
	K = 0;
	Spot = false;
	Regist = false;
}

ScColor::ScColor(int c, int m, int y, int k)
{
	Spot = false;
	Regist = false;
	setColor(c, m, y, k);
}
 
ScColor::ScColor(int r, int g, int b)
{
	Spot = false;
	Regist = false;
	setColorRGB(r, g, b);
}

bool ScColor::operator==(const ScColor& other) const
{
	if (Model != other.Model)
		return false;
	if (Spot != other.Spot)
		return false;
	if (Regist != other.Regist)
		return false;
	if (Model == colorModelRGB)
	{
		return ((CR == other.CR) && (MG == other.MG) && (YB == other.YB));
	}
	if (Model == colorModelCMYK)
	{
		return ((CR == other.CR) && (MG == other.MG) && (YB == other.YB) && (K == other.K));
	}
	return false;
}

void ScColor::setColor(int c, int m, int y, int k)
{
	CR = c;
	MG = m;
	YB = y;
	K = k;
	Model = colorModelCMYK;
}

void ScColor::setColorRGB(int r, int g, int b)
{
	CR = r;
	MG = g;
	YB = b;
	Model = colorModelRGB;
}

colorModel ScColor::getColorModel () const
{
	return Model;
}

void ScColor::fromQColor(QColor color)
{
	if (color.spec() == QColor::Cmyk)
	{
		int c, m, y, k;
		color.getCmyk(&c, &m, &y, &k);
		setColor(c, m, y, k);
	}
	else
	{
		int r, g, b;
		color.getRgb(&r, &g, &b);
		setColorRGB(r, g, b);
	}
}

void ScColor::getRawRGBColor(int *r, int *g, int *b) const
{
	if (Model == colorModelRGB)
	{
		*r = CR;
		*g = MG;
		*b = YB;
	}
	else
	{
		*r = 255-qMin(255, CR + K);
		*g = 255-qMin(255, MG + K);
		*b = 255-qMin(255, YB + K);
	}
}

QColor ScColor::getRawRGBColor() const
{
	if (Model == colorModelRGB)
		return QColor(CR, MG, YB);
	return QColor(255-qMin(255, CR + K), 255-qMin(255, MG + K), 255 - qMin(255, YB + K));
}

void ScColor::getRGB(int *r, int *g, int *b) const
{
	if( Model != colorModelRGB )
		qDebug("calling getRGB with a cmyk color");
	*r = CR;
	*g = MG;
	*b = YB;
}

void ScColor::getCMYK(int *c, int *m, int *y, int *k) const
{
	if( Model != colorModelCMYK )
		qDebug("calling getCMYK with a rgb color");
	*c = CR;
	*m = MG;
	*y = YB;
	*k = K;
}

QString ScColor::name()
{
	QString tmp, name="#";
	switch (Model) 
	{
	case colorModelCMYK:
		tmp.setNum(CR, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(MG, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(YB, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(K, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		break;
	case colorModelRGB:
		tmp.setNum(CR, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(MG, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(YB, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		break;
	default:
		name = CommonStrings::None;
	}
	return name;
}

QString ScColor::nameCMYK(const ScribusDoc* doc)
{
	if ((Model != colorModelCMYK) && (!doc))
		qDebug("calling nameCMYK with a rgb color");
	CMYKColor cmyk;
	int c, m, y, k;
	QString tmp, name = CommonStrings::None;
	ScColorEngine::getCMYKValues(*this, doc, cmyk);
	cmyk.getValues(c, m, y, k);
	name="#";
	tmp.setNum(c, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	name += tmp;
	tmp.setNum(m, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	name += tmp;
	tmp.setNum(y, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	name += tmp;
	tmp.setNum(k, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	name += tmp;
	return name;
}

QString ScColor::nameRGB(const ScribusDoc* doc)
{
	if ((Model != colorModelRGB) && (!doc))
		qDebug("calling nameRGB with a cmyk color");
	int r, g, b;
	RGBColor rgb;
	QString tmp, name = CommonStrings::None;
	ScColorEngine::getRGBValues(*this, doc, rgb);
	rgb.getValues(r, g, b);
	name="#";
	tmp.setNum(r, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	name += tmp;
	tmp.setNum(g, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	name += tmp;
	tmp.setNum(b, 16);
	if (tmp.length() < 2)
		tmp.insert(0, "0");
	name += tmp;
	return name;
}

void ScColor::setNamedColor(QString name)
{
	bool ok;
	if (name.length () == 9)
	{
		int c = name.mid(1,2).toInt(&ok, 16);
		int m = name.mid(3,2).toInt(&ok, 16);
		int y = name.mid(5,2).toInt(&ok, 16);
		int k = name.mid(7,2).toInt(&ok, 16);
		setColor(c, m, y, k);
	}
	else if (name.length () == 7)
	{
		int r = name.mid(1,2).toInt(&ok, 16);
		int g = name.mid(3,2).toInt(&ok, 16);
		int b = name.mid(5,2).toInt(&ok, 16);
		setColorRGB(r, g, b);
	}
}

bool ScColor::isRegistrationColor() const
{
	return Regist;
}

void ScColor::setRegistrationColor(bool s)
{
	Regist = s;
}

bool ScColor::isSpotColor() const
{
	return Spot;
}

void ScColor::setSpotColor(bool s)
{
	Spot = s;
}

ColorList::ColorList(ScribusDoc* doc, bool retainDoc) : QMap<QString,ScColor>()
{
	m_doc = doc;
	m_retainDoc = retainDoc;
}

void ColorList::setDocument(ScribusDoc* doc)
{
	m_doc = doc;
}

ColorList& ColorList::operator= (const ColorList& list)
{
	clear();
	if (!m_retainDoc)
		m_doc = list.m_doc;
	addColors(list);
	return *this;
}

void ColorList::addColors(const ColorList& colorList, bool overwrite)
{
	ColorList::ConstIterator it;
	ColorList::ConstIterator itend;
	itend = colorList.end();
	for (it = colorList.begin(); it != itend; ++it)
	{
		if (overwrite || !contains(it.key()))
			insert(it.key(), it.value());
	}
}

void ColorList::copyColors(const ColorList& colorList, bool overwrite)
{
	clear();
	addColors(colorList, overwrite);
}

ScribusDoc* ColorList::document(void) const
{ 
	return m_doc;
}

void ColorList::ensureDefaultColors(void)
{
	ensureBlack();
	ensureWhite();
	ensureRegistration();
}

void ColorList::ensureBlack(void)
{
	bool addBlack = true;
	ColorList::Iterator itb = find("Black");
	if (itb != end())
	{
		ScColor& black = itb.value();
		colorModel model = black.getColorModel();
		if (model == colorModelCMYK)
		{
			int c, m, y, k;
			black.getCMYK(&c, &m, &y, &k);
			if (c == 0 && m == 0 && y == 0 && k == 255)
				addBlack = false;
		}
	}
	if (addBlack)
		insert("Black", ScColor(0, 0, 0, 255));
}

void ColorList::ensureWhite(void)
{
	bool addWhite = true;
	ColorList::Iterator itw = find("White");
	if (itw != end())
	{
		ScColor& white = itw.value();
		colorModel model = white.getColorModel();
		if (model == colorModelCMYK)
		{
			int c, m, y, k;
			white.getCMYK(&c, &m, &y, &k);
			if (c == 0 && m == 0 && y == 0 && k == 0)
				addWhite = false;
		}
	}
	if (addWhite)
		insert("White", ScColor(0, 0, 0, 0));
}

void ColorList::ensureRegistration(void)
{
	ScColor cc = ScColor(255, 255, 255, 255);
	cc.setRegistrationColor(true);
	insert("Registration", cc);
}
