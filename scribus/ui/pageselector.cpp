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
#include <QSignalBlocker>
#include <QToolTip>
#include <QValidator>

#include "scpaths.h"
#include "scribusapp.h"
#include "iconmanager.h"
#include "util.h"

PageSelector::PageSelector( QWidget* parent, int maximumPage ) : QWidget( parent, Qt::WindowFlags() )
{
	pageCountString = "%1";
	m_lastPage = maximumPage;
	pageSelectorLayout = new QHBoxLayout( this );
	pageSelectorLayout->setContentsMargins(0, 0, 0, 0);
	pageSelectorLayout->setSpacing(1);

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

	startButton->setIcon(IconManager::instance().loadIcon("16/go-first.png"));
	backButton->setIcon(IconManager::instance().loadIcon("16/go-previous.png"));
	forwardButton->setIcon(IconManager::instance().loadIcon("16/go-next.png"));
	lastButton->setIcon(IconManager::instance().loadIcon("16/go-last.png"));

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
	m_pageCombo->setMinimumSize(fontMetrics().horizontalAdvance( "9999" )+20, 20);
	m_pageCombo->setFocusPolicy(Qt::ClickFocus);
	m_pageCombo->setObjectName("pageCombo");
	
	pageCountLabel = new QLabel(pageCountString.arg(m_lastPage), this);

	forwardButton->setEnabled(true);
	lastButton->setEnabled(true);
	backButton->setEnabled(false);
	startButton->setEnabled(false);
	if (m_currentPage == m_lastPage)
	{
		forwardButton->setEnabled(false);
		lastButton->setEnabled(false);
	}

	iconSetChange();

	pageSelectorLayout->addWidget( startButton );
	pageSelectorLayout->addWidget( backButton );
	pageSelectorLayout->addWidget( m_pageCombo );
	pageSelectorLayout->addWidget( pageCountLabel );
	pageSelectorLayout->addWidget( forwardButton );
	pageSelectorLayout->addWidget( lastButton );

	languageChange();

	// signals and slots connections
	connect(ScQApp, SIGNAL(iconSetChanged()), this, SLOT(iconSetChange()));

	connect( m_pageCombo, SIGNAL( activated(int) ), this, SLOT( gotoPage(int) ) );
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

void PageSelector::setFont(const QFont& font)
{
	pageCountLabel->setFont(font);
	QWidget::setFont(font);
}

int PageSelector::getCurrentPage()
{
	return m_currentPage;
}

void PageSelector::gotoPage(int i)
{
	clearFocus();
	setGUIForPage(i);
	emit pageChanged(i + 1);
}

void PageSelector::setGUIForPage(int i)
{
	QSignalBlocker signalBlocker(m_pageCombo);
	m_pageCombo->setCurrentIndex(i);
	m_currentPage = i + 1;
	setCurrentComboItem(m_pageCombo, QString::number(m_currentPage));
	backButton->setEnabled(i != 0);
	startButton->setEnabled(i != 0);
	forwardButton->setEnabled(i != m_lastPage - 1);
	lastButton->setEnabled(i != m_lastPage - 1);
}

void PageSelector::setMaximum(int i)
{
	QSignalBlocker signalBlocker(m_pageCombo);
	m_pageCombo->clear();
	m_lastPage = i;
	m_validator->setRange(1, m_lastPage);
	for (int b = 0; b < m_lastPage; ++b)
	{
		m_pageCombo->addItem(QString::number(b + 1));
	}
	setCurrentComboItem(m_pageCombo, QString::number(m_currentPage));
	pageCountLabel->setText(pageCountString.arg(m_lastPage));
}

void PageSelector::goToStart()
{
	if (m_currentPage == 1)
		return;
	gotoPage(0);
}

void PageSelector::goToEnd()
{
	if (m_currentPage == m_lastPage)
		return;
	gotoPage(m_lastPage - 1);
}

void PageSelector::goBackward()
{
	m_currentPage--;
	if (m_currentPage < 1)
		m_currentPage = 1;
	gotoPage(m_currentPage - 1);
}

void PageSelector::goForward()
{
	m_currentPage++;
	if (m_currentPage > m_lastPage)
		m_currentPage = m_lastPage;
	gotoPage(m_currentPage - 1);
}

void PageSelector::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
		languageChange();
	else
		QWidget::changeEvent(e);
}

void PageSelector::iconSetChange()
{
	QByteArray stylesheet;
	if (loadRawText(ScPaths::instance().libDir() + "scribus.css", stylesheet))
	{
		QString downArrow(IconManager::instance().pathForIcon("16/go-down.png"));
		QByteArray da;
		da.append(downArrow.toUtf8());
		stylesheet.replace("___downArrow___", da);
		setStyleSheet(QString(stylesheet));
	}
}

void PageSelector::languageChange()
{
	startButton->setToolTip( tr("Go to the first page") );
	backButton->setToolTip( tr("Go to the previous page") );
	forwardButton->setToolTip( tr("Go to the next page") );
	lastButton->setToolTip( tr("Go to the last page") );
	m_pageCombo->setToolTip( tr("Select the current page") );
	pageCountString =  tr(" of %1", "number of pages in document");
	pageCountLabel->setText(pageCountString.arg(m_lastPage));
	setCurrentComboItem(m_pageCombo, QString::number(m_currentPage));
}

