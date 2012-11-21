/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "query.h"
#include <QPixmap>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QRegExp>
#include <QValidator>
#include <QRegExpValidator>
#include "commonstrings.h"
#include "util_icon.h"

Query::Query( QWidget* parent,  const char* name, bool modal, Qt::WFlags fl, QString text, QString titel )
		: QDialog( parent )
{
	setWindowTitle( titel );
	setWindowIcon(loadIcon("AppIcon.png"));
	setModal(modal);
	queryLayout = new QVBoxLayout( this );
	queryLayout->setMargin(5);
	queryLayout->setSpacing(10);
	editLayout = new QHBoxLayout;
	editLayout->setSpacing( 5 );
	editLayout->setMargin( 0 );
	answerEdit = new QLineEdit( this );
	questionLabel = new QLabel( text, this );
	questionLabel->setBuddy(answerEdit);
	questionLabel->adjustSize();
	editLayout->addWidget( questionLabel );
	editLayout->addWidget( answerEdit );
	queryLayout->addLayout( editLayout );
	okCancelLayout = new QHBoxLayout;
	okCancelLayout->setSpacing( 5 );
	okCancelLayout->setMargin( 0 );
	QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	okCancelLayout->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this);
	okButton->setDefault( true );
	okCancelLayout->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	okCancelLayout->addWidget( cancelButton );
	queryLayout->addLayout( okCancelLayout );
	setMaximumSize(sizeHint());
	answerEdit->setFocus();
	checkList = QStringList();
	checkMode = false;

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
	if (!forbiddenList.isEmpty())
	{
		if (forbiddenList.contains(answerEdit->text()))
		{
			QMessageBox::warning(this, CommonStrings::trWarning, tr("Name \"%1\" is not allowed.\nPlease choose another.").arg(answerEdit->text()), CommonStrings::tr_OK);
			return;
		}
	}
	if (!checkList.isEmpty())
	{
		if (checkList.contains(answerEdit->text()))
		{
			if (checkMode)
			{
				int ret = QMessageBox::warning(this, 
												CommonStrings::trWarning,
												tr("Name \"%1\" already exists.\nDo you want to replace the current contents?").arg(answerEdit->text()),
												QMessageBox::Yes | QMessageBox::No,
												QMessageBox::No);
				if (ret == QMessageBox::No)
					return;
				else
					accept();
			}
			else
			{
				QMessageBox::warning(this, CommonStrings::trWarning, tr("Name \"%1\" is not unique.\nPlease choose another.").arg(answerEdit->text()), CommonStrings::tr_OK);
				return;
			}
		}
		accept();
	}
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

void Query::setTestList(QStringList tList)
{
	checkList = tList;
}

void Query::setForbiddenList(QStringList tList)
{
	forbiddenList = tList;
}

void Query::setCheckMode(bool mode)
{
	checkMode = mode;
}

void Query::setValidator(QRegExp rx)
{
	QValidator* vaild = new QRegExpValidator( rx, this );
	answerEdit->setValidator(vaild);
}
