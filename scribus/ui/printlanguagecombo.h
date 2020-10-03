/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTLANGUAGECOMBO_H
#define PRINTLANGUAGECOMBO_H

#include <QComboBox>
#include <QEvent>

#include "scribusapi.h"
#include "scribusstructs.h"

/*!
    \class PrintLanguageCombo printlanguagecombo.h
    \brief The PrintLanguageCombo widget is a combo box for displaying print languages
*/
class SCRIBUS_API PrintLanguageCombo : public QComboBox
{
	Q_OBJECT
	
public:
	PrintLanguageCombo(QWidget* parent = nullptr);

	/**
	 * @brief Retrieve currently selected language
	 */
	PrintLanguage currentLanguage() const;

	/**
	 * @brief Add a print language to current combobox
	 */
	void addLanguage(PrintLanguage prnLanguage);

	/**
	 * @brief Find item index for specified print language
	 */
	int findLanguage(PrintLanguage prnLanguage) const;

	/**
	 * @brief Returns if combobox currently contains specified language
	 */
	bool hasLanguage(PrintLanguage prnLanguage) const;

	/**
	 * @brief Returns if PDF is currently available in combobox
	 */
	bool hasPDF() const;

	/**
	 * @brief Returns if Postscript 1, 2, or 3 is currently available in combobox
	 */
	bool hasPostscript() const;

	/**
	 * @brief Retrieve print language at specified item index
	 */
	PrintLanguage languageAt(int index) const;

	/**
	 * @brief Define current print language
	 */
	void setCurrentLanguage(PrintLanguage prnLanguage);
	
	/**
	 * @brief Set supported languages for specified printer
	 */
	void setupLanguages(const QString& printerName, bool toFile);

protected:
	/**
	 * @brief Process change events
	 */
	void changeEvent(QEvent* e) override;

	/**
	 * @brief Process language change event
	 */
	void languageChange();
};

#endif
