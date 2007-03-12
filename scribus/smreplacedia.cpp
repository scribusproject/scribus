/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smreplacedia.h"
#include "smreplacedia.moc"

#include <qstring.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <q3ptrlist.h>
#include <qlayout.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3ValueList>
#include <Q3HBoxLayout>

SMRowWidget::SMRowWidget(const QString &toBeDeleted, QStringList replaceOptions, QWidget *parent)
: QWidget(parent, "SMRowWidget")
{
	layout = new Q3HBoxLayout(this, 0, 5, "layout");
	deleteLabel = new QLabel(toBeDeleted, this, "deleteLabel");
	layout->addWidget(deleteLabel);
	optionsCombo = new QComboBox(this, "optionsCombo");
	optionsCombo->insertStringList(replaceOptions);
	optionsCombo->insertItem( tr("No Style"), 0);
	layout->addWidget(optionsCombo);
}

QString SMRowWidget::toBeDeleted()
{
	return deleteLabel->text();
}

QString SMRowWidget::replaceWith()
{
	return optionsCombo->currentItem() == 0 ? "" : optionsCombo->currentText();
}

SMRowWidget::~SMRowWidget()
{
	delete deleteLabel;
	delete optionsCombo;
	delete layout;
}

/*************************************************************************/
/*************************************************************************/

SMReplaceDia::SMReplaceDia(const QStringList &toBeDeleted, const QStringList &replaceOptions) : SMReplaceDiaBase()
{
	rowWidgets.setAutoDelete(true);

	QStringList options;

	for (uint i = 0; i < replaceOptions.count(); ++i)
	{
		if (!toBeDeleted.contains(replaceOptions[i]))
			options << replaceOptions[i];
	}

	layout = new Q3VBoxLayout(mainFrame, 0, 5, "layout");
	headerLayout = new Q3HBoxLayout(layout, 5, "headerLayout");
	deleteHeader = new QLabel("<b>" + tr("Remove") + "</b>", mainFrame);
	optionsHeader = new QLabel("<b>" + tr("Replace with") + "</b>", mainFrame);
	headerLayout->addWidget(deleteHeader);
	headerLayout->addWidget(optionsHeader);

	for (uint i = 0; i < toBeDeleted.count(); ++i)
	{
		SMRowWidget *tmp = new SMRowWidget(toBeDeleted[i], options, mainFrame);
		layout->addWidget(tmp);
		rowWidgets.append(tmp);
	}

	layout->addStretch(10);
}

Q3ValueList<RemoveItem> SMReplaceDia::items()
{
	Q3ValueList<RemoveItem> tmp;
	for (uint i = 0; i < rowWidgets.count(); ++i)
	{
		QString s1 = rowWidgets.at(i)->toBeDeleted();
		QString s2 = rowWidgets.at(i)->replaceWith();
		tmp.append(RemoveItem(s1, s2));
	}
	return tmp;
}

SMReplaceDia::~SMReplaceDia()
{
	delete optionsHeader;
	delete deleteHeader;
	delete headerLayout;
	delete layout;
}
