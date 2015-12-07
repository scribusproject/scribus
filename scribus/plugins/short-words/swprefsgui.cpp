/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "commonstrings.h"
#include "swprefsgui.h"
#include "swsyntaxhighlighter.h"
#include "version.h"
#include "scpaths.h"
#include "ui/scmessagebox.h"

#include <QDir>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QTextCodec>
#include <QTextEdit>
#include <QTextStream>
#include <QToolTip>

SWPrefsGui::SWPrefsGui(QWidget* parent ) : PrefsPanel(parent)
{
	m_SWPrefsGuiLayout = new QGridLayout(this);
	m_SWPrefsGuiLayout->setMargin(10);
	m_SWPrefsGuiLayout->setSpacing(5);

	m_editLayout = new QVBoxLayout;
	m_editLayout->setMargin(0);
	m_editLayout->setSpacing(5);

	titleLabel = new QLabel(this);
	m_editLayout->addWidget(titleLabel);
	cfgEdit = new QTextEdit(this);
	m_editLayout->addWidget(cfgEdit);

	m_buttonLayout = new QHBoxLayout;
	m_buttonLayout->setMargin(0);
	m_buttonLayout->setSpacing(5);
	m_buttonSpacer = new QSpacerItem(4, 2, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_buttonLayout->addItem(m_buttonSpacer);

	okButton = new QPushButton(this);
	m_buttonLayout->addWidget(okButton);

	resetButton = new QPushButton(this);
	m_buttonLayout->addWidget(resetButton);
	m_editLayout->addLayout(m_buttonLayout);

	m_SWPrefsGuiLayout->addLayout(m_editLayout, 0, 0);
	languageChange();
	resize(QSize(362, 359).expandedTo(minimumSizeHint()));

	// defaults
	if (QFile::exists(RC_PATH_USR))
	{
		titleLabel->setText( tr("User settings"));
		loadCfgFile(RC_PATH_USR);
	}
	else
	{
		titleLabel->setText( tr("System wide configuration"));
		resetButton->setEnabled(false);
		loadCfgFile(RC_PATH);
	}
	okButton->setEnabled(false);
	new SWSyntaxHighlighter(cfgEdit);

	// signals
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_pressed()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(resetButton_pressed()));
	connect(cfgEdit, SIGNAL(textChanged()), this, SLOT(cfgEdit_changed()));
}

SWPrefsGui::~SWPrefsGui()
{
	//delete the highlighter
//	delete cfgEdit->syntaxHighlighter();
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SWPrefsGui::languageChange()
{
	okButton->setText( tr("&Save"));
	resetButton->setText( tr("&Reset"));
	// tooltips
	okButton->setToolTip( tr("Save user configuration"));
	resetButton->setToolTip( "<qt>" + tr("Reload system wide configuration and remove user defined one") + "</qt>");
	cfgEdit->setToolTip( "<qt>" + tr("Edit custom configuration. If you save it, it will be used over system wide configuration.") + "</qt>");
}

void SWPrefsGui::apply()
{
	if (okButton->isEnabled())
		okButton_pressed();
}

void SWPrefsGui::okButton_pressed()
{
	if (cfgEdit->document()->isModified() && QFile::exists(RC_PATH_USR))
	{
		if ((ScMessageBox::warning(this, tr("Short Words"),
				"<qt>" + tr("User configuration exists already. "
						"Do you really want to overwrite it?") + "</qt>",
				QMessageBox::Yes|QMessageBox::No,
				QMessageBox::NoButton,	// GUI default
				QMessageBox::Yes)	// batch default
			) == QMessageBox::No)
			return;
	}

	QFile f(RC_PATH_USR);
	if (!f.open(QIODevice::WriteOnly))
	{
		ScMessageBox::warning(this, tr("Short Words"),
			 "<qt>" + tr("Cannot write file %1.").arg(RC_PATH_USR) + "</qt>");
	}
	QTextStream stream(&f);
	stream.setCodec("UTF-8");
	stream << cfgEdit->toPlainText();
	f.close();
	titleLabel->setText( tr("User settings saved"));
	okButton->setEnabled(false);
}

void SWPrefsGui::resetButton_pressed()
{
	loadCfgFile(RC_PATH);
	QDir d;
	d.remove(RC_PATH_USR);
	okButton->setEnabled(false);
	resetButton->setEnabled(false);
	titleLabel->setText( tr("System wide configuration reloaded"));
}

void SWPrefsGui::cfgEdit_changed()
{
	resetButton->setEnabled(true);
	okButton->setEnabled(true);
}

bool SWPrefsGui::loadCfgFile(QString filename)
{
	QFile f(filename);
	if (!f.open(QIODevice::ReadOnly))
	{
		titleLabel->setText( tr("Cannot open file %1").arg(f.fileName()));
		return false;
	}
	cfgEdit->clear();
	QTextStream stream(&f);
	stream.setCodec("UTF-8");
	while (!stream.atEnd())
		cfgEdit->append(stream.readLine());
	f.close();
	// it's a must to prevent "overwrite?" message box on saving prefs
	cfgEdit->document()->setModified(false);
	return true;
}

