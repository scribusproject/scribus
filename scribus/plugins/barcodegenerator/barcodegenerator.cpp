/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "barcodegenerator.h"
#include "util.h"
#include "scribus.h"

#include "commonstrings.h"
#include <qcombobox.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qcolor.h>
#include <qcolordialog.h>
#include <qlabel.h>
#include <qfile.h>
#include <qdir.h>
#include <qfiledialog.h> 


BarcodeType::BarcodeType(QString cmd, QString exa, QString comm)
{
	command = cmd;
	example = exa;
	comment = comm;
}

BarcodeGenerator::BarcodeGenerator(QWidget* parent, const char* name)
	: BarcodeGeneratorBase(parent, name, true)
{
	map["EAN-13"] = BarcodeType("ean13", "9781860742712", "12 or 13 digits");
	map["EAN-8"] = BarcodeType("ean8", "12345678", "8 digits");
	map["UPC-A"] = BarcodeType("upca", "78858101497", "11 or 12 digits");
	map["UPC-E"] = BarcodeType("upce", "0123456", "7 or 8 digits");
	map["EAN-5"] = BarcodeType("ean5", "90200", "5 digits");
	map["EAN-2"] = BarcodeType("ean2", "42", "2 digits");
	map["ISBN"] = BarcodeType("isbn", "1-58880-149", "9 or 10 digits seperated appropriately with dashes");
//    "Code-11"] = "code11"
	map["Code-39"] = BarcodeType("code39", "CODE-39", "Variable number of characters, digits and any of the symbols -. *$/+%.");
//    "Code-93"] = "code93"
	map["Code-128"] = BarcodeType("code128", "^104^102Count^0991234^101!", "Variable number of ASCII characters and special funtion symbols, starting with the approriate start character for the initial character set. UCC/EAN-128s must have a manditory FNC 1 symbol immediately following the start character.");
	map["UCC/EAN-128"] = BarcodeType("code128", "^104^102Count^0991234^101!", "Variable number of ASCII characters and special funtion symbols, starting with the approriate start character for the initial character set. UCC/EAN-128s must have a manditory FNC 1 symbol immediately following the start character.");
	map["Rationalized Codabar"] = BarcodeType("rationalizedCodabar", "0123456789", "Variable number of digits and any of the symbols -$:/.+ABCD.");
	map["Interleaved 2 of 5"] = BarcodeType("interleaved2of5", "05012345678900", "Variable number of digits. An ITF-14 is 14 characters and does not have a check digit");
	map["ITF-14"] = BarcodeType("interleaved2of5", "05012345678900", "Variable number of digits. An ITF-14 is 14 characters and does not have a check digit");
	map["Code 2 of 5"] = BarcodeType("code2of5", "0123456789", "Variable number of digits");
	map["Postnet"] = BarcodeType("postnet", "01234567", "Variable number digits");
	map["Royal Mail"] = BarcodeType("royalmail", "LE28HS9Z", "Variable number digits and capital letters");
//    "Auspost"] = "auspost"
	map["MSI"] = BarcodeType("msi", "0120823635162", "Variable number digits");
//    "KIX"] = "kix"
	map["Plessey"] = BarcodeType("plessey", "012345ABCDEF", "Variable number of hexadecimal characters");
//    "Symbol"] = "symbol"
	
	useSamples = true;
	bcCombo->insertStringList(map.keys());
	okButton->setText(CommonStrings::tr_OK);
	cancelButton->setText(CommonStrings::tr_Cancel);
	resetButton->setPixmap(loadIcon("u_undo16.png"));
	lnColor = Qt::black;
	txtColor = Qt::black;
	bgColor = Qt::white;
	paintColorSample(linesLabel, lnColor);
	paintColorSample(txtLabel, txtColor);
	paintColorSample(bgLabel, bgColor);

	tmpFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/bcode.png");
	psFile = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/bcode.ps");

	// PS engine
	psCommand.append("%!PS-Adobe-2.0 EPSF-2.0\n");
	QFile f(QString(PLUGINDATA) + QString("barcode.ps"));
	f.open(IO_ReadOnly);
	QTextStream ts(&f);
	QString s = ts.read();
	int begin = s.find("% --BEGIN TEMPLATE--");
	int end = s.find("% --END TEMPLATE--");
	psCommand.append(s.mid(begin, end));
	f.close();
	psCommand.append("\n\n%command\n");
	bcComboChanged();
}

BarcodeGenerator::~BarcodeGenerator()
{
	QFile::remove(psFile);
	QFile::remove(tmpFile);
}

void BarcodeGenerator::bcComboChanged()
{
	QString s = bcCombo->currentText();
	commentEdit->setText(map[s].comment);
	if (useSamples)
	{
		disconnect(codeEdit, SIGNAL(textChanged(const QString&)), this, SLOT(codeEdit_textChanged(const QString&)));
		codeEdit->setText(map[s].example);
		connect(codeEdit, SIGNAL(textChanged(const QString&)), this, SLOT(codeEdit_textChanged(const QString&)));
	}
	paintBarcode();
}

void BarcodeGenerator::textCheck_changed()
{
	txtColorButton->setEnabled(textCheck->state());
	paintBarcode();
}

void BarcodeGenerator::guardCheck_changed()
{
	paintBarcode();
}

void BarcodeGenerator::paintColorSample(QLabel *l, QColor c)
{
	QPixmap pm(1, 1);
	pm.fill(c);
	l->setPixmap(pm);
}

void BarcodeGenerator::bgColorButton_pressed()
{
	bgColor = QColorDialog::getColor(bgColor, this);
	if (bgColor.isValid())
	{
		paintColorSample(bgLabel, bgColor);
		paintBarcode();
	}
}

void BarcodeGenerator::lnColorButton_pressed()
{
	lnColor = QColorDialog::getColor(lnColor, this);
	if (lnColor.isValid())
	{
		paintColorSample(linesLabel, lnColor);
		paintBarcode();
	}
}

void BarcodeGenerator::txtColorButton_pressed()
{
	txtColor = QColorDialog::getColor(txtColor, this);
	if (txtColor.isValid())
	{
		paintColorSample(txtLabel, txtColor);
		paintBarcode();
	}
}

void BarcodeGenerator::okButton_pressed()
{
	QString s = QFileDialog::getSaveFileName(
			QDir::currentDirPath(),
			"PNG Images (*.png)",
			this,
			"save file dialog",
			"Choose a filename to save under" );
	if (s == QString::null)
		return;
	if (!paintBarcode(s, 300)) // in 300dpi
	{
		qDebug("Error creating barcode itself!");
		return;
	}
	int z = ScMW->doc->itemAdd(PageItem::ImageFrame, PageItem::Unspecified,
							   ScMW->doc->currentPage->Margins.Left + ScMW->doc->currentPage->xOffset(),
							   ScMW->doc->currentPage->Margins.Top + ScMW->doc->currentPage->yOffset(),
							   200, 150,
							   1, "None", ScMW->doc->toolSettings.dPen, true);
	ScMW->doc->LoadPict(s, z, true);
	ScMW->view->DrawNew();
	accept();
}

void BarcodeGenerator::cancelButton_pressed()
{
	reject();
}

void BarcodeGenerator::codeEdit_textChanged(const QString&)
{
	useSamples = false;
	paintBarcode();
}

bool BarcodeGenerator::paintBarcode(QString fileName, int dpi)
{
	if (fileName == QString::null)
		fileName = tmpFile;
	QString opts("barcolor=%1 showbackground backgroundcolor=%2 textcolor=%3");
	opts = opts.arg(lnColor.name().replace('#', "")) \
			.arg(bgColor.name().replace('#', "")) \
			.arg(txtColor.name().replace('#', ""));
	if (textCheck->isChecked())
		opts += " includetext";
	if (guardCheck->isChecked())
		opts += " guardwhitespace";
	QString comm("15 10 moveto (%1) (%2) %3 barcode");
	comm = comm.arg(codeEdit->text()).arg(opts).arg(map[bcCombo->currentText()].command);
	comm = psCommand + comm;
	QFile f(psFile);
	if (!f.open(IO_WriteOnly))
	{
		sampleLabel->setText("<qt>" + tr("Error opening file: %1").arg(psFile) + "</qt>");
		return false;
	}
	QTextStream ts(&f);
	ts << comm;
	f.close();
	
	QString gargs("-dDEVICEWIDTHPOINTS=200 -dDEVICEHEIGHTPOINTS=150 -r%1 -sOutputFile=%2 %3");
	gargs = gargs.arg(dpi).arg(fileName).arg(psFile);
	int gs = callGS(gargs);
	bool retval = true;
	if (gs != 0)
		retval = false;
	// setup only preview
	if (fileName != tmpFile)
		return retval;
    if (gs == 0)
		sampleLabel->setPixmap(QPixmap(fileName));
	else
		sampleLabel->setText("<qt>" + tr("Error creating preview") + "</qt>");
	return retval;
}

void BarcodeGenerator::resetButton_clicked()
{
	useSamples = true;
	bcComboChanged();
}
