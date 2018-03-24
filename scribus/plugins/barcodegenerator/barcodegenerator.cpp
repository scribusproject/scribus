/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/


#include "../formatidlist.h"
#include "barcodegenerator.h"
#include "commonstrings.h"
#include "iconmanager.h"
#include "loadsaveplugin.h"
#include "scpaths.h"
#include "scribuscore.h"
#include "ui/colorsandfills.h"
#include "undomanager.h"

#include <QDebug>
#include <QTextStream>
#include <QStandardItemModel>
#include <QTimer>
#include <QThread>

BarcodeType::BarcodeType(const QString &cmd, const QString &exa, const QString &exaop)
{
	command = cmd;
	exampleContents = exa;
	exampleOptions = exaop;
}

BarcodeGenerator::BarcodeGenerator(QWidget* parent, const char* name)
	: QDialog(parent),
	  paintBarcodeTimer(NULL)
{
	ui.setupUi(this);
	setObjectName(name);
	setModal(true);

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

	QRegExp rx(
		"[\\r\\n]+% --BEGIN (RESOURCE|RENDERER|ENCODER) ([\\w-]+)--[\\r\\n]+"
		"(.*[\\r\\n]+)?"
		"(%%BeginResource.*[\\r\\n]+)"
		"% --END \\1 \\2--[\\r\\n]+");
	rx.setMinimal(true);
	int pos = 0;
	while ( (pos = rx.indexIn(bwipp, pos)) != -1 )
	{
		int len=rx.matchedLength();
		QString restype=rx.cap(1);
	 	QString resname=rx.cap(2);
	 	QString reshead=rx.cap(3);
	 	QString resbody=rx.cap(4);

	 	resbodys[resname]=resbody;

	 	if (restype=="ENCODER")
		{
			QRegExp rxhead(
				"% --REQUIRES (.*)--[\\r\\n]+"
				"% --DESC:(.*)[\\r\\n]+"
				"% --EXAM:(.*)[\\r\\n]+"
				"% --EXOP:(.*)[\\r\\n]+"
				"% --RNDR:(.*)[\\r\\n]+"
			);
			rxhead.indexIn(reshead);
			resreqs[resname]=rxhead.cap(1).trimmed();
			resdescs[resname]=rxhead.cap(2).trimmed();
			resexams[resname]=rxhead.cap(3).trimmed();
			resexops[resname]=rxhead.cap(4).trimmed();
			resrndrs[resname]=rxhead.cap(5).trimmed();
			encoderlist.append(resname);
	 	}
		pos+=len;
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
	resvers["qrcode"]="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40";
	resecls["qrcode"]="L,Q,M,H";
	resvers["gs1qrcode"]=resvers["qrcode"];
	resecls["gs1qrcode"]=resecls["qrcode"];
	resvers["hibcqrcode"]=resvers["qrcode"];
	resecls["hibcqrcode"]=resecls["qrcode"];
	resvers["microqrcode"]="M1,M2,M3,M4";
	resecls["microqrcode"]="L,Q,M,H";
	resvers["datamatrix"]="10x10,12x12,14x14,16x16,18x18,20x20,22x22,24x24,26x26,32x32,36x36,40x40,44x44,48x48,52x52,64x64,72x72,80x80,88x88,96x96,104x104,120x120,132x132,144x144,8x18,8x32,12x26,12x36,16x36,16x48";
	resecls["datamatrix"]="";
	resvers["gs1datamatrix"]=resvers["datamatrix"];
	resecls["gs1datamatrix"]=resvers["datamatrix"];
	resvers["hibcdatamatrix"]=resvers["datamatrix"];
	resecls["hibcdatamatrix"]=resecls["datamatrix"];
	resvers["azteccode"]="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32";
	resvlbl["azteccode"]="Layers";
	resecls["azteccode"]="5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95";
	resvers["azteccodecompact"]="1,2,3,4";
	resvlbl["azteccodecompact"]=resvlbl["azteccode"];
	resecls["azteccodecompact"]=resecls["azteccode"];
	resvers["pdf417"]="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30";
	resvlbl["pdf417"]="Columns";
	resecls["pdf417"]="1,2,3,4,5";
	resvers["pdf417compact"]=resvers["pdf417"];
	resvlbl["pdf417compact"]=resvlbl["pdf417"];
	resecls["pdf417compact"]=resecls["pdf417"];
	resvers["hibcpdf417"]=resvers["pdf417"];
	resvlbl["hibcpdf417"]=resvlbl["pdf417"];
	resecls["hibcpdf417"]=resecls["pdf417"];
	resvers["micropdf417"]="1x11,1x14,1x17,1x20,1x24,1x28,2x8,2x11,2x14,2x17,2x20,2x23,2x26,3x6,3x8,3x10,3x12,3x15,3x20,3x26,3x32,3x38,3x44,4x4,4x6,4x8,4x10,4x12,4x15,4x20,4x26,4x32,4x38,4x44";
	resecls["micropdf417"]="";
	resvers["hibcmicropdf417"]=resvers["micropdf417"];
	resecls["hibcmicropdf417"]=resecls["micropdf417"];
	resvers["hanxin"]="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84";
	resecls["hanxin"]="L1,L2,L3,L4";
	resecls["ultracode"]="EC0,EC1,EC2,EC3,EC4,EC5";


	// Which options checkboxes are enabled for each encoder
	QStringList includetextAvail=encoderlist;
	foreach (const QString& enc, includetextAvail)
		resincludetextAvail[enc]=true; 

	QStringList guardwhitespaceAvail;
	guardwhitespaceAvail << "ean13" << "ean8" << "isbn" << "ismn" << "issn" << "ean13composite" << "ean8composite";
	foreach (const QString& enc, guardwhitespaceAvail)
		resguardwhitespaceAvail[enc]=true; 

	QStringList includecheckAvail;
	includecheckAvail << "bc412" << "channelcode" << "code11" << "code2of5" << "coop2of5" << "datalogic2of5"
					  << "iata2of5" << "industrial2of5" << "matrix2of5" << "code39" << "code39ext"
					  << "code93" << "code93ext" << "interleaved2of5" << "msi" << "rationalizedCodabar";
	foreach (const QString& enc, includecheckAvail)
		resincludecheckAvail[enc]=true; 

	QStringList includecheckintextAvail;
	includecheckintextAvail << "bc412" << "code11" << "code2of5" << "coop2of5" << "datalogic2of5" << "iata2of5"
							<< "industrial2of5" << "matrix2of5" << "code39" << "code39ext" << "interleaved2of5"
							<< "japanpost" << "msi" << "planet" << "plessey" << "postnet" << "rationalizedCodabar" << "royalmail";
	foreach (const QString& enc, includecheckintextAvail)
		resincludecheckintextAvail[enc]=true; 

	QStringList parseAvail;
	parseAvail << "azteccode" << "azteccodecompact" << "codablockf" << "hibccodablockf" << "code128" << "hibccode128" << "code16k" << "code39ext" << "code49"
			   << "code93ext" << "codeone" << "datamatrix" << "hibcdatamatrix" << "maxicode" << "micropdf417" << "hibcmicropdf417" << "pdf417" << "hibcpdf417"
			   << "pdf417compact" << "posicode" << "qrcode" << "hibcqrcode" << "microqrcode" << "telepen" << "hanxin" << "dotcode" << "ultracode";
	foreach (const QString& enc, parseAvail)
		resparseAvail[enc]=true; 

	QStringList parsefncAvail;
	parsefncAvail << "codablockf" << "code128" << "code16k" << "code49" << "code93" << "codeone"
				  << "datamatrix" << "posicode" << "qrcode" << "microqrcode" << "dotcode" << "ultracode";
	foreach (const QString& enc, parsefncAvail)
		resparsefncAvail[enc]=true; 

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
	bcNames << "GS1-14" << "GS1 Data Matrix" << "GS1 QR Code" << "GS1-128" << "ITF-14" << "SSCC-18";
	familyName = tr("Supply Chain");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "QR Code" << "Micro QR Code" << "Data Matrix" << "Aztec Code" << "Compact Aztec Code"
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
			<< "Royal Mail 4 State Customer Code" << "USPS Intelligent Mail" << "USPS PLANET" << "USPS POSTNET";
	familyName = tr("Postal symbols");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "Italian Pharmacode" << "Pharmaceutical Binary Code" << "Two-track Pharmacode"
			<< "Pharmazentralnummer (PZN)" << "HIBC Codablock F" << "HIBC Code 128" << "HIBC Code 39"
			<< "HIBC Data Matrix" << "HIBC MicroPDF417" << "HIBC PDF417" << "HIBC QR Code";
	familyName = tr("Pharmaceutical symbols");
	familyList.append(familyName);
	familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "Code 11" << "Codabar" << "Code 25" << "COOP 2 of 5" << "Datalogic 2 of 5" << "IATA 2 of 5"
			<< "Industrial 2 of 5" << "Matrix 2 of 5" << "MSI Modified Plessey" << "Plessey UK"
			<< "PosiCode" << "Telepen" << "Telepen Numeric"<< "Code 16K" << "Codablock F" << "Code 49"
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
	ui.resetButton->setIcon(IconManager::instance()->loadIcon("u_undo16.png"));

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

	paintBarcodeTimer=new QTimer(this);
	paintBarcodeTimer->setSingleShot(true);
	connect(paintBarcodeTimer, SIGNAL(timeout()), this, SLOT(paintBarcode()));

	connect(ui.bcFamilyCombo, SIGNAL(activated(QString)), this, SLOT(bcFamilyComboChanged()));
	connect(ui.bcCombo, SIGNAL(activated(QString)), this, SLOT(bcComboChanged()));
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
	paintBarcodeTimer=NULL;
}

void BarcodeGenerator::updateOptions()
{
	QString enc=map[ui.bcCombo->currentText()].command;

	ui.formatLabel->setText(resvlbl[enc]!="" ? resvlbl[enc]+":" : "Version:");
	ui.formatCombo->blockSignals(true);
	ui.formatCombo->clear();
	ui.formatCombo->addItem("Auto");
	if (resvers[enc]!="")
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
	if (resecls[enc]!="")
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

void BarcodeGenerator::bcComboChanged(int i)
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

	QString enc=map[s].command;
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
	QString opts=ui.optionsEdit->text();

	if (ui.includetextCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bincludetext\\b")))
			opts.append(" includetext");
	}
	else
	{
		opts.replace(QRegExp("\\bincludetext\\b")," "); 
	}

	if (ui.guardwhitespaceCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bguardwhitespace\\b"))) 
			opts.append(" guardwhitespace");
	}
	else
	{
		opts.replace(QRegExp("\\bguardwhitespace\\b")," "); 
	}

	if (ui.includecheckCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bincludecheck\\b")))
			opts.append(" includecheck");
	}
	else
	{
		opts.replace(QRegExp("\\bincludecheck\\b")," ");
	}

	if (ui.includecheckintextCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bincludecheckintext\\b")))
			opts.append(" includecheckintext");
	}
	else
	{
		opts.replace(QRegExp("\\bincludecheckintext\\b")," ");
	}

	if (ui.parseCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bparse\\b")))
			opts.append(" parse");
	}
	else
	{
		opts.replace(QRegExp("\\bparse\\b")," ");
	}

	if (ui.parsefncCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bparsefnc\\b"))) 
			opts.append(" parsefnc");
	}
	else
	{
		opts.replace(QRegExp("\\bparsefnc\\b")," ");
	}

	QString enc=map[ui.bcCombo->currentText()].command;
	QString vlbl=resvlbl[enc]!="" ? resvlbl[enc].toLower() : "version";

 	if (ui.formatCombo->currentIndex() != 0)
	{
		QString t=ui.formatCombo->currentText();
		if (!opts.contains(QRegExp("\\b"+QRegExp::escape(vlbl)+"=.*\\b"))) 
			opts.append(" "+vlbl+"="+t);
		else
			opts.replace(QRegExp("\\b"+QRegExp::escape(vlbl)+"=\\S*\\b"),vlbl+"="+t);
	}
	else
	{
		opts.replace(QRegExp("\\b"+QRegExp::escape(vlbl)+"=\\S*\\b")," ");
	}

	if (ui.eccCombo->currentIndex() != 0)
	{
		QString t=ui.eccCombo->currentText();
		if (!opts.contains(QRegExp("\\beclevel=.*\\b"))) 
			opts.append(" eclevel="+t);
		else
			opts.replace(QRegExp("\\beclevel=\\S*\\b"),"eclevel="+t);
	}
	else
	{
		opts.replace(QRegExp("\\beclevel=\\S*\\b")," ");
	}

	ui.optionsEdit->blockSignals(true);
	ui.optionsEdit->setText(opts.simplified());
	ui.optionsEdit->blockSignals(false);

}

void BarcodeGenerator::updateUIFromOptionsText()
{
	ui.includetextCheck->blockSignals(true);	
	ui.includetextCheck->setChecked(ui.optionsEdit->text().contains(QRegExp("\\bincludetext\\b")));
	ui.includetextCheck->blockSignals(false);	

	ui.guardwhitespaceCheck->blockSignals(true);	
	ui.guardwhitespaceCheck->setChecked(ui.optionsEdit->text().contains(QRegExp("\\bguardwhitespace\\b")));
	ui.guardwhitespaceCheck->blockSignals(false);	

	ui.includecheckCheck->blockSignals(true);	
	ui.includecheckCheck->setChecked(ui.optionsEdit->text().contains(QRegExp("\\bincludecheck\\b")));
	ui.includecheckCheck->blockSignals(false);	

	ui.includecheckintextCheck->blockSignals(true);	
	ui.includecheckintextCheck->setChecked(ui.optionsEdit->text().contains(QRegExp("\\bincludecheckintext\\b")));
	ui.includecheckintextCheck->blockSignals(false);	

	ui.parseCheck->blockSignals(true);	
	ui.parseCheck->setChecked(ui.optionsEdit->text().contains(QRegExp("\\bparse\\b")));
	ui.parseCheck->blockSignals(false);	

	ui.parsefncCheck->blockSignals(true);	
	ui.parsefncCheck->setChecked(ui.optionsEdit->text().contains(QRegExp("\\bparsefnc\\b")));
	ui.parsefncCheck->blockSignals(false);	

	QString enc=map[ui.bcCombo->currentText()].command;
	QString vlbl=resvlbl[enc]!="" ? resvlbl[enc].toLower() : "version";

	QRegExp rxf("\\b"+QRegExp::escape(vlbl)+"=(\\S*)\\b");
	ui.formatCombo->blockSignals(true);	
	if (ui.optionsEdit->text().contains(rxf))
	{
		int idx=ui.formatCombo->findText(rxf.cap(1));
		if (idx == -1)
			idx=0;
		ui.formatCombo->setCurrentIndex(idx);
	}
	else
	{
		ui.formatCombo->setCurrentIndex(0);
	}
	ui.formatCombo->blockSignals(false);	

	QRegExp rxe("\\beclevel=(\\S*)\\b");
	ui.eccCombo->blockSignals(true);	
	if (ui.optionsEdit->text().contains(rxe))
	{
		int idx=ui.eccCombo->findText(rxe.cap(1));
		if (idx == -1)
			idx=0;
		ui.eccCombo->setCurrentIndex(idx);
	}
	else
	{
		ui.eccCombo->setCurrentIndex(0);
	}
	ui.eccCombo->blockSignals(false);
}

void BarcodeGenerator::updatePreview(QString errorMsg)
{
	QString pngFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.png");
	if (errorMsg=="")
	{
		ui.sampleLabel->setPixmap(QPixmap(pngFile));
		ui.okButton->setEnabled(true);
	}
	else
	{
		ui.sampleLabel->setText("<qt>"+errorMsg+"</qt>");
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

void BarcodeGenerator::paintColorSample(QLabel *l, const ScColor & c)
{
	QRect rect = l->frameRect();
	QPixmap pm(rect.width(), rect.height());
	pm.fill(c.getRawRGBColor()); // brute force sc2qt color convert for preview
	l->setPixmap(pm);
}

void BarcodeGenerator::bgColorButton_pressed()
{
	ColorsAndFillsDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;
	bgColor = d.selectedColor();
	ui.bgLabel->setToolTip(d.selectedColorName());
	paintColorSample(ui.bgLabel, bgColor);
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::lnColorButton_pressed()
{
	ColorsAndFillsDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;
	lnColor = d.selectedColor();
	ui.linesLabel->setToolTip(d.selectedColorName());
	paintColorSample(ui.linesLabel, lnColor);
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::txtColorButton_pressed()
{
	ColorsAndFillsDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;
	txtColor = d.selectedColor();
	ui.txtLabel->setToolTip(d.selectedColorName());
	paintColorSample(ui.txtLabel, txtColor);
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::okButton_pressed()
{

	QString psFile = QDir::toNativeSeparators(ScPaths::tempFileDir() + "bcode.ps");

	// no need to call paintBarcode(pngFile, 300); because
	// it's created by previous run...
	hide();
	const FileFormat * fmt = LoadSavePlugin::getFormatByExt("ps");

	UndoTransaction tran;
	if (UndoManager::undoEnabled())
	{
		tran = UndoManager::instance()->beginTransaction(
							ScCore->primaryMainWindow()->doc->currentPage()->getUName(),
							Um::IImageFrame,
							Um::ImportBarcode,
							ui.bcCombo->currentText() + " (" + ui.codeEdit->text() + ")",
							Um::IEPS);
	}

	if (fmt)
	{
		fmt->loadFile(psFile, LoadSavePlugin::lfUseCurrentPage|LoadSavePlugin::lfInteractive);
		if (tran)
			tran.commit();
	}
	accept();
}

void BarcodeGenerator::cancelButton_pressed()
{
	reject();
}

void BarcodeGenerator::codeEdit_textChanged(const QString& s)
{
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::on_optionsEdit_textChanged(const QString &s )
{
	updateUIFromOptionsText();
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::paintBarcode()
{

	QString coloropts("barcolor=%1 showbackground backgroundcolor=%2 textcolor=%3");
	coloropts = coloropts.arg(lnColor.name().replace('#', "")) \
			.arg(bgColor.name().replace('#', "")) \
			.arg(txtColor.name().replace('#', ""));

	QString opts=ui.optionsEdit->text()+" "+coloropts;

	// Assemble PS from encoder and requirement bodies
	QString psCommand="%!PS-Adobe-2.0 EPSF-2.0\n";
	QString req;
	QString enc=map[ui.bcCombo->currentText()].command;
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
	QString bcdata(ui.codeEdit->text().toLatin1().toHex());
	QString bcopts(opts.toLatin1().toHex());
	comm = comm.arg(bcdata).arg(bcopts).arg(map[ui.bcCombo->currentText()].command);
	psCommand.append(comm);
	psCommand.append("showpage\n");

	thread.render(psCommand);
}


void BarcodeGenerator::resetButton_clicked()
{
	bcComboChanged();
}
