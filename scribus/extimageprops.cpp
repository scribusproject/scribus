/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "extimageprops.h"
//#include "extimageprops.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <q3table.h>
#include <q3listbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3whatsthis.h>
#include <q3header.h>
#include <qpainter.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3ValueList>
#include <QPixmap>
#include <Q3VBoxLayout>

#include "pageitem.h"
#include "sccombobox.h"
#include "scpainter.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "util.h"

ExtImageProps::ExtImageProps( QWidget* parent, ImageInfoRecord *info, PageItem *item, ScribusView *view )  : QDialog( parent, "ExtImageProps", true, 0 )
{
	setIcon(loadIcon("AppIcon.png"));
	setCaption( tr( "Extended Image Properties" ) );
	ExtImagePropsLayout = new Q3VBoxLayout( this, 1, 2, "ExtImagePropsLayout");
	viewWidget = view;
	currentItem = item;
	currentLayer = 0;
	blendModes.clear();
	blendModes.insert("norm", tr("Normal"));
	blendModes.insert("dark", tr("Darken"));
	blendModes.insert("lite", tr("Lighten"));
	blendModes.insert("hue ", tr("Hue"));
	blendModes.insert("sat ", tr("Saturation"));
	blendModes.insert("colr", tr("Color"));
	blendModes.insert("lum ", tr("Luminosity"));
	blendModes.insert("mul ", tr("Multiply"));
	blendModes.insert("scrn", tr("Screen"));
	blendModes.insert("diss", tr("Dissolve"));
	blendModes.insert("over", tr("Overlay"));
	blendModes.insert("hLit", tr("Hard Light"));
	blendModes.insert("sLit", tr("Soft Light"));
	blendModes.insert("diff", tr("Difference"));
	blendModes.insert("smud", tr("Exclusion"));
	blendModes.insert("div ", tr("Color Dodge"));
	blendModes.insert("idiv", tr("Color Burn"));
	blendModesRev.clear();
	blendModesRev.insert( tr("Normal"), "norm");
	blendModesRev.insert( tr("Darken"), "dark");
	blendModesRev.insert( tr("Lighten"), "lite");
	blendModesRev.insert( tr("Hue"), "hue ");
	blendModesRev.insert( tr("Saturation"), "sat ");
	blendModesRev.insert( tr("Color"), "colr");
	blendModesRev.insert( tr("Luminosity"), "lum ");
	blendModesRev.insert( tr("Multiply"), "mul ");
	blendModesRev.insert( tr("Screen"), "scrn");
	blendModesRev.insert( tr("Dissolve"), "diss");
	blendModesRev.insert( tr("Overlay"), "over");
	blendModesRev.insert( tr("Hard Light"), "hLit");
	blendModesRev.insert( tr("Soft Light"), "sLit");
	blendModesRev.insert( tr("Difference"), "diff");
	blendModesRev.insert( tr("Exclusion"), "smud");
	blendModesRev.insert( tr("Color Dodge"), "div ");
	blendModesRev.insert( tr("Color Burn"), "idiv");
	propsTab = new QTabWidget( this, "propsTab" );

	if (info->layerInfo.count() != 0)
	{
		tab = new QWidget( propsTab, "tab" );
		tabLayout = new Q3VBoxLayout( tab, 4, 4, "tabLayout");
		layout1 = new Q3HBoxLayout( 0, 0, 4, "layout1");
		textLabel1 = new QLabel( tab, "textLabel1" );
		textLabel1->setText( tr( "Blend Mode:" ) );
		layout1->addWidget( textLabel1 );
		blendMode = new ScComboBox( false, tab, "blendMode" );
		blendMode->clear();
		blendMode->insertItem( tr("Normal"));
		blendMode->insertItem( tr("Darken"));
		blendMode->insertItem( tr("Lighten"));
		blendMode->insertItem( tr("Hue"));
		blendMode->insertItem( tr("Saturation"));
		blendMode->insertItem( tr("Color"));
		blendMode->insertItem( tr("Luminosity"));
		blendMode->insertItem( tr("Multiply"));
		blendMode->insertItem( tr("Screen"));
		blendMode->insertItem( tr("Dissolve"));
		blendMode->insertItem( tr("Overlay"));
		blendMode->insertItem( tr("Hard Light"));
		blendMode->insertItem( tr("Soft Light"));
		blendMode->insertItem( tr("Difference"));
		blendMode->insertItem( tr("Exclusion"));
		blendMode->insertItem( tr("Color Dodge"));
		blendMode->insertItem( tr("Color Burn"));
		layout1->addWidget( blendMode );
		textLabel2 = new QLabel( tab, "textLabel2" );
		textLabel2->setText( tr( "Opacity:" ) );
		layout1->addWidget( textLabel2 );
		opacitySpinBox = new QSpinBox( tab, "opacitySpinBox" );
		opacitySpinBox->setMinValue(0);
		opacitySpinBox->setMaxValue(100);
		opacitySpinBox->setLineStep(10);
		opacitySpinBox->setSuffix( tr(" %"));
		layout1->addWidget( opacitySpinBox );
		tabLayout->addLayout( layout1 );
		layerTable = new Q3Table( tab, "layerTable" );
		layerTable->setNumRows( 0 );
		layerTable->setNumCols( 3 );
		Q3Header *header = layerTable->horizontalHeader();
		header->setLabel(0, loadIcon("16/show-object.png"), "");
		header->setLabel(1, "");
		header->setLabel(2, tr("Name"));
		layerTable->setColumnReadOnly(0, true);
		layerTable->setColumnReadOnly(1, true);
		layerTable->setColumnReadOnly(2, true);
		if (info->layerInfo.count() == 1)
		{
			layerTable->setColumnWidth(1, 40);
			layerTable->setColumnWidth(0, 24);
		}
		layerTable->setColumnStretchable(2, true);
		layerTable->setRowMovingEnabled(false);
		layerTable->setSorting(false);
		layerTable->setSelectionMode( Q3Table::SingleRow );
		layerTable->setFocusStyle( Q3Table::FollowStyle );
		Q3Header *Header = layerTable->verticalHeader();
		Header->setMovingEnabled(false);
		Header->setResizeEnabled(false);
		layerTable->setLeftMargin(0);
		Header->hide();
		FlagsSicht.clear();
		if (info->layerInfo.count() != 0)
		{
			if ((info->isRequest) && (info->RequestProps.contains(0)))
			{
				opacitySpinBox->setValue(qRound(info->RequestProps[0].opacity / 255.0 * 100));
				blendMode->setCurrentText(blendModes[info->RequestProps[0].blend]);
			}
			else
			{
				opacitySpinBox->setValue(qRound(info->layerInfo[0].opacity / 255.0 * 100));
				blendMode->setCurrentText(blendModes[info->layerInfo[0].blend]);
			}
			opacitySpinBox->setEnabled(true);
			blendMode->setEnabled(true);
			QString tmp;
			Q3ValueList<PSDLayer>::iterator it2;
			layerTable->setNumRows(info->layerInfo.count());
			uint counter = 0;
			for (it2 = info->layerInfo.begin(); it2 != info->layerInfo.end(); ++it2)
			{
				QCheckBox *cp = new QCheckBox(this, (*it2).layerName);
				cp->setPaletteBackgroundColor(Qt::white);
				if ((info->isRequest) && (info->RequestProps.contains(counter)))
					cp->setChecked(info->RequestProps[counter].visible);
				else
					cp->setChecked(!((*it2).flags & 2));
				QPixmap pm;
				pm.convertFromImage((*it2).thumb);
				cp->setPixmap(pm);
				FlagsSicht.append(cp);
				connect(cp, SIGNAL(clicked()), this, SLOT(changedLayer()));
				layerTable->setCellWidget(info->layerInfo.count()-counter-1, 0, cp);
				if (!(*it2).thumb_mask.isNull())
				{
					QCheckBox *cp2 = new QCheckBox(this, (*it2).layerName);
					cp2->setPaletteBackgroundColor(Qt::white);
					if ((info->isRequest) && (info->RequestProps.contains(counter)))
						cp2->setChecked(info->RequestProps[counter].useMask);
					else
						cp2->setChecked(true);
					QPixmap pm2;
					pm2.convertFromImage((*it2).thumb_mask);
					cp2->setPixmap(pm2);
					connect(cp2, SIGNAL(clicked()), this, SLOT(changedLayer()));
					layerTable->setCellWidget(info->layerInfo.count()-counter-1, 1, cp2);
					FlagsMask.append(cp2);
				}
				else
					FlagsMask.append(0);
				layerTable->setText(info->layerInfo.count()-counter-1, 2, (*it2).layerName);
				Header->setLabel(info->layerInfo.count()-counter-1, tmp.setNum(counter+1));
				layerTable->setRowHeight(info->layerInfo.count()-counter-1, 40);
				counter++;
			}
			layerTable->adjustColumn(0);
			layerTable->adjustColumn(1);
		}
		tabLayout->addWidget( layerTable );
		blendMode->setCurrentItem(0);
		propsTab->insertTab( tab,  tr( "Layers" ) );
	}
	tab_2 = new QWidget( propsTab, "tab_2" );
	tabLayout_2 = new Q3VBoxLayout( tab_2, 4, 4, "tabLayout_2");
	pathList = new Q3ListBox( tab_2, "pathList" );
	pathList->clear();
	QMap<QString, FPointArray>::Iterator it;
	if (info->PDSpathData.count() != 0)
	{
		for (it = info->PDSpathData.begin(); it != info->PDSpathData.end(); ++it)
		{
			QPixmap pixm(40, 40);
			ScPainter *p = new ScPainter(&pixm, 40, 40);
			p->clear();
			p->translate(3.0, 3.0);
			if (it.key() == info->clipPath)
			{
				pixm.fill(Qt::green);
				p->clear(Qt::green);
			}
			else
				pixm.fill(Qt::white);
			FPointArray Path;
			Path.resize(0);
			Path = info->PDSpathData[it.key()].copy();
			FPoint min = getMinClipF(&Path);
			Path.translate(-min.x(), -min.y());
			FPoint max = Path.WidthHeight();
			QMatrix mm;
			mm.scale(34.0 / qMax(max.x(), max.y()), 34.0 / qMax(max.x(), max.y()));
			Path.map(mm);
			p->setupPolygon(&Path);
			p->setPen(Qt::black);
			p->setBrush(Qt::white);
			p->setFillMode(0);
			p->setLineWidth(1.0);
			p->strokePath();
			p->end();
			delete p;
			pathList->insertItem(pixm, it.key());
			if (it.key() == info->usedPath)
				pathList->setSelected(pathList->count()-1, true);
		}
	}
	tabLayout_2->addWidget( pathList );
	resetPath = new QPushButton( tr("Don't use any Path"), tab_2, "reset");
	tabLayout_2->addWidget( resetPath );
	propsTab->insertTab( tab_2, tr( "Paths" ) );
	ExtImagePropsLayout->addWidget( propsTab );
	resize(330, 320);

	connect(pathList, SIGNAL( highlighted(Q3ListBoxItem*) ), this, SLOT( selPath(Q3ListBoxItem*) ) );
	connect(resetPath, SIGNAL(clicked()), this, SLOT(noPath()));
	if (info->layerInfo.count() != 0)
	{
		connect(layerTable, SIGNAL(currentChanged(int, int)), this, SLOT(selLayer(int)));
		connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedLayer()));
		connect(blendMode, SIGNAL(activated(int)), this, SLOT(changedLayer()));
	}
}

void ExtImageProps::changedLayer()
{
	struct ImageLoadRequest loadingInfo;
	currentItem->pixm.imgInfo.isRequest = true;
	for (int r = 0; r < layerTable->numRows(); ++r)
	{
		if (currentLayer == layerTable->numRows() - r - 1)
		{
			loadingInfo.blend = blendModesRev[blendMode->currentText()];
			loadingInfo.opacity = qRound(opacitySpinBox->value() / 100.0 * 255);
		}
/*		else if (currentItem->pixm.imgInfo.RequestProps.contains(layerTable->numRows() - r - 1))
		{
			loadingInfo.blend = currentItem->pixm.imgInfo.RequestProps[layerTable->numRows() - r - 1].blend;
			loadingInfo.opacity = currentItem->pixm.imgInfo.RequestProps[layerTable->numRows() - r - 1].opacity;
		} */
		else
		{
			loadingInfo.blend = currentItem->pixm.imgInfo.layerInfo[layerTable->numRows() - r - 1].blend;
			loadingInfo.opacity = currentItem->pixm.imgInfo.layerInfo[layerTable->numRows() - r - 1].opacity;
		}
		loadingInfo.visible = FlagsSicht.at(layerTable->numRows() - r - 1)->isChecked();
		if (FlagsMask.at(layerTable->numRows() - r - 1))
			loadingInfo.useMask = FlagsMask.at(layerTable->numRows() - r - 1)->isChecked();
		else
			loadingInfo.useMask = true;
		currentItem->pixm.imgInfo.RequestProps.insert(layerTable->numRows() - r - 1, loadingInfo);
	}
	viewWidget->Doc->LoadPict(currentItem->Pfile, currentItem->ItemNr, true);
	viewWidget->updateContents();
}

void ExtImageProps::selLayer(int layer)
{
	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedLayer()));
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changedLayer()));
	if ((currentItem->pixm.imgInfo.isRequest) && (currentItem->pixm.imgInfo.RequestProps.contains(layerTable->numRows() - layer - 1)))
	{
		opacitySpinBox->setValue(qRound(currentItem->pixm.imgInfo.RequestProps[layerTable->numRows() - layer - 1].opacity / 255.0 * 100));
		blendMode->setCurrentText(blendModes[currentItem->pixm.imgInfo.RequestProps[layerTable->numRows() - layer - 1].blend]);
	}
	else
	{
		opacitySpinBox->setValue(qRound(currentItem->pixm.imgInfo.layerInfo[layerTable->numRows() - layer - 1].opacity / 255.0 * 100));
		blendMode->setCurrentText(blendModes[currentItem->pixm.imgInfo.layerInfo[layerTable->numRows() - layer - 1].blend]);
	}
	opacitySpinBox->setEnabled(true);
	blendMode->setEnabled(true);
	currentLayer = layerTable->numRows() - layer - 1;
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedLayer()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changedLayer()));
}

void ExtImageProps::noPath()
{
	disconnect(pathList, SIGNAL( highlighted(Q3ListBoxItem*) ), this, SLOT( selPath(Q3ListBoxItem*) ) );
	currentItem->imageClip.resize(0);
	currentItem->pixm.imgInfo.usedPath = "";
	pathList->clearSelection();
	viewWidget->updateContents();
	connect(pathList, SIGNAL( highlighted(Q3ListBoxItem*) ), this, SLOT( selPath(Q3ListBoxItem*) ) );
}

void ExtImageProps::selPath(Q3ListBoxItem *c)
{
	currentItem->imageClip = currentItem->pixm.imgInfo.PDSpathData[c->text()].copy();
	currentItem->pixm.imgInfo.usedPath = c->text();
	QMatrix cl;
	cl.translate(currentItem->imageXOffset()*currentItem->imageXScale(), currentItem->imageYOffset()*currentItem->imageYScale());
	cl.scale(currentItem->imageXScale(), currentItem->imageYScale());
	currentItem->imageClip.map(cl);
	viewWidget->updateContents();
}

