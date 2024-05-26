#include "colorpicker_color_swatches.h"

ColorPickerColorSwatches::ColorPickerColorSwatches(QWidget *parent) : QWidget(parent)
{
	setupUi(this);

	listSwatches->setPixmapType(ColorListBox::fancyPixmap);

	connect(listSwatches, &ColorListBox::currentTextChanged, this, &ColorPickerColorSwatches::setColorFromWidget);
}

QString ColorPickerColorSwatches::currentColor() const
{
	return listSwatches->currentColor();
}

void ColorPickerColorSwatches::setColors(const ColorList &list, bool insertNone)
{
	ColorListModel* clm = qobject_cast<ColorListModel*>(listSwatches->model());
	if (!clm)
		return;

	clm->setColorList(list, insertNone);
}

void ColorPickerColorSwatches::setCurrentColor(QString colorName)
{
	if (colorName == CommonStrings::tr_NoneColor || colorName == CommonStrings::None)
		colorName = CommonStrings::tr_NoneColor;

	// update color list
	QSignalBlocker sig(listSwatches);
	listSwatches->setCurrentColor(colorName);

}

void ColorPickerColorSwatches::setColorFromWidget(QString colorName)
{
	emit colorChanged();
}

