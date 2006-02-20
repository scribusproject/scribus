/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "colorm.h"
#include "colorm.moc"
#include <qvariant.h>
#include <qfontmetrics.h>
#include <qpixmap.h>
#include <cstdlib>

#include "commonstrings.h"
#include "customfdialog.h"
#include "dcolor.h"
#include "scribusXml.h"
#include "cmykfw.h"
#include "query.h"
#include "scribus.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "scpaths.h"

#include "scconfig.h"
#include "util.h"
#include "dynamictip.h"

ColorListBoxItem::ColorListBoxItem( const ScColor& col, const QString colName ) : QListBoxItem()
{
	color = col;
	setText(colName);
}

int	ColorListBoxItem::height( const QListBox *lb ) const
{
	int h;
	QString colorName = text();
    if ( colorName.isEmpty() )
		h = 15;
    else
		h = QMAX( 15, lb->fontMetrics().lineSpacing() + 2 );
    return QMAX( h, QApplication::globalStrut().height() );
}

ColorSmallPixmapItem::ColorSmallPixmapItem( const ScColor& col, const QString colName ) : ColorListBoxItem(col, colName)
{
}

void ColorSmallPixmapItem::paint( QPainter *p )
{
	static QPixmap smallPix(15, 15);

	int itemHeight = height( listBox() );
    int yPos;
		
	QColor rgb = color.getRawRGBColor();
	smallPix.fill(rgb);
	QPainter painter(&smallPix);
	painter.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	painter.setPen(b);
	painter.drawRect(0, 0, 15, 15);
	painter.end();

	QRect rect;
	rect.setCoords(3, 0, 15, 15);

	yPos = ( itemHeight - 15 ) / 2;
	p->drawPixmap( 3, yPos, smallPix );

	if( !text().isEmpty() )
	{
		QFontMetrics fm = p->fontMetrics();
		yPos = ( ( itemHeight - fm.height() ) / 2 ) + fm.ascent();
		p->drawText( smallPix.width() + 5, yPos, text() );
	}
}

int	ColorSmallPixmapItem::width( const QListBox *lb )  const
{
	QString colorName = text();
    if ( text().isEmpty() )
		return QMAX( 21, QApplication::globalStrut().width() );
    return QMAX( 21 + lb->fontMetrics().width( colorName ), QApplication::globalStrut().width() );
}

ColorWidePixmapItem::ColorWidePixmapItem( const ScColor& col, const QString colName ) : ColorListBoxItem(col, colName)
{
}

void ColorWidePixmapItem::paint( QPainter *p )
{
	static QPixmap widePix(30, 15);

	int itemHeight = height( listBox() );
    int yPos;
		
	QColor rgb = color.getRawRGBColor();
	widePix.fill(rgb);

	QRect rect;
	rect.setCoords(3, 0, 30, 15);

	yPos = ( itemHeight - 15 ) / 2;
	p->drawPixmap( 3, yPos, widePix );

	if( !text().isEmpty() )
	{
		QFontMetrics fm = p->fontMetrics();
		yPos = ( ( itemHeight - fm.height() ) / 2 ) + fm.ascent();
		p->drawText( widePix.width() + 5, yPos, text() );
	}
}

int	ColorWidePixmapItem::width( const QListBox *lb )  const
{
	QString colorName = text();
    if ( text().isEmpty() )
		return QMAX( 36, QApplication::globalStrut().width() );
    return QMAX( 36 + lb->fontMetrics().width( colorName ), QApplication::globalStrut().width() );
}

ColorFancyPixmapItem::ColorFancyPixmapItem( const ScColor& col, const QString colName ) : ColorListBoxItem(col, colName)
{
}

void ColorFancyPixmapItem::paint( QPainter *p )
{
	static QPixmap cellPix(60, 15);
	static QPixmap smallPix(15, 15);

	static QPixmap alertIcon;
	static QPixmap cmykIcon;
	static QPixmap rgbIcon;
	static QPixmap spotIcon;
	static QPixmap regIcon;
	static bool iconsInitialized = false;

	if ( !iconsInitialized ) {
		alertIcon = loadIcon("alert.png");
		cmykIcon = loadIcon("cmyk.png");
		rgbIcon = loadIcon("rgb.png");
		spotIcon = loadIcon("spot.png");
		regIcon = loadIcon("register.png");
		iconsInitialized = true;
	}

	int itemHeight = height( listBox() );
    int yPos;
		
	QColor rgb = color.getRawRGBColor();
	smallPix.fill(rgb);
	QPainter painter(&smallPix);
	painter.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	painter.setPen(b);
	painter.drawRect(0, 0, 15, 15);
	painter.end();

	cellPix.fill(Qt::white);
	paintAlert(smallPix, cellPix, 0, 0);
	color.checkGamut();
	if (color.isOutOfGamut())
		paintAlert(alertIcon, cellPix, 15, 0);
	if ((color.getColorModel() == colorModelCMYK) || (color.isSpotColor()))
		paintAlert(cmykIcon, cellPix, 30, 0);
	else
		paintAlert(rgbIcon, cellPix, 30, 0);
	if (color.isSpotColor())
		paintAlert(spotIcon, cellPix, 46, 2);
	if (color.isRegistrationColor())
		paintAlert(regIcon, cellPix, 45, 0);

	QRect rect;
	rect.setCoords(3, 0, 60, 15);

	yPos = ( itemHeight - 15 ) / 2;
	p->drawPixmap( 3, yPos, cellPix );

	if( !text().isEmpty() )
	{
		QFontMetrics fm = p->fontMetrics();
		yPos = ( ( itemHeight - fm.height() ) / 2 ) + fm.ascent();
		p->drawText( cellPix.width() + 5, yPos, text() );
	}
}

int	ColorFancyPixmapItem::width( const QListBox *lb )  const
{
	QString colorName = text();
    if ( text().isEmpty() )
		return QMAX( 66, QApplication::globalStrut().width() );
    return QMAX( 66 + lb->fontMetrics().width( colorName ), QApplication::globalStrut().width() );
}

ColorListBox::ColorListBox(QWidget * parent, const char * name, WFlags f)
	: QListBox(parent, name, f)
{
	if (name == "")
		setName("ColorListBox");
}

void ColorListBox::updateBox(ColorList list, ColorListBox::PixmapType type, bool clearFirst)
{
	ColorList::Iterator it;
	if (clearFirst)
		clear();
	for (it = list.begin(); it != list.end(); ++it)
	{
		// if condition 10/21/2004 pv #1191
		if (it.key() == CommonStrings::None || it.key() == tr("None"))
			continue;

		ScColor col = list[it.key()];
		if (type == ColorListBox::fancyPixmap)
			insertItem( new ColorFancyPixmapItem(col, it.key()) );
		else if (type == ColorListBox::widePixmap)
			insertItem( new ColorWidePixmapItem(col, it.key()) );
		else if (type == ColorListBox::smallPixmap)
			insertItem( new ColorSmallPixmapItem(col, it.key()) );
	}
}

ColorManager::ColorManager(QWidget* parent, ColorList doco, bool haveDoc, QString docColSet, QStringList custColSet)
		: QDialog( parent, "ColorManager", true, 0 )
{
	setName( "ColorManager" );
	HaveDoc = haveDoc;
	customColSet = custColSet;
	setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
	setSizeGripEnabled(true);
	setCaption( tr( "Colors" ) );
	setIcon(loadIcon("AppIcon.png"));
	Layout2 = new QVBoxLayout( this );
	Layout2->setSpacing( 6 );
	Layout2->setMargin( 11 );

	layout5 = new QHBoxLayout( 0, 0, 6, "layout5");
	layout3 = new QVBoxLayout( 0, 0, 6, "layout3");
	colorListBox = new ColorListBox( this, "colorListBox" );
	colorListBox->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, colorListBox->sizePolicy().hasHeightForWidth() ) );
	colorListBox->setMinimumSize( QSize( 164, 228 ) );
	colorListBox->setColumnMode( QListBox::FixedNumber );
	layout5->addWidget( colorListBox );

	ColorsGroup = new QGroupBox( this, "ColorsGroup" );
	ColorsGroup->setColumnLayout(0, Qt::Vertical );
	ColorsGroup->layout()->setSpacing( 6 );
	ColorsGroup->layout()->setMargin( 11 );
	Layout1 = new QVBoxLayout( ColorsGroup->layout() );
	Layout1->setAlignment( Qt::AlignTop );
	LoadF = new QPushButton( tr( "&Import" ), ColorsGroup, "LoadF" );
	Layout1->addWidget( LoadF );
	NewF = new QPushButton( tr( "&New" ), ColorsGroup, "NewF" );
	Layout1->addWidget( NewF );
	EditF = new QPushButton( tr( "&Edit" ), ColorsGroup, "EditF" );
	EditF->setEnabled( false );
	EditF->setDefault( true );
	Layout1->addWidget( EditF );
	DupF = new QPushButton( tr( "D&uplicate" ), ColorsGroup, "DupF" );
	DupF->setEnabled( false );
	Layout1->addWidget( DupF );
	DelF = new QPushButton( tr( "&Delete" ), ColorsGroup, "DelF" );
	DelF->setEnabled( false );
	Layout1->addWidget( DelF );
	if (HaveDoc)
	{
		DelU = new QPushButton( tr( "&Remove Unused" ), ColorsGroup, "DelU" );
		Layout1->addWidget( DelU );
	}
	layout3->addWidget( ColorsGroup );
	if (!HaveDoc)
	{
		ColsSetGroup = new QGroupBox( this, "ColsSetGroup" );
		ColsSetGroup->setTitle( tr( "Color Sets" ) );
		ColsSetGroup->setColumnLayout(0, Qt::Vertical );
		ColsSetGroup->layout()->setSpacing( 6 );
		ColsSetGroup->layout()->setMargin( 11 );
		ColsSetGroupLayout = new QVBoxLayout( ColsSetGroup->layout() );
		ColsSetGroupLayout->setAlignment( Qt::AlignTop );
		textLabel1 = new QLabel( ColsSetGroup, "textLabel1" );
		textLabel1->setText( tr( "Current Color Set:" ) );
		ColsSetGroupLayout->addWidget( textLabel1 );
		CSets = new QPopupMenu();
		CSets->insertItem("Scribus Small");
/*
		CSets->insertItem("X11 RGB-Set");
		CSets->insertItem("X11 Grey-Set");
		CSets->insertItem("Gnome-Set");
		CSets->insertItem("SVG-Set");
		CSets->insertItem("OpenOffice.org-Set");
*/
		csm.findPaletteLocations();
		csm.findPalettes();
		QStringList allSets(csm.paletteNames());
		for ( QStringList::Iterator it = allSets.begin(); it != allSets.end(); ++it )
		{
			CSets->insertItem((*it));
		}
		customSetStartIndex=CSets->count();
		if (custColSet.count() != 0)
		{
			QStringList realEx;
			realEx.clear();
			for (uint m = 0; m < custColSet.count(); ++m)
			{
				QString Cpfad = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/"+custColSet[m]);
				QFileInfo cfi(Cpfad);
				if (cfi.exists())
				{
					CSets->insertItem(custColSet[m]);
					realEx.append(custColSet[m]);
				}
			}
			customColSet = realEx;
		}
		LoadColSet = new QToolButton( ColsSetGroup, "LoadColSet" );
		LoadColSet->setPopup(CSets);
		LoadColSet->setPopupDelay(0);
		LoadColSet->setText(docColSet);
		ColsSetGroupLayout->addWidget( LoadColSet );
		SaveColSet = new QPushButton( tr( "&Save Color Set" ), ColsSetGroup, "SaveColSet" );
		ColsSetGroupLayout->addWidget( SaveColSet );
		layout3->addWidget( ColsSetGroup );
	}
	SaveF = new QPushButton( CommonStrings::tr_OK, this, "SaveF" );
	layout3->addWidget( SaveF );
	CancF = new QPushButton( CommonStrings::tr_Cancel, this, "CancF" );
	CancF->setDefault( true );
	layout3->addWidget( CancF );
	layout5->addLayout( layout3 );
	Layout2->addLayout( layout5 );
	replaceMap.clear();
	EditColors = doco;
	dynTip = new DynamicTip(colorListBox, &EditColors);
	updateCList();
	// signals and slots connections
	if (!HaveDoc)
	{
		connect(CSets, SIGNAL(activated(int)), this, SLOT(loadDefaults(int)));
		connect(SaveColSet, SIGNAL( clicked() ), this, SLOT( saveDefaults() ) );
		QToolTip::add( LoadColSet, "<qt>" + tr( "Choose a color set to load" ) + "</qt>");
		QToolTip::add( SaveColSet, "<qt>" + tr( "Save the current color set" ) + "</qt>");
	}
	else
	{
		connect(DelU, SIGNAL( clicked() ), this, SLOT( delUnused() ) );
		QToolTip::add( DelU, "<qt>" + tr( "Remove unused colors from current document's color set" ) + "</qt>");
	}
	QToolTip::add( LoadF, "<qt>" + tr( "Import colors to the current set from an existing document" ) + "</qt>");
	QToolTip::add( NewF, "<qt>" + tr( "Create a new color within the current set" ) + "</qt>");
	QToolTip::add( EditF, "<qt>" + tr( "Edit the currently selected color" ) + "</qt>");
	QToolTip::add( DupF, "<qt>" + tr( "Make a copy of the currently selected color" ) + "</qt>");
	QToolTip::add( DelF, "<qt>" + tr( "Delete the currently selected color" ) + "</qt>");
	QToolTip::add( SaveF, "<qt>" + tr( "Make the current colorset the default color set" ) + "</qt>");
	connect( SaveF, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( CancF, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( NewF, SIGNAL( clicked() ), this, SLOT( neueFarbe() ) );
	connect( EditF, SIGNAL( clicked() ), this, SLOT( editFarbe() ) );
	connect( DupF, SIGNAL( clicked() ), this, SLOT( duplFarbe() ) );
	connect( DelF, SIGNAL( clicked() ), this, SLOT( delFarbe() ) );
	connect( LoadF, SIGNAL( clicked() ), this, SLOT( loadFarben() ) );
	connect( colorListBox, SIGNAL( highlighted(QListBoxItem*) ), this, SLOT( selFarbe(QListBoxItem*) ) );
	connect( colorListBox, SIGNAL( selected(QListBoxItem*) ), this, SLOT( selEditFarbe(QListBoxItem*) ) );
}

void ColorManager::saveDefaults()
{
	QString Cpfad = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/");
	QString Name = LoadColSet->text();
	Query* dia = new Query(this, "Name", 1, 0, tr("&Name:"), tr("Choose a Name"));
	if ((Name == "Scribus Small") || (Name == "X11 RGB-Set") || (Name == "OpenOffice.org-Set")
	        || (Name == "X11 Grey-Set") || (Name == "Gnome-Set") || (Name == "SVG-Set"))
		dia->setEditText("", false);
	else
		dia->setEditText(Name, false);
	if (dia->exec())
	{
		QString Fname = Cpfad+dia->getEditText();
		LoadColSet->setText(dia->getEditText());
		QFile fx(Fname);
		if (fx.open(IO_WriteOnly))
		{
			QTextStream tsx(&fx);
			QString tmp;
			ColorList::Iterator itc;
			tsx << "Color Set:"+dia->getEditText()+"\n";
			int cp, mp, yp, kp;
			for (itc = EditColors.begin(); itc != EditColors.end(); ++itc)
			{
				EditColors[itc.key()].getCMYK(&cp, &mp, &yp, &kp);
				tsx << tmp.setNum(cp) << "\t" ;
				tsx << tmp.setNum(mp) << "\t" ;
				tsx << tmp.setNum(yp) << "\t" ;
				tsx << tmp.setNum(kp) << "\t" ;
				tsx << itc.key() << "\n" ;
			}
			fx.close();
			if (dia->getEditText() != Name)
			{
				customColSet.append(dia->getEditText());
				CSets->insertItem(dia->getEditText());
			}
		}
	}
	delete dia;
}

void ColorManager::loadDefaults(int id)
{
	int c = CSets->indexOf(id);
	bool cus = false;
	LoadColSet->setText(CSets->text(id));
	EditColors.clear();
	QString Cpfad = QDir::convertSeparators(QDir::homeDirPath()+"/.scribus/"+CSets->text(id));
	QString pfadC = ScPaths::instance().libDir()+"swatches/";
	QString pfadC2 = pfadC + "Scribus_X11.txt";
	switch (c)
	{
	case 0:
		LoadColSet->setText("Scribus Small");
		EditColors.insert("White", ScColor(0, 0, 0, 0));
		EditColors.insert("Black", ScColor(0, 0, 0, 255));
		EditColors.insert("Blue", ScColor(255, 255, 0, 0));
		EditColors.insert("Cyan", ScColor(255, 0, 0, 0));
		EditColors.insert("Green", ScColor(255, 0, 255, 0));
		EditColors.insert("Red", ScColor(0, 255, 255, 0));
		EditColors.insert("Yellow", ScColor(0, 0, 255, 0));
		EditColors.insert("Magenta", ScColor(0, 255, 0, 0));
		break;
		/*
	case 1:
		pfadC2 = pfadC + "Scribus_X11.txt";
		break;
	case 2:
		pfadC2 = pfadC + "Scribus_X11Grey.txt";
		break;
	case 3:
		pfadC2 = pfadC + "Scribus_Gnome.txt";
		break;
	case 4:
		pfadC2 = pfadC + "Scribus_SVG.txt";
		break;
	case 5:
		pfadC2 = pfadC + "Scribus_OpenOffice.txt";
		cus = true;
		break;*/
	default:
		if (c<customSetStartIndex)
		{
			QString listText=CSets->text(id);
			if (listText=="Scribus OpenOffice")
				cus=true;
			pfadC2 = csm.paletteFileFromName(listText);
		}
		else
		{
			pfadC2 = Cpfad;
			cus = true;
		}
		break;
	}
	if (c != 0)
	{
		QFile fiC(pfadC2);
		if (fiC.open(IO_ReadOnly))
		{
			QString ColorEn, Cname;
			int Rval, Gval, Bval, Kval;
			QTextStream tsC(&fiC);
			ColorEn = tsC.readLine();
			while (!tsC.atEnd())
			{
				ScColor tmp;
				ColorEn = tsC.readLine();
				if (ColorEn.length()>0 && ColorEn[0]==QChar('#'))
					continue;
				QTextStream CoE(&ColorEn, IO_ReadOnly);
				CoE >> Rval;
				CoE >> Gval;
				CoE >> Bval;
				if (cus)
				{
					CoE >> Kval;
					Cname = CoE.read().stripWhiteSpace();
					tmp.setColor(Rval, Gval, Bval, Kval);
				}
				else
				{
					Cname = CoE.read().stripWhiteSpace();
					tmp.setColorRGB(Rval, Gval, Bval);
				}
				if ((c<customSetStartIndex) && (Cname.length()==0))
				{
					if (!cus)
						Cname=QString("#%1%2%3").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).upper();
					else
						Cname=QString("#%1%2%3%4").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).arg(Kval,2,16).upper();
					Cname.replace(" ","0");
				}
				if (EditColors.contains(Cname))
				{
					if (tmp==EditColors[Cname])
						continue;
					Cname=QString("%1%2").arg(Cname).arg(EditColors.count());
				}
				EditColors.insert(Cname, tmp);
			}
			fiC.close();
		}
		else
		{
			LoadColSet->setText("Scribus Small");
			EditColors.insert("White", ScColor(0, 0, 0, 0));
			EditColors.insert("Black", ScColor(0, 0, 0, 255));
			EditColors.insert("Blue", ScColor(255, 255, 0, 0));
			EditColors.insert("Cyan", ScColor(255, 0, 0, 0));
			EditColors.insert("Green", ScColor(255, 0, 255, 0));
			EditColors.insert("Red", ScColor(0, 255, 255, 0));
			EditColors.insert("Yellow", ScColor(0, 0, 255, 0));
			EditColors.insert("Magenta", ScColor(0, 255, 0, 0));
		}
	}
	updateCList();
}

void ColorManager::loadFarben()
{
	QString fileName;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("colors", ".");
#ifdef HAVE_LIBZ
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		dirs->set("colors", fileName.left(fileName.findRev("/")));
		ScriXmlDoc *ss = new ScriXmlDoc();
		if (ss->ReadColors(fileName))
		{
			ColorList LColors = ss->Farben;
			ColorList::Iterator it;
			for (it = LColors.begin(); it != LColors.end(); ++it)
			{
				if (!EditColors.contains(it.key()))
					EditColors.insert(it.key(), it.data());
			}
			updateCList();
		}
		delete ss;
	}
}

void ColorManager::delUnused()
{
	PageItem* ite;
	bool found;
	UsedC.clear();
	ColorList::Iterator it;
	for (it = EditColors.begin(); it != EditColors.end(); ++it)
	{
		found = false;
		if ((it.key() == ScMW->doc->toolSettings.dBrush) || (it.key() == ScMW->doc->toolSettings.dPen) ||
		        (it.key() == ScMW->doc->toolSettings.dBrushPict)
		        || (it.key() == ScMW->doc->toolSettings.dPenLine) || (it.key() == ScMW->doc->toolSettings.dPenText))
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < ScMW->doc->MasterItems.count(); ++c)
		{
			ite = ScMW->doc->MasterItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((ite->asTextFrame()) || (ite->asPathText()))
			{
				for (uint d=0; d<ite->itemText.count(); ++d)
				{
					if (it.key() == ite->itemText.at(d)->ccolor)
						found = true;
					if (it.key() == ite->itemText.at(d)->cstroke)
						found = true;
					if (found)
						break;
				}
			}
			/* PFJ - 29.02.04 - merged if's to one line */
			if ((it.key() == ite->fillColor()) || (it.key() == ite->lineColor()))
				found = true;
			if (found)
				break;
		}
		if (found)
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < ScMW->doc->FrameItems.count(); ++c)
		{
			ite = ScMW->doc->FrameItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((ite->asTextFrame()) || (ite->asPathText()))
			{
				for (uint d=0; d<ite->itemText.count(); ++d)
				{
					if (it.key() == ite->itemText.at(d)->ccolor)
						found = true;
					if (it.key() == ite->itemText.at(d)->cstroke)
						found = true;
					if (found)
						break;
				}
			}
			/* PFJ - 29.02.04 - merged if's to one line */
			if ((it.key() == ite->fillColor()) || (it.key() == ite->lineColor()))
				found = true;
			if (found)
				break;
		}
		if (found)
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < ScMW->doc->DocItems.count(); ++c)
		{
			ite = ScMW->doc->DocItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((ite->asTextFrame()) || (ite->asPathText()))
			{
				for (uint d=0; d<ite->itemText.count(); ++d)
				{
					/* PFJ - 29.02.04 - Merged if's */
					if ((it.key() == ite->itemText.at(d)->ccolor) ||
							(it.key() == ite->itemText.at(d)->cstroke))
						found = true;
					if (found)
						break;
				}
			}
			/* PFJ - 29.02.04 - Merged if's */
			if ((it.key() == ite->fillColor()) || (it.key() == ite->lineColor()))
				found = true;
			if (found)
				break;
		}
		/* PFJ - 29.02.04 - Merged if's */
		if ((it.key() == ScMW->doc->CurrTextFill) ||
		        (it.key() == ScMW->doc->CurrTextStroke))
			found = true;
		if (found)
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
	}
	EditColors = UsedC;
	if (EditColors.count() == 0)
	{
		EditColors.insert("White", ScColor(0, 0, 0, 0));
		EditColors.insert("Black", ScColor(0, 0, 0, 255));
	}
	updateCList();
}

void ColorManager::duplFarbe()
{
	QString nam = tr("Copy of %1").arg(sFarbe);
	EditColors.insert(nam, EditColors[sFarbe]);
	sFarbe = nam;
	editFarbe();
	updateCList();
}

void ColorManager::neueFarbe()
{
	ScColor tmpFarbe = ScColor(0, 0, 0, 0);
	CMYKChoose* dia = new CMYKChoose(this, tmpFarbe, tr("New Color"), &EditColors, customColSet);
	int newItemIndex=0;
	int colCount=0;
	if (dia->exec())
	{
		dia->Farbe.setSpotColor(dia->Separations->isChecked());
		ColorList::Iterator itnew=EditColors.insert(dia->Farbname->text(), dia->Farbe);
		bool regChecked=dia->Regist->isChecked();
		ColorList::Iterator it;
		for (it = EditColors.begin(); it != EditColors.end(); ++it)
		{
			if (regChecked)
				EditColors[it.key()].setRegistrationColor(false);
			if (it==itnew)
				newItemIndex=colCount;
			++colCount;
		}
		EditColors[dia->Farbname->text()].setRegistrationColor(dia->Regist->isChecked());
		updateCList();
	}
	delete dia;
	colorListBox->setSelected(newItemIndex, true);
	colorListBox->setTopItem(newItemIndex);
}

void ColorManager::editFarbe()
{
	int selectedIndex=colorListBox->currentItem();
	int topIndex=colorListBox->topItem();
	ScColor tmpFarbe = EditColors[sFarbe];
	CMYKChoose* dia = new CMYKChoose(this, tmpFarbe, sFarbe, &EditColors, customColSet);
	if (dia->exec())
	{
		dia->Farbe.setSpotColor(dia->Separations->isChecked());
		EditColors[dia->Farbname->text()] = dia->Farbe;
		if (dia->Regist->isChecked())
		{
			ColorList::Iterator it;
			for (it = EditColors.begin(); it != EditColors.end(); ++it)
			{
				EditColors[it.key()].setRegistrationColor(false);
			}
		}
		EditColors[dia->Farbname->text()].setRegistrationColor(dia->Regist->isChecked());
		if (sFarbe != dia->Farbname->text())
		{
			replaceMap.insert(sFarbe, dia->Farbname->text());
			EditColors.remove(sFarbe);
		}
		updateCList();
	}
	delete dia;
	colorListBox->setSelected(selectedIndex, true);
	colorListBox->setTopItem(topIndex);
}

void ColorManager::delFarbe()
{
	int selectedIndex=colorListBox->currentItem();
	int topIndex=colorListBox->topItem();
	DelColor *dia = new DelColor(this, EditColors, sFarbe, HaveDoc);
	if (dia->exec())
	{
		if (replaceMap.values().contains(sFarbe))
		{
			QMap<QString,QString>::Iterator it;
			for (it = replaceMap.begin(); it != replaceMap.end(); ++it)
			{
				if (it.data() == sFarbe)
					it.data() = dia->getReplacementColor();
			}
		}
		replaceMap.insert(sFarbe, dia->getReplacementColor());
		EditColors.remove(sFarbe);
		updateCList();
	}
	delete dia;
	int listBoxCount=colorListBox->count();
	if (listBoxCount>selectedIndex)
		colorListBox->setSelected(selectedIndex, true);
	if (listBoxCount>topIndex)
		colorListBox->setTopItem(topIndex);
}

void ColorManager::selFarbe(QListBoxItem *c)
{
	sFarbe = c->text();
	EditF->setEnabled(true);
	DupF->setEnabled(true);
	DelF->setEnabled(EditColors.count() == 1 ? false : true);
}

void ColorManager::selEditFarbe(QListBoxItem *c)
{
	sFarbe = c->text();
	EditF->setEnabled(true);
	DupF->setEnabled(true);
	DelF->setEnabled(EditColors.count() == 1 ? false : true);
	editFarbe();
}

void ColorManager::updateCList()
{
	colorListBox->updateBox(EditColors, ColorListBox::fancyPixmap);
	DelF->setEnabled(EditColors.count() == 1 ? false : true);
	if (colorListBox->currentItem() == -1)
	{
		DupF->setEnabled(false);
		EditF->setEnabled(false);
		DelF->setEnabled(false);
	}
	colorListBox->setSelected(colorListBox->currentItem(), false);
}

QString ColorManager::getColorSetName()
{
	return LoadColSet->text();
}
