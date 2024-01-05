/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QDateTime>
#include <QLocale>

#include "prefs_documentinformation.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_DocumentInformation::Prefs_DocumentInformation(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	scrollArea->viewport()->setAutoFillBackground(false);
	scrollArea->widget()->setAutoFillBackground(false);

	languageChange();

	m_caption = tr("Document Information");
	m_icon = "documentinfo.png";

	dateLineEdit->addItem(QDateTime::currentDateTime().toString("yyyy-MM-dd"));

	// DCT1 from https://www.dublincore.org/specifications/dublin-core/resource-typelist/
	typeLineEdit->addItem("Collection");
	typeLineEdit->addItem("Dataset");
	typeLineEdit->addItem("Event");
	typeLineEdit->addItem("Image");
	typeLineEdit->addItem("InteractiveResource");
	typeLineEdit->addItem("Model");
	typeLineEdit->addItem("Party");
	typeLineEdit->addItem("PhysicalObject");
	typeLineEdit->addItem("Place");
	typeLineEdit->addItem("Service");
	typeLineEdit->addItem("Software");
	typeLineEdit->addItem("Sound");
	typeLineEdit->addItem("Text");

	auto allLocales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

	QStringList languages;
	for (const QLocale &locale : allLocales)
		languages << locale.bcp47Name();
	languages.sort();
	languages.removeDuplicates();
	for (const auto& language : languages)
		languageLineEdit->addItem(language);

	QStringList countries;
	for (const QLocale &locale : allLocales)
		countries << QLocale::countryToString(locale.country());
	countries.sort();
	countries.removeDuplicates();
	coverageLineEdit->addItem("Worldwide");
	for (const auto& country : countries)
		coverageLineEdit->addItem(country);

	rightsLineEdit->addItem("All Rights Reserved");
	rightsLineEdit->addItem("Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International");
	rightsLineEdit->addItem("Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International");
	rightsLineEdit->addItem("Creative Commons Attribution-NonCommercial 4.0 International");
	rightsLineEdit->addItem("Creative Commons Attribution-NoDerivatives 4.0 International");
	rightsLineEdit->addItem("Creative Commons Attribution-ShareAlike 4.0 International");
	rightsLineEdit->addItem("Creative Commons Attribution 4.0 International");
	rightsLineEdit->addItem("Creative Commons Zero 1.0 Universal Public Domain Dedication");
}

void Prefs_DocumentInformation::languageChange()
{
}

void Prefs_DocumentInformation::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	titleLineEdit->setText(prefsData->docInfo.title());
	authorLineEdit->setText(prefsData->docInfo.author());
	subjectLineEdit->setText(prefsData->docInfo.subject());
	keywordsTextEdit->setText(prefsData->docInfo.keywords());
	descriptionTextEdit->setText(prefsData->docInfo.comments());
	publisherLineEdit->setText(prefsData->docInfo.publisher());
	contributorsTextEdit->setText(prefsData->docInfo.contrib());
	dateLineEdit->setEditText(prefsData->docInfo.date());
	typeLineEdit->setEditText(prefsData->docInfo.type());
	formatLineEdit->setText(prefsData->docInfo.format());
	identifierLineEdit->setText(prefsData->docInfo.ident());
	sourceLineEdit->setText(prefsData->docInfo.source());
	languageLineEdit->setEditText(prefsData->docInfo.langInfo());
	relationLineEdit->setText(prefsData->docInfo.relation());
	coverageLineEdit->setEditText(prefsData->docInfo.cover());
	rightsLineEdit->setEditText(prefsData->docInfo.rights());
}

void Prefs_DocumentInformation::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	prefsData->docInfo.setTitle(titleLineEdit->text());
	prefsData->docInfo.setAuthor(authorLineEdit->text());
	prefsData->docInfo.setSubject(subjectLineEdit->text());
	prefsData->docInfo.setKeywords(keywordsTextEdit->toPlainText());
	prefsData->docInfo.setComments(descriptionTextEdit->toPlainText());
	prefsData->docInfo.setPublisher(publisherLineEdit->text());
	prefsData->docInfo.setContrib(contributorsTextEdit->toPlainText());
	prefsData->docInfo.setDate(dateLineEdit->currentText());
	prefsData->docInfo.setType(typeLineEdit->currentText());
	prefsData->docInfo.setFormat(formatLineEdit->text());
	prefsData->docInfo.setIdent(identifierLineEdit->text());
	prefsData->docInfo.setSource(sourceLineEdit->text());
	prefsData->docInfo.setLangInfo(languageLineEdit->currentText());
	prefsData->docInfo.setRelation(relationLineEdit->text());
	prefsData->docInfo.setCover(coverageLineEdit->currentText());
	prefsData->docInfo.setRights(rightsLineEdit->currentText());
}

