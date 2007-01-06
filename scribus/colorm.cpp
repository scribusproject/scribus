/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "colorm.h"
#include "colorm.moc"
#include <qpainter.h>
#include <qvariant.h>
#include <qfontmetrics.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <cstdlib>

#include "commonstrings.h"
#include "customfdialog.h"
#include "dcolor.h"
#include "fileloader.h"
#include "cmykfw.h"
#include "query.h"
#include "scribusdoc.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "scpaths.h"

#include "scconfig.h"
#include "colorutil.h"
#include "util.h"
#include "dynamictip.h"
#include "sccolorengine.h"

ColorSmallPixmapItem::ColorSmallPixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName ) 
					: ScListBoxPixmap<15,15>()
{
	m_color = col;
	m_doc = doc;
	setText(colName);
}

void ColorSmallPixmapItem::redraw(void)
{
	QPixmap* pPixmap = ScListBoxPixmap<15,15>::pmap.get();
	QColor rgb = ScColorEngine::getDisplayColor(m_color, m_doc);
	pPixmap->fill(rgb);
	QPainter painter(pPixmap);
	painter.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	painter.setPen(b);
	painter.drawRect(0, 0, 15, 15);
	painter.end();
}

ColorWidePixmapItem::ColorWidePixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName ) 
					: ScListBoxPixmap<30,15>()
{
	m_color = col;
	m_doc = doc;
	setText(colName);
}

void ColorWidePixmapItem::redraw(void)
{
	QColor rgb = ScColorEngine::getDisplayColor(m_color, m_doc);
	ScListBoxPixmap<30,15>::pmap->fill(rgb);
}

ColorFancyPixmapItem::ColorFancyPixmapItem( const ScColor& col, ScribusDoc* doc, const QString colName ) 
					: ScListBoxPixmap<60,15>()
{
	m_color = col;
	m_doc = doc;
	setText(colName);
}

void ColorFancyPixmapItem::redraw(void)
{
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

	QColor rgb = ScColorEngine::getDisplayColor(m_color, m_doc);
	smallPix.fill(rgb);
	QPainter painter(&smallPix);
	painter.setBrush(Qt::NoBrush);
	QPen b(Qt::black, 1);
	painter.setPen(b);
	painter.drawRect(0, 0, 15, 15);
	painter.end();

	QPixmap* pPixmap = ScListBoxPixmap<60,15>::pmap.get();
	pPixmap->fill(Qt::color0);
	paintAlert(smallPix, *pPixmap, 0, 0);
	bool isOutOfGamut = ScColorEngine::isOutOfGamut(m_color, m_doc);
	if (isOutOfGamut)
		paintAlert(alertIcon, *pPixmap, 15, 0);
	if ((m_color.getColorModel() == colorModelCMYK) || (m_color.isSpotColor()))
		paintAlert(cmykIcon, *pPixmap, 30, 0);
	else
		paintAlert(rgbIcon, *pPixmap, 30, 0);
	if (m_color.isSpotColor())
		paintAlert(spotIcon, *pPixmap, 45, 0);
	if (m_color.isRegistrationColor())
		paintAlert(regIcon, *pPixmap, 46, 0);
	if (pPixmap->mask() && ((!m_color.isSpotColor() && !m_color.isRegistrationColor()) || !isOutOfGamut))
	{
		QPainter alpha; // transparency handling
		alpha.begin(pPixmap->mask());
		alpha.setBrush(Qt::color0);
		alpha.setPen(Qt::color0);
		if (!m_color.isSpotColor() && !m_color.isRegistrationColor())
			alpha.drawRect(45, 0, 15, 15);
		if (!isOutOfGamut)
			alpha.drawRect(15, 0, 15, 15);
		alpha.end();
	}
}

ColorListBox::ColorListBox(QWidget * parent, const char * name, WFlags f)
	: QListBox(parent, name, f)
{
	if (name == "")
		setName("ColorListBox");
}

void ColorListBox::updateBox(ColorList& list, ColorListBox::PixmapType type, bool clearFirst)
{
	if (clearFirst)
		clear();
	if (type == ColorListBox::fancyPixmap)
		insertFancyPixmapItems( list );
	else if (type == ColorListBox::widePixmap)
		insertWidePixmapItems( list );
	else if (type == ColorListBox::smallPixmap)
		insertSmallPixmapItems( list );
}

void ColorListBox::insertSmallPixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (it.key() == CommonStrings::None || it.key() == CommonStrings::NoneColor)
			continue;
		insertItem( new ColorSmallPixmapItem(it.data(), doc, it.key()) );
	}
}

void ColorListBox::insertWidePixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (it.key() == CommonStrings::None || it.key() == CommonStrings::NoneColor)
			continue;
		insertItem( new ColorWidePixmapItem(it.data(), doc, it.key()) );
	}
}

void ColorListBox::insertFancyPixmapItems(ColorList& list)
{
	ColorList::Iterator it;
	ScribusDoc* doc = list.document();
	for (it = list.begin(); it != list.end(); ++it)
	{
		if (it.key() == CommonStrings::None || it.key() == CommonStrings::NoneColor)
			continue;
		insertItem( new ColorFancyPixmapItem(it.data(), doc, it.key()) );
	}
}

ColorManager::ColorManager(QWidget* parent, ColorList doco, ScribusDoc* doc, QString docColSet, QStringList custColSet)
		: QDialog( parent, "ColorManager", true, 0 ), EditColors(doc, true), UsedC(doc, true)
{
	setName( "ColorManager" );
	m_Doc=doc;
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
	if (m_Doc!=0)
	{
		DelU = new QPushButton( tr( "&Remove Unused" ), ColorsGroup, "DelU" );
		Layout1->addWidget( DelU );
	}
	layout3->addWidget( ColorsGroup );
	if (m_Doc==0)
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
				QString Cpfad = QDir::convertSeparators(ScPaths::getApplicationDataDir() + custColSet[m]);
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
	if (m_Doc==0)
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
        QToolTip::add( colorListBox, "<qt>" + tr( "If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected.What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management." ) + "</qt>");
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
	QString Cpfad = QDir::convertSeparators(ScPaths::getApplicationDataDir());
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
			CMYKColor cmyk;
			QTextStream tsx(&fx);
			QString tmp;
			ColorList::Iterator itc;
			tsx << "Color Set:"+dia->getEditText()+"\n";
			int cp, mp, yp, kp;
			for (itc = EditColors.begin(); itc != EditColors.end(); ++itc)
			{
				ScColorEngine::getCMYKValues(itc.data(), m_Doc, cmyk);
				cmyk.getValues(cp, mp, yp, kp);
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
	QString Cpfad = QDir::convertSeparators(ScPaths::getApplicationDataDir()+CSets->text(id));
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
				ColorEn = tsC.readLine().stripWhiteSpace();
				if (ColorEn.length()>0 && ColorEn[0]==QChar('#'))
					continue;
				
				if (ColorEn[0].isNumber()) {
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
				}
				else {
					QStringList fields = QStringList::split(QChar(9), ColorEn);
					if (fields.count() != 5)
						continue;
					Cname = fields[0];
					Rval = fields[1].toInt();
					Gval = fields[2].toInt();
					Bval = fields[3].toInt();
					Kval = fields[4].toInt();
					tmp.setColor(Rval, Gval, Bval, Kval);
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
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;Other Files (*.eps *.epsi *.ps *.ai);;All Files (*)"));
#else
	CustomFDialog dia(this, wdir, tr("Open"), tr("Documents (*.sla *.scd);;Other Files (*.eps *.epsi *.ps *.ai);;All Files (*)"));
#endif
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		uint oldCount = EditColors.count();
		dirs->set("colors", fileName.left(fileName.findRev("/")));
		QFileInfo fi = QFileInfo(fileName);
		QString ext = fi.extension(false).lower();
		if ((ext == "ps") || (ext == "eps") || (ext == "epsi") || (ext == "ai"))
		{
			QString tmp, tmp2, FarNam;
			double c, m, y, k;
			ScColor cc;
			QFile f(fileName);
			if (f.open(IO_ReadOnly))
			{
				QTextStream ts(&f);
				while (!ts.atEnd())
				{
					tmp = ts.readLine();
					if ((tmp.startsWith("%%CMYKCustomColor")) || (tmp.startsWith("%%CMYKProcessColor")))
					{
						if (tmp.startsWith("%%CMYKCustomColor"))
							tmp = tmp.remove(0,18);
						else if (tmp.startsWith("%%CMYKProcessColor"))
							tmp = tmp.remove(0,19);
						QTextStream ts2(&tmp, IO_ReadOnly);
						ts2 >> c >> m >> y >> k;
						FarNam = ts2.read();
						FarNam = FarNam.stripWhiteSpace();
						FarNam = FarNam.remove(0,1);
						FarNam = FarNam.remove(FarNam.length()-1,1);
						FarNam = FarNam.simplifyWhiteSpace();
						cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
						cc.setSpotColor(true);
						if (!EditColors.contains(FarNam))
							EditColors.insert(FarNam, cc);
						while (!ts.atEnd())
						{
							uint oldPos = ts.device()->at();
							tmp = ts.readLine();
							if (!tmp.startsWith("%%+"))
							{
								ts.device()->at(oldPos);
								break;
							}
							tmp = tmp.remove(0,3);
							QTextStream ts2(&tmp, IO_ReadOnly);
							ts2 >> c >> m >> y >> k;
							FarNam = ts2.read();
							FarNam = FarNam.stripWhiteSpace();
							FarNam = FarNam.remove(0,1);
							FarNam = FarNam.remove(FarNam.length()-1,1);
							FarNam = FarNam.simplifyWhiteSpace();
							cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
							cc.setSpotColor(true);
							if (!EditColors.contains(FarNam))
								EditColors.insert(FarNam, cc);
						}
					}
					if (tmp.startsWith("%%EndComments"))
					{
						if (ext == "ai")
						{
							while (!ts.atEnd())
							{
								tmp = ts.readLine();
								if ((tmp.endsWith("Xa") || tmp.endsWith(" k")) && (tmp.length() > 4))
								{
									QTextStream ts2(&tmp, IO_ReadOnly);
									ts2 >> c >> m >> y >> k;
									tmp = ts.readLine();
									if (tmp.endsWith("Pc"))
									{
										tmp = tmp.stripWhiteSpace();
										tmp = tmp.remove(0,1);
										int en = tmp.find(")");
										FarNam = tmp.mid(0, en);
										FarNam = FarNam.simplifyWhiteSpace();
										cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
										cc.setSpotColor(true);
										if (!EditColors.contains(FarNam))
											EditColors.insert(FarNam, cc);
									}
								}
							}
						}
						break;
					}
				}
				f.close();
				updateCList();
			}
		}
		else
		{
			FileLoader fl(fileName);
			if (fl.TestFile() == -1)
			//TODO put in nice user warning
				return;
			ColorList LColors;
			if (fl.ReadColors(fileName, LColors))
			{
				ColorList::Iterator it;
				for (it = LColors.begin(); it != LColors.end(); ++it)
				{
					if (!EditColors.contains(it.key()))
						EditColors.insert(it.key(), it.data());
				}
				updateCList();
			}
		}
		if (EditColors.count() == oldCount)
			QMessageBox::information(this, tr("Information"), "<qt>" + tr("The file %1 does not contain colors which can be imported.\nIf the file was a PostScript-based, try to import it with File -&gt; Import. \nNot all files have DSC conformant comments where the color descriptions are located.\n This prevents importing colors from some files.").arg(fileName) + "</qt>", 1, 0, 0);
		
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
		if ((it.key() == m_Doc->toolSettings.dBrush) || (it.key() == m_Doc->toolSettings.dPen) ||
		        (it.key() == m_Doc->toolSettings.dBrushPict)
		        || (it.key() == m_Doc->toolSettings.dPenLine) || (it.key() == m_Doc->toolSettings.dPenText))
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
		QStringList patterns = m_Doc->getUsedPatterns();
		for (uint c = 0; c < patterns.count(); ++c)
		{
			ScPattern pa = m_Doc->docPatterns[patterns[c]];
			for (uint c = 0; c < pa.items.count(); ++c)
			{
				ite = pa.items.at(c);
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
					for (int d=0; d<ite->itemText.length(); ++d)
					{
						if (it.key() == ite->itemText.charStyle(d).fillColor())
							found = true;
						if (it.key() == ite->itemText.charStyle(d).strokeColor())
							found = true;
						if (found)
							break;
					}
				}
				if ((it.key() == ite->fillColor()) || (it.key() == ite->lineColor()))
					found = true;
				if (found)
					break;
			}
			if (found)
				break;
		}
		if (found)
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
		for (uint c = 0; c < m_Doc->MasterItems.count(); ++c)
		{
			ite = m_Doc->MasterItems.at(c);
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
				for (int d=0; d<ite->itemText.length(); ++d)
				{
					if (it.key() == ite->itemText.charStyle(d).fillColor())
						found = true;
					if (it.key() == ite->itemText.charStyle(d).strokeColor())
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
		for (uint c = 0; c < m_Doc->FrameItems.count(); ++c)
		{
			ite = m_Doc->FrameItems.at(c);
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
				for (int d=0; d<ite->itemText.length(); ++d)
				{
					if (it.key() == ite->itemText.charStyle(d).fillColor())
						found = true;
					if (it.key() == ite->itemText.charStyle(d).strokeColor())
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
		for (uint c = 0; c < m_Doc->DocItems.count(); ++c)
		{
			ite = m_Doc->DocItems.at(c);
			QPtrVector<VColorStop> cstops = ite->fill_gradient.colorStops();
			for (uint cst = 0; cst < ite->fill_gradient.Stops(); ++cst)
			{
				if (it.key() == cstops.at(cst)->name)
					found = true;
				if (found)
					break;
			}
			if ((it.key() == ite->itemText.defaultStyle().charStyle().fillColor()) ||
		        (it.key() == ite->itemText.defaultStyle().charStyle().strokeColor()))
				found = true;
			if ((ite->asTextFrame()) || (ite->asPathText()))
			{
				for (int d=0; d<ite->itemText.length(); ++d)
				{
					/* PFJ - 29.02.04 - Merged if's */
					if ((it.key() == ite->itemText.charStyle(d).fillColor()) ||
							(it.key() == ite->itemText.charStyle(d).strokeColor()))
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
		if ((it.key() == m_Doc->currentStyle.charStyle().fillColor()) ||
		        (it.key() == m_Doc->currentStyle.charStyle().strokeColor()))
			found = true;
		if (found)
		{
			UsedC.insert(it.key(), it.data());
			continue;
		}
	}
	EditColors = UsedC;
	EditColors.insert("White", ScColor(0, 0, 0, 0));
	EditColors.insert("Black", ScColor(0, 0, 0, 255));
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
	CMYKChoose* dia = new CMYKChoose(this, m_Doc, tmpFarbe, tr("New Color"), &EditColors, customColSet, true);
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
				it.data().setRegistrationColor(false);
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
	CMYKChoose* dia = new CMYKChoose(this, m_Doc, tmpFarbe, sFarbe, &EditColors, customColSet, false);
	if (dia->exec())
	{
		dia->Farbe.setSpotColor(dia->Separations->isChecked());
		EditColors[dia->Farbname->text()] = dia->Farbe;
		if (dia->Regist->isChecked())
		{
			ColorList::Iterator it;
			for (it = EditColors.begin(); it != EditColors.end(); ++it)
				it.data().setRegistrationColor(false);
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
	DelColor *dia = new DelColor(this, EditColors, sFarbe, (m_Doc!=0));
	if (dia->exec())
	{
		QString replacementColor(dia->getReplacementColor());
		if (replacementColor == CommonStrings::NoneColor)
			replacementColor = CommonStrings::None;
		if (replaceMap.values().contains(sFarbe))
		{
			QMap<QString,QString>::Iterator it;
			for (it = replaceMap.begin(); it != replaceMap.end(); ++it)
			{
				if (it.data() == sFarbe)
					it.data() = replacementColor;
			}
		}
		replaceMap.insert(sFarbe, replacementColor);
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
	bool enableEdit = (sFarbe != "Black" && sFarbe != "White");
	bool enableDel  = (sFarbe != "Black" && sFarbe != "White") && (EditColors.count() > 1);
	EditF->setEnabled(enableEdit);
	DupF->setEnabled(true);
	DelF->setEnabled(enableDel);
}

void ColorManager::selEditFarbe(QListBoxItem *c)
{
	sFarbe = c->text();
	bool enableEdit = (sFarbe != "Black" && sFarbe != "White");
	bool enableDel  = (sFarbe != "Black" && sFarbe != "White") && (EditColors.count() > 1);
	EditF->setEnabled(enableEdit);
	DupF->setEnabled(true);
	DelF->setEnabled(enableDel);
	if(enableEdit)
		editFarbe();
}

void ColorManager::updateCList()
{
	ColorList::Iterator it;
	ScribusDoc* doc = EditColors.document();
	colorListBox->clear();
	for (it = EditColors.begin(); it != EditColors.end(); ++it)
	{
		if (it.key() == CommonStrings::None || it.key() == CommonStrings::NoneColor)
			continue;
		colorListBox->insertItem( new ColorFancyPixmapItem(it.data(), doc, it.key()) );
	}
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
