#include "sxwdia.h"

#ifdef HAVE_XML

#include "sxwdia.moc"
#include <qtooltip.h>

extern QPixmap loadIcon(QString nam);

SxwDialog::SxwDialog(bool update, bool prefix) : QDialog(0, "sxwdia", true, 0)
{
	setCaption(tr("OpenOffice.org Writer Importer Options"));
	setIcon(loadIcon("AppIcon.png"));

	QBoxLayout* layout = new QVBoxLayout(this);

	QBoxLayout* hlayout = new QHBoxLayout(0, 5, 5, "hlayout");
	updateCheck = new QCheckBox(tr("Update paragraph styles"), this, "updateCheck");
	updateCheck->setChecked(update);
	QToolTip::add(updateCheck, tr("If a paragraph style already exists with the same name as the current\n"
	                              "OpenOffice.org document's paragraph, should the style in Scribus be\n"
	                              "edited to match the one being imported, or left untouched"));
	hlayout->addWidget(updateCheck);
	layout->addLayout(hlayout);

	QBoxLayout* playout = new QHBoxLayout(0, 5, 5, "playout");
	prefixCheck = new QCheckBox(tr("Use document name as a prefix for paragraph styles"), this, "prefixCheck");
	prefixCheck->setChecked(prefix);
	QToolTip::add(prefixCheck, tr("Should importer add the name of the document\n"
	                              "on front of the paragraph style name in Scribus"));
	playout->addWidget(prefixCheck);
	layout->addLayout(playout);

	QBoxLayout* dlayout = new QHBoxLayout(0, 5, 5, "dlayout");
	doNotAskCheck = new QCheckBox(tr("Do not ask again"), this, "doNotAskCheck");
	doNotAskCheck->setChecked(false);
	QToolTip::add(doNotAskCheck, tr("Should the importer always use currently\n"
	                                "set value when importing OpenOffice.org document and\n"
	                                "never ask your confirmation again"));
	dlayout->addStretch(10);
	dlayout->addWidget(doNotAskCheck);
	layout->addLayout(dlayout);

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

bool SxwDialog::usePrefix()
{
	return prefixCheck->isChecked();
}

bool SxwDialog::askAgain()
{
	return !(doNotAskCheck->isChecked());
}

SxwDialog::~SxwDialog()
{

}

#endif // HAVE_XML
