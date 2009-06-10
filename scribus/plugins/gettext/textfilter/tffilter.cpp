/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "tffilter.h"

#include <QHBoxLayout>
#include <QFrame>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QToolTip>
#include <QCheckBox>
#include "scribusapi.h"
#include "scribuscore.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "util_icon.h"
#include "util.h"

tfFilter::tfFilter(QWidget *parent, const char *name,
                   int action, QString regExp, QString replace, QString pstyleName,
                   int less, int more, int style, bool match, bool enabled, bool regexp)
                   : QWidget(parent)
{
	createWidget();
	setObjectName(name);
	firstChanged(action);
	currentAction = action;
	firstCombo->setCurrentIndex(action);
	if (action == APPLY)
	{
		setCurrentComboItem(thirdCombo, pstyleName);
		fourthChanged(style);
		fourthCombo->setCurrentIndex(style);
		if (style == STARTS_WITH)
		{
			fifthCombo->setEditText(regExp);
			fifthRegexpCheck->setChecked(regexp);
			if (match)
				sixthCombo->setCurrentIndex(0);
			else
				sixthCombo->setCurrentIndex(1);
		}
		else if (style == LESS_THAN)
		{
			if (less > 0)
				fifthCombo->setEditText(QString("%1").arg(less));
		}
		else if (style == MORE_THAN)
		{
			if (more > 0)
				fifthCombo->setEditText(QString("%1").arg(more));
		}
	}
	else if (action == REMOVE) 
	{
		secondCombo->setEditText(regExp);
		secondRegexpCheck->setChecked(regexp);
	}
	else if (action == REPLACE)
	{
		secondCombo->setEditText(regExp);
		secondRegexpCheck->setChecked(regexp);
		thirdCombo->setEditText(replace);
	}
	enableCheck->setChecked(enabled);
	enableToggled(enabled);
}

tfFilter::tfFilter(QWidget *parent, const char *name) : QWidget(parent)
{
	createWidget();
	setObjectName(name);
}

void tfFilter::createWidget()
{
	firstCombo = NULL;
	firstLabel = NULL;
	secondCombo = NULL;
	secondLabel = NULL;
	thirdCombo = NULL;
	thirdLabel = NULL;
	fourthCombo = NULL;
	fourthLabel = NULL;
	fifthCombo = NULL;
	fifthLabel = NULL;
	sixthCombo = NULL;
	secondRegexpCheck = NULL;
// 	thirdRegexpCheck = NULL;
	fifthRegexpCheck = NULL;

	prefs = PrefsManager::instance()->prefsFile->getPluginContext("TextFilter");
	history = prefs->getTable("history");

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);

	enableCheck = new QCheckBox(this);
	enableCheck->setMaximumSize(QSize(25,25));
	enableCheck->setMinimumSize(QSize(25,25));
	enableCheck->setChecked(true);
	enableCheck->setToolTip( tr("Disable or enable this filter row"));
	layout->addWidget(enableCheck);

	actionFrame = new QFrame(this);
	layout->addWidget(actionFrame);

	QBoxLayout* layout2 = new QVBoxLayout(actionFrame);
	layout2->setMargin(0);
	layout2->setSpacing(0);
	alayout = new QHBoxLayout();
	alayout->setMargin(0);
	alayout->setSpacing(0);
	layout2->addLayout(alayout);
	layout2->addSpacing(4);
	blayout = new QHBoxLayout();
	blayout->setSpacing(0);
	blayout->setMargin(0);
	layout2->addLayout(blayout);

	currentAction = REMOVE;
	getFirstCombo();

// 	layout->addStretch(10);

	layout->addSpacing(20);
	removeButton = new QPushButton(loadIcon("22/list-remove.png"), 0, this);
	removeButton->setToolTip( tr("Remove this filter row"));
	removeButton->setMaximumSize(QSize(25,25));
	removeButton->setMinimumSize(QSize(25,25));
	layout->addWidget(removeButton);
	addButton = new QPushButton(loadIcon("22/list-add.png"), 0, this);
	addButton->setToolTip( tr("Add a new filter row"));
	addButton->setMaximumSize(QSize(25,25));
	addButton->setMinimumSize(QSize(25,25));
	layout->addWidget(addButton);

	connect(enableCheck, SIGNAL(toggled(bool)), this, SLOT(enableToggled(bool)));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addClick()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(removeClick()));
}

void tfFilter::setRemovable(bool b)
{
	removeButton->setEnabled(b);
}

void tfFilter::enableToggled(bool on)
{
	actionFrame->setEnabled(on);
}

void tfFilter::addClick()
{
	emit addClicked(this);
}

void tfFilter::removeClick()
{
	emit removeClicked(this);
}

void tfFilter::firstChanged(int index)
{
	currentAction = index;
	getSecondCombo();
}

void tfFilter::secondChanged(int)
{

}

void tfFilter::thirdChanged(int)
{

}

void tfFilter::fourthChanged(int index)
{
	switch (currentAction)
	{
		case APPLY:
			thirdLabel->setText( tr("to"));
			thirdLabel->show();
// 			thirdRegexpCheck->hide();
			switch (index)
			{
				case ALL_PARAGRAPHS:
					fourthLabel->hide();
					fifthCombo->hide();
					fifthLabel->hide();
					sixthCombo->hide();
					fifthRegexpCheck->hide();
					break;
				case STARTS_WITH:
					fourthLabel->hide();
					fifthCombo->clear();
					fifthCombo->setEditable(true);
					fifthCombo->show();
					fifthRegexpCheck->show();
					fifthLabel->setText( tr("and"));
					fifthLabel->show();
					sixthCombo->clear();
					sixthCombo->setEditable(false);
					sixthCombo->addItem( tr("remove match"));
					sixthCombo->addItem( tr("do not remove match"));
					sixthCombo->show();
					break;
				case LESS_THAN:
				case MORE_THAN:
					fourthLabel->hide();
					fifthCombo->clear();
					fifthCombo->setEditable(true);
					fifthCombo->show();
					fifthRegexpCheck->hide();
					fifthLabel->setText( tr("words"));
					fifthLabel->show();
					sixthCombo->hide();
					break;
			}
			break;
	}
}

void tfFilter::fifthChanged(int)
{

}

void tfFilter::sixthChanged(int)
{

}

void tfFilter::getFirstCombo()
{
	resetBRow();
	if (!firstCombo)
	{
		firstCombo = new QComboBox(actionFrame);
		firstCombo->addItem("");
		firstCombo->show();
		alayout->addWidget(firstCombo, -1);
		alayout->setSpacing(5);
		connect(firstCombo, SIGNAL(activated(int)), this, SLOT(firstChanged(int)));
	}
	if (!firstLabel)
	{
		firstLabel = new QLabel(actionFrame);
		alayout->addWidget(firstLabel, -1);
		firstLabel->hide();
	}
	firstCombo->clear();
	firstCombo->setMinimumSize(QSize(120, 0));
	firstCombo->addItem( tr("Remove"));
	firstCombo->addItem( tr("Replace"));
	firstCombo->addItem( tr("Apply"));
	firstCombo->show();
	getSecondCombo();
}

void tfFilter::getSecondCombo()
{
	resetBRow();
	if (!secondCombo)
	{
		secondCombo = new QComboBox(actionFrame);
		secondCombo->addItem("");
		secondCombo->show();
		alayout->addWidget(secondCombo, 8);
		connect(secondCombo, SIGNAL(activated(int)), this, SLOT(secondChanged(int)));
	}
	if (!secondRegexpCheck)
	{
		secondRegexpCheck = new QCheckBox(actionFrame);
		secondRegexpCheck->setToolTip( tr("Value at the left is a regular expression"));
		secondRegexpCheck->show();
		alayout->addWidget(secondRegexpCheck, -1);
	}
	if (!secondLabel)
	{
		secondLabel = new QLabel(actionFrame);
		secondLabel->hide();
		alayout->addWidget(secondLabel, -1);
	}
	switch (currentAction)
	{
		case REPLACE:
			firstLabel->hide();
			secondLabel->setText( tr("with"));
			secondLabel->show();
			secondCombo->setEditable(true);
			secondCombo->clear();
			secondCombo->show();
			secondRegexpCheck->show();
			break;
		case APPLY:
			firstLabel->hide();
			secondLabel->hide();
			secondCombo->setEditable(false);
			secondCombo->clear();
			secondCombo->addItem( tr("paragraph style"));
			secondRegexpCheck->hide();
			break;
		case REMOVE:
			firstLabel->setText( tr("all instances of"));
			firstLabel->show();
			secondCombo->clear();
			secondCombo->setEditable(true);
			secondCombo->show();
			secondLabel->hide();
			secondRegexpCheck->show();
			break;
	}
	getThirdCombo(secondCombo->currentIndex());
}

void tfFilter::getThirdCombo(int)
{
	if (!thirdCombo)
	{
		thirdCombo = new QComboBox(actionFrame);
		thirdCombo->addItem("");
		thirdCombo->hide();
		alayout->addWidget(thirdCombo, 8);
		connect(thirdCombo, SIGNAL(activated(int)), this, SLOT(thirdChanged(int)));
	}
// 	if (!thirdRegexpCheck)
// 	{
// 		thirdRegexpCheck = new QCheckBox(actionFrame, "secondRegexpCheck");
// 		thirdRegexpCheck->setToolTip( tr("Value is treated as a regular expression"));
// 		thirdRegexpCheck->hide();
// 		alayout->addWidget(thirdRegexpCheck, -1);
// 	}
	if (!thirdLabel)
	{
		thirdLabel = new QLabel(actionFrame);
		thirdLabel->hide();
		blayout->addWidget(thirdLabel, -1);
		blayout->addSpacing(5);
	}
	switch (currentAction)
	{
		case REPLACE:
			thirdCombo->clear();
			thirdCombo->setEditable(true);
			thirdCombo->show();
// 			thirdRegexpCheck->show();
			break;
		case APPLY:
			thirdCombo->clear();
			getParagraphStyles();
			thirdCombo->setEditable(true);
			thirdCombo->show();
// 			thirdRegexpCheck->hide();
			getFourthCombo();
			break;
		case REMOVE:
			thirdCombo->hide();
// 			thirdRegexpCheck->hide();
			break;
	}
}

void tfFilter::getFourthCombo()
{
	if (!fourthCombo)
	{
		fourthCombo = new QComboBox(actionFrame);
		fourthCombo->addItem("");
		fourthCombo->hide();
		blayout->addWidget(fourthCombo, 8);
		connect(fourthCombo, SIGNAL(activated(int)), this, SLOT(fourthChanged(int)));
	}
	if (!fourthLabel)
	{
		fourthLabel = new QLabel(actionFrame);
		fourthLabel->hide();
		blayout->addWidget(fourthLabel, -1);
		blayout->addSpacing(5);
	}
	switch (currentAction)
	{
		case APPLY:
			thirdLabel->setText( tr("to"));
			thirdLabel->show();
			fourthCombo->clear();
			fourthCombo->addItem( tr("all paragraphs"));
			fourthCombo->addItem( tr("paragraphs starting with"));
			fourthCombo->addItem( tr("paragraphs with less than"));
			fourthCombo->addItem( tr("paragraphs with more than"));
			fourthCombo->setEditable(false);
			fourthCombo->show();
			fourthLabel->hide();
			getFifthCombo();
			break;
	}
}

void tfFilter::getFifthCombo()
{
	if (!fifthCombo)
	{
		fifthCombo = new QComboBox(actionFrame);
		fifthCombo->addItem("");
		fifthCombo->hide();
		blayout->addWidget(fifthCombo, 8);
		blayout->addSpacing(5);
		connect(fifthCombo, SIGNAL(activated(int)), this, SLOT(fifthChanged(int)));
	}
	if (!fifthRegexpCheck)
	{
		fifthRegexpCheck = new QCheckBox(actionFrame);
		fifthRegexpCheck->setToolTip( tr("Value at the left is a regular expression"));
		fifthRegexpCheck->hide();
		blayout->addWidget(fifthRegexpCheck, -1);
		blayout->addSpacing(5);
	}
	if (!fifthLabel)
	{
		fifthLabel = new QLabel(actionFrame);
		fifthLabel->hide();
		blayout->addWidget(fifthLabel, -1);
		blayout->addSpacing(5);
	}
	getSixthCombo();
}

void tfFilter::getSixthCombo()
{
	if (!sixthCombo)
	{
		sixthCombo = new QComboBox(actionFrame);
		sixthCombo->addItem("");
		sixthCombo->hide();
		blayout->addWidget(sixthCombo, 7);
		connect(sixthCombo, SIGNAL(activated(int)), this, SLOT(sixthChanged(int)));
	}
}

void tfFilter::resetBRow()
{
	if (thirdLabel)
	{
		thirdLabel->hide();
		thirdLabel->setText("");
	}
	if (fourthCombo)
	{
		fourthCombo->hide();
		fourthCombo->clear();
	}
	if (fourthLabel)
	{
		fourthLabel->hide();
		fourthLabel->setText("");
	}
	if (fifthCombo)
	{
		fifthCombo->hide();
		fifthCombo->clear();
	}
	if (fifthRegexpCheck)
	{
		fifthRegexpCheck->hide();
	}
	if (fifthLabel)
	{
		fifthLabel->hide();
		fifthLabel->setText("");
	}
	if (sixthCombo)
	{
		sixthCombo->hide();
		sixthCombo->clear();
	}
}

void tfFilter::getParagraphStyles()
{
	thirdCombo->addItem("");
	for (int i = 0; i < ScCore->primaryMainWindow()->doc->paragraphStyles().count(); ++i)
	{
		thirdCombo->addItem(ScCore->primaryMainWindow()->doc->paragraphStyles()[i].name());
	}
}

int tfFilter::getAction()
{
	return currentAction;
}

QString tfFilter::regExp()
{
	switch (currentAction)
	{
		case REMOVE:
		case REPLACE:
			if (!secondCombo)
				return "";
			return QString(secondCombo->currentText());
			break;
		case APPLY:
			if (!fifthCombo)
				return "";
			return QString(fifthCombo->currentText());
			break;
		default:
			return "";
			break;
	}
	return "";
}

QString tfFilter::replaceWith()
{
	if (!thirdCombo)
		return "";
	return QString(thirdCombo->currentText());
}

QString tfFilter::getPStyleName()
{
	if (!thirdCombo)
		return "";
	return QString(thirdCombo->currentText());
}

int tfFilter::getLessThan()
{
	if (!fifthCombo)
		return -1;
	bool ok = false;
	QString text = QString(fifthCombo->currentText());
	int i = text.toInt(&ok);
	if (ok)
		return i;
	else
		return -1;
}

int tfFilter::getStyle()
{
	if (!fourthCombo)
		return 0;
	return fourthCombo->currentIndex();
}

bool tfFilter::removeMatch()
{
	if (!sixthCombo)
		return false;
	return sixthCombo->currentIndex() == 0;
}

int tfFilter::getMoreThan()
{
	return getLessThan();
}

bool tfFilter::isEnabled()
{
	return enableCheck->isChecked();
}

bool tfFilter::isRegExp()
{
	bool checked = false;
	if ((currentAction == REPLACE) || (currentAction == REMOVE))
		checked = secondRegexpCheck->isChecked();
	else if (currentAction == APPLY)
		checked = fifthRegexpCheck->isChecked();
	return checked;
}

tfFilter::~tfFilter()
{

}
