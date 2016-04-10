/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "missing.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QPixmap>

#include "commonstrings.h"
#include "fontcombo.h"
#include "iconmanager.h"
#include "prefsmanager.h"
#include "scpage.h"
#include "scribusdoc.h"
#include "util.h"

MissingFont::MissingFont( QWidget* parent, QString fon, ScribusDoc* doc ) : QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr("Missing Font"));
	setWindowIcon(IconManager::instance()->loadIcon("AppIcon.png"));
	missingFontLayout = new QHBoxLayout( this );
	missingFontLayout->setMargin(10);
	missingFontLayout->setSpacing(0);
	missingFontGridLayout = new QGridLayout;
	missingFontGridLayout->setSpacing( 10 );
	missingFontGridLayout->setMargin( 0 );
	notInstalledLabel = new QLabel( tr("The Font %1 is not installed.").arg(fon), this );
	missingFontGridLayout->addWidget( notInstalledLabel, 0, 0, 1, 4 );
	pixmapLabel = new QLabel( this );
	pixmapLabel->setPixmap(style()->standardIcon(QStyle::SP_MessageBoxWarning).pixmap(32, 32));
	missingFontGridLayout->addWidget( pixmapLabel, 1, 0 );
	useLabel = new QLabel( tr( "Use" ), this );
	missingFontGridLayout->addWidget( useLabel, 1, 1 );
	replaceFontCombo = new FontComboH(this);
	replaceFontCombo->setCurrentFont(doc == NULL ? PrefsManager::instance()->appPrefs.itemToolPrefs.textFont : doc->itemToolPrefs().textFont);
	replacementFont = replaceFontCombo->currentFont();
	missingFontGridLayout->addWidget( replaceFontCombo, 1, 2 );
	insteadLabel = new QLabel( tr( "instead" ), this );
	missingFontGridLayout->addWidget( insteadLabel, 1, 3 );
	okButton = new QPushButton( CommonStrings::tr_OK, this );
	missingFontGridLayout->addWidget( okButton, 2, 2 );
	missingFontLayout->addLayout( missingFontGridLayout );

	// signals and slots connections
	connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
	connect( replaceFontCombo, SIGNAL(fontSelected(QString)), this, SLOT( newFont(const QString&) ) );
}

void MissingFont::newFont(const QString& replacement)
{
	replacementFont = replacement;
}

const QString MissingFont::getReplacementFont()
{
	return replacementFont;
}

