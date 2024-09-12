#include "combo_blendmode.h"
#include "scribusstructs.h"

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
	this->addItem( tr("Normal"), QVariant::fromValue(BlendMode::Normal));
	this->addItem( tr("Darken"), QVariant::fromValue(BlendMode::Darken));
	this->addItem( tr("Lighten"), QVariant::fromValue(BlendMode::Lighten));
	this->addItem( tr("Multiply"), QVariant::fromValue(BlendMode::Multiply));
	this->addItem( tr("Screen"), QVariant::fromValue(BlendMode::Screen));
	this->addItem( tr("Overlay"), QVariant::fromValue(BlendMode::Overlay));
	this->addItem( tr("Hard Light"), QVariant::fromValue(BlendMode::HardLight));
	this->addItem( tr("Soft Light"), QVariant::fromValue(BlendMode::Softlight));
	this->addItem( tr("Difference"), QVariant::fromValue(BlendMode::Difference));
	this->addItem( tr("Exclusion"), QVariant::fromValue(BlendMode::Exclusion));
	this->addItem( tr("Color Dodge"), QVariant::fromValue(BlendMode::ColorDodge));
	this->addItem( tr("Color Burn"), QVariant::fromValue(BlendMode::ColorBurn));
	this->addItem( tr("Hue"), QVariant::fromValue(BlendMode::Hue));
	this->addItem( tr("Saturation"), QVariant::fromValue(BlendMode::Saturation));
	this->addItem( tr("Color"), QVariant::fromValue(BlendMode::Color));
	this->addItem( tr("Luminosity"), QVariant::fromValue(BlendMode::Luminosity));

	this->setCurrentText( tr("Normal"));

	// TODO: Merge Cairo blend modes with image blend modes

	// blendModes.insert("norm", tr("Normal"));
	// blendModes.insert("dark", tr("Darken"));
	// blendModes.insert("lite", tr("Lighten"));
	// blendModes.insert("hue ", tr("Hue"));
	// blendModes.insert("sat ", tr("Saturation"));
	// blendModes.insert("colr", tr("Color"));
	// blendModes.insert("lum ", tr("Luminosity"));
	// blendModes.insert("mul ", tr("Multiply"));
	// blendModes.insert("scrn", tr("Screen"));
	// blendModes.insert("diss", tr("Dissolve"));
	// blendModes.insert("over", tr("Overlay"));
	// blendModes.insert("hLit", tr("Hard Light"));
	// blendModes.insert("sLit", tr("Soft Light"));
	// blendModes.insert("diff", tr("Difference"));
	// blendModes.insert("smud", tr("Exclusion"));
	// blendModes.insert("div ", tr("Color Dodge"));
	// blendModes.insert("idiv", tr("Color Burn"));
	// blendModes.insert("plus", tr("Plus"));
	// blendModes.insert("dsti", tr("Destination In"));
	// blendModes.insert("dsto", tr("Destination Out"));
}
