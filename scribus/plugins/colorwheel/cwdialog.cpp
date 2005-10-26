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
#include <qmenubar.h>
#include <qgroupbox.h>
#include <qslider.h>

#include "prefsmanager.h"
#include "commonstrings.h"
#include "cmykfw.h"
#include "prefsfile.h"
#include "mpalette.h"
#include "colorblind.h"
#include "cwsetcolor.h"


extern ScribusApp SCRIBUS_API *ScApp;

ScribusColorList::ScribusColorList(QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog(parent, name, modal, fl)
{
	if (!name)
		setName("ScribusColorList");
	ScribusColorListLayout = new QGridLayout(this, 1, 1, 11, 6, "ScribusColorListLayout");

	listLayout = new QVBoxLayout(0, 0, 6, "listLayout");

	listView = new QListView(this, "listView");
	listView->setAllColumnsShowFocus(true);
	listView->addColumn(tr("Sample"));
	listView->addColumn(tr("Color"));
	listLayout->addWidget(listView);

	btnLayout = new QHBoxLayout(0, 0, 6, "btnLayout");
	btnSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	btnLayout->addItem(btnSpacer);

	okButton = new QPushButton(this, "okButton");
	btnLayout->addWidget(okButton);

	cancelButton = new QPushButton(this, "cancelButton");
	btnLayout->addWidget(cancelButton);
	listLayout->addLayout(btnLayout);

	ScribusColorListLayout->addLayout(listLayout, 0, 0);
	languageChange();
	resize(QSize(288, 310).expandedTo(minimumSizeHint()));
	clearWState(WState_Polished);

	ColorList::Iterator it;
	PrefsManager *prefsManager = PrefsManager::instance();
	ColorList clist = prefsManager->colorSet();
	// only for sample() method
	ColorWheel *cw = new ColorWheel(this, "cw");
	for (it = clist.begin(); it != clist.end(); ++it)
	{
		if (it.key() != "None" && it.key() != tr("None"))
		{
			ScColor col = clist[it.key()];
			QPixmap pm = cw->sample(col.getRGBColor());
			QListViewItem *item = new QListViewItem(listView);
			item->setPixmap(0, pm);
			item->setText(1, it.key());
			listView->insertItem(item);
		}
	}
	delete cw;

	connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void ScribusColorList::languageChange()
{
	setCaption(tr("Document Colors"));
	okButton->setText(CommonStrings::tr_OK);
	cancelButton->setText(CommonStrings::tr_Cancel);
}

void ScribusColorList::okButton_clicked()
{
	PrefsManager *prefsManager = PrefsManager::instance();
	ScColor c = prefsManager->colorSet()[listView->currentItem()->text(1)];
	selectedColor = c.getRGBColor();
	accept();
}


ColorWheelDialog::ColorWheelDialog(QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog(parent, name, modal, fl)
{
	if (!name)
		setName("ColorWheelDialog");

	QMenuBar *menuBar = new QMenuBar(this, "menuBar");
	QPopupMenu *colorMenu = new QPopupMenu(this);
	colorMenu->insertItem(tr("Cr&eate color..."), this, SLOT(createColor()));
	colorMenu->insertItem(tr("C&olor Components..."), this, SLOT(setColorComponents()));
	colorMenu->insertItem(tr("&Import existing color..."), this, SLOT(importColor()));
	colorMenu->insertSeparator();
	colorMenu->insertItem(tr("&Merge colors"), this, SLOT(addButton_clicked()));
	colorMenu->insertItem(tr("&Replace colors"), this, SLOT(replaceButton_clicked()));
	colorMenu->insertItem(tr("E&xit"), this, SLOT(cancelButton_clicked()));
	menuBar->insertItem(tr("C&olor"), colorMenu);

	formLayout = new QGridLayout(this, 1, 1, 11, 6, "formLayout");
	formLayout->setMenuBar(menuBar);
	mainLayout = new QHBoxLayout(0, 0, 6, "mainLayout");
	wheelLayout = new QVBoxLayout(0, 0, 6, "wheelLayout");

	colorWheel = new ColorWheel(this, "colorWheel");
	colorWheel->setFrameShape(QFrame::Box);
	colorWheel->setMinimumSize(QSize(300, 300));
	colorWheel->setMaximumSize(QSize(300, 300));
	wheelLayout->addWidget(colorWheel);

	typeLabel = new QLabel(this, "typeLabel");
	wheelLayout->addWidget(typeLabel);
	typeCombo = new QComboBox(false, this, "typeCombo");
	wheelLayout->addWidget(typeCombo);

	angleLabel = new QLabel(this, "angleLabel");
	angleLayout = new QHBoxLayout(0, 0, 6, "angleLayout");
	angleLayout->addWidget(angleLabel);
	angleSpin = new QSpinBox(this, "angleSpin");
	angleSpin->setMinValue(0);
	angleSpin->setMaxValue(90);
	angleLayout->addWidget(angleSpin);
	wheelLayout->addLayout(angleLayout);

	spacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	wheelLayout->addItem(spacer1);
	mainLayout->addLayout(wheelLayout);

	listLayout = new QVBoxLayout(0, 0, 6, "listLayout");

	defectLayout = new QHBoxLayout(0, 0, 6, "defectLayout");
	defectLabel = new QLabel(this, "defectLabel");
	defectLayout->addWidget(defectLabel);
	defectCombo = new QComboBox(false, this, "defectCombo");
	defectLayout->addWidget(defectCombo);
	listLayout->addLayout(defectLayout);

	previewLabel = new QLabel(this, "previewLabel");
	previewLabel->setFrameShape(QFrame::Box);
	previewLabel->setMinimumSize(QSize(400, 160));
	previewLabel->setMaximumSize(QSize(400, 160));
	listLayout->addWidget(previewLabel);

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
	// defects
	defectCombo->insertItem(tr("Normal Vision"));
	defectCombo->insertItem(tr("Protanopia (Red)"));
	defectCombo->insertItem(tr("Deuteranopia (Green)"));
	defectCombo->insertItem(tr("Tritanopia (Blue)"));
	defectCombo->insertItem(tr("Full Color Blindness"));

	// preferences
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("colorwheel");
	typeCombo->setCurrentItem(prefs->getInt("cw_type", 0));
	angleSpin->setValue(prefs->getInt("cw_angle", 15));
	colorWheel->angle = angleSpin->value();
	colorWheel->baseAngle = prefs->getInt("cw_baseangle", 0);
	colorWheel->actualColor = QColor(prefs->getInt("cw_r", 0), prefs->getInt("cw_g", 0), prefs->getInt("cw_b", 0));
	colorWheel->paintWheel();

	// actions
	typeCombo_activated(typeCombo->currentItem());

	// signals and slots connections
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(typeCombo_activated(int)));
	connect(defectCombo, SIGNAL(activated(int)), this, SLOT(defectCombo_activated(int)));
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
	prefs->set("cw_baseangle", colorWheel->baseAngle);
	prefs->set("cw_r", colorWheel->actualColor.red());
	prefs->set("cw_g", colorWheel->actualColor.green());
	prefs->set("cw_b", colorWheel->actualColor.blue());
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ColorWheelDialog::languageChange()
{
	defectLabel->setText(tr("Vision Defect:"));
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
	cancelButton->setText(CommonStrings::tr_Cancel);
	// tips
	QToolTip::add(addButton, "<qt>" + tr("Merge created colors into the document colors") + "</qt>");
	QToolTip::add(replaceButton, "<qt>" + tr("Replace created colors in the document colors") + "</qt>");
	QToolTip::add(cancelButton, "<qt>" + tr("Leave colors untouched") + "</qt>");
	QToolTip::add(angleSpin, "<qt>" + tr("Difference between the selected value and the counted ones. Refer to documentation for more information.") + "</qt>");
	QToolTip::add(colorWheel, "<qt>" + tr("Click the wheel to get the base color. It is hue in HSV mode.") + "</qt>");
	QToolTip::add(previewLabel, "<qt>" + tr("Sample color scheme") + "</qt>");
	QToolTip::add(typeCombo, "<qt>" + tr("Select one of the methods to create a color scheme. Refer to documentation for more information.") + "</qt>");
	QToolTip::add(colorList, "<qt>" + tr("Colors of your chosen color scheme") + "</qt>");
	QToolTip::add(defectCombo, "<qt>" + tr("Simulate common vision defects here. Select type of the defect.") + "</qt>");
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
		angleSpin->setEnabled(false);
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
			status += "<b>" + tr("Error: ") + "</b>" + tr("Color %1 exists already!").arg(it.key()) + "<br/>";
			err = true;
		}
		else
		{
			status += tr("Color %1 appended.").arg(it.key()) + "<br/>";
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
	ScApp->propertiesPalette->updateColorList();
	ScApp->propertiesPalette->updateCList();
	accept();
}

void ColorWheelDialog::replaceButton_clicked()
{
	for (ColorList::iterator it = colorWheel->colorList.begin(); it != colorWheel->colorList.end(); ++it)
	{
		ScApp->doc->PageColors[it.key()] = it.data();
	}
	ScApp->propertiesPalette->updateColorList();
	ScApp->propertiesPalette->updateCList();
	accept();
}

void ColorWheelDialog::cancelButton_clicked()
{
	reject();
}

void ColorWheelDialog::defectCombo_activated(int /*index*/)
{
	setPreview();
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
		QColor c = computeDefect(cols[i].getRGBColor());
		p->setPen(c);
		p->setBrush(c);
		p->drawRect(i * xstep, 0, xstep, y);
	}
	p->setPen(Qt::black);
	p->setBrush(Qt::black);
	p->drawText(15, 5 + fm.height(), "Lorem ipsum dolor sit amet");
	p->setPen(Qt::white);
	p->setBrush(Qt::white);
	p->drawText(125, y - 5 - fm.height(), "Lorem ipsum dolor sit amet");
	p->end();
	delete(p);
	previewLabel->clear();
	previewLabel->setPixmap(pm);
}

QColor ColorWheelDialog::computeDefect(QColor c)
{
	if (defectCombo->currentItem() == VisionDefectColor::normalVision)
		return c;
	VisionDefectColor *defect = new VisionDefectColor(c);
	defect->deficiency = defectCombo->currentItem();
	defect->convertDefect();
	QColor nc = defect->getColor();
	delete defect;
	return nc;
}

void ColorWheelDialog::createColor()
{
	ScColor beginColor;
	/* these 2 variables are defined for CMYKChoose constructor.
	No other need to use them. */
	ColorList tmpcl;
	QStringList tmpsl;

	beginColor.fromQColor(colorWheel->actualColor);
	CMYKChoose* dia = new CMYKChoose(this, beginColor, tr("New Color"), &tmpcl, tmpsl);
	if (dia->exec())
	{
		userColorInput(dia->Farbe.getRGBColor());
	}
	delete dia;
}

void ColorWheelDialog::setColorComponents()
{
	CwSetColor *dia = new CwSetColor(colorWheel->actualColor, this);
	if (dia->exec())
	{
		userColorInput(dia->newColor);
	}
	delete dia;
}

void ColorWheelDialog::importColor()
{
	ScribusColorList *dia = new ScribusColorList(this, "dia", true, 0);
	if (dia->exec())
	{
		userColorInput(dia->selectedColor);
	}
	delete dia;
}

void ColorWheelDialog::userColorInput(QColor c)
{
	if (colorWheel->recomputeColor(c))
		typeCombo_activated(typeCombo->currentItem());
	else
		QMessageBox::information(this, caption(),
				"<qt>" + tr("Unable to find the requested color. "
							"You have probably selected black, gray or white. "
							"There is no way to process this color.") + "</qt>");
}
