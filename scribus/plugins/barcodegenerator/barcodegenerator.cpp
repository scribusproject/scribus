/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDebug>
#include <QRegularExpression>
#include <QStandardItemModel>
#include <QTextStream>
#include <QThread>
#include <QTimer>

#include "../formatidlist.h"
#include "barcodegenerator.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "loadsaveplugin.h"
#include "scpaths.h"
#include "scribus.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "selection.h"
#include "ui/colorsandfills.h"
#include "undomanager.h"

static constexpr int debounceInterval = 250;  // ms

BarcodeType::BarcodeType(const QString &cmd, const QString &exa, const QString &exaop)
	: command(cmd),
	  exampleContents(exa),
	  exampleOptions(exaop)
{

}

BarcodeGenerator::BarcodeGenerator(QWidget* parent, const char* name)
	: QDialog(parent)
{
	ui.setupUi(this);
	setObjectName(name);
	setModal(true);

	ui.bcodeBox->layout()->setAlignment(Qt::AlignTop);
	ui.colorBox->layout()->setAlignment(Qt::AlignTop);

	connect(&thread, SIGNAL(renderedImage(QString)),this, SLOT(updatePreview(QString)));

	/*
	 *  We extract the barcode information from the BWIPP metadata which looks like this:
	 *
	 *  % --BEGIN ENCODER gs1-128--
	 *  % --REQUIRES preamble raiseerror renlinear code128--
	 *  % --DESC: GS1-128
	 *  % --EXAM: (01)95012345678903(3103)000123
	 *  % --EXOP: includetext
	 *  % --RNDR: renlinear
	 *
	 */

	QFile f(ScPaths::instance().shareDir() + QString("/plugins/barcode.ps"));
	if(!f.open(QIODevice::ReadOnly))
	{
		qDebug()<<"Barcodegenerator unable to open "<<f.fileName();
		return;
	}
	QTextStream ts(&f);
	QString bwipp = ts.readAll();
	f.close();

	QRegularExpression rx(
				"[\\r\\n]+% --BEGIN (RESOURCE|RENDERER|ENCODER) ([\\w-]+)--[\\r\\n]+"
				"(.*[\\r\\n]+)?"
				"(%%BeginResource.*[\\r\\n]+)"
				"% --END \\1 \\2--[\\r\\n]+",
				QRegularExpression::InvertedGreedinessOption | QRegularExpression::DotMatchesEverythingOption);

	for (const QRegularExpressionMatch& match : rx.globalMatch(bwipp))
	{
		QString restype = match.captured(1);
		QString resname = match.captured(2);
		QString reshead = match.captured(3);
		QString resbody = match.captured(4);

		resbodys[resname] = resbody;

		if (restype == "ENCODER")
		{
			QRegularExpression rxhead(
						"% --REQUIRES (.*)--[\\r\\n]+"
						"% --DESC:(.*)[\\r\\n]+"
						"% --EXAM:(.*)[\\r\\n]+"
						"% --EXOP:(.*)[\\r\\n]+"
						"% --RNDR:(.*)[\\r\\n]+"
						);
			QRegularExpressionMatch match = rxhead.match(reshead);
			if (match.hasMatch())
			{
				resreqs[resname] = match.captured(1).trimmed();
				resdescs[resname] = match.captured(2).trimmed();
				resexams[resname] = match.captured(3).trimmed();
				resexops[resname] = match.captured(4).trimmed();
				resrndrs[resname] = match.captured(5).trimmed();
				encoderlist.append(resname);
			}
		}
	}

	foreach (const QString& enc, encoderlist)
		map[resdescs[enc]] = BarcodeType(enc, resexams[enc], resexops[enc]);

	/*
	 *  Ultimately all of this static data about the capabilities of each barcode
	 *  encoder will be replaced by data read from the barcode.ps metadata, when
	 *  such data exists...
	 *
	 */

	// Content for the version and ecc combos
	resvers["qrcode"] = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40";
	resecls["qrcode"] = "L,Q,M,H";
	resvers["gs1qrcode"] = resvers["qrcode"];
	resecls["gs1qrcode"] = resecls["qrcode"];
	resvers["gs1dlqrcode"] = resvers["qrcode"];
	resecls["gs1dlqrcode"] = resecls["qrcode"];
	resvers["hibcqrcode"] = resvers["qrcode"];
	resecls["hibcqrcode"] = resecls["qrcode"];
	resvers["microqrcode"] = "M1,M2,M3,M4";
	resecls["microqrcode"] = "L,Q,M,H";
	resvers["datamatrix"] = "10x10,12x12,14x14,16x16,18x18,20x20,22x22,24x24,26x26,32x32,36x36,40x40,44x44,48x48,52x52,64x64,72x72,80x80,88x88,96x96,104x104,120x120,132x132,144x144,8x18,8x32,12x26,12x36,16x36,16x48";
	resecls["datamatrix"] = "";
	resvers["gs1datamatrix"] = resvers["datamatrix"];
	resecls["gs1datamatrix"] = resvers["datamatrix"];
	resvers["gs1dldatamatrix"] = resvers["datamatrix"];
	resecls["gs1dldatamatrix"] = resvers["datamatrix"];
	resvers["hibcdatamatrix"] = resvers["datamatrix"];
	resecls["hibcdatamatrix"] = resecls["datamatrix"];
	resvers["azteccode"] = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32";
	resvlbl["azteccode"] = "Layers";
	resecls["azteccode"] = "5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95";
	resvers["azteccodecompact"] = "1,2,3,4";
	resvlbl["azteccodecompact"] = resvlbl["azteccode"];
	resecls["azteccodecompact"] = resecls["azteccode"];
	resvers["pdf417"] = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30";
	resvlbl["pdf417"] = "Columns";
	resecls["pdf417"] = "1,2,3,4,5";
	resvers["pdf417compact"] = resvers["pdf417"];
	resvlbl["pdf417compact"] = resvlbl["pdf417"];
	resecls["pdf417compact"] = resecls["pdf417"];
	resvers["hibcpdf417"] = resvers["pdf417"];
	resvlbl["hibcpdf417"] = resvlbl["pdf417"];
	resecls["hibcpdf417"] = resecls["pdf417"];
	resvers["micropdf417"] = "1x11,1x14,1x17,1x20,1x24,1x28,2x8,2x11,2x14,2x17,2x20,2x23,2x26,3x6,3x8,3x10,3x12,3x15,3x20,3x26,3x32,3x38,3x44,4x4,4x6,4x8,4x10,4x12,4x15,4x20,4x26,4x32,4x38,4x44";
	resecls["micropdf417"] = "";
	resvers["hibcmicropdf417"] = resvers["micropdf417"];
	resecls["hibcmicropdf417"] = resecls["micropdf417"];
	resvers["hanxin"] = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84";
	resecls["hanxin"] = "L1,L2,L3,L4";
	resecls["ultracode"] = "EC0,EC1,EC2,EC3,EC4,EC5";
	resvers["rectangularmicroqrcode"] = "R7x43,R7x59,R7x77,R7x99,R7x139,R9x43,R9x59,R9x77,R9x99,R9x139,R11x27,R11x43,R11x59,R11x77,R11x99,R11x139,R13x27,R13x43,R13x59,R13x77,R13x99,R13x139,R15x43,R15x59,R15x77,R15x99,R15x139,R17x43,R17x59,R17x77,R17x99,R17x139";
	resecls["rectangularmicroqrcode"] = "M,H";
	resvers["datamatrixrectangular"] = "8x18,8x32,12x26,12x36,16x36,16x48";
	resecls["datamatrixrectangular"] = "";
	resvers["datamatrixrectangularextension"] = "8x18,8x32,12x26,12x36,16x36,16x48,8x48,8x64,8x80,8x96,8x120,8x144,12x64,12x88,16x64,20x36,20x44,20x64,22x48,24x48,24x64,26x40,26x48,26x64";
	resecls["datamatrixrectangularextension"] = "";
	resvers["gs1datamatrixrectangular"] = resvers["datamatrixrectangular"];
	resecls["gs1datamatrixrectangular"] = resecls["datamatrixrectangular"];
	resvers["hibcdatamatrixrectangular"] = resvers["datamatrixrectangular"];
	resecls["hibcdatamatrixrectangular"] = resecls["datamatrixrectangular"];
	resvers["hibcazteccode"] = resvers["azteccode"];
	resvlbl["hibcazteccode"] = resvlbl["azteccode"];
	resecls["hibcazteccode"] = resecls["azteccode"];
	resvers["maxicode"] = "2,3,4,5,6";
	resvlbl["maxicode"] = "Mode";
	resvers["mailmark"] = "7,9,29";
	resvlbl["mailmark"] = "Type";


	// Which options checkboxes are enabled for each encoder
	// Check whether `includetext` option is available for each encoder
	for(const QString &enc : std::as_const(encoderlist))
		resincludetextAvail[enc] = resexops[enc].contains("includetext");

	QStringList guardwhitespaceAvail;
	guardwhitespaceAvail << "ean13" << "ean8" << "isbn" << "ismn" << "issn" << "ean13composite" << "ean8composite";
	foreach (const QString& enc, guardwhitespaceAvail)
		resguardwhitespaceAvail[enc] = true;

	QStringList includecheckAvail;
	includecheckAvail << "bc412" << "channelcode" << "code11" << "code2of5" << "coop2of5" << "datalogic2of5"
					  << "iata2of5" << "industrial2of5" << "matrix2of5" << "code39" << "code39ext"
					  << "code93" << "code93ext" << "interleaved2of5" << "msi" << "rationalizedCodabar";
	foreach (const QString& enc, includecheckAvail)
		resincludecheckAvail[enc] = true;

	QStringList includecheckintextAvail;
	includecheckintextAvail << "bc412" << "code11" << "code2of5" << "coop2of5" << "datalogic2of5" << "iata2of5"
							<< "industrial2of5" << "matrix2of5" << "code39" << "code39ext" << "interleaved2of5"
							<< "japanpost" << "msi" << "planet" << "plessey" << "postnet" << "rationalizedCodabar" << "royalmail";
	foreach (const QString& enc, includecheckintextAvail)
		resincludecheckintextAvail[enc] = true;

	QStringList parseAvail;
	parseAvail << "azteccode" << "azteccodecompact" << "codablockf" << "hibccodablockf" << "code128" << "hibccode128" << "code16k" << "code39ext" << "code49"
			   << "code93ext" << "codeone" << "datamatrix" << "hibcdatamatrix" << "maxicode" << "micropdf417" << "hibcmicropdf417" << "pdf417" << "hibcpdf417"
			   << "pdf417compact" << "posicode" << "qrcode" << "hibcqrcode" << "microqrcode" << "telepen" << "hanxin" << "dotcode" << "ultracode"
			   << "datamatrixrectangular" << "datamatrixrectangularextension" << "rectangularmicroqrcode"
			   << "hibcazteccode" << "hibcdatamatrixrectangular" << "mailmark";
	foreach (const QString& enc, parseAvail)
		resparseAvail[enc] = true;

	QStringList parsefncAvail;
	parsefncAvail << "codablockf" << "code128" << "code16k" << "code49" << "code93" << "codeone"
				  << "datamatrix" << "posicode" << "qrcode" << "microqrcode" << "dotcode" << "ultracode"
				  << "datamatrixrectangular" << "datamatrixrectangularextension" << "rectangularmicroqrcode";
	foreach (const QString& enc, parsefncAvail)
		resparsefncAvail[enc] = true;

	// Building up the bcFamilyCombo grouping the formats for readablity
	ui.bcFamilyCombo->addItem(tr("Select a barcode family")); // to prevent 1st gs call
	ui.bcFamilyCombo->insertSeparator(999);

	// Building up the bcCombo grouping the formats for readablity
	ui.bcCombo->addItem(tr("Select a barcode format")); // to prevent 1st gs call
	ui.bcCombo->insertSeparator(999);

	QString familyName;
	QStringList bcNames;
	bcNames << "EAN-13" << "EAN-8" << "UPC-A" << "UPC-E" << "ISBN" << "ISMN" << "ISSN";
	familyName = tr("Point of Sale");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "EAN-14" << "GS1 Data Matrix" << "GS1 Data Matrix Rectangular" << "GS1 QR Code" << "GS1-128" << "ITF-14" << "SSCC-18"
			<< "GS1 Digital Link QR Code" << "GS1 Digital Link Data Matrix" << "GS1 DotCode";
	familyName = tr("Supply Chain");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "QR Code" << "Micro QR Code" << "Rectangular Micro QR Code" << "Data Matrix" << "Data Matrix Rectangular"
			<< "Data Matrix Rectangular Extension" << "MaxiCode" << "Aztec Code" << "Compact Aztec Code"
			<< "Aztec Runes" << "PDF417" << "Compact PDF417" << "MicroPDF417" << "Han Xin Code"
			<< "DotCode" << "Ultracode";
	familyName = tr("Two-dimensional symbols");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "Code 128" << "Code 39" << "Code 39 Extended" << "Code 93" << "Code 93 Extended"
			<< "Interleaved 2 of 5 (ITF)";
	familyName = tr("One-dimensional symbols");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "GS1 DataBar Omnidirectional" << "GS1 DataBar Stacked Omnidirectional"
			<< "GS1 DataBar Expanded" << "GS1 DataBar Expanded Stacked" << "GS1 DataBar Truncated"
			<< "GS1 DataBar Stacked" << "GS1 DataBar Limited" << "GS1 North American Coupon";
	familyName = tr("GS1 DataBar family");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "AusPost 4 State Customer Code" << "Deutsche Post Identcode" << "Deutsche Post Leitcode"
			<< "Japan Post 4 State Customer Code" << "Royal Dutch TPG Post KIX"
			<< "Royal Mail 4 State Customer Code" << "Royal Mail Mailmark" << "USPS Intelligent Mail" << "USPS PLANET" << "USPS POSTNET";
	familyName = tr("Postal symbols");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "Italian Pharmacode" << "Pharmaceutical Binary Code" << "Two-track Pharmacode"
			<< "Pharmazentralnummer (PZN)" << "HIBC Codablock F" << "HIBC Code 128" << "HIBC Code 39"
			<< "HIBC Data Matrix" << "HIBC Data Matrix Rectangular" << "HIBC MicroPDF417" << "HIBC PDF417" << "HIBC QR Code"
			<< "HIBC Aztec Code";
	familyName = tr("Pharmaceutical symbols");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "Code 11" << "Codabar" << "Code 25" << "COOP 2 of 5" << "Datalogic 2 of 5" << "IATA 2 of 5"
			<< "Industrial 2 of 5" << "Matrix 2 of 5" << "MSI Modified Plessey" << "Plessey UK"
			<< "PosiCode" << "Telepen" << "Telepen Numeric" << "Channel Code"
			<< "Code 16K" << "Codablock F" << "Code 49"
			<< "Code One";
	familyName = tr("Less-used symbols");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "EAN-13 Composite" << "EAN-8 Composite" << "UPC-A Composite" << "UPC-E Composite"
			<< "GS1 DataBar Omnidirectional Composite" << "GS1 DataBar Stacked Omnidirectional Composite"
			<< "GS1 DataBar Expanded Composite" << "GS1 DataBar Expanded Stacked Composite"
			<< "GS1 DataBar Truncated Composite" << "GS1 DataBar Stacked Composite"
			<< "GS1 DataBar Limited Composite" << "GS1-128 Composite";
	familyName = tr("GS1 Composite symbols");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	ui.bcFamilyCombo->addItems(familyList);

	/*
	 *  End of the hard-coded data
	 *
	 */

	guiColor = ui.codeEdit->palette().color(QPalette::Window);

	ui.okButton->setText(CommonStrings::tr_OK);
	ui.cancelButton->setText(CommonStrings::tr_Cancel);
	ui.resetButton->setIcon(IconManager::instance().loadIcon("u_undo"));

	if (ScCore->primaryMainWindow()->doc->PageColors.contains("Black"))
	{
		lnColor = ScCore->primaryMainWindow()->doc->PageColors["Black"];
		txtColor = ScCore->primaryMainWindow()->doc->PageColors["Black"];
		ui.linesLabel->setToolTip("Black");
		ui.txtLabel->setToolTip("Black");
	}
	else
	{
		ui.linesLabel->setToolTip("n.a.");
		ui.txtLabel->setToolTip("n.a.");
	}
	if (ScCore->primaryMainWindow()->doc->PageColors.contains("White"))
	{
		bgColor = ScCore->primaryMainWindow()->doc->PageColors["White"];
		ui.bgLabel->setToolTip("White");
	}
	else
		ui.bgLabel->setToolTip("n.a.");

	paintColorSample(ui.linesLabel, lnColor);
	paintColorSample(ui.txtLabel, txtColor);
	paintColorSample(ui.bgLabel, bgColor);

	paintBarcodeTimer = new QTimer(this);
	paintBarcodeTimer->setSingleShot(true);
	connect(paintBarcodeTimer, SIGNAL(timeout()), this, SLOT(paintBarcode()));

	syncOptionsUITimer = new QTimer(this);
	syncOptionsUITimer->setSingleShot(true);
	connect(syncOptionsUITimer, SIGNAL(timeout()), this, SLOT(syncOptionsUI()));

	connect(ui.bcFamilyCombo, SIGNAL(activated(int)), this, SLOT(bcFamilyComboChanged()));
	connect(ui.bcCombo, SIGNAL(activated(int)), this, SLOT(bcComboChanged()));
	connect(ui.bgColorButton, SIGNAL(clicked()), this, SLOT(bgColorButton_pressed()));
	connect(ui.lnColorButton, SIGNAL(clicked()), this, SLOT(lnColorButton_pressed()));
	connect(ui.txtColorButton, SIGNAL(clicked()), this, SLOT(txtColorButton_pressed()));
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(okButton_pressed()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_pressed()));
	connect(ui.codeEdit, SIGNAL(textChanged(QString)), this, SLOT(codeEdit_textChanged(QString)));
	connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(resetButton_clicked()));
	bcComboChanged();

}

BarcodeGenerator::~BarcodeGenerator()
{
	if (!paintBarcodeTimer)
		return;
	delete paintBarcodeTimer;
	paintBarcodeTimer = nullptr;
}

void BarcodeGenerator::loadBarcode(const QString& encoder, const QString& content, const QString& options)
{
	// Reverse-lookup encoder command to find the display name
	QString displayName;
	QString familyName;
	for (auto it = map.cbegin(); it != map.cend(); ++it)
	{
		if (it.value().command == encoder)
		{
			displayName = it.key();
			break;
		}
	}

	if (displayName.isEmpty())
		return;

	// Find the family containing this display name
	for (auto it = familyItems.cbegin(); it != familyItems.cend(); ++it)
	{
		if (it.value().contains(displayName))
		{
			familyName = it.key();
			break;
		}
	}

	// Set family combo
	if (!familyName.isEmpty())
	{
		int familyIdx = ui.bcFamilyCombo->findText(familyName);
		if (familyIdx >= 0)
		{
			ui.bcFamilyCombo->setCurrentIndex(familyIdx);
			bcFamilyComboChanged();
		}
	}

	// Set barcode format combo
	int bcIdx = ui.bcCombo->findText(displayName);
	if (bcIdx >= 0)
	{
		ui.bcCombo->setCurrentIndex(bcIdx);
		// Don't call bcComboChanged() yet - we'll set content/options first
		updateOptions();
	}

	// Set content and options
	ui.codeEdit->blockSignals(true);
	ui.codeEdit->setText(content);
	ui.codeEdit->blockSignals(false);

	ui.optionsEdit->blockSignals(true);
	ui.optionsEdit->setText(options);
	ui.optionsEdit->blockSignals(false);

	updateUIFromOptionsText();

	// Enable controls
	ui.codeEdit->setEnabled(true);
	ui.optionsEdit->setEnabled(true);
	ui.bgColorButton->setEnabled(true);
	ui.lnColorButton->setEnabled(true);
	ui.txtColorButton->setEnabled(true);
	ui.okButton->setEnabled(true);
}

void BarcodeGenerator::loadFromItem(PageItem* item)
{
	m_editItem = item;

	QString encoder = item->getObjectAttribute("bwipp-encoder").value;
	QString content = item->getObjectAttribute("bwipp-content").value;
	QString options = item->getObjectAttribute("bwipp-options").value;

	loadBarcode(encoder, content, options);
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::loadFromParams(const QMap<QString, QString>& params)
{
	loadBarcode(params.value("bwipp-encoder"),
				params.value("bwipp-content"),
				params.value("bwipp-options"));
}

static bool parseBwippColor(const QString& hex, ScColor& color)
{
	bool ok;
	if (hex.length() == 6)
	{
		// RRGGBB
		int r = hex.mid(0, 2).toInt(&ok, 16); if (!ok) return false;
		int g = hex.mid(2, 2).toInt(&ok, 16); if (!ok) return false;
		int b = hex.mid(4, 2).toInt(&ok, 16); if (!ok) return false;
		color.setRgbColor(r, g, b);
		return true;
	}
	if (hex.length() == 8)
	{
		// CCMMYYKK
		int c = hex.mid(0, 2).toInt(&ok, 16); if (!ok) return false;
		int m = hex.mid(2, 2).toInt(&ok, 16); if (!ok) return false;
		int y = hex.mid(4, 2).toInt(&ok, 16); if (!ok) return false;
		int k = hex.mid(6, 2).toInt(&ok, 16); if (!ok) return false;
		color.setCmykColor(c, m, y, k);
		return true;
	}
	return false;
}

void BarcodeGenerator::updateOptionValue(const QString& key, const QString& value)
{
	QString opts = ui.optionsEdit->text();
	QRegularExpression rx("\\b" + QRegularExpression::escape(key) + "=\\S*");
	if (opts.contains(rx))
		opts.replace(rx, key + "=" + value);
	else
		opts.append(" " + key + "=" + value);
	ui.optionsEdit->blockSignals(true);
	ui.optionsEdit->setText(opts.simplified());
	ui.optionsEdit->blockSignals(false);
}

void BarcodeGenerator::ensureOptionPresent(const QString& key)
{
	QString opts = ui.optionsEdit->text();
	QRegularExpression rx("\\b" + QRegularExpression::escape(key) + "\\b");
	if (!opts.contains(rx))
	{
		opts.append(" " + key);
		ui.optionsEdit->blockSignals(true);
		ui.optionsEdit->setText(opts.simplified());
		ui.optionsEdit->blockSignals(false);
	}
}

void BarcodeGenerator::updateOptions()
{
	QString enc = map[ui.bcCombo->currentText()].command;

	ui.formatLabel->setText(resvlbl.contains(enc) ? resvlbl[enc] + ":" : "Version:");
	ui.formatCombo->blockSignals(true);
	ui.formatCombo->clear();
	ui.formatCombo->addItem("Auto");
	if (resvers.contains(enc))
	{
		ui.formatCombo->insertSeparator(999);
		ui.formatCombo->addItems(resvers[enc].split(","));
		ui.formatLabel->setEnabled(true);
		ui.formatCombo->setEnabled(true);
	}
	else
	{
		ui.formatLabel->setEnabled(false);
		ui.formatCombo->setEnabled(false);
	}
	ui.formatCombo->blockSignals(false);

	ui.eccCombo->blockSignals(true);
	ui.eccCombo->clear();
	ui.eccCombo->addItem("Auto");
	if (resecls.contains(enc))
	{
		ui.eccCombo->insertSeparator(999);
		ui.eccCombo->addItems(resecls[enc].split(","));
		ui.eccLabel->setEnabled(true);
		ui.eccCombo->setEnabled(true);
	}
	else
	{
		ui.eccLabel->setEnabled(false);
		ui.eccCombo->setEnabled(false);
	}
	ui.eccCombo->blockSignals(false);

}


void BarcodeGenerator::bcFamilyComboChanged()
{
	ui.bcCombo->blockSignals(true);
	ui.bcCombo->clear();
	ui.bcCombo->addItem(tr("Select a barcode format")); // to prevent 1st gs call
	ui.bcCombo->insertSeparator(999);
	ui.bcCombo->addItems(familyItems[ui.bcFamilyCombo->currentText()]);
	ui.bcCombo->blockSignals(false);
	bcComboChanged();
}

void BarcodeGenerator::bcComboChanged(int)
{
	bcComboChanged();
}

void BarcodeGenerator::bcComboChanged()
{
	updateOptions();

	if (ui.bcCombo->currentIndex() == 0)
	{
		ui.okButton->setEnabled(false);
		ui.sampleLabel->setText(tr("Select Type"));
		ui.codeEdit->clear();
		ui.codeEdit->setEnabled(false);
		ui.optionsEdit->clear();
		ui.optionsEdit->setEnabled(false);
		ui.includetextCheck->setEnabled(false);
		ui.guardwhitespaceCheck->setEnabled(false);
		ui.includecheckCheck->setEnabled(false);
		ui.includecheckintextCheck->setEnabled(false);
		ui.parseCheck->setEnabled(false);
		ui.parsefncCheck->setEnabled(false);
		ui.formatLabel->setEnabled(false);
		ui.formatCombo->setEnabled(false);
		ui.eccLabel->setEnabled(false);
		ui.eccCombo->setEnabled(false);
		ui.bgColorButton->setEnabled(false);
		ui.lnColorButton->setEnabled(false);
		ui.txtColorButton->setEnabled(false);
		return;
	}

	ui.codeEdit->setEnabled(true);
	ui.optionsEdit->setEnabled(true);
	ui.bgColorButton->setEnabled(true);
	ui.lnColorButton->setEnabled(true);
	ui.txtColorButton->setEnabled(true);
	ui.okButton->setEnabled(true);

	QString s = ui.bcCombo->currentText();
	ui.codeEdit->blockSignals(true);
	ui.codeEdit->setText(map[s].exampleContents);
	ui.codeEdit->blockSignals(false);
	ui.optionsEdit->blockSignals(true);
	ui.optionsEdit->setText(map[s].exampleOptions);
	ui.optionsEdit->blockSignals(false);

	QString enc = map[s].command;
	ui.includetextCheck->setEnabled(resincludetextAvail[enc]);
	ui.guardwhitespaceCheck->setEnabled(resguardwhitespaceAvail[enc]);
	ui.includecheckCheck->setEnabled(resincludecheckAvail[enc]);
	ui.includecheckintextCheck->setEnabled(resincludetextAvail[enc] && resincludecheckintextAvail[enc]);
	ui.parseCheck->setEnabled(resparseAvail[enc]);
	ui.parsefncCheck->setEnabled(resparsefncAvail[enc]);

	updateUIFromOptionsText();

	enqueuePaintBarcode(0);
}

void BarcodeGenerator::enqueuePaintBarcode(int delay)
{
	ui.okButton->setEnabled(false);
	//	paintBarcode();
	paintBarcodeTimer->start(delay);
}

void BarcodeGenerator::updateOptionsTextFromUI()
{
	QString opts = ui.optionsEdit->text();

	const std::initializer_list<std::pair<QCheckBox*, const char*>> boolOpts = {
		{ui.includetextCheck, "includetext"},
		{ui.guardwhitespaceCheck, "guardwhitespace"},
		{ui.includecheckCheck, "includecheck"},
		{ui.includecheckintextCheck, "includecheckintext"},
		{ui.parseCheck, "parse"},
		{ui.parsefncCheck, "parsefnc"},
	};
	for (const auto& [cb, kw] : boolOpts)
	{
		QRegularExpression rx("\\b" + QString(kw) + "\\b");
		if (cb->isChecked())
		{
			if (!opts.contains(rx))
				opts.append(" " + QString(kw));
		}
		else
			opts.replace(rx, " ");
	}

	QString enc = map[ui.bcCombo->currentText()].command;
	QString vlbl = resvlbl.contains(enc) ? resvlbl[enc].toLower() : "version";

	if (ui.formatCombo->currentIndex() != 0)
	{
		QString t = ui.formatCombo->currentText();
		if (!opts.contains(QRegularExpression("\\b" + QRegularExpression::escape(vlbl) + "=.*\\b")))
			opts.append(" " + vlbl + "=" + t);
		else
			opts.replace(QRegularExpression("\\b" + QRegularExpression::escape(vlbl) + "=\\S*\\b"), vlbl + "=" + t);
	}
	else
	{
		opts.replace(QRegularExpression("\\b" + QRegularExpression::escape(vlbl) + "=\\S*\\b"), " ");
	}

	if (ui.eccCombo->currentIndex() != 0)
	{
		QString t = ui.eccCombo->currentText();
		if (!opts.contains(QRegularExpression("\\beclevel=.*\\b")))
			opts.append(" eclevel=" + t);
		else
			opts.replace(QRegularExpression("\\beclevel=\\S*\\b"), "eclevel=" + t);
	}
	else
	{
		opts.replace(QRegularExpression("\\beclevel=\\S*\\b")," ");
	}

	if (ui.inkspreadSlider->value() > 0)
	{
		QString val = QString::number(ui.inkspreadSlider->value() / 100.0, 'f', 2);
		if (!opts.contains(QRegularExpression("\\binkspread=\\S*")))
			opts.append(" inkspread=" + val);
		else
			opts.replace(QRegularExpression("\\binkspread=\\S*"), "inkspread=" + val);
	}
	else
	{
		opts.replace(QRegularExpression("\\binkspread=\\S*"), " ");
	}

	ui.optionsEdit->blockSignals(true);
	ui.optionsEdit->setText(opts.simplified());
	ui.optionsEdit->blockSignals(false);

}

void BarcodeGenerator::updateUIFromOptionsText()
{
	QString opts = ui.optionsEdit->text();

	auto setCheckIfChanged = [](QCheckBox* cb, bool val) {
		if (cb->isChecked() != val)
		{
			cb->blockSignals(true);
			cb->setChecked(val);
			cb->blockSignals(false);
		}
	};

	setCheckIfChanged(ui.includetextCheck, opts.contains(QRegularExpression("\\bincludetext\\b")));
	setCheckIfChanged(ui.guardwhitespaceCheck, opts.contains(QRegularExpression("\\bguardwhitespace\\b")));
	setCheckIfChanged(ui.includecheckCheck, opts.contains(QRegularExpression("\\bincludecheck\\b")));
	setCheckIfChanged(ui.includecheckintextCheck, opts.contains(QRegularExpression("\\bincludecheckintext\\b")));
	setCheckIfChanged(ui.parseCheck, opts.contains(QRegularExpression("\\bparse\\b")));
	setCheckIfChanged(ui.parsefncCheck, opts.contains(QRegularExpression("\\bparsefnc\\b")));

	QString enc = map[ui.bcCombo->currentText()].command;
	QString vlbl = resvlbl.contains(enc) ? resvlbl[enc].toLower() : "version";

	QRegularExpression rxf("\\b" + QRegularExpression::escape(vlbl) + "=(\\S*)\\b");
	QRegularExpressionMatch matchf = rxf.match(opts);
	int fmtIdx = matchf.hasMatch() ? ui.formatCombo->findText(matchf.captured(1)) : 0;
	if (fmtIdx == -1)
		fmtIdx = 0;
	if (ui.formatCombo->currentIndex() != fmtIdx)
	{
		ui.formatCombo->blockSignals(true);
		ui.formatCombo->setCurrentIndex(fmtIdx);
		ui.formatCombo->blockSignals(false);
	}

	QRegularExpression rxe("\\beclevel=(\\S*)\\b");
	QRegularExpressionMatch matche = rxe.match(opts);
	int eccIdx = matche.hasMatch() ? ui.eccCombo->findText(matche.captured(1)) : 0;
	if (eccIdx == -1)
		eccIdx = 0;
	if (ui.eccCombo->currentIndex() != eccIdx)
	{
		ui.eccCombo->blockSignals(true);
		ui.eccCombo->setCurrentIndex(eccIdx);
		ui.eccCombo->blockSignals(false);
	}

	// Sync inkspread slider from options text
	QRegularExpression rxInk("\\binkspread=(\\S*)");
	QRegularExpressionMatch matchInk = rxInk.match(opts);
	int inkVal = 0;
	if (matchInk.hasMatch())
		inkVal = qBound(0, (int)(matchInk.captured(1).toDouble() * 100), 25);
	if (ui.inkspreadSlider->value() != inkVal)
	{
		ui.inkspreadSlider->blockSignals(true);
		ui.inkspreadSlider->setValue(inkVal);
		ui.inkspreadSlider->blockSignals(false);
	}
	ui.inkspreadValue->setText(QString::number(inkVal / 100.0, 'f', 2));


	// Sync color members from options text
	ScColor parsed;

	QRegularExpression rxLn("\\bbarcolor=(\\w+)");
	QRegularExpressionMatch mLn = rxLn.match(opts);
	if (mLn.hasMatch() && parseBwippColor(mLn.captured(1), parsed) && !(parsed == lnColor))
	{
		lnColor = parsed;
		ui.linesLabel->setToolTip(mLn.captured(1));
		paintColorSample(ui.linesLabel, lnColor);
	}

	QRegularExpression rxBg("\\bbackgroundcolor=(\\w+)");
	QRegularExpressionMatch mBg = rxBg.match(opts);
	if (mBg.hasMatch() && parseBwippColor(mBg.captured(1), parsed) && !(parsed == bgColor))
	{
		bgColor = parsed;
		ui.bgLabel->setToolTip(mBg.captured(1));
		paintColorSample(ui.bgLabel, bgColor);
	}

	QRegularExpression rxTxt("\\btextcolor=(\\w+)");
	QRegularExpressionMatch mTxt = rxTxt.match(opts);
	if (mTxt.hasMatch() && parseBwippColor(mTxt.captured(1), parsed) && !(parsed == txtColor))
	{
		txtColor = parsed;
		ui.txtLabel->setToolTip(mTxt.captured(1));
		paintColorSample(ui.txtLabel, txtColor);
	}
}

void BarcodeGenerator::updatePreview(const QString& errorMsg)
{
	QString pngFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.png");
	if (errorMsg.isEmpty())
	{
		ui.sampleLabel->setPixmap(QPixmap(pngFile));
		ui.okButton->setEnabled(true);
	}
	else
	{
		ui.sampleLabel->setText("<qt>" + errorMsg + "</qt>");
	}
}

void BarcodeGenerator::on_includetextCheck_stateChanged(int)
{
	updateOptionsTextFromUI();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_guardwhitespaceCheck_stateChanged(int)
{
	updateOptionsTextFromUI();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_includecheckCheck_stateChanged(int)
{
	updateOptionsTextFromUI();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_includecheckintextCheck_stateChanged(int)
{
	updateOptionsTextFromUI();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_parseCheck_stateChanged(int)
{
	updateOptionsTextFromUI();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_parsefncCheck_stateChanged(int)
{
	updateOptionsTextFromUI();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_formatCombo_currentIndexChanged(int)
{
	updateOptionsTextFromUI();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_eccCombo_currentIndexChanged(int)
{
	updateOptionsTextFromUI();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_inkspreadSlider_valueChanged(int value)
{
	ui.inkspreadValue->setText(QString::number(value / 100.0, 'f', 2));
	updateOptionsTextFromUI();
	enqueuePaintBarcode(debounceInterval);
}

void BarcodeGenerator::paintColorSample(QLabel *l, const ScColor & c)
{
	QPixmap currentPixmap = l->pixmap(Qt::ReturnByValue);
	QSize pixmapSize(currentPixmap.width(), currentPixmap.height());
	if (currentPixmap.isNull())
	{
		QRect rect = l->frameRect();
		double pixelRatio = l->devicePixelRatioF();
		pixmapSize = QSize(rect.width() * pixelRatio, rect.height() * pixelRatio);
	}
	QPixmap pm(pixmapSize.width(), pixmapSize.height());
	pm.fill(c.getRawRGBColor()); // brute force sc2qt color convert for preview
	l->setPixmap(pm);
}

void BarcodeGenerator::bgColorButton_pressed()
{
	ColorsAndFillsDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;

	QString selectedColorName = d.selectedColorName();
	if (selectedColorName == CommonStrings::None)
		return;

	bgColor = d.selectedColor();
	ui.bgLabel->setToolTip(d.selectedColorName());
	paintColorSample(ui.bgLabel, bgColor);
	QString hex = bgColor.name().replace('#', "").toUpper();
	updateOptionValue("backgroundcolor", hex);
	ensureOptionPresent("showbackground");
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::lnColorButton_pressed()
{
	ColorsAndFillsDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;

	QString selectedColorName = d.selectedColorName();
	if (selectedColorName == CommonStrings::None)
		return;

	lnColor = d.selectedColor();
	ui.linesLabel->setToolTip(d.selectedColorName());
	paintColorSample(ui.linesLabel, lnColor);
	QString hex = lnColor.name().replace('#', "").toUpper();
	updateOptionValue("barcolor", hex);
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::txtColorButton_pressed()
{
	ColorsAndFillsDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;

	QString selectedColorName = d.selectedColorName();
	if (selectedColorName == CommonStrings::None)
		return;

	txtColor = d.selectedColor();
	ui.txtLabel->setToolTip(d.selectedColorName());
	paintColorSample(ui.txtLabel, txtColor);
	QString hex = txtColor.name().replace('#', "").toUpper();
	updateOptionValue("textcolor", hex);
	enqueuePaintBarcode(0);
}

bool BarcodeGenerator::generateBarcode(PageItem* replaceItem, double placeX, double placeY)
{
	QString psFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.ps");

	// Write PS file synchronously (the preview path writes it via the
	// render thread, but callers like the silent-regeneration path may
	// not have run a preview)
	{
		QFile f(psFile);
		if (!f.open(QIODevice::WriteOnly))
			return false;
		QTextStream ts(&f);
		ts << buildPSCommand();
	}

	const FileFormat* fmt = LoadSavePlugin::getFormatByExt("ps");
	if (!fmt)
		return false;

	ScribusMainWindow* mw = ScCore->primaryMainWindow();
	ScribusDoc* doc = mw->doc;

	UndoTransaction tran;
	if (UndoManager::undoEnabled())
	{
		tran = UndoManager::instance()->beginTransaction(
					doc->currentPage()->getUName(),
					Um::IImageFrame,
					Um::ImportBarcode,
					ui.bcCombo->currentText() + " (" + ui.codeEdit->text() + ")",
					Um::IEPS);
	}

	// Save geometry of item being replaced
	double ox = 0, oy = 0, orot = 0, scaleX = 1.0, scaleY = 1.0;
	bool oar = false;
	if (replaceItem)
	{
		ox = replaceItem->xPos();
		oy = replaceItem->yPos();
		orot = replaceItem->rotation();
		oar = replaceItem->aspectRatioLocked();
		double storedNativeW = replaceItem->getObjectAttribute("bwipp-nativeWidth").value.toDouble();
		double storedNativeH = replaceItem->getObjectAttribute("bwipp-nativeHeight").value.toDouble();
		scaleX = (storedNativeW > 0) ? replaceItem->width() / storedNativeW : 1.0;
		scaleY = (storedNativeH > 0) ? replaceItem->height() / storedNativeH : 1.0;
	}

	int itemsBefore = doc->Items->count();
	fmt->loadFile(psFile, LoadSavePlugin::lfUseCurrentPage
				  | LoadSavePlugin::lfInteractive
				  | LoadSavePlugin::lfScripted
				  | LoadSavePlugin::lfNoDialogs
				  | LoadSavePlugin::lfLockAspectRatio);

	PageItem* newItem = nullptr;
	double nativeW = 0, nativeH = 0;
	if (doc->Items->count() > itemsBefore)
	{
		newItem = doc->Items->last();
		nativeW = newItem->width();
		nativeH = newItem->height();
	}

	if (newItem && replaceItem)
	{
		// Apply relative scaling and restore geometry
		newItem->setXYPos(ox, oy);
		newItem->setWidthHeight(nativeW * scaleX, nativeH * scaleY);
		newItem->SetRectFrame();
		newItem->ClipEdited = true;
		newItem->setRotation(orot);
		newItem->setAspectRatioLocked(oar);

		// Delete original, select replacement
		doc->m_Selection->clear();
		doc->m_Selection->addItem(replaceItem);
		doc->itemSelection_DeleteItem();
		doc->m_Selection->clear();
		doc->m_Selection->addItem(newItem);
	}
	else if (newItem && placeX >= 0 && placeY >= 0)
	{
		newItem->setXYPos(placeX, placeY);
	}
	else if (newItem)
	{
		// New barcode: center on current page
		ScPage* page = doc->currentPage();
		newItem->setXYPos(
					page->xOffset() + (page->width() - nativeW) / 2.0,
					page->yOffset() + (page->height() - nativeH) / 2.0);
		doc->m_Selection->setGroupRect();
		if (doc->view())
			doc->view()->DrawNew();
	}

	// Attach barcode attributes directly on the new item
	if (newItem)
	{
		ObjAttrVector attrs;
		auto addAttr = [&attrs](const QString& name, const QString& value) {
			ObjectAttribute attr;
			attr.name = name;
			attr.type = "string";
			attr.value = value;
			attrs.append(attr);
		};
		addAttr("bwipp-encoder", map[ui.bcCombo->currentText()].command);
		addAttr("bwipp-content", ui.codeEdit->text());
		addAttr("bwipp-options", ui.optionsEdit->text());
		addAttr("bwipp-nativeWidth", QString::number(nativeW, 'f', 6));
		addAttr("bwipp-nativeHeight", QString::number(nativeH, 'f', 6));
		addAttr("plugin-editAction", "BarcodeGenerator");
		newItem->setObjectAttributes(&attrs);
	}

	if (tran)
		tran.commit();

	return true;
}

void BarcodeGenerator::okButton_pressed()
{
	hide();
	generateBarcode(m_editItem);
	m_editItem = nullptr;
	accept();
}

void BarcodeGenerator::cancelButton_pressed()
{
	reject();
}

void BarcodeGenerator::codeEdit_textChanged(const QString&)
{
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_optionsEdit_textChanged(const QString&)
{
	syncOptionsUITimer->start(debounceInterval);
}

void BarcodeGenerator::syncOptionsUI()
{
	updateUIFromOptionsText();
	enqueuePaintBarcode(0);
}

QString BarcodeGenerator::buildPSCommand()
{
	QString opts = ui.optionsEdit->text();

	// Only append default colors for values NOT already in the options string
	if (!opts.contains(QRegularExpression("\\bbarcolor=")))
		opts += " barcolor=" + lnColor.name().replace('#', "").toUpper();
	if (!opts.contains(QRegularExpression("\\bbackgroundcolor=")))
		opts += " showbackground backgroundcolor=" + bgColor.name().replace('#', "").toUpper();
	if (!opts.contains(QRegularExpression("\\btextcolor=")))
		opts += " textcolor=" + txtColor.name().replace('#', "").toUpper();

	// Assemble PS from encoder and requirement bodies
	QString psCommand = "%!PS-Adobe-2.0 EPSF-2.0\n"
					"currentglobal true setglobal\n"
					"/uk.co.terryburton.bwipp.global_ctx << /default_inkspread 0 >> def\n"
					"setglobal\n";
	QString req;
	QString enc = map[ui.bcCombo->currentText()].command;
	foreach (req, resreqs[enc].split(" "))
		psCommand.append(resbodys[req]);
	psCommand.append(resbodys[enc]);
	psCommand.append(
				"errordict begin\n"
				"/handleerror {\n"
				"$error begin\n"
				"errorname dup length string cvs 0 6 getinterval (bwipp.) eq {\n"
				"(%stderr) (w) file\n"
				"dup (\nBWIPP ERROR: ) writestring\n"
				"dup errorname dup length string cvs writestring\n"
				"dup ( ) writestring\n"
				"dup errorinfo dup length string cvs writestring\n"
				"dup (\n) writestring\n"
				"dup flushfile end quit\n"
				"} if\n"
				"end //handleerror exec\n"
				"} bind def\n"
				"end\n"
				);
	QString comm("20 10 moveto <%1> <%2> /%3 /uk.co.terryburton.bwipp findresource exec\n");
	QString bcString = ui.codeEdit->text();
	QByteArray bcLatin1 = ui.codeEdit->text().toLatin1();
	QByteArray bcUtf8 = ui.codeEdit->text().toUtf8();
	QByteArray bcArray = (bcString != QString::fromLatin1(bcLatin1)) ? ("\xef\xbb\xbf" + bcUtf8) : bcLatin1;
	QString bcdata(bcArray.toHex());
	QString bcopts(opts.toLatin1().toHex());
	comm = comm.arg(bcdata, bcopts, map[ui.bcCombo->currentText()].command);
	psCommand.append(comm);
	psCommand.append("showpage\n");

	return psCommand;
}

void BarcodeGenerator::paintBarcode()
{
	thread.render(buildPSCommand());
}


void BarcodeGenerator::resetButton_clicked()
{
	bcComboChanged();
}
