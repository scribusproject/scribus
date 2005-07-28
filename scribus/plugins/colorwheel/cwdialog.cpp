/* $Id$ */
#include "cwdialog.h"
#include "cwdialog.moc"

#include <qvariant.h>
#include <qcombobox.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qpainter.h>

#include "prefsmanager.h"
#include "prefsfile.h"
#include "mpalette.h"

extern ScribusApp *ScApp;

ColorWheelDialog::ColorWheelDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog(parent, name, modal, fl)
{
	if (!name)
		setName("ColorWheelDialog");

	formLayout = new QGridLayout(this, 1, 1, 11, 6, "formLayout");
	mainLayout = new QHBoxLayout(0, 0, 6, "mainLayout");
	wheelLayout = new QVBoxLayout(0, 0, 6, "wheelLayout");

	colorWheel = new ColorWheel(this, "colorWheel");
	colorWheel->setFrameShape(QFrame::Box);
	colorWheel->setMinimumSize(QSize(300, 300));
	colorWheel->setMaximumSize(QSize(300, 300));
	wheelLayout->addWidget(colorWheel);

	previewLabel = new QLabel(this, "previewLabel");
	previewLabel->setFrameShape(QFrame::Box);
	previewLabel->setMinimumSize(QSize(300, 120));
	previewLabel->setMaximumSize(QSize(300, 120));
	wheelLayout->addWidget(previewLabel);

	spacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	wheelLayout->addItem(spacer1);
	mainLayout->addLayout(wheelLayout);

	listLayout = new QVBoxLayout(0, 0, 6, "listLayout");

	typeLabel = new QLabel(this, "typeLabel");
	listLayout->addWidget(typeLabel);
	typeCombo = new QComboBox(false, this, "typeCombo");
	listLayout->addWidget(typeCombo);

	angleLabel = new QLabel(this, "angleLabel");
	angleLayout = new QHBoxLayout(0, 0, 6, "angleLayout");
	angleLayout->addWidget(angleLabel);

	angleSpin = new QSpinBox(this, "angleSpin");
	angleSpin->setValue(15);
	angleSpin->setMinValue(0);
	angleSpin->setMaxValue(90);
	angleLayout->addWidget(angleSpin);
	listLayout->addLayout(angleLayout);

	colorList = new QListView(this, "colorList");
	listLayout->addWidget(colorList);

	buttonLayout = new QHBoxLayout(0, 0, 6, "buttonLayout");
	addButton = new QPushButton(this, "addButton");
	buttonLayout->addWidget(addButton);
	replaceButton = new QPushButton(this, "replaceButton");
	buttonLayout->addWidget(replaceButton);
	cancelButton = new QPushButton(this, "cancelButton");
	buttonLayout->addWidget(cancelButton);

	listLayout->addLayout(buttonLayout);
	mainLayout->addLayout(listLayout);
	formLayout->addLayout(mainLayout, 0, 0);

	languageChange();
	resize(QSize(600, 480).expandedTo(minimumSizeHint()));
	clearWState(WState_Polished);

	// setup combobox
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Monochromatic), colorWheel->Monochromatic);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Analogous), colorWheel->Analogous);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Complementary), colorWheel->Complementary);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Split), colorWheel->Split);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Triadic), colorWheel->Triadic);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Tetradic), colorWheel->Tetradic);

	// preferences
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("colorwheel");
	typeCombo->setCurrentItem(prefs->getInt("cw_type", 0));
	angleSpin->setValue(prefs->getInt("cw_angle", 15));
	colorWheel->angle = angleSpin->value();
	QValueVector<QPoint> vp;
	int x = prefs->getInt("cw_x", colorWheel->width()/2 + 1);
	int y = prefs->getInt("cw_y", colorWheel->height()/2);
	vp.append(QPoint(x, y));
	colorWheel->actualPoint = QPoint(x, y);
	colorWheel->actualRgb = QColor(prefs->getInt("cw_r", 0), prefs->getInt("cw_g", 0), prefs->getInt("cw_b", 0)).rgb();
	colorWheel->paintWheel(vp);

	// actions
	typeCombo_activated(typeCombo->currentItem());
	// signals and slots connections
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(typeCombo_activated(int)));
	connect(colorWheel, SIGNAL(clicked(int, const QPoint&)), this, SLOT(colorWheel_clicked(int, const QPoint&)));
	colorWheel_clicked(0, QPoint(0, 0));
	connect(angleSpin, SIGNAL(valueChanged(int)), this, SLOT(angleSpin_valueChanged(int)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addButton_clicked()));
	connect(replaceButton, SIGNAL(clicked()), this, SLOT(replaceButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_clicked()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
ColorWheelDialog::~ColorWheelDialog()
{
	// no need to delete child widgets, Qt does it all for us
	// preferences
	prefs->set("cw_type", typeCombo->currentItem());
	prefs->set("cw_angle", angleSpin->value());
	prefs->set("cw_x", colorWheel->actualPoint.x());
	prefs->set("cw_y", colorWheel->actualPoint.y());
	prefs->set("cw_r", qRed(colorWheel->actualRgb));
	prefs->set("cw_g", qGreen(colorWheel->actualRgb));
	prefs->set("cw_b", qBlue(colorWheel->actualRgb));
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ColorWheelDialog::languageChange()
{
	setCaption(tr("Color Wheel"));
	colorList->addColumn(tr("Color"));
	colorList->addColumn(tr("Name"));
	colorList->addColumn(tr("C"));
	colorList->addColumn(tr("M"));
	colorList->addColumn(tr("Y"));
	colorList->addColumn(tr("K"));
	colorList->setSorting(1);
	typeLabel->setText(tr("Select Method:"));
	angleLabel->setText(tr("Angle (0 - 90 degrees):"));
	addButton->setText(tr("&Merge Colors"));
	replaceButton->setText(tr("&Replace Colors"));
	cancelButton->setText(tr("&Cancel"));
	// tips
	QToolTip::add(addButton, "<qt>" + tr("Merge created colors into the document colors") + "</qt>");
	QToolTip::add(addButton, "<qt>" + tr("Replace created colors in the document colors") + "</qt>");
	QToolTip::add(cancelButton, "<qt>" + tr("Leave colors untouched") + "</qt>");
	QToolTip::add(angleSpin, "<qt>" + tr("Difference between selected value and counted ones. See documentation for more info") + "</qt>");
	QToolTip::add(colorWheel, "<qt>" + tr("Click the wheel to get base color") + "</qt>");
	QToolTip::add(previewLabel, "<qt>" + tr("Here you have the sample color schema") + "</qt>");
	QToolTip::add(typeCombo, "<qt>" + tr("Select one of the method to create color schema. See documentation for more info") + "</qt>");
	QToolTip::add(colorList, "<qt>" + tr("Here you have the color of your chosen color schema") + "</qt>");
}

void ColorWheelDialog::fillColorList()
{
	colorList->clear();
	for (ColorList::iterator it = colorWheel->colorList.begin(); it != colorWheel->colorList.end(); ++it)
	{
		int c, m, y, k;
		QListViewItem *item = new QListViewItem(colorList);
		item->setPixmap(0, colorWheel->sample(it.data().getRGBColor()));
		item->setText(1, it.key());
		it.data().getCMYK(&c, &m, &y, &k);
		item->setText(2, QString("%1").arg(c));
		item->setText(3, QString("%1").arg(m));
		item->setText(4, QString("%1").arg(y));
		item->setText(5, QString("%1").arg(k));
	}
}

void ColorWheelDialog::typeCombo_activated(int index)
{
	colorList->clear();
	if (index == colorWheel->Monochromatic)
	{
		angleSpin->setEnabled(false);
		angleLabel->setEnabled(false);
		colorWheel->makeMonochromatic();
	}
	if (index == colorWheel->Analogous)
	{
		angleSpin->setEnabled(true);
		angleLabel->setEnabled(true);
		colorWheel->makeAnalogous();
	}
	if (index == colorWheel->Complementary)
	{
		angleSpin->setEnabled(false);
		angleLabel->setEnabled(false);
		colorWheel->makeComplementary();
	}
	if (index == colorWheel->Split)
	{
		angleSpin->setEnabled(true);
		angleLabel->setEnabled(true);
		colorWheel->makeSplit();
	}
	if (index == colorWheel->Triadic)
	{
		angleSpin->setEnabled(false); //TODO
		angleLabel->setEnabled(false);
		colorWheel->makeTriadic();
	}
	if (index == colorWheel->Tetradic)
	{
		angleSpin->setEnabled(true);
		angleLabel->setEnabled(true);
		colorWheel->makeTetradic();
	}
	fillColorList();
	setPreview();
}

void ColorWheelDialog::colorWheel_clicked(int, const QPoint&)
{
	typeCombo_activated(typeCombo->currentItem());
}

void ColorWheelDialog::angleSpin_valueChanged(int value)
{
	colorWheel->angle = value;
	typeCombo_activated(typeCombo->currentItem());
}

void ColorWheelDialog::addButton_clicked()
{
	QString status("<qt><h2>" + tr("Merging colors") + "</h2><p>");
	bool err = false;
	for (ColorList::iterator it = colorWheel->colorList.begin(); it != colorWheel->colorList.end(); ++it)
	{
		if (ScApp->doc->PageColors.contains(it.key()))
		{
			status += "<b>" + tr("Error: ") + "</b>" + tr(QString("Color %1 exists already!").arg(it.key())) + "<br/>";
			err = true;
		}
		else
		{
			status += tr(QString("Color %1 appended.").arg(it.key())) + "<br/>";
			ScApp->doc->PageColors[it.key()] = it.data();
		}
	}
	status += "<p>" + tr("Now opening the color manager.") + "</p></qt>";
	if (err)
	{
		QMessageBox::information(this, tr("Color Merging"), status);
		ScApp->slotEditColors();
		return;
	}
	ScApp->propertiesPalette->Cpal->SetColors(ScApp->doc->PageColors);
	ScApp->propertiesPalette->updateCList();
	accept();
}

void ColorWheelDialog::replaceButton_clicked()
{
	for (ColorList::iterator it = colorWheel->colorList.begin(); it != colorWheel->colorList.end(); ++it)
	{
		ScApp->doc->PageColors[it.key()] = it.data();
	}
	ScApp->propertiesPalette->Cpal->SetColors(ScApp->doc->PageColors);
	ScApp->propertiesPalette->updateCList();
	accept();
}

void ColorWheelDialog::cancelButton_clicked()
{
	reject();
}

void ColorWheelDialog::setPreview()
{
	int x = previewLabel->width();
	int y = previewLabel->height();
	QValueList<ScColor> cols = colorWheel->colorList.values();
	int xstep = x / cols.count();
	QPixmap pm = QPixmap(x, y);
	QPainter *p = new QPainter(&pm);
	QFontMetrics fm = p->fontMetrics();

	pm.fill(Qt::white);
	p->setPen(Qt::white);
	p->drawRect(0, 0, x, y);
	for (uint i = 0; i < cols.count(); ++i)
	{
		p->setPen(cols[i].getRGBColor());
		p->setBrush(cols[i].getRGBColor());
		p->drawRect(i * xstep, 0, xstep, y);
	}
	p->setPen(Qt::black);
	p->setBrush(Qt::black);
	p->drawText(15, 5 + fm.height(), "Lorem ipsum dolor sit amet");
	p->setPen(Qt::white);
	p->setBrush(Qt::white);
	p->drawText(25, y - 5 - fm.height(), "Lorem ipsum dolor sit amet");
	p->end();
	delete(p);
	previewLabel->clear();
	previewLabel->setPixmap(pm);
}
