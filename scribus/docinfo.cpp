/****************************************************************************
** Form implementation generated from reading ui file 'Info.ui'
**
** Created: Mon Apr 30 10:14:47 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "docinfo.h"
#include "docinfo.moc"
extern QPixmap loadIcon(QString nam);

#include <qtooltip.h>
#include "scribusdoc.h"
/*
 *  Constructs a DocInfos which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
DocInfos::DocInfos( QWidget* parent, ScribusDoc* doc )
		: QTabDialog( parent, "i", true, 0 )
{
	setMaximumSize( QSize( 32767, 32767 ) );
	setCaption( tr( "Document Information" ) );
	setIcon(loadIcon("AppIcon.png"));

	page1 = new QWidget( this );
	layout1 = new QGridLayout(page1);
	layout1->setSpacing( 6 );
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

	keywordsEdit = new QMultiLineEdit( page1, "keywordsEdit" );
	keywordsLabel = new QLabel( keywordsEdit, tr("&Keywords:"), page1, "keywordsLabel" );
	keywordsLabel->setAlignment( static_cast<int>( QLabel::AlignTop | QLabel::AlignLeft ) );
	keywordsEdit->setMinimumSize(QSize(200, 105));
	layout1->addWidget( keywordsLabel, 2, 0 );
	layout1->addWidget( keywordsEdit, 2, 1 );

	descriptionEdit = new QMultiLineEdit( page1, "descriptionEdit" );
	descriptionLabel = new QLabel( descriptionEdit, tr("Descri&ption:"), page1, "descriptionLabel" );
	descriptionLabel->setAlignment( static_cast<int>( QLabel::AlignTop | QLabel::AlignLeft ) );
	descriptionEdit->setMinimumSize(QSize(200, 105));
	layout1->addWidget( descriptionLabel, 3, 0 );
	layout1->addWidget( descriptionEdit, 3, 1 );


	page2 = new QWidget( this );
	layout2 = new QGridLayout(page2);
	layout2->setSpacing( 6 );
	layout2->setMargin( 5 );

	publisherEdit = new QLineEdit( page2, "publisherEdit" );
	publisherLabel = new QLabel( publisherEdit, tr("P&ublisher:") , page2, "publisherLabel");
	publisherLabel->resize( publisherLabel->sizeHint() );
	layout2->addWidget( publisherLabel, 0, 0 );
	layout2->addWidget( publisherEdit, 0, 1 );

	contributorsEdit = new QMultiLineEdit( page2, "contributorsEdit" );
	contributorsLabel = new QLabel( contributorsEdit, tr("&Contributors:"), page2, "contributorsLabel" );
	contributorsLabel->setAlignment( static_cast<int>( QLabel::AlignTop | QLabel::AlignLeft ) );
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


	addTab( page1, tr("&Document") );
	addTab( page2, tr("Further &Information") );

	//set values
	titleEdit->setText(doc->documentInfo.DocTitel);
	authorEdit->setText(doc->documentInfo.DocAutor);
	descriptionEdit->setText(doc->documentInfo.DocComments);
	keywordsEdit->setText(doc->documentInfo.DocKeyWords);
	publisherEdit->setText(doc->documentInfo.DocPublisher);
	dateEdit->setText(doc->documentInfo.DocDate);
	typeEdit->setText(doc->documentInfo.DocType);
	formatEdit->setText(doc->documentInfo.DocFormat);
	identifierEdit->setText(doc->documentInfo.DocIdent);
	sourceEdit->setText(doc->documentInfo.DocSource);
	languageEdit->setText(doc->documentInfo.DocLangInfo);
	relationEdit->setText(doc->documentInfo.DocRelation);
	coverageEdit->setText(doc->documentInfo.DocCover);
	rightsEdit->setText(doc->documentInfo.DocRights);
	contributorsEdit->setText(doc->documentInfo.DocContrib);

	//tooltips
	QToolTip::add( authorEdit, tr( "The person or organisation primarily responsible for making the content of the document.\nThis field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF" ) );
	QToolTip::add( titleEdit, tr( "A name given to the document.\nThis field can be embedded in the Scribus document for reference, as well as in the metadata of a PDF" ) );
	QToolTip::add( descriptionEdit, tr( "An account of the content of the document.\nThis field is for a brief description or abstract of the document. It is embedded in the PDF on export" ) );
	QToolTip::add( keywordsEdit, tr( "The topic of the content of the document.\nThis field is for document keywords you wish to embed in a PDF, to assist searches and indexing of PDF files" ) );
	QToolTip::add( publisherEdit, tr( "A person or organisation responsible for making the document available" ) );
	QToolTip::add( contributorsEdit, tr( "A person or organisation responsible for making contributions to the content of the document" ) );
	QToolTip::add( dateEdit, tr( "A date associated with an event in the life cycle of the document, in YYYY-MM-DD format, as per ISO 8601" ) );
	QToolTip::add( typeEdit, tr( "The nature or genre of the content of the document, eg. categories, functions, genres, etc" ) );
	QToolTip::add( formatEdit, tr( "The physical or digital manifestation of the document. Media type and dimensions would be worth noting.\nRFC2045,RFC2046 for MIME types are also useful here" ) );
	QToolTip::add( identifierEdit, tr( "An unambiguous reference to the document within a given context such as ISBN or URI" ) );
	QToolTip::add( sourceEdit, tr( "A reference to a document from which the present document is derived, eg. ISBN or URI" ) );
	QToolTip::add( languageEdit, tr( "The language in which the content of the document is written, usually a ISO-639 language code\noptionally suffixed with a hypen and an ISO-3166 country code, eg. en-GB, fr-CH" ) );
	QToolTip::add( relationEdit, tr( "A reference to a related document, possibly using a formal identifier such as a ISBN or URI" ) );
	QToolTip::add( coverageEdit, tr( "The extent or scope of the content of the document, possibly including location, time and jurisdiction ranges" ) );
	QToolTip::add( rightsEdit, tr( "Information about rights held in and over the document, eg. copyright, patent or trademark" ) );

	setOKButton( tr("&OK") );
	setCancelButton( tr("&Cancel") );

	// signals and slots connections
	connect( this, SIGNAL( applyButtonPressed() ), this, SLOT( accept() ) );
	connect( this, SIGNAL( cancelButtonPressed() ), this, SLOT( reject() ) );
}
