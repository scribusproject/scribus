
#include "dialog.h"
#include "dialog.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qdir.h>
#include "dialog.ui.h"

/*
 *  Constructs a ExportForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ExportForm::ExportForm(QWidget* parent, int size, int quality, QString type)
	: QDialog(parent, "ExportForm", true, 0)
{
	ExportFormLayout = new QGridLayout(this, 1, 1, 11, 6, "ExportFormLayout");

	layout9 = new QHBoxLayout(0, 0, 6, "layout9");

	TextLabel1 = new QLabel(this, "TextLabel1");
	layout9->addWidget(TextLabel1);

	OutputDirectory = new QLineEdit(this, "OutputDirectory");
	OutputDirectory->setText(QDir::currentDirPath());
	layout9->addWidget(OutputDirectory);

	OutputDirectoryButton = new QPushButton(this, "OutputDirectoryButton");
	layout9->addWidget(OutputDirectoryButton);

	ExportFormLayout->addLayout(layout9, 0, 0);

	layout11 = new QHBoxLayout(0, 0, 6, "layout11");

	layout10 = new QHBoxLayout(0, 0, 6, "layout10");

	TextLabel2 = new QLabel(this, "TextLabel2");
	layout10->addWidget(TextLabel2);

	BitmapType = new QComboBox(true, this, "BitmapType");
	BitmapType->insertStrList(QImageIO::outputFormats());
	BitmapType->setCurrentText(type);
	BitmapType->setEditable(false);
	layout10->addWidget(BitmapType);
	layout11->addLayout(layout10);
	Spacer2 = new QSpacerItem(163, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout11->addItem(Spacer2);

	ExportFormLayout->addLayout(layout11, 1, 0);

	layout14 = new QHBoxLayout(0, 0, 6, "layout14");

	layout12 = new QHBoxLayout(0, 0, 6, "layout12");

	textLabel1 = new QLabel(this, "textLabel1");
	layout12->addWidget(textLabel1);

	QualityBox = new QSpinBox(this, "QualityBox");
	QualityBox->setMaxValue(100);
	QualityBox->setMinValue(1);
	QualityBox->setValue(quality);
	layout12->addWidget(QualityBox);

	textLabel2 = new QLabel(this, "textLabel2");
	layout12->addWidget(textLabel2);
	layout14->addLayout(layout12);
	spacer4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout14->addItem(spacer4);

	layout13 = new QHBoxLayout(0, 0, 6, "layout13");

	textLabel3 = new QLabel(this, "textLabel3");
	layout13->addWidget(textLabel3);

	SizeBox = new QSpinBox(this, "SizeBox");
	SizeBox->setMaxValue(9999);
	SizeBox->setMinValue(1);
	SizeBox->setValue(size);
	layout13->addWidget(SizeBox);
	layout14->addLayout(layout13);
	spacer5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout14->addItem(spacer5);

	ExportFormLayout->addLayout(layout14, 2, 0);

	ButtonGroup1 = new QButtonGroup(this, "ButtonGroup1");
	ButtonGroup1->setFrameShape(QButtonGroup::Box);

	OnePageRadio = new QRadioButton(ButtonGroup1, "OnePageRadio");
	OnePageRadio->setGeometry(QRect(31, 31, 269, 23));
	OnePageRadio->setChecked(TRUE);

	AllPagesRadio = new QRadioButton(ButtonGroup1, "AllPagesRadio");
	AllPagesRadio->setGeometry(QRect(30, 60, 269, 23));

	IntervalPagesRadio = new QRadioButton(ButtonGroup1, "IntervalPagesRadio");
	IntervalPagesRadio->setGeometry(QRect(30, 90, 122, 23));

	QWidget* privateLayoutWidget = new QWidget(ButtonGroup1, "layout13");
	privateLayoutWidget->setGeometry(QRect(170, 75, 159, 40));
	layout13_2 = new QHBoxLayout(privateLayoutWidget, 11, 6, "layout13_2");

	FromBox = new QSpinBox(privateLayoutWidget, "FromBox");
	FromBox->setEnabled(FALSE);
	layout13_2->addWidget(FromBox);

	layout16 = new QHBoxLayout(0, 0, 6, "layout16");

	TextLabel4 = new QLabel(privateLayoutWidget, "TextLabel4");
	layout16->addWidget(TextLabel4);

	ToBox = new QSpinBox(privateLayoutWidget, "ToBox");
	ToBox->setEnabled(FALSE);
	layout16->addWidget(ToBox);
	layout13_2->addLayout(layout16);

	ToBox->setMinimumHeight(QualityBox->height());
	FromBox->setMinimumHeight(QualityBox->height());
	TextLabel4->setMinimumHeight(TextLabel1->height());

	ExportFormLayout->addWidget(ButtonGroup1, 3, 0);

	layout8 = new QHBoxLayout(0, 0, 6, "layout8");
	Spacer1 = new QSpacerItem(260, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	layout8->addItem(Spacer1);

	OkButton = new QPushButton(this, "OkButton");
	layout8->addWidget(OkButton);

	CancelButton = new QPushButton(this, "CancelButton");
	layout8->addWidget(CancelButton);

	ExportFormLayout->addLayout(layout8, 4, 0);
	languageChange();
	readConfig();
	resize(QSize(437, 300).expandedTo(minimumSizeHint()));
	clearWState(WState_Polished);

	// signals and slots connections
	connect(OutputDirectoryButton, SIGNAL(clicked()), this, SLOT(OutputDirectoryButton_pressed()));
	connect(OkButton, SIGNAL(clicked()), this, SLOT(OkButton_pressed()));
	connect(CancelButton, SIGNAL(clicked()), this, SLOT(CancelButton_pressed()));
	connect(IntervalPagesRadio, SIGNAL(stateChanged(int)), this, SLOT(IntervalPagesRadio_stateChanged(int)));
	connect(AllPagesRadio, SIGNAL(stateChanged(int)), this, SLOT(AllPagesRadio_stateChanged(int)));
	connect(OnePageRadio, SIGNAL(stateChanged(int)), this, SLOT(OnePageRadio_stateChanged(int)));
	connect(ToBox, SIGNAL(valueChanged(int)), this, SLOT(ChTo()));
	connect(FromBox, SIGNAL(valueChanged(int)), this, SLOT(ChFrom()));
}


/*
 *  Destroys the object and frees any allocated resources
 */
ExportForm::~ExportForm()
{
	// no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ExportForm::languageChange()
{
	setCaption(tr("Export as Image"));
	TextLabel1->setText(tr("Export to directory:"));
	OutputDirectoryButton->setText(tr("&Change..."));
	OutputDirectoryButton->setAccel(QKeySequence(tr("Alt+C")));
	TextLabel2->setText(tr("Image type:"));
	textLabel1->setText(tr("Quality :"));
	textLabel2->setText(tr("%"));
	textLabel3->setText(tr("Size:"));
	ButtonGroup1->setTitle(tr("Export "));
	OnePageRadio->setText(tr("&One page"));
	OnePageRadio->setAccel(QKeySequence(tr("Alt+O")));
	AllPagesRadio->setText(tr("&All pages"));
	AllPagesRadio->setAccel(QKeySequence(tr("Alt+A")));
	IntervalPagesRadio->setText(tr("&Interval from:"));
	IntervalPagesRadio->setAccel(QKeySequence(tr("Alt+I")));
	TextLabel4->setText(tr("to:"));
	OkButton->setText(tr("&OK"));
	OkButton->setAccel(QKeySequence(tr("Alt+O")));
	CancelButton->setText(tr("&Cancel"));
	CancelButton->setAccel(QKeySequence(tr("Alt+C")));
	// tooltips
	QToolTip::add(OutputDirectoryButton, tr("Change the output directory"));
	QToolTip::add(OutputDirectory, tr("The output directory - the place to store your images.\nName of the export file will be 'documentname-pagenumber.filetype'"));
	QToolTip::add(OnePageRadio, tr("Export only the current page"));
	QToolTip::add(AllPagesRadio, tr("Export all pages to images"));
	QToolTip::add(IntervalPagesRadio, tr("Export a range of pages to images"));
	QToolTip::add(FromBox, tr("First page to export when exporting a range"));
	QToolTip::add(ToBox, tr("Last page to export when exporting a range"));
	QToolTip::add(SizeBox, tr("The height of the output images in pixels"));
	QToolTip::add(QualityBox, tr("The quality of your images - 100% is the best, 1% the highest compression"));
	QToolTip::add(BitmapType, tr("Available export formats" ));
}


void ExportForm::readConfig()
{
	QString fname = QDir::convertSeparators(
		QDir::homeDirPath()+"/.scribus/scribus-pixmap-export.rc");
	QString value;
	QFile f(fname);

	if (f.open(IO_ReadOnly))
	{
		// cfg exists
		QTextStream of(&f);
		of >> value;
		if (value && value.length()>0)
			SizeBox->setValue(value.toUInt());
		of >> value;
		if (value && value.length()>0)
			QualityBox->setValue(value.toUInt());
		of >> value;
		if (value && value.length()>0)
		{
			ButtonGroup1->setButton(value.toInt());
			if (value.toInt()==2)
			{
				ToBox->setEnabled(TRUE);
				FromBox->setEnabled(TRUE);
			}
		}
		of >> value;
		if (value && value.length()>0)
			FromBox->setValue(value.toUInt());
		of >> value;
		if (value && value.length()>0)
			ToBox->setValue(value.toUInt());
		of >> value;
		if (value && value.length()>0)
			BitmapType->setCurrentItem(value.toUInt());
		f.close();
	}
}


void ExportForm::writeConfig()
{
	QString fname = QDir::convertSeparators(
		QDir::homeDirPath()+"/.scribus/scribus-pixmap-export.rc");
	QString value;
	QFile f(fname);

	if (f.open(IO_WriteOnly))
	{
		// cfg exists
		QTextStream of(&f);
		of << SizeBox->value() << endl;
		of << QualityBox->value() << endl;
		of << ButtonGroup1->id(ButtonGroup1->selected()) << endl;
		of << FromBox->value() << endl;
		of << ToBox->value() << endl;
		of << BitmapType->currentItem() << endl;
		f.close();
	}
}
