#include "cwsetcolor.h"
#include "commonstrings.h"
#include "cmykfw.h"
#include "util.h"

#include <qlabel.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qpixmap.h>
#include <qpainter.h>


CwSetColor::CwSetColor(QColor beginColor, QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog(parent, name, modal, fl)
{
	if (!name)
		setName("CwSetColor");
	CwSetColorLayout = new QGridLayout(this, 1, 1, 11, 6, "CwSetColorLayout");

	mainLayout = new QVBoxLayout(0, 0, 6, "mainLayout");

	// CMYK
	cmykBox = new QGroupBox(this, "cmykBox");
	cmykBox->setColumnLayout(0, Qt::Vertical );
	cmykBox->layout()->setSpacing(6);
	cmykBox->layout()->setMargin(11);
	cmykBoxLayout = new QGridLayout(cmykBox->layout());

	cLabel = new QLabel(cmykBox, "cLabel");
	cSpin = new QSpinBox(cmykBox, "cSpin");
	cSpin->setMinValue(0);
	cSpin->setMaxValue(255);

	mLabel = new QLabel(cmykBox, "mLabel");
	mSpin = new QSpinBox(cmykBox, "mSpin");
	mSpin->setMinValue(0);
	mSpin->setMaxValue(255);

	yLabel = new QLabel(cmykBox, "yLabel");
	ySpin = new QSpinBox(cmykBox, "ySpin");
	ySpin->setMinValue(0);
	ySpin->setMaxValue(255);

	kLabel = new QLabel(cmykBox, "kLabel");
	kSpin = new QSpinBox(cmykBox, "kSpin");
	kSpin->setMinValue(0);
	kSpin->setMaxValue(255);

	cmykSample = new QLabel(cmykBox, "cmykSample");

	cmykButton = new QPushButton(cmykBox, "cmykButton");

	cmykBoxLayout->addWidget(cLabel, 0, 0);
	cmykBoxLayout->addWidget(cSpin, 0, 1);
	cmykBoxLayout->addWidget(mLabel, 0, 2);
	cmykBoxLayout->addWidget(mSpin, 0, 3);
	cmykBoxLayout->addWidget(yLabel, 0, 4);
	cmykBoxLayout->addWidget(ySpin, 0, 5);
	cmykBoxLayout->addWidget(kLabel, 0, 6);
	cmykBoxLayout->addWidget(kSpin, 0, 7);
	cmykBoxLayout->addWidget(cmykSample, 0, 8);
	cmykBoxLayout->addWidget(cmykButton, 0, 9);

	// RGB
	rgbBox = new QGroupBox(this, "rgbBox");
	rgbBox->setColumnLayout(0, Qt::Vertical );
	rgbBox->layout()->setSpacing(6);
	rgbBox->layout()->setMargin(11);
	rgbBoxLayout = new QGridLayout(rgbBox->layout());

	rLabel = new QLabel(rgbBox, "rLabel");
	rSpin = new QSpinBox(rgbBox, "rSpin");
	rSpin->setMinValue(0);
	rSpin->setMaxValue(255);

	gLabel = new QLabel(rgbBox, "gLabel");
	gSpin = new QSpinBox(rgbBox, "gSpin");
	gSpin->setMinValue(0);
	gSpin->setMaxValue(255);

	bLabel = new QLabel(rgbBox, "bLabel");
	bSpin = new QSpinBox(rgbBox, "bSpin");
	bSpin->setMinValue(0);
	bSpin->setMaxValue(255);

	rgbSample = new QLabel(rgbBox, "rgbSample");

	rgbButton = new QPushButton(rgbBox, "rgbButton");

	rgbBoxLayout->addWidget(rLabel, 0, 0);
	rgbBoxLayout->addWidget(rSpin, 0, 1);
	rgbBoxLayout->addWidget(gLabel, 0, 2);
	rgbBoxLayout->addWidget(gSpin, 0, 3);
	rgbBoxLayout->addWidget(bLabel, 0, 4);
	rgbBoxLayout->addWidget(bSpin, 0, 5);
	rgbBoxLayout->addWidget(rgbSample, 0, 8);
	rgbBoxLayout->addWidget(rgbButton, 0, 9);

	// HSV
	hsvBox = new QGroupBox(this, "hsvBox");
	hsvBox->setColumnLayout(0, Qt::Vertical );
	hsvBox->layout()->setSpacing(6);
	hsvBox->layout()->setMargin(11);
	hsvBoxLayout = new QGridLayout(hsvBox->layout());

	hLabel = new QLabel(hsvBox, "hLabel");
	hSpin = new QSpinBox(hsvBox, "hSpin");
	hSpin->setMinValue(0);
	hSpin->setMaxValue(364);

	sLabel = new QLabel(hsvBox, "sLabel");
	sSpin = new QSpinBox(hsvBox, "sSpin");
	sSpin->setMinValue(0);
	sSpin->setMaxValue(255);

	vLabel = new QLabel(hsvBox, "vLabel");
	vSpin = new QSpinBox(hsvBox, "vSpin");
	vSpin->setMinValue(0);
	vSpin->setMaxValue(255);

	hsvSample = new QLabel(hsvBox, "hsvSample");

	hsvButton = new QPushButton(hsvBox, "hsvButton");

	hsvBoxLayout->addWidget(hLabel, 0, 0);
	hsvBoxLayout->addWidget(hSpin, 0, 1);
	hsvBoxLayout->addWidget(sLabel, 0, 2);
	hsvBoxLayout->addWidget(sSpin, 0, 3);
	hsvBoxLayout->addWidget(vLabel, 0, 4);
	hsvBoxLayout->addWidget(vSpin, 0, 5);
	hsvBoxLayout->addWidget(hsvSample, 0, 8);
	hsvBoxLayout->addWidget(hsvButton, 0, 9);

	// layouts
	mainLayout->addWidget(cmykBox);
	mainLayout->addWidget(rgbBox);
	mainLayout->addWidget(hsvBox);

	cancelButton = new QPushButton(CommonStrings::tr_Cancel, this, "cancelButton");
	QHBoxLayout *cancelLayout = new QHBoxLayout(0, 0, 6, "cancelLayout");
	QSpacerItem *cancelSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	cancelLayout->addItem(cancelSpacer);
	cancelLayout->addWidget(cancelButton);
	mainLayout->addLayout(cancelLayout);

	CwSetColorLayout->addLayout(mainLayout, 0, 0);
	resize(QSize(288, 310).expandedTo(minimumSizeHint()));
	languageChange();
	clearWState(WState_Polished);

	// fill begin values
	fillBeginComponents(beginColor);

	sampleRGB();
	sampleHSV();
	sampleCMYK();

	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(rgbButton, SIGNAL(clicked()), this, SLOT(rgbButton_clicked()));
	connect(hsvButton, SIGNAL(clicked()), this, SLOT(hsvButton_clicked()));
	connect(cmykButton, SIGNAL(clicked()), this, SLOT(cmykButton_clicked()));
	connect(rSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleRGB()));
	connect(gSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleRGB()));
	connect(bSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleRGB()));
	connect(hSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleHSV()));
	connect(sSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleHSV()));
	connect(vSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleHSV()));
	connect(cSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleCMYK()));
	connect(mSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleCMYK()));
	connect(ySpin, SIGNAL(valueChanged(int)), this, SLOT(sampleCMYK()));
	connect(kSpin, SIGNAL(valueChanged(int)), this, SLOT(sampleCMYK()));
}

void CwSetColor::languageChange()
{
	setCaption(tr("Set Color Components"));
	cmykBox->setTitle(tr("CMYK"));
	rgbBox->setTitle(tr("RGB"));
	hsvBox->setTitle(tr("HSV"));
	hLabel->setText(tr("H:"));
	sLabel->setText(tr("S:"));
	vLabel->setText(tr("V:"));
	rLabel->setText(tr("R:"));
	gLabel->setText(tr("G:"));
	bLabel->setText(tr("B:"));
	cLabel->setText(tr("C:"));
	mLabel->setText(tr("M:"));
	yLabel->setText(tr("Y:"));
	kLabel->setText(tr("K:"));
	rgbButton->setText(tr("Set &RGB"));
	cmykButton->setText(tr("Set C&MYK"));
	hsvButton->setText(tr("Set &HSV"));
}

void CwSetColor::hsvButton_clicked()
{
	newColor.setHsv(hSpin->value(), sSpin->value(), vSpin->value());
	accept();
}

void CwSetColor::rgbButton_clicked()
{
	newColor.setRgb(rSpin->value(), gSpin->value(), bSpin->value());
	accept();
}

void CwSetColor::cmykButton_clicked()
{
	ScColor col = ScColor(cSpin->value(), mSpin->value(), ySpin->value(), kSpin->value());
	newColor = col.getRGBColor();
	accept();
}

void CwSetColor::sampleRGB()
{
	QColor col;
	QPixmap *pm;
	col.setRgb(rSpin->value(), gSpin->value(), bSpin->value());
	pm = getWidePixmap(col);
	rgbSample->setPixmap(*pm);
}

void CwSetColor::sampleHSV()
{
	QColor col;
	QPixmap *pm;
	col.setHsv(hSpin->value(), sSpin->value(), vSpin->value());
	pm = getWidePixmap(col);
	hsvSample->setPixmap(*pm);
}

void CwSetColor::sampleCMYK()
{
	QPixmap *pm;
	ScColor col = ScColor(cSpin->value(), mSpin->value(), ySpin->value(), kSpin->value());
	pm = getWidePixmap(col.getRGBColor());
	cmykSample->setPixmap(*pm);
}

void CwSetColor::fillBeginComponents(QColor col)
{
	int c, m, y, k, r, g, b, h, s, v;
	ScColor sc;

	col.getRgb(&r, &g, &b);
	col.getHsv(&h, &s, &v);
	sc.fromQColor(col);
	sc.getCMYK(&c, &m, &y, &k);
	rSpin->setValue(r);
	gSpin->setValue(g);
	bSpin->setValue(b);
	hSpin->setValue(h);
	sSpin->setValue(s);
	vSpin->setValue(v);
	cSpin->setValue(c);
	mSpin->setValue(m);
	ySpin->setValue(y);
	kSpin->setValue(k);
}

#include "cwsetcolor.moc"
