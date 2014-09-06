/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "javadocs.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QRegExp>
#include <QTextEdit>
#include <QToolTip>

#include "commonstrings.h"
#include "editor.h"
#include "page.h"
#include "query.h"
#include "scribusdoc.h"
#include "util_icon.h"

JavaDocs::JavaDocs(QWidget* parent, ScribusDoc *doc, ScribusView* vie) : QDialog( parent )
{
	setModal(true);
	setWindowTitle( tr( "Edit JavaScripts" ) );
	setWindowIcon(loadIcon("AppIcon.png"));
	m_Doc = doc;
	m_View = vie;
	JavaDocsLayout = new QHBoxLayout(this);
	JavaDocsLayout->setMargin(10);
	JavaDocsLayout->setSpacing(5);

	Scripts = new QListWidget( this );
	Scripts->setMinimumSize( QSize( 150, 200 ) );
	QMap<QString,QString>::Iterator it;
	for (it = m_Doc->JavaScripts.begin(); it != m_Doc->JavaScripts.end(); ++it)
		Scripts->addItem(it.key());
	JavaDocsLayout->addWidget( Scripts );

	Layout1 = new QVBoxLayout;
	Layout1->setMargin(0);
	Layout1->setSpacing(5);

	EditScript = new QPushButton( tr( "&Edit..." ), this);
	Layout1->addWidget( EditScript );

	AddScript = new QPushButton( tr( "&Add..." ), this);
	Layout1->addWidget( AddScript );

	DeleteScript = new QPushButton( tr( "&Delete" ), this);
	Layout1->addWidget( DeleteScript );
	QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout1->addItem( spacer );

	ExitDia = new QPushButton( tr( "&Close" ), this);
	ExitDia->setDefault( true );
	Layout1->addWidget( ExitDia );
	if (m_Doc->JavaScripts.count() == 0)
	{
		EditScript->setEnabled(false);
		DeleteScript->setEnabled(false);
	}
	JavaDocsLayout->addLayout( Layout1 );
	connect(AddScript, SIGNAL(clicked()), this, SLOT(slotAdd()));
	connect(EditScript, SIGNAL(clicked()), this, SLOT(slotEdit()));
	connect(DeleteScript, SIGNAL(clicked()), this, SLOT(slotDelete()));
	connect(ExitDia, SIGNAL(clicked()), this, SLOT(accept()));
	connect(Scripts, SIGNAL(itemActivated (QListWidgetItem *)), this, SLOT(slotEdit()));
	connect(Scripts, SIGNAL(itemSelectionChanged()), this, SLOT(slotSelectionChanged()));
	AddScript->setToolTip( "<qt>" + tr( "Adds a new Script, predefines a function with the same name. If you want to use this script as an \"Open Action\" script be sure not to change the name of the function." ) + "</qt>" );
}

void JavaDocs::slotAdd()
{
	QString nam;
	Query dia(this, "tt", 1, 0, tr("&New Script:"), tr("New Script"));
	dia.setEditText( tr("New Script"), false );
	dia.setTestList(m_Doc->JavaScripts.keys());
	if (dia.exec())
	{
		nam = dia.getEditText();
		nam.replace( QRegExp("[\\s\\/\\{\\[\\]\\}\\<\\>\\(\\)\\%]"), "_" );
		Editor* dia2 = new Editor(this, "", m_View);
		dia2->EditTex->setText("function "+nam+"()\n{\n}");
		if (dia2->exec())
		{
			m_Doc->JavaScripts[nam] = dia2->EditTex->toPlainText();
			Scripts->addItem(nam);
			Scripts->setCurrentRow(Scripts->count() - 1);
			emit docChanged(false);
		}
		delete dia2;
	}
}

void JavaDocs::slotEdit()
{
	QListWidgetItem* currentItem = Scripts->currentItem();
	if (!currentItem)
		return;
	QString name = currentItem->text();
	Editor* dia2 = new Editor(this, m_Doc->JavaScripts[name], m_View);
	if (dia2->exec())
	{
		m_Doc->JavaScripts[name] = dia2->EditTex->toPlainText();
		emit docChanged(false);
	}
	delete dia2;
}

void JavaDocs::slotDelete()
{
	QListWidgetItem* currentItem = Scripts->currentItem();
	if (!currentItem)
		return;

	int exit = QMessageBox::warning(this,
	                               CommonStrings::trWarning,
	                               tr("Do you really want to delete this script?"),
	                               QMessageBox::Yes | QMessageBox::No);
	if (exit == QMessageBox::Yes)
	{
		QString name = currentItem->text();
		m_Doc->JavaScripts.remove(name);
		Scripts->clear();
		QMap<QString,QString>::Iterator it;
		for (it = m_Doc->JavaScripts.begin(); it != m_Doc->JavaScripts.end(); ++it)
			Scripts->addItem(it.key());
		if (m_Doc->JavaScripts.count() == 0)
		{
			EditScript->setEnabled(false);
			DeleteScript->setEnabled(false);
		}
		emit docChanged(false);
	}
}

void JavaDocs::slotSelectionChanged()
{
	QListWidgetItem* currentItem = Scripts->currentItem();
	EditScript->setEnabled(currentItem != 0);
	DeleteScript->setEnabled(currentItem != 0);
}