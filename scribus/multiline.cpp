/****************************************************************************
** Form implementation generated from reading ui file 'Mline.ui'
**
** Created: Mit Jun 4 23:28:21 2003
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "multiline.h"
#include "multiline.moc"
#include <qpainter.h>
#include <qmessagebox.h>
extern QPixmap loadIcon(QString nam);

MultiLine::MultiLine( QWidget* parent, ScribusDoc* doc, multiLine ml, QString nam, bool ed, QMap<QString,multiLine> *Sty)
				 : QDialog( parent, "Multiline", true, 0 )
{
	setIcon(loadIcon("AppIcon.png"));
	setCaption( tr( "Edit Style" ) );
	Docu = doc;
	TempVorl = ml;
	CurLin = 0;
	TempStyles = Sty;
	GivenName = nam;
	MultiLineLayout = new QVBoxLayout( this, 5, 4, "MultiLineLayout");
	SName = new QLineEdit(this, "Name");
	SName->setReadOnly(!ed);
	SName->setText(nam);
	MultiLineLayout->addWidget(SName); 
	Preview = new QLabel( this, "Preview" );
	Preview->setFrameShape( QLabel::Panel );
	Preview->setFrameShadow( QLabel::Sunken );
	Preview->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignHCenter ) );
	Preview->setScaledContents( FALSE );
	MultiLineLayout->addWidget( Preview );

	layout2 = new QHBoxLayout( 0, 0, 4, "layout2"); 

	AddStyle = new QPushButton( this, "AddStyle" );
	AddStyle->setPixmap(loadIcon("penciladd.png"));
	AddStyle->setText( QString::null );
	AddStyle->setAutoDefault(false);
	AddStyle->setDefault(false);
	layout2->addWidget( AddStyle );

	RemoveStyle = new QPushButton( this, "RemoveStyle" );
	RemoveStyle->setPixmap(loadIcon("pencilsub.png"));
	RemoveStyle->setText( QString::null );
	RemoveStyle->setAutoDefault(false);
	RemoveStyle->setDefault(false);
	layout2->addWidget( RemoveStyle );
	QSpacerItem* spacer = new QSpacerItem( 71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layout2->addItem( spacer );
	MultiLineLayout->addLayout( layout2 );

	Styles = new QListBox( this, "Styles" );
	RebuildList();
	MultiLineLayout->addWidget( Styles );

	Properties = new QGroupBox( this, "Properties" );
	Properties->setTitle( QString::null );
	Properties->setFrameShape( QGroupBox::NoFrame );
	Properties->setFrameShadow( QGroupBox::Plain );
	Properties->setColumnLayout(0, Qt::Vertical );
	Properties->layout()->setSpacing( 4 );
	Properties->layout()->setMargin( 0 );
	PropertiesLayout = new QVBoxLayout( Properties->layout() );
	PropertiesLayout->setAlignment( Qt::AlignTop );

	Dashes = new LineCombo(Properties);
	PropertiesLayout->addWidget( Dashes );

	LineEnds = new QComboBox( true, Properties, "LineEnds" );
	LineEnds->setEditable(false);
	LineEnds->insertItem(loadIcon("ButtCap.png"), tr( "Flat Cap" ) );
	LineEnds->insertItem(loadIcon("SquareCap.png"), tr( "Square Cap" ) );
	LineEnds->insertItem(loadIcon("RoundCap.png"), tr( "Round Cap" ) );
	PropertiesLayout->addWidget( LineEnds );

	LineJoin = new QComboBox( true, Properties, "LineJoin" );
	LineJoin->setEditable(false);
	LineJoin->insertItem(loadIcon("MiterJoin.png"), tr( "Miter Join" ) );
	LineJoin->insertItem(loadIcon("BevelJoin.png"), tr( "Bevel Join" ) );
	LineJoin->insertItem(loadIcon("RoundJoin.png"), tr( "Round Join" ) );
	PropertiesLayout->addWidget( LineJoin );

	layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

	WidthText = new QLabel( Properties, "WidthText" );
	WidthText->setText( tr( "Line Width:" ) );
	WidthText->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignLeft ) );
	layout1->addWidget( WidthText );

	LWidth = new MSpinBox( Properties, 1 );
	LWidth->setSuffix( tr( " pt" ) );
	LWidth->setMaxValue( 36 );
	LWidth->setMinValue( 1 );
	layout1->addWidget( LWidth );
	PropertiesLayout->addLayout( layout1 );

	layout4 = new QHBoxLayout( 0, 0, 6, "layout4");
	Color = new QComboBox( true, Properties, "Color" );
	CListe::Iterator it;
	QPixmap pm = QPixmap(30, 15);
	for (it = doc->PageColors.begin(); it != doc->PageColors.end(); ++it)
		{
		pm.fill(doc->PageColors[it.key()].getRGBColor());
		Color->insertItem(pm, it.key());
		}
	Color->setEditable(false);
	layout4->addWidget( Color );
	Shade = new QSpinBox( Properties, "Shade" );
	Shade->setSuffix( tr( " %" ) );
	Shade->setMaxValue( 100 );
	Shade->setMinValue( 0 );
	layout4->addWidget( Shade );
	PropertiesLayout->addLayout( layout4 );
	MultiLineLayout->addWidget( Properties );

	layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 

	OK = new QPushButton( this, "OK" );
	OK->setText( tr( "OK" ) );
	OK->setAutoDefault(false);
	OK->setDefault(false);
	layout3->addWidget( OK );

	Cancel = new QPushButton( this, "Cancel" );
	Cancel->setText( tr( "Cancel" ) );
	Cancel->setAutoDefault(false);
	Cancel->setDefault(false);
	layout3->addWidget( Cancel );
	MultiLineLayout->addLayout( layout3 );
	resize( QSize(222, 349).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

    // signals and slots connections
	connect(Styles, SIGNAL(highlighted(int)), this, SLOT(slotEditStyle(int)));
	connect( Cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( OK, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect(Dashes, SIGNAL(activated(int)), this, SLOT(NewLSty()));
	connect(LineJoin, SIGNAL(activated(int)), this, SLOT(NewLJoin()));
	connect(LineEnds, SIGNAL(activated(int)), this, SLOT(NewLEnd()));
	connect(LWidth, SIGNAL(valueChanged(int)), this, SLOT(NewLWidth()));
	connect(Color, SIGNAL(activated(int)), this, SLOT(NewLColor()));
	connect(AddStyle, SIGNAL(clicked()), this, SLOT(NewSubLine()));
	connect(RemoveStyle, SIGNAL(clicked()), this, SLOT(DelSubLine()));
	connect(Shade, SIGNAL(valueChanged(int)), this, SLOT(NewLShade()));
	if (ed)
		connect(SName, SIGNAL(lostFocus()), this, SLOT(NewName()));
	slotEditStyle(0);
	updatePreview();
}

MultiLine::~MultiLine()
{
    // no need to delete child widgets, Qt does it all for us
}

void MultiLine::updatePreview()
{
	QPixmap pm = QPixmap(200, 37);
	pm.fill(white);
	QPainter p;
	p.begin(&pm);
	for (int it = TempVorl.size()-1; it > -1; it--)
		{
		p.setPen(QPen(calcFarbe(TempVorl[it].Color, TempVorl[it].Shade),
									QMAX(static_cast<int>(TempVorl[it].Width), 1),
									static_cast<PenStyle>(TempVorl[it].Dash),
									static_cast<PenCapStyle>(TempVorl[it].LineEnd),
									static_cast<PenJoinStyle>(TempVorl[it].LineJoin)));
		p.drawLine(17, 18, 183, 18);
		}
	p.end();
	Preview->setPixmap(pm);
}

QColor MultiLine::calcFarbe(QString name, int shade)
{
	QColor tmpf;
	int h, s, v, sneu;
	Docu->PageColors[name].getRGBColor().rgb(&h, &s, &v);
	if ((h == s) && (s == v))
		{
		Docu->PageColors[name].getRGBColor().hsv(&h, &s, &v);
		sneu = 255 - ((255 - v) * shade / 100);
		tmpf.setHsv(h, s, sneu);
		}
	else
		{
		Docu->PageColors[name].getRGBColor().hsv(&h, &s, &v);
		sneu = s * shade / 100;
		tmpf.setHsv(h, sneu, v);
		}
	return tmpf;
}

void MultiLine::updateSList()
{
	QString tmp, tmp2;
	QPixmap pm = QPixmap(30, 15);
	pm.fill(calcFarbe(TempVorl[CurLin].Color, TempVorl[CurLin].Shade));
	tmp2 = " "+tmp.setNum(TempVorl[CurLin].Width)+ tr(" pt ");
	switch (static_cast<PenStyle>(TempVorl[CurLin].Dash))
		{
		case SolidLine:
			tmp2 += tr("Solid Line");
			break;
		case DashLine:
			tmp2 += tr("Dashed Line");
			break;
		case DotLine:
			tmp2 += tr("Dotted Line");
			break;
		case DashDotLine:
			tmp2 += tr("Dash Dot Line");
			break;
		case DashDotDotLine:
			tmp2 += tr("Dash Dot Dot Line");
			break;
		default:
			tmp2 += tr("Solid Line");
			break;
		}
	tmp2 += " ";
	if (Styles->count() == 1)				// to avoid Bug in Qt-3.1.2
		{
		Styles->clear();
		Styles->insertItem(pm, tmp2);
		}
	else
		Styles->changeItem(pm, tmp2, CurLin);
	}

void MultiLine::reSort()
{
	int cc = 0;
	struct singleLine sl;
	sl.Color = TempVorl[CurLin].Color;
	sl.Shade = TempVorl[CurLin].Shade;
	sl.Dash = TempVorl[CurLin].Dash;
	sl.LineEnd = TempVorl[CurLin].LineEnd;
	sl.LineJoin = TempVorl[CurLin].LineJoin;
	sl.Width = TempVorl[CurLin].Width;
	multiLine::iterator it3;
	for (it3 = TempVorl.begin(); it3 != TempVorl.end(); ++it3)
		{
		if (cc == CurLin)
			{
			TempVorl.erase(it3);
			break;
			}
		cc++;
		}
	cc = 0;
	bool fo = false;
	for (multiLine::iterator it2 = TempVorl.begin(); it2 != TempVorl.end(); ++it2)
		{
		if (sl.Width < (*it2).Width)
			{
			TempVorl.insert(it2, sl);
			fo = true;
			break;
			}
		cc++;
		}
	if (!fo)
		TempVorl.push_back(sl);
	CurLin = cc;
	RebuildList();
	slotEditStyle(cc);
	updatePreview();
}

void MultiLine::RebuildList()
{
	QString tmp, tmp2;
	Styles->clear();
	QPixmap pm2 = QPixmap(30, 15);
	for (multiLine::iterator it = TempVorl.begin(); it != TempVorl.end(); ++it)
		{
		pm2.fill(calcFarbe((*it).Color, (*it).Shade));
		tmp2 = " "+tmp.setNum((*it).Width)+ tr(" pt ");
		switch (static_cast<PenStyle>((*it).Dash))
			{
			case SolidLine:
				tmp2 += tr("Solid Line");
				break;
			case DashLine:
				tmp2 += tr("Dashed Line");
				break;
			case DotLine:
				tmp2 += tr("Dotted Line");
				break;
			case DashDotLine:
				tmp2 += tr("Dash Dot Line");
				break;
			case DashDotDotLine:
				tmp2 += tr("Dash Dot Dot Line");
				break;
			default:
				tmp2 += tr("Solid Line");
				break;
			}
		tmp2 += " ";
		Styles->insertItem(pm2, tmp2);
		}
}

void MultiLine::NewName()
{
	QString NameNew = SName->text();
	if (TempStyles->contains(NameNew))
		{
		QMessageBox::warning(this, tr("Warning"), tr("Name \"%1\" isn't unique.\nPlease choose another.").arg(NameNew), tr("OK"));
		SName->setText(GivenName);
		SName->setFocus();
		}
	else
		GivenName = NameNew;
}

void MultiLine::NewSubLine()
{
	struct singleLine sl;
	sl.Color = TempVorl[CurLin].Color;
	sl.Shade = TempVorl[CurLin].Shade;
	sl.Dash = TempVorl[CurLin].Dash;
	sl.LineEnd = TempVorl[CurLin].LineEnd;
	sl.LineJoin = TempVorl[CurLin].LineJoin;
	sl.Width = TempVorl[CurLin].Width;
	int cc = 0;
	bool fo = false;
	for (multiLine::iterator it2 = TempVorl.begin(); it2 != TempVorl.end(); ++it2)
		{
		if (sl.Width < (*it2).Width)
			{
			TempVorl.insert(it2, sl);
			fo = true;
			break;
			}
		cc++;
		}
	if (!fo)
		TempVorl.push_back(sl);
	CurLin = cc;
	RebuildList();
	slotEditStyle(cc);
	updatePreview();
}

void MultiLine::DelSubLine()
{
	if (TempVorl.size() == 1)
		return;
	int cc = 0;
	for (multiLine::iterator it3 = TempVorl.begin(); it3 != TempVorl.end(); ++it3)
		{
		if (cc == CurLin)
			{
			TempVorl.erase(it3);
			break;
			}
		cc++;
		}
	CurLin = 0;
	RebuildList();
	slotEditStyle(0);
	updatePreview();
}

void MultiLine::NewLJoin()
{
	PenJoinStyle c = MiterJoin;
	switch (LineJoin->currentItem())
		{
		case 0:
			c = MiterJoin;
			break;
		case 1:
			c = BevelJoin;
			break;
		case 2:
			c = RoundJoin;
			break;
		}
	TempVorl[CurLin].LineJoin = static_cast<int>(c);
	updateSList();
}

void MultiLine::NewLEnd()
{
	PenCapStyle c = FlatCap;
	switch (LineEnds->currentItem())
		{
		case 0:
			c = FlatCap;
			break;
		case 1:
			c = SquareCap;
			break;
		case 2:
			c = RoundCap;
			break;
		}
	TempVorl[CurLin].LineEnd = static_cast<int>(c);
	updateSList();
	updatePreview();
}

void MultiLine::NewLSty()
{
	PenStyle c = SolidLine;
	switch (Dashes->currentItem())
		{
		case 0:
			c = SolidLine;
			break;
		case 1:
			c = DashLine;
			break;
		case 2:
			c = DotLine;
			break;
		case 3:
			c = DashDotLine;
			break;
		case 4:
			c = DashDotDotLine;
			break;
		}
	TempVorl[CurLin].Dash = static_cast<int>(c);
	updateSList();
	updatePreview();
}

void MultiLine::NewLColor()
{
	TempVorl[CurLin].Color = Color->currentText();
	updateSList();
	updatePreview();
}

void MultiLine::NewLWidth()
{
	TempVorl[CurLin].Width = LWidth->value();
	reSort();
	updatePreview();
}

void MultiLine::NewLShade()
{
	TempVorl[CurLin].Shade = Shade->value();
	updateSList();
	updatePreview();
}

void MultiLine::slotEditStyle(int i)
{
	disconnect(Styles, SIGNAL(highlighted(int)), this, SLOT(slotEditStyle(int)));
	disconnect(Dashes, SIGNAL(activated(int)), this, SLOT(NewLSty()));
	disconnect(LineJoin, SIGNAL(activated(int)), this, SLOT(NewLJoin()));
	disconnect(LineEnds, SIGNAL(activated(int)), this, SLOT(NewLEnd()));
	disconnect(LWidth, SIGNAL(valueChanged(int)), this, SLOT(NewLWidth()));
	disconnect(Color, SIGNAL(activated(int)), this, SLOT(NewLColor()));
	disconnect(Shade, SIGNAL(valueChanged(int)), this, SLOT(NewLShade()));
	Styles->setSelected(i, true);
	CurLin = i;
	LWidth->setValue(TempVorl[i].Width);
	Color->setCurrentText(TempVorl[i].Color);
	Shade->setValue(TempVorl[i].Shade);
	switch (static_cast<PenStyle>(TempVorl[i].Dash))
		{
		case SolidLine:
			Dashes->setCurrentItem(0);
			break;
		case DashLine:
			Dashes->setCurrentItem(1);
			break;
		case DotLine:
			Dashes->setCurrentItem(2);
			break;
		case DashDotLine:
			Dashes->setCurrentItem(3);
			break;
		case DashDotDotLine:
			Dashes->setCurrentItem(4);
			break;
		default:
			Dashes->setCurrentItem(0);
			break;
		}
	switch (static_cast<PenCapStyle>(TempVorl[i].LineEnd))
		{
		case FlatCap:
			LineEnds->setCurrentItem(0);
			break;
		case SquareCap:
			LineEnds->setCurrentItem(1);
			break;
		case RoundCap:
			LineEnds->setCurrentItem(2);
			break;
		default:
			LineEnds->setCurrentItem(0);
			break;
		}
	switch (static_cast<PenJoinStyle>(TempVorl[i].LineJoin))
		{
		case MiterJoin:
			LineJoin->setCurrentItem(0);
			break;
		case BevelJoin:
			LineJoin->setCurrentItem(1);
			break;
		case RoundJoin:
			LineJoin->setCurrentItem(2);
			break;
		default:
			LineJoin->setCurrentItem(0);
			break;
		}
	connect(Styles, SIGNAL(highlighted(int)), this, SLOT(slotEditStyle(int)));
	connect(Dashes, SIGNAL(activated(int)), this, SLOT(NewLSty()));
	connect(LineJoin, SIGNAL(activated(int)), this, SLOT(NewLJoin()));
	connect(LineEnds, SIGNAL(activated(int)), this, SLOT(NewLEnd()));
	connect(LWidth, SIGNAL(valueChanged(int)), this, SLOT(NewLWidth()));
	connect(Color, SIGNAL(activated(int)), this, SLOT(NewLColor()));
	connect(Shade, SIGNAL(valueChanged(int)), this, SLOT(NewLShade()));
}

