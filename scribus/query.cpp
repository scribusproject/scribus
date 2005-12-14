#include "query.h"
#include "query.moc"
extern QPixmap loadIcon(QString nam);

#include <qtooltip.h>
#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

#include "commonstrings.h"

Query::Query( QWidget* parent,  const char* name, bool modal, WFlags fl, QString text, QString titel )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
		setName( "Query" );
    setCaption( titel );
  	setIcon(loadIcon("AppIcon.png"));
    queryLayout = new QVBoxLayout( this, 11, 6 );
    editLayout = new QHBoxLayout;
    editLayout->setSpacing( 6 );
    editLayout->setMargin( 0 );
    answerEdit = new QLineEdit( this, "answerEdit" );
    questionLabel = new QLabel( answerEdit, text, this, "questionLabel" );
    questionLabel->setFrameShape( QLabel::MShape );
    questionLabel->setFrameShadow( QLabel::MShadow );
    questionLabel->adjustSize();
    editLayout->addWidget( questionLabel );
    editLayout->addWidget( answerEdit );
    queryLayout->addLayout( editLayout );
    okCancelLayout = new QHBoxLayout;
    okCancelLayout->setSpacing( 6 );
    okCancelLayout->setMargin( 0 );
    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    okCancelLayout->addItem( spacer );
    okButton = new QPushButton( CommonStrings::tr_OK, this, "okButton" );
    okButton->setDefault( true );
    okCancelLayout->addWidget( okButton );
    cancelButton = new QPushButton( CommonStrings::tr_Cancel, this, "cancelButton" );
    okCancelLayout->addWidget( cancelButton );
    queryLayout->addLayout( okCancelLayout );
    setMaximumSize(sizeHint());
    answerEdit->setFocus();

    // signals and slots connections
    connect( okButton, SIGNAL( clicked() ), this, SLOT( Leave() ) );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    // tab order
    setTabOrder( answerEdit, okButton );
    setTabOrder( okButton, cancelButton );
}

void Query::Leave()
{
	if (answerEdit->text().isEmpty())
		return;
	else
		accept();
}

const QString Query::getEditText()
{
	return answerEdit->text();
}

void Query::setEditText(QString newText, bool setSelected)
{
	answerEdit->setText(newText);
	if (setSelected)
		answerEdit->selectAll();
}
