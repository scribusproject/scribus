/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "marginpresetlayout.h"

PresetLayout::PresetLayout(QWidget *parent) : QComboBox(parent)
{
	addItem( tr("None", "layout type"), PresetLayout::none);
	addItem( tr("Gutenberg"), PresetLayout::gutenberg);
	addItem( tr("Magazine"), PresetLayout::magazine);
	addItem( tr("Fibonacci"), PresetLayout::fibonacci);
	addItem( tr("Golden Mean"), PresetLayout::goldencut);
	addItem( tr("Nine Parts"), PresetLayout::nineparts);
	setCurrentIndex(PresetLayout::none);

	this->setToolTip( "<qt>" + tr("When you have selected a Document Layout other than Single Page, you can select a predefined page layout here. 'None' leaves margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins to the same value. Leading is Left/Inside value.") + "</qt>");
}

MarginStruct PresetLayout::getMargins(int index, double pageWidth, double pageHeight, double leftMargin)
{
	MarginStruct ret;

	updateMargins = true;

	switch (index)
	{
		case PresetLayout::magazine:
			ret.Top = ret.Bottom = ret.Left = ret.Right = leftMargin;
			break;
		case PresetLayout::gutenberg:
			{
				double ratio = pageHeight / pageWidth;
				ret.Left = leftMargin;
				ret.Top = leftMargin * ratio;
				ret.Right = leftMargin * 2.0;
				ret.Bottom = ret.Right * ratio;
			}
			break;
		case PresetLayout::fibonacci:
			ret.Left = leftMargin;
			ret.Top = leftMargin / 2.0 * 3.0;
			ret.Right = leftMargin / 2.0 * 5.0;
			ret.Bottom = leftMargin / 2.0 * 8.0;
			break;
		case PresetLayout::goldencut:
			ret.Left = leftMargin;
			ret.Top = leftMargin / 2.0 * 3.4;
			ret.Right = leftMargin / 2.0 * 4.8;
			ret.Bottom = leftMargin / 2.0 * 6.8;
			break;
		case PresetLayout::nineparts:
			ret.Left = pageWidth / 9.0;
			ret.Top = pageHeight / 9.0;
			ret.Right = pageWidth / 9.0 * 2.0;
			ret.Bottom = pageHeight / 9.0 * 2.0;
			break;
		default:
			updateMargins = false;
			ret.Top = ret.Bottom = ret.Left = ret.Right = -1.0;
	}
	return ret;
}

bool PresetLayout::needUpdate() const
{
	return updateMargins;
}

