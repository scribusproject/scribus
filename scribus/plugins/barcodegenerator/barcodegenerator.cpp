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
#include "ui/paintmanager.h"
#include "undomanager.h"


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
	: QDialog(parent)
{
	m_ui.setupUi(this);
	setObjectName(name);
	setModal(true);

	connect(&m_thread, SIGNAL(renderedImage(QString)),this, SLOT(updatePreview(QString)));

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

	QFile f( ScPaths::instance().shareDir() + QString("/plugins/barcode.ps") );
	f.open(QIODevice::ReadOnly);
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

	 	m_resbodys[resname]=resbody;

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
			m_resreqs[resname]=rxhead.cap(1).trimmed();
			m_resdescs[resname]=rxhead.cap(2).trimmed();
			m_resexams[resname]=rxhead.cap(3).trimmed();
			m_resexops[resname]=rxhead.cap(4).trimmed();
			m_resrndrs[resname]=rxhead.cap(5).trimmed();
			m_encoderlist.append(resname);
	 	}
		pos+=len;
	}

	QString enc;
	foreach (enc, m_encoderlist) m_map[m_resdescs[enc]] = BarcodeType(enc, m_resexams[enc], m_resexops[enc]);

	/*
	 *  Ultimately all of this static data about the capabilities of each barcode
	 *  encoder will be replaced by data read from the barcode.ps metadata, when 
	 *  such data exists...
	 *
	 */ 

	// Content for the version and ecc combos
	m_resvers["qrcode"]="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40";
	m_resecls["qrcode"]="L,Q,M,H";
	m_resvers["gs1qrcode"]=m_resvers["qrcode"];
	m_resecls["gs1qrcode"]=m_resecls["qrcode"];
	m_resvers["hibcqrcode"]=m_resvers["qrcode"];
	m_resecls["hibcqrcode"]=m_resecls["qrcode"];
	m_resvers["microqrcode"]="M1,M2,M3,M4";
	m_resecls["microqrcode"]="L,Q,M,H";
	m_resvers["datamatrix"]="10x10,12x12,14x14,16x16,18x18,20x20,22x22,24x24,26x26,32x32,36x36,40x40,44x44,48x48,52x52,64x64,72x72,80x80,88x88,96x96,104x104,120x120,132x132,144x144,8x18,8x32,12x26,12x36,16x36,16x48";
	m_resecls["datamatrix"]="";
	m_resvers["gs1datamatrix"]=m_resvers["datamatrix"];
	m_resecls["gs1datamatrix"]=m_resvers["datamatrix"];
	m_resvers["hibcdatamatrix"]=m_resvers["datamatrix"];
	m_resecls["hibcdatamatrix"]=m_resecls["datamatrix"];
	m_resvers["azteccode"]="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32";
	m_resvlbl["azteccode"]="Layers";
	m_resecls["azteccode"]="5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95";
	m_resvers["azteccodecompact"]="1,2,3,4";
	m_resvlbl["azteccodecompact"]=m_resvlbl["azteccode"];
	m_resecls["azteccodecompact"]=m_resecls["azteccode"];
	m_resvers["pdf417"]="1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30";
	m_resvlbl["pdf417"]="Columns";
	m_resecls["pdf417"]="1,2,3,4,5";
	m_resvers["pdf417compact"]=m_resvers["pdf417"];
	m_resvlbl["pdf417compact"]=m_resvlbl["pdf417"];
	m_resecls["pdf417compact"]=m_resecls["pdf417"];
	m_resvers["hibcpdf417"]=m_resvers["pdf417"];
	m_resvlbl["hibcpdf417"]=m_resvlbl["pdf417"];
	m_resecls["hibcpdf417"]=m_resecls["pdf417"];
	m_resvers["micropdf417"]="1x11,1x14,1x17,1x20,1x24,1x28,2x8,2x11,2x14,2x17,2x20,2x23,2x26,3x6,3x8,3x10,3x12,3x15,3x20,3x26,3x32,3x38,3x44,4x4,4x6,4x8,4x10,4x12,4x15,4x20,4x26,4x32,4x38,4x44";
	m_resecls["micropdf417"]="";
	m_resvers["hibcmicropdf417"]=m_resvers["micropdf417"];
	m_resecls["hibcmicropdf417"]=m_resecls["micropdf417"];

	// Which options checkboxes are enabled for each encoder
	QStringList includetextAvail=m_encoderlist;
	foreach (enc,includetextAvail)
		m_resincludetextAvail[enc]=true; 

	QStringList guardwhitespaceAvail;
	guardwhitespaceAvail << "ean13" << "ean8" << "isbn" << "ismn" << "issn" << "ean13composite";
	guardwhitespaceAvail << "ean8composite";
	foreach (enc,guardwhitespaceAvail)
		m_resguardwhitespaceAvail[enc]=true; 

	QStringList includecheckAvail;
	includecheckAvail << "bc412" << "channelcode" << "code11" << "code2of5" << "coop2of5" << "datalogic2of5";
	includecheckAvail << "iata2of5" << "industrial2of5" << "matrix2of5" << "code39" << "code39ext";
	includecheckAvail << "code93" << "code93ext" << "interleaved2of5" << "msi" << "rationalizedCodabar"; 
	foreach (enc,includecheckAvail)
		m_resincludecheckAvail[enc]=true; 

	QStringList includecheckintextAvail;
	includecheckintextAvail << "bc412" << "code11" << "code2of5" << "coop2of5" << "datalogic2of5" << "iata2of5";
	includecheckintextAvail << "industrial2of5" << "matrix2of5" << "code39" << "code39ext" << "interleaved2of5";
	includecheckintextAvail << "japanpost" << "msi" << "planet" << "plessey" << "postnet" << "rationalizedCodabar" << "royalmail";
	foreach (enc,includecheckintextAvail)
		m_resincludecheckintextAvail[enc]=true; 

	QStringList parseAvail;
	parseAvail << "azteccode" << "azteccodecompact" << "codablockf" << "hibccodablockf" << "code128" << "hibccode128" << "code16k" << "code39ext" << "code49";
	parseAvail << "code93ext" << "codeone" << "datamatrix" << "hibcdatamatrix" << "maxicode" << "micropdf417" << "hibcmicropdf417" << "pdf417" << "hibcpdf417" << "pdf417compact";
	parseAvail << "posicode" << "qrcode" << "hibcqrcode" << "microqrcode" << "telepen";
	foreach (enc,parseAvail)
		m_resparseAvail[enc]=true; 

	QStringList parsefncAvail;
	parsefncAvail << "codablockf" << "code128" << "code16k" << "code49" << "code93" << "codeone";
	parsefncAvail << "datamatrix" << "posicode" << "qrcode" << "microqrcode";
	foreach (enc,parsefncAvail)
		m_resparsefncAvail[enc]=true; 

	// Building up the bcFamilyCombo grouping the formats for readablity
	m_ui.bcFamilyCombo->addItem(tr("Select a barcode family")); // to prevent 1st gs call
	m_ui.bcFamilyCombo->insertSeparator(999);

	// Building up the bcCombo grouping the formats for readablity
	m_ui.bcCombo->addItem(tr("Select a barcode format")); // to prevent 1st gs call
	m_ui.bcCombo->insertSeparator(999);

	QString familyName;
	QStringList bcNames;
	bcNames << "EAN-13" << "EAN-8" << "UPC-A" << "UPC-E" << "ISBN" << "ISMN" << "ISSN";
	familyName = tr("Point of Sale");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "GS1-14" << "GS1 Data Matrix" << "GS1 QR Code" << "GS1-128" << "ITF-14" << "SSCC-18";
	familyName = tr("Supply Chain");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "QR Code" << "Micro QR Code" << "Data Matrix" << "Aztec Code" << "Compact Aztec Code";
	bcNames << "Aztec Runes" << "PDF417" << "Compact PDF417" << "MicroPDF417";
	familyName = tr("Two-dimensional symbols");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "Code 128" << "Code 39" << "Code 39 Extended" << "Code 93" << "Code 93 Extended";
	bcNames << "Interleaved 2 of 5 (ITF)";
	familyName = tr("One-dimensional symbols");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "GS1 DataBar Omnidirectional" << "GS1 DataBar Stacked Omnidirectional";
	bcNames << "GS1 DataBar Expanded" << "GS1 DataBar Expanded Stacked" << "GS1 DataBar Truncated";
	bcNames << "GS1 DataBar Stacked" << "GS1 DataBar Limited";
	familyName = tr("GS1 DataBar family");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "AusPost 4 State Customer Code" << "Deutsche Post Identcode" << "Deutsche Post Leitcode";
	bcNames << "Japan Post 4 State Customer Code" << "Royal Dutch TPG Post KIX";
	bcNames << "Royal Mail 4 State Customer Code" << "USPS Intelligent Mail" << "USPS PLANET" << "USPS POSTNET";
	familyName = tr("Postal symbols");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "Italian Pharmacode" << "Pharmaceutical Binary Code" << "Two-track Pharmacode";
	bcNames << "Pharmazentralnummer (PZN)" << "HIBC Codablock F" << "HIBC Code 128" << "HIBC Code 39";
	bcNames << "HIBC Data Matrix" << "HIBC MicroPDF417" << "HIBC PDF417" << "HIBC QR Code";
	familyName = tr("Pharmaceutical symbols");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "Code 11" << "Codabar" << "Code 25" << "COOP 2 of 5" << "Datalogic 2 of 5" << "IATA 2 of 5";
	bcNames << "Industrial 2 of 5" << "Matrix 2 of 5" << "MSI Modified Plessey" << "Plessey UK";
	bcNames << "PosiCode" << "Telepen" << "Telepen Numeric"<< "Code 16K" << "Codablock F" << "Code 49";
	bcNames << "Code One";
	familyName = tr("Less-used symbols");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	bcNames.clear();
	bcNames << "EAN-13 Composite" << "EAN-8 Composite" << "UPC-A Composite" << "UPC-E Composite";
	bcNames << "GS1 DataBar Omnidirectional Composite" << "GS1 DataBar Stacked Omnidirectional Composite";
	bcNames << "GS1 DataBar Expanded Composite" << "GS1 DataBar Expanded Stacked Composite";
	bcNames << "GS1 DataBar Truncated Composite" << "GS1 DataBar Stacked Composite";
	bcNames << "GS1 DataBar Limited Composite" << "GS1-128 Composite";
	familyName = tr("GS1 Composite symbols");
	m_familyList.append(familyName);
	m_familyItems.insert(familyName, bcNames);

	m_ui.bcFamilyCombo->addItems(m_familyList);

	/*
	 *  End of the hard-coded data 
	 *
	 */

	m_guiColor = m_ui.codeEdit->palette().color(QPalette::Window);

	m_ui.okButton->setText(CommonStrings::tr_OK);
	m_ui.cancelButton->setText(CommonStrings::tr_Cancel);
	m_ui.resetButton->setIcon(IconManager::instance()->loadIcon("u_undo16.png"));

	if (ScCore->primaryMainWindow()->doc->PageColors.contains("Black"))
	{
		m_lnColor = ScCore->primaryMainWindow()->doc->PageColors["Black"];
		m_txtColor = ScCore->primaryMainWindow()->doc->PageColors["Black"];
		m_ui.linesLabel->setToolTip("Black");
		m_ui.txtLabel->setToolTip("Black");
	}
	else
	{
		m_ui.linesLabel->setToolTip("n.a.");
		m_ui.txtLabel->setToolTip("n.a.");
	}
	if (ScCore->primaryMainWindow()->doc->PageColors.contains("White"))
	{
		m_bgColor = ScCore->primaryMainWindow()->doc->PageColors["White"];
		m_ui.bgLabel->setToolTip("White");
	}
	else
		m_ui.bgLabel->setToolTip("n.a.");

	paintColorSample(m_ui.linesLabel, m_lnColor);
	paintColorSample(m_ui.txtLabel, m_txtColor);
	paintColorSample(m_ui.bgLabel, m_bgColor);

	m_paintBarcodeTimer=new QTimer(this);
	m_paintBarcodeTimer->setSingleShot(true);
	connect(m_paintBarcodeTimer, SIGNAL(timeout()), this, SLOT(paintBarcode()));

	connect(m_ui.bcFamilyCombo, SIGNAL(activated(QString)), this, SLOT(bcFamilyComboChanged()));
	connect(m_ui.bcCombo, SIGNAL(activated(QString)), this, SLOT(bcComboChanged()));
	connect(m_ui.bgColorButton, SIGNAL(clicked()), this, SLOT(bgColorButton_pressed()));
	connect(m_ui.lnColorButton, SIGNAL(clicked()), this, SLOT(lnColorButton_pressed()));
	connect(m_ui.txtColorButton, SIGNAL(clicked()), this, SLOT(txtColorButton_pressed()));
	connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(okButton_pressed()));
	connect(m_ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_pressed()));
	connect(m_ui.codeEdit, SIGNAL(textChanged(QString)), this, SLOT(codeEdit_textChanged(QString)));
	connect(m_ui.resetButton, SIGNAL(clicked()), this, SLOT(resetButton_clicked()));
	bcComboChanged();

}

BarcodeGenerator::~BarcodeGenerator()
{
	if (m_paintBarcodeTimer)
	{
		delete m_paintBarcodeTimer;
		m_paintBarcodeTimer=NULL;
	}
}

void BarcodeGenerator::updateOptions()
{

	QString enc=m_map[m_ui.bcCombo->currentText()].command;

	m_ui.formatLabel->setText(m_resvlbl[enc]!="" ? m_resvlbl[enc]+":" : "Version:");
	m_ui.formatCombo->blockSignals(true);
	m_ui.formatCombo->clear();
	m_ui.formatCombo->addItem("Auto");
	if (m_resvers[enc]!="")
	{
		m_ui.formatCombo->insertSeparator(999);
		m_ui.formatCombo->addItems(m_resvers[enc].split(","));
		m_ui.formatLabel->setEnabled(true);
		m_ui.formatCombo->setEnabled(true);
	}
	else
	{
		m_ui.formatLabel->setEnabled(false);
		m_ui.formatCombo->setEnabled(false);
	}
	m_ui.formatCombo->blockSignals(false);

	m_ui.eccCombo->blockSignals(true);
	m_ui.eccCombo->clear();
	m_ui.eccCombo->addItem("Auto");
	if (m_resecls[enc]!="")
	{
		m_ui.eccCombo->insertSeparator(999);
		m_ui.eccCombo->addItems(m_resecls[enc].split(","));
		m_ui.eccLabel->setEnabled(true);
		m_ui.eccCombo->setEnabled(true);
	}
	else
	{
		m_ui.eccLabel->setEnabled(false);
		m_ui.eccCombo->setEnabled(false);
	}
	m_ui.eccCombo->blockSignals(false);

}


void BarcodeGenerator::bcFamilyComboChanged()
{
	m_ui.bcCombo->blockSignals(true);
	m_ui.bcCombo->clear();
	m_ui.bcCombo->addItem(tr("Select a barcode format")); // to prevent 1st gs call
	m_ui.bcCombo->insertSeparator(999);
	m_ui.bcCombo->addItems(m_familyItems[m_ui.bcFamilyCombo->currentText()]);
	m_ui.bcCombo->blockSignals(false);
	bcComboChanged();
}

void BarcodeGenerator::bcComboChanged(int i)
{
	bcComboChanged();
}

void BarcodeGenerator::bcComboChanged()
{
	updateOptions();

	if (m_ui.bcCombo->currentIndex() == 0)
	{
		m_ui.okButton->setEnabled(false);
		m_ui.sampleLabel->setText(tr("Select Type"));
		m_ui.codeEdit->clear();
		m_ui.codeEdit->setEnabled(false);
		m_ui.optionsEdit->clear();
		m_ui.optionsEdit->setEnabled(false);
		m_ui.includetextCheck->setEnabled(false);
		m_ui.guardwhitespaceCheck->setEnabled(false);
		m_ui.includecheckCheck->setEnabled(false);
		m_ui.includecheckintextCheck->setEnabled(false);
		m_ui.parseCheck->setEnabled(false);
		m_ui.parsefncCheck->setEnabled(false);
		m_ui.formatLabel->setEnabled(false);
		m_ui.formatCombo->setEnabled(false);
		m_ui.eccLabel->setEnabled(false);
		m_ui.eccCombo->setEnabled(false);
		m_ui.bgColorButton->setEnabled(false);
		m_ui.lnColorButton->setEnabled(false);
		m_ui.txtColorButton->setEnabled(false);
		return;
	}
	
	m_ui.codeEdit->setEnabled(true);
	m_ui.optionsEdit->setEnabled(true);
	m_ui.bgColorButton->setEnabled(true);
	m_ui.lnColorButton->setEnabled(true);
	m_ui.txtColorButton->setEnabled(true);
	m_ui.okButton->setEnabled(true);

	QString s = m_ui.bcCombo->currentText();
	m_ui.codeEdit->blockSignals(true);
	m_ui.codeEdit->setText(m_map[s].exampleContents);
	m_ui.codeEdit->blockSignals(false);
	m_ui.optionsEdit->blockSignals(true);
	m_ui.optionsEdit->setText(m_map[s].exampleOptions);
	m_ui.optionsEdit->blockSignals(false);

	QString enc=m_map[s].command;
	m_ui.includetextCheck->setEnabled(m_resincludetextAvail[enc]);
	m_ui.guardwhitespaceCheck->setEnabled(m_resguardwhitespaceAvail[enc]);
	m_ui.includecheckCheck->setEnabled(m_resincludecheckAvail[enc]);
	m_ui.includecheckintextCheck->setEnabled(m_resincludetextAvail[enc] && m_resincludecheckintextAvail[enc]);
	m_ui.parseCheck->setEnabled(m_resparseAvail[enc]);
	m_ui.parsefncCheck->setEnabled(m_resparsefncAvail[enc]);

	updateUIFromOptionsText();

	enqueuePaintBarcode(0);
}

void BarcodeGenerator::enqueuePaintBarcode(int delay)
{
	m_ui.okButton->setEnabled(false);
//	paintBarcode();
	m_paintBarcodeTimer->start(delay);
}

void BarcodeGenerator::updateOptionsTextFromUI()
{
	QString opts=m_ui.optionsEdit->text();

	if (m_ui.includetextCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bincludetext\\b")))
			opts.append(" includetext");
	}
	else
	{
		opts.replace(QRegExp("\\bincludetext\\b")," "); 
	}

	if (m_ui.guardwhitespaceCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bguardwhitespace\\b"))) 
			opts.append(" guardwhitespace");
	}
	else
	{
		opts.replace(QRegExp("\\bguardwhitespace\\b")," "); 
	}

	if (m_ui.includecheckCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bincludecheck\\b")))
			opts.append(" includecheck");
	}
	else
	{
		opts.replace(QRegExp("\\bincludecheck\\b")," ");
	}

	if (m_ui.includecheckintextCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bincludecheckintext\\b")))
			opts.append(" includecheckintext");
	}
	else
	{
		opts.replace(QRegExp("\\bincludecheckintext\\b")," ");
	}

	if (m_ui.parseCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bparse\\b")))
			opts.append(" parse");
	}
	else
	{
		opts.replace(QRegExp("\\bparse\\b")," ");
	}

	if (m_ui.parsefncCheck->isChecked())
	{
		if (!opts.contains(QRegExp("\\bparsefnc\\b"))) 
			opts.append(" parsefnc");
	}
	else
	{
		opts.replace(QRegExp("\\bparsefnc\\b")," ");
	}

	QString enc=m_map[m_ui.bcCombo->currentText()].command;
	QString vlbl=m_resvlbl[enc]!="" ? m_resvlbl[enc].toLower() : "version";

 	if (m_ui.formatCombo->currentIndex() != 0)
	{
		QString t=m_ui.formatCombo->currentText();
		if (!opts.contains(QRegExp("\\b"+QRegExp::escape(vlbl)+"=.*\\b"))) 
			opts.append(" "+vlbl+"="+t);
		else
			opts.replace(QRegExp("\\b"+QRegExp::escape(vlbl)+"=\\S*\\b"),vlbl+"="+t);
	}
	else
	{
		opts.replace(QRegExp("\\b"+QRegExp::escape(vlbl)+"=\\S*\\b")," ");
	}

	if (m_ui.eccCombo->currentIndex() != 0)
	{
		QString t=m_ui.eccCombo->currentText();
		if (!opts.contains(QRegExp("\\beclevel=.*\\b"))) 
			opts.append(" eclevel="+t);
		else
			opts.replace(QRegExp("\\beclevel=\\S*\\b"),"eclevel="+t);
	}
	else
	{
		opts.replace(QRegExp("\\beclevel=\\S*\\b")," ");
	}

	m_ui.optionsEdit->blockSignals(true);
	m_ui.optionsEdit->setText(opts.simplified());
	m_ui.optionsEdit->blockSignals(false);

}

void BarcodeGenerator::updateUIFromOptionsText()
{
	m_ui.includetextCheck->blockSignals(true);	
	m_ui.includetextCheck->setChecked(m_ui.optionsEdit->text().contains(QRegExp("\\bincludetext\\b")));
	m_ui.includetextCheck->blockSignals(false);	

	m_ui.guardwhitespaceCheck->blockSignals(true);	
	m_ui.guardwhitespaceCheck->setChecked(m_ui.optionsEdit->text().contains(QRegExp("\\bguardwhitespace\\b")));
	m_ui.guardwhitespaceCheck->blockSignals(false);	

	m_ui.includecheckCheck->blockSignals(true);	
	m_ui.includecheckCheck->setChecked(m_ui.optionsEdit->text().contains(QRegExp("\\bincludecheck\\b")));
	m_ui.includecheckCheck->blockSignals(false);	

	m_ui.includecheckintextCheck->blockSignals(true);	
	m_ui.includecheckintextCheck->setChecked(m_ui.optionsEdit->text().contains(QRegExp("\\bincludecheckintext\\b")));
	m_ui.includecheckintextCheck->blockSignals(false);	

	m_ui.parseCheck->blockSignals(true);	
	m_ui.parseCheck->setChecked(m_ui.optionsEdit->text().contains(QRegExp("\\bparse\\b")));
	m_ui.parseCheck->blockSignals(false);	

	m_ui.parsefncCheck->blockSignals(true);	
	m_ui.parsefncCheck->setChecked(m_ui.optionsEdit->text().contains(QRegExp("\\bparsefnc\\b")));
	m_ui.parsefncCheck->blockSignals(false);	

	QString enc=m_map[m_ui.bcCombo->currentText()].command;
	QString vlbl=m_resvlbl[enc]!="" ? m_resvlbl[enc].toLower() : "version";

	QRegExp rxf("\\b"+QRegExp::escape(vlbl)+"=(\\S*)\\b");
	m_ui.formatCombo->blockSignals(true);	
	if (m_ui.optionsEdit->text().contains(rxf))
	{
		int idx=m_ui.formatCombo->findText(rxf.cap(1));
		if (idx == -1)
			idx=0;
		m_ui.formatCombo->setCurrentIndex(idx);
	}
	else
	{
		m_ui.formatCombo->setCurrentIndex(0);
	}
	m_ui.formatCombo->blockSignals(false);	

	QRegExp rxe("\\beclevel=(\\S*)\\b");
	m_ui.eccCombo->blockSignals(true);	
	if (m_ui.optionsEdit->text().contains(rxe))
	{
		int idx=m_ui.eccCombo->findText(rxe.cap(1));
		if (idx == -1)
			idx=0;
		m_ui.eccCombo->setCurrentIndex(idx);
	}
	else
	{
		m_ui.eccCombo->setCurrentIndex(0);
	}
	m_ui.eccCombo->blockSignals(false);
}

void BarcodeGenerator::updatePreview(QString errorMsg)
{
	QString pngFile = QDir::toNativeSeparators(ScPaths::getTempFileDir() + "bcode.png");
	if (errorMsg=="")
	{
		m_ui.sampleLabel->setPixmap(QPixmap(pngFile));
		m_ui.okButton->setEnabled(true);
	}
	else
	{
		m_ui.sampleLabel->setText("<qt>"+errorMsg+"</qt>");
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
	PaintManagerDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;
	m_bgColor = d.selectedColor();
	m_ui.bgLabel->setToolTip(d.selectedColorName());
	paintColorSample(m_ui.bgLabel, m_bgColor);
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::lnColorButton_pressed()
{
	PaintManagerDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;
	m_lnColor = d.selectedColor();
	m_ui.linesLabel->setToolTip(d.selectedColorName());
	paintColorSample(m_ui.linesLabel, m_lnColor);
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::txtColorButton_pressed()
{
	PaintManagerDialog d(this, &ScCore->primaryMainWindow()->doc->docGradients, ScCore->primaryMainWindow()->doc->PageColors, "", &ScCore->primaryMainWindow()->doc->docPatterns, ScCore->primaryMainWindow()->doc, ScCore->primaryMainWindow());
	if (!d.exec())
		return;
	m_txtColor = d.selectedColor();
	m_ui.txtLabel->setToolTip(d.selectedColorName());
	paintColorSample(m_ui.txtLabel, m_txtColor);
	enqueuePaintBarcode(0);
}

void BarcodeGenerator::okButton_pressed()
{

	QString psFile = QDir::toNativeSeparators(ScPaths::getTempFileDir() + "bcode.ps");

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
							m_ui.bcCombo->currentText() + " (" + m_ui.codeEdit->text() + ")",
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
	coloropts = coloropts.arg(m_lnColor.name().replace('#', "")) \
			.arg(m_bgColor.name().replace('#', "")) \
			.arg(m_txtColor.name().replace('#', ""));

	QString opts=m_ui.optionsEdit->text()+" "+coloropts;

	// Assemble PS from encoder and requirement bodies
	QString psCommand="%!PS-Adobe-2.0 EPSF-2.0\n";
	QString req;
	QString enc=m_map[m_ui.bcCombo->currentText()].command;
	foreach (req, m_resreqs[enc].split(" ")) 
		psCommand.append(m_resbodys[req]);
	psCommand.append(m_resbodys[enc]);
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
	QString bcdata(m_ui.codeEdit->text().toLatin1().toHex());
	QString bcopts(opts.toLatin1().toHex());
	comm = comm.arg(bcdata).arg(bcopts).arg(m_map[m_ui.bcCombo->currentText()].command);
	psCommand.append(comm);
	psCommand.append("showpage\n");

	m_thread.render(psCommand);
}


void BarcodeGenerator::resetButton_clicked()
{
	bcComboChanged();
}
