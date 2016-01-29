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
	m_Model = colorModelRGB;
	m_CR = 150;
	m_MG = 100;
	m_YB = 50;
	m_K = 0;
	m_L_val = m_a_val = m_b_val = 0;
	m_Spot = false;
	m_Regist = false;
}

ScColor::ScColor(int c, int m, int y, int k)
{
	m_L_val = m_a_val = m_b_val = 0;
	m_Spot = false;
	m_Regist = false;
	setColor(c, m, y, k);
}
 
ScColor::ScColor(int r, int g, int b)
{
	m_K = 0;
	m_L_val = m_a_val = m_b_val = 0;
	m_Spot = false;
	m_Regist = false;
	setColorRGB(r, g, b);
}

ScColor::ScColor(double l, double a, double b)
{
	m_Spot = false;
	m_Regist = false;
	m_Model = colorModelLab;
	m_L_val = l;
	m_a_val = a;
	m_b_val = b;
}

bool ScColor::operator==(const ScColor& other) const
{
	if (m_Model != other.m_Model)
		return false;
	if (m_Spot != other.m_Spot)
		return false;
	if (m_Regist != other.m_Regist)
		return false;
	if (m_Model == colorModelRGB)
	{
		return ((m_CR == other.m_CR) && (m_MG == other.m_MG) && (m_YB == other.m_YB));
	}
	if (m_Model == colorModelCMYK)
	{
		return ((m_CR == other.m_CR) && (m_MG == other.m_MG) && (m_YB == other.m_YB) && (m_K == other.m_K));
	}
	return false;
}

void ScColor::setColor(int c, int m, int y, int k)
{
	m_CR = c;
	m_MG = m;
	m_YB = y;
	m_K = k;
	m_Model = colorModelCMYK;
}

void ScColor::setColor(double l, double a, double b)
{
	m_Model = colorModelLab;
	m_L_val = l;
	m_a_val = a;
	m_b_val = b;
}

void ScColor::setColorRGB(int r, int g, int b)
{
	m_CR = r;
	m_MG = g;
	m_YB = b;
	m_Model = colorModelRGB;
}

colorModel ScColor::getColorModel () const
{
	return m_Model;
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
	if (m_Model == colorModelRGB)
	{
		*r = m_CR;
		*g = m_MG;
		*b = m_YB;
	}
	else
	{
		*r = 255-qMin(255, m_CR + m_K);
		*g = 255-qMin(255, m_MG + m_K);
		*b = 255-qMin(255, m_YB + m_K);
	}
}

QColor ScColor::getRawRGBColor() const
{
	if (m_Model == colorModelRGB)
		return QColor(m_CR, m_MG, m_YB);
	return QColor(255-qMin(255, m_CR + m_K), 255-qMin(255, m_MG + m_K), 255 - qMin(255, m_YB + m_K));
}

void ScColor::getRGB(int *r, int *g, int *b) const
{
	if (m_Model != colorModelRGB)
		qDebug("calling getRGB with a cmyk color");
	*r = m_CR;
	*g = m_MG;
	*b = m_YB;
}

void ScColor::getCMYK(int *c, int *m, int *y, int *k) const
{
	if (m_Model != colorModelCMYK)
		qDebug("calling getCMYK with a rgb color");
	*c = m_CR;
	*m = m_MG;
	*y = m_YB;
	*k = m_K;
}

void ScColor::getLab(double *L, double *a, double *b) const
{
	if (m_Model != colorModelLab)
		qDebug("calling getLab with a non Lab color");
	*L = m_L_val;
	*a = m_a_val;
	*b = m_b_val;
}

QString ScColor::name()
{
	QString tmp, name="#";
	switch (m_Model) 
	{
	case colorModelCMYK:
		tmp.setNum(m_CR, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(m_MG, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(m_YB, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(m_K, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		break;
	case colorModelRGB:
		tmp.setNum(m_CR, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(m_MG, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		tmp.setNum(m_YB, 16);
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
	if ((m_Model != colorModelCMYK) && (!doc))
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
	if ((m_Model != colorModelRGB) && (!doc))
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
	return m_Regist;
}

void ScColor::setRegistrationColor(bool s)
{
	m_Regist = s;
}

bool ScColor::isSpotColor() const
{
	return m_Spot;
}

void ScColor::setSpotColor(bool s)
{
	m_Spot = s;
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

QString ColorList::tryAddColor(QString name, ScColor col)
{
	if (contains(name))
		return name;
	bool found = false;
	QString ret = name;
	ColorList::Iterator it;
	for (it = begin(); it != end(); ++it)
	{
		if (it.value() == col)
		{
			ret = it.key();
			found = true;
			break;
		}
	}
	if (!found)
		insert(name, col);
	return ret;
}
