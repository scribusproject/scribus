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

#include <cassert>

#include "commonstrings.h"
#include "sccolor.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scribuscore.h"
#include "scribusdoc.h"
#include "scribusstructs.h"

ScColor::ScColor()
{
	m_values[0] = 0.6;
	m_values[1] = 0.4;
	m_values[2] = 0.2;
}

ScColor::ScColor(int c, int m, int y, int k)
{
	setColor(c, m, y, k);
}
 
ScColor::ScColor(int r, int g, int b)
{
	setRgbColor(r, g, b);
}

ScColor::ScColor(double l, double a, double b)
	   : m_L_val(l),
	     m_a_val(a),
	     m_b_val(b),
	     m_Model(colorModelLab)
{
	m_values[0] = 0.6;
	m_values[1] = 0.4;
	m_values[2] = 0.2;
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
		return ((m_values[0] == other.m_values[0]) && (m_values[1] == other.m_values[1]) && (m_values[2] == other.m_values[2]));
	}
	if (m_Model == colorModelCMYK)
	{
		return ((m_values[0] == other.m_values[0]) && (m_values[1] == other.m_values[1]) && (m_values[2] == other.m_values[2]) && (m_values[3] == other.m_values[3]));
	}
	return false;
}

void ScColor::setColor(int c, int m, int y, int k)
{
	m_values[0] = c / 255.0;
	m_values[1] = m / 255.0;
	m_values[2] = y / 255.0;
	m_values[3] = k / 255.0;
	m_Model = colorModelCMYK;
}

void ScColor::setColorF(double c, double m, double y, double k)
{
	m_values[0] = qMax(0.0, qMin(c, 1.0));
	m_values[1] = qMax(0.0, qMin(m, 1.0));
	m_values[2] = qMax(0.0, qMin(y, 1.0));
	m_values[3] = qMax(0.0, qMin(k, 1.0));
	m_Model = colorModelCMYK;
}

void ScColor::setCmykColor(int c, int m, int y, int k)
{
	m_values[0] = c / 255.0;
	m_values[1] = m / 255.0;
	m_values[2] = y / 255.0;
	m_values[3] = k / 255.0;
	m_Model = colorModelCMYK;
}

void ScColor::setCmykColorF(double c, double m, double y, double k)
{
	m_values[0] = qMax(0.0, qMin(c, 1.0));
	m_values[1] = qMax(0.0, qMin(m, 1.0));
	m_values[2] = qMax(0.0, qMin(y, 1.0));
	m_values[3] = qMax(0.0, qMin(k, 1.0));
	m_Model = colorModelCMYK;
}

void ScColor::setRgbColor(int r, int g, int b)
{
	m_values[0] = r / 255.0;
	m_values[1] = g / 255.0;
	m_values[2] = b / 255.0;
	m_Model = colorModelRGB;
}

void ScColor::setRgbColorF(double r, double g, double b)
{
	m_values[0] = qMax(0.0, qMin(r, 1.0));
	m_values[1] = qMax(0.0, qMin(g, 1.0));
	m_values[2] = qMax(0.0, qMin(b, 1.0));
	m_Model = colorModelRGB;
}

/** \brief Same as the Constructor but for an existing Color */
void ScColor::setLabColor(double l, double a, double b)
{
	m_Model = colorModelLab;
	m_L_val = l;
	m_a_val = a;
	m_b_val = b;
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
		setRgbColor(r, g, b);
	}
}

void ScColor::getRawRGBColor(RGBColorF* rgb) const
{
	if (m_Model == colorModelRGB)
	{
		rgb->r = m_values[0];
		rgb->g = m_values[1];
		rgb->b = m_values[2];
		return;
	}

	rgb->r = 1.0 - qMin(1.0, m_values[0] + m_values[3]);
	rgb->g = 1.0 - qMin(1.0, m_values[1] + m_values[3]);
	rgb->b = 1.0 - qMin(1.0, m_values[2] + m_values[3]);
}

void ScColor::getRawRGBColor(int *r, int *g, int *b) const
{
	if (m_Model == colorModelRGB)
	{
		*r = qRound(m_values[0] * 255.0);
		*g = qRound(m_values[1] * 255.0);
		*b = qRound(m_values[2] * 255.0);
		return;
	}

	double dR = 1.0 - qMin(1.0, m_values[0] + m_values[3]);
	double dG = 1.0 - qMin(1.0, m_values[1] + m_values[3]);
	double dB = 1.0 - qMin(1.0, m_values[2] + m_values[3]);
	*r = qRound(dR * 255.0);
	*g = qRound(dG * 255.0);
	*b = qRound(dB * 255.0);
}

QColor ScColor::getRawRGBColor() const
{
	if (m_Model == colorModelRGB)
	{
		double r = m_values[0];
		double g = m_values[1];
		double b = m_values[2];
		return QColor::fromRgbF(r, g, b);
	}

	double dR = 1.0 - qMin(1.0, m_values[0] + m_values[3]);
	double dG = 1.0 - qMin(1.0, m_values[1] + m_values[3]);
	double dB = 1.0 - qMin(1.0, m_values[2] + m_values[3]);
	return QColor::fromRgbF(dR, dG, dB);
}

void ScColor::getRGB(int *r, int *g, int *b) const
{
	if (m_Model != colorModelRGB)
		qDebug("calling getRGB with a cmyk color");
	*r = qRound(m_values[0] * 255.0);
	*g = qRound(m_values[1] * 255.0);
	*b = qRound(m_values[2] * 255.0);
}

void ScColor::getRGB(double *r, double *g, double *b) const
{
	if (m_Model != colorModelRGB)
		qDebug("calling getRGB with a cmyk color");
	*r = m_values[0];
	*g = m_values[1];
	*b = m_values[2];
}

void ScColor::getCMYK(int *c, int *m, int *y, int *k) const
{
	if (m_Model != colorModelCMYK)
		qDebug("calling getCMYK with a rgb color");
	*c = qRound(m_values[0] * 255.0);
	*m = qRound(m_values[1] * 255.0);
	*y = qRound(m_values[2] * 255.0);
	*k = qRound(m_values[3] * 255.0);
}

void ScColor::getCMYK(double *c, double *m, double *y, double *k) const
{
	if (m_Model != colorModelCMYK)
		qDebug("calling getCMYK with a rgb color");
	*c = m_values[0];
	*m = m_values[1];
	*y = m_values[2];
	*k = m_values[3];
}

void ScColor::getLab(double *L, double *a, double *b) const
{
	if (m_Model != colorModelLab)
		qDebug("calling getLab with a non Lab color");
	*L = m_L_val;
	*a = m_a_val;
	*b = m_b_val;
}

QString ScColor::name() const
{
	int value;
	QString tmp, name="#";

	switch (m_Model) 
	{
	case colorModelCMYK:
		value = qRound(m_values[0] * 255.0);
		tmp.setNum(value, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		value = qRound(m_values[1] * 255.0);
		tmp.setNum(value, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		value = qRound(m_values[2] * 255.0);
		tmp.setNum(value, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		value = qRound(m_values[3] * 255.0);
		tmp.setNum(value, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		break;
	case colorModelRGB:
		value = qRound(m_values[0] * 255.0);
		tmp.setNum(value, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		value = qRound(m_values[1] * 255.0);
		tmp.setNum(value, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		value = qRound(m_values[2] * 255.0);
		tmp.setNum(value, 16);
		if (tmp.length() < 2)
			tmp.insert(0, "0");
		name += tmp;
		break;
	default:
		name = CommonStrings::None;
	}
	return name;
}

QString ScColor::nameCMYK(const ScribusDoc* doc) const
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

QString ScColor::nameRGB(const ScribusDoc* doc) const
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

void ScColor::setNamedColor(QString colorName)
{
	bool ok;
	if (colorName.length () == 9)
	{
		int c = colorName.midRef(1,2).toInt(&ok, 16);
		int m = colorName.midRef(3,2).toInt(&ok, 16);
		int y = colorName.midRef(5,2).toInt(&ok, 16);
		int k = colorName.midRef(7,2).toInt(&ok, 16);
		setColor(c, m, y, k);
	}
	else if (colorName.length () == 7)
	{
		int r = colorName.midRef(1,2).toInt(&ok, 16);
		int g = colorName.midRef(3,2).toInt(&ok, 16);
		int b = colorName.midRef(5,2).toInt(&ok, 16);
		setRgbColor(r, g, b);
	}
}

bool ScColor::isProcessColor() const
{
	return (!m_Regist && !m_Spot);
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

ColorList::ColorList(ScribusDoc* doc, bool retainDoc)
	     : m_doc(doc),
	       m_retainDoc(retainDoc)
{

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

ScribusDoc* ColorList::document() const
{ 
	return m_doc;
}

void ColorList::ensureDefaultColors()
{
	ensureBlack();
	ensureWhite();
	ensureRegistration();
}

void ColorList::ensureBlack()
{
	ColorList::Iterator itb = find("Black");
	if (itb == end())
	{
		insert("Black", ScColor(0, 0, 0, 255));
		return;
	}

	ScColor& black = itb.value();
	colorModel model = black.getColorModel();
	 if (model == colorModelRGB)
	{
		double r, g, b;
		black.getRGB(&r, &g, &b);
		if (r != 0.0 || g != 0.0 || b != 0.0)
			black.setRgbColorF(0.0, 0.0, 0.0);
	}
	else if (model == colorModelCMYK)
	{
		double c, m, y, k;
		black.getCMYK(&c, &m, &y, &k);
		if (c != 0.0 || m != 0.0 || y != 0.0 || k != 1.0)
			black.setCmykColorF(0.0, 0.0, 0.0, 1.0);
	}
	else if (model == colorModelLab)
	{
		double l, a, b;
		black.getLab(&l, &a, &b);
		if (l != 0.0 || a != 0.0 || b != 0.0)
			black.setLabColor(0.0, 0.0, 0.0);
	}
	else
	{
		black.setCmykColorF(0.0, 0.0, 0.0, 1.0);
	}
}

void ColorList::ensureWhite()
{
	ColorList::Iterator itw = find("White");
	if (itw == end())
	{
		insert("White", ScColor(0, 0, 0, 0));
		return;
	}

	ScColor& white = itw.value();
	colorModel model = white.getColorModel();
	if (model == colorModelRGB)
	{
		double r, g, b;
		white.getRGB(&r, &g, &b);
		if (r != 1.0 || g != 1.0 || b != 1.0)
			white.setRgbColorF(1.0, 1.0, 1.0);
	}
	else if (model == colorModelCMYK)
	{
		double c, m, y, k;
		white.getCMYK(&c, &m, &y, &k);
		if (c != 0.0 || m != 0.0 || y != 0.0 || k != 0.0)
			white.setCmykColorF(0.0, 0.0, 0.0, 0.0);
	}
	else if (model == colorModelLab)
	{
		double l, a, b;
		white.getLab(&l, &a, &b);
		if (l != 100.0 || a != 0.0 || b != 0.0)
			white.setLabColor(100.0, 0.0, 0.0);
	}
	else
	{
		white.setCmykColorF(0.0, 0.0, 0.0, 0.0);
	}
}

void ColorList::ensureRegistration()
{
	ScColor cc(255, 255, 255, 255);
	cc.setRegistrationColor(true);
	insert("Registration", cc);
}

QString ColorList::tryAddColor(QString name, const ScColor& col)
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
