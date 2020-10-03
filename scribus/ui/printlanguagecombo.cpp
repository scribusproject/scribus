/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <cassert>

#include "commonstrings.h"
#include "printlanguagecombo.h"
#include "scribusstructs.h"
#include "util_printer.h"

PrintLanguageCombo::PrintLanguageCombo(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);

	addItem(CommonStrings::trPostScript1, (int) PrintLanguage::PostScript1);
	addItem(CommonStrings::trPostScript2, (int) PrintLanguage::PostScript2);
	addItem(CommonStrings::trPostScript3, (int) PrintLanguage::PostScript3);

	setCurrentIndex(2);
}

void PrintLanguageCombo::addLanguage(PrintLanguage prnLanguage)
{
	if (prnLanguage == PrintLanguage::PostScript1)
		addItem(CommonStrings::trPostScript1, (int) PrintLanguage::PostScript1);
	else if (prnLanguage == PrintLanguage::PostScript2)
		addItem(CommonStrings::trPostScript2, (int) PrintLanguage::PostScript2);
	else if (prnLanguage == PrintLanguage::PostScript3)
		addItem(CommonStrings::trPostScript3, (int) PrintLanguage::PostScript3);
	else if (prnLanguage == PrintLanguage::WindowsGDI)
		addItem(CommonStrings::trWindowsGDI, (int) PrintLanguage::WindowsGDI);
	else if (prnLanguage == PrintLanguage::PDF)
		addItem(CommonStrings::trPDF, (int) PrintLanguage::PDF);
}

PrintLanguage PrintLanguageCombo::currentLanguage() const
{
	return static_cast<PrintLanguage>(this->currentData().toInt());
}

int PrintLanguageCombo::findLanguage(PrintLanguage prnLanguage) const
{
	for (int i = 0; i < this->count(); ++i)
	{
		PrintLanguage printLanguage = languageAt(i);
		if (printLanguage == prnLanguage)
			return i;
	}

	return -1;
}

bool PrintLanguageCombo::hasLanguage(PrintLanguage prnLanguage) const
{
	for (int i = 0; i < this->count(); ++i)
	{
		PrintLanguage printLanguage = languageAt(i);
		if (printLanguage == prnLanguage)
			return true;
	}

	return false;
}

bool PrintLanguageCombo::hasPDF() const
{
	for (int i = 0; i < this->count(); ++i)
	{
		PrintLanguage prnLanguage = languageAt(i);
		if (prnLanguage == PrintLanguage::PDF)
			return true;
	}

	return false;
}

bool PrintLanguageCombo::hasPostscript() const
{
	for (int i = 0; i < this->count(); ++i)
	{
		PrintLanguage prnLanguage = languageAt(i);
		if (prnLanguage == PrintLanguage::PostScript1)
			return true;
		if (prnLanguage == PrintLanguage::PostScript2)
			return true;
		if (prnLanguage == PrintLanguage::PostScript3)
			return true;
	}

	return false;
}

PrintLanguage PrintLanguageCombo::languageAt(int index) const
{
	assert(index >= 0 && index < this->count());
	return ((PrintLanguage) itemData(index).toInt());
}

void PrintLanguageCombo::setCurrentLanguage(PrintLanguage prnLanguage)
{
	int newIndex = findData((int) prnLanguage);
	if (newIndex >= 0)
		setCurrentIndex(newIndex);
}

void PrintLanguageCombo::setupLanguages(const QString& printerName, bool toFile)
{
	int index = 0, oldPDLIndex = -1;
	QString oldPDL  = this->currentText();

	clear();

	PrintLanguageMap printLanguageMap = PrinterUtil::getPrintLanguageSupport(printerName, toFile);
	if (printLanguageMap.count() <= 0)
	{
#if defined(Q_OS_WIN32)
		printLanguageMap.insert(CommonStrings::trWindowsGDI, PrintLanguage::WindowsGDI);
#else
		printLanguageMap.insert(CommonStrings::trPostScript3, PrintLanguage::PostScript3);
#endif
	}

	PrintLanguageMap::Iterator itEnd = printLanguageMap.end();
	for (auto it = printLanguageMap.begin(); it != itEnd; ++it)
	{
		addItem(it.key(), (int) it.value());
		if (it.key() == oldPDL)
			oldPDLIndex = index;
		index++;
	}

	// Try to not default on PostScript 1 when switching
	// from a GDI printer to a Postscript printer
	if (oldPDLIndex < 0)
		oldPDLIndex = findText(CommonStrings::trPostScript3);
	if (oldPDLIndex < 0)
	{
		PrintLanguage defaultPrintLang = PrinterUtil::getDefaultPrintLanguage(printerName, toFile);
		oldPDLIndex = findData((int) defaultPrintLang);
	}
	if (oldPDLIndex < 0)
		oldPDLIndex = 0;
	setCurrentIndex(oldPDLIndex);
}

void PrintLanguageCombo::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	
	QWidget::changeEvent(e);
}

void PrintLanguageCombo::languageChange()
{
	for (int i = 0; i < this->count(); ++i)
	{
		PrintLanguage prnLanguage = languageAt(i);
		if (prnLanguage == PrintLanguage::PostScript1)
			setItemText(i, CommonStrings::trPostScript1);
		if (prnLanguage == PrintLanguage::PostScript2)
			setItemText(i, CommonStrings::trPostScript2);
		if (prnLanguage == PrintLanguage::PostScript3)
			setItemText(i, CommonStrings::trPostScript3);
		if (prnLanguage == PrintLanguage::WindowsGDI)
			setItemText(i, CommonStrings::trWindowsGDI);
		if (prnLanguage == PrintLanguage::PDF)
			setItemText(i, CommonStrings::trPDF);
	}
}