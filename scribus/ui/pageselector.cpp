/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pageselector.h"

#include <QByteArray>
#include <QComboBox>
#include <QDebug>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QRegExp>
#include <QToolTip>
#include <QValidator>

#include "scpaths.h"
#include "iconmanager.h"
#include "util.h"

PageSelector::PageSelector( QWidget* parent, int maxPg ) : QWidget( parent, 0 )
{
	PageCountString = "%1" ;
	m_lastPage = maxPg;
	m_currentPage = 1;
	PageSelectorLayout = new QHBoxLayout( this );
	PageSelectorLayout->setMargin(0);
	PageSelectorLayout->setSpacing(1);

	startButton = new QPushButton( this );
	backButton = new QPushButton( this );
	forwardButton = new QPushButton( this );
	lastButton = new QPushButton( this );

	startButton->setDefault( false );
	backButton->setDefault( false );
	forwardButton->setDefault( false );
	lastButton->setDefault( false );

	startButton->setAutoDefault( false );
	backButton->setAutoDefault( false );
	forwardButton->setAutoDefault( false );
	lastButton->setAutoDefault( false );

	startButton->setIcon(IconManager::instance()->loadIcon("16/go-first.png"));
	backButton->setIcon(IconManager::instance()->loadIcon("16/go-previous.png"));
	forwardButton->setIcon(IconManager::instance()->loadIcon("16/go-next.png"));
	lastButton->setIcon(IconManager::instance()->loadIcon("16/go-last.png"));

	startButton->setFocusPolicy(Qt::NoFocus);
	backButton->setFocusPolicy(Qt::NoFocus);
	forwardButton->setFocusPolicy(Qt::NoFocus);
	lastButton->setFocusPolicy(Qt::NoFocus);

	forwardButton->setAutoRepeat(true);
	backButton->setAutoRepeat(true);

	m_validator = new QIntValidator(1, m_lastPage, this);
	m_pageCombo = new QComboBox( this );
	m_pageCombo->setEditable(true);
	m_pageCombo->setDuplicatesEnabled( false );
	m_pageCombo->setInsertPolicy(QComboBox::NoInsert);
	m_pageCombo->lineEdit()->setAlignment(Qt::AlignHCenter);
	for (int i = 0; i < m_lastPage; ++i)
		m_pageCombo->addItem(QString::number(i+1));
	m_pageCombo->setValidator(m_validator);
	m_pageCombo->setMinimumSize(fontMetrics().width( "9999" )+20, 20);
	m_pageCombo->setFocusPolicy(Qt::ClickFocus);
	m_pageCombo->setObjectName("pageCombo");
	
	pageCountLabel = new QLabel(PageCountString.arg(m_lastPage), this);

	forwardButton->setEnabled(true);
	lastButton->setEnabled(true);
	backButton->setEnabled(false);
	startButton->setEnabled(false);
	if (m_currentPage == m_lastPage)
	{
		forwardButton->setEnabled(false);
		lastButton->setEnabled(false);
	}

	QByteArray stylesheet;
	if (loadRawText(ScPaths::instance().libDir() + "scribus.css", stylesheet))
	{
		QString downArrow(IconManager::instance()->pathForIcon("16/go-down.png"));
		QByteArray da;
		da.append(downArrow);
		stylesheet.replace("___downArrow___", da);
		setStyleSheet(QString(stylesheet));
	}

	PageSelectorLayout->addWidget( startButton );
	PageSelectorLayout->addWidget( backButton );
	PageSelectorLayout->addWidget( m_pageCombo );
	PageSelectorLayout->addWidget( pageCountLabel );
	PageSelectorLayout->addWidget( forwardButton );
	PageSelectorLayout->addWidget( lastButton );


	languageChange();
	// signals and slots connections
	connect( m_pageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	connect( backButton, SIGNAL( clicked() ), this, SLOT( goBackward()) );
	connect( startButton, SIGNAL( clicked() ), this, SLOT( goToStart() ) );
	connect( forwardButton, SIGNAL( clicked() ), this, SLOT( goForward() ) );
	connect( lastButton, SIGNAL( clicked() ), this, SLOT( goToEnd() ) );
}

bool PageSelector::hasFocus()
{
	return m_pageCombo->hasFocus();
}

void PageSelector::clearFocus()
{
	m_pageCombo->clearFocus();
}

void PageSelector::focusPolicy(Qt::FocusPolicy policy)
{
	m_pageCombo->setFocusPolicy(policy);
}

void PageSelector::setFont ( const QFont &fo )
{
	pageCountLabel->setFont(fo);
	QWidget::setFont(fo);
}

int PageSelector::getCurrentPage()
{
	return m_currentPage;
}

void PageSelector::GotoPgE(int i)
{
	clearFocus();
	setGUIForPage(i);
	emit GotoPage(i+1);
}

void PageSelector::GotoPage()
{
	static QRegExp rx("^([0-9])+.*");
	int p = rx.cap(1).toInt();
	if (p < 1)
		p=1;
	if (p > m_lastPage)
		p = m_lastPage;
	setGUIForPage(p-1);
	emit GotoPage(p);
}

void PageSelector::setGUIForPage(int i)
{
	m_pageCombo->blockSignals(true);
	m_pageCombo->setCurrentIndex(i);
	setCurrentComboItem(m_pageCombo, QString::number(i+1));
	m_currentPage = i+1;
	backButton->setEnabled(i != 0);
	startButton->setEnabled(i != 0);
	forwardButton->setEnabled(i != m_lastPage-1);
	lastButton->setEnabled(i != m_lastPage-1);
	m_pageCombo->blockSignals(false);
}

void PageSelector::setMaximum(int i)
{
	m_pageCombo->blockSignals(true);
	m_pageCombo->clear();
	m_lastPage = i;
	m_validator->setRange(1, m_lastPage);
	for (int b = 0; b < m_lastPage; ++b)
	{
		m_pageCombo->addItem(QString::number(b+1));
	}
	setCurrentComboItem(m_pageCombo, QString::number(m_currentPage));
	pageCountLabel->setText(PageCountString.arg(m_lastPage));
	m_pageCombo->blockSignals(false);
}

void PageSelector::goToStart()
{
	if (m_currentPage == 1)
		return;
	GotoPgE(0);
}

void PageSelector::goToEnd()
{
	if (m_currentPage == m_lastPage)
		return;
	GotoPgE(m_lastPage-1);
}

void PageSelector::goBackward()
{
	m_currentPage--;
	if (m_currentPage < 1)
		m_currentPage = 1;
	GotoPgE(m_currentPage-1);
}

void PageSelector::goForward()
{
	m_currentPage++;
	if (m_currentPage > m_lastPage)
		m_currentPage = m_lastPage;
	GotoPgE(m_currentPage-1);
}

void PageSelector::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void PageSelector::languageChange()
{
	startButton->setToolTip( tr("Go to the first page") );
	backButton->setToolTip( tr("Go to the previous page") );
	forwardButton->setToolTip( tr("Go to the next page") );
	lastButton->setToolTip( tr("Go to the last page") );
	m_pageCombo->setToolTip( tr("Select the current page") );
	PageCountString =  tr(" of %1", "number of pages in document");
	pageCountLabel->setText(PageCountString.arg(m_lastPage));
	setCurrentComboItem(m_pageCombo, QString::number(m_currentPage));
}

