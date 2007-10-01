/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "colorm.h"

#include <QAbstractItemView>
#include <QAction>
#include <QByteArray>
#include <QDataStream>
#include <QDomDocument>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSignalMapper>
#include <QTextStream>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>

#include <cstdlib>

#include "cmykfw.h"
#include "colorlistbox.h"
#include "commonstrings.h"
#include "customfdialog.h"
#include "dcolor.h"
#include "fileloader.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "sccolorengine.h"
#include "scconfig.h"
#include "scpaths.h"
#include "scribusdoc.h"
#include "util.h"
#include "util_formats.h"
#include "util_icon.h"


ColorManager::ColorManager(QWidget* parent, ColorList doco, ScribusDoc* doc, QString docColSet, QStringList custColSet)
		: QDialog( parent ), EditColors(doc, true), UsedC(doc, true)
{
	setModal(true);
	setWindowTitle( tr( "Colors" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	m_Doc=doc;
	customColSet = custColSet;
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	setSizeGripEnabled(true);
	Layout2 = new QVBoxLayout( this );
	Layout2->setSpacing( 5 );
	Layout2->setMargin( 10 );

	layout5 = new QHBoxLayout;
	layout5->setSpacing( 5 );
	layout5->setMargin( 0 );
	layout3 = new QVBoxLayout;
	layout3->setSpacing( 5 );
	layout3->setMargin( 0 );
	colorListBox = new ColorListBox(this);
	colorListBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
	colorListBox->setMinimumSize( QSize( 164, 228 ) );
	layout5->addWidget( colorListBox );

	ColorsGroup = new QGroupBox( this );
	Layout1 = new QVBoxLayout( ColorsGroup );
	Layout1->setSpacing( 5 );
	Layout1->setMargin( 10 );
	Layout1->setAlignment( Qt::AlignTop );
	importColorsButton = new QPushButton( tr( "&Import" ), ColorsGroup );
	Layout1->addWidget( importColorsButton );
	newColorButton = new QPushButton( tr( "&New" ), ColorsGroup );
	Layout1->addWidget( newColorButton );
	editColorButton = new QPushButton( tr( "&Edit" ), ColorsGroup );
	editColorButton->setEnabled( false );
	editColorButton->setDefault( true );
	Layout1->addWidget( editColorButton );
	duplicateColorButton = new QPushButton( tr( "D&uplicate" ), ColorsGroup );
	duplicateColorButton->setEnabled( false );
	Layout1->addWidget( duplicateColorButton );
	deleteColorButton = new QPushButton( tr( "&Delete" ), ColorsGroup );
	deleteColorButton->setEnabled( false );
	Layout1->addWidget( deleteColorButton );
	if (m_Doc!=0)
	{
		deleteUnusedButton = new QPushButton( tr( "&Remove Unused" ), ColorsGroup );
		Layout1->addWidget( deleteUnusedButton );
	}
	layout3->addWidget( ColorsGroup );
	if (m_Doc==0)
	{
		ColsSetGroup = new QGroupBox( this );
		ColsSetGroup->setTitle( tr( "Color Sets" ) );
		ColsSetGroupLayout = new QVBoxLayout( ColsSetGroup );
		ColsSetGroupLayout->setSpacing( 5 );
		ColsSetGroupLayout->setMargin( 10 );
		ColsSetGroupLayout->setAlignment( Qt::AlignTop );
		textLabel1 = new QLabel( ColsSetGroup );
		textLabel1->setText( tr( "Current Color Set:" ) );
		ColsSetGroupLayout->addWidget( textLabel1 );
		QSignalMapper *signalMapper = new QSignalMapper(this);
		CSets = new QMenu();
		QAction *action = CSets->addAction("Scribus Small");
		connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
		signalMapper->setMapping(action, action->text());
		int setCount = 1;
		csm.findPaletteLocations();
		csm.findPalettes();
		QStringList allSets(csm.paletteNames());
		for ( QStringList::Iterator it = allSets.begin(); it != allSets.end(); ++it )
		{
			action = CSets->addAction((*it));
			connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
			signalMapper->setMapping(action, action->text());
			setCount++;
		}
		customSetStartIndex = setCount;
		if (custColSet.count() != 0)
		{
			QStringList realEx;
			realEx.clear();
			for (int m = 0; m < custColSet.count(); ++m)
			{
				QString Cpfad = QDir::convertSeparators(ScPaths::getApplicationDataDir() + custColSet[m]);
				QFileInfo cfi(Cpfad);
				if (cfi.exists())
				{
					action = CSets->addAction(custColSet[m]);
					signalMapper->setMapping(action, action->text());
					realEx.append(custColSet[m]);
				}
			}
			customColSet = realEx;
		}
		connect(signalMapper, SIGNAL(mapped(const QString &)), this, SLOT(loadDefaults(const QString &)));
		LoadColSet = new QToolButton( ColsSetGroup );
		LoadColSet->setMenu(CSets);
		LoadColSet->setPopupMode(QToolButton::MenuButtonPopup);
		LoadColSet->setText(docColSet);
		LoadColSet->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		ColsSetGroupLayout->addWidget( LoadColSet );
		SaveColSet = new QPushButton( tr( "&Save Color Set" ), ColsSetGroup );
		SaveColSet->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
		ColsSetGroupLayout->addWidget( SaveColSet );
		layout3->addWidget( ColsSetGroup );
	}
	saveButton = new QPushButton( CommonStrings::tr_OK, this );
	layout3->addWidget( saveButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	cancelButton->setDefault( true );
	layout3->addWidget( cancelButton );
	layout5->addLayout( layout3 );
	Layout2->addLayout( layout5 );
	replaceMap.clear();
	EditColors = doco;
	updateCList();
	// signals and slots connections
	if (m_Doc==0)
	{
		connect(SaveColSet, SIGNAL( clicked() ), this, SLOT( saveDefaults() ) );
		LoadColSet->setToolTip( "<qt>" + tr( "Choose a color set to load" ) + "</qt>");
		SaveColSet->setToolTip( "<qt>" + tr( "Save the current color set" ) + "</qt>");
	}
	else
	{
		connect(deleteUnusedButton, SIGNAL( clicked() ), this, SLOT( deleteUnusedColors() ) );
		deleteUnusedButton->setToolTip( "<qt>" + tr( "Remove unused colors from current document's color set" ) + "</qt>");
	}
	importColorsButton->setToolTip( "<qt>" + tr( "Import colors to the current set from an existing document" ) + "</qt>");
	newColorButton->setToolTip( "<qt>" + tr( "Create a new color within the current set" ) + "</qt>");
	editColorButton->setToolTip( "<qt>" + tr( "Edit the currently selected color" ) + "</qt>");
	duplicateColorButton->setToolTip( "<qt>" + tr( "Make a copy of the currently selected color" ) + "</qt>");
	deleteColorButton->setToolTip( "<qt>" + tr( "Delete the currently selected color" ) + "</qt>");
	saveButton->setToolTip( "<qt>" + tr( "Make the current colorset the default color set" ) + "</qt>");
	colorListBox->setToolTip( "<qt>" + tr( "If color management is enabled, a triangle warning indicator is a warning the the color maybe outside of the color gamut of the current printer profile selected.What this means is the color may not print exactly as indicated on screen. Spot colors are indicated by a red circle. Registration colors will have a registration mark next to the color. More hints about gamut warnings are in the online help under Color Management." ) + "</qt>");
	connect( saveButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );
	connect( newColorButton, SIGNAL( clicked() ), this, SLOT( newColor() ) );
	connect( editColorButton, SIGNAL( clicked() ), this, SLOT( editColor() ) );
	connect( duplicateColorButton, SIGNAL( clicked() ), this, SLOT( duplicateColor() ) );
	connect( deleteColorButton, SIGNAL( clicked() ), this, SLOT( deleteColor() ) );
	connect( importColorsButton, SIGNAL( clicked() ), this, SLOT( importColors() ) );
	connect( colorListBox, SIGNAL( itemClicked(QListWidgetItem*) ), this, SLOT( selColor(QListWidgetItem*) ) );
	connect( colorListBox, SIGNAL( itemActivated(QListWidgetItem*) ), this, SLOT( selEditColor(QListWidgetItem*) ) );
	resize(minimumSizeHint());
}

void ColorManager::saveDefaults()
{
	QString Cpfad = QDir::convertSeparators(ScPaths::getApplicationDataDir());
	QString Name = LoadColSet->text();
	Query* dia = new Query(this, "Name", 1, 0, tr("&Name:"), tr("Choose a Name"));
	if ((Name == "Scribus Basic") || (Name == "Scribus Small") || (Name == "X11 RGB-Set") || (Name == "OpenOffice.org-Set")
	        || (Name == "X11 Grey-Set") || (Name == "Gnome-Set") || (Name == "SVG-Set"))
		dia->setEditText("", false);
	else
		dia->setEditText(Name, false);
	if (dia->exec())
	{
		QString Fname = Cpfad+dia->getEditText();
		LoadColSet->setText(dia->getEditText());
		QFile fx(Fname);
		if (fx.open(QIODevice::WriteOnly))
		{
			QDomDocument docu("scribus");
			QString st="<SCRIBUSCOLORS></SCRIBUSCOLORS>";
			docu.setContent(st);
			QDomElement elem = docu.documentElement();
			elem.setAttribute("Name", dia->getEditText());
			ColorList::Iterator itc;
			for (itc = EditColors.begin(); itc != EditColors.end(); ++itc)
			{
				QDomElement co = docu.createElement("COLOR");
				co.setAttribute("NAME",itc.key());
				if (EditColors[itc.key()].getColorModel() == colorModelRGB)
					co.setAttribute("RGB",EditColors[itc.key()].nameRGB());
				else
					co.setAttribute("CMYK",EditColors[itc.key()].nameCMYK());
				co.setAttribute("Spot",static_cast<int>(EditColors[itc.key()].isSpotColor()));
				co.setAttribute("Register",static_cast<int>(EditColors[itc.key()].isRegistrationColor()));
				elem.appendChild(co);
			}
			static const char* xmlpi = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			QByteArray cs = docu.toString().toUtf8();
			QDataStream s(&fx);
			s.writeRawData(xmlpi, strlen(xmlpi));
			s.writeRawData(cs, cs.length());
/*			CMYKColor cmyk;
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
			} */
			fx.close();
			if (dia->getEditText() != Name)
			{
				customColSet.append(dia->getEditText());
				QAction* action = CSets->addAction(dia->getEditText());
				connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
				signalMapper->setMapping(action, action->text());
			}
		}
	}
	delete dia;
}

void ColorManager::loadDefaults(const QString &txt)
{
	int c = 0;
	QList<QAction*> cAct = CSets->actions();
	for (int a = 0; a < cAct.count(); a++)
	{
		if (txt == cAct.at(a)->text())
		{
			c = a;
			break;
		}
	}
	bool cus = false;
	LoadColSet->setText(txt);
	EditColors.clear();
	QString Cpfad = QDir::convertSeparators(ScPaths::getApplicationDataDir()+txt);
	QString pfadC = ScPaths::instance().libDir()+"swatches/";
	QString pfadC2 = pfadC + "Scribus_Basic.txt";
	if (txt == "Scribus Small")
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
	else
	{
		if ( c < customSetStartIndex)
		{
			QString listText = txt;
			if (listText == "Scribus OpenOffice")
				cus=true;
			pfadC2 = csm.paletteFileFromName(listText);
		}
		else
		{
			pfadC2 = Cpfad;
			cus = true;
		}
	}
	if (txt != "Scribus Small")
	{
		QFile fiC(pfadC2);
		if (fiC.open(QIODevice::ReadOnly))
		{
			QString ColorEn, Cname;
			int Rval, Gval, Bval, Kval;
			QTextStream tsC(&fiC);
			ColorEn = tsC.readLine();
			if (ColorEn.startsWith("<?xml version="))
			{
				QByteArray docBytes("");
				loadRawText(pfadC2, docBytes);
				QString docText("");
				docText = QString::fromUtf8(docBytes);
				QDomDocument docu("scridoc");
				docu.setContent(docText);
				ScColor lf = ScColor();
				QDomElement elem = docu.documentElement();
				QDomNode PAGE = elem.firstChild();
				while(!PAGE.isNull())
				{
					QDomElement pg = PAGE.toElement();
					if(pg.tagName()=="COLOR" && pg.attribute("NAME")!=CommonStrings::None)
					{
						if (pg.hasAttribute("CMYK"))
							lf.setNamedColor(pg.attribute("CMYK"));
						else
							lf.fromQColor(QColor(pg.attribute("RGB")));
						if (pg.hasAttribute("Spot"))
							lf.setSpotColor(static_cast<bool>(pg.attribute("Spot").toInt()));
						else
							lf.setSpotColor(false);
						if (pg.hasAttribute("Register"))
							lf.setRegistrationColor(static_cast<bool>(pg.attribute("Register").toInt()));
						else
							lf.setRegistrationColor(false);
						EditColors.insert(pg.attribute("NAME"), lf);
					}
					PAGE=PAGE.nextSibling();
				}
			}
			else
			{
				while (!tsC.atEnd())
				{
					ScColor tmp;
					ColorEn = tsC.readLine().trimmed();
					if (ColorEn.length()>0 && ColorEn[0]==QChar('#'))
						continue;
					
					if (ColorEn[0].isNumber()) {
						QTextStream CoE(&ColorEn, QIODevice::ReadOnly);
						CoE >> Rval;
						CoE >> Gval;
						CoE >> Bval;
						if (cus)
						{
							CoE >> Kval;
							Cname = CoE.readAll().trimmed();
							tmp.setColor(Rval, Gval, Bval, Kval);
						}
						else
						{
							Cname = CoE.readAll().trimmed();
							tmp.setColorRGB(Rval, Gval, Bval);
						}
					}
					else
					{
						QStringList fields = ColorEn.split(QChar(9), QString::SkipEmptyParts);
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
							Cname=QString("#%1%2%3").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).toUpper();
						else
							Cname=QString("#%1%2%3%4").arg(Rval,2,16).arg(Gval,2,16).arg(Bval,2,16).arg(Kval,2,16).toUpper();
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

void ColorManager::importColors()
{
	QString fileName;
	PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
	QString wdir = dirs->get("colors", ".");
	
	
	QString docexts("*.sla *.sla.gz *.scd *.scd.gz");
	CustomFDialog dia(this, wdir, tr("Import"), tr("Documents (%1);;Other Files (%2);;All Files (*)").arg(docexts).arg(FormatsManager::instance()->extensionListForFormat(FormatsManager::EPS|FormatsManager::PS|FormatsManager::AI, 0)));
	
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
	if (!fileName.isEmpty())
	{
		int oldCount = EditColors.count();
		dirs->set("colors", fileName.left(fileName.lastIndexOf("/")));
		QFileInfo fi = QFileInfo(fileName);
		QString ext = fi.suffix().toLower();
		if (extensionIndicatesEPSorPS(ext) || (ext == "ai"))
		{
			QString tmp, tmp2, FarNam;
			double c, m, y, k;
			ScColor cc;
			QFile f(fileName);
			if (f.open(QIODevice::ReadOnly))
			{
				QDataStream ts(&f);
				while (!ts.atEnd())
				{
					tmp = readLinefromDataStream(ts);
					if ((tmp.startsWith("%%CMYKCustomColor")) || (tmp.startsWith("%%CMYKProcessColor")))
					{
						if (tmp.startsWith("%%CMYKCustomColor"))
							tmp = tmp.remove(0,18);
						else if (tmp.startsWith("%%CMYKProcessColor"))
							tmp = tmp.remove(0,19);
						QTextStream ts2(&tmp, QIODevice::ReadOnly);
						ts2 >> c >> m >> y >> k;
						FarNam = ts2.readAll();
						FarNam = FarNam.trimmed();
						FarNam = FarNam.remove(0,1);
						FarNam = FarNam.remove(FarNam.length()-1,1);
						FarNam = FarNam.simplified();
						cc = ScColor(static_cast<int>(255 * c), static_cast<int>(255 * m), static_cast<int>(255 * y), static_cast<int>(255 * k));
						cc.setSpotColor(true);
						if (!EditColors.contains(FarNam))
							EditColors.insert(FarNam, cc);
						while (!ts.atEnd())
						{
							quint64 oldPos = ts.device()->pos();
							tmp = readLinefromDataStream(ts);
							if (!tmp.startsWith("%%+"))
							{
								ts.device()->seek(oldPos);
								break;
							}
							tmp = tmp.remove(0,3);
							QTextStream ts2(&tmp, QIODevice::ReadOnly);
							ts2 >> c >> m >> y >> k;
							FarNam = ts2.readAll();
							FarNam = FarNam.trimmed();
							FarNam = FarNam.remove(0,1);
							FarNam = FarNam.remove(FarNam.length()-1,1);
							FarNam = FarNam.simplified();
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
								tmp = readLinefromDataStream(ts);
								if ((tmp.endsWith("Xa") || tmp.endsWith(" k")) && (tmp.length() > 4))
								{
									QTextStream ts2(&tmp, QIODevice::ReadOnly);
									ts2 >> c >> m >> y >> k;
									tmp = readLinefromDataStream(ts);
									if (tmp.endsWith("Pc"))
									{
										tmp = tmp.trimmed();
										tmp = tmp.remove(0,1);
										int en = tmp.indexOf(")");
										FarNam = tmp.mid(0, en);
										FarNam = FarNam.simplified();
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
						EditColors.insert(it.key(), it.value());
				}
				updateCList();
			}
		}
		if (EditColors.count() == oldCount)
			QMessageBox::information(this, tr("Information"), "<qt>" + tr("The file %1 does not contain colors which can be imported.\nIf the file was a PostScript-based, try to import it with File -&gt; Import. \nNot all files have DSC conformant comments where the color descriptions are located.\n This prevents importing colors from some files.\nSee the Edit Colors section of the documentation for more details.").arg(fileName) + "</qt>", 1, 0, 0);
		
	}
}

void ColorManager::deleteUnusedColors()
{
	m_Doc->getUsedColors(UsedC);
	EditColors = UsedC;
	EditColors.ensureBlackAndWhite();
	updateCList();
	updateButtons();
}

void ColorManager::duplicateColor()
{
	QString nam = tr("Copy of %1").arg(sColor);
	EditColors.insert(nam, EditColors[sColor]);
	sColor = nam;
	editColor();
	updateCList();
	updateButtons();
}

void ColorManager::newColor()
{
	ScColor tmpColor = ScColor(0, 0, 0, 0);
	CMYKChoose* dia = new CMYKChoose(this, m_Doc, tmpColor, tr("New Color"), &EditColors, customColSet, true);
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
				it.value().setRegistrationColor(false);
			if (it==itnew)
				newItemIndex=colCount;
			++colCount;
		}
		EditColors[dia->Farbname->text()].setRegistrationColor(dia->Regist->isChecked());
		updateCList();
	}
	delete dia;
	colorListBox->item(newItemIndex)->setSelected(true);
	colorListBox->setCurrentRow(newItemIndex);
	updateButtons();
//	colorListBox->setTopItem(newItemIndex);
}

void ColorManager::editColor()
{
	int selectedIndex = colorListBox->row(colorListBox->currentItem());
//	int topIndex=colorListBox->topItem();
	ScColor tmpColor = EditColors[sColor];
	CMYKChoose* dia = new CMYKChoose(this, m_Doc, tmpColor, sColor, &EditColors, customColSet, false);
	if (dia->exec())
	{
		dia->Farbe.setSpotColor(dia->Separations->isChecked());
		EditColors[dia->Farbname->text()] = dia->Farbe;
		if (dia->Regist->isChecked())
		{
			ColorList::Iterator it;
			for (it = EditColors.begin(); it != EditColors.end(); ++it)
				it.value().setRegistrationColor(false);
		}
		EditColors[dia->Farbname->text()].setRegistrationColor(dia->Regist->isChecked());
		if (sColor != dia->Farbname->text())
		{
			replaceMap.insert(sColor, dia->Farbname->text());
			EditColors.remove(sColor);
		}
		updateCList();
	}
	delete dia;
	colorListBox->item(selectedIndex)->setSelected(true);
	colorListBox->setCurrentRow(selectedIndex);
	updateButtons();
//	colorListBox->setTopItem(topIndex);
}

void ColorManager::deleteColor()
{
	int selectedIndex = colorListBox->row(colorListBox->currentItem());
//	int topIndex=colorListBox->topItem();
	DelColor *dia = new DelColor(this, EditColors, sColor, (m_Doc!=0));
	if (dia->exec())
	{
		QString replacementColor(dia->getReplacementColor());
		if (replacementColor == CommonStrings::tr_NoneColor)
			replacementColor = CommonStrings::None;
		if (replaceMap.values().contains(sColor))
		{
			QMap<QString,QString>::Iterator it;
			for (it = replaceMap.begin(); it != replaceMap.end(); ++it)
			{
				if (it.value() == sColor)
					it.value() = replacementColor;
			}
		}
		replaceMap.insert(sColor, replacementColor);
		EditColors.remove(sColor);
		updateCList();
	}
	delete dia;
	int listBoxCount = colorListBox->count();
	if (listBoxCount > selectedIndex)
	{
		colorListBox->item(selectedIndex)->setSelected(true);
		colorListBox->setCurrentRow(selectedIndex);
	}
	updateButtons();
//	if (listBoxCount>topIndex)
//		colorListBox->setTopItem(topIndex);
}

void ColorManager::selColor(QListWidgetItem *c)
{
	sColor = c->text();
	bool enableEdit = (sColor != "Black" && sColor != "White");
	bool enableDel  = (sColor != "Black" && sColor != "White") && (EditColors.count() > 1);
	editColorButton->setEnabled(enableEdit);
	duplicateColorButton->setEnabled(true);
	deleteColorButton->setEnabled(enableDel);
}

void ColorManager::selEditColor(QListWidgetItem *c)
{
	sColor = c->text();
	bool enableEdit = (sColor != "Black" && sColor != "White");
	bool enableDel  = (sColor != "Black" && sColor != "White") && (EditColors.count() > 1);
	editColorButton->setEnabled(enableEdit);
	duplicateColorButton->setEnabled(true);
	deleteColorButton->setEnabled(enableDel);
	if(enableEdit)
		editColor();
}

void ColorManager::updateButtons()
{
	ColorPixmapItem* currItem = dynamic_cast<ColorPixmapItem*>(colorListBox->currentItem());
	if (currItem)
	{
		QString curCol = currItem->text();
		bool enableDel  = (curCol != "Black" && curCol != "White") && (EditColors.count() > 1);
		bool enableEdit = (curCol != "Black" && curCol != "White");
		duplicateColorButton->setEnabled(true);
		deleteColorButton->setEnabled(enableDel);
		editColorButton->setEnabled(enableEdit);
	}
	else
	{
		duplicateColorButton->setEnabled(false);
		editColorButton->setEnabled(false);
		deleteColorButton->setEnabled(false);
	}
}

void ColorManager::updateCList()
{
	colorListBox->updateBox(EditColors, ColorListBox::fancyPixmap);
	if(colorListBox->currentItem())
		colorListBox->currentItem()->setSelected(false);
	updateButtons();
}

QString ColorManager::getColorSetName()
{
	return LoadColSet->text();
}
