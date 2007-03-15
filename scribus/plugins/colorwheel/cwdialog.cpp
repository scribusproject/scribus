/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "cwdialog.h"
//#include "cwdialog.moc"

#include <qvariant.h>
#include <qcombobox.h>
#include <q3header.h>
#include <q3listview.h>
#include <q3listbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qpainter.h>
#include <qmenubar.h>
#include <q3groupbox.h>
#include <qslider.h>
#include <qtabwidget.h>
#include <q3table.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <QPixmap>

#include "prefsmanager.h"
#include "commonstrings.h"
#include "cmykfw.h"
#include "prefsfile.h"
#include "mpalette.h"
#include "colorblind.h"
#include "colorutil.h"
#include "colorlistbox.h"
#include "sccolorengine.h"

CWDialog::CWDialog(QWidget* parent, ScribusDoc* doc, const char* name, bool modal, Qt::WFlags fl)
	: QDialog (parent, name, modal, fl),
	  m_Doc(doc)
{
	setupUi(this);
	int h, s, v;
	ScColor color;
	QString colorName;
	connectSlots(false);
	// setup combobox
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Monochromatic), colorWheel->Monochromatic);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Analogous), colorWheel->Analogous);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Complementary), colorWheel->Complementary);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Split), colorWheel->Split);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Triadic), colorWheel->Triadic);
	typeCombo->insertItem(colorWheel->getTypeDescription(colorWheel->Tetradic), colorWheel->Tetradic);
	// defects
	defectCombo->insertItem(CommonStrings::trVisionNormal);
	defectCombo->insertItem(CommonStrings::trVisionProtanopia);
	defectCombo->insertItem(CommonStrings::trVisionDeuteranopia);
	defectCombo->insertItem(CommonStrings::trVisionTritanopia);
	defectCombo->insertItem(CommonStrings::trVisionFullColorBlind);
	// document colors
	documentColorList->updateBox(m_Doc->PageColors, ColorListBox::fancyPixmap);
	// preferences
	prefs = PrefsManager::instance()->prefsFile->getPluginContext("colorwheel");
	typeCombo->setCurrentItem(prefs->getInt("cw_type", 0));
	angleSpin->setValue(prefs->getInt("cw_angle", 15));
	colorWheel->currentDoc = m_Doc;
	colorWheel->angle = angleSpin->value();
	colorWheel->baseAngle = prefs->getInt("cw_baseangle", 0);
	colorspaceTab->setCurrentPage(prefs->getInt("cw_space", 0));
	color.setNamedColor(prefs->get("cw_color", "#00000000"));
	// Handle color previously selected in the document tab
	if (colorspaceTab->currentPage() == tabDocument)
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
		colorspaceTab->setCurrentPage(0);
	}
	else if (colorspaceTab->currentPage() == tabDocument)
	{
		colorWheel->actualColor = color;
		documentColorList->setCurrentItem(documentColorList->findItem(colorName));
	}
	else
		colorWheel->actualColor = color;

	resize(QSize(prefs->getInt("cw_width", 640),
		   prefs->getInt("cw_height", 480)).expandedTo(minimumSizeHint()));
	previewLabel->resize(prefs->getInt("cw_samplex", 300), prefs->getInt("cw_sampley", 100));
		
	// setup
	colorspaceTab_currentChanged(colorspaceTab->currentPage());
	currentColorTable->horizontalHeader()->hide();
	currentColorTable->setTopMargin(0);
	currentColorTable->setNumCols(5);

	// signals and slots that cannot be in ui file
	connect(colorWheel, SIGNAL(clicked(int, const QPoint&)),
			this, SLOT(colorWheel_clicked(int, const QPoint&)));
	connect(documentColorList, SIGNAL(currentChanged(Q3ListBoxItem *)),
			this, SLOT(documentColorList_currentChanged(Q3ListBoxItem *)));
	connect(colorList, SIGNAL(currentChanged(Q3ListBoxItem *)),
			this, SLOT(colorList_currentChanged(Q3ListBoxItem *)));
	connectSlots(true);
}

CWDialog::~CWDialog()
{
	// preferences
	QString colorName = (colorspaceTab->currentPage() == tabDocument) ? documentColorList->currentText() : "";
	prefs->set("cw_type", typeCombo->currentItem());
	prefs->set("cw_angle", angleSpin->value());
	prefs->set("cw_baseangle", colorWheel->baseAngle);
	prefs->set("cw_color", colorWheel->actualColor.name());
	prefs->set("cw_colorname", colorName);
	prefs->set("cw_space", colorspaceTab->currentPageIndex());
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

void CWDialog::documentColorList_currentChanged(Q3ListBoxItem *item)
{
	if (!item)
		return;
	ScColor c(m_Doc->PageColors[documentColorList->currentText()]);
	colorWheel->currentColorSpace = c.getColorModel();
	setupColorComponents();
}

void CWDialog::colorspaceTab_currentChanged( QWidget * tab)
{
	if (tab == tabCMYK)
		colorWheel->currentColorSpace = colorModelCMYK;
	if (tab == tabRGB)
		colorWheel->currentColorSpace = colorModelRGB;
	if (tab == tabDocument)
	{
		if (documentColorList->currentItem() == -1)
			documentColorList->setSelected(0, true);
		documentColorList_currentChanged(documentColorList->item(documentColorList->currentItem()));
	}
	processColors(typeCombo->currentItem(), true);
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
}

void CWDialog::colorWheel_clicked(int, const QPoint&)
{
	processColors(typeCombo->currentItem(), true);
}

void CWDialog::angleSpin_valueChanged(int value)
{
	colorWheel->angle = value;
	processColors(typeCombo->currentItem(), false);
}

void CWDialog::setPreview()
{
	int x = previewLabel->width();
	int y = previewLabel->height();
	Q3ValueList<ScColor> cols = colorWheel->colorList.values();
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
	if (defectCombo->currentItem() == VisionDefectColor::normalVision)
		return c;
	VisionDefectColor *defect = new VisionDefectColor(c);
	defect->deficiency = defectCombo->currentItem();
	defect->convertDefect();
	QColor nc = defect->getColor();
	delete defect;
	return nc;
}

void CWDialog::fillColorList()
{
	uint ix = colorList->currentItem();
	colorList->updateBox(colorWheel->colorList, ColorListBox::fancyPixmap);
	Q3ListBoxItem *item = colorList->findItem(colorWheel->trBaseColor);
	if (item->prev())
	{
		colorList->takeItem(item);
		colorList->insertItem(item, 0);
	}
	colorList->setCurrentItem(ix > colorList->count() ? 0 : ix);
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
			m_Doc->PageColors[it.key()] = it.data();
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
		m_Doc->PageColors[it.key()] = it.data();
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
	if (colorspaceTab->currentPage() == tabCMYK)
	{
		c = ScColor(qRound(cSpin->value() * 2.55), qRound(mSpin->value() * 2.55),
					qRound(ySpin->value() * 2.55), qRound(kSpin->value() * 2.55));
		c = ScColorEngine::convertToModel(c, m_Doc, colorModelCMYK);
		setupRGBComponent(c);
		setupHSVComponent(c);
	}
	if (colorspaceTab->currentPage() == tabRGB)
	{
		c = ScColor(rSpin->value(), gSpin->value(), bSpin->value());
		c = ScColorEngine::convertToModel(c, m_Doc, colorModelRGB);
		setupCMYKComponent(c);
		setupHSVComponent(c);
	}
	if (colorspaceTab->currentPage() == tabHSV)
	{
		QColor qc;
		qc.setHsv(hSpin->value(), sSpin->value(), vSpin->value());
		c.fromQColor(qc);
		c = ScColorEngine::convertToModel(c, m_Doc, colorModelRGB);
		setupCMYKComponent(c);
		setupRGBComponent(c);
	}
	if (colorspaceTab->currentPage() == tabDocument)
	{
		c = m_Doc->PageColors[documentColorList->currentText()];
		setupRGBComponent(c);
		setupCMYKComponent(c);
		setupHSVComponent(c);
	}

	if (colorWheel->recomputeColor(c))
		processColors(typeCombo->currentItem(), false);
	else
	{
		colorList->clear();
		QMessageBox::information(this, caption(),
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

void CWDialog::colorList_currentChanged(Q3ListBoxItem * item)
{
	if (!item)
		return;

	// if it's base color we do not need to recompute it again
	if (item->text() == colorWheel->trBaseColor)
	{
		currentColorTable->setText(0, 4, cmykLabel->text());
		currentColorTable->setText(1, 4, rgbLabel->text());
		currentColorTable->setText(2, 4, hsvLabel->text());
		currentColorTable->setText(0, 0, cSpin->text());
		currentColorTable->setText(0, 1, mSpin->text());
		currentColorTable->setText(0, 2, ySpin->text());
		currentColorTable->setText(0, 3, kSpin->text());
		currentColorTable->setText(1, 0, rSpin->text());
		currentColorTable->setText(1, 1, gSpin->text());
		currentColorTable->setText(1, 2, bSpin->text());
		currentColorTable->setText(2, 0, hSpin->text());
		currentColorTable->setText(2, 1, sSpin->text());
		currentColorTable->setText(2, 2, vSpin->text());

	}
	else
	{
		ScColor col(colorWheel->colorList[item->text()]);
		currentColorTable->setText(0, 4, col.nameCMYK(m_Doc));
		currentColorTable->setText(1, 4, col.nameRGB(m_Doc));
		currentColorTable->setText(2, 4, getHexHsv(col));
		// components
		QString num;
		CMYKColor cmyk;
		ScColorEngine::getCMYKValues(col, m_Doc, cmyk);
		currentColorTable->setText(0, 0, num.setNum(cmyk.c));
		currentColorTable->setText(0, 1, num.setNum(cmyk.m));
		currentColorTable->setText(0, 2, num.setNum(cmyk.y));
		currentColorTable->setText(0, 3, num.setNum(cmyk.k));
		RGBColor rgb;
		ScColorEngine::getRGBValues(col, m_Doc, rgb);
		currentColorTable->setText(1, 0, num.setNum(rgb.r));
		currentColorTable->setText(1, 1, num.setNum(rgb.g));
		currentColorTable->setText(1, 2, num.setNum(rgb.b));
		int h, s, v;
		QColor hsvCol(ScColorEngine::getRGBColor(col, m_Doc));
		hsvCol.getHsv(&h, &s, &v);
		currentColorTable->setText(2, 0, num.setNum(h));
		currentColorTable->setText(2, 1, num.setNum(s));
		currentColorTable->setText(2, 2, num.setNum(v));
	}
	currentColorTable->adjustColumn(0);
	currentColorTable->adjustColumn(1);
	currentColorTable->adjustColumn(2);
	currentColorTable->adjustColumn(3);
	currentColorTable->adjustColumn(4);
}

QString CWDialog::getHexHsv(ScColor c)
{
	int h, s, v;
	QColor hsvCol(ScColorEngine::getRGBColor(c, m_Doc));
	hsvCol.getHsv(&h, &s, &v);
	return QString("#%1%2%3").arg(h, 0, 16).arg(s, 0, 16).arg(v, 0, 16);
}
