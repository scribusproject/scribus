#include "sxwdia.h"

#ifdef HAVE_XML

#include "sxwdia.moc"
#include <qtooltip.h>

extern QPixmap loadIcon(QString nam);

SxwDialog::SxwDialog(bool update) : QDialog(0, "sxwdia", true, 0)
{
	setCaption(tr("OO.o Writer Importer Options"));
	setIcon(loadIcon("AppIcon.png"));

	QBoxLayout* layout = new QVBoxLayout(this);

	QBoxLayout* hlayout = new QHBoxLayout(0, 5, 5, "hlayout");
	updateCheck = new QCheckBox(tr("Update paragraph styles"), this, "updateCheck");
	updateCheck->setChecked(update);
	QToolTip::add(updateCheck, tr("If there already exists a paragraph style with the same\n"
	                              "name than current OO.o document's paragraph should the style\n"
	                              "in Scribus be edited to match the one being imported\n"
	                              "or left untouched"));
	hlayout->addWidget(updateCheck);
	layout->addLayout(hlayout);

// 	QBoxLayout* dlayout = new QHBoxLayout(0, 5, 5, "dlayout");
// 	doNotAskCheck = new QCheckBox(tr("Do not ask again"), this, "doNotAskCheck");
// 	doNotAskCheck->setChecked(false);
// 	QToolTip::add(doNotAskCheck, tr("Should the importer always use currently\n"
// 	                                "set value when importing OO.o document and\n"
// 	                                "never ask your confirmation again"));
// 	dlayout->addStretch(10);
// 	dlayout->addWidget(doNotAskCheck);
// 	layout->addLayout(dlayout);

	QBoxLayout* blayout = new QHBoxLayout(0, 5, 5, "blayout");
	blayout->addStretch(10);
	okButton = new QPushButton(tr("OK"), this, "okButton");
	blayout->addWidget(okButton);
	layout->addLayout(blayout);

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

bool SxwDialog::shouldUpdate()
{
	return updateCheck->isChecked();
}

bool SxwDialog::askAgain()
{
	return !(doNotAskCheck->isChecked());
}

SxwDialog::~SxwDialog()
{

}

#endif // HAVE_XML
