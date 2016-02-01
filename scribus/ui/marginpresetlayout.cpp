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

	updateMargins = false;

	this->setToolTip( "<qt>" + tr("When you have selected a Document Layout other than Single Page, you can select a predefined page layout here. 'None' leaves margins as is, Gutenberg sets margins classically. 'Magazine' sets all margins to the same value. Leading is Left/Inside value.") + "</qt>");
}

MarginStruct PresetLayout::getMargins(int index, double pageWidth, double pageHeight, double leftMargin)
{
	MarginStruct ret;

	updateMargins = true;

	switch (index)
	{
		case PresetLayout::magazine:
			ret.set(leftMargin, leftMargin, leftMargin, leftMargin);
			break;
		case PresetLayout::gutenberg:
			{
				double ratio = pageHeight / pageWidth;
				ret.setLeft(leftMargin);
				ret.setTop(leftMargin * ratio);
				ret.setRight(leftMargin * 2.0);
				ret.setBottom(ret.right() * ratio);
			}
			break;
		case PresetLayout::fibonacci:
			ret.setLeft(leftMargin);
			ret.setTop(leftMargin / 2.0 * 3.0);
			ret.setRight(leftMargin / 2.0 * 5.0);
			ret.setBottom(leftMargin / 2.0 * 8.0);
			break;
		case PresetLayout::goldencut:
			ret.setLeft(leftMargin);
			ret.setTop(leftMargin / 2.0 * 3.4);
			ret.setRight(leftMargin / 2.0 * 4.8);
			ret.setBottom(leftMargin / 2.0 * 6.8);
			break;
		case PresetLayout::nineparts:
			ret.setLeft(pageWidth / 9.0);
			ret.setTop(pageHeight / 9.0);
			ret.setRight(pageWidth / 9.0 * 2.0);
			ret.setBottom(pageHeight / 9.0 * 2.0);
			break;
		default:
			updateMargins = false;
			ret.set(-1.0, -1.0, -1.0, -1.0);
	}
	return ret;
}

bool PresetLayout::needUpdate() const
{
	return updateMargins;
}

