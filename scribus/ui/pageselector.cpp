/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "pageselector.h"

#include <QEvent>
#include <QLineEdit>
#if OPTION_USE_QTOOLBUTTON
    #include <QToolButton>
#else
    #include <QPushButton>
#endif
#include <QLabel>
#include <QToolTip>
#include <QRegExp>
#include <QPixmap>
#include <QHBoxLayout>
#include <QValidator>
#include "sccombobox.h"
#include "util_icon.h"
#include "util.h"

PageSelector::PageSelector( QWidget* parent, int maxPg ) : QWidget( parent, 0 )
{
	PageCountString = "%1" ;
	m_lastPage = maxPg;
	m_currentPage = 1;
	PageSelectorLayout = new QHBoxLayout( this );
	PageSelectorLayout->setMargin(0);
	PageSelectorLayout->setSpacing(1);

#if OPTION_USE_QTOOLBUTTON
	startButton = new QToolButton( this );
	startButton->setAutoRaise(OPTION_FLAT_BUTTON);
	backButton = new QToolButton( this );
	backButton->setAutoRaise(OPTION_FLAT_BUTTON);
	forwardButton = new QToolButton( this );
	forwardButton->setAutoRaise(OPTION_FLAT_BUTTON);
	lastButton = new QToolButton( this );
	lastButton->setAutoRaise(OPTION_FLAT_BUTTON);
#else
	startButton = new QPushButton( this );
	startButton->setDefault( false );
	startButton->setAutoDefault( false );
	startButton->setFlat(OPTION_FLAT_BUTTON);
	backButton = new QPushButton( this );
	backButton->setDefault( false );
	backButton->setAutoDefault( false );
	backButton->setFlat(OPTION_FLAT_BUTTON);
	forwardButton->setDefault( false );
	forwardButton->setAutoDefault( false );
	forwardButton->setFlat(OPTION_FLAT_BUTTON);
	lastButton = new QPushButton( this );
	lastButton->setDefault( false );
	lastButton->setAutoDefault( false );
	lastButton->setFlat(OPTION_FLAT_BUTTON);
#endif
	startButton->setIcon(QIcon(loadIcon("16/go-first.png")));
	startButton->setFocusPolicy(Qt::NoFocus);
	PageSelectorLayout->addWidget( startButton );

	backButton->setIcon(QIcon(loadIcon("16/go-previous.png")));
	backButton->setFocusPolicy(Qt::NoFocus);
	backButton->setAutoRepeat(true);
	PageSelectorLayout->addWidget( backButton );

	m_validator = new QIntValidator(1, m_lastPage, this);
	m_pageCombo = new ScComboBox( this );
	m_pageCombo->setEditable(true);
	m_pageCombo->setDuplicatesEnabled( false );
	m_pageCombo->lineEdit()->setAlignment(Qt::AlignHCenter);
	for (int i = 0; i < m_lastPage; ++i)
	{
		m_pageCombo->addItem(QString::number(i+1));
	}
	m_pageCombo->setValidator(m_validator);
	m_pageCombo->setMinimumSize(fontMetrics().width( "9999" )+20, 20);
	m_pageCombo->setFocusPolicy(Qt::ClickFocus);
	PageSelectorLayout->addWidget( m_pageCombo );
	
	pageCountLabel = new QLabel(PageCountString.arg(m_lastPage), this);
	PageSelectorLayout->addWidget(pageCountLabel);
			
	forwardButton->setIcon(QIcon(loadIcon("16/go-next.png")));
	forwardButton->setFocusPolicy(Qt::NoFocus);
	forwardButton->setAutoRepeat(true);
	PageSelectorLayout->addWidget( forwardButton );

	lastButton->setIcon(QIcon(loadIcon("16/go-last.png")));
	lastButton->setFocusPolicy(Qt::NoFocus);
	PageSelectorLayout->addWidget( lastButton );
	forwardButton->setEnabled(true);
	lastButton->setEnabled(true);
	backButton->setEnabled(false);
	startButton->setEnabled(false);
	if (m_currentPage == m_lastPage)
	{
		forwardButton->setEnabled(false);
		lastButton->setEnabled(false);
	}

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
	bool signalsBlocked=m_pageCombo->blockSignals(true);
	setCurrentComboItem(m_pageCombo, QString::number(m_currentPage));
	m_pageCombo->blockSignals(signalsBlocked);
}

