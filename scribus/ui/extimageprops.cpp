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
#include <QComboBox>
#include <QPushButton>
#include <QToolTip>
#include <QPainter>
#include <QWidget>
#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTimer>

#include "pageitem.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "iconmanager.h"
#include "util.h"
#include "util_math.h"

ExtImageProps::ExtImageProps( QWidget* parent, PageItem *item, ScribusView *view )
	: QDialog( parent )
{
	setupUi(this);

	setModal(false);
	setWindowTitle( tr( "Extended Image Properties" ) );
	setWindowIcon(IconManager::instance().loadIcon("app-icon"));

	if (!item->pixm.imgInfo.valid)
		return;

	m_item = item;
	m_view = view;

	ImageInfoRecord *info = &m_item->pixm.imgInfo;

	originalInfo = *info;
	originalImageClip = m_item->imageClip.copy();

	imageLayers->setCurrentItem(m_item, m_view);

	if (info->PDSpathData.count() == 0)
	{
		tabWidget->removeTab(1);
		imagePaths->setCurrentItem(nullptr, m_view);
	}
	else
		imagePaths->setCurrentItem(m_item, m_view);

	tabWidget->setCurrentIndex(0);
}

void ExtImageProps::accept()
{
	QDialog::accept();
}

void ExtImageProps::reject()
{
	if (m_item)
	{
		m_item->pixm.imgInfo = originalInfo;
		m_view->m_doc->loadPict(m_item->Pfile, m_item, true);
		m_item->imageClip = originalImageClip.copy();
		m_item->update();
	}

	QDialog::reject();
}
