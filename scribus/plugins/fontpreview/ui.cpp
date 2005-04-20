#include "ui.h"
#include "ui.moc"
#include "pluginmanager.h"
#include <qvariant.h>
#include <qpushbutton.h>
#include <qlistview.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qstring.h>
#include <qspinbox.h>

#include <prefsfile.h>

extern QPixmap fontSamples(QString da, int s, QString ts, QColor back);
extern QPixmap loadIcon(QString nam);
extern PrefsFile *prefsFile;

/*! Main window for "Font Preview" plugin. It's only gui.
 *
 *  Constructs a FontPreview as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
FontPreview::FontPreview(ScribusApp *carrier, QWidget* parent, const char* name, bool modal, WFlags fl)
	: QDialog(parent, name, modal, fl)
{
	this->carrier = carrier;
	if (!name)
		setName( "FontPreview" );
	setIcon(loadIcon("AppIcon.png"));
	// scribus config
	prefs = prefsFile->getPluginContext("fontpreview");
	sortColumn = prefs->getUInt("sortColumn", 0);
	xsize = prefs->getUInt("xsize", 640);
	ysize = prefs->getUInt("ysize", 480);

	FontPreviewLayout = new QGridLayout(this, 1, 1, 10, 5, "FontPreviewLayout");
	mainLayout = new QVBoxLayout( 0, 0, 5, "mainLayout");

	// searching
	searchLayout = new QHBoxLayout(0, 0, 5, "searchLayout");
	searchLabel = new QLabel(this, "searchLabel");
	searchLayout->addWidget(searchLabel);
	searchEdit = new QLineEdit(this, "searchEdit");
	searchLayout->addWidget(searchEdit);
	searchButton = new QPushButton(this, "searchButton");
	searchLayout->addWidget(searchButton);

	mainLayout->addLayout(searchLayout, 0);

	fontList = new QListView(this, "fontList" );
	fontList->setAllColumnsShowFocus(true);
	fontList->setShowSortIndicator(true);
	// columns
	fontList->addColumn(tr("Font Name", "font preview"));
	fontList->addColumn(tr("Doc", "font preview"));
	fontList->setColumnAlignment(1, Qt::AlignCenter);
	fontList->addColumn(tr("Type", "font preview"));
	fontList->addColumn(tr("Subset", "font preview"));
	fontList->setColumnAlignment(3, Qt::AlignCenter);
	fontList->addColumn(tr("Access", "font preview"));

	mainLayout->addWidget(fontList);

	fontPreview = new QLabel(this, "fontPreview");
	fontPreview->setMinimumSize(QSize(400, 90));
	fontPreview->setFrameShape(QFrame::Box);
	fontPreview->setPaletteBackgroundColor(white /* MrB RFE ;) paletteBackgroundColor()*/);
	mainLayout->addWidget(fontPreview);

	sizeLayout = new QHBoxLayout(0, 0, 5, "sizeLayout");
	sizeLabel = new QLabel(this, "sizeLabel");
	sizeLabel->setText(tr("Font Size:"));
	sizeLayout->addWidget(sizeLabel);
	sizeSpin = new QSpinBox(10, 72, 1, this, "sizeSpin");
	sizeSpin->setValue(prefs->getUInt("fontSize", 18));
	sizeLayout->addWidget(sizeSpin);

	buttonSpacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
	sizeLayout->addItem(buttonSpacer);

	okButton = new QPushButton(this, "okButton");
	sizeLayout->addWidget(okButton);
	cancelButton = new QPushButton(this, "cancelButton");
	sizeLayout->addWidget(cancelButton);

	mainLayout->addLayout(sizeLayout);

	FontPreviewLayout->addLayout(mainLayout, 0, 0);

	languageChange();
	resize(QSize(xsize, ysize).expandedTo(minimumSizeHint()));
	layout()->activate();
	clearWState(WState_Polished);

	/* go through available fonts and check their properties */
	reallyUsedFonts.clear();
	carrier->GetUsedFonts(&reallyUsedFonts);
	ttfFont = loadIcon("font_truetype16.png");
	otfFont = loadIcon("font_otf16.png");
	psFont = loadIcon("font_type1_16.png");
	okIcon = loadIcon("ok.png");

	updateFontList("");

	fontList->setSorting(sortColumn);

	// set initial listitem
	QListViewItem *item;
	if (carrier->pluginManager->dllInput != "")
		item = fontList->findItem(carrier->pluginManager->dllInput, 0);
	else
	{
		if (carrier->view->SelItem.count() != 0)
			item = fontList->findItem(carrier->doc->CurrFont, 0);
		else
			item = fontList->findItem(carrier->Prefs.toolSettings.defFont, 0);
	}
	if (item != 0)
	{
		fontList->setCurrentItem(item);
		fontList_changed();
	}

	// signals and slots connections
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(fontList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(fontList_changed()));
	connect(sizeSpin, SIGNAL(valueChanged(int)), this, SLOT(fontList_changed()));
	// searching
	connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButton_clicked()));
	connect(searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchEdit_textChanged(const QString &)));
}

/**
 * Writes configuration, destroys the object and frees any allocated resources.
 */
FontPreview::~FontPreview()
{
   prefs->set("sortColumn", fontList->sortColumn());
   prefs->set("xsize", width());
   prefs->set("ysize", height());
   prefs->set("fontSize", sizeSpin->value());
}

/**
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void FontPreview::languageChange()
{
	setCaption(tr("Fonts Preview", "font preview"));
	okButton->setText(tr("&OK", "font preview"));
	okButton->setAccel(QKeySequence(tr("Alt+O", "font preview")));
	searchLabel->setText(tr("Quick Search: "));
	searchButton->setText(tr("&Search"));
	cancelButton->setText(tr("&Close", "font preview"));
	cancelButton->setAccel(QKeySequence(tr("Alt+C", "font preview")));
	QToolTip::add(okButton, "<qt>" + tr("Append selected font into Style, Font menu", "font preview") + "</qt>");
	QToolTip::add(cancelButton,tr("Leave preview", "font preview"));
	QToolTip::add(searchEdit, "<qt>" + tr("Typing the text here provides quick searching in the font names. E.g. 'bold' shows all fonts with Bold in name. Searching is case insensitive.") + "</qt>");
	QToolTip::add(searchButton, tr("Start searching"));
	QToolTip::add(sizeSpin, tr("Size of the selected font"));
}

/**
 * Creates pixmap with font sample
 */
void FontPreview::fontList_changed()
{
	QString t = tr("Woven silk pyjamas exchanged for blue quartz", "font preview");
	t.replace('\n', " "); // remove French <NL> from translation...
	QListViewItem *item = fontList->currentItem();
	QString da = carrier->Prefs.AvailFonts[item->text(0)]->Datei;
	QPixmap pixmap = fontSamples(da, sizeSpin->value(), t, white /*paletteBackgroundColor()*/);
	fontPreview->clear();
	if (!pixmap.isNull())
		fontPreview->setPixmap(pixmap);
}

void FontPreview::updateFontList(QString searchStr)
{
	fontList->clear();
	QString sstr = searchStr.lower();
	for (SCFontsIterator fontIter(carrier->Prefs.AvailFonts); fontIter.current(); ++fontIter)
	{
		if (searchStr.length()!=0 && !fontIter.current()->SCName.lower().contains(sstr))
			continue;
		if (fontIter.current()->UseFont)
		{
			QListViewItem *row = new QListViewItem(fontList);
			QFileInfo fi = QFileInfo(fontIter.current()->Datei);
			QString ext = fi.extension(false).lower();

			row->setText(0, fontIter.current()->SCName);
			// searching
			if (reallyUsedFonts.contains(fontIter.current()->SCName))
				row->setPixmap(1, okIcon);

			if (ext == "otf")
			{
				row->setPixmap(2, otfFont);
				row->setText(2, "OpenType");
			}
			else
				if (fontIter.current()->Subset)
					row->setPixmap(3, okIcon);

			if ((ext == "pfa") || (ext == "pfb")) // type1
			{
				row->setPixmap(2, psFont);
				row->setText(2, "Type1");
			}

			if (ext == "ttf")
			{
				row->setPixmap(2, ttfFont);
				row->setText(2, "TrueType");
			}

			fi.absFilePath().contains(QDir::homeDirPath()) ?
					row->setText(4, tr("User", "font preview")):
					row->setText(4, tr("System", "font preview"));

			fontList->insertItem(row);
		}
	} // for fontIter
}

void FontPreview::searchEdit_textChanged(const QString &s)
{
	if (s.length()==0)
		updateFontList(s);
}

void FontPreview::searchButton_clicked()
{
	updateFontList(searchEdit->text());
}
