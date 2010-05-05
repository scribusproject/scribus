/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_documentinformation.h"
#include "prefsstructs.h"
#include "scribusdoc.h"

Prefs_DocumentInformation::Prefs_DocumentInformation(QWidget* parent, ScribusDoc* doc)
	: Prefs_Pane(parent)
{
	setupUi(this);
	languageChange();
}

Prefs_DocumentInformation::~Prefs_DocumentInformation()
{
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
	dateLineEdit->setText(prefsData->docInfo.date());
	typeLineEdit->setText(prefsData->docInfo.type());
	formatLineEdit->setText(prefsData->docInfo.format());
	identifierLineEdit->setText(prefsData->docInfo.ident());
	sourceLineEdit->setText(prefsData->docInfo.source());
	languageLineEdit->setText(prefsData->docInfo.langInfo());
	relationLineEdit->setText(prefsData->docInfo.relation());
	coverageLineEdit->setText(prefsData->docInfo.cover());
	rightsLineEdit->setText(prefsData->docInfo.rights());
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
	prefsData->docInfo.setDate(dateLineEdit->text());
	prefsData->docInfo.setType(typeLineEdit->text());
	prefsData->docInfo.setFormat(formatLineEdit->text());
	prefsData->docInfo.setIdent(identifierLineEdit->text());
	prefsData->docInfo.setSource(sourceLineEdit->text());
	prefsData->docInfo.setLangInfo(languageLineEdit->text());
	prefsData->docInfo.setRelation(relationLineEdit->text());
	prefsData->docInfo.setCover(coverageLineEdit->text());
	prefsData->docInfo.setRights(rightsLineEdit->text());
}

