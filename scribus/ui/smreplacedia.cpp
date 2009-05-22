/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smreplacedia.h"

#include <QLabel>
#include <QComboBox>


SMRowWidget::SMRowWidget(const QString &toBeDeleted, QStringList replaceOptions, QWidget *parent)
: QWidget(parent)
{
	layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(5);
	deleteLabel = new QLabel(toBeDeleted, this);
	layout->addWidget(deleteLabel);
	optionsCombo = new QComboBox(this);
	optionsCombo->addItem( tr("No Style"));
	optionsCombo->addItems(replaceOptions);
	layout->addWidget(optionsCombo);
}

QString SMRowWidget::toBeDeleted()
{
	return deleteLabel->text();
}

QString SMRowWidget::replaceWith()
{
	return optionsCombo->currentIndex() == 0 ? "" : optionsCombo->currentText();
}

SMRowWidget::~SMRowWidget()
{
	delete deleteLabel;
	delete optionsCombo;
	delete layout;
}

/*************************************************************************/
/*************************************************************************/

SMReplaceDia::SMReplaceDia(const QStringList &toBeDeleted, const QStringList &replaceOptions, QWidget *parent) : QDialog(parent)
{
	setupUi(this);

	QStringList options;

	for (int i = 0; i < replaceOptions.count(); ++i)
	{
		if (!toBeDeleted.contains(replaceOptions[i]))
			options << replaceOptions[i];
	}

	layout = new QVBoxLayout(mainFrame);
	layout->setMargin(0);
	layout->setSpacing(5);
	headerLayout = new QHBoxLayout();
	headerLayout->setMargin(5);
	deleteHeader = new QLabel("<b>" + tr("Remove") + "</b>", mainFrame);
	optionsHeader = new QLabel("<b>" + tr("Replace with") + "</b>", mainFrame);
	headerLayout->addWidget(deleteHeader);
	headerLayout->addWidget(optionsHeader);
	layout->addLayout(headerLayout);

	for (int i = 0; i < toBeDeleted.count(); ++i)
	{
		SMRowWidget *tmp = new SMRowWidget(toBeDeleted[i], options, mainFrame);
		layout->addWidget(tmp);
		rowWidgets.append(tmp);
	}
	layout->addStretch(10);
}

QList<RemoveItem> SMReplaceDia::items()
{
	QList<RemoveItem> tmp;
	for (int i = 0; i < rowWidgets.count(); ++i)
	{
		QString s1 = rowWidgets.at(i)->toBeDeleted();
		QString s2 = rowWidgets.at(i)->replaceWith();
		tmp.append(RemoveItem(s1, s2));
	}
	return tmp;
}

SMReplaceDia::~SMReplaceDia()
{
	while(!rowWidgets.isEmpty())
		delete rowWidgets.takeFirst();
	delete optionsHeader;
	delete deleteHeader;
	delete headerLayout;
	delete layout;
}
