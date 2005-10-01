#include "pageselector.h"
#include "pageselector.moc"

#include <qvariant.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpopupmenu.h>
#if OPTION_USE_QTOOLBUTTON
    #include <qtoolbutton.h>
#else
    #include <qpushbutton.h>
#endif
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>
#include "sccombobox.h"

extern QPixmap loadIcon(QString nam);

class PageValidator : public QValidator {
public:
	PageValidator(int min, int max, QObject * parent);
	void fixup(QString & input) const;
	State validate(QString & input, int & pos) const;
private:
	QRegExp rx;
	QRegExp rx2;
	PageSelector * pageSelector;
};

PageValidator::PageValidator(int min, int max, QObject * parent) : QValidator
(parent), rx("^([0-9]+).*"), rx2("^[0-9]+$") 
{
	pageSelector = static_cast<PageSelector*>(parent);
}

QValidator::State PageValidator::validate(QString & input, int & pos) const
{
	if (rx2.search(input) == 0 && pageSelector->PageCombo->text(input.toInt()-1) == input)
		return Acceptable;
	else
		return Intermediate;
}

void PageValidator::fixup(QString & input) const
{
	if (rx.search(input) == 0)
		input = const_cast<QRegExp &>(rx).cap(1);
}
	

PageSelector::PageSelector( QWidget* parent, int maxPg ) : QWidget( parent, "pgsel", 0 )
{
	LastPG = maxPg;
	APage = 1;
	PageSelectorLayout = new QHBoxLayout( this, 0, 1, "PageSelectorLayout");

#if OPTION_USE_QTOOLBUTTON
	Start = new QToolButton( this, "Start" );
	Start->setAutoRaise(OPTION_FLAT_BUTTON);
	Back = new QToolButton( this, "Back" );
	Back->setAutoRaise(OPTION_FLAT_BUTTON);
	Forward = new QToolButton( this, "Forward" );
	Forward->setAutoRaise(OPTION_FLAT_BUTTON);
	Last = new QToolButton( this, "Last" );
	Last->setAutoRaise(OPTION_FLAT_BUTTON);
#else
	Start = new QPushButton( this, "Start" );
	Start->setDefault( false );
	Start->setAutoDefault( false );
	Start->setFlat(OPTION_FLAT_BUTTON);
	Back = new QPushButton( this, "Back" );
	Back->setDefault( false );
	Back->setAutoDefault( false );
	Back->setFlat(OPTION_FLAT_BUTTON);
	Forward = new QPushButton( this, "Forward" );
	Forward->setDefault( false );
	Forward->setAutoDefault( false );
	Forward->setFlat(OPTION_FLAT_BUTTON);
	Last = new QPushButton( this, "Last" );
	Last->setDefault( false );
	Last->setAutoDefault( false );
	Last->setFlat(OPTION_FLAT_BUTTON);
#endif
	Start->setPixmap( loadIcon("start.png") );
	Start->setFocusPolicy(QWidget::NoFocus);
	PageSelectorLayout->addWidget( Start );

	Back->setPixmap( loadIcon("back.png") );
	Back->setFocusPolicy(QWidget::NoFocus);
	Back->setAutoRepeat(true);
	PageSelectorLayout->addWidget( Back );

	v = new PageValidator(1, LastPG, this);
	PageCombo = new ScComboBox( true, this, "PageCombo" );
	PageCombo->setEditable( true );
	PageCombo->setDuplicatesEnabled( false );
	PageCombo->lineEdit()->setAlignment(Qt::AlignHCenter);
	QString tmp;
	for (int a = 0; a < LastPG; ++a)
		PageCombo->insertItem(tmp.setNum(a+1));
	PageCombo->setValidator(v);
	PageCombo->setMinimumSize(fontMetrics().width( "999 of 999" )+20, 20);
	PageCombo->setFocusPolicy(QWidget::ClickFocus);
	PageSelectorLayout->addWidget( PageCombo );
	
	Forward->setPixmap( loadIcon("forward.png") );
	Forward->setFocusPolicy(QWidget::NoFocus);
	Forward->setAutoRepeat(true);
	PageSelectorLayout->addWidget( Forward );

	Last->setPixmap( loadIcon("finish.png") );
	Last->setFocusPolicy(QWidget::NoFocus);
	PageSelectorLayout->addWidget( Last );
	Forward->setEnabled(true);
	Last->setEnabled(true);
	Back->setEnabled(false);
	Start->setEnabled(false);
	if (APage == LastPG)
	{
		Forward->setEnabled(false);
		Last->setEnabled(false);
	}

	languageChange();
	// signals and slots connections
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	connect( Back, SIGNAL( clicked() ), this, SLOT( goBk() ) );
	connect( Start, SIGNAL( clicked() ), this, SLOT( ToStart() ) );
	connect( Forward, SIGNAL( clicked() ), this, SLOT( goFw() ) );
	connect( Last, SIGNAL( clicked() ), this, SLOT( ToEnd() ) );
}

bool PageSelector::focused()
{
	return PageCombo->hasFocus();
}


void PageSelector::focusPolicy(QWidget::FocusPolicy policy)
{
	PageCombo->setFocusPolicy(policy);
}


void PageSelector::GotoPgE(int a)
{
	GotoPg(a);
	emit GotoPage(a+1);
}


void PageSelector::GotoPage()
{
	static QRegExp rx("^([0-9])+.*");
	int p = rx.cap(1).toInt();
	if (p < 1)
		p=1;
	if (p > LastPG)
		p = LastPG;
	GotoPg(p-1);
	emit GotoPage(p);
}


void PageSelector::GotoPg(int a)
{
	disconnect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	PageCombo->setCurrentItem(a);
	PageCombo->setEditText( tr( "%1 of %1" ).arg(a+1).arg(LastPG) );
	APage = a+1;
	Back->setEnabled(true);
	Start->setEnabled(true);
	Forward->setEnabled(true);
	Last->setEnabled(true);
	if (a == 0)
	{
		Back->setEnabled(false);
		Start->setEnabled(false);
	}
	if (a == LastPG-1)
	{
		Forward->setEnabled(false);
		Last->setEnabled(false);
	}
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
}

void PageSelector::setMaxValue(int a)
{
	disconnect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	PageCombo->clear();
	LastPG = a;
	QString tmp;
//	v->setTop(LastPG);
	for (int b = 0; b < LastPG; ++b)
	{
		PageCombo->insertItem(tmp.setNum(b+1));
	}
	PageCombo->setEditText( tr( "%1 of %1" ).arg(APage).arg(LastPG) );
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
}

void PageSelector::ToStart()
{
	if (APage == 1)
		return;
	GotoPgE(0);
}

void PageSelector::ToEnd()
{
	if (APage == LastPG-1)
		return;
	GotoPgE(LastPG-1);
}

void PageSelector::goBk()
{
	APage--;
	if (APage < 1)
		APage = 1;
	GotoPgE(APage-1);
}

void PageSelector::goFw()
{
	APage++;
	if (APage > LastPG)
		APage = LastPG;
	GotoPgE(APage-1);
}

void PageSelector::languageChange()
{
	disconnect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	PageCombo->setEditText( tr( "%1 of %2").arg(APage).arg(LastPG) );
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
}
