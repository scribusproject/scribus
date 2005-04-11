#include "edit1format.h"
#include "edit1format.moc"
#include "tabruler.h"
#include "units.h"

#include <qmessagebox.h>
#include <qtooltip.h>
#include "scribusdoc.h"
#include "styleselect.h"
#include "pageitem.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include "scpaths.h"

extern QPixmap loadIcon(QString nam);
extern bool loadText(QString nam, QString *Buffer);


EditStyle::EditStyle( QWidget* parent, struct ParagraphStyle *vor, QValueList<ParagraphStyle> v, bool neu, ApplicationPrefs *Prefs, double au, int dEin, ScribusDoc *doc)
		: QDialog( parent, "EditST", true, 0)
{
	parentDoc = doc;
	setCaption( tr( "Edit Style" ) );
	setIcon(loadIcon("AppIcon.png"));
	AutoVal = au;
	DocsEin = dEin;
	EditStyleLayout = new QGridLayout( this );
	EditStyleLayout->setSpacing( 6 );
	EditStyleLayout->setMargin( 10 );

	TextLabel1 = new QLabel( tr( "&Name:" ), this, "TextLabel1" );
	EditStyleLayout->addMultiCellWidget( TextLabel1, 0, 0, 0, 1 );

	Name = new QLineEdit( this, "Name" );
	Name->setMinimumSize( QSize( 200, 22 ) );
	Name->setText( vor->Vname );
	OldName = vor->Vname;
	IsNew = neu;
	TextLabel1->setBuddy(Name);
	EditStyleLayout->addMultiCellWidget( Name, 1, 1, 0, 1 );

	GroupFont = new QGroupBox( tr( "Character" ), this, "GroupFont" );
	GroupFont->setColumnLayout(0, Qt::Vertical );
	GroupFont->layout()->setSpacing( 5 );
	GroupFont->layout()->setMargin( 10 );
	GroupFontLayout = new QGridLayout( GroupFont->layout() );
	GroupFontLayout->setAlignment( Qt::AlignTop );
	TextF1 = new QLabel( tr( "&Font:"),  GroupFont, "TextF1" );
	TextF1->setMinimumSize( QSize( 50, 22 ) );
	GroupFontLayout->addWidget( TextF1, 0, 0 );
	FontC = new FontCombo(GroupFont, Prefs);
	for (int fc=0; fc<FontC->count(); ++fc)
	{
		if (FontC->text(fc) == vor->Font)
		{
			FontC->setCurrentItem(fc);
			break;
		}
	}
	TextF1->setBuddy(FontC);
	GroupFontLayout->addMultiCellWidget( FontC, 0, 0, 1, 4 );
	SizeC = new MSpinBox( 1, 1024, GroupFont, 1 );
	SizeC->setMinimumSize( QSize( 70, 22 ) );
	SizeC->setSuffix( tr( " pt" ) );
	SizeC->setValue(vor->FontSize / 10.0);
	TextF2 = new QLabel( SizeC, tr("Si&ze:") ,GroupFont, "TextF2" );
	TextF2->setMinimumSize( QSize( 50, 22 ) );
	GroupFontLayout->addWidget( TextF2, 1, 0 );
	GroupFontLayout->addWidget( SizeC, 1, 1 );

	EffeS = new StyleSelect(GroupFont);
	EffeS->setStyle(vor->FontEffect);
	EffeLabel = new QLabel( tr("Effect:"), GroupFont, "EffeLabel" );
	GroupFontLayout->addWidget( EffeLabel, 1, 2 );
	GroupFontLayout->addMultiCellWidget( EffeS, 1, 1, 3, 4, Qt::AlignLeft );

	AligS = new AlignSelect(GroupFont);
	AligS->setStyle(vor->textAlignment);
	AligLabel = new QLabel( AligS, tr("&Alignment:"), GroupFont, "AligLabel" );
	GroupFontLayout->addWidget( AligLabel, 2, 0 );
	GroupFontLayout->addWidget( AligS, 2, 1, Qt::AlignLeft );

	DropCaps = new QCheckBox( tr("&Drop Caps"), GroupFont, "DropCaps" );
	DropCaps->setChecked(vor->Drop);
	GroupFontLayout->addWidget( DropCaps, 2, 2 );
	DropLines = new QSpinBox( GroupFont, "DropLines" );
	DropLines->setMinValue( 2 );
	DropLines->setMaxValue( 20 );
	DropLines->setValue(vor->DropLin);
	CapLabel = new QLabel( DropLines, tr("&Lines:"), GroupFont, "CapLabel" );
	GroupFontLayout->addWidget( CapLabel, 2, 3, Qt::AlignRight );
	GroupFontLayout->addWidget( DropLines, 2, 4 );
	bool enable = vor->Drop ? true : false;
	DropLines->setEnabled(enable);
	CapLabel->setEnabled(enable);

	TxFill = new QComboBox( true, GroupFont, "TxFill" );
	TxFill->setEditable(false);
	FillIcon = new QLabel( TxFill, tr( "F&ill Color:" ), GroupFont, "FillIcon" );
	GroupFontLayout->addWidget( FillIcon, 3, 0 );
	GroupFontLayout->addWidget( TxFill, 3, 1 );
	PM2 = new ShadeButton(GroupFont);
	GroupFontLayout->addWidget( PM2, 3, 2, Qt::AlignLeft );

	TxStroke = new QComboBox( true, GroupFont, "TxStroke" );
	TxStroke->setEditable(false);
	StrokeIcon = new QLabel( TxStroke, tr("St&roke Color:"), GroupFont, "StrokeIcon" );
	GroupFontLayout->addWidget( StrokeIcon, 4, 0 );
	GroupFontLayout->addWidget( TxStroke, 4, 1 );
	PM1 = new ShadeButton(GroupFont);
	GroupFontLayout->addWidget( PM1, 4, 2, Qt::AlignLeft );

	TxFill->clear();
	TxStroke->clear();
	ColorList::Iterator it;
	QPixmap pm = QPixmap(15, 15);
	TxFill->insertItem( tr("None"));
	TxStroke->insertItem( tr("None"));
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
	{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		TxFill->insertItem(pm, it.key());
		TxStroke->insertItem(pm, it.key());
	}
	StrokeIcon->setEnabled(false);
	PM1->setEnabled(false);
	TxStroke->setEnabled(false);
	TxFill->setCurrentText(vor->FColor);
	TxStroke->setCurrentText(vor->SColor);
	PM2->setValue(vor->FShade);
	PM1->setValue(vor->SShade);

	EditStyleLayout->addWidget( GroupFont, 2, 0 );

	AbstandV = new QGroupBox( tr("Vertical Spaces"), this, "AbstandV" );
	AbstandV->setColumnLayout(0, Qt::Vertical );
	AbstandV->layout()->setSpacing( 0 );
	AbstandV->layout()->setMargin( 0 );
	AbstandVLayout = new QGridLayout( AbstandV->layout() );
	AbstandVLayout->setAlignment( Qt::AlignTop );
	AbstandVLayout->setSpacing( 5 );
	AbstandVLayout->setMargin( 10 );

	BaseGrid = new QCheckBox( tr("Adjust to Baseline &Grid"), AbstandV, "BaseGrid" );
	BaseGrid->setChecked(vor->BaseAdj);
	AbstandVLayout->addMultiCellWidget( BaseGrid, 0, 0, 0, 1 );
	AboveV = new MSpinBox( 0, 300, AbstandV, 1 );
	AboveV->setMinimumSize( QSize( 70, 22 ) );
	AbstandVLayout->addWidget( AboveV, 1, 1 );

	BelowV = new MSpinBox( 0, 300, AbstandV, 1 );
	BelowV->setMinimumSize( QSize( 70, 22 ) );
	AbstandVLayout->addWidget( BelowV, 2, 1 );

	LineSpVal = new MSpinBox( 1, 300, AbstandV, 1 );
	LineSpVal->setMinimumSize( QSize( 70, 22 ) );
	LineSpVal->setSuffix( tr( " pt" ) );
	LineSpVal->setValue(vor->LineSpa);
	AbstandVLayout->addWidget( LineSpVal, 3, 1 );

	TextLabel3 = new QLabel( LineSpVal, tr("Line &Spacing:"), AbstandV, "TextLabel3" );
	TextLabel3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                                        TextLabel3->sizePolicy().hasHeightForWidth() ) );
	TextLabel3->setMinimumSize( QSize( 90, 22 ) );
	AbstandVLayout->addWidget( TextLabel3, 3, 0 );
	TextLabel1_2_2 = new QLabel( AboveV, tr("Abo&ve:"), AbstandV, "TextLabel1_2_2" );
	TextLabel1_2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                               TextLabel1_2_2->sizePolicy().hasHeightForWidth() ) );
	TextLabel1_2_2->setMinimumSize( QSize( 90, 22 ) );
	AbstandVLayout->addWidget( TextLabel1_2_2, 1, 0 );
	TextLabel1_2_3 = new QLabel( BelowV, tr("&Below:"), AbstandV, "TextLabel1_2_3" );
	TextLabel1_2_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1,
	                               TextLabel1_2_3->sizePolicy().hasHeightForWidth() ) );
	TextLabel1_2_3->setMinimumSize( QSize( 90, 22 ) );
	AbstandVLayout->addWidget( TextLabel1_2_3, 2, 0 );
	EditStyleLayout->addWidget( AbstandV, 2, 1 );

	GroupBox10 = new QGroupBox( tr("Tabulators and Indentation"), this, "GroupBox10" );
	GroupBox10->setColumnLayout(0, Qt::Vertical );
	GroupBox10->layout()->setSpacing( 0 );
	GroupBox10->layout()->setMargin( 0 );
	GroupBox10Layout = new QVBoxLayout(GroupBox10->layout());
	GroupBox10Layout->setAlignment( Qt::AlignTop );
	GroupBox10Layout->setSpacing( 5 );
	GroupBox10Layout->setMargin( 10 );
	TabList = new Tabruler(GroupBox10, true, DocsEin, vor->TabValues, -1);
	TabList->setLeftIndentData(vor->Indent);
	TabList->setLeftIndent();
	TabList->setFirstLineData(vor->First);
	TabList->setFirstLine();
	GroupBox10Layout->addWidget( TabList );

	EditStyleLayout->addMultiCellWidget( GroupBox10, 3, 3, 0, 1 );

	// Label for holding "style preview" bitmap 12/30/2004 petr vanek
	layoutPreview = new QVBoxLayout;
	layoutPreview->setSpacing(6);
	layoutPreview->setMargin(0);
	previewCaption = new QLabel(this, "previewCaption");
	previewCaption->setText(tr("Preview of the Paragraph Style:"));
	layoutPreview->addWidget(previewCaption);
	previewText = new QLabel(this, "previewText");
	previewText->setMinimumSize(640, 200);
	previewText->setMaximumSize(640, 200);
	previewText->setAlignment( static_cast<int>( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	previewText->setFrameShape(QFrame::Box);
	previewText->setPaletteBackgroundColor(paletteBackgroundColor());
	layoutPreview->addWidget(previewText);
	// preview setting - reading first paragraphs from LoremIpsum.txt etc.
	previewItem = new PageItem(parentDoc, PageItem::TextFrame, 0, 0, previewText->width(), previewText->height(), 0, "None", parentDoc->toolSettings.dPenText);
	previewItem->FrameType = PageItem::TextFrame;
	if (!loadText(ScPaths::instance().sampleScriptDir() + "LoremIpsum.txt", &lorem))
		qDebug("edit1format.cpp: Error reading sample text");
	lorem = QString::fromUtf8(lorem);
	lorem = lorem.section('\n', 0, 2);

	Layout17 = new QHBoxLayout;
	Layout17->setSpacing( 6 );
	Layout17->setMargin( 0 );
	QSpacerItem* spacer2 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout17->addItem( spacer2 );
	OkButton = new QPushButton( tr( "&OK" ), this, "OkButton" );
	Layout17->addWidget( OkButton );
	Cancel = new QPushButton( tr( "&Cancel" ), this, "Cancel" );
	OkButton->setDefault( true );
	Layout17->addWidget( Cancel );
	layoutPreview->addLayout(Layout17);
	EditStyleLayout->addMultiCellLayout( layoutPreview, 4, 4, 0, 1 );
	werte = vor;
	allV = v;
	// tooltips
	QToolTip::add( Name, "<qt>" + tr( "Name of your paragraph style" ) + "</qt>" );
	QToolTip::add( FontC, "<qt>" + tr( "Font of selected text or object" ) + "</qt>" );
	QToolTip::add( SizeC, "<qt>" + tr( "Font Size" ) + "</qt>" );
	QToolTip::add( TxFill, "<qt>" + tr( "Color of text fill" ) + "</qt>" );
	QToolTip::add( TxStroke, "<qt>" + tr( "Color of text stroke" ) + "</qt>" );
	QToolTip::add( DropCaps, "<qt>" + tr( "Provides an oversized first letter for a paragraph. Used for stylistic effect" ) + "</qt>" );
	QToolTip::add( DropLines, "<qt>" + tr( "Determines the overall height, in line numbers, of the Drop Caps" ) );
	QToolTip::add( BaseGrid, "<qt>" + tr( "Align text to baseline grid" ) + "</qt>" );
	QToolTip::add( AboveV, "<qt>" + tr( "Spacing above the paragraph" ) + "</qt>" );
	QToolTip::add( BelowV, "<qt>" + tr( "Spacing below the paragraph" ) + "</qt>" );
	QToolTip::add( LineSpVal, "<qt>" + tr( "Line Spacing" ) + "</qt>" );
	QToolTip::add( previewText, "<qt>" + tr( "Sample text of this paragraph style" ) + "</qt>");

	// signals and slots connections
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( OkButton, SIGNAL( clicked() ), this, SLOT( Verlassen() ) );
	connect( DropCaps, SIGNAL( clicked() ), this, SLOT( ManageDrops() ) );
	connect(SizeC, SIGNAL(valueChanged(int)), this, SLOT(FontChange()));
	connect(EffeS, SIGNAL(State(int)), this, SLOT(ColorChange()));
	// preview generators
	connect(FontC, SIGNAL(activated(const QString &)), this, SLOT(updatePreview()));
	connect(LineSpVal, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(TabList, SIGNAL(tabrulerChanged()), this, SLOT(updatePreview()));
	connect(AboveV, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(BelowV, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));
	connect(AligS, SIGNAL(State(int)), this, SLOT(updatePreview()));
	connect(TxStroke, SIGNAL(activated(const QString &)), this, SLOT(updatePreview()));
	connect(PM1, SIGNAL(clicked()), this, SLOT(updatePreview()));
	connect(TxFill, SIGNAL(activated(const QString &)), this, SLOT(updatePreview()));
	connect(PM2, SIGNAL(clicked()), this, SLOT(updatePreview()));
	connect(BaseGrid, SIGNAL(stateChanged(int)), this, SLOT(updatePreview()));
	connect(DropLines, SIGNAL(valueChanged(int)), this, SLOT(updatePreview()));

	AboveV->setDecimals(10);
	BelowV->setDecimals(10);
	/* PFJ - 29.02.04 - Altered switch so only case 2 is tested */
	QString ein = unitGetSuffixFromIndex(dEin);
	if (dEin == 2)
	{
		AboveV->setDecimals(10000);
		BelowV->setDecimals(10000);
	}
	AboveV->setSuffix(ein);
	BelowV->setSuffix(ein);
	BelowV->setValue(vor->gapAfter * parentDoc->unitRatio);
	AboveV->setValue(vor->gapBefore * parentDoc->unitRatio);
	ColorChange();
}

void EditStyle::ColorChange()
{
	int s = EffeS->getStyle();
	bool enabled = (s & 4) ? true : false;
	StrokeIcon->setEnabled(enabled);
	PM1->setEnabled(enabled);
	TxStroke->setEnabled(enabled);
	updatePreview();
}

void EditStyle::ManageDrops()
{
	bool enabled = DropCaps->isChecked() ? true : false;
	DropLines->setEnabled(enabled);
	CapLabel->setEnabled(enabled);
	updatePreview();
}

void EditStyle::FontChange()
{
	double val = SizeC->value();
	LineSpVal->setValue((val  * AutoVal / 100) + val);
	updatePreview();
}

void EditStyle::Verlassen()
{
	if (IsNew)
	{
		for (uint x=0; x<allV.count()-1; ++x)
		{
			if (Name->text() == allV[x].Vname)
			{
				QMessageBox::information(this, tr("Warning"), tr("Name of the Style is not unique"), tr("OK"),
				                         0, 0, 0, QMessageBox::Ok);
				Name->selectAll();
				Name->setFocus();
				return;
			}
		}
	}
	else
	{
		if (OldName != Name->text())
		{
			for (uint x=0; x<allV.count(); ++x)
			{
				if (Name->text() == allV[x].Vname)
				{
					QMessageBox::information(this, tr("Warning"), tr("Name of the Style is not unique"),
					                         tr("OK"),0, 0, 0, QMessageBox::Ok);
					Name->selectAll();
					Name->setFocus();
					return;
				}
			}
		}
	}
	werte->FontEffect = EffeS->getStyle();
	werte->textAlignment = AligS->getStyle();
	werte->LineSpa = LineSpVal->value();
	werte->Indent = TabList->getLeftIndent();
	werte->First = TabList->getFirstLine();
	werte->gapBefore = AboveV->value() / parentDoc->unitRatio;
	werte->gapAfter = BelowV->value() / parentDoc->unitRatio;
	werte->Vname = Name->text();
	werte->Font = FontC->currentText();
	werte->FontSize = qRound(SizeC->value() * 10.0);
	werte->Drop = DropCaps->isChecked();
	werte->DropLin = DropLines->value();
	werte->FColor = TxFill->currentText();
	werte->FShade = PM2->getValue();
	werte->SColor = TxStroke->currentText();
	werte->SShade = PM1->getValue();
	werte->BaseAdj = BaseGrid->isChecked();
	werte->TabValues = TabList->getTabVals();
	accept();
}

void EditStyle::updatePreview()
{
	int x = previewText->width();
	int y = previewText->height();
	QPixmap pm(x, y);
	ScPainter *painter = new ScPainter(&pm, x, y, 0, 0);
	painter->clear(white);
	pm.fill(white);
	previewText->clear();

	ParagraphStyle tmpStyle;
	tmpStyle.Vname = Name->text() + " (preview temporary)";
	tmpStyle.FontEffect = EffeS->getStyle();
	tmpStyle.textAlignment = AligS->getStyle();
	tmpStyle.LineSpa = LineSpVal->value();
	tmpStyle.Indent = TabList->getLeftIndent();
	tmpStyle.First = TabList->getFirstLine();
	tmpStyle.gapBefore = AboveV->value() / parentDoc->unitRatio;
	tmpStyle.gapAfter = BelowV->value() / parentDoc->unitRatio;
	tmpStyle.Font = FontC->currentText();
	tmpStyle.FontSize = qRound(SizeC->value() * 10.0);
	tmpStyle.Drop = DropCaps->isChecked();
	tmpStyle.DropLin = DropLines->value();
	tmpStyle.FColor = TxFill->currentText();
	tmpStyle.FShade = PM2->getValue();
	tmpStyle.SColor = TxStroke->currentText();
	tmpStyle.SShade = PM1->getValue();
	tmpStyle.BaseAdj = BaseGrid->isChecked();
	tmpStyle.TabValues = TabList->getTabVals();

	QFont fo = QFont(FontC->currentText());
	fo.setPointSize(qRound(parentDoc->toolSettings.defSize / 10.0));
	parentDoc->AddFont(FontC->currentText(), fo);
	parentDoc->docParagraphStyles.append(tmpStyle);
	int tmpIndex = parentDoc->docParagraphStyles.count() - 1;
	previewItem->itemText.clear();
	previewItem->IFont = FontC->currentText();

	for (uint i = 0; i < lorem.length(); ++i)
	{
		ScText *hg = new ScText;
		hg->ch = lorem.at(i);
		if ((hg->ch == QChar(10)) || (hg->ch == QChar(5)))
			hg->ch = QChar(13);
		hg->cfont = (*parentDoc->AllFonts)[FontC->currentText()];
		hg->csize = tmpStyle.FontSize;
		hg->ccolor = tmpStyle.FColor;
		hg->cshade = tmpStyle.FShade;
		hg->cstroke = tmpStyle.SColor;
		hg->cshade2 = tmpStyle.SShade;
		hg->cscale = 100;
		hg->cselect = false;
		hg->cstyle = tmpStyle.FontEffect;
		hg->cab = tmpIndex;
		hg->cextra = 0;
		hg->xp = 0;
		hg->yp = 0;
		hg->PRot = 0;
		hg->PtransX = 0;
		hg->PtransY = 0;
		previewItem->itemText.append(hg);
	}

	previewItem->DrawObj(painter, QRect(0, 0, x, y));
	painter->end();
	previewText->setPixmap(pm);
	delete(painter);
	parentDoc->docParagraphStyles.remove(parentDoc->docParagraphStyles.fromLast());
}
