/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "extimageprops.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QTabWidget>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>
#include <QToolTip>
#include <QPainter>
#include <QWidget>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>

#include "commonstrings.h"
#include "pageitem.h"
#include "sccombobox.h"
#include "scpainter.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "util_icon.h"
#include "util_math.h"
#include "util.h"

ExtImageProps::ExtImageProps( QWidget* parent, ImageInfoRecord *info, PageItem *item, ScribusView *view )
	: QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr( "Extended Image Properties" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));
	ExtImagePropsLayout = new QVBoxLayout( this );
	ExtImagePropsLayout->setMargin(6);
	ExtImagePropsLayout->setSpacing(6);
	m_view = view;
	m_timer = 0;
	if (info->layerInfo.count() != 0)
	{
		m_timer = new QTimer(this);
		m_timer->setSingleShot(true);
		m_timer->setInterval(350);
	}
	m_item = item;
	currentLayer = 0;
	originalInfo = *info;
	originalImageClip = item->imageClip.copy();
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
	propsTab = new QTabWidget( this );
	QPalette palette;
	palette.setColor(backgroundRole(), Qt::white);
	if (info->layerInfo.count() != 0)
	{
		tab = new QWidget( propsTab );
		tabLayout = new QVBoxLayout( tab );
		tabLayout->setMargin(6);
		tabLayout->setSpacing(6);
		layout1 = new QHBoxLayout;
		layout1->setMargin(0);
		layout1->setSpacing(6);
		textLabel1 = new QLabel( tab );
		textLabel1->setText( tr( "Blend Mode:" ) );
		layout1->addWidget( textLabel1 );
		blendMode = new ScComboBox( tab );
		blendMode->clear();
		blendMode->addItem( tr("Normal"));
		blendMode->addItem( tr("Darken"));
		blendMode->addItem( tr("Lighten"));
		blendMode->addItem( tr("Hue"));
		blendMode->addItem( tr("Saturation"));
		blendMode->addItem( tr("Color"));
		blendMode->addItem( tr("Luminosity"));
		blendMode->addItem( tr("Multiply"));
		blendMode->addItem( tr("Screen"));
		blendMode->addItem( tr("Dissolve"));
		blendMode->addItem( tr("Overlay"));
		blendMode->addItem( tr("Hard Light"));
		blendMode->addItem( tr("Soft Light"));
		blendMode->addItem( tr("Difference"));
		blendMode->addItem( tr("Exclusion"));
		blendMode->addItem( tr("Color Dodge"));
		blendMode->addItem( tr("Color Burn"));
		layout1->addWidget( blendMode );
		textLabel2 = new QLabel( tab );
		textLabel2->setText( tr( "Opacity:" ) );
		layout1->addWidget( textLabel2 );
		opacitySpinBox = new QSpinBox( tab );
		opacitySpinBox->setKeyboardTracking(false);
		opacitySpinBox->setMinimum(0);
		opacitySpinBox->setMaximum(100);
		opacitySpinBox->setSingleStep(10);
		opacitySpinBox->setSuffix( tr(" %"));
		layout1->addWidget( opacitySpinBox );
		tabLayout->addLayout( layout1 );
		layerTable = new QTableWidget(info->layerInfo.count(), 3, tab );
		layerTable->setHorizontalHeaderItem(0, new QTableWidgetItem(QIcon(loadIcon("16/show-object.png")), ""));
		layerTable->setHorizontalHeaderItem(1, new QTableWidgetItem(""));
		layerTable->setHorizontalHeaderItem(2, new QTableWidgetItem( tr("Name")));
		QHeaderView* headerH = layerTable->horizontalHeader();
		headerH->setStretchLastSection(true);
		headerH->setMovable(false);
		headerH->setClickable(false);
		if (info->layerInfo.count() == 1)
		{
			layerTable->setColumnWidth(1, 40);
			layerTable->setColumnWidth(0, 24);
		}
		layerTable->setSortingEnabled(false);
		layerTable->setSelectionBehavior(QTableWidget::SelectRows);
		QHeaderView *Header = layerTable->verticalHeader();
		Header->setMovable(false);
		Header->setResizeMode(QHeaderView::Fixed);
		Header->hide();
		FlagsSicht.clear();
		int col2Width = 0;
		int col1Width = 0;
		if ((info->isRequest) && (info->RequestProps.contains(0)))
		{
			opacitySpinBox->setValue(qRound(info->RequestProps[0].opacity / 255.0 * 100));
			setCurrentComboItem(blendMode, blendModes[info->RequestProps[0].blend]);
		}
		else
		{
			opacitySpinBox->setValue(qRound(info->layerInfo[0].opacity / 255.0 * 100));
			setCurrentComboItem(blendMode, blendModes[info->layerInfo[0].blend]);
		}
		opacitySpinBox->setEnabled(true);
		blendMode->setEnabled(true);
		QString tmp;
		QList<PSDLayer>::iterator it2;
		uint counter = 0;
		for (it2 = info->layerInfo.begin(); it2 != info->layerInfo.end(); ++it2)
		{
			QCheckBox *cp = new QCheckBox(it2->layerName, this);
			cp->setPalette(palette);
			QPixmap pm;
			pm=QPixmap::fromImage(it2->thumb);
			col1Width = qMax(col1Width, pm.width());
			cp->setIcon(pm);
			FlagsSicht.append(cp);
			connect(cp, SIGNAL(clicked()), this, SLOT(changedLayer()));
			layerTable->setCellWidget(info->layerInfo.count()-counter-1, 0, cp);
			if ((info->isRequest) && (info->RequestProps.contains(counter)))
				cp->setChecked(info->RequestProps[counter].visible);
			else
				cp->setChecked(!(it2->flags & 2));
			if (!it2->thumb_mask.isNull())
			{
				QCheckBox *cp2 = new QCheckBox(it2->layerName, this);
				cp2->setPalette(palette);
				QPixmap pm2;
				pm2=QPixmap::fromImage(it2->thumb_mask);
				col2Width = qMax(col2Width, pm2.width());
				cp2->setIcon(pm2);
				connect(cp2, SIGNAL(clicked()), this, SLOT(changedLayer()));
				layerTable->setCellWidget(info->layerInfo.count()-counter-1, 1, cp2);
				if ((info->isRequest) && (info->RequestProps.contains(counter)))
					cp2->setChecked(info->RequestProps[counter].useMask);
				else
					cp2->setChecked(true);
				FlagsMask.append(cp2);
			}
			else
				FlagsMask.append(0);
			QTableWidgetItem *tW = new QTableWidgetItem(it2->layerName);
			tW->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			layerTable->setItem(info->layerInfo.count()-counter-1, 2, tW);
			layerTable->setRowHeight(info->layerInfo.count()-counter-1, 40);
			counter++;
		}
		tabLayout->addWidget( layerTable );
		layerTable->setColumnWidth(1, 24 + col2Width);
		layerTable->setColumnWidth(0, 24 + col1Width);
		blendMode->setCurrentIndex(0);
// 		headerH->setResizeMode(QHeaderView::Fixed);
		propsTab->addTab( tab,  tr( "Layers" ) );
	}
	tab_2 = new QWidget( propsTab );
	tabLayout_2 = new QVBoxLayout( tab_2 );
	tabLayout_2->setMargin(6);
	tabLayout_2->setSpacing(6);
	pathList = new QListWidget( tab_2 );
	pathList->clear();
	pathList->setIconSize(QSize(40, 40));
	QMap<QString, FPointArray>::Iterator it;
	if (info->PDSpathData.count() != 0)
	{
		for (it = info->PDSpathData.begin(); it != info->PDSpathData.end(); ++it)
		{
			QImage pixm(40, 40, QImage::Format_ARGB32);
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
			QPixmap pm;
			pm=QPixmap::fromImage(pixm);
			new QListWidgetItem(QIcon(pm), it.key(), pathList);
			if (it.key() == info->usedPath)
			{
				pathList->setCurrentRow(pathList->count()-1);
				pathList->currentItem()->setSelected(true);
			}
		}
	}
	tabLayout_2->addWidget( pathList );
	resetPath = new QPushButton( tr("Don't use any Path"), tab_2);
	tabLayout_2->addWidget( resetPath );
	propsTab->addTab( tab_2, tr( "Paths" ) );
	ExtImagePropsLayout->addWidget( propsTab );

	layoutBottom = new QHBoxLayout;
	layoutBottom->setMargin(0);
	layoutBottom->setSpacing(6);
	livePreview = new QCheckBox( this );
	livePreview->setText( tr( "Live Preview" ) );
	livePreview->setChecked(true);
	doPreview = true;
	layoutBottom->addWidget( livePreview );
	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	layoutBottom->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this );
	layoutBottom->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	cancelButton->setDefault( true );
	layoutBottom->addWidget( cancelButton );
	ExtImagePropsLayout->addLayout( layoutBottom );
	resize(330, 320);

	connect(pathList, SIGNAL( itemClicked(QListWidgetItem*) ), this, SLOT( selPath(QListWidgetItem*) ) );
	connect(resetPath, SIGNAL(clicked()), this, SLOT(noPath()));
	connect(livePreview, SIGNAL(clicked()), this, SLOT(changePreview()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(leaveOK()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(leaveCancel()));
	if (info->layerInfo.count() != 0)
	{
		layerTable->selectionModel()->clearSelection();
		opacitySpinBox->setEnabled(false);
		blendMode->setEnabled(false);
		connect(m_timer, SIGNAL(timeout()), this,  SLOT(changedLayer()));
		connect(layerTable, SIGNAL(itemSelectionChanged()), this, SLOT(selLayer()));
		connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(delayedLayerChange()));
		connect(blendMode, SIGNAL(activated(int)), this, SLOT(changedLayer()));
	}
}

void ExtImageProps::leaveOK()
{
	doPreview = false;
	if (originalInfo.layerInfo.count() != 0)
		changedLayer();
	m_view->Doc->LoadPict(m_item->Pfile, m_item->ItemNr, true);
	if (pathList->count() != 0)
	{
		QList<QListWidgetItem *>sel = pathList->selectedItems();
		if (sel.count() != 0)
		{
			m_item->imageClip = m_item->pixm.imgInfo.PDSpathData[sel[0]->text()].copy();
			m_item->pixm.imgInfo.usedPath = sel[0]->text();
			QMatrix cl;
			cl.translate(m_item->imageXOffset()*m_item->imageXScale(), m_item->imageYOffset()*m_item->imageYScale());
			cl.scale(m_item->imageXScale(), m_item->imageYScale());
			m_item->imageClip.map(cl);
		}
		else
		{
			m_item->imageClip.resize(0);
			m_item->pixm.imgInfo.usedPath = "";
		}
	}
	m_item->update();
	accept();
}

void ExtImageProps::leaveCancel()
{
	m_item->pixm.imgInfo = originalInfo;
	m_view->Doc->LoadPict(m_item->Pfile, m_item->ItemNr, true);
	m_item->imageClip = originalImageClip.copy();
	m_item->update();
	reject();
}

void ExtImageProps::changePreview()
{
	doPreview = livePreview->isChecked();
	if (doPreview)
	{
		if (originalInfo.layerInfo.count() != 0)
			changedLayer();
		m_view->Doc->LoadPict(m_item->Pfile, m_item->ItemNr, true);
		if (pathList->count() != 0)
		{
			QList<QListWidgetItem *>sel = pathList->selectedItems();
			if (sel.count() != 0)
			{
				m_item->imageClip = m_item->pixm.imgInfo.PDSpathData[sel[0]->text()].copy();
				m_item->pixm.imgInfo.usedPath = sel[0]->text();
				QMatrix cl;
				cl.translate(m_item->imageXOffset()*m_item->imageXScale(), m_item->imageYOffset()*m_item->imageYScale());
				cl.scale(m_item->imageXScale(), m_item->imageYScale());
				m_item->imageClip.map(cl);
			}
			else
			{
				m_item->imageClip.resize(0);
				m_item->pixm.imgInfo.usedPath = "";
			}
		}
		m_item->update();
	}
	else
	{
		m_item->pixm.imgInfo = originalInfo;
		m_view->Doc->LoadPict(m_item->Pfile, m_item->ItemNr, true);
		m_item->imageClip = originalImageClip.copy();
		m_item->update();
	}
}

void ExtImageProps::changedLayer()
{
	updateLayerInfo();
	if (doPreview)
	{
		m_view->Doc->LoadPict(m_item->Pfile, m_item->ItemNr, true);
		m_item->update();
	}
}

void ExtImageProps::delayedLayerChange()
{
	if (m_timer->isActive())
		m_timer->stop();
	updateLayerInfo();
	m_timer->start();
}

void ExtImageProps::selLayer()
{
	QModelIndexList selectedRows = layerTable->selectionModel()->selectedRows();
	if (selectedRows.count() <= 0)
	{
		currentLayer = -1;
		opacitySpinBox->setEnabled(false);
		blendMode->setEnabled(false);
		return;
	}

	int selectedRow = selectedRows.at(0).row();
	currentLayer = layerTable->rowCount() - selectedRow - 1;

	disconnect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(delayedLayerChange()));
	disconnect(blendMode, SIGNAL(activated(int)), this, SLOT(changedLayer()));
	if ((m_item->pixm.imgInfo.isRequest) && (m_item->pixm.imgInfo.RequestProps.contains(currentLayer)))
	{
		opacitySpinBox->setValue(qRound(m_item->pixm.imgInfo.RequestProps[currentLayer].opacity / 255.0 * 100));
		setCurrentComboItem(blendMode, blendModes[m_item->pixm.imgInfo.RequestProps[currentLayer].blend]);
	}
	else
	{
		opacitySpinBox->setValue(qRound(m_item->pixm.imgInfo.layerInfo[currentLayer].opacity / 255.0 * 100));
		setCurrentComboItem(blendMode, blendModes[m_item->pixm.imgInfo.layerInfo[currentLayer].blend]);
	}
	opacitySpinBox->setEnabled(true);
	blendMode->setEnabled(true);
	connect(opacitySpinBox, SIGNAL(valueChanged(int)), this, SLOT(delayedLayerChange()));
	connect(blendMode, SIGNAL(activated(int)), this, SLOT(changedLayer()));
}


void ExtImageProps::updateLayerInfo()
{
	struct ImageLoadRequest loadingInfo;
	bool isRequest = m_item->pixm.imgInfo.isRequest;
	for (int r = 0; r < layerTable->rowCount(); ++r)
	{
		int layerIndex = layerTable->rowCount() - r - 1;
		if (currentLayer == layerIndex)
		{
			loadingInfo.blend = blendModesRev[blendMode->currentText()];
			loadingInfo.opacity = qRound(opacitySpinBox->value() / 100.0 * 255);
		}
		else if ((isRequest) && (m_item->pixm.imgInfo.RequestProps.contains(layerIndex)))
		{
			loadingInfo.blend = m_item->pixm.imgInfo.RequestProps[layerIndex].blend;
			loadingInfo.opacity = m_item->pixm.imgInfo.RequestProps[layerIndex].opacity;
		}
		else
		{
			loadingInfo.blend = m_item->pixm.imgInfo.layerInfo[layerIndex].blend;
			loadingInfo.opacity = m_item->pixm.imgInfo.layerInfo[layerIndex].opacity;
		}
		loadingInfo.visible = FlagsSicht.at(layerIndex)->isChecked();
		if (FlagsMask.at(layerIndex))
			loadingInfo.useMask = FlagsMask.at(layerIndex)->isChecked();
		else
			loadingInfo.useMask = true;
		m_item->pixm.imgInfo.RequestProps.insert(layerIndex, loadingInfo);
	}
	m_item->pixm.imgInfo.isRequest = true;
}

void ExtImageProps::noPath()
{
	disconnect(pathList, SIGNAL( itemClicked(QListWidgetItem*) ), this, SLOT( selPath(QListWidgetItem*) ) );
	pathList->clearSelection();
	if (doPreview)
	{
		m_item->imageClip.resize(0);
		m_item->pixm.imgInfo.usedPath = "";
		m_item->update();
	}
	connect(pathList, SIGNAL( itemClicked(QListWidgetItem*) ), this, SLOT( selPath(QListWidgetItem*) ) );
}

void ExtImageProps::selPath(QListWidgetItem *c)
{
	if ((c != NULL) && (doPreview))
	{
		m_item->imageClip = m_item->pixm.imgInfo.PDSpathData[c->text()].copy();
		m_item->pixm.imgInfo.usedPath = c->text();
		QMatrix cl;
		cl.translate(m_item->imageXOffset()*m_item->imageXScale(), m_item->imageYOffset()*m_item->imageYScale());
		cl.scale(m_item->imageXScale(), m_item->imageYScale());
		m_item->imageClip.map(cl);
		m_item->update();
	}
}

