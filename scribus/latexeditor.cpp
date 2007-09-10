/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
						latexeditor.cpp  -  description
						-------------------
copyright            : Scribus Team
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/
#include "latexeditor.h"
#include "pageitem_latexframe.h"
#include "latexhighlighter.h"
#include <QDebug>

LatexEditor::LatexEditor(PageItem_LatexFrame *frame):QDialog(), frame(frame)
{
	setupUi(this);
	highlighter = new LatexHighlighter(sourceTextEdit->document());
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(okClicked()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancelClicked()));
	connect(updatePushButton, SIGNAL(clicked(bool)), this, SLOT(updateClicked(bool)));
	connect(revertPushButton, SIGNAL(clicked(bool)), this, SLOT(revertClicked(bool)));
	connect(killPushButton, SIGNAL(clicked(bool)), frame, SLOT(killProcess()));
	connect(updateAppSettingsPushButton, SIGNAL(clicked(bool)), this, SLOT(updateAppSettings(bool)));
	connect(frame, SIGNAL(formulaAutoUpdate(QString, QString)), this, SLOT(formulaChanged(QString, QString)));
	connect(frame, SIGNAL(latexFinished()), this, SLOT(latexFinished()));
	connect(frame, SIGNAL(stateChanged(QProcess::ProcessState)), 
			this, SLOT(stateChanged(QProcess::ProcessState)));
}

LatexEditor::~LatexEditor()
{
	buttonBox->disconnect();
	exitEditor();
	delete highlighter;
}

void LatexEditor::startEditor()
{
	revert();
	preambleCheckBox->setChecked(frame->getUsePreamble());
	dpiSpinBox->setValue(frame->getDpi());
	stateChanged(frame->getState());
	show();
}

void LatexEditor::exitEditor()
{
	hide();
}

void LatexEditor::revert()
{
	sourceTextEdit->setPlainText(frame->getFormula());
}

void LatexEditor::apply()
{
	if (frame->setFormula(sourceTextEdit->toPlainText())) {
		frame->rerunApplication(true);
	}
}

void LatexEditor::formulaChanged(QString oldText, QString newText)
{
	sourceTextEdit->setPlainText(newText);
}

void LatexEditor::okClicked()
{
	apply();
	exitEditor();
}

void LatexEditor::cancelClicked()
{
	revert();
	exitEditor();
}

void LatexEditor::revertClicked(bool unused)
{
	revert();
}

void LatexEditor::updateClicked(bool unused)
{
	apply();
}

void LatexEditor::latexFinished()
{
	messagesTextEdit->setPlainText(frame->getOutput());
}

void LatexEditor::stateChanged(QProcess::ProcessState state)
{
	if (state == QProcess::Starting) {
		messagesTextEdit->setPlainText("");
	}
	statusLabel->setText(state == QProcess::NotRunning?"Status: Not Running":"Status: Running");
	killPushButton->setEnabled(state != QProcess::NotRunning);
}

void LatexEditor::updateAppSettings(bool unused)
{
	frame->setUsePreamble(preambleCheckBox->isChecked());
	frame->setDpi(dpiSpinBox->value());
	frame->rerunApplication(true);
}
