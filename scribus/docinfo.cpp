/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QPixmap>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QToolTip>

#include "docinfo.h"
#include "documentinformation.h"
#include "util_icon.h"

/*
 *  Constructs a DocInfos which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DocInfos::DocInfos( QWidget* parent, DocumentInformation& docInfo ) : QTabWidget( parent )
{
	infos = docInfo;
	setMaximumSize( QSize( 32767, 32767 ) );
	setWindowTitle( tr( "Document Information" ) );
	setWindowIcon(QIcon(loadIcon ( "AppIcon.png" )));

	page1 = new QWidget( this );
	layout1 = new QGridLayout(page1);
	layout1->setSpacing( 5 );
	layout1->setMargin( 5 );

	titleEdit = new QLineEdit( page1, "titleEdit" );
	titleLabel = new QLabel( titleEdit, tr("&Title:") , page1, "titleLabel");
	titleLabel->resize( titleLabel->sizeHint() );
	layout1->addWidget( titleLabel, 0, 0 );
	layout1->addWidget( titleEdit, 0, 1 );

	authorEdit = new QLineEdit( page1, "lineEditAuthor" );
	authorLabel = new QLabel( authorEdit, tr("&Author:"), page1, "authorLabel" );
	authorLabel->resize( authorLabel->sizeHint() );
	layout1->addWidget( authorLabel, 1, 0 );
	layout1->addWidget( authorEdit, 1, 1 );

	keywordsEdit = new QTextEdit( page1, "keywordsEdit" );
	keywordsLabel = new QLabel( keywordsEdit, tr("&Keywords:"), page1, "keywordsLabel" );
	keywordsLabel->setAlignment( static_cast<int>( Qt::AlignTop | Qt::AlignLeft ) );
	keywordsEdit->setMinimumSize(QSize(200, 105));
	layout1->addWidget( keywordsLabel, 2, 0 );
	layout1->addWidget( keywordsEdit, 2, 1 );

	descriptionEdit = new QTextEdit( page1, "descriptionEdit" );
	descriptionLabel = new QLabel( descriptionEdit, tr("Descri&ption:"), page1, "descriptionLabel" );
	descriptionLabel->setAlignment( static_cast<int>( Qt::AlignTop | Qt::AlignLeft ) );
	descriptionEdit->setMinimumSize(QSize(200, 105));
	layout1->addWidget( descriptionLabel, 3, 0 );
	layout1->addWidget( descriptionEdit, 3, 1 );

	page2 = new QWidget( this );
	layout2 = new QGridLayout(page2);
	layout2->setSpacing( 5 );
	layout2->setMargin( 5 );

	publisherEdit = new QLineEdit( page2, "publisherEdit" );
	publisherLabel = new QLabel( publisherEdit, tr("P&ublisher:") , page2, "publisherLabel");
	publisherLabel->resize( publisherLabel->sizeHint() );
	layout2->addWidget( publisherLabel, 0, 0 );
	layout2->addWidget( publisherEdit, 0, 1 );

	contributorsEdit = new QTextEdit( page2, "contributorsEdit" );
	contributorsLabel = new QLabel( contributorsEdit, tr("&Contributors:"), page2, "contributorsLabel" );
	contributorsLabel->setAlignment( static_cast<int>( Qt::AlignTop | Qt::AlignLeft ) );
	contributorsEdit->setMinimumSize(QSize(200, 105));
	layout2->addWidget( contributorsLabel, 1, 0 );
	layout2->addWidget( contributorsEdit, 1, 1 );

	dateEdit = new QLineEdit( page2, "dateEdit" );
	dateLabel = new QLabel( dateEdit, tr("Dat&e:") , page2, "dateLabel");
	dateLabel->resize( dateLabel->sizeHint() );
	layout2->addWidget( dateLabel, 2, 0 );
	layout2->addWidget( dateEdit, 2, 1 );

	typeEdit = new QLineEdit( page2, "typeEdit" );
	typeLabel = new QLabel( typeEdit, tr("T&ype:") , page2, "typeLabel");
	typeLabel->resize( typeLabel->sizeHint() );
	layout2->addWidget( typeLabel, 3, 0 );
	layout2->addWidget( typeEdit, 3, 1 );

	formatEdit = new QLineEdit( page2, "formatEdit" );
	formatLabel = new QLabel( formatEdit, tr("F&ormat:") , page2, "formatLabel");
	formatLabel->resize( formatLabel->sizeHint() );
	layout2->addWidget( formatLabel, 4, 0 );
	layout2->addWidget( formatEdit, 4, 1 );

	identifierEdit = new QLineEdit( page2, "identifierEdit" );
	identifierLabel = new QLabel( identifierEdit, tr("Identi&fier:") , page2, "identifierLabel");
	identifierLabel->resize( identifierLabel->sizeHint() );
	layout2->addWidget( identifierLabel, 5, 0 );
	layout2->addWidget( identifierEdit, 5, 1 );

	sourceEdit = new QLineEdit( page2, "sourceEdit" );
	sourceLabel = new QLabel( sourceEdit, tr("&Source:") , page2, "sourceLabel");
	sourceLabel->resize( sourceLabel->sizeHint() );
	layout2->addWidget( sourceLabel, 6, 0 );
	layout2->addWidget( sourceEdit, 6, 1 );

	languageEdit = new QLineEdit( page2, "languageEdit" );
	languageLabel = new QLabel( languageEdit, tr("&Language:") , page2, "languageLabel");
	languageLabel->resize( languageLabel->sizeHint() );
	layout2->addWidget( languageLabel, 7, 0 );
	layout2->addWidget( languageEdit, 7, 1 );

	relationEdit = new QLineEdit( page2, "relationEdit" );
	relationLabel = new QLabel( relationEdit, tr("&Relation:") , page2, "relationLabel");
	relationLabel->resize( relationLabel->sizeHint() );
	layout2->addWidget( relationLabel, 8, 0 );
	layout2->addWidget( relationEdit, 8, 1 );

	coverageEdit = new QLineEdit( page2, "coverageEdit" );
	coverageLabel = new QLabel( coverageEdit, tr("Co&verage:") , page2, "coverageLabel");
	coverageLabel->resize( coverageLabel->sizeHint() );
	layout2->addWidget( coverageLabel, 9, 0 );
	layout2->addWidget( coverageEdit, 9, 1 );

	rightsEdit = new QLineEdit( page2, "rightsEdit" );
	rightsLabel = new QLabel( rightsEdit, tr("Ri&ghts:") , page2, "rightsLabel");
	rightsLabel->resize( rightsLabel->sizeHint() );
	layout2->addWidget( rightsLabel, 10, 0 );
	layout2->addWidget( rightsEdit, 10, 1 );


	addTab( page1, tr("Documen&t") );
	addTab( page2, tr("Further &Information") );

	// switched off as it's called in main prefs classes - PV
	//restoreDefaults();

	//tooltips
	authorEdit->setToolTip( "<qt>" + tr( "The person or organisation primarily responsible for making the content of the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF" ) );
	titleEdit->setToolTip( "<qt>" + tr( "A name given to the document. This field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF" ) + "</qt>");
	descriptionEdit->setToolTip( "<qt>" + tr( "An account of the content of the document. This field is for a brief description or abstract of the document. It is embedded in the PDF on export" ) + "</qt>" );
	keywordsEdit->setToolTip( "<qt>" + tr( "The topic of the content of the document. This field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files" ) + "</qt>" );
	publisherEdit->setToolTip( "<qt>" + tr( "A person or organisation responsible for making the document available" ) + "</qt>" );
	contributorsEdit->setToolTip( "<qt>" + tr( "A person or organisation responsible for making contributions to the content of the document" ) + "</qt>" );
	dateEdit->setToolTip( "<qt>" + tr( "A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601" ) + "</qt>" );
	typeEdit->setToolTip( "<qt>" + tr( "The nature or genre of the content of the document, eg. categories, functions, genres, etc" )  + "</qt>");
	formatEdit->setToolTip( "<qt>" + tr( "The physical or digital manifestation of the document. Media type and dimensions would be worth noting. RFC2045,RFC2046 for MIME types are also useful here" ) + "</qt>" );
	identifierEdit->setToolTip( "<qt>" + tr( "An unambiguous reference to the document within a given context such as ISBN or URI" ) + "</qt>" );
	sourceEdit->setToolTip( "<qt>" + tr( "A reference to a document from which the present document is derived, eg. ISBN or URI" ) + "</qt>" );
	languageEdit->setToolTip( "<qt>" + tr( "The language in which the content of the document is written, usually a ISO-639 language code optionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH" ) + "</qt>" );
	relationEdit->setToolTip( "<qt>" + tr( "A reference to a related document, possibly using a formal identifier such as a ISBN or URI" ) + "</qt>" );
	coverageEdit->setToolTip( "<qt>" + tr( "The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges" ) + "</qt>" );
	rightsEdit->setToolTip( "<qt>" + tr( "Information about rights held in and over the document, eg. copyright, patent or trademark" ) + "</qt>" );
}


DocumentInformation DocInfos::getDocInfo()
{
	DocumentInformation docInfo;
	docInfo.setAuthor(authorEdit->text());
	docInfo.setComments(descriptionEdit->toPlainText());
	docInfo.setContrib(contributorsEdit->toPlainText());
	docInfo.setCover(coverageEdit->text());
	docInfo.setDate(dateEdit->text());
	docInfo.setFormat(formatEdit->text());
	docInfo.setIdent(identifierEdit->text());
	docInfo.setKeywords(keywordsEdit->toPlainText());
	docInfo.setLangInfo(languageEdit->text());
	docInfo.setPublisher(publisherEdit->text());
	docInfo.setRelation(relationEdit->text());
	docInfo.setRights(rightsEdit->text());
	docInfo.setSource(sourceEdit->text());
	docInfo.setTitle(titleEdit->text());
	docInfo.setType(typeEdit->text());
	return docInfo;
}

void DocInfos::restoreDefaults()
{
	titleEdit->setText(infos.getTitle());
	authorEdit->setText(infos.getAuthor());
	descriptionEdit->setPlainText(infos.getComments());
	keywordsEdit->setPlainText(infos.getKeywords());
	publisherEdit->setText(infos.getPublisher());
	dateEdit->setText(infos.getDate());
	typeEdit->setText(infos.getType());
	formatEdit->setText(infos.getFormat());
	identifierEdit->setText(infos.getIdent());
	sourceEdit->setText(infos.getSource());
	languageEdit->setText(infos.getLangInfo());
	relationEdit->setText(infos.getRelation());
	coverageEdit->setText(infos.getCover());
	rightsEdit->setText(infos.getRights());
	contributorsEdit->setPlainText(infos.getContrib());
}
