#include "extimageprops.h"
#include "extimageprops.moc"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qtable.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qheader.h>
#include "scpainter.h"
extern QPixmap loadIcon(QString nam);
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);
extern FPoint getMinClipF(FPointArray* Clip);

ExtImageProps::ExtImageProps( QWidget* parent, ScImage::ImageInfoRecord *info, PageItem *item, ScribusView *view )  : QDialog( parent, "ExtImageProps", true, 0 )
{
	setIcon(loadIcon("AppIcon.png"));
	setCaption( tr( "Extended Image Properties" ) );
	ExtImagePropsLayout = new QVBoxLayout( this, 10, 5, "ExtImagePropsLayout");
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
	blendModesRev.insert( tr("Exlusion"), "smud");
	blendModesRev.insert( tr("Color Dodge"), "div ");
	blendModesRev.insert( tr("Color Burn"), "idiv");
	propsTab = new QTabWidget( this, "propsTab" );

	tab = new QWidget( propsTab, "tab" );
	tabLayout = new QVBoxLayout( tab, 10, 5, "tabLayout");
	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");
	textLabel1 = new QLabel( tab, "textLabel1" );
	textLabel1->setText( tr( "Blend Mode:" ) );
	layout1->addWidget( textLabel1 );
	blendMode = new QComboBox( true, tab, "blendMode" );
	blendMode->setEditable(false);
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
	blendMode->insertItem( tr("Exlusion"));
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
	layerTable = new QTable( tab, "layerTable" );
	layerTable->setNumRows( 0 );
	layerTable->setNumCols( 3 );
	QHeader *header = layerTable->horizontalHeader();
	header->setLabel(0, loadIcon("Layervisible.xpm"), "");
	header->setLabel(1, "");
	header->setLabel(2, tr("Name"));
	layerTable->setColumnReadOnly(0, true);
	layerTable->setColumnReadOnly(1, true);
	layerTable->setColumnReadOnly(2, true);
	layerTable->setColumnWidth(0, 24);
	layerTable->setColumnWidth(1, 40);
	layerTable->setColumnStretchable(2, true);
	layerTable->setRowMovingEnabled(false);
	layerTable->setSorting(false);
	layerTable->setSelectionMode( QTable::SingleRow );
	layerTable->setFocusStyle( QTable::FollowStyle );
	QHeader *Header = layerTable->verticalHeader();
	Header->setMovingEnabled(false);
	Header->setResizeEnabled(false);
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
		QValueList<ScImage::PSDLayer>::iterator it2;
		layerTable->setNumRows(info->layerInfo.count());
		uint counter = 0;
		for (it2 = info->layerInfo.begin(); it2 != info->layerInfo.end(); ++it2)
		{
			QCheckBox *cp = new QCheckBox(this, (*it2).layerName);
			if ((info->isRequest) && (info->RequestProps.contains(counter)))
				cp->setChecked(info->RequestProps[counter].visible);
			else
				cp->setChecked(!((*it2).flags & 2));
			FlagsSicht.append(cp);
			connect(cp, SIGNAL(clicked()), this, SLOT(changedLayer()));
			layerTable->setCellWidget(info->layerInfo.count()-counter-1, 0, cp);
			QPixmap pm;
			pm.convertFromImage((*it2).thumb);
			layerTable->setPixmap(info->layerInfo.count()-counter-1, 1, pm);
			layerTable->setText(info->layerInfo.count()-counter-1, 2, (*it2).layerName);
			Header->setLabel(info->layerInfo.count()-counter-1, tmp.setNum(counter+1));
			layerTable->setRowHeight(info->layerInfo.count()-counter-1, 40);
			counter++;
		}
	}
	else
	{
		opacitySpinBox->setValue(100);
		opacitySpinBox->setEnabled(false);
		blendMode->setCurrentText( tr("Normal"));
		blendMode->setEnabled(false);
		layerTable->setNumRows(1);
		QPixmap pm;
		QImage imt;
		double sx = item->pixm.width() / 40.0;
		double sy = item->pixm.height() / 40.0;
		imt = sy < sx ?  item->pixm.smoothScale(qRound(item->pixm.width() / sx), qRound(item->pixm.height() / sx)) :
								item->pixm.smoothScale(qRound(item->pixm.width() / sy), qRound(item->pixm.height() / sy));
		pm.convertFromImage(imt);
		layerTable->setPixmap(0, 1, pm);
		layerTable->setText(0, 2, tr("Background"));
		QCheckBox *cp = new QCheckBox(this, "");
		cp->setChecked(true);
		cp->setEnabled(false);
		layerTable->setCellWidget(0, 0, cp);
		layerTable->setRowHeight(0, 40);
		FlagsSicht.append(cp);
	}
	tabLayout->addWidget( layerTable );
	blendMode->setCurrentItem(0);
	propsTab->insertTab( tab,  tr( "Layers" ) );

	tab_2 = new QWidget( propsTab, "tab_2" );
	tabLayout_2 = new QVBoxLayout( tab_2, 10, 5, "tabLayout_2");
	pathList = new QListBox( tab_2, "pathList" );
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
				pixm.fill(green);
				p->clear(green);
			}
			else
				pixm.fill(white);
			FPointArray Path;
			Path.resize(0);
			Path = info->PDSpathData[it.key()].copy();
			FPoint min = getMinClipF(&Path);
			Path.translate(-min.x(), -min.y());
			FPoint max = Path.WidthHeight();
			QWMatrix mm;
			mm.scale(34.0 / QMAX(max.x(), max.y()), 34.0 / QMAX(max.x(), max.y()));
			Path.map(mm);
			p->setupPolygon(&Path);
			p->setPen(black);
			p->setBrush(white);
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
	clearWState( WState_Polished );
	connect(pathList, SIGNAL( highlighted(QListBoxItem*) ), this, SLOT( selPath(QListBoxItem*) ) );
	connect(layerTable, SIGNAL(currentChanged(int, int)), this, SLOT(selLayer(int)));
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(changedLayer()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changedLayer()));
	connect(resetPath, SIGNAL(clicked()), this, SLOT(noPath()));
	
}

void ExtImageProps::changedLayer()
{
	struct ScImage::LoadRequest loadingInfo;
	currentItem->pixm.imgInfo.isRequest = true;
	for (int r = 0; r < layerTable->numRows(); ++r)
	{
		if (currentLayer == layerTable->numRows() - r - 1)
		{
			loadingInfo.blend = blendModesRev[blendMode->currentText()];
			loadingInfo.opacity = qRound(opacitySpinBox->value() / 100.0 * 255);
		}
		else if (currentItem->pixm.imgInfo.RequestProps.contains(layerTable->numRows() - r - 1))
		{
			loadingInfo.blend = currentItem->pixm.imgInfo.RequestProps[layerTable->numRows() - r - 1].blend;
			loadingInfo.opacity = currentItem->pixm.imgInfo.RequestProps[layerTable->numRows() - r - 1].opacity;
		}
		else
		{
			loadingInfo.blend = currentItem->pixm.imgInfo.layerInfo[layerTable->numRows() - r - 1].blend;
			loadingInfo.opacity = currentItem->pixm.imgInfo.layerInfo[layerTable->numRows() - r - 1].opacity;
		}
		loadingInfo.visible = FlagsSicht.at(layerTable->numRows() - r - 1)->isChecked();
		currentItem->pixm.imgInfo.RequestProps.insert(layerTable->numRows() - r - 1, loadingInfo);
	}
	viewWidget->LoadPict(currentItem->Pfile, currentItem->ItemNr, true);
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
	disconnect(pathList, SIGNAL( highlighted(QListBoxItem*) ), this, SLOT( selPath(QListBoxItem*) ) );
	currentItem->imageClip.resize(0);
	currentItem->pixm.imgInfo.usedPath = "";
	pathList->clearSelection();
	viewWidget->updateContents();
	connect(pathList, SIGNAL( highlighted(QListBoxItem*) ), this, SLOT( selPath(QListBoxItem*) ) );
}

void ExtImageProps::selPath(QListBoxItem *c)
{
	currentItem->imageClip = currentItem->pixm.imgInfo.PDSpathData[c->text()].copy();
	currentItem->pixm.imgInfo.usedPath = c->text();
	QWMatrix cl;
	cl.translate(currentItem->LocalX*currentItem->LocalScX, currentItem->LocalY*currentItem->LocalScY);
	cl.scale(currentItem->LocalScX, currentItem->LocalScY);
	currentItem->imageClip.map(cl);
	viewWidget->updateContents();
}

