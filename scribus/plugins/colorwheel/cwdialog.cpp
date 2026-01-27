/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "cwdialog.h"

#include <QHeaderView>
#include <QMessageBox>

#include "colorblind.h"
#include "commonstrings.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "sccolorengine.h"
#include "scribus.h"
#include "ui/cmykfw.h"
#include "ui/colorlistbox.h"
#include "ui/contentpalette.h"
#include "ui/propertiespalette.h"
#include "util_color.h"



CWDialog::CWDialog(QWidget* parent, ScribusDoc* doc, const char* name, bool modal)
	: QDialog (parent),
	  m_Doc(doc)
{
	setupUi(this);
	setObjectName(name);
	setModal(modal);
	ScColor color;
	QString colorName;
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
	documentColorList->setPixmapType(ColorListBox::fancyPixmap);
	documentColorList->setColors(m_Doc->PageColors, false);
	// preferences
	prefs = PrefsManager::instance().prefsFile->getPluginContext("colorwheel");
	typeCombo->setCurrentIndex(prefs->getInt("cw_type", 0));
	angleSpin->setValue(prefs->getInt("cw_angle", 15));
	colorList->setPixmapType(ColorListBox::fancyPixmap);
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
			color.setRgbColor(0, 0, 0); //Trigger use of defaults
	}
	// Handle achromatic colors
	QColor rgb = ScColorEngine::getRGBColor(color, m_Doc);
	int h, s, v;
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
		QStringList results = documentColorList->findColors(colorName, Qt::MatchFixedString|Qt::MatchCaseSensitive);
		if (results.count() > 0)
			documentColorList->setCurrentColor(results[0]);
	}
	else
		colorWheel->actualColor = color;

	resize(QSize(prefs->getInt("cw_width", 640),
		   prefs->getInt("cw_height", 480)).expandedTo(minimumSizeHint()));
	previewLabel->resize(prefs->getInt("cw_samplex", 300), prefs->getInt("cw_sampley", 100));

	for (int i = 0; i < currentColorTable->rowCount(); i++)
	{
		for (int j = 0; j < currentColorTable->columnCount(); j++)
		{
			auto item = new QTableWidgetItem();
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
			currentColorTable->setItem(i, j, item);
		}
	}
		
	// setup
	colorspaceTab_currentChanged(colorspaceTab->currentIndex());

	// signals and slots that cannot be in ui file
	connect(colorWheel, SIGNAL(clicked(int,QPoint)),this, SLOT(colorWheel_clicked(int,QPoint)));
	connect(documentColorList, SIGNAL(currentTextChanged(QString)), this, SLOT(documentColorList_currentChanged(QString)));
	connect(colorList, SIGNAL(currentTextChanged(QString)), this, SLOT(colorList_currentChanged(QString)));
	connect(angleSpin, SIGNAL(valueChanged(int)), this, SLOT(angleSpin_valueChanged(int)));
	connect(colorspaceTab, SIGNAL(currentChanged(int)), this, SLOT(colorspaceTab_currentChanged(int)));
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(typeCombo_activated(int)));
	connect(defectCombo, SIGNAL(activated(int)), this, SLOT(defectCombo_activated(int)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addButton_clicked()));
	connect(replaceButton, SIGNAL(clicked()), this, SLOT(replaceButton_clicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_clicked()));

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

void CWDialog::blockSpinSignals(bool block)
{
	cSpin->blockSignals(block);
	mSpin->blockSignals(block);
	ySpin->blockSignals(block);
	kSpin->blockSignals(block);
	rSpin->blockSignals(block);
	gSpin->blockSignals(block);
	bSpin->blockSignals(block);
	hSpin->blockSignals(block);
	sSpin->blockSignals(block);
	vSpin->blockSignals(block);
}

void CWDialog::documentColorList_currentChanged(const QString& itemText)
{
	if (itemText.isEmpty())
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
		if (!documentColorList->hasSelection())
			documentColorList->setCurrentRow(0);
		documentColorList_currentChanged(documentColorList->currentColor());
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
	QStringList results = colorList->findColors(colorWheel->trBaseColor, Qt::MatchFixedString|Qt::MatchCaseSensitive);
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
	QPixmap pm = QPixmap(x * devicePixelRatioF(), y * devicePixelRatioF());
	pm.setDevicePixelRatio(devicePixelRatioF());
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
	VisionDefectColor defect(c);
	defect.deficiency = defectCombo->currentIndex();
	defect.convertDefect();
	QColor nc = defect.getColor();
	return nc;
}

void CWDialog::fillColorList()
{
	int ix = colorList->currentRow();
	colorList->updateBox(colorWheel->colorList);
	QStringList results = colorList->findColors(colorWheel->trBaseColor, Qt::MatchFixedString|Qt::MatchCaseSensitive);
	if (results.count() > 0)
	{
		QString itemColor = results[0];
		int row = colorList->row(itemColor);
		if (row > 0)
		{
			ScColor color = colorWheel->colorList.value(itemColor);
			colorList->removeItem(row);
			colorList->insertItem(0, color, itemColor);
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
	for (auto it = colorWheel->colorList.cbegin(); it != colorWheel->colorList.cend(); ++it)
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
		ScMessageBox::information(this, tr("Color Merging"), status);
		m_Doc->scMW()->manageColorsAndFills();
		return;
	}
	m_Doc->scMW()->propertiesPalette->updateColorList();
	m_Doc->scMW()->contentPalette->updateColorList();
	accept();
}

void CWDialog::replaceButton_clicked()
{
	for (ColorList::iterator it = colorWheel->colorList.begin(); it != colorWheel->colorList.end(); ++it)
	{
		m_Doc->PageColors[it.key()] = it.value();
	}
	m_Doc->scMW()->propertiesPalette->updateColorList();
	m_Doc->scMW()->contentPalette->updateColorList();
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

void CWDialog::setupRGBComponent(const ScColor& col)
{
	RGBColor rgb;
	ScColorEngine::getRGBValues(col, m_Doc, rgb);
	blockSpinSignals(true);
	rSpin->setValue(rgb.r);
	gSpin->setValue(rgb.g);
	bSpin->setValue(rgb.b);
	blockSpinSignals(false);
}

void CWDialog::setupCMYKComponent(const ScColor& col)
{
	CMYKColorF cmyk;
	ScColorEngine::getCMYKValues(col, m_Doc, cmyk);
	blockSpinSignals(true);
	cSpin->setValue(qRound(cmyk.c * 100.0));
	mSpin->setValue(qRound(cmyk.m * 100.0));
	ySpin->setValue(qRound(cmyk.y * 100.0));
	kSpin->setValue(qRound(cmyk.k * 100.0));
	blockSpinSignals(false);
}

void CWDialog::setupHSVComponent(const ScColor& col)
{
	int h, s, v;
	QColor qc(ScColorEngine::getRGBColor(col, m_Doc));
	qc.getHsv(&h, &s, &v);
	blockSpinSignals(true);
	hSpin->setValue(h);
	sSpin->setValue(s);
	vSpin->setValue(v);
	blockSpinSignals(false);
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
		ScMessageBox::information(this, windowTitle(),
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

void CWDialog::colorList_currentChanged(const QString& itemText)
{
	if (itemText.isEmpty())
		return;

	// if it's base color we do not need to recompute it again
	if (itemText == colorWheel->trBaseColor)
	{
		currentColorTable->item(0, 4)->setText(cmykLabel->text());
		currentColorTable->item(1, 4)->setText(rgbLabel->text());
		currentColorTable->item(2, 4)->setText(hsvLabel->text());
		currentColorTable->item(0, 0)->setText(cSpin->text());
		currentColorTable->item(0, 1)->setText(mSpin->text());
		currentColorTable->item(0, 2)->setText(ySpin->text());
		currentColorTable->item(0, 3)->setText(kSpin->text());
		currentColorTable->item(1, 0)->setText(rSpin->text());
		currentColorTable->item(1, 1)->setText(gSpin->text());
		currentColorTable->item(1, 2)->setText(bSpin->text());
		currentColorTable->item(2, 0)->setText(hSpin->text());
		currentColorTable->item(2, 1)->setText(sSpin->text());
		currentColorTable->item(2, 2)->setText(vSpin->text());
	}
	else
	{
		ScColor col(colorWheel->colorList[itemText]);
		currentColorTable->item(0, 4)->setText(col.nameCMYK(m_Doc));
		currentColorTable->item(1, 4)->setText(col.nameRGB(m_Doc));
		currentColorTable->item(2, 4)->setText(getHexHsv(col));
		// components
		QString num;
		CMYKColor cmyk;
		ScColorEngine::getCMYKValues(col, m_Doc, cmyk);
		currentColorTable->item(0, 0)->setText(num.setNum(cmyk.c));
		currentColorTable->item(0, 1)->setText(num.setNum(cmyk.m));
		currentColorTable->item(0, 2)->setText(num.setNum(cmyk.y));
		currentColorTable->item(0, 3)->setText(num.setNum(cmyk.k));
		RGBColor rgb;
		ScColorEngine::getRGBValues(col, m_Doc, rgb);
		currentColorTable->item(1, 0)->setText(num.setNum(rgb.r));
		currentColorTable->item(1, 1)->setText(num.setNum(rgb.g));
		currentColorTable->item(1, 2)->setText(num.setNum(rgb.b));
		int h, s, v;
		QColor hsvCol(ScColorEngine::getRGBColor(col, m_Doc));
		hsvCol.getHsv(&h, &s, &v);
		currentColorTable->item(2, 0)->setText(num.setNum(h));
		currentColorTable->item(2, 1)->setText(num.setNum(s));
		currentColorTable->item(2, 2)->setText(num.setNum(v));
	}
}

QString CWDialog::getHexHsv(const ScColor& c)
{
	int h, s, v;
	QColor hsvCol(ScColorEngine::getRGBColor(c, m_Doc));
	hsvCol.getHsv(&h, &s, &v);
	return QString("#%1%2%3").arg(h, 0, 16).arg(s, 0, 16).arg(v, 0, 16);
}
