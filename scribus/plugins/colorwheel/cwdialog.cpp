/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QHeaderView>

#include "cmykfw.h"
#include "colorblind.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "cwdialog.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "propertiespalette.h"
#include "sccolorengine.h"
#include "util_color.h"


CWDialog::CWDialog(QWidget* parent, ScribusDoc* doc, const char* name, bool modal, Qt::WFlags fl)
	: QDialog (parent, fl),
	  m_Doc(doc)
{
	setupUi(this);
	setObjectName(name);
	setModal(modal);
	int h, s, v;
	ScColor color;
	QString colorName;
	connectSlots(false);
	// setup combobox
	typeCombo->addItem(colorWheel->getTypeDescription(colorWheel->Monochromatic), colorWheel->Monochromatic);
	typeCombo->addItem(colorWheel->getTypeDescription(colorWheel->Analogous), colorWheel->Analogous);
	typeCombo->addItem(colorWheel->getTypeDescription(colorWheel->Complementary), colorWheel->Complementary);
	typeCombo->addItem(colorWheel->getTypeDescription(colorWheel->Split), colorWheel->Split);
	typeCombo->addItem(colorWheel->getTypeDescription(colorWheel->Triadic), colorWheel->Triadic);
	typeCombo->addItem(colorWheel->getTypeDescription(colorWheel->Tetradic), colorWheel->Tetradic);
	// defects
	defectCombo->addItem(CommonStrings::trVisionNormal);
	defectCombo->addItem(CommonStrings::trVisionProtanopia);
	defectCombo->addItem(CommonStrings::trVisionDeuteranopia);
	defectCombo->addItem(CommonStrings::trVisionTritanopia);
	defectCombo->addItem(CommonStrings::trVisionFullColorBlind);
	// document colors
	documentColorList->updateBox(m_Doc->PageColors, ColorListBox::fancyPixmap);
	// preferences
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("colorwheel");
	typeCombo->setCurrentIndex(prefs->getInt("cw_type", 0));
	angleSpin->setValue(prefs->getInt("cw_angle", 15));
	colorWheel->currentDoc = m_Doc;
	colorWheel->angle = angleSpin->value();
	colorWheel->baseAngle = prefs->getInt("cw_baseangle", 0);
	colorspaceTab->setCurrentIndex(prefs->getInt("cw_space", 0));
	color.setNamedColor(prefs->get("cw_color", "#00000000"));
	// Handle color previously selected in the document tab
	if (colorspaceTab->currentWidget() == tabDocument)
	{
		colorName = prefs->get("cw_colorname", "");
		if (!colorName.isEmpty() && m_Doc->PageColors.contains(colorName))
			color = m_Doc->PageColors[colorName];
		else
			color.setColorRGB(0, 0, 0); //Trigger use of defaults
	}
	// Handle achromatic colors
	QColor rgb = ScColorEngine::getRGBColor(color, m_Doc);
	rgb.getHsv(&h, &s, &v);
	if (h == -1)
	{   // Reset to defaults
		colorWheel->baseAngle = 0;
		colorWheel->currentColorSpace = colorModelCMYK;
		colorWheel->actualColor = colorWheel->colorByAngle(0);
		colorspaceTab->setCurrentIndex(0);
	}
	else if (colorspaceTab->currentWidget() == tabDocument)
	{
		colorWheel->actualColor = color;
		QList<QListWidgetItem*> results = documentColorList->findItems(colorName, Qt::MatchFixedString|Qt::MatchCaseSensitive);
		if (results.count() > 0)
			documentColorList->setCurrentItem(results[0]);
	}
	else
		colorWheel->actualColor = color;

	resize(QSize(prefs->getInt("cw_width", 640),
		   prefs->getInt("cw_height", 480)).expandedTo(minimumSizeHint()));
	previewLabel->resize(prefs->getInt("cw_samplex", 300), prefs->getInt("cw_sampley", 100));
		
	// setup
	currentColorTable->horizontalHeader()->hide();
	colorspaceTab_currentChanged(colorspaceTab->currentIndex());

	// signals and slots that cannot be in ui file
	connect(colorWheel, SIGNAL(clicked(int, const QPoint&)),
			this, SLOT(colorWheel_clicked(int, const QPoint&)));
	connect(documentColorList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
			this, SLOT(documentColorList_currentChanged(QListWidgetItem *)));
	connect(colorList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
			this, SLOT(colorList_currentChanged(QListWidgetItem *)));
	connect(angleSpin, SIGNAL(valueChanged(int)),
			this, SLOT(angleSpin_valueChanged(int)));
	connect(colorspaceTab, SIGNAL(currentChanged(int)),
			this, SLOT(colorspaceTab_currentChanged(int)));
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(typeCombo_activated(int)));
	connect(defectCombo, SIGNAL(activated(int)), this, SLOT(defectCombo_activated(int)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addButton_clicked()));
	connect(replaceButton, SIGNAL(clicked()), this, SLOT(replaceButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_clicked()));

	connectSlots(true);
}

CWDialog::~CWDialog()
{
	// preferences
	QString colorName = (colorspaceTab->currentWidget() == tabDocument) ? documentColorList->currentColor() : "";
	prefs->set("cw_type", typeCombo->currentIndex());
	prefs->set("cw_angle", angleSpin->value());
	prefs->set("cw_baseangle", colorWheel->baseAngle);
	prefs->set("cw_color", colorWheel->actualColor.name());
	prefs->set("cw_colorname", colorName);
	prefs->set("cw_space", colorspaceTab->currentIndex());
	// GUI settings
	prefs->set("cw_width", width());
	prefs->set("cw_height", height());
	prefs->set("cw_samplex", previewLabel->width());
	prefs->set("cw_sampley", previewLabel->height());
}

void CWDialog::connectSlots(bool conn)
{
	if (conn)
	{
		connect( cSpin, SIGNAL( valueChanged(int) ), this, SLOT( cSpin_valueChanged(int) ) );
		connect( mSpin, SIGNAL( valueChanged(int) ), this, SLOT( mSpin_valueChanged(int) ) );
		connect( ySpin, SIGNAL( valueChanged(int) ), this, SLOT( ySpin_valueChanged(int) ) );
		connect( kSpin, SIGNAL( valueChanged(int) ), this, SLOT( kSpin_valueChanged(int) ) );
		connect( rSpin, SIGNAL( valueChanged(int) ), this, SLOT( rSpin_valueChanged(int) ) );
		connect( gSpin, SIGNAL( valueChanged(int) ), this, SLOT( gSpin_valueChanged(int) ) );
		connect( bSpin, SIGNAL( valueChanged(int) ), this, SLOT( bSpin_valueChanged(int) ) );
		connect( hSpin, SIGNAL( valueChanged(int) ), this, SLOT( hSpin_valueChanged(int) ) );
		connect( sSpin, SIGNAL( valueChanged(int) ), this, SLOT( sSpin_valueChanged(int) ) );
		connect( vSpin, SIGNAL( valueChanged(int) ), this, SLOT( vSpin_valueChanged(int) ) );
	}
	else
	{
		disconnect( cSpin, SIGNAL( valueChanged(int) ), this, SLOT( cSpin_valueChanged(int) ) );
		disconnect( mSpin, SIGNAL( valueChanged(int) ), this, SLOT( mSpin_valueChanged(int) ) );
		disconnect( ySpin, SIGNAL( valueChanged(int) ), this, SLOT( ySpin_valueChanged(int) ) );
		disconnect( kSpin, SIGNAL( valueChanged(int) ), this, SLOT( kSpin_valueChanged(int) ) );
		disconnect( rSpin, SIGNAL( valueChanged(int) ), this, SLOT( rSpin_valueChanged(int) ) );
		disconnect( gSpin, SIGNAL( valueChanged(int) ), this, SLOT( gSpin_valueChanged(int) ) );
		disconnect( bSpin, SIGNAL( valueChanged(int) ), this, SLOT( bSpin_valueChanged(int) ) );
		disconnect( hSpin, SIGNAL( valueChanged(int) ), this, SLOT( hSpin_valueChanged(int) ) );
		disconnect( sSpin, SIGNAL( valueChanged(int) ), this, SLOT( sSpin_valueChanged(int) ) );
		disconnect( vSpin, SIGNAL( valueChanged(int) ), this, SLOT( vSpin_valueChanged(int) ) );
	}
}

void CWDialog::documentColorList_currentChanged(QListWidgetItem *item)
{
	if (!item)
		return;
	ScColor c(m_Doc->PageColors[documentColorList->currentColor()]);
	colorWheel->currentColorSpace = c.getColorModel();
	setupColorComponents();
}

void CWDialog::colorspaceTab_currentChanged(int index)
{
	QWidget * tab = colorspaceTab->widget(index);
	if (tab == tabCMYK)
		colorWheel->currentColorSpace = colorModelCMYK;
	if (tab == tabRGB)
		colorWheel->currentColorSpace = colorModelRGB;
	if (tab == tabDocument)
	{
		if (!documentColorList->currentItem())
		{
			documentColorList->setCurrentItem(documentColorList->item(0));
			documentColorList->item(0)->setSelected(true);
		}
		documentColorList_currentChanged(documentColorList->currentItem());
	}
	processColors(typeCombo->currentIndex(), true);
}

void CWDialog::typeCombo_activated(int index)
{
	processColors(index, false);
}

void CWDialog::processColors(int index, bool updateSpins)
{
	bool angEnable = false;
	//colorList->clear();
	if (index == colorWheel->Monochromatic)
		colorWheel->currentType = colorWheel->Monochromatic;
	if (index == colorWheel->Analogous)
	{
		angEnable = true;
		colorWheel->currentType = colorWheel->Analogous;
	}
	if (index == colorWheel->Complementary)
		colorWheel->currentType = colorWheel->Complementary;
	if (index == colorWheel->Split)
	{
		angEnable = true;
		colorWheel->currentType = colorWheel->Split;
	}
	if (index == colorWheel->Triadic)
		colorWheel->currentType = colorWheel->Triadic;
	if (index == colorWheel->Tetradic)
	{
		angEnable = true;
		colorWheel->currentType = colorWheel->Tetradic;
	}
	angleSpin->setEnabled(angEnable);
	angleLabel->setEnabled(angEnable);
	colorWheel->makeColors();
	fillColorList();
	setPreview();
	if (updateSpins)
	{
// 		setupFromColor(colorWheel->actualColor);
		setupRGBComponent(colorWheel->actualColor);
		setupCMYKComponent(colorWheel->actualColor);
		setupHSVComponent(colorWheel->actualColor);
	}
	updateNamedLabels();
	QList<QListWidgetItem*> results = colorList->findItems(colorWheel->trBaseColor, Qt::MatchFixedString|Qt::MatchCaseSensitive);
	if (results.count() > 0)
		colorList_currentChanged(results[0]);
	colorWheel->update(); // force paint event
}

void CWDialog::colorWheel_clicked(int, const QPoint&)
{
	processColors(typeCombo->currentIndex(), true);
}

void CWDialog::angleSpin_valueChanged(int value)
{
	colorWheel->angle = value;
	processColors(typeCombo->currentIndex(), false);
}

void CWDialog::setPreview()
{
	int x = previewLabel->width();
	int y = previewLabel->height();
	QList<ScColor> cols = colorWheel->colorList.values();
	int xstep = x / cols.count();
	QPixmap pm = QPixmap(x, y);
	QPainter *p = new QPainter(&pm);
	QFontMetrics fm = p->fontMetrics();

	pm.fill(Qt::white);
	p->setPen(Qt::white);
	p->drawRect(0, 0, x, y);
	QColor c;
	for (int i = 0; i < cols.count(); ++i)
	{
		//c = computeDefect(cols[i].getRGBColor());
		c = computeDefect( ScColorEngine::getDisplayColor(cols[i], m_Doc) );
		p->setPen(c);
		p->setBrush(c);
		p->drawRect(i * xstep, 0, xstep, y);
	}
	p->setPen(Qt::black);
	p->setBrush(Qt::black);
	p->drawText(15, 5 + fm.height(), "Lorem ipsum dolor sit amet");
	p->setPen(Qt::white);
	p->setBrush(Qt::white);
	p->drawText(90, y - 5 - fm.height(), "Lorem ipsum dolor sit amet");
	p->end();
	delete(p);
	previewLabel->clear();
	previewLabel->setPixmap(pm);
}

QColor CWDialog::computeDefect(QColor c)
{
	if (defectCombo->currentIndex() == VisionDefectColor::normalVision)
		return c;
	VisionDefectColor *defect = new VisionDefectColor(c);
	defect->deficiency = defectCombo->currentIndex();
	defect->convertDefect();
	QColor nc = defect->getColor();
	delete defect;
	return nc;
}

void CWDialog::fillColorList()
{
	int ix = colorList->currentRow();
	colorList->updateBox(colorWheel->colorList, ColorListBox::fancyPixmap);
	QList<QListWidgetItem*> results = colorList->findItems(colorWheel->trBaseColor, Qt::MatchFixedString|Qt::MatchCaseSensitive);
	if (results.count() > 0)
	{
		QListWidgetItem *item = results[0];
		int row = colorList->row(item);
		if (row > 0)
		{
			colorList->takeItem(row);
			colorList->insertItem(0, item);
		}
	}
	colorList->setCurrentRow(ix > colorList->count() ? 0 : ix);
}

void CWDialog::defectCombo_activated(int)
{
	setPreview();
}

void CWDialog::addButton_clicked()
{
	QString status("<qt><h2>" + tr("Merging colors") + "</h2><p>");
	bool err = false;
	for (ColorList::iterator it = colorWheel->colorList.begin(); it != colorWheel->colorList.end(); ++it)
	{
		if (m_Doc->PageColors.contains(it.key()))
		{
			status += "<b>" + tr("Error: ") + "</b>" + tr("Color %1 exists already!").arg(it.key()) + "<br/>";
			err = true;
		}
		else
		{
			status += tr("Color %1 appended.").arg(it.key()) + "<br/>";
			m_Doc->PageColors[it.key()] = it.value();
		}
	}
	status += "<p>" + tr("Now opening the color manager.") + "</p></qt>";
	if (err)
	{
		QMessageBox::information(this, tr("Color Merging"), status);
		m_Doc->scMW()->slotEditColors();
		return;
	}
	m_Doc->scMW()->propertiesPalette->updateColorList();
	accept();
}

void CWDialog::replaceButton_clicked()
{
	for (ColorList::iterator it = colorWheel->colorList.begin(); it != colorWheel->colorList.end(); ++it)
	{
		m_Doc->PageColors[it.key()] = it.value();
	}
	m_Doc->scMW()->propertiesPalette->updateColorList();
	accept();
}

void CWDialog::cancelButton_clicked()
{
	reject();
}

void CWDialog::cSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::mSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::ySpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::kSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::rSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::gSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::bSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::hSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::sSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::vSpin_valueChanged( int )
{
	setupColorComponents();
}

void CWDialog::setupRGBComponent(ScColor col)
{
	RGBColor rgb;
	ScColorEngine::getRGBValues(col, m_Doc, rgb);
	connectSlots(false);
	rSpin->setValue(rgb.r);
	gSpin->setValue(rgb.g);
	bSpin->setValue(rgb.b);
	connectSlots(true);
}

void CWDialog::setupCMYKComponent(ScColor col)
{
	CMYKColor cmyk;
	ScColorEngine::getCMYKValues(col, m_Doc, cmyk);
	connectSlots(false);
	cSpin->setValue(qRound(cmyk.c / 2.55));
	mSpin->setValue(qRound(cmyk.m / 2.55));
	ySpin->setValue(qRound(cmyk.y / 2.55));
	kSpin->setValue(qRound(cmyk.k / 2.55));
	connectSlots(true);
}

void CWDialog::setupHSVComponent(ScColor col)
{
	int h, s, v;
	QColor qc(ScColorEngine::getRGBColor(col, m_Doc));
	qc.getHsv(&h, &s, &v);
	connectSlots(false);
	hSpin->setValue(h);
	sSpin->setValue(s);
	vSpin->setValue(v);
	connectSlots(true);
}

void CWDialog::setupColorComponents()
{
	ScColor c;
	if (colorspaceTab->currentWidget() == tabCMYK)
	{
		c = ScColor(qRound(cSpin->value() * 2.55), qRound(mSpin->value() * 2.55),
					qRound(ySpin->value() * 2.55), qRound(kSpin->value() * 2.55));
		c = ScColorEngine::convertToModel(c, m_Doc, colorModelCMYK);
		setupRGBComponent(c);
		setupHSVComponent(c);
	}
	if (colorspaceTab->currentWidget() == tabRGB)
	{
		c = ScColor(rSpin->value(), gSpin->value(), bSpin->value());
		c = ScColorEngine::convertToModel(c, m_Doc, colorModelRGB);
		setupCMYKComponent(c);
		setupHSVComponent(c);
	}
	if (colorspaceTab->currentWidget() == tabHSV)
	{
		QColor qc;
		qc.setHsv(hSpin->value(), sSpin->value(), vSpin->value());
		c.fromQColor(qc);
		c = ScColorEngine::convertToModel(c, m_Doc, colorModelRGB);
		setupCMYKComponent(c);
		setupRGBComponent(c);
	}
	if (colorspaceTab->currentWidget() == tabDocument)
	{
		c = m_Doc->PageColors[documentColorList->currentColor()];
		setupRGBComponent(c);
		setupCMYKComponent(c);
		setupHSVComponent(c);
	}

	if (colorWheel->recomputeColor(c))
		processColors(typeCombo->currentIndex(), false);
	else
	{
		colorList->clear();
		QMessageBox::information(this, windowTitle(),
								 "<qt>" + tr("Unable to find the requested color. "
										 "You have probably selected black, gray or white. "
										 "There is no way to process this color.") + "</qt>");
	}
	updateNamedLabels();
}

void CWDialog::updateNamedLabels()
{
	cmykLabel->setText(colorWheel->actualColor.nameCMYK(m_Doc));
	cmykLabel2->setText(colorWheel->actualColor.nameCMYK(m_Doc));
	rgbLabel->setText(colorWheel->actualColor.nameRGB(m_Doc));
	rgbLabel2->setText(colorWheel->actualColor.nameRGB(m_Doc));
	hsvLabel->setText(getHexHsv(colorWheel->actualColor));
	hsvLabel2->setText(getHexHsv(colorWheel->actualColor));
}

void CWDialog::colorList_currentChanged(QListWidgetItem * item)
{
	if (!item)
		return;

	// if it's base color we do not need to recompute it again
	if (item->text() == colorWheel->trBaseColor)
	{
		currentColorTable->setItem(0, 4, new QTableWidgetItem(cmykLabel->text()));
		currentColorTable->setItem(1, 4, new QTableWidgetItem(rgbLabel->text()));
		currentColorTable->setItem(2, 4, new QTableWidgetItem(hsvLabel->text()));
		currentColorTable->setItem(0, 0, new QTableWidgetItem(cSpin->text()));
		currentColorTable->setItem(0, 1, new QTableWidgetItem(mSpin->text()));
		currentColorTable->setItem(0, 2, new QTableWidgetItem(ySpin->text()));
		currentColorTable->setItem(0, 3, new QTableWidgetItem(kSpin->text()));
		currentColorTable->setItem(1, 0, new QTableWidgetItem(rSpin->text()));
		currentColorTable->setItem(1, 1, new QTableWidgetItem(gSpin->text()));
		currentColorTable->setItem(1, 2, new QTableWidgetItem(bSpin->text()));
		currentColorTable->setItem(2, 0, new QTableWidgetItem(hSpin->text()));
		currentColorTable->setItem(2, 1, new QTableWidgetItem(sSpin->text()));
		currentColorTable->setItem(2, 2, new QTableWidgetItem(vSpin->text()));
	}
	else
	{
		ScColor col(colorWheel->colorList[item->text()]);
		currentColorTable->setItem(0, 4, new QTableWidgetItem(col.nameCMYK(m_Doc)));
		currentColorTable->setItem(1, 4, new QTableWidgetItem(col.nameRGB(m_Doc)));
		currentColorTable->setItem(2, 4, new QTableWidgetItem(getHexHsv(col)));
		// components
		QString num;
		CMYKColor cmyk;
		ScColorEngine::getCMYKValues(col, m_Doc, cmyk);
		currentColorTable->setItem(0, 0, new QTableWidgetItem(num.setNum(cmyk.c)));
		currentColorTable->setItem(0, 1, new QTableWidgetItem(num.setNum(cmyk.m)));
		currentColorTable->setItem(0, 2, new QTableWidgetItem(num.setNum(cmyk.y)));
		currentColorTable->setItem(0, 3, new QTableWidgetItem(num.setNum(cmyk.k)));
		RGBColor rgb;
		ScColorEngine::getRGBValues(col, m_Doc, rgb);
		currentColorTable->setItem(1, 0, new QTableWidgetItem(num.setNum(rgb.r)));
		currentColorTable->setItem(1, 1, new QTableWidgetItem(num.setNum(rgb.g)));
		currentColorTable->setItem(1, 2, new QTableWidgetItem(num.setNum(rgb.b)));
		int h, s, v;
		QColor hsvCol(ScColorEngine::getRGBColor(col, m_Doc));
		hsvCol.getHsv(&h, &s, &v);
		currentColorTable->setItem(2, 0, new QTableWidgetItem(num.setNum(h)));
		currentColorTable->setItem(2, 1, new QTableWidgetItem(num.setNum(s)));
		currentColorTable->setItem(2, 2, new QTableWidgetItem(num.setNum(v)));
	}
	currentColorTable->resizeColumnsToContents();
}

QString CWDialog::getHexHsv(ScColor c)
{
	int h, s, v;
	QColor hsvCol(ScColorEngine::getRGBColor(c, m_Doc));
	hsvCol.getHsv(&h, &s, &v);
	return QString("#%1%2%3").arg(h, 0, 16).arg(s, 0, 16).arg(v, 0, 16);
}
