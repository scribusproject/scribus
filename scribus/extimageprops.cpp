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
extern QPixmap loadIcon(QString nam);
extern QPointArray FlattenPath(FPointArray ina, QValueList<uint> &Segs);

ExtImageProps::ExtImageProps( QWidget* parent, ImageInfoRecord *info, PageItem *item, ScribusView *view )  : QDialog( parent, "ExtImageProps", true, 0 )
{
	setIcon(loadIcon("AppIcon.png"));
	setCaption( tr( "Extended Image Properties" ) );
	ExtImagePropsLayout = new QVBoxLayout( this, 10, 5, "ExtImagePropsLayout");
	viewWidget = view;
	currentItem = item;
	propsTab = new QTabWidget( this, "propsTab" );

	tab = new QWidget( propsTab, "tab" );
	tabLayout = new QVBoxLayout( tab, 10, 5, "tabLayout");
	layout1 = new QHBoxLayout( 0, 0, 5, "layout1");
	textLabel1 = new QLabel( tab, "textLabel1" );
	textLabel1->setText( tr( "Blend Mode:" ) );
	layout1->addWidget( textLabel1 );
	blendMode = new QComboBox( false, tab, "blendMode" );
	layout1->addWidget( blendMode );
	textLabel2 = new QLabel( tab, "textLabel2" );
	textLabel2->setText( tr( "Opacity:" ) );
	layout1->addWidget( textLabel2 );
	opacitySpinBox = new QSpinBox( tab, "opacitySpinBox" );
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
//	Header->setResizeEnabled(false);
	FlagsSicht.clear();
	if (info->layerInfo.count() != 0)
	{
		QString tmp;
		QValueList<PSDLayer>::iterator it2;
		layerTable->setNumRows(info->layerInfo.count());
		uint counter = 0;
		for (it2 = info->layerInfo.begin(); it2 != info->layerInfo.end(); ++it2)
		{
			QCheckBox *cp = new QCheckBox(this, (*it2).layerName);
			cp->setChecked(!((*it2).flags & 2));
			FlagsSicht.append(cp);
//			connect(cp2, SIGNAL(clicked()), this, SLOT(visibleLayer()));
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
	propsTab->insertTab( tab,  tr( "Layers" ) );

	tab_2 = new QWidget( propsTab, "tab_2" );
	tabLayout_2 = new QVBoxLayout( tab_2, 10, 5, "tabLayout_2");
	pathList = new QListBox( tab_2, "pathList" );
	pathList->clear();
	QMap<QString, FPointArray>::Iterator it;
	QPixmap dummy(16,16);
	dummy.fill(white);
	if (info->PDSpathData.count() != 0)
	{
		for (it = info->PDSpathData.begin(); it != info->PDSpathData.end(); ++it)
		{
			if (it.key() == info->clipPath)
				pathList->insertItem(loadIcon("ok.png"), it.key());
			else
				pathList->insertItem(dummy, it.key());
		}
	}
	tabLayout_2->addWidget( pathList );
	propsTab->insertTab( tab_2, tr( "Paths" ) );
	ExtImagePropsLayout->addWidget( propsTab );
	resize(330, 320);
	clearWState( WState_Polished );
	connect( pathList, SIGNAL( highlighted(QListBoxItem*) ), this, SLOT( selPath(QListBoxItem*) ) );
}

void ExtImageProps::selPath(QListBoxItem *c)
{
	currentItem->PoLine = currentItem->imgInfo.PDSpathData[c->text()].copy();
	currentItem->imgInfo.usedPath = c->text();
	QWMatrix cl;
	cl.scale(72.0 / currentItem->dpiX, 72.0 / currentItem->dpiY);
	currentItem->PoLine.map(cl);
	currentItem->FrameType = 3;
	currentItem->Clip = FlattenPath(currentItem->PoLine, currentItem->Segments);
	currentItem->ClipEdited = true;
	double lx = currentItem->Xpos;
	double ly = currentItem->Ypos;
	currentItem->LocalX = 0;
	currentItem->LocalY = 0;
	viewWidget->AdjustItemSize(currentItem);
	viewWidget->MoveItem(lx- currentItem->Xpos, ly - currentItem->Ypos, currentItem, false);
	viewWidget->setRedrawBounding(currentItem);
}

