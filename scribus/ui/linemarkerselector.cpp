#include "linemarkerselector.h"
#include "ui/delegates/sclistitemdelegate.h"
#include "scribusapp.h"

/* ********************************************************************************* *
 *
 * Constructor + Setup
 *
 * ********************************************************************************* */

LineMarkerSelector::LineMarkerSelector(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);

	listMarker->setIconSize(QSize(60,48));
	listMarker->setGridSize(QSize(76,64));
	listMarker->setSpacing(4);
	listMarker->setItemDelegate(new ScListItemDelegate(QListWidget::IconMode, QSize(60,48)));

	connect(listMarker, &QListWidget::itemSelectionChanged, this, [this]() {
		emit markerChanged(marker());
	});
	connect(numberScale, &ScrSpinBox::valueChanged, this, [this]() {
		emit scaleChanged(scale());
	});

	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(redrawIcons()));

}

/* ********************************************************************************* *
 *
 * Methods
 *
 * ********************************************************************************* */

void LineMarkerSelector::rebuildList(const QList<ArrowDesc> *arrowStyles)
{
	// ArrowStyles defined in: void PrefsManager::initArrowStyles()
	int index = marker() == -1 ? 0 : marker();

	m_arrowStyles = arrowStyles;
	listMarker->clear();

	qreal m_devicePixelRatio = qApp->devicePixelRatio();

	int width = 60 * m_devicePixelRatio;
	int height = 48 * m_devicePixelRatio;
	int scale = 2 * m_devicePixelRatio;

	FPointArray path;
	QPixmap ico = renderPixmap(path, width, height, scale);

	addItem(ico, CommonStrings::tr_None, 0);

	for (int i = 0; i < arrowStyles->count(); ++i)
	{
		ico = renderPixmap(arrowStyles->at(i).points.copy(), width, height, scale);
		addItem(ico, arrowStyles->at(i).name, i+1);
	}

	setMarker(index);
}

void LineMarkerSelector::addItem(QPixmap pixmap, const QString &label, int id)
{
	QListWidgetItem *item = new QListWidgetItem();
	item->setIcon(pixmap);
	item->setText("");
	item->setData(arrow, id);
	listMarker->addItem(item);
}

void LineMarkerSelector::redrawIcons()
{
	if (m_arrowStyles->empty())
		return;

	rebuildList(m_arrowStyles);
}

/* ********************************************************************************* *
 *
 * Properties
 *
 * ********************************************************************************* */

void LineMarkerSelector::setArrowDirection(ArrowDirection direction)
{
	m_arrowDirection = direction;
}

void LineMarkerSelector::languageChange()
{
	QString pctSuffix = tr(" %");
	setScaleSuffix(pctSuffix);

	numberScale->setToolTip( tr("Arrow head scale of line"));
}

double LineMarkerSelector::scale() const
{
	return numberScale->value();
}

void LineMarkerSelector::setScale(double value)
{
	numberScale->setValue(value);
}

int LineMarkerSelector::marker() const
{
	if (listMarker->currentItem() != nullptr)
		return listMarker->currentItem()->data(arrow).toInt();
	return -1;
}

void LineMarkerSelector::setMarker(int id)
{
	for (int i = 0; i < listMarker->count(); ++i)
	{
		QListWidgetItem* item = listMarker->item(i);

		if(item->data(arrow) == id)
		{
			listMarker->setCurrentItem(item);
			break;
		}
	}

}

QString LineMarkerSelector::scaleSuffix() const
{
	return numberScale->suffix();
}

void LineMarkerSelector::setScaleSuffix(const QString suffix)
{
	numberScale->setSuffix(suffix);
}

QPixmap LineMarkerSelector::renderPixmap(FPointArray path, int width, int height, int scale)
{
	const QPalette& pal = ScQApp->palette();
	QColor textColor = pal.color(QPalette::Active, QPalette::Text);

	int offsetX = 0, offsetY = 0, lineStart = 0, lineEnd = 0;
	int vCenter = height / 2;
	int flipScale = scale;

	if (m_arrowDirection == ArrowDirection::StartArrow)
	{
		flipScale = -flipScale;
		lineStart = width * .5;
		lineEnd = width;
		offsetX = -lineStart / scale;
		offsetY = vCenter / scale;
	}
	else
	{
		lineStart = 0;
		lineEnd = width * .5;
		offsetX = lineEnd / scale;
		offsetY = vCenter / scale;
	}

	QImage image(width, height, QImage::Format_ARGB32_Premultiplied);
	image.fill(0);

	auto *painter = new ScPainter(&image, width, height);
	painter->setStrokeMode(ScPainter::Solid); // solid line

	// paint line
	painter->setPen(textColor, 2 * scale, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
	painter->setPenOpacity(0.5);
	painter->drawLine(FPoint(lineStart, vCenter), FPoint(lineEnd, vCenter));

	// paint head
	if (!path.isEmpty())
	{
		painter->setBrush(textColor);
		painter->setPen(textColor, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		painter->setPenOpacity(1.0);
		painter->setFillMode(ScPainter::Solid);
		QTransform mm;
		mm.scale(flipScale, scale);
		mm.translate(offsetX, offsetY);
		path.map(mm);
		painter->setupPolygon(&path);
		painter->drawPolygon();
		painter->drawPolyLine();
	}

	painter->end();
	delete painter;

	return QPixmap::fromImage(image);
}

QIcon LineMarkerSelector::currentIcon() const
{
	QIcon ico;

	if (listMarker->currentItem() != nullptr)
		ico = listMarker->currentItem()->icon();

	return ico;
}


