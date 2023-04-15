/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QStringList>
#include <QVBoxLayout>

#include "annota.h"
#include "commonstrings.h"
#include "navigator.h"
#include "prefsmanager.h"
#include "prefsfile.h"
#include "pageitem.h"
#include "scfonts.h"
#include "scribusdoc.h"
#include "scribusview.h"
#include "util_formats.h"
#include "ui/customfdialog.h"
#include "ui/scrspinbox.h"

Annota::Annota(QWidget* parent, PageItem *it, ScribusDoc* doc, ScribusView* view) :
	QDialog(parent),
	m_item(it),
	m_doc(doc),
	m_view(view)
{
	setModal(true);
	setWindowTitle( tr( "Annotation Properties" ) );

	m_pageCount = m_doc->DocPages.count();
	m_width = static_cast<int>(m_doc->pageWidth());
	m_height = static_cast<int>(m_doc->pageHeight());
	m_oriWidth = static_cast<int>(m_doc->pageWidth());
	m_oriHeight = static_cast<int>(m_doc->pageHeight());

	QStringList tl;
	if ((m_item->annotation().ActionType() == Annotation::Action_GoTo) || (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileRel) || (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileAbs))
	{
		QString tm = m_item->annotation().Action();
		tl = tm.split(" ", Qt::SkipEmptyParts);
	}
	else
	{
		tl.append("0");
		tl.append(QString::number(m_height));
	}

	AnnotLayout = new QVBoxLayout( this );
	AnnotLayout->setSpacing(6);
	AnnotLayout->setContentsMargins(9, 9, 9, 9);

	Layout1 = new QHBoxLayout;
	Layout1->setSpacing(6);
	Layout1->setContentsMargins(0, 0, 0, 0);

	typeCombo = new QComboBox(this);
	if (!m_view->m_doc->masterPageMode())
		typeCombo->addItem( tr("Text"));
	typeCombo->addItem( tr("Link"));
	typeCombo->addItem( tr("External Link"));
	typeCombo->addItem( tr("External Web-Link"));
	typeCombo->setEditable(false);
	typeComboLabel = new QLabel( tr("&Type:"), this);
	typeComboLabel->setBuddy(typeCombo);
	Layout1->addWidget( typeComboLabel );
	Layout1->addWidget( typeCombo );
	AnnotLayout->addLayout( Layout1 );
	if (m_view->m_doc->masterPageMode())
	{
		if (m_item->annotation().Type() < 2)
			typeCombo->setCurrentIndex(m_item->annotation().Type() - 1);
		else
			typeCombo->setCurrentIndex(m_item->annotation().Type() - 11);
		if ((m_item->annotation().ActionType() == Annotation::Action_GoToR_FileRel) || (m_item->annotation().ActionType() == Annotation::Action_URI))
			typeCombo->setCurrentIndex(m_item->annotation().ActionType() - 6);
		if (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileAbs)
			typeCombo->setCurrentIndex(1);
	}
	else
	{
		if (m_item->annotation().Type() < 2)
			typeCombo->setCurrentIndex(m_item->annotation().Type());
		else
			typeCombo->setCurrentIndex(m_item->annotation().Type() - 10);
		if ((m_item->annotation().ActionType() == Annotation::Action_GoToR_FileRel) || (m_item->annotation().ActionType() == Annotation::Action_URI))
			typeCombo->setCurrentIndex(m_item->annotation().ActionType() - 5);
		if (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileAbs)
			typeCombo->setCurrentIndex(2);
	}
	Fram = new QStackedWidget(this);
	AnnotLayout->addWidget( Fram );

//	Frame9 = new QFrame( this );
//	Frame9->setFrameShape( QFrame::NoFrame );
//	Frame9->setFrameShadow( QFrame::Plain );

	GroupBox2 = new QGroupBox( this );
	GroupBox2->setTitle( tr( "Options" ) );
	GroupBox2Layout = new QGridLayout( GroupBox2 );
	GroupBox2Layout->setAlignment( Qt::AlignTop );
	GroupBox2Layout->setSpacing(6);
	GroupBox2Layout->setContentsMargins(9, 9, 9, 9);
	iconComboLabel = new QLabel( tr("Icon:"), GroupBox2);
	iconCombo = new QComboBox(GroupBox2);
	iconCombo->addItem( tr("Note"));
	iconCombo->addItem( tr("Comment"));
	iconCombo->addItem( tr("Key"));
	iconCombo->addItem( tr("Help"));
	iconCombo->addItem( tr("New Paragraph"));
	iconCombo->addItem( tr("Paragraph"));
	iconCombo->addItem( tr("Insert"));
	iconCombo->addItem( tr("Cross"));
	iconCombo->addItem( tr("Circle"));
	iconCombo->setEditable(false);
	iconCombo->setCurrentIndex(m_item->annotation().Icon());
	GroupBox2Layout->addWidget( iconComboLabel, 0, 0 );
	GroupBox2Layout->addWidget( iconCombo, 0, 1 );
	textIsOpen = new QCheckBox( tr("Annotation is Open"), GroupBox2);
	textIsOpen->setChecked(m_item->annotation().IsAnOpen());
	GroupBox2Layout->addWidget( textIsOpen, 1, 0, 1, 1 );
	Fram->addWidget(GroupBox2);

	GroupBox1 = new QGroupBox( this );
	GroupBox1->setTitle( tr( "Destination" ) );
	GroupBox1Layout = new QGridLayout( GroupBox1 );
	GroupBox1Layout->setAlignment( Qt::AlignTop );
	GroupBox1Layout->setSpacing(6);
	GroupBox1Layout->setContentsMargins(9, 9, 9, 9);

	destFile = new QLineEdit(GroupBox1);
	destFile->setText(m_item->annotation().Extern());
	destFile->setReadOnly(true);
	GroupBox1Layout->addWidget( destFile, 0, 0, 1, 2 );
	changeFile = new QPushButton(GroupBox1);
	changeFile->setText( tr("C&hange..."));
	GroupBox1Layout->addWidget( changeFile, 0, 2 );
	useAbsolute = new QCheckBox( tr("Export absolute Filename"), GroupBox1);
	GroupBox1Layout->addWidget( useAbsolute, 1, 0, 1, 3 );
	if (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileRel)
		useAbsolute->setChecked(false);
	else if (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileAbs)
		useAbsolute->setChecked(true);
	if ((m_item->annotation().ActionType() != Annotation::Action_GoToR_FileRel) && (m_item->annotation().ActionType() != Annotation::Action_URI) && (m_item->annotation().ActionType() != Annotation::Action_GoToR_FileAbs))
	{
		destFile->hide();
		changeFile->hide();
		useAbsolute->hide();
	}

	pageSpin = new ScrSpinBox( GroupBox1);
	pageSpin->setDecimals(0);
	pageSpin->setMinimum(1);
	pageSpin->setMaximum(((m_item->annotation().ActionType() == Annotation::Action_GoToR_FileRel) || (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileAbs)) ? 1000 : m_pageCount);
	pageSpin->setSuffix(QString());
	pageSpinLabel = new QLabel( tr("&Page:"), GroupBox1);
	pageSpinLabel->setBuddy(pageSpin);
	GroupBox1Layout->addWidget( pageSpinLabel, 2, 0 );
	GroupBox1Layout->addWidget( pageSpin, 2, 1 );
	if ((!destFile->text().isEmpty()) && ((m_item->annotation().ActionType() == Annotation::Action_GoToR_FileRel) || (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileAbs)))
		navigator = new Navigator( GroupBox1, 100, m_item->annotation().Ziel() + 1, m_view, m_item->annotation().Extern());
	else
	{
		int targetPage = m_item->annotation().Ziel();
		if ((targetPage >= 0) && (targetPage < m_view->m_doc->Pages->count()))
			navigator = new Navigator( GroupBox1, 100, m_item->annotation().Ziel(), m_view);
		else
		{
			m_item->annotation().setZiel(m_view->m_doc->currentPageNumber());
			navigator = new Navigator( GroupBox1, 100, m_item->annotation().Ziel(), m_view);
		}
	}
	pageSpin->setValue(m_item->annotation().Ziel() + 1);
	navigator->setMinimumSize(QSize(navigator->pmx.width(), navigator->pmx.height()));
	GroupBox1Layout->addWidget(navigator, 2, 2, 3, 1);

	xSpin = new ScrSpinBox( GroupBox1);
	xSpin->setDecimals(0);
	xSpin->setSuffix( tr( " pt" ) );
	xSpin->setMaximum(m_width);
	xSpin->setValue(tl[0].toInt());
	xSpinLabel = new QLabel( tr("&X-Pos:"), GroupBox1 );
	xSpinLabel->setBuddy(xSpin);
	GroupBox1Layout->addWidget( xSpinLabel, 3, 0 );
	GroupBox1Layout->addWidget( xSpin, 3, 1 );
	ySpin = new ScrSpinBox( GroupBox1 );
	ySpin->setDecimals(0);
	ySpin->setMaximum(m_height);
	ySpin->setSuffix( tr( " pt" ) );
	ySpin->setValue(m_height - tl[1].toInt());
	ySpinLabel = new QLabel( tr("&Y-Pos:"), GroupBox1 );
	ySpinLabel->setBuddy(ySpin);
	GroupBox1Layout->addWidget( ySpinLabel, 4, 0 );
	GroupBox1Layout->addWidget( ySpin, 4, 1 );
	Fram->addWidget(GroupBox1);

	Layout1_2 = new QHBoxLayout;
	Layout1_2->setSpacing(6);
	Layout1_2->setContentsMargins(0, 0, 0, 0);

	QSpacerItem* spacer = new QSpacerItem( 2, 2, QSizePolicy::Expanding, QSizePolicy::Minimum );
	Layout1_2->addItem( spacer );
	okButton = new QPushButton( CommonStrings::tr_OK, this );
	okButton->setDefault( true );
	Layout1_2->addWidget( okButton );
	cancelButton = new QPushButton( CommonStrings::tr_Cancel, this );
	Layout1_2->addWidget( cancelButton );
	AnnotLayout->addLayout( Layout1_2 );

	connect(okButton, SIGNAL(clicked()), this, SLOT(SetValues()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(SetTarget(int)));
	connect(pageSpin, SIGNAL(valueChanged(double)), this, SLOT(SetPage(double)));
	connect(navigator, SIGNAL(Coords(double, double)), this, SLOT(SetCoords(double, double)));
	connect(xSpin, SIGNAL(valueChanged(double)), this, SLOT(SetCross()));
	connect(ySpin, SIGNAL(valueChanged(double)), this, SLOT(SetCross()));
	connect(changeFile, SIGNAL(clicked()), this, SLOT(GetFile()));
	if (m_view->m_doc->masterPageMode())
		SetTarget(m_item->annotation().Type() - 1);
	else
		SetTarget(m_item->annotation().Type());
	SetCross();
}

void Annota::SetCoords(double x, double y)
{
	xSpin->setValue(static_cast<int>(x * m_width));
	ySpin->setValue(static_cast<int>(y * m_height));
}

void Annota::SetPage(double v)
{
	QSignalBlocker spinBox1Blocker(pageSpin);

	int link = m_view->m_doc->masterPageMode() ? 1 : 2;
	if (typeCombo->currentIndex() == link)
	{
		if (!navigator->setPage(static_cast<int>(v), 100, destFile->text()))
		{
			pageSpin->setValue(1);
			navigator->setPage(1, 100, destFile->text());
		}
		m_width = navigator->Width;
		m_height = navigator->Height;
	}
	else
	{
		navigator->setPage(qMin(static_cast<int>(v) - 1, m_pageCount - 1), 100);
		pageSpin->setValue(qMin(static_cast<int>(v), m_pageCount));
		m_width = m_oriWidth;
		m_height = m_oriHeight;
	}
	xSpin->setMaximum(m_width);
	ySpin->setMaximum(m_height);
}

void Annota::SetCross()
{
	disconnect(navigator, SIGNAL(Coords(double, double)), this, SLOT(SetCoords(double, double)));
	int x = static_cast<int>(xSpin->value() / static_cast<double>(m_width) * navigator->pmx.width());
	int y = static_cast<int>(ySpin->value() / static_cast<double>(m_height) * navigator->pmx.height());
	navigator->drawMark(x, y);
	connect(navigator, SIGNAL(Coords(double, double)), this, SLOT(SetCoords(double, double)));
}

void Annota::SetValues()
{
	QString tmp;
	QString tmp2;
	m_item->annotation().setZiel(pageSpin->value() - 1);
	if (m_view->m_doc->masterPageMode())
		m_item->annotation().setType(typeCombo->currentIndex() + 11);
	else
		m_item->annotation().setType(typeCombo->currentIndex() + 10);
	switch (m_item->annotation().Type())
	{
	case 10:
		m_item->annotation().setActionType(Annotation::Action_None);
		m_item->annotation().setAnOpen(textIsOpen->isChecked());
		m_item->annotation().setIcon(iconCombo->currentIndex());
		break;
	case 11:
		m_item->annotation().setAction(tmp.setNum(xSpin->value()) + " " + tmp2.setNum(m_height - ySpin->value()) + " 0");
		m_item->annotation().setExtern("");
		m_item->annotation().setActionType(Annotation::Action_GoTo);
		break;
	case 12:
		m_item->annotation().setAction(tmp.setNum(xSpin->value()) + " " + tmp2.setNum(m_height - ySpin->value()) + " 0");
		if (!destFile->text().isEmpty())
		{
			m_item->annotation().setExtern(destFile->text());
			if (useAbsolute->isChecked())
				m_item->annotation().setActionType(Annotation::Action_GoToR_FileAbs);
			else
				m_item->annotation().setActionType(Annotation::Action_GoToR_FileRel);
		}
		m_item->annotation().setType(Annotation::Link);
		break;
	case 13:
		m_item->annotation().setAction("");
		if (!destFile->text().isEmpty())
		{
			m_item->annotation().setExtern(destFile->text());
			m_item->annotation().setActionType(Annotation::Action_URI);
		}
		m_item->annotation().setType(Annotation::Link);
		break;
	}
	accept();
}

void Annota::SetTarget(int it)
{
	int a = it;
	disconnect(typeCombo, SIGNAL(activated(int)), this, SLOT(SetTarget(int)));
	if (m_view->m_doc->masterPageMode())
		a++;
	navigator->show();
	pageSpinLabel->show();
	xSpinLabel->show();
	ySpinLabel->show();
	pageSpin->show();
	xSpin->show();
	ySpin->show();
	switch (a)
	{
	case 1:
		Fram->setCurrentIndex(1);
		destFile->setText("");
		destFile->hide();
		changeFile->hide();
		useAbsolute->hide();
		m_item->annotation().setActionType(Annotation::Action_GoTo);
		SetPage(qMin(static_cast<int>(pageSpin->value()), m_pageCount));
		break;
	case 2:
		Fram->setCurrentIndex(1);
		destFile->show();
		changeFile->show();
		useAbsolute->show();
		destFile->setReadOnly(true);
		if ((destFile->text().isEmpty())  || (m_item->annotation().ActionType() == Annotation::Action_URI))
		{
			destFile->setText("");
			GetFile();
		}
		if (destFile->text().isEmpty())
		{
			m_item->annotation().setActionType(Annotation::Action_GoTo);
			destFile->setText("");
			destFile->hide();
			changeFile->hide();
			useAbsolute->hide();
			if (m_view->m_doc->masterPageMode())
				typeCombo->setCurrentIndex(0);
			else
				typeCombo->setCurrentIndex(1);
		}
		else
		{
			if (useAbsolute->isChecked())
				m_item->annotation().setActionType(Annotation::Action_GoToR_FileAbs);
			else
				m_item->annotation().setActionType(Annotation::Action_GoToR_FileRel);
		}
		SetPage(qMin(static_cast<int>(pageSpin->value()), m_pageCount));
		break;
	case 3:
		Fram->setCurrentIndex(1);
		destFile->show();
		destFile->setReadOnly(false);
		changeFile->hide();
		useAbsolute->hide();
		navigator->hide();
		pageSpinLabel->hide();
		xSpinLabel->hide();
		ySpinLabel->hide();
		pageSpin->hide();
		xSpin->hide();
		ySpin->hide();
		m_item->annotation().setActionType(Annotation::Action_URI);
		break;
	case 11:
		Fram->setCurrentIndex(1);
		if ((m_item->annotation().ActionType() == Annotation::Action_GoToR_FileRel) || (m_item->annotation().ActionType() == Annotation::Action_GoToR_FileAbs))
		{
			destFile->show();
			changeFile->show();
			useAbsolute->show();
			destFile->setReadOnly(true);
		}
		if (m_item->annotation().ActionType() == Annotation::Action_URI)
		{
			destFile->show();
			destFile->setReadOnly(false);
			changeFile->hide();
			useAbsolute->hide();
			navigator->hide();
			pageSpinLabel->hide();
			xSpinLabel->hide();
			ySpinLabel->hide();
			pageSpin->hide();
			xSpin->hide();
			ySpin->hide();
		}
		if (navigator->isVisible())
			SetPage(qMin(static_cast<int>(pageSpin->value()), m_pageCount));
		break;
	default:
		Fram->setCurrentIndex(0);
		break;
	}
	connect(typeCombo, SIGNAL(activated(int)), this, SLOT(SetTarget(int)));
}

void Annota::GetFile()
{
	PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
	QString wdir = dirs->get("annot_getfile", ".");

	CustomFDialog dia(this, wdir, tr("Open"), tr("%1;;All Files (*)").arg(FormatsManager::instance()->extensionsForFormat(FormatsManager::PDF)));
	if (!destFile->text().isEmpty())
		dia.setSelection(destFile->text());
	if (dia.exec() != QDialog::Accepted)
		return;

	QString fn = dia.selectedFile();
	if (fn.isEmpty())
		return;
	dirs->set("annot_getfile", fn.left(fn.lastIndexOf("/")));
	destFile->setText(fn);
	pageSpin->setValue(1);
	pageSpin->setMaximum(1000);
	SetPage(1);
}
