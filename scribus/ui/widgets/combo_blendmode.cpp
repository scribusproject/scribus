#include "combo_blendmode.h"

ComboBlendMode::ComboBlendMode(QWidget *parent) : QComboBox(parent)
{	
	initialize();
}

void ComboBlendMode::languageChange()
{

	int oldBlendMode = currentIndex();
	QSignalBlocker signalBlocker(this);

	initialize();

	this->setCurrentIndex(oldBlendMode);
}

void ComboBlendMode::initialize()
{
	this->clear();
	this->addItem( tr("Normal"), 0);
	this->addItem( tr("Darken"), 1);
	this->addItem( tr("Lighten"), 2);
	this->addItem( tr("Multiply"), 3);
	this->addItem( tr("Screen"), 4);
	this->addItem( tr("Overlay"), 5);
	this->addItem( tr("Hard Light"), 6);
	this->addItem( tr("Soft Light"), 7);
	this->addItem( tr("Difference"), 8);
	this->addItem( tr("Exclusion"), 9);
	this->addItem( tr("Color Dodge"), 10);
	this->addItem( tr("Color Burn"), 11);
	this->addItem( tr("Hue"), 12);
	this->addItem( tr("Saturation"), 13);
	this->addItem( tr("Color"), 14);
	this->addItem( tr("Luminosity"), 15);

	this->setCurrentText( tr("Normal"));

}
