#include "pageselector.h"
#include "pageselector.moc"

#include <qvariant.h>
#include <qcombobox.h>
#include <qlabel.h>
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

extern QPixmap loadIcon(QString nam);

class PageValidator : public QIntValidator {
public:
	PageValidator(int min, int max, QObject * parent);
	void fixup(QString & input);
	State validate(QString & input, int & pos);
private:
        QRegExp rx;
	PageSelector * pageSelector;
};

PageValidator::PageValidator(int min, int max, QObject * parent) : QIntValidator
(min, max, parent), rx("^([0-9])*.*") 
{
	pageSelector = static_cast<PageSelector*>(parent);
}

QValidator::State PageValidator::validate(QString & input, int & pos)
{
	if (rx.search(input) < 0) 
		return Intermediate;
	else {
		if (rx.cap(1).toInt() <= pageSelector->LastPG)
			return Acceptable;
		else
			return Intermediate;
	}
}

void PageValidator::fixup(QString & input)
{
	if (rx.search(pageSelector->pageEdit->text()) == 0)
		input = rx.cap(1);
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
	PageSelectorLayout->addWidget( Back );

#if OLD_PAGESEL
	Label1 = new QLabel( this, "Label1" );
	PageSelectorLayout->addWidget( Label1 );
	v = new QIntValidator(1, LastPG, this);
	PageCombo = new QComboBox( true, this, "PageCombo" );
	PageCombo->setEditable( true );
	PageCombo->setDuplicatesEnabled( false );
	QString tmp;
	for (int a = 0; a < LastPG; ++a)
		PageCombo->insertItem(tmp.setNum(a+1));
	PageCombo->setValidator(v);
	PageCombo->setFocusPolicy(QWidget::ClickFocus);
	PageSelectorLayout->addWidget( PageCombo );

	Label2 = new QLabel( this, "Label2" );
	PageSelectorLayout->addWidget( Label2 );
#else
	v = new PageValidator(1, LastPG, this);
	pageEdit = new QLineEdit("1", QString::null, this);     
//      pageEdit->setValidator(v);
	pageEdit->setAlignment(Qt::AlignHCenter);
	pageEdit->setFocusPolicy(QWidget::ClickFocus);
	PageSelectorLayout->addWidget(pageEdit);	
#if OPTION_USE_QTOOLBUTTON
	pageList = new QToolButton(this);
	pageList->setMaximumWidth(12);
	pageList->setPopupDelay(0);
#else
	pageList = new QPushButton(this);
	pageList->setMaximumWidth(12 + AQUA_EXTRA);
	pageList->setFlat(true);
#endif
	pageMenu = new QPopupMenu(this);
	pageList->setPopup(pageMenu);
	pageList->setFocusPolicy(QWidget::NoFocus);
	QString tmp;
	for (int a = 0; a < LastPG; ++a)
		pageMenu->insertItem(tmp.setNum(a+1), a);
	PageSelectorLayout->addWidget(pageList);
#endif
	
	Forward->setPixmap( loadIcon("forward.png") );
	Forward->setFocusPolicy(QWidget::NoFocus);
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
#if OLD_PAGESEL
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
#else
	connect( pageEdit, SIGNAL( returnPressed() ), this, SLOT( GotoPage() ) );
	connect( pageEdit, SIGNAL( lostFocus() ), this, SLOT( GotoPage() ) );
	connect( pageMenu, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ));
#endif
	connect( Back, SIGNAL( clicked() ), this, SLOT( goBk() ) );
	connect( Start, SIGNAL( clicked() ), this, SLOT( ToStart() ) );
	connect( Forward, SIGNAL( clicked() ), this, SLOT( goFw() ) );
	connect( Last, SIGNAL( clicked() ), this, SLOT( ToEnd() ) );
}

bool PageSelector::focused()
{
#if OLD_PAGESEL
	return PageCombo->hasFocus();
#else
	return pageEdit->hasFocus();	
#endif
}


void PageSelector::focusPolicy(QWidget::FocusPolicy policy)
{
#if OLD_PAGESEL
	PageCombo->setFocusPolicy(policy);
#else
	pageEdit->setFocusPolicy(policy);	
#endif
}


void PageSelector::GotoPgE(int a)
{
	GotoPg(a);
	emit GotoPage(a+1);
}


void PageSelector::GotoPage()
{
	static QRegExp rx("^([0-9])+.*");
	if (rx.search(pageEdit->text()) != 0)
		return;
	qDebug(QString("GotoPage %1").arg(rx.cap(1)));
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
#if OLD_PAGESEL
	disconnect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	PageCombo->setCurrentItem(a);
#else
	disconnect( pageEdit, SIGNAL( returnPressed() ), this, SLOT( GotoPage() ) );
	disconnect( pageEdit, SIGNAL( lostFocus() ), this, SLOT( GotoPage() ) );
	disconnect( pageMenu, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	pageEdit->setText(tr( "%1 of %2").arg(a+1).arg(LastPG));
#endif
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
#if OLD_PAGESEL
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
#else
	connect( pageEdit, SIGNAL( returnPressed() ), this, SLOT( GotoPage() ) );
	connect( pageEdit, SIGNAL( lostFocus() ), this, SLOT( GotoPage() ) );
	connect( pageMenu, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
#endif
}

void PageSelector::setMaxValue(int a)
{
#if OLD_PAGESEL
	disconnect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	PageCombo->clear();
#else
	disconnect( pageEdit, SIGNAL( returnPressed() ), this, SLOT( GotoPage() ) );
	disconnect( pageEdit, SIGNAL( lostFocus() ), this, SLOT( GotoPage() ) );
	disconnect( pageMenu, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
	pageMenu->clear();
#endif
	LastPG = a;
	QString tmp;
	v->setTop(LastPG);
#if OLD_PAGESEL
	for (int b = 0; b < LastPG; ++b)
	{
		PageCombo->insertItem(tmp.setNum(b+1));
	}
	Label2->setText( tr( " of %1" ).arg(LastPG) );
	connect( PageCombo, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
#else
	for (int b = 0; b < LastPG; ++b)
	{
		pageMenu->insertItem(tmp.setNum(b+1), b);
	}
	pageEdit->setText(tr( "%1 of %2").arg(APage).arg(LastPG));
	connect( pageEdit, SIGNAL( returnPressed() ), this, SLOT( GotoPage() ) );
	connect( pageEdit, SIGNAL( lostFocus() ), this, SLOT( GotoPage() ) );
	connect( pageMenu, SIGNAL( activated(int) ), this, SLOT( GotoPgE(int) ) );
#endif
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
#if OLD_PAGESEL
	Label1->setText( tr( "Page " ) );
	Label2->setText( tr( " of %1" ).arg(LastPG) );
#else
	pageEdit->setText(tr( "%1 of %2").arg(APage).arg(LastPG));
#endif
}
